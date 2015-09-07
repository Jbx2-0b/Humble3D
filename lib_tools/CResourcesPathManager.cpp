#include "CResourcesPathManager.h"

CResourcesPathManager* CResourcesPathManager::s_pInstance = 0;
QMutex CResourcesPathManager::s_Mutex(QMutex::Recursive);