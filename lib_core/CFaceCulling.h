#ifndef CFACECULLING_H
#define CFACECULLING_H


class CFaceCulling
{
public:

    CFaceCulling()
    {
        m_bEnabled = true;
        m_eCullFace = eBack;
    }

    CFaceCulling(bool bEnabled, EnumCullFace eFaceCulling = eBack)
    {
        m_bEnabled = bEnabled;
        m_eCullFace = eFaceCulling;
    }

    bool isEnabled() const { return m_bEnabled; }

    void setEnabled(bool bEnabled) { m_bEnabled = bEnabled; }

    EnumCullFace getCullFace() const { return m_eCullFace; }

    void setCullFace(EnumCullFace eCullFace) { m_eCullFace = eCullFace; }

    friend bool operator != (const CFaceCulling& f1, const CFaceCulling& f2);

    virtual QString toString() const
    {
        QString result;
        result += QString("Face culling enabled: ").arg(m_bEnabled);
        return result;
    }

private:

    bool m_bEnabled;
    EnumCullFace m_eCullFace;
};

inline bool operator != (const CFaceCulling& f1, const CFaceCulling& f2)
{
    return
            f1.m_bEnabled != f2.m_bEnabled ||
            f1.m_eCullFace != f2.m_eCullFace;
}
#endif // CFACECULLING_H
