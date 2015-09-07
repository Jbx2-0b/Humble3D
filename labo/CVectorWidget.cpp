#include "CVectorWidget.h"

//-----------------------------------------------------------------------------------------
CVectorWidget::CVectorWidget(QWidget* parent /*= 0*/,CVectorWidget::EnumType eWidgetType /*= eVectorWidget4D*/)
    : QWidget(parent)
{
    setGeometry(QRect(0, 0, 250, 35));
    m_pHorizontalLayout = new QHBoxLayout(this);
    m_pHorizontalLayout->setSpacing(2);
    m_pHorizontalLayout->setContentsMargins(0, 0, 0, 0);

    m_pXLineEdit = new QLineEdit(this);
    m_pXLineEdit->setStyleSheet("background-color:rgba(255, 100, 100, 100);");
    m_pHorizontalLayout->addWidget(m_pXLineEdit);

    m_pYLineEdit = new QLineEdit(this);
    m_pYLineEdit->setStyleSheet("background-color:rgba(100, 255, 100, 100);");
    m_pHorizontalLayout->addWidget(m_pYLineEdit);

    m_pZLineEdit = new QLineEdit(this);
    m_pZLineEdit->setStyleSheet("background-color:rgba(100, 100, 255, 100);");
    m_pHorizontalLayout->addWidget(m_pZLineEdit);

    m_pWLineEdit = new QLineEdit(this);
    m_pHorizontalLayout->addWidget(m_pWLineEdit);

    setType(eWidgetType);

    m_pXValidator = new QDoubleValidator();
    //m_pXLineEdit->setValidator(m_pXValidator);

    m_pYValidator = new QDoubleValidator();
    //m_pYLineEdit->setValidator(m_pYValidator);

    m_pZValidator = new QDoubleValidator();
    //m_pZLineEdit->setValidator(m_pZValidator);

    m_pWValidator = new QDoubleValidator();
    //m_pWLineEdit->setValidator(m_pWValidator);

    connect(m_pXLineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
    connect(m_pYLineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
    connect(m_pZLineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
    connect(m_pWLineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
}

//-----------------------------------------------------------------------------------------
CVectorWidget::~CVectorWidget()
{
    delete m_pXValidator;
    delete m_pYValidator;
    delete m_pZValidator;
    delete m_pWValidator;
}

//-----------------------------------------------------------------------------------------
void CVectorWidget::setType(EnumType eWidgetType /*= eVectorWidget4D*/)
{
    if (eWidgetType == eVectorWidget2D)
    {
        m_pZLineEdit->setEnabled(false);
        m_pWLineEdit->setEnabled(false);
    }
    else if (eWidgetType == eVectorWidget3D)
    {
        m_pWLineEdit->setEnabled(false);
    }
    else // eVectorWidget4D
    {
        m_pZLineEdit->setEnabled(true);
        m_pWLineEdit->setEnabled(true);
    }
}

//-----------------------------------------------------------------------------------------
void CVectorWidget::setVector(const QVector2D& vector)
{
    m_pXLineEdit->setText(QString::number(vector.x()));
    m_pYLineEdit->setText(QString::number(vector.y()));
}

//-----------------------------------------------------------------------------------------
void CVectorWidget::setVector(const QVector3D& vector)
{
    m_pXLineEdit->setText(QString::number(vector.x()));
    m_pYLineEdit->setText(QString::number(vector.y()));
    m_pZLineEdit->setText(QString::number(vector.z()));
}

//-----------------------------------------------------------------------------------------
void CVectorWidget::setVector(const QVector4D& vector)
{
    m_pXLineEdit->setText(QString::number(vector.x()));
    m_pYLineEdit->setText(QString::number(vector.y()));
    m_pZLineEdit->setText(QString::number(vector.z()));
    m_pWLineEdit->setText(QString::number(vector.w()));
}

//-----------------------------------------------------------------------------------------
QVector2D CVectorWidget::getVector2D() const
{
    return QVector2D(m_pXLineEdit->text().toDouble(), m_pYLineEdit->text().toDouble());
}

//-----------------------------------------------------------------------------------------
QVector3D CVectorWidget::getVector3D() const
{
    return QVector3D(m_pXLineEdit->text().toDouble(), m_pYLineEdit->text().toDouble(), m_pZLineEdit->text().toDouble());
}

//-----------------------------------------------------------------------------------------
QVector4D CVectorWidget::getVector4D() const
{
    return QVector4D(m_pXLineEdit->text().toDouble(), m_pYLineEdit->text().toDouble(), m_pZLineEdit->text().toDouble(), m_pWLineEdit->text().toDouble());
}
