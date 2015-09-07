#include "CFilterEvent.h"

//-----------------------------------------------------------------------
CFilterEvent::CFilterEvent(CFilterEvent::IFilterEventListener* pListener, int iUpdateDT)
	: m_pListener(pListener)
    , m_iUpdateDT(iUpdateDT)
{
	connect(&m_UpdateTimer, SIGNAL(timeout()), this, SLOT(filter()));
	m_LastUpdateTime.start();
}

//-----------------------------------------------------------------------
void CFilterEvent::filter()
{
	if (m_LastUpdateTime.elapsed() > m_iUpdateDT)
	{
		m_UpdateTimer.stop();
        m_pListener->onUpdate(this);
		m_LastUpdateTime.start();
	}
	else
	{
		m_UpdateTimer.start(qMax(m_iUpdateDT - m_LastUpdateTime.elapsed(), 0));
	}
}
