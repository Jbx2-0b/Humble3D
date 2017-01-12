#ifndef CKEYBOARDSTATES_H
#define CKEYBOARDSTATES_H

class CKeyboardStates
{
public:
    CKeyboardStates()
        : m_iKey(0)
    {}

    void setKey(int iKey) { m_iKey = iKey; }

    int getKey() const { return m_iKey; }
private:

    int m_iKey;
};

#endif // CKEYBOARDSTATES_H
