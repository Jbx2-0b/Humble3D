#ifndef CNODE_H
#define CNODE_H

#include "AEntity.h"

template <class T>
class CNode : public AEntity
{
public:
    //! Ctor
    CNode(T* pParent = 0)
        : m_pParent(pParent) {}

    //! Ctor
    CNode(const QString& name, T* pParent = 0)
        : AEntity(name)
        , m_pParent(pParent) {}

    //! Dtor
    virtual ~CNode()
    {
        notifyDelete();

        // On prévient le parent
        if (m_pParent)
        {
            m_pParent->removeChild(static_cast<T*>(this));
        }

        // On supprime l'ensemble des enfants

        foreach (CNode* pNode, m_ChildNodes)
        {
            delete pNode;
        }
    }

    class INodeListener
    {
    public:
        virtual void onUpdate(T* pNode) = 0;
        virtual void onDelete(T* pNode) = 0;
    };

    //! Enregistre un listener
    void registerListener(INodeListener* pListener)
    {
        m_NodeListeners.insert(pListener);
    }

    //! Libére un listener
    void unregisterListener(INodeListener* pListener)
    {
        m_NodeListeners.remove(pListener);
    }

    //! Définit le noeud parent
    void setParent(T* pParent)
    {
        m_pParent = pParent;
    }

    //! Retourne le noeud parent
    T* getParent() const
    {
        return m_pParent;
    }

    //! Retourne vrai si a des enfants
    bool hasChildNodes() const { return !m_ChildNodes.isEmpty(); }

    //! Retourne la liste des enfants
    QList<T*>& getChildNodes() { return m_ChildNodes; }

    //! Retourne la liste des enfants
    const QList<T*>& getChildNodes() const { return m_ChildNodes; }

    //! Retourne vrai si les deux noeuds sont liés
    bool isLinked(T* pNode) const
    {
        bool bLinked = false;

        if (pNode)
        {
            if (pNode == m_pParent)
            {
                bLinked = true;
            }
            else
            {
                foreach (T* pChild, getChildNodes())
                {
                    if (pNode == pChild)
                    {
                        bLinked = true;
                        break;
                    }
                }
            }
        }
        return bLinked;
    }

    //! Retourne vrai si les deux noeuds sont liés
    bool isLinked(const QString& nodeName) const
    {
        if (m_pParent)
        {
            if (m_pParent->getName() == nodeName)
            {
                return true;
            }
        }
        else
        {
            foreach (T* pChild, getChildNodes())
            {
                if (nodeName == pChild->getName())
                {
                    return true;
                }
            }
        }

        return false;
    }

    //! Retourne la profondeur du noeud
    unsigned int getDepth() const
    {
        unsigned int uiLevel = 0;

        const T* pCurrent = static_cast<const T*>(this);

        while (pCurrent != 0)
        {
            uiLevel++;
            pCurrent = pCurrent->getParent();
        }

        return uiLevel;
    }


    //! Ajoute un noeud enfant
    virtual void addChild(T* pNode)
    {
        m_ChildNodes.append(pNode);
        pNode->setParent(static_cast<T*>(this));
        notifyUpdate();
    }

    //! Détache un noeud enfant. Ne détruit pas le noeud
    T* removeChild(T* pNode)
    {
        m_ChildNodes.removeOne(pNode);
        pNode->setParent(0);
        notifyUpdate();
        return pNode;
    }

    //! Détache un noeud enfant. Ne détruit pas le noeud
    void deleteChild(T* pNode)
    {
        m_ChildNodes.removeOne(pNode);
        delete pNode;
        notifyUpdate();
    }

    //! Détache un noeud enfant. Ne détruit pas le noeud
    T* removeChild(const QString& name)
    {
        if (T* pNode = findNode(name))
        {
            return removeChild(pNode);
        }
        return 0;
    }


    //! Détache l'ensemble des noeuds enfants. Ne détruit pas les noeuds
    virtual void clearChildNodes()
    {
        foreach (T* pNode, m_ChildNodes)
        {
            pNode->setParent(0);
        }

        m_ChildNodes.clear();
        notifyUpdate();
    }

    //! Détache l'ensemble des noeuds enfants. Ne détruit pas les noeuds
    virtual void deleteChildNodes()
    {
        foreach (T* pNode, m_ChildNodes)
        {
            delete pNode;
        }

        m_ChildNodes.clear();
        notifyUpdate();
    }

    //! Retrouve un noeud en fonction de son nom
    T* findNode(const QString& name)
    {
        if (getName() == name) return static_cast<T*>(this);

        foreach (T* pChildNode, m_ChildNodes)
        {
            if (T* pNode = pChildNode->findNode(name))
            {
                return pNode;
            }
        }
        return 0;
    }

    //! Retrouve un noeud en fonction de son ID
    T* findNode(int iID)
    {
        if (getID() == iID) return static_cast<T*>(this);

        foreach (T* pChildNode, m_ChildNodes)
        {
            if (T* pNode = pChildNode->findNode(iID))
            {
                return pNode;
            }
        }
        return 0;
    }

    //! Retourne vrai si est une feuille
    bool isLeaf() const
    {
        return m_ChildNodes.isEmpty();
    }


protected:

    virtual void onChildUpdate(T*) {}

    virtual void onParentUpdate(T*) {}

    //! Notifie d'une mise à jour
    virtual void notifyUpdate()
    {
        if (m_bNotificationsEnabled)
        {
            if (m_pParent)
            {
                m_pParent->onChildUpdate(static_cast<T*>(this));
            }

            foreach (INodeListener* pListener, m_NodeListeners)
            {
                pListener->onUpdate(static_cast<T*>(this));
            }

            foreach (T* pNode, m_ChildNodes)
            {
                pNode->onParentUpdate(static_cast<T*>(this));
            }

            AEntity::notifyUpdate();
        }
    }

    //! Notifie d'une suppression
    virtual void notifyDelete()
    {
        if (m_bNotificationsEnabled)
        {
            foreach (INodeListener* pListener, m_NodeListeners)
            {
                pListener->onDelete(static_cast<T*>(this));
            }

            AEntity::notifyDelete();
        }
    }

    //! Liste des listeners
    QSet<INodeListener*> m_NodeListeners;

    //! Le noeud parent
    T* m_pParent;

    //! Les noeuds enfants
    QList<T*> m_ChildNodes;

    //! Parcourt et affiche l'arbre des noeuds sur la console
    virtual void recursiveDump(int iLevel) const
    {
        QString branchNode = "|";
        QString branchItems = " ";

        for (int i = 0; i < iLevel; ++i)
        {
            branchNode += "-";
            branchItems += " ";
        }
        pLog->addMessage(eINFO, QString("%1 Node name : %2.").arg(branchNode).arg(getName()));

        foreach (T* pChild, m_ChildNodes)
        {
            pChild->recursiveDump(iLevel + 1);
        }
    }
};

#endif // CNODE_H
