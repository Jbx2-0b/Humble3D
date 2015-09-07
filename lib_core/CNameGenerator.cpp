#include "CNameGenerator.h"
#include "CLogManager.h"
#include <QDebug>

//-----------------------------------------------------------------------------------------
CNameGenerator::CNameGenerator()
: m_Mutex(QMutex::Recursive)
{}

//-----------------------------------------------------------------------------------------
CNameGenerator::~CNameGenerator()
{
}

//-----------------------------------------------------------------------------------------
QString CNameGenerator::generateName(const QString& prefix)
{
    QMutexLocker locker(&m_Mutex);

    if (m_NameCount.count(prefix) == 0)
    {
        m_NameCount[prefix] = 0;
        return prefix;
    }

    return QString("%1_%2").arg(prefix).arg(m_NameCount[prefix]++);
}


