#ifndef CASSIMPTOCSCENE_H
#define CASSIMPTOCSCENE_H

// Lib
#include "lib_assimp_global.h"

// Assimp
#ifndef _MSC_VER // Special stuff for GCC
#include "include/assimp/Importer.hpp"
#include "include/assimp/postprocess.h"
#include "include/assimp/scene.h"
#include "include/assimp/DefaultLogger.hpp"
#include "include/assimp/LogStream.hpp"
#else
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/LogStream.hpp"
#endif

// Foundations
#include "CSceneManager.h"
#include "CMeshManager.h"

class CAssimpScene
{


};

class CAssimpMesh : public CMesh
{
public:

    CAssimpMesh()
        : CMesh("Mesh")
        , m_pAssimpMesh(0)
    {
    }

    void setAiMesh(aiMesh* pAssimpMesh);

private:
    virtual void updateGeometry();

    aiMesh* m_pAssimpMesh;
};

class LIB_ASSIMPSHARED_EXPORT CAssimpImporter
{

public:

    static QList<AEntity*> mergeScene(const QString& fileName, CSceneManager* pSceneManager, bool bMergeMaterials, CSceneNode* pNode = 0);
    static QList<AEntity*> loadScene(const QString& fileName, CSceneManager* pSceneManager, bool bMergeMaterials);

    static QVector3D aiVector3DToQVector3D(const aiVector3D& v);
    static QMatrix4x4 aiMatrix4x4ToQMatrix4x4(const aiMatrix4x4& m);
    static QQuaternion aiQuaternionToQQuaternion(const aiQuaternion& q);
    static void aiNodeToCNode(CSceneManager* pSceneManager, const aiNode* pAssimpParentNode, CSceneNode* pParentNode, const QMap<int, CMeshInstance*>& mapMeshID);
    static QVector4D aiColor4DToQVector4D(const aiColor4D& c);
    static QVector4D aiColor3DToQVector4D(const aiColor3D& c);
    static EnumLightSourceType aiLightSourceTypeToCLightSourceLightType(aiLightSourceType e);
    static CVectorKey aiVectorKeyToCVectorKey(const aiVectorKey& k);
    static CQuaternionKey aiQuatKeyToCQuaternionKey(const aiQuatKey& k);
    static CMeshKey aiMeshKeyToAMeshKey(const aiMeshKey& k);
    static CVertexWeight aiVertexWeightToCVertexWeight(const aiVertexWeight& vw);
    static CBone aiBoneToCBone(const aiBone& b);

private:
    static QList<AEntity*> load(const QString& rootDirectory, const aiScene* pAssimpScene, CSceneManager* pSceneManager, bool bMergeMaterials, bool bMerge, CSceneNode* pNode);
};

#endif // CASSIMPTOCSCENE_H
