#ifndef CQMLWIDGET3D_H
#define CQMLWIDGET3D_H

// lib
#include "lib_qt_ui_global.h"

// Qt
#include <QQuickFramebufferObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QSGTransformNode>


class LIB_QT_UI_SHARED_EXPORT CQmlWidget3D : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QVector4D backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

    Q_PROPERTY(QVector3D eyePosition READ eyePosition WRITE setEyePosition NOTIFY eyePositionChanged)
    Q_PROPERTY(QVector3D center READ center WRITE setCenter NOTIFY centerChanged)
public:
    CQmlWidget3D();

    Renderer *createRenderer() const;

    bool enabled() const;
    void setEnabled(bool enabled);

    QVector4D backgroundColor() const;
    void setBackgroundColor(const QVector4D& color);

    QVector3D eyePosition() const;
    void setEyePosition(const QVector3D& position);

    QVector3D center() const;
    void setCenter(const QVector3D& center);

    virtual QSGNode *updatePaintNode(QSGNode* node, UpdatePaintNodeData* data) override
    {
        QMatrix4x4 flipY;
        flipY.translate(width() * 0.5, height() * 0.5);
        flipY.scale(1.0, -1.0);
        flipY.translate(-width() * 0.5, -height() * 0.5);
        flipY.translate(x(), y());
        data->transformNode->setMatrix(flipY);
        return QQuickFramebufferObject::updatePaintNode(node, data);
    }


signals:
    void enabledChanged();
    void backgroundColorChanged();
    void eyePositionChanged();
    void centerChanged();

private:
    bool m_Enabled;
    QVector4D m_BackgroundColor;
    QVector3D m_EyePosition;
    QVector3D m_Center;
};

#endif // CQMLWIDGET3D_H
