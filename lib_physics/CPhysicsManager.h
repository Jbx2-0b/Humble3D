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

    //! Définit la gravité
    void setGravity(const QVector3D& gravity);

    //! Démarre le moteur physique
    void start();

    //! Arrête le moteur physique
    void stop();

    //! Définit le temps entre deux itérations
    void setTimeStep(real dTime);

protected slots:

    void onTimeout();

protected:  

    //! Implémentation de ASceneNodeItem::ISceneNodeItemListener
    virtual void onUpdate(ASceneNodeItem* pNodeItem);

    //! Implémentation de ASceneNodeItem::ISceneNodeItemListener
    virtual void onDelete(ASceneNodeItem* pNodeItem);

    void removeShapeInfo(const QPair<ASceneNodeItem*, CSceneNode*>& key);

    // Monde physique
    btDiscreteDynamicsWorld* m_pWorld;

    // Interface pour détecter les objets où leurs AABB se chevauchent.
    btBroadphaseInterface* m_pBroadphase;

    // btCollisionDispatcher supporte des algorithmes qui peuvent gérer des pairs de collisions ConvexConvex et ConvexConcave. Temps de l'impact, le point le plus proche et pénétration de profondeur.
    btCollisionDispatcher* m_pDispatcher;

    // btCollisionConfiguration permet de configurer les allocataires de mémoire.
    btDefaultCollisionConfiguration* m_pCollisionConfiguration;

    // btSequentialImpulseConstraintSolver est une implémentation SIMD rapide de la méthode Projected Gauss Seidel (iterative LCP).
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
