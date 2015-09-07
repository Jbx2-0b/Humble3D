#ifndef CMESHBUFFER_H
#define CMESHBUFFER_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CTriangle.h"
#include "CGeometryGlobal.h"
#include "CBuffer.h"
#include "CBox3D.h"
#include "CBone.h"
#include "CSceneNode.h"
#include "CArray.h"

#include <QDebug>


#ifdef EMBEDDED_TARGET
typedef unsigned short IndiceType;
#else
typedef unsigned int IndiceType;
#endif

// typedef CArray<float, DefaultMaxBonesPerVertex> TBoneIDs;
// typedef CArray<float, DefaultMaxBonesPerVertex> TBoneWeights;

typedef float TBoneIDs;
typedef float TBoneWeights;

class LIB_CORE_SHARED_EXPORT CMeshBuffer : public AEntity
{
public:

    //! Constructeur
    CMeshBuffer(EnumPrimitiveType ePrimitiveType = ePrimitiveTriangles, unsigned int iEdgeCount = 3)
        : m_bDirty(true)
        , m_eUsagePattern(eStaticDraw)
        , m_bNeedBoundingBoxUpdate(true)
        , m_uiPrimitiveIndex(iEdgeCount)
        , m_ePrimitiveType(ePrimitiveType)
        , m_bAutoComputeNormals(true)
    {
    }

    //! Destructeur
    virtual ~CMeshBuffer()
    {
        foreach (IMeshBufferListener* pListener, m_MeshBufferListeners)
        {
            pListener->onDelete(this);
        }
    }

    //! Ajoute un buffer
    void append(const CMeshBuffer& buffer);

    //! Efface la géométrie
    void clear();

    //! Crée un buffer utilisateur
    CUserBuffer* createUserBuffer(const QString& name, int iTupleSize);

    //--------------------------------------------------------------------------------------------
    // LISTENERS
    //--------------------------------------------------------------------------------------------

    class IMeshBufferListener
    {
        friend class CMeshBuffer;

    protected:
        virtual void onDelete(CMeshBuffer* pVB) = 0;
    };

    //! Enregistre un listener
    void registerListener(IMeshBufferListener* pListener)
    {
        m_MeshBufferListeners.insert(pListener);
    }

    //! Libére un listener
    void unregisterListener(IMeshBufferListener* pListener)
    {
        m_MeshBufferListeners.remove(pListener);
    }

    //--------------------------------------------------------------------------------------------
    // SETTERS
    //--------------------------------------------------------------------------------------------

    //! Définit le type de primitive
    void setPrimitiveType(EnumPrimitiveType ePrimitiveType)                         { m_ePrimitiveType = ePrimitiveType; }

    //! Définit le type d'utilisation du vertex buffer
    void setUsagePattern(EnumUsagePattern eUsagePattern)							{ m_eUsagePattern = eUsagePattern; }

    //! Définit si le buffer hardware doit être mis à jour
    void setDirty()																	{ m_bDirty = true; m_bNeedBoundingBoxUpdate = true; }

    //! Définit que le buffer hardware est à jour
    void wash() const;

    //! Définit le nombre de côté par face
    void setPrimitiveIndex(unsigned int uiPrimitiveIndex)							{ m_uiPrimitiveIndex = uiPrimitiveIndex; }

    //! Définit si on calcule automatiquement les normales
    void setAutoComputeNormals(bool bEnabled)										{ m_bAutoComputeNormals = bEnabled; }

    //--------------------------------------------------------------------------------------------
    // GETTERS
    //--------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const                                             { return "MeshBuffer"; }

    //! Retourne le type de primitive
    EnumPrimitiveType getPrimitiveType() const                                      { return m_ePrimitiveType; }

    //! Retourne le type d'utilisation du vertex buffer
    EnumUsagePattern getUsagePattern()	const										{ return m_eUsagePattern; }

    //! Retourne le nombre de face
    unsigned int getPolygonCount() const;

    //! Retourne le nombre de côté par face
    inline unsigned int getPrimitiveIndex() const									{ return m_uiPrimitiveIndex; }

    //! Retourne la boîte englobant l'objet
    const CBox3D& getBoundingBox() const;

    //! Retourne vrai si le buffer hardware doit être mis à jour
    bool isDirty() const															{ return m_bDirty; }

    //! Retourne le nombre de canaux de texture
    inline unsigned int getMaxTextureChannelCount() const							{ return DefaultMaxTextureChannelCount; }

    //! Retourne vrai si le moteur calcule automatiquement les normales
    bool getAutoComputeNormals() const												{ return m_bAutoComputeNormals; }

    //! Retourne true si le buffer a un squelette d'animation
    bool hasSkeleton() const														{ return !m_BonesBuffer.isEmpty();}

    //! Retourne un buffer utilisateur
    CUserBuffer& getUserBuffer(const QString& name)									{ return m_UserBuffers[name]; }


    //--------------------------------------------------------------------------------------------
    // ACCESSEURS
    //--------------------------------------------------------------------------------------------

    //! Accesseur buffer positions
    inline CBuffer<QVector3D>& positionsBuffer()									{ return m_PositionsBuffer; }

    //! Accesseur buffer positions
    inline const CBuffer<QVector3D>& positionsBuffer() const						{ return m_PositionsBuffer; }

    //! Accesseur buffer indices
    inline CBuffer<IndiceType>& indicesBuffer()										{ return m_IndicesBuffer; }

    //! Accesseur buffer indices
    inline const CBuffer<IndiceType>& indicesBuffer() const							{ return m_IndicesBuffer; }

    //! Accesseur buffer normales
    inline CBuffer<QVector3D>& normalsBuffer()										{ return m_NormalsBuffer; }

    //! Accesseur buffer normales
    inline const CBuffer<QVector3D>& normalsBuffer() const							{ return m_NormalsBuffer; }

    //! Accesseur buffer tangeantes
    inline CBuffer<QVector3D>& tangentsBuffer()										{ return m_TangeantsBuffer; }

    //! Accesseur buffer tangeantes
    inline const CBuffer<QVector3D>& tangentsBuffer() const							{ return m_TangeantsBuffer; }

    //! Accesseur buffer bitangeantes
    inline CBuffer<QVector3D>& bitangentsBuffer()									{ return m_BitangeantsBuffer; }

    //! Accesseur buffer bitangeantes
    inline const CBuffer<QVector3D>& bitangentsBuffer() const						{ return m_BitangeantsBuffer; }

    //! Accesseur buffer normales de face
    inline CBuffer<QVector3D>& faceNormalsBuffer()									{ return m_FaceNormalsBuffer; }

    //! Accesseur buffer normales de face
    inline const CBuffer<QVector3D>& faceNormalsBuffer() const						{ return m_FaceNormalsBuffer; }

    //! Accesseur buffer texture
    inline CBuffer<QVector3D>& texCoordBuffer(unsigned int iChannel)				{ return m_TexCoordsBuffer[iChannel]; }

    //! Accesseur buffer texture
    inline const CBuffer<QVector3D>& texCoordBuffer(unsigned int iChannel) const	{ return m_TexCoordsBuffer[iChannel]; }

    //! Accesseur buffer texture
    inline CBuffer<QVector3D>* texCoordBuffer()										{ return m_TexCoordsBuffer; }

    //! Accesseur buffer squelette d'animation
    inline CBuffer<CBone>& bonesBuffer()											{ return m_BonesBuffer; }

    //! Accesseur buffer squelette d'animation
    inline const CBuffer<CBone>& bonesBuffer() const								{ return m_BonesBuffer; }

    //! Accesseur sur le buffer des ID de bones
    inline CBuffer<TBoneIDs>& boneIDsBuffer()										{ return m_BonesIDsBuffer; }

    //! Accesseur sur le buffer des ID de bones
    inline const CBuffer<TBoneIDs>& boneIDsBuffer() const							{ return m_BonesIDsBuffer; }

    //! Accesseur sur le buffer des poids de bones
    inline CBuffer<TBoneWeights>& boneWeightsBuffer()								{ return m_BonesWeightsBuffer; }

    //! Accesseur sur le buffer des poids de bones
    inline const CBuffer<TBoneWeights>& boneWeightsBuffer() const					{ return m_BonesWeightsBuffer; }

    //! Accesseur sur les buffers utilisateur
    inline QMap<QString, CUserBuffer>& userBuffers()                                { return m_UserBuffers; }

    //! Accesseur sur les buffers utilisateur
    inline const QMap<QString, CUserBuffer>& userBuffers() const					{ return m_UserBuffers; }

    //--------------------------------------------------------------------------------------------
    // MATHS
    //--------------------------------------------------------------------------------------------

    //! Effectue une translation
    void translate(real dX, real dY, real dZ);

    //! Effectue un changement d'échelle
    void scale(real dX, real dY, real dZ);

    //! Effectue une rotation
    void rotate(const QVector3D &vAxis, real dAngle);

    //! Calcule les normales
    virtual void computeNormals();

    //! Calcule les tangeantes
    virtual void computeTangents();

    //! Calcule les os
    virtual void computeBones();

    //! Calcule l'intersection avec un rayon
    bool intersection(const CRay& ray, real* dDistance, const QMatrix4x4& transformation = QMatrix4x4()) const;

    //! Calcule l'intersection avec un rayon
    bool intersects(const CRay &ray, const QMatrix4x4& transformation = QMatrix4x4()) const;

    //--------------------------------------------------------------------------------------------
    // OPERATEURS
    //--------------------------------------------------------------------------------------------

    //! Ajoute un autre buffer et retourne une reference sur ce buffer
    CMeshBuffer& operator << (const CMeshBuffer& meshBuffer);

protected:

    //! True si le buffer a été modifié
    mutable bool m_bDirty;

    //! Type de buffer
    EnumUsagePattern m_eUsagePattern;

    //! Boîte englobante
    mutable CBox3D m_BoundingBox;

    //! Vrai si la maj de la boîte englobante nécessaire
    mutable bool m_bNeedBoundingBoxUpdate;

    //! Calcul la boîte englobante
    void computeBoundinBox() const;

    //! Nombre de côté par face
    unsigned int m_uiPrimitiveIndex;

    //! Type de vertexbuffer
    EnumPrimitiveType m_ePrimitiveType;

    //! Buffer position
    CBuffer<QVector3D> m_PositionsBuffer;

    //! Buffer des indices de face
    CBuffer<IndiceType> m_IndicesBuffer;

    //! Buffer de normales
    CBuffer<QVector3D> m_NormalsBuffer;

    //! Buffer de tangeantes
    CBuffer<QVector3D> m_TangeantsBuffer;

    //! Buffer de bitangeantes
    CBuffer<QVector3D> m_BitangeantsBuffer;

    //! Buffers des coordonnées de texture
    CBuffer<QVector3D> m_TexCoordsBuffer[DefaultMaxTextureChannelCount];

    //! Buffer des coordonnées de normales de face
    CBuffer<QVector3D> m_FaceNormalsBuffer;

    //! Buffer des bones
    CBuffer<CBone> m_BonesBuffer;

    //! Buffer des ID de bones
    CBuffer<TBoneIDs> m_BonesIDsBuffer;

    //! Buffer des poids des bones
    CBuffer<TBoneWeights> m_BonesWeightsBuffer;

    //! Buffer utilisateur
    QMap<QString, CUserBuffer> m_UserBuffers;

    //! Vrai si le moteur calcule automatiquement les normales
    bool m_bAutoComputeNormals;

    //! Listeners
    QSet<IMeshBufferListener*> m_MeshBufferListeners;
};

#endif // CMESHBUFFER_H
