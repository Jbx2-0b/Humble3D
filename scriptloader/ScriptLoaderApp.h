#ifndef SCRIPTLOADERAPP_H
#define SCRIPTLOADERAPP_H


#include "Humble3DApp.h"
#include "Controller.h"


class ScriptLoaderApp : public Humble3DApp
{
public:
    ScriptLoaderApp();

    virtual ~ScriptLoaderApp();

private:

    Controller* m_pController;
};

#endif // SCRIPTLOADERAPP_H
