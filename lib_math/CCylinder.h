#ifndef CCYLINDER_H
#define CCYLINDER_H

// Lib
#include "lib_math_global.h"

#include "CMath.h"


class LIB_MATHSHARED_EXPORT CCylinder
{
public:
    CCylinder();
    CCylinder(real dRadius, real dHeight);

    real getRadius() const;
    real getHeight() const;

private:

    real m_dRadius;
    real m_dHeight;
};

#endif // CCYLINDER_H
