#ifndef TESTOCTREEAPP_H
#define TESTOCTREEAPP_H

#include "Controller.h"
#include "Humble3DApp.h"

class TestOctreeApp : public Humble3DApp
{

public:
    TestOctreeApp();

    virtual ~TestOctreeApp();

private:

    Controller* m_pController;
    Controller* m_pController2;
};

#endif // TESTOCTREEAPP_H
