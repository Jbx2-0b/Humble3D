#ifndef CRENDERSTATES_H
#define CRENDERSTATES_H

// Qt
#include <QList>

// Foundations
#include "CGeometryGlobal.h"
#include "CFaceCulling.h"
#include "CScissorTest.h"
#include "CDepthTest.h"
#include "CDepthRange.h"
#include "CBlending.h"
#include "CColorMask.h"
#include "CStencilState.h"


//! Paramètres de rendu
class CRenderStates
{
public:

    enum EnumDepthTestFunction
    {
        eNever,
        eLess,
        eEqual,
        eLessThanOrEqual,
        eGreater,
        eNotEqual,
        eGreaterThanOrEqual,
        eAlways
    };

    //! Constructeur
    CRenderStates()
        : m_bDepthMask(true)
    {
    }

    //! Constructeur de recopie
    CRenderStates(const CRenderStates& states)
    {
        *this = states;
    }

    //! Operateur d'assignement
    CRenderStates& operator = (const CRenderStates& states)
    {
        m_FaceCulling = states.m_FaceCulling;
        m_ScissorTest = states.m_ScissorTest;
        m_DepthTest = states.m_DepthTest;
        m_DepthRange = states.m_DepthRange;
        m_Blending = states.m_Blending;
        m_ColorMask = states.m_ColorMask;
        m_StencilState = states.m_StencilState;
        m_bDepthMask = states.m_bDepthMask;

        return *this;
    }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    CFaceCulling& faceCulling()
    {
        return m_FaceCulling;
    }

    const CFaceCulling& getFaceCulling() const
    {
        return m_FaceCulling;
    }

    CScissorTest& scissorTest()
    {
        return m_ScissorTest;
    }

    const CScissorTest& getScissorTest() const
    {
        return m_ScissorTest;
    }

    CDepthTest& depthTest()
    {
        return m_DepthTest;
    }

    const CDepthTest& getDepthTest() const
    {
        return m_DepthTest;
    }

    CDepthRange& depthRange()
    {
        return m_DepthRange;
    }

    const CDepthRange& getDepthRange() const
    {
        return m_DepthRange;
    }

    CBlending& blending()
    {
        return m_Blending;
    }

    const CBlending& getBlending() const
    {
        return m_Blending;
    }

    CColorMask& colorMask()
    {
        return m_ColorMask;
    }

    const CColorMask& getColorMask() const
    {
        return m_ColorMask;
    }

    bool hasDepthMask() const
    {
        return m_bDepthMask;
    }

    CStencilState& stencilState()
    {
        return m_StencilState;
    }

    const CStencilState& getStencilState() const
    {
        return m_StencilState;
    }

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    void setFaceCulling(const CFaceCulling& faceCulling)
    {
        m_FaceCulling = faceCulling;
    }

    void setScissorTest(const CScissorTest& scissorTest)
    {
        m_ScissorTest = scissorTest;
    }

    void setDepthTest(const CDepthTest& depthTest)
    {
        m_DepthTest = depthTest;
    }

    void setDepthRange(const CDepthRange& depthRange)
    {
        m_DepthRange = depthRange;
    }

    void setBlending(const CBlending& blending)
    {
        m_Blending = blending;
    }

    void setColorMask(const CColorMask& colorMask)
    {
        m_ColorMask = colorMask;
    }

    void setDepthMask(bool bDepthMask)
    {
        m_bDepthMask = bDepthMask;
    }

    void setStencilState(const CStencilState& stencilState)
    {
        m_StencilState = stencilState;
    }

private:

    CFaceCulling m_FaceCulling;
    CScissorTest m_ScissorTest;
    CDepthTest m_DepthTest;
    CDepthRange m_DepthRange;
    CBlending m_Blending;
    CColorMask m_ColorMask;
    CStencilState m_StencilState;
    bool m_bDepthMask;
};

#endif // CRENDERSTATES_H
