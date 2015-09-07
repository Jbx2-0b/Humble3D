#ifndef QVECTORWIDGET_H
#define QVECTORWIDGET_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QDoubleValidator>
#include <QDebug>

//-----------------------------------------------------------------------------------------
class CVectorWidget : public QWidget
{
    Q_OBJECT

public:

    enum EnumType
    {
        eVectorWidget2D,
        eVectorWidget3D,
        eVectorWidget4D
    };

    CVectorWidget(QWidget* parent = 0, EnumType eWidgetType = eVectorWidget4D);

    virtual ~CVectorWidget();

    void setType(EnumType eWidgetType = eVectorWidget4D);

    void setVector(const QVector2D& vector);
    void setVector(const QVector3D& vector);
    void setVector(const QVector4D& vector);

    QVector2D getVector2D() const;
    QVector3D getVector3D() const;
    QVector4D getVector4D() const;

    virtual void keyPressEvent(QKeyEvent* /*pEvent*/)
    {
        emit editingFinished();
    }

signals:

    void editingFinished();

private:

    QHBoxLayout* m_pHorizontalLayout;
    QLineEdit* m_pXLineEdit;
    QLineEdit* m_pYLineEdit;
    QLineEdit* m_pZLineEdit;
    QLineEdit* m_pWLineEdit;
    QDoubleValidator* m_pXValidator;
    QDoubleValidator* m_pYValidator;
    QDoubleValidator* m_pZValidator;
    QDoubleValidator* m_pWValidator;
};


#endif // QVECTORWIDGET_H
