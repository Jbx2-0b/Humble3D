#ifndef CEDITSHADERTEXTITEM_H
#define CEDITSHADERTEXTITEM_H


// Foundations
#include "CGraphicsWidgetItem.h"
#include "CShader.h"
#include "CGeometryGlobal.h"
#include "CCodeEditor.h"


// Qt
#include <QGraphicsScene>
#include <QList>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTimer>



static const int iUpdateShaderTime = 1000;

class QVBoxLayout;
class QTextEdit;
class GLSLSyntaxHighlighter;

class CEditShaderTextItem : public CGraphicsWidgetItem
        , public CShader::IShaderListener
{
    Q_OBJECT

public:

    //! Constructeur
    CEditShaderTextItem(QGraphicsScene* pScene);

    //! Destructeur
    virtual ~CEditShaderTextItem();

    //! Definit la feuille de style CSS
    void setStyleSheet(const QString& styleSheet);

    //! Retourne le texte
    QString getText() const { return m_pEdit->toPlainText(); }

    //! Definit le type de shader en cours d'edition
    void setShaderType(EnumShaderType eType);

    //! Met a jour le texte
    void updateText();

    //! Définit un nouveau message
    void setMessage(bool bIsError, const QString& message);

protected:

    //! Implementation de CShader::IListener
    virtual void onUpdate(CShader* pShader);

    //! Implementation de CShader::IListener
    virtual void onDelete(CShader* pShader);

public slots:

    void onEditShader(CShader* pShader);

    void onSaveShader();

private slots:

    void onTextChanged();
    void onUpdateTimerTimeout();

private:

    QFrame* m_pCentralFrame;

    QVBoxLayout* m_pCentralLayout;

    CCodeEditor* m_pEdit;

    QTextEdit* m_pConsole;

    GLSLSyntaxHighlighter* m_pHighlighter;

    CShader* m_pCurrentShader;

    EnumShaderType m_eCurrentType;

    QTimer m_UpdateShaderTimer;
};



//-----------------------------------------------------------------------
class GLSLSyntaxHighlighter  : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    GLSLSyntaxHighlighter(QTextDocument* document);

    virtual void highlightBlock(const QString & text);

private:
    QList<QString>  m_Keywords;
    QList<QString>  m_Variables;

    QTextCharFormat m_KeywordFormat;
    QTextCharFormat m_VariableFormat;
    QTextCharFormat m_CommentFormat;
    QTextCharFormat m_VersionFormat;
};

#endif // CEDITSHADERTEXTITEM_H
