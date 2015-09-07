#ifndef CRENDERQUEUE_H
#define CRENDERQUEUE_H

// Qt
#include <QMultiHash>

// Foundations
#include "ARenderableItem.h"


class CRenderQueue
{
public:

	CRenderQueue() {}

	QList<int> materialIDs() const
	{
		return m_RenderHashMap.uniqueKeys();
	}

	QList<ARenderableItem*> getItemsByMaterialID(int iMaterialID) const
	{
		return m_RenderHashMap.values(iMaterialID);
	}

	void insert(int iMaterialID, ARenderableItem* pItem)
	{
		m_RenderHashMap.insert(iMaterialID, pItem);
	}

	void remove(ARenderableItem* item)
	{
		int materialID = m_RenderHashMap.key(item, -1);

		if (m_RenderHashMap.contains(materialID))
		{
			m_RenderHashMap.remove(materialID, item);
		}
	}

    void clear()
    {
        m_RenderHashMap.clear();
    }

private:

	QMultiHash<int, ARenderableItem*> m_RenderHashMap;
};

#endif // CRENDERQUEUE_H
