#include "CEditShaderTextItem.h"

// Qt
#include <QTextStream>
#include <QVBoxLayout>
#include <QTextEdit>



//-----------------------------------------------------------------------
CEditShaderTextItem::CEditShaderTextItem(QGraphicsScene* pScene)
    : CGraphicsWidgetItem(pScene)
    , m_pCurrentShader(0)
    , m_eCurrentType(eVertexShader)
{
    m_pCentralFrame = new QFrame();
    m_pCentralFrame->setAutoFillBackground(false);

    m_pCentralLayout = new QVBoxLayout();
    m_pCentralFrame->setLayout(m_pCentralLayout);

    m_pEdit = new CCodeEditor();
    m_pHighlighter = new GLSLSyntaxHighlighter(m_pEdit->document());

    QFont font("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    m_pEdit->setFont(font);

    const int tabStop = 4;  // 4 characters
    QFontMetrics metrics(font);
    m_pEdit->setTabStopWidth(tabStop * metrics.width(' '));

    m_pCentralLayout->addWidget(m_pEdit);

    m_pConsole = new QTextEdit();
    m_pConsole->setReadOnly(false);
    m_pConsole->setFixedHeight(100);
    m_pCentralLayout->addWidget(m_pConsole);

    setWidget(m_pCentralFrame);
    setZValue(1e30);

    m_UpdateShaderTimer.setSingleShot(true);

    connect(m_pEdit,				SIGNAL(textChanged()),	this, SLOT(onTextChanged()));
    connect(&m_UpdateShaderTimer,	SIGNAL(timeout()),		this, SLOT(onUpdateTimerTimeout()));
}

//-----------------------------------------------------------------------
CEditShaderTextItem::~CEditShaderTextItem()
{
    if (m_pCurrentShader)
    {
        m_pCurrentShader->unregisterListener(this);
    }
}

//-----------------------------------------------------------------------
void CEditShaderTextItem::setStyleSheet(const QString& styleSheet)
{
    m_pCentralFrame->setStyleSheet(styleSheet);
}

//-----------------------------------------------------------------------
void CEditShaderTextItem::setShaderType(EnumShaderType eType)
{
    m_eCurrentType = eType;

    if (m_pCurrentShader)
    {
        QString code;

        switch (eType)
        {
        case eVertexShader:
            code = m_pCurrentShader->getVertexShaderCode();
            break;
        case eGeometryShader:
            code = m_pCurrentShader->getGeometryShaderCode();
            break;
        case eTessellationControlShader:
            code = m_pCurrentShader->getTessellationControlShaderCode();
            break;
        case eTessellationEvaluationShader:
            code = m_pCurrentShader->getTessellationEvaluationShaderCode();
            break;
        case eFragmentShader:
            code = m_pCurrentShader->getFragmentShaderCode();
            break;
        case eComputeShader:
            code = m_pCurrentShader->getComputeShaderCode();
            break;
        }

        m_pEdit->setPlainText(code);
    }
}

//-----------------------------------------------------------------------
void CEditShaderTextItem::onUpdate(CShader* /*pShader*/)
{
    // nothing to do
}

//-----------------------------------------------------------------------
void CEditShaderTextItem::onDelete(CShader* pShader)
{
    if (m_pCurrentShader == pShader)
    {
        m_pCurrentShader->unregisterListener(this);
        m_pCurrentShader = 0;
    }
}

//-----------------------------------------------------------------------
//void CEditShaderTextItem::onNewCompileShadersMessage(QGLGlobal::EnumTypeMessage eType, const QString& message)
//{
//	/*switch (eType)
//	{
//	case QGLGlobal::eERROR:
//		m_pConsole->setTextColor(Qt::red);
//		break;
//	case QGLGlobal::eINFO:
//		m_pConsole->setTextColor(Qt::green);
//		break;
//	};*/
//
//	m_pConsole->setPlainText(message);
//}

//-----------------------------------------------------------------------
void CEditShaderTextItem::onEditShader(CShader* pShader)
{
    if (m_pCurrentShader != pShader)
    {
        if (m_pCurrentShader) m_pCurrentShader->unregisterListener(this);
        m_pCurrentShader = pShader;

        m_pCurrentShader->registerListener(this);

        m_eCurrentType = eVertexShader;
        setShaderType(eVertexShader);
    }
}

//-----------------------------------------------------------------------
void CEditShaderTextItem::onSaveShader()
{
    if (m_pCurrentShader)
    {
        QString sText, sFileName;

        switch (m_eCurrentType)
        {
        case eVertexShader:
            sText = m_pCurrentShader->getVertexShaderCode();
            sFileName = m_pCurrentShader->getVertexShaderFileName();
            break;
        case eGeometryShader:
            sText = m_pCurrentShader->getGeometryShaderCode();
            sFileName = m_pCurrentShader->getGeometryShaderFileName();
            break;
        case eTessellationControlShader:
            // TODO
            break;
        case eTessellationEvaluationShader:
            // TODO
            break;
        case eFragmentShader:
            sText = m_pCurrentShader->getFragmentShaderCode();
            sFileName = m_pCurrentShader->getFragmentShaderFileName();
            break;
        case eComputeShader:
            // TODO
            break;
        }

        QFile file(sFileName);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << sText;

            file.close();
        }
    }
}

//-----------------------------------------------------------------------
void CEditShaderTextItem::onTextChanged()
{
    m_UpdateShaderTimer.start(iUpdateShaderTime);
}

//-----------------------------------------------------------------------
void CEditShaderTextItem::onUpdateTimerTimeout()
{
    if (m_pCurrentShader)
    {
        switch (m_eCurrentType)
        {
        case eVertexShader:
            m_pCurrentShader->setVertexShaderCode(m_pEdit->toPlainText());
            break;
        case eGeometryShader:
            m_pCurrentShader->setGeometryShaderCode(m_pEdit->toPlainText());
            break;
        case eTessellationControlShader:
            // TODO
            break;
        case eTessellationEvaluationShader:
            // TODO
            break;
        case eFragmentShader:
            m_pCurrentShader->setFragmentShaderCode(m_pEdit->toPlainText());
            break;
        case eComputeShader:
            // TODO
            break;
        }
    }
}

//-----------------------------------------------------------------------
// Highlighter
//-----------------------------------------------------------------------
GLSLSyntaxHighlighter::GLSLSyntaxHighlighter(QTextDocument* document)
    : QSyntaxHighlighter(document)
{
    m_Variables  << "bool"
                 << "float"
                 << "double"
                 << "int"
                 << "uint"
                 << "bvec2"
                 << "bvec3"
                 << "bvec4"
                 << "ivec2"
                 << "ivec3"
                 << "ivec4"
                 << "uvec2"
                 << "uvec3"
                 << "uvec4"
                 << "vec2"
                 << "vec3"
                 << "vec4"
                 << "mat2"
                 << "mat3"
                 << "mat4"
                 << "centroid"
                 << "patch"
                 << "sample"
                 << "dvec2"
                 << "dvec3"
                 << "dvec4"
                 << "dmat2"
                 << "dmat3"
                 << "dmat4"
                 << "mat2x2"
                 << "mat2x3"
                 << "mat2x4"
                 << "mat3x2"
                 << "mat3x3"
                 << "mat3x4"
                 << "mat4x2"
                 << "mat4x3"
                 << "mat4x4"
                 << "dmat2x2"
                 << "dmat2x3"
                 << "dmat2x4"
                 << "dmat3x2"
                 << "dmat3x3"
                 << "dmat3x4"
                 << "dmat4x2"
                 << "dmat4x3"
                 << "dmat4x4"
                 << "sampler1d"
                 << "sampler2d"
                 << "sampler3d"
                 << "samplercube"
                 << "sampler1dshadow"
                 << "sampler2dshadow"
                 << "samplercubeshadow"
                 << "sampler1darray"
                 << "sampler2darray"
                 << "sampler1darrayshadow"
                 << "sampler2darrayshadow"
                 << "isampler1d"
                 << "isampler2d"
                 << "isampler3d"
                 << "isamplercube"
                 << "isampler1darray"
                 << "isampler2darray"
                 << "usampler1d"
                 << "usampler2d"
                 << "usampler3d"
                 << "usamplercube"
                 << "usampler1darray"
                 << "usampler2darray"
                 << "sampler2drect"
                 << "sampler2drectshadow"
                 << "isampler2drect"
                 << "usampler2drect"
                 << "samplerbuffer"
                 << "isamplerbuffer"
                 << "usamplerbuffer"
                 << "samplercubearray"
                 << "samplercubearrayshadow"
                 << "isamplercubearray"
                 << "usamplercubearray"
                 << "sampler2dms"
                 << "isampler2dms"
                 << "usampler2dms"
                 << "sampler2dmsarray"
                 << "isampler2dmsarray"
                 << "usampler2dmsarray"
                 << "void";

    m_Keywords   << "const"
                 << "break"
                 << "continue"
                 << "do"
                 << "else"
                 << "for"
                 << "if"
                 << "discard"
                 << "return"
                 << "switch"
                 << "case"
                 << "default"
                 << "subroutine"
                 << "in"
                 << "out"
                 << "inout"
                 << "uniform"
                 << "varying"
                 << "attribute"
                 << "noperspective"
                 << "flat"
                 << "smooth"
                 << "layout"
                 << "struct"
                 << "while";

    m_KeywordFormat.setFontWeight(QFont::Bold);
    m_KeywordFormat.setForeground(Qt::red);

    m_VariableFormat = QTextCharFormat(m_KeywordFormat);
    m_VariableFormat.setFontWeight(QFont::Bold);
    m_VariableFormat.setForeground(Qt::blue);

    m_VersionFormat = QTextCharFormat(m_KeywordFormat);
    m_VersionFormat.setFontWeight (QFont::Normal);
    m_VersionFormat.setForeground(Qt::white);

    m_CommentFormat = QTextCharFormat(m_KeywordFormat);
    m_CommentFormat.setFontItalic(true);
    m_CommentFormat.setForeground(Qt::green);
}

//-----------------------------------------------------------------------
void GLSLSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (QString variable : m_Variables)
    {
        QString pattern = "\\b" + variable + "\\b";

        QRegExp expression(pattern);
        int index = text.indexOf(expression);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, m_VariableFormat);
            index = text.indexOf(expression, index + length);
        }

    }

    for (QString keyword : m_Keywords)
    {
        QString pattern = "\\b" + keyword +"\\b";

        QRegExp expression(pattern);
        int index = text.indexOf(expression);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, m_KeywordFormat);
            index = text.indexOf(expression, index + length);
        }
    }

    QString pattern = "\\/\\/";

    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    if (index >= 0)
    {
        int length = text.size() - index;
        setFormat(index, length, m_CommentFormat);
    }

    pattern = QRegExp::escape("#");
    expression = QRegExp(pattern);
    index = text.indexOf(expression);
    if (index >= 0)
    {
        int length = text.size() - index;
        setFormat(index, length, m_VersionFormat);
    }
}

