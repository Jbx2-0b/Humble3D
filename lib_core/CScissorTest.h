#ifndef CSCISSORTEST_H
#define CSCISSORTEST_H

// Qt
#include <QRect>


class CScissorTest
{
public:
    CScissorTest()
        : m_bEnabled(false)
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

    const QRect& getRect() const
    {
        return m_Rect;
    }

    void setRect(const QRect& rect)
    {
        m_Rect = rect;
    }

    friend bool operator != (const CScissorTest& c1, const CScissorTest& c2);

private:

    bool m_bEnabled;
    QRect m_Rect;
};

inline bool operator != (const CScissorTest& c1, const CScissorTest& c2)
{
    return
            c1.m_bEnabled != c2.m_bEnabled ||
            c1.m_Rect != c2.m_Rect;
}

#endif // CSCISSORTEST_H
