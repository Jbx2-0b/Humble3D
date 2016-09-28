#ifndef CSCRIPTABLEFACTORY_H
#define CSCRIPTABLEFACTORY_H

#include "CRegisterFactory.h"
#include "IScriptable.h"

class CScriptableFactory : public CRegisterFactory<IScriptable>
{

public:

    template<typename Scriptable> void registerScriptable(const QString& typeName)
    {
        registerClass<Scriptable>(typeName);
    }

    IScriptable* createScriptable(const QString& typeName, const TParameters& parameters)
    {
        if (IScriptable* pScriptable = create(typeName))
        {
            pScriptable->init(parameters);
            return pScriptable;
        }

        return 0;
    }

    template<typename Scriptable> IScriptable* createScriptable(const QString& typeName, const TParameters& parameters)
    {
        return dynamic_cast<Scriptable*>(createScriptable(typeName, parameters));
    }

};

#endif //CSCRIPTABLEFACTORY_H
