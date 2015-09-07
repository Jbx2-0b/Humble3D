#ifndef CFILTEREVENT_H
#define CFILTEREVENT_H

// Lib
#include "lib_tools_global.h"

// Qt
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDebug>


class LIB_TOOLSSHARED_EXPORT CFilterEvent : public QObject
{
	Q_OBJECT

public:

    class IFilterEventListener
    {
        friend class CFilterEvent;

    protected:

        virtual void onUpdate(CFilterEvent* pFilter) = 0;
    };

	//! Constructeur
    CFilterEvent(CFilterEvent::IFilterEventListener* pListener, int iUpdateDT);

public slots:

	void filter();

protected:

    //! Listeners
    CFilterEvent::IFilterEventListener* m_pListener;

	int m_iUpdateDT;
	QTime m_LastUpdateTime;
	QTimer m_UpdateTimer;
};

#endif // CFILTEREVENT_H
