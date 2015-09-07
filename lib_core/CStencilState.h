#ifndef CSTENCILSTATE_H
#define CSTENCILSTATE_H


#include "CStencilTestFace.h"


class CStencilState
{
public:

    CStencilState()
        : m_bEnabled(false)
        , m_eFunction(eAlways)
        , m_iReferenceValue(0)
        , m_iMask(0)
    {

    }

    bool isEnabled() const
    {
        return m_bEnabled;
    }

    void setEnabled(bool bEnabled)
    {
        m_bEnabled = bEnabled;
    }

    const CStencilTestFace& getFrontFace() const
    {
        return m_StencilTestFrontFace;
    }

    const CStencilTestFace& getBackFace() const
    {
        return m_StencilTestBackFace;
    }

    CStencilTestFace& frontFace()
    {
        return m_StencilTestFrontFace;
    }

    CStencilTestFace& backFace()
    {
        return m_StencilTestBackFace;
    }

    void setFrontFace(const CStencilTestFace& testFace)
    {
        m_StencilTestFrontFace = testFace;
    }

    void setBackFace(const CStencilTestFace& testFace)
    {
        m_StencilTestBackFace = testFace;
    }

    void setBoth(const CStencilTestFace& testFace)
    {
        m_StencilTestFrontFace = testFace;
        m_StencilTestBackFace = testFace;
    }

    void setStencilTestFunction(EnumStencilTestFunction eFunction, int iRefValue, int iMask)
    {
        m_eFunction = eFunction;
        m_iReferenceValue = iRefValue;
        m_iMask = iMask;
    }

    EnumStencilTestFunction getStencilTestFunction() const
    {
        return m_eFunction;
    }

    int getReferenceValue() const
    {
        return m_iReferenceValue;
    }

    int getMask() const
    {
        return m_iMask;
    }

    friend bool operator != (const CStencilState& s1, const CStencilState& s2);

private:

    bool m_bEnabled;
    CStencilTestFace m_StencilTestFrontFace;
    CStencilTestFace m_StencilTestBackFace;

    EnumStencilTestFunction m_eFunction;

    int m_iReferenceValue;

    int m_iMask;
};


inline bool operator != (const CStencilState& s1, const CStencilState& s2)
{
    return
            s1.m_bEnabled				!= s2.m_bEnabled ||
            s1.m_StencilTestFrontFace	!= s2.m_StencilTestFrontFace ||
            s1.m_StencilTestBackFace	!= s2.m_StencilTestBackFace ||
            s1.m_eFunction				!= s2.m_eFunction ||
            s1.m_iReferenceValue		!= s2.m_iReferenceValue ||
            s1.m_iMask					!= s2.m_iMask;
}

#endif // CSTENCILSTATE_H
