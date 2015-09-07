#ifndef CPHYSICSMANAGER_H
#define CPHYSICSMANAGER_H

// Lib
#include "lib_physics_global.h"

// Math
#include "CMath.h"

// Core
#include "ASceneNodeItem.h"

// Qt
#include <QObject>
#include <QHash>
#include <QTimer>

// Bullet physics
#include "btBulletDynamicsCommon.h"


// Foundations
class CSceneNode;
class ASceneNodeItem;

static const btVector3 DefaultGravity(0.f, -9.8f, 0.f);
static const real dDefaultTimeStep = 1000. / 60.;   // 120/s

class LIB_PHYSICSSHARED_EXPORT CPhysicsManager : public QObject, ASceneNodeItem::ISceneNodeItemListener
{
    Q_OBJECT

public:

    //! Ctor
    CPhysicsManager();

    //! Dtor
    virtual ~CPhysicsManager();

    //! Enregistre un item qui participiera a la simulation physique
    void registerItem(ASceneNodeItem* pItem);

    //! Enregistre un item qui participiera a la simulation physique
    void unregisterItem(ASceneNodeItem* pItem);

    //! D�finit la gravit�
    void setGravity(const QVector3D& gravity);

    //! D�marre le moteur physique
    void start();

    //! Arr�te le moteur physique
    void stop();

    //! D�finit le temps entre deux it�rations
    void setTimeStep(real dTime);

protected slots:

    void onTimeout();

protected:  

    //! Impl�mentation de ASceneNodeItem::ISceneNodeItemListener
    virtual void onUpdate(ASceneNodeItem* pNodeItem);

    //! Impl�mentation de ASceneNodeItem::ISceneNodeItemListener
    virtual void onDelete(ASceneNodeItem* pNodeItem);

    void removeShapeInfo(const QPair<ASceneNodeItem*, CSceneNode*>& key);

    // Monde physique
    btDiscreteDynamicsWorld* m_pWorld;

    // Interface pour d�tecter les objets o� leurs AABB se chevauchent.
    btBroadphaseInterface* m_pBroadphase;

    // btCollisionDispatcher supporte des algorithmes qui peuvent g�rer des pairs de collisions ConvexConvex et ConvexConcave. Temps de l'impact, le point le plus proche et p�n�tration de profondeur.
    btCollisionDispatcher* m_pDispatcher;

    // btCollisionConfiguration permet de configurer les allocataires de m�moire.
    btDefaultCollisionConfiguration* m_pCollisionConfiguration;

    // btSequentialImpulseConstraintSolver est une impl�mentation SIMD rapide de la m�thode Projected Gauss Seidel (iterative LCP).
    btSequentialImpulseConstraintSolver* m_pSequentialImpulseConstraintSolver;

    class CShapeInfo
    {
    public:
        CShapeInfo();
        CShapeInfo(btDiscreteDynamicsWorld* pWorld, CSceneNode* pNode, ASceneNodeItem* pItem);
        ~CShapeInfo();

        CSceneNode* getNode() { return m_pNode; }
        const btTransform& getWorldTransformation() const { return m_pMotionState->m_graphicsWorldTrans; }

        void updateBodyParameters(ASceneNodeItem* pItem);

    private:
        CSceneNode* m_pNode;
        ASceneNodeItem* m_pItem;
        btDiscreteDynamicsWorld* m_pWorld;
        btCollisionShape* m_pShape;
        btVector3 m_LocalScaling;
        btDefaultMotionState* m_pMotionState;
        btRigidBody* m_pBody;
    };

    real m_dTimeStep;
    QTime m_TimeElaspedSinceLastStep;
    QSet<ASceneNodeItem*> m_Items;
    QHash<QPair<ASceneNodeItem*, CSceneNode*>, CShapeInfo*> m_ShapeInfos;
    QTimer m_Timer;
};

#endif // CPHYSICSMANAGER_H
