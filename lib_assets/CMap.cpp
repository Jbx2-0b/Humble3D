#include "CMap.h"

const CMapKey CMap::NullKey = CMapKey(-1, -1);

//-----------------------------------------------------------------------------------------
CMap::CMap() 
    : m_Key(NullKey)
    , m_iPolygonCount(0)
{}

//-----------------------------------------------------------------------------------------
CMap::CMap(const CMapKey& key)
    : m_Key(key)
    , m_iPolygonCount(0)
{}

