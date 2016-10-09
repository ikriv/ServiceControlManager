#ifndef __SERVICE_TRACKER_H__
#define __SERVICE_TRACKER_H__

#ifndef _WINSVC_
    #include <winsvc.h>
#endif

class CListCtrlEx;

//------------------------------------------------------------------------
// functions of this class are called by service tracker when some events
// occur on service; these functions may be called from context of different
// thread
class IServiceStatusNotifySink
{
    public:
        virtual void OnServiceStatusChange( int nService, SERVICE_STATUS const* Status ) = 0;
        virtual void OnServiceTargetState( int nService ) = 0;
        virtual void OnServiceError( int nService, DWORD ErrorCode ) = 0;
        virtual void OnServiceTimeout( int nService ) = 0;
};

//------------------------------------------------------------------------
class KServiceTracker
{
    public:
        KServiceTracker();
        virtual ~KServiceTracker();

        // starts service tracking; stops previous tracker for this service,
        // if needed
        void TrackService( 
                            int nService,                      // number of service in internal list
                            HANDLE hService,                   // opened handle of the service
                            DWORD  TargetState,                // target state for the service
                            SERVICE_STATUS const* pKnownStatus,// pointer to last known status
                            IServiceStatusNotifySink* pSink    // sink for notifications
                         );

        // remove service tracker
        void RemoveTracker( int nService );

        // stop all service trackers
        void StopAll();

    private:

        // unfortunately, usage of CMap requires definition of KTrackerThread
        // to be included here. Otherwise, we could put it into .CPP file
        class KTrackerThread
        {
            public:
                KTrackerThread(
                                int nService,                      // number of service in internal list
                                HANDLE hService,                   // opened handle of the service
                                DWORD  TargetState,                // target state for the service
                                SERVICE_STATUS const* pKnownStatus,// pointer to last known status
                                IServiceStatusNotifySink* pSink    // sink for notifications
                              );

                ~KTrackerThread();

                void Terminate() { Terminate(FALSE); };

            private:

                int                       m_nService;
                HANDLE                    m_hService;
                DWORD                     m_TargetState;
                SERVICE_STATUS            m_Status;
                IServiceStatusNotifySink* m_pSink;
                HANDLE                    m_hThread;

                static DWORD StaticTrackerThread( KTrackerThread* pTrackerThread );
                void TrackerThread();
                void Terminate(BOOL bFromThread);

                CCriticalSection RightToDestroy;
        };

        CMap<int,int,KTrackerThread*, KTrackerThread*> Map;


};

#endif