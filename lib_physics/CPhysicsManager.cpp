#include "CPhysicsManager.h"
#include "CSceneNode.h"
#include "CBulletHelper.h"

#include <QDebug>

// Maths
#include "CBox3D.h"
#include "CSphere.h"

// Core
#include "CMeshInstance.h"
#include "CDebugManager.h"

// Bullet
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"

//-----------------------------------------------------------------------
CPhysicsManager::CPhysicsManager()
: m_dTimeStep(dDefaultTimeStep)
{
    ///collision configuration contains default setup for memory, collision setup
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);

    m_pBroadphase = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_pSequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;

    // initialisation du monde bullet
    m_pWorld = new btDiscreteDynamicsWorld(
                m_pDispatcher,
                m_pBroadphase,
                m_pSequentialImpulseConstraintSolver,
                m_pCollisionConfiguration);

    // On definit la gravite, de faeon e ce que les objets tombent vers le bas (-Y).
    m_pWorld->setGravity(DefaultGravity);

    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

//-----------------------------------------------------------------------
void CPhysicsManager::start()
{
    m_Timer.start(m_dTimeStep);
    m_TimeElaspedSinceLastStep.start();
}

//-----------------------------------------------------------------------
void CPhysicsManager::stop()
{
    m_Timer.stop();           
}

//-----------------------------------------------------------------------
void CPhysicsManager::setTimeStep(real dTimeStep)
{
    m_dTimeStep = dTimeStep;

    if (m_Timer.isActive())
    {
        m_Timer.start(dTimeStep);
    }
}

//-----------------------------------------------------------------------
CPhysicsManager::~CPhysicsManager()
{
    foreach (ASceneNodeItem* pItem, m_Items)
    {
        unregisterItem(pItem);
    }

    delete m_pWorld;
    delete m_pSequentialImpulseConstraintSolver;
    delete m_pBroadphase;
    delete m_pDispatcher;
    delete m_pCollisionConfiguration;
}

//-----------------------------------------------------------------------
void CPhysicsManager::setGravity(const QVector3D& gravity)
{
    m_pWorld->setGravity(CBulletHelper::toBulletVector3D(gravity));
}

//-----------------------------------------------------------------------
void CPhysicsManager::registerItem(ASceneNodeItem* pItem)
{
    pLog->addMessage(eINFO, QString("PhysicsManager: register item %1").arg(pItem->getName()));
    m_Items.insert(pItem);
}

//-----------------------------------------------------------------------
void CPhysicsManager::unregisterItem(ASceneNodeItem* pItem)
{
    pLog->addMessage(eINFO, QString("PhysicsManager: unregister item %1").arg(pItem->getName()));
    m_Items.remove(pItem);
    pItem->unregisterListener(this);

    foreach (CSceneNode* pSceneNode, pItem->getNodes())
    {
        QPair<ASceneNodeItem*, CSceneNode*> key(pItem, pSceneNode);
        removeShapeInfo(key);
    }
}

//-----------------------------------------------------------------------
void CPhysicsManager::onTimeout()
{
    m_pWorld->stepSimulation(m_TimeElaspedSinceLastStep.restart());

    foreach (ASceneNodeItem* pItem, m_Items)
    {
        foreach (CSceneNode* pSceneNode, pItem->getNodes())
        {
            if (!pSceneNode->isLocked())
            {
                QPair<ASceneNodeItem*, CSceneNode*> key(pItem, pSceneNode);
                if (!m_ShapeInfos.contains(key))
                {
                    m_ShapeInfos[key] = new CShapeInfo(m_pWorld, pSceneNode, pItem);
                    pItem->registerListener(this);
                }
                CShapeInfo* pShapeInfo = m_ShapeInfos.value(key);

                // Bullet suppose our bounding box is centered on zero. It's not always true (in particulary with assimp models).
                // We need to fix that with a local translation matrix
                QMatrix4x4 localTranslation;
                localTranslation.translate(-pSceneNode->getLocalAxisAlignedBoundingBox(false).getCenter());

                QMatrix4x4 globalInvTransfMatrix;

                if (CSceneNode* pParent = pSceneNode->getParent())
                {
                    globalInvTransfMatrix = pParent->getGlobalInverseTransformation();
                }

                QMatrix4x4 scaleMatrix;
                scaleMatrix.scale(pSceneNode->getLocalScaling());

                QMatrix4x4 transfMatrix =
                        globalInvTransfMatrix * CBulletHelper::toQMatrix4x4(pShapeInfo->getWorldTransformation());
                pSceneNode->setLocalTransformation(transfMatrix * scaleMatrix * localTranslation);
            }
        }
    }
}

//-----------------------------------------------------------------------
void CPhysicsManager::onUpdate(ASceneNodeItem* pNodeItem)
{
    foreach (CSceneNode* pSceneNode, pNodeItem->getNodes())
    {
        QPair<ASceneNodeItem*, CSceneNode*> key(pNodeItem, pSceneNode);

        if (m_ShapeInfos.contains(key))
        {
            CShapeInfo* pShapeInfo = m_ShapeInfos.value(key);
            pShapeInfo->updateBodyParameters(pNodeItem);
        }
    }
}

//-----------------------------------------------------------------------
void CPhysicsManager::onDelete(ASceneNodeItem* pItem)
{
    pItem->unregisterListener(this);
    foreach (CSceneNode* pSceneNode, pItem->getNodes())
    {
        QPair<ASceneNodeItem*, CSceneNode*> key(pItem, pSceneNode);
        removeShapeInfo(key);
    }
}

//-----------------------------------------------------------------------
void CPhysicsManager::removeShapeInfo(const QPair<ASceneNodeItem*, CSceneNode*>& key)
{
    if (m_ShapeInfos.contains(key))
    {
        CShapeInfo* pShapeInfo = m_ShapeInfos.value(key);
        m_ShapeInfos.remove(key);
        delete pShapeInfo;
    }
}

//-----------------------------------------------------------------------
CPhysicsManager::CShapeInfo::CShapeInfo()
    : m_pNode(0)
    , m_pWorld(0)
    , m_pShape(0)
    , m_pMotionState(0)
    , m_pBody(0)
{
}

//-----------------------------------------------------------------------
CPhysicsManager::CShapeInfo::CShapeInfo(btDiscreteDynamicsWorld* pWorld, CSceneNode* pNode, ASceneNodeItem* pItem)
    : m_pNode(pNode)
    , m_pItem(pItem)
    , m_pWorld(pWorld)
{
    switch (pItem->getPhysicShape())
    {
        case eBoxShape:
        {
            btVector3 halfSize = CBulletHelper::toBulletVector3D(m_pNode->getLocalAxisAlignedBoundingBox(false).getHalfSize());
            m_pShape = new btBoxShape(halfSize);
        }
        break;

        case eCylinderShapeX:
        {
            btVector3 halfSize = CBulletHelper::toBulletVector3D(m_pNode->getLocalAxisAlignedBoundingBox(false).getHalfSize());
            m_pShape = new btCylinderShapeX(halfSize);
        }
        break;

        case eCylinderShapeY:
        {
            btVector3 halfSize = CBulletHelper::toBulletVector3D(m_pNode->getLocalAxisAlignedBoundingBox(false).getHalfSize());
            m_pShape = new btCylinderShape(halfSize);
        }
        break;

        case eCylinderShapeZ:
        {
            btVector3 halfSize = CBulletHelper::toBulletVector3D(m_pNode->getLocalAxisAlignedBoundingBox(false).getHalfSize());
            m_pShape = new btCylinderShapeZ(halfSize);
        }
        break;

        case eConvexHullShape:
        {
            m_pShape = new btConvexHullShape();
            if (CMeshInstance* pMeshInstance = dynamic_cast<CMeshInstance*>(pItem))
            {
                btConvexHullShape* pConvexShape = static_cast<btConvexHullShape*>(m_pShape);
                foreach (CSubMesh* pSubMesh, pMeshInstance->getMesh()->subMeshs())
                {
                        foreach (const QVector3D& pt, pSubMesh->positionsBuffer())
                        {
                            pConvexShape->addPoint(CBulletHelper::toBulletVector3D(pt), false);
                        }
                }
                pConvexShape->recalcLocalAabb();
            }
            else
            {
                pLog->addMessage(eERROR, "Only mesh can have convex hull shape !");
            }
        }
        break;

        default:
        case eSphereShape:
        {
            real radius = CSphere(m_pNode->getLocalAxisAlignedBoundingBox(false), CSphere::eInnerSphere).getRadius();
            m_pShape = new btSphereShape(radius);
        }
        break;

//            TODO
//            eCapsuleShape,
//            eConeShape,
//            eMultiSphereShape,
//            eCompoundShape,
//            eConvexHullShape,
//            eBvhTriangleMeshShape,
//            eHeightfieldTerrainShape,
//            eStaticPlaneShape
    }

    // Mass, inertia
    btVector3 localInertia(0, 0, 0);
    btScalar dMass = m_pNode->getGlobalMass();

    if (dMass > 0.)
    {
        m_pShape->calculateLocalInertia(dMass, localInertia);
    }

    // Bullet suppose our bounding box is centered on zero. It's not always true (in particulary with assimp models).
    // We need to fix that with a local translation matrix
    QMatrix4x4 localTranslation;
    localTranslation.translate(pNode->getLocalAxisAlignedBoundingBox(false).getCenter());

    // Position, orientation.
    btTransform transform = CBulletHelper::toBulletTransform(localTranslation * m_pNode->getGlobalTransformation());

    // Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    m_pMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo boxRigidBodyConstructionInfo(dMass, m_pMotionState, m_pShape, localInertia);
    m_pBody = new btRigidBody(boxRigidBodyConstructionInfo);
    updateBodyParameters(pItem);
    pWorld->addRigidBody(m_pBody);

    QVector3D localScaling = pNode->getLocalScaling();
    m_pShape->setLocalScaling(CBulletHelper::toBulletVector3D(localScaling));

//    btTransform t;
//    t.setIdentity();
//    btVector3 min, max;
//    m_pShape->getAabb(t, min, max);

//    m_pNode->addItem(CDebugManager::getInstance()->createBox(
//                         CBox3D(Math::divide(CBulletHelper::toQVector3D(min), localScaling),
//                                Math::divide(CBulletHelper::toQVector3D(max), localScaling)),
//                         Color::eRed));
}

//-----------------------------------------------------------------------
CPhysicsManager::CShapeInfo::~CShapeInfo()
{
    if (m_pWorld && m_pBody && m_pShape)
    {
        m_pWorld->removeRigidBody(m_pBody);
        delete m_pBody;
        delete m_pShape;
    }
}

//-----------------------------------------------------------------------
void CPhysicsManager::CShapeInfo::updateBodyParameters(ASceneNodeItem* pItem)
{
    m_pBody->setRestitution(pItem->getRestitution());
    m_pBody->setFriction(pItem->getFriction());
    m_pBody->setRollingFriction(pItem->getRollingFriction());
    m_pBody->setLinearVelocity(CBulletHelper::toBulletVector3D(pItem->getLinearVelocity()));
    m_pBody->setAngularVelocity(CBulletHelper::toBulletVector3D(pItem->getAngularVelocity()));
}
