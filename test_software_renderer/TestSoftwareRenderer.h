#ifndef TEST_SOFTWARE_RENDERER_H
#define TEST_SOFTWARE_RENDERER_H

#include "Humble3DApp.h"

class TestSoftwareRenderer : public QObject, public Humble3DApp
{
    Q_OBJECT

public:

    //! Constructeur
    TestSoftwareRenderer();

    //! Destructeur
    virtual ~TestSoftwareRenderer();

};

#endif // TEST_SOFTWARE_RENDERER_H
