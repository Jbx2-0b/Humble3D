#ifndef CTREEEXPLORER_H
#define CTREEEXPLORER_H

#include <vector>
#include <deque>
#include <limits>
#include <map>

using namespace std;

/*! \class TreeExplorer
   * \brief TreeExplorer permet de parcourir un arbre de fa�on it�rative
   *
   *  Contrat pour le template Node :
   * - Poss�der une methode getChildNodes qui retourne les enfants dans un conteneur
   * - Le conteneur des enfants doit poss�der une m�thode size()
   * Exemple d'utilisation :
   *
   * CTreeExplorer<CSceneNode> te;
   *
   * struct SNodeTreatment : public CTreeExplorer<CSceneNode>::IExploreDepthListener
   * {
   *	virtual void onLeaf(CSceneNode* pNode)
   *	{
   *		qDebug() << pNode->getName();
   *	}
   *
   *	virtual void onExplore(CSceneNode* pNode)
   *	{
   *		qDebug() << pNode->getDepth();
   *	}
   * };
   *
   * SNodeTreatment t;
   *
   * te.exploreDepth(pRootNode, &t);
   */
template <typename Node>
class CTreeExplorer
{
public:

    //! Interface � impl�menter pour explorer un arbre en largeur
    struct IExploreWidthListener
    {
        virtual void onExplore(Node* pNode) = 0;
    };

    //! Explore un arbre en largeur
    static void exploreWidth(Node* pRoot, IExploreWidthListener* pListener, bool bMark = true)
    {
        map<Node*, bool> marked;
        deque<Node*> qu;
        qu.push_back(pRoot);

        if (bMark) marked[pRoot] = true;

        while (qu.size() > 0)
        {
            Node* pCurrent = qu.front();
            qu.pop_front();

            pListener->onExplore(pCurrent);

            for (int i = 0; i < pCurrent->getChildNodes().size(); ++i)
            {
                Node* pChild = pCurrent->getChildNodes()[i];

                if (bMark)
                {
                    // Non marqu� ?
                    if (marked.find(pChild) == marked.end())
                    {
                        marked[pChild] = true;
                        qu.push_back(pChild);
                    }
                }
                else
                {
                    qu.push_back(pChild);
                }
            }
        }
    }

    //! Interface � impl�menter pour explorer un arbre en profondeur
    struct IExploreDepthListener
    {
        virtual void onExplore(Node* pNode) = 0;
        virtual void onLeaf(Node* pNode) = 0;
    };

    //! Explore un arbre en profondeur
    static void exploreDepth(Node* pRoot, IExploreDepthListener* pListener, bool bMark = true)
    {
        map<Node*, bool> marked;

        deque<Node*> st;
        st.push_back(pRoot);

        if (bMark) marked[pRoot] = true;

        while (st.size() > 0)
        {
            Node* pCurrent = st.back();
            st.pop_back();

            pListener->onExplore(pCurrent);

            if (pCurrent->getChildNodes().size() == 0)
            {
                pListener->onLeaf(pCurrent);
            }
            else
            {
                for (int i = 0; i < pCurrent->getChildNodes().size(); ++i)
                {
                    Node* pChild = pCurrent->getChildNodes()[i];

                    if (bMark)
                    {
                        // Non marqu� ?
                        if (marked.find(pChild) == marked.end())
                        {
                            st.push_back(pChild);
                            marked[pChild] = true;
                        }
                    }
                    else
                    {
                        st.push_back(pChild);
                    }
                }
            }
        }
    }
};


#endif // CTREEEXPLORER_H
