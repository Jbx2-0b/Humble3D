#ifndef CREGISTERFACTORY_H
#define CREGISTERFACTORY_H

#include <QMap>
#include <QString>


//--------------------------------------------------------------------------
template<class Base>
struct Instanciator
{
    virtual Base* create() = 0;
    virtual ~Instanciator() {}
};

//--------------------------------------------------------------------------
template<class Base, class Derived>
struct NewInstanciator : public Instanciator<Base>
{
    Base* create()
    {
        return new Derived();
    }
};

//--------------------------------------------------------------------------
template <class Base>
class CRegisterFactory
{
    QMap<QString, Instanciator<Base>*> m_registerMap;

public:

    void registerClass(const QString& id, Instanciator<Base>* pInst)
    {
        // gérer les cas où l'id est déjà enregistré
        // si la clé n'est pas déjà présente
        if (!m_registerMap.contains(id))
        {
            //on ajoute l'objet dans la map
            m_registerMap[id] = pInst;
        }
    }

    template<class Derived>
    void registerClass(const QString& id)
    {
        registerClass(id, new NewInstanciator<Base, Derived>());
    }

    Base* create(const QString& id)
    {
        if (m_registerMap.contains(id))
        {
            return m_registerMap[id]->create();
        }

        return 0;
    }

    virtual ~CRegisterFactory()
    {
        //détruire les instanciators
        foreach (Instanciator<Base>* pInst, m_registerMap)
        {
            delete pInst;
        }
    }
};

#endif // CREGISTERFACTORY_H
