#include "CSceneNodeAnimation.h"
#include <QDebug>

//-----------------------------------------------------------------------------------------
real CSceneNodeAnimation::getAnimationTime() const
{
    if (m_PositionKeys.isEmpty() && m_RotationKeys.isEmpty() && m_ScalingKeys.isEmpty())
        return 0.;

    real dPosTime = m_PositionKeys.isEmpty()    ? 0. : m_PositionKeys.last().getTime();
    real dRotTime = m_RotationKeys.isEmpty()    ? 0. : m_RotationKeys.last().getTime();
    real dScaTime = m_ScalingKeys.isEmpty()     ? 0. : m_ScalingKeys.last().getTime();

    return qMax(dPosTime, qMax(dRotTime, dScaTime));
}

//-----------------------------------------------------------------------------------------
QPair<CVectorKey, CVectorKey> CSceneNodeAnimation::getPositionKeys(real dT) const
{
    QPair<CVectorKey, CVectorKey> result(CVectorKey(0, QVector3D()), CVectorKey(0, QVector3D()));

    if (!m_PositionKeys.isEmpty())
    {
        int iKey = 0;

        result.first = result.second = m_PositionKeys.first();

        for (iKey = 1; iKey < m_PositionKeys.count(); ++iKey)
        {
            if (m_PositionKeys[iKey - 1].getTime() <= dT && dT <= m_PositionKeys[iKey].getTime())
            {
                result.first = m_PositionKeys[iKey - 1];
                result.second = m_PositionKeys[iKey];
                return result;
            }
        }
    }

    return result;
}

//-----------------------------------------------------------------------------------------
QPair<CQuaternionKey, CQuaternionKey> CSceneNodeAnimation::getRotationKeys(real dT) const
{
    QPair<CQuaternionKey, CQuaternionKey> result;

    if (!m_RotationKeys.isEmpty())
    {
        int iKey = 0;

        result.first = result.second = m_RotationKeys.first();

        for (iKey = 1; iKey < m_RotationKeys.count(); ++iKey)
        {
            if (m_RotationKeys[iKey - 1].getTime() <= dT && dT <= m_RotationKeys[iKey].getTime())
            {
                result.first = m_RotationKeys[iKey - 1];
                result.second = m_RotationKeys[iKey];
                return result;
            }
        }
    }

    return result;
}

//-----------------------------------------------------------------------------------------
QPair<CVectorKey, CVectorKey> CSceneNodeAnimation::getScalingKeys(real dT) const
{
    QPair<CVectorKey, CVectorKey> result(CVectorKey(0, QVector3D(1., 1., 1.)), CVectorKey(0, QVector3D(1., 1., 1.)));

    if (!m_ScalingKeys.isEmpty())
    {
        int iKey = 0;

        result.first = result.second = m_ScalingKeys.first();

        for (iKey = 1; iKey < m_ScalingKeys.count(); ++iKey)
        {
            if (m_ScalingKeys[iKey - 1].getTime() <= dT && dT <= m_ScalingKeys[iKey].getTime())
            {
                result.first = m_ScalingKeys[iKey - 1];
                result.second = m_ScalingKeys[iKey];
                return result;
            }
        }
    }
    return result;
}
