#ifndef CBINARYMESH_H
#define CBINARYMESH_H

class CBinaryMesh : public CMesh
{
public:

    CBinaryMesh(const QString& fileName, const QString& name)
        : CMesh(name)
        , m_FileName(fileName)
    {
        setMultithreadingEnabled(true);
    }
private:

    //! Fonction appelée à l'initialisation de l'objet
    virtual void updateGeometry()
    {
        QFile file(m_FileName);

        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray compressData = file.readAll();
            file.close();
            QByteArray uncompressData = qUncompress(compressData);

            QDataStream in(&uncompressData, QIODevice::ReadOnly);

            unsigned int iSubMeshCount;
            in >> iSubMeshCount;

            for (unsigned int i = 0; i < iSubMeshCount; ++i)
            {
                CSubMesh* pSubMesh = createSubMesh();
                in >> *pSubMesh;
            }

            computeNormals();
            computeTangents();
        }
    }

    QString m_FileName;
};

#endif // CBINARYMESH_H
