// ServiceTracker.cpp
#include "stdafx.h"
#include "ServiceTracker.h"

// this declaration allows to write KTrackerThread instead of
// KServiceTracker::KTrackerThread
using KServiceTracker::KTrackerThread;

//-------------------------------------------------
KServiceTracker::KTrackerThread::KTrackerThread(                        
                                                int nService,                      
                                                HANDLE hService,                   
                                                DWORD  TargetState,                
                                                SERVICE_STATUS const* pKnownStatus, 
                                                IServiceStatusNotifySink* pSink
                                               )
{
    ASSERT( hService != NULL );
    ASSERT( pSink != NULL );
    ASSERT( pKnownStatus != NULL );

    m_nService = nService;
    m_hService = hService;
    m_TargetState = TargetState;
    m_Status = *pKnownStatus;
    m_pSink = pSink;
    

    DWORD ThreadId;
    m_hThread = CreateThread(
                                NULL, 
                                0,
                                LPTHREAD_START_ROUTINE(StaticTrackerThread),
                                this,
                                0,
                                &ThreadId   
                            );

};

//-------------------------------------------------
KTrackerThread::~KTrackerThread()
{
    Terminate();
};

//-------------------------------------------------
void KTrackerThread::Terminate( BOOL bFromThread )
{
    CSingleLock Lock(&RightToDestroy);

    if (m_hThread)
    {
        if (!bFromThread)
            TerminateThread( m_hThread, 0 );

        CloseHandle(m_hThread);
        CloseServiceHandle(m_hService);
        m_hThread = NULL;
    };

    if (bFromThread)
        ExitThread(0);
};

//-------------------------------------------------
DWORD KTrackerThread::StaticTrackerThread( KTrackerThread* pTrackerThread )
{
    pTrackerThread->TrackerThread();
    return 0;
};

//-------------------------------------------------
void KTrackerThread::TrackerThread()
{
    m_pSink->OnServiceStatusChange( m_nService, &m_Status );

    DWORD TimeoutValue = m_Status.dwWaitHint;

    while (m_Status.dwCurrentState != m_TargetState)
    {
        DWORD OldCheckPoint = m_Status.dwCheckPoint;

        // compute wait for this time        
        DWORD WaitTime = TimeoutValue;
        if (WaitTime > 1000) { WaitTime = 1000; };
        if (WaitTime < 100) { WaitTime = 100; };

        // wait for service status to change
        Sleep(WaitTime);

        // check new service status
        if (!QueryServiceStatus(m_hService, &m_Status))
        {
            DWORD Error = GetLastError();
            m_pSink->OnServiceError( m_nService, Error );
            Terminate(TRUE);
            return;
        };

        // check whether we reached target state
        if (m_Status.dwCurrentState != m_TargetState)
        {
            // we did not reach target state
            // check whether checkpoint incremented
            if (m_Status.dwCheckPoint == OldCheckPoint)
            {
                // checkpoint was not incremented
                // check if we waited too long
                if (TimeoutValue > WaitTime)
                {
                    // there is still some time to wait
                    TimeoutValue -= WaitTime;
                }
                else
                {
                    // we waited too long - timeout occured
                    m_pSink->OnServiceTimeout( m_nService );
                    Terminate(TRUE);
                    return;
                }
            }
            else
            {
                // checkpoint was incremented
                m_pSink->OnServiceStatusChange( m_nService, &m_Status );
                TimeoutValue = m_Status.dwWaitHint;
            }
        }
    }; // end while

    // we reached target state
    ASSERT( m_Status.dwCurrentState == m_TargetState );
    m_pSink->OnServiceStatusChange( m_nService, &m_Status);
    m_pSink->OnServiceTargetState( m_nService );
    
    // terminate the thread
    {
        CSingleLock Lock(&RightToDestroy);
        CloseHandle(m_hThread);
        m_hThread = NULL;
        RightToDestroy.Unlock();
    }
};

//*************************************************
//*************************************************
KServiceTracker::KServiceTracker()
{
};

//-------------------------------------------------
KServiceTracker::~KServiceTracker()
{
    StopAll();
};

//-------------------------------------------------
void KServiceTracker::TrackService( 
                                    int nService,                      
                                    HANDLE hService,                   
                                    DWORD  TargetState,                
                                    SERVICE_STATUS const* pKnownStatus,
                                    IServiceStatusNotifySink* pSink    
                                  )
{
    RemoveTracker(nService);

    KTrackerThread* pTracker = 
        new KTrackerThread( nService, hService, TargetState, pKnownStatus, pSink );

    Map.SetAt(nService, pTracker);
};

//-------------------------------------------------
void KServiceTracker::RemoveTracker( int nService )
{
    KTrackerThread* pTracker;
    if (Map.Lookup(nService, pTracker))
    {
        pTracker->Terminate();
    };

    Map.RemoveKey(nService);
};
    
//-------------------------------------------------
void KServiceTracker::StopAll()
{
    POSITION Pos = Map.GetStartPosition();

    while (Pos)
    {
        int n_Service;
        KTrackerThread* pTracker;

        Map.GetNextAssoc(Pos, n_Service, pTracker);
        pTracker->Terminate();
    };

    Map.RemoveAll();
};