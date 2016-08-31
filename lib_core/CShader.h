#ifndef CSHADER_H
#define CSHADER_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "AEntity.h"
#include "CMath.h"

// Qt
#include <QCoreApplication>
#include <QString>
#include <QVariant>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix2x2>
#include <QMatrix3x3>
#include <QMatrix4x4>

Q_DECLARE_METATYPE(QVector<int>)
Q_DECLARE_METATYPE(QVector<real>);
Q_DECLARE_METATYPE(QVector<QVector2D>);
Q_DECLARE_METATYPE(QVector<QVector3D>);
Q_DECLARE_METATYPE(QVector<QVector4D>);
Q_DECLARE_METATYPE(QVector<QMatrix4x4>);

template<typename T>
QVariantList toVariantList(const QVector<T>& list)
{
    QVariantList newList;
    foreach(const T& item, list)
        newList << item;

    return newList;
}

//QMetaType::Type type(const QVariantList& list)
//{
//	if (list.size() > 0)
//		return (QMetaType::Type) list[0].type();

//	return QMetaType::Void;
//}

typedef QPair<QString, QVariant> TUniformValue;
static TUniformValue InvalidUniformValue("", "");

typedef QPair<QString, QVariant> TUniformArray;
static TUniformValue InvalidUniformArray("", "");

class LIB_CORE_SHARED_EXPORT CShader : public AEntity
{

public:

    //! Constructeur
    CShader(const QString& name = "Shader");

    //! Constructeur
    CShader(const QString& name, const QString& vertexShaderFileName, const QString& geometryShaderFileName, const QString& fragmentShaderFileName);

    //! Destructeur
    virtual ~CShader();

    //! Charge les shaders
    void load();

    //! Retourne true si est valide
    bool isValid() const { return m_bValid; }

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IShaderListener
    {
        friend class CShader;

    protected:

        virtual void onUpdate(CShader* pShader) = 0;
        virtual void onDelete(CShader* pShader) = 0;
    };

    //! Enregistre un listener
    void registerListener(IShaderListener* pListener);

    //! Libére un listener
    void unregisterListener(IShaderListener* pListener);

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit le nom du fichier contenant le vertex shader
    void setVertexShaderFileName(const QString& fileName)                               { m_VertexShaderFileName = fileName; load(fileName, m_VertexShaderCode); notifyUpdate(); }

    //! Définit le nom du fichier contenant le geometry shader
    void setGeometryShaderFileName(const QString& fileName)                             { m_GeometryShaderFileName = fileName; load(fileName, m_GeometryShaderCode); notifyUpdate(); }

    //! Définit le nom du fichier contenant le fragment shader
    void setFragmentShaderFileName(const QString& fileName)                             { m_FragmentShaderFileName = fileName; load(fileName, m_FragmentShaderCode); notifyUpdate(); }

    //! Définit le nom du fichier contenant le vertex shader
    void setTessellationControlFileName(const QString& fileName)                        { m_TessellationControlShaderFileName = fileName; load(fileName, m_TessellationControlShaderCode); notifyUpdate(); }

    //! Définit le nom du fichier contenant le geometry shader
    void setTessellationEvaluationShaderFileName(const QString& fileName)               { m_TessellationEvaluationShaderFileName = fileName; load(fileName, m_TessellationEvaluationShaderCode); notifyUpdate(); }

    //! Définit le nom du fichier contenant le fragment shader
    void setComputeShaderFileName(const QString& fileName)                              { m_ComputeShaderFileName = fileName; load(fileName, m_ComputeShaderCode); notifyUpdate(); }

    //! Définit le code du vertex shader
    void setVertexShaderCode(const QString& code)                                       { m_VertexShaderCode = code; notifyUpdate(); }

    //! Définit le code du geometry shader
    void setGeometryShaderCode(const QString& code)                                     { m_GeometryShaderCode = code; notifyUpdate(); }

    //! Définit le code du fragment shader
    void setFragmentShaderCode(const QString& code)                                     { m_FragmentShaderCode = code; notifyUpdate(); }

    //! Définit le code du vertex shader
    void setTessellationControlCode(const QString& code)                                { m_TessellationControlShaderCode = code; notifyUpdate(); }

    //! Définit le code du geometry shader
    void setTessellationEvaluationShaderCode(const QString& code)                       { m_TessellationEvaluationShaderCode = code; notifyUpdate(); }

    //! Définit le code du fragment shader
    void setComputeShaderCode(const QString& code)                                      { m_ComputeShaderCode = code; notifyUpdate(); }

    // Ajoute une valeur uniforme de type double
    void setUniformValue(const QString& name, double dValue)							{ m_UniformValues[name] = TUniformValue(name, dValue); }

    // Ajoute une valeur uniforme de type double
    void setUniformValue(const QString& name, float dValue)								{ m_UniformValues[name] = TUniformValue(name, dValue); }

    // Ajoute une valeur uniforme de type entier
    void setUniformValue(const QString& name, int iValue)								{ m_UniformValues[name] = TUniformValue(name, iValue); }

    // Ajoute une valeur uniforme de type vector 2D
    void setUniformValue(const QString& name, const QVector2D& vValue)					{ m_UniformValues[name] = TUniformValue(name, vValue); }

    // Ajoute une valeur uniforme de type vector 3D
    void setUniformValue(const QString& name, const QVector3D& vValue)					{ m_UniformValues[name] = TUniformValue(name, vValue); }

    // Ajoute une valeur uniforme de type vector 4D
    void setUniformValue(const QString& name, const QVector4D& vValue)					{ m_UniformValues[name] = TUniformValue(name, vValue); }

    // Ajoute une valeur uniforme de type vector 2D
    void setUniformValue(const QString& name, const QMatrix2x2& mValue)					{ m_UniformValues[name] = TUniformValue(name, QVariant::fromValue<QMatrix2x2>(mValue)); }

    // Ajoute une valeur uniforme
    void setUniformValue(const QString& name, const QMatrix3x3& mValue)					{ m_UniformValues[name] = TUniformValue(name, QVariant::fromValue<QMatrix3x3>(mValue)); }

    // Ajoute une valeur uniforme
    void setUniformValue(const QString& name, const QMatrix4x4& mValue)					{ m_UniformValues[name] = TUniformValue(name, mValue); }

    // Ajoute une tableau uniforme
    void setUniformValueArray(const QString& name, const QVector<real>& mArray)		{ m_UniformArrays[name] = TUniformArray(name, QVariant::fromValue(mArray)); }

    // Ajoute une tableau uniforme
    void setUniformValueArray(const QString& name, const QVector<QVector2D>& mArray)	{ m_UniformArrays[name] = TUniformArray(name, QVariant::fromValue(mArray)); }

    // Ajoute une tableau uniforme
    void setUniformValueArray(const QString& name, const QVector<QVector3D>& mArray)	{ m_UniformArrays[name] = TUniformArray(name, QVariant::fromValue(mArray)); }

    // Ajoute une tableau uniforme
    void setUniformValueArray(const QString& name, const QVector<QVector4D>& mArray)	{ m_UniformArrays[name] = TUniformArray(name, QVariant::fromValue(mArray)); }

    // Ajoute une tableau uniforme
    void setUniformValueArray(const QString& name, const QVector<QMatrix4x4>& mArray)	{ m_UniformArrays[name] = TUniformArray(name, QVariant::fromValue(mArray)); }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Shader"; }

    //! Retourne vrai si le shader est considéré comme valide
    bool isValid() { return hasVertexShader() && hasFragmentShader(); }

    //! Retourne le nom du fichier contenant le vertex shader
    const QString& getVertexShaderFileName() const										{ return m_VertexShaderFileName; }

    //! Retourne le nom du fichier contenant le geometry shader
    const QString& getGeometryShaderFileName() const									{ return m_GeometryShaderFileName; }

    //! Retourne le nom du fichier contenant le fragment shader
    const QString& getFragmentShaderFileName() const									{ return m_FragmentShaderFileName; }

    //! Retourne le nom du fichier contenant le vertex shader
    const QString& getTessellationControlShaderFileName() const							{ return m_TessellationControlShaderFileName; }

    //! Retourne le nom du fichier contenant le geometry shader
    const QString& getTessellationEvaluationShaderFileName() const						{ return m_TessellationEvaluationShaderFileName; }

    //! Retourne le nom du fichier contenant le fragment shader
    const QString& getComputeShaderFileName() const                                     { return m_ComputeShaderFileName; }

    //! Retourne le code du vertex shader
    const QString& getVertexShaderCode() const											{ return m_VertexShaderCode; }

    //! Retourne le code du geometry shader
    const QString& getGeometryShaderCode() const										{ return m_GeometryShaderCode; }

    //! Retourne le code du fragment shader
    const QString& getFragmentShaderCode() const										{ return m_FragmentShaderCode; }

    //! Retourne le code du vertex shader
    const QString& getTessellationControlShaderCode() const								{ return m_TessellationControlShaderCode; }

    //! Retourne le code du geometry shader
    const QString& getTessellationEvaluationShaderCode() const							{ return m_TessellationEvaluationShaderCode; }

    //! Retourne le code du fragment shader
    const QString& getComputeShaderCode() const                                         { return m_ComputeShaderCode; }

    //! Retourne vrai si a un vertex shader de définit
    bool hasVertexShader() const														{ return !m_VertexShaderCode.isEmpty(); }

    //! Retourne vrai si a un geometry shader de définit
    bool hasGeometryShader() const														{ return !m_GeometryShaderCode.isEmpty(); }

    //! Retourne vrai si a un fragment shader de définit
    bool hasFragmentShader() const														{ return !m_FragmentShaderCode.isEmpty(); }

    //! Retourne vrai si a un vertex shader de définit
    bool hasTessellationControlShader() const											{ return !m_TessellationControlShaderCode.isEmpty(); }

    //! Retourne vrai si a un geometry shader de définit
    bool hasTessellationEvaluationShader() const										{ return !m_TessellationEvaluationShaderCode.isEmpty(); }

    //! Retourne vrai si a un fragment shader de définit
    bool hasComputeShader() const														{ return !m_ComputeShaderCode.isEmpty(); }

    //! Retourne l'ensemble des uniform values
    const QHash<QString, TUniformValue>& getUniformValues() const						{ return m_UniformValues; }

    //! Retourne l'ensemble des uniform array
    const QHash<QString, TUniformArray>& getUniformValueArrays() const					{ return m_UniformArrays; }

    //! Retourne une uniform value depuis son nom
    TUniformValue getUniformValueByName(const QString& name) const						{ return m_UniformValues.value(name, InvalidUniformValue); }

    //! Retourne un uniform array depuis son nom
    TUniformArray getUniformValueArrayByName(const QString& name) const					{ return m_UniformArrays.value(name, InvalidUniformArray); }

    //-------------------------------------------------------------------------------------------------
    // Operators
    //-------------------------------------------------------------------------------------------------

    inline bool operator == (const CShader& sD) const
    {
        return getVertexShaderFileName()                    == sD.getVertexShaderFileName() &&
                getGeometryShaderFileName()                  == sD.getGeometryShaderFileName() &&
                getFragmentShaderFileName()                  == sD.getFragmentShaderFileName() &&
                getTessellationControlShaderFileName()       == sD.getTessellationControlShaderFileName() &&
                getTessellationEvaluationShaderFileName()    == sD.getTessellationEvaluationShaderFileName() &&
                getComputeShaderFileName()                   == sD.getComputeShaderFileName();

    }

    //-------------------------------------------------------------------------------------------------
    // Tools
    //-------------------------------------------------------------------------------------------------

    //! Sauvegarde le vertex shader
    void saveVertexShader();

    //! Sauvegarde le geometry shader
    void saveGeometryShader();

    //! Sauvegarde le fragment shader
    void saveFragmentShader();

    //! Sauvegarde le vertex shader
    void saveTessellationControlShader();

    //! Sauvegarde le geometry shader
    void saveTessellationEvaluationShader();

    //! Sauvegarde le fragment shader
    void saveComputeShader();

    //! Affiche l'ensemble des valeurs uniformes sur la console
    void dumpUniformValues() const
    {
        foreach (const TUniformValue& uniformValue, m_UniformValues)
        {
            qDebug() << uniformValue.first << uniformValue.second;
        }
    }

protected:

    //! Transmet le message de mise à jour
    virtual void notifyUpdate();

    //! Transmet le message de suppression
    virtual void notifyDelete();

    //! Liste des listeners
    QSet<IShaderListener*> m_ShaderListeners;

private:

    //! Vrai si est valide
    bool m_bValid;

    //! Nom du fichier contenant le vertex shader
    QString m_VertexShaderFileName;

    //! Nom du fichier contenant le geometry shader
    QString m_GeometryShaderFileName;

    //! Nom du fichier contenant le fragment shader
    QString m_FragmentShaderFileName;

    //! Nom du fichier contenant le tessellation control shader
    QString m_TessellationControlShaderFileName;

    //! Nom du fichier contenant le tessellation evaluation shader
    QString m_TessellationEvaluationShaderFileName;

    //! Nom du fichier contenant le compute shader
    QString m_ComputeShaderFileName;

    //! Code du vertex shader
    QString m_VertexShaderCode;

    //! Code du geometry shader
    QString m_GeometryShaderCode;

    //! Code du fragment shader
    QString m_FragmentShaderCode;

    //! Code du tessellation control shader
    QString m_TessellationControlShaderCode;

    //! Code du tessellation evaluation shader
    QString m_TessellationEvaluationShaderCode;

    //! Code du compute shader
    QString m_ComputeShaderCode;

    //! Valeures uniformes
    QHash<QString, TUniformValue> m_UniformValues;

    //! Tableau uniformes
    QHash<QString, TUniformArray> m_UniformArrays;

    static bool load(const QString& shaderFileName, QString& shaderCode);
    static void save(const QString& shaderFileName, const QString& shaderCode);
};

#endif // CSHADER_H
