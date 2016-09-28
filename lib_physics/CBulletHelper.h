#ifndef CBULLETHELPER_H
#define CBULLETHELPER_H

// Qt
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

// Bullet
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"


class CBulletHelper
{
public:

    inline static btVector3 toBulletVector3D(const QVector3D& v)
    {
        return btVector3(v.x(), v.y(), v.z());
    }

    inline static QVector3D toQVector3D(const btVector3& v)
    {
        return QVector3D(v.getX(), v.getY(), v.getZ());
    }

    inline static btTransform toBulletTransform(const QMatrix4x4& mat, qreal scale = 1.0)
    {
        btTransform transform;

        btScalar m[16];

        for (int i = 0; i < 16; ++i)
        {
            m[i] = mat.constData()[i];
        }

        transform.setFromOpenGLMatrix(m);
        transform.getOrigin() *= scale;
        return transform;

    }

    inline static QMatrix4x4 toQMatrix4x4(const btTransform& transform, qreal scale = 1.0)
    {
        float data[16];
        transform.getOpenGLMatrix((btScalar*)data);
        data[12] *= scale;
        data[13] *= scale;
        data[14] *= scale;
        return QMatrix4x4(data[0],  data[1],  data[2],  data[3],
                data[4],  data[5],  data[6],  data[7],
                data[8],  data[9],  data[10], data[11],
                data[12], data[13], data[14], data[15]).transposed();
    }

    inline static QQuaternion toQQuaternion(const btQuaternion& value)
    {
        return QQuaternion(value.getW(), value.getX(), value.getY(), value.getZ());
    }

    btQuaternion toBulletQuaternion(const QQuaternion& value)
    {
        return btQuaternion(value.x(), value.y(), value.z(), value.scalar());
    }

};

#endif // CBULLETHELPER_H
