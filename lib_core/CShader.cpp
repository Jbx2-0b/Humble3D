#include "CShader.h"
#include "CGeometryGlobal.h"


//-------------------------------------------------------------------------------------------------
CShader::CShader(const QString& name /*= "Shader"*/)
    : AEntity(name)
    , m_bValid(false)
{

}

//-------------------------------------------------------------------------------------------------
CShader::CShader(const QString& name, 
                 const QString& vertexShaderFileName,
                 const QString& geometryShaderFileName,
                 const QString& fragmentShaderFileName)
    : AEntity(name)
    , m_bValid(false)
{
    m_VertexShaderFileName = vertexShaderFileName;
    m_GeometryShaderFileName = geometryShaderFileName;
    m_FragmentShaderFileName = fragmentShaderFileName;

    load();
}

//-------------------------------------------------------------------------------------------------
CShader::~CShader()
{
    notifyDelete();
}

//-------------------------------------------------------------------------------------------------
void CShader::load()
{
    bool bVertexValid = load(m_VertexShaderFileName, m_VertexShaderCode);
    bool bFragmentValid = load(m_FragmentShaderFileName, m_FragmentShaderCode);

    if (!bVertexValid || !bFragmentValid)
    {
        pLog->addMessage(eWARN, "Shader error loading : " + getName() + " vertex shader or fragment shader missing. ");
    }
    load(m_GeometryShaderFileName, m_GeometryShaderCode);
    load(m_TessellationControlShaderFileName, m_TessellationControlShaderCode);
    load(m_TessellationEvaluationShaderFileName, m_TessellationEvaluationShaderCode);
    load(m_ComputeShaderFileName, m_ComputeShaderCode);

    m_bValid = bVertexValid && bFragmentValid;
}

//-------------------------------------------------------------------------------------------------
bool CShader::load(const QString& shaderFileName, QString& shaderCode)
{
    bool bReturn = false;
    if (!shaderFileName.isEmpty())
    {
        QFile file(shaderFileName);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            shaderCode = file.readAll();
            file.close();
            bReturn = true;
        }
        else
        {
            pLog->addMessage(eERROR, "CShader : " + shaderFileName + " not found");
        }
    }

    return bReturn;
}

//-------------------------------------------------------------------------------------------------
void CShader::save(const QString& shaderFileName, const QString& shaderCode)
{
    QFile file(shaderFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << shaderCode;

    file.close();
}

//-------------------------------------------------------------------------------------------------
void CShader::saveVertexShader()
{
    save(m_VertexShaderFileName, m_VertexShaderCode);
}

//-------------------------------------------------------------------------------------------------
void CShader::saveGeometryShader()
{
    save(m_GeometryShaderFileName, m_GeometryShaderCode);
}

//-------------------------------------------------------------------------------------------------
void CShader::saveFragmentShader()
{
    save(m_FragmentShaderFileName, m_FragmentShaderCode);
}

//-------------------------------------------------------------------------------------------------
void CShader::saveTessellationControlShader()
{
    save(m_TessellationControlShaderFileName, m_TessellationControlShaderCode);
}

//-------------------------------------------------------------------------------------------------
void CShader::saveTessellationEvaluationShader()
{
    save(m_TessellationEvaluationShaderFileName, m_TessellationEvaluationShaderCode);
}

//-------------------------------------------------------------------------------------------------
void CShader::saveComputeShader()
{
    save(m_ComputeShaderFileName, m_ComputeShaderCode);
}

//--------------------------------------------------------------------------
void CShader::registerListener(IShaderListener* pListener)
{
    m_ShaderListeners.insert(pListener);
}

//--------------------------------------------------------------------------
void CShader::unregisterListener(IShaderListener* pListener)
{
    m_ShaderListeners.remove(pListener);
}

//--------------------------------------------------------------------------
void CShader::notifyUpdate()
{
    if (m_bNotificationsEnabled)
    {
        foreach (IShaderListener* pListener, m_ShaderListeners)
        {
            pListener->onUpdate(this);
        }

        AEntity::notifyUpdate();
    }
}

//--------------------------------------------------------------------------
void CShader::notifyDelete()
{
    if (m_bNotificationsEnabled)
    {
        foreach (IShaderListener* pListener, m_ShaderListeners)
        {
            pListener->onDelete(this);
        }

        AEntity::notifyDelete();
    }
}

