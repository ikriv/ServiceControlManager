// ServiceControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Winsvc.h"
#include "MiscUtil.h"
#include "ServiceControl.h"
#include "ServiceControlWnd.h"
#include "ServiceProperty.h"
#include "ColumnsDlg.h"
#include "ComputerNameDlg.h"
#include "ConfigServiceDlg.h"
#include "MultiString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------
// IMPLEMENTATION NOTES
// on m_pStatus and  m_pConfig pointers
//
// We use quite complicated scheme to store service information.
//
// The straightforward solution would be to declare a structure 
// that contains information on one service (e.g. KServiceInfo) and
// hold array of such structures.
//
// Although, the way Windows NT returns service information makes this
// approach too expensive.
//
// In Windows NT, information on each service is divided into two parts:
// ENUM_SERVICE_STATUS structure and QUERY_SERVICE_CONFIG structure.
//
// Correspondingly, information on services is retrieved by two system calls: 
// EnumServiceStatus() and QueryServiceConfig().
//
// EnumServicesStatus() function receives memory buffer as a parameter and
// fills it with array of ENUM_SERVICE_STATUS structures for all services
// in the system. It also returns a number of services in the system 
// (we store it in m_nServices).
//
// ENUM_SERVICE_STATUS structure contains pointers to strings 
// (service name and others), and these strings must be stored somewhere.
// EnumServicesStatus() stores these strings in the end of the same memory 
// buffer where array of ENUM_SERVICE_STATUS structures resides.
//
// Thus, after EnumSericesStatus() returns, memory buffer contains array of
// ENUM_SERVICE_STATUS[m_nServices] structures, followed by some number of
// character strings. We hold pointer to this memory buffer in m_pStatus 
// variable.
//
// If additional information on a service is required, we must 
// open service handle (OpenService()) and query service configuration
// using QueryServiceConfig() system call.
//
// QueryServiceConfig() fills supplied memory buffer with QUERY_SERVICE_CONFIG
// structure followed by character strings used by this structure.
//
// Since we must call QueryServiceConfig() for each service in the system, 
// this may take a long time, especially on remote computer. 
// 
// To deal with QueryServiceConfig(), we allocate array of pointers to
// QUERY_SERIVE_CONFIG structures and hold it in m_pConfig variable.
//
// For each service, we allocate memory buffer (512 bytes by default) and
// supply it to QueryServiceConfig() function. If 512 bytes is too small, we
// allocate larger buffer. After QueryServiceConfig() successfully returns,
// we store pointer to allocated buffer in m_pConfig[n], where n is number of
// the service.
//
// We use malloc() to allocate buffer for each service. 
// Of course, malloc() has its overhead and more effective allocation strategy 
// may be found, but number of services is usually small (200-300), so we 
// don't have performance problems with malloc.
//
// Note that we call QueryServiceConfig() only if we really need information in 
// QUERY_SERVICE_CONFIG, i.e only when that information is displayed. 
//

//---------------------------------------------------------------------
// FillServiceProperties() reads service properties from Status and Cfg
// and fills corresponding strings in Properties array
// Properties array must have SVC_LAST items.
static void FillServiceProperties( 
                                   ENUM_SERVICE_STATUS  const& Status,
                                   QUERY_SERVICE_CONFIG const* Cfg, // may be NULL
                                   CString* Properties              // array of SVC_LAST strings
                                 )
{
    for (int nProp=0; nProp<SVC_LAST; nProp++)
        Properties[nProp].Empty();

    Properties[SVC_DISPLAY_NAME]  = Status.lpDisplayName;
    Properties[SVC_INTERNAL_NAME] = Status.lpServiceName;

    // SVC_TYPE,
    // SVC_TYPE_REMARKS,
    // SVC_INTERACTIVE

    BOOL  bService = FALSE;
    DWORD Type = Status.ServiceStatus.dwServiceType;
    BOOL  bInteractive = Type & SERVICE_INTERACTIVE_PROCESS;

    Type &= ~SERVICE_INTERACTIVE_PROCESS;

    CString TypeStr;

    switch (Type)
    {
        case SERVICE_WIN32_OWN_PROCESS:   
            Properties[SVC_TYPE] = _T("Service");
            bService = TRUE;
            break;

        case SERVICE_WIN32_SHARE_PROCESS: 
            Properties[SVC_TYPE] = _T("Service");
            Properties[SVC_TYPE_REMARKS] = _T("Shared");
            bService = TRUE;
            break;

        case SERVICE_KERNEL_DRIVER:   
            Properties[SVC_TYPE] = _T("Driver"); 
            break;

        case SERVICE_FILE_SYSTEM_DRIVER:  
            Properties[SVC_TYPE] = _T("Driver"); 
            Properties[SVC_TYPE_REMARKS] = _T("FS");
            break;

        default:
            Properties[SVC_TYPE].Empty();
    };

    if (bService)
        if (bInteractive)
            Properties[SVC_INTERACTIVE] = _T("Yes");


    // SVC_STATE
    switch (Status.ServiceStatus.dwCurrentState)
    {
        case SERVICE_STOPPED:          Properties[SVC_STATE].Empty(); break;
        case SERVICE_START_PENDING:    Properties[SVC_STATE] = _T("Starting..."); break;
        case SERVICE_STOP_PENDING:     Properties[SVC_STATE] = _T("Stopping..."); break;
        case SERVICE_RUNNING:          Properties[SVC_STATE] = _T("Running"); break;
        case SERVICE_CONTINUE_PENDING: Properties[SVC_STATE] = _T("Continue pending"); break;
        case SERVICE_PAUSE_PENDING:    Properties[SVC_STATE] = _T("Pause pending"); break;
        case SERVICE_PAUSED:           Properties[SVC_STATE] = _T("Paused"); break;
    };

    if (Cfg)
    {
        // SVC_STARTUP
        switch (Cfg->dwStartType)
        {
            case SERVICE_BOOT_START:    Properties[SVC_STARTUP] = _T("Boot"); break;
            case SERVICE_SYSTEM_START:  Properties[SVC_STARTUP] = _T("System"); break;
            case SERVICE_AUTO_START:    Properties[SVC_STARTUP] = _T("Auto"); break;
            case SERVICE_DEMAND_START:  Properties[SVC_STARTUP] = _T("Manual"); break;
            case SERVICE_DISABLED:      Properties[SVC_STARTUP] = _T("Disabled"); break;
        };

        // SVC_ERROR_CTRL
        switch (Cfg->dwErrorControl)
        {
            case SERVICE_ERROR_IGNORE:   Properties[SVC_ERROR_CTRL] = _T("Ignore"); break;
            case SERVICE_ERROR_NORMAL:   Properties[SVC_ERROR_CTRL] = _T("Normal"); break;
            case SERVICE_ERROR_SEVERE:   Properties[SVC_ERROR_CTRL] = _T("Severe"); break;
            case SERVICE_ERROR_CRITICAL: Properties[SVC_ERROR_CTRL] = _T("Critical"); break;
        };

        Properties[SVC_PATH] = Cfg->lpBinaryPathName;
        Properties[SVC_LOAD_GROUP] = Cfg->lpLoadOrderGroup;
        Properties[SVC_TAG_ID].Format( _T("%u"), Cfg->dwTagId );

        // SVC_DEPENDENCIES
        {
            KMultiString Dependencies;
            Dependencies.CopyFromBuffer( Cfg->lpDependencies );
            Properties[SVC_DEPENDENCIES] = Dependencies.GetList();
        }

        if (bService)
        {
            Properties[SVC_ACCOUNT] = Cfg->lpServiceStartName;
        }
        else
        {
            Properties[SVC_DRIVER_OBJECT_NAME] = Cfg->lpServiceStartName;
        };
    };
};

//---------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CServiceControlWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CServiceControlWnd)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_COMMAND(IDC_OPTIONS_COLUMNS, OnOptionsColumns)
	ON_COMMAND(IDC_SELECT_COMPUTER, OnSelectComputer)
    ON_COMMAND(IDC_SERVICE_START, OnServiceStart)
    ON_COMMAND(IDC_SERVICE_STOP, OnServiceStop)
    ON_COMMAND(IDC_SERVICE_PAUSE, OnServicePause)
    ON_COMMAND(IDC_SERVICE_CONTINUE, OnServiceContinue)
    ON_WM_CREATE()
	ON_COMMAND(IDC_SERVICE_ADD, OnServiceAdd)
	ON_COMMAND(IDC_SERVICE_CONFIGURE, OnServiceConfigure)
    ON_COMMAND(IDC_SERVICE_REMOVE, OnServiceRemove)
	ON_WM_SETFOCUS()
	ON_COMMAND(IDC_HELP_ABOUT, OnHelpAbout)
    ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_NOTIFY( NM_DBLCLK, AFX_IDW_PANE_FIRST, OnServiceDblClk )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServiceControlWnd message handlers
static CString GetComputerName()
{
    CString S;
    DWORD Size = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName( S.GetBufferSetLength(Size), &Size );
    S.ReleaseBuffer();
    return S;
};

//---------------------------------------------------------------------
CServiceControlWnd::CServiceControlWnd() :
m_pStatus(NULL),
m_pConfig(NULL),
m_nServices(0),
m_Computer( GetComputerName() )
{
    LoadRegistry();
};

//---------------------------------------------------------------------
CServiceControlWnd::~CServiceControlWnd()
{
    FreeStatusAndConfigInfo();
    SaveRegistry();
};

//---------------------------------------------------------------------
int CServiceControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    m_Toolbar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS );
    m_Toolbar.LoadToolBar(IDR_MAINFRAME);	

    CRect R(0,0,0,0);
    DWORD const Style = WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS;
    m_Services.Create( Style, R, this, (UINT)AFX_IDW_PANE_FIRST );
    
    // set 3D look to the list control and remove it from frame
    m_Services.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0 );
    ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED); 

    SetCaption();
    InitListControl();
    PopulateListControl();

	return 0;
}

//---------------------------------------------------------------------
void CServiceControlWnd::InitListControl()
{
    CRect R;
    m_Services.GetClientRect(&R);

/*
    int Width = R.Width();
    Width -= GetSystemMetrics( SM_CXVSCROLL );
    Width -= COL_LAST; // borders between columns

    if (Width < 0) Width = 0;
*/
    // init list box
    int i;
    int const nColumns = m_Columns.GetSize();
    for (i=0; i<nColumns; i++)
    {
        int ColType = m_Columns[i];
        m_Services.InsertColumn( i, PropertyName[ColType], LVCFMT_LEFT, m_Widths[ColType] );
    };

//    m_Services.SetColumnTypes( SortTypes, COL_LAST );
    m_Services.EnableSort();
//    m_Services.EnableHeaderDrag();
    m_Services.SetHighlightType( HIGHLIGHT_ROW );
};

//---------------------------------------------------------------------
ENUM_SERVICE_STATUS* CServiceControlWnd::AllocStatusInfo( DWORD nBytesNeeded )
{
    // see implementation notes in the beginning of this file
    return (ENUM_SERVICE_STATUS*)malloc(nBytesNeeded);
};

//---------------------------------------------------------------------
void CServiceControlWnd::FreeStatusInfo( ENUM_SERVICE_STATUS* pStatus )
{
    if (pStatus)
        free( pStatus );
};

//---------------------------------------------------------------------
LPQUERY_SERVICE_CONFIG* CServiceControlWnd::AllocConfigInfo( DWORD nServices )
{
    // use malloc() uniformity
    // we could use operator new here, but we use malloc() in all other places
    DWORD const Size = nServices*sizeof(LPQUERY_SERVICE_CONFIG);
    LPQUERY_SERVICE_CONFIG* pConfig = (LPQUERY_SERVICE_CONFIG*)malloc(Size);
    ZeroMemory( pConfig, Size );

    return pConfig;
};

//---------------------------------------------------------------------
void CServiceControlWnd::FreeConfigInfo( LPQUERY_SERVICE_CONFIG* pConfig, DWORD nServices )
{
    if (pConfig)
    {
        for (unsigned i=0; i<nServices; i++)
        {
            if (pConfig[i])
            {
                free(pConfig[i]);
            }
        };
        free(pConfig);
    };
};

//---------------------------------------------------------------------
void CServiceControlWnd::FreeStatusAndConfigInfo()
{
    FreeConfigInfo(m_pConfig, m_nServices);
    FreeStatusInfo(m_pStatus);

    m_nServices = 0;
    m_pConfig = NULL;
    m_pStatus = NULL;
};

//---------------------------------------------------------------------
static BOOL QueryServiceConfigRequired( CArray<int,int>& Columns )
{
    int Sz = Columns.GetSize();

    for (int i=0; i<Sz; i++)
        if (!Property_Returned_By_EnumServicesStatus[Columns[i]])
            return TRUE;

    return FALSE;
};

//---------------------------------------------------------------------
void CServiceControlWnd::PopulateListControl()
{
    CSingleLock Lock(&m_ListCtrlAccess);

    m_ServiceTracker.StopAll();
    FreeStatusAndConfigInfo();

    m_Services.DeleteAllItems();

    KServiceHandle hSCManager = OpenSCManager( 
                                                m_Computer,
                                                NULL,
                                                SC_MANAGER_ENUMERATE_SERVICE
                                             );

    if (!hSCManager)
    {
        DWORD Error = GetLastError();
        CString Msg;
        Msg.Format( 
                     _T("Cannot open service control manager on \\\\%s.\n%s"), 
                     m_Computer, 
                     GetSystemMessage(Error)
                  );

        MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return;
    };

    DWORD nBytesNeeded;
    DWORD ResumeHandle = 0;

    // get number of records
    BOOL Result = EnumServicesStatus( 
                                        hSCManager,
                                        SERVICE_WIN32 | SERVICE_DRIVER,
                                        SERVICE_ACTIVE | SERVICE_INACTIVE,
                                        NULL,
                                        0,
                                        &nBytesNeeded,
                                        &m_nServices,
                                        &ResumeHandle
                                    );

    if (Result) 
        // result must be FALSE at this time
        return;

    {
        DWORD Error = GetLastError();
        if (Error != ERROR_MORE_DATA)
        {
            CString Msg = _T("EnumServicesStatus() failed.\n") + GetSystemMessage(Error);
            MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
            return;
        }
    };

    DWORD nBytesAllocated = nBytesNeeded;
    m_pStatus = AllocStatusInfo( nBytesAllocated );

    Result = EnumServicesStatus(
                                  hSCManager,
                                  SERVICE_WIN32 | SERVICE_DRIVER,
                                  SERVICE_ACTIVE | SERVICE_INACTIVE,
                                  m_pStatus,
                                  nBytesAllocated,
                                  &nBytesNeeded,
                                  &m_nServices,
                                  &ResumeHandle
                               );

    if (!Result)
    {
        delete[] m_pStatus; m_pStatus = NULL;
        DWORD Error = GetLastError();
        CString Msg = _T("EnumServicesStatus() failed.\n") + GetSystemMessage(Error);
        MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return;
    };

    //---------------------------------------------------------------
    // now query service configuration for each service and fill in service info

    m_pConfig = AllocConfigInfo(m_nServices);

    CString Properties[ SVC_LAST ];
    int const nColumns = m_Columns.GetSize();

    BOOL bCallQueryServiceConfig = QueryServiceConfigRequired(m_Columns);

    for (unsigned i=0; i<m_nServices; i++)
    {

        if (bCallQueryServiceConfig)
        {

            // the handle will be closed automatically in destructor
            KServiceHandle hService = OpenService( 
                                                    hSCManager,
                                                    m_pStatus[i].lpServiceName,
                                                    SERVICE_QUERY_CONFIG
                                                 );

            if (hService)
            {
                DWORD nBytesNeeded;
                DWORD BufSize = 512; // default buffer size
                m_pConfig[i] = (QUERY_SERVICE_CONFIG*)malloc( BufSize );

                BOOL bSuccess;

                bSuccess = QueryServiceConfig( hService, m_pConfig[i], BufSize, &nBytesNeeded );

                if (!bSuccess)
                {
                    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                    {
                        // not enough memory - re-allocate it according to demands
                        DWORD BufSize = nBytesNeeded;
                        free(m_pConfig[i]);
                        m_pConfig[i] = (QUERY_SERVICE_CONFIG*)malloc( BufSize );

                        bSuccess = QueryServiceConfig( hService, m_pConfig[i], BufSize, &nBytesNeeded );
                    }
                };

                if (!bSuccess)
                {
                    free( m_pConfig[i] );
                    m_pConfig[i] = NULL;
                }
            }
        };

        FillServiceProperties( m_pStatus[i], m_pConfig[i], Properties );

        int Idx = m_Services.InsertItem( i, Properties[m_Columns[0]] );
        
        // set service number as item data
        m_Services.SetItemData( Idx, i );

        for (int Col = 1; Col<nColumns; Col++)
        {
            m_Services.SetItem( Idx, Col, LVIF_TEXT, Properties[m_Columns[Col]], 0,0,0,0 );
        }
    }

    int nColumn = m_Services.GetSortedColumn();
    BOOL bAscending = m_Services.IsSortAscending();

    if (nColumn == -1)
    {
        nColumn = 0;
        bAscending = TRUE;
    };

    m_Services.SortByColumn( nColumn, bAscending);
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnRefresh() 
{
    // PopulateListControl() also acquired this lock:
    // recursive locks on critical sections are allowed
    CSingleLock Lock(&m_ListCtrlAccess);

    CWaitCursor Wait;
    m_Services.SetRedraw(FALSE);
    
    // retrieve internal name of currently selected service, if any
    CString CurrentServiceName;

    {
        int nService = GetCurrentService(FALSE);

        if (nService != -1)
            CurrentServiceName = m_pStatus[nService].lpServiceName;
    }

    PopulateListControl();

    // search for service name in the array of retrieved services
    // since number of services is rarely more than 200, we use simple 
    // linear search
    int nCurrentService = -1;

    for (int i=0; i<(int)m_nServices; ++i)
    {
        if (CurrentServiceName.CompareNoCase(m_pStatus[i].lpServiceName) == 0)
        {
            nCurrentService = i;
            break;
        }
    };

    if (nCurrentService != -1)
    {
        int nItem = GetServiceNumberOnScreen(nCurrentService);
        if (nItem != -1)
        {
            m_Services.EnsureVisible(nItem, FALSE);
            m_Services.SetItemState( 
                                     nItem, 
                                     LVIS_FOCUSED | LVIS_SELECTED, 
                                     LVIS_FOCUSED | LVIS_SELECTED
                                   );
        }
    };

    m_Services.SetRedraw(TRUE);
    m_Services.UpdateWindow();
}

//---------------------------------------------------------------------
void CServiceControlWnd::OnOptionsColumns() 
{
    // PopulateListControl() also acquired this lock:
    // recursive locks on critical sections are allowed
    CSingleLock Lock( &m_ListCtrlAccess ); 

    CColumnsDlg Dlg(this);
    Dlg.DisplayedArr.Copy(m_Columns);

    if (Dlg.DoModal() == IDOK)
    {
        int nColumns = m_Columns.GetSize();
        
        m_Services.SetRedraw(FALSE);
        m_Services.DeleteAllItems();

        for (int i=0; i<nColumns; i++)
            m_Services.DeleteColumn(0);

        m_Columns.RemoveAll();
        m_Columns.Copy(Dlg.DisplayedArr);
        InitListControl();
        PopulateListControl();
        m_Services.SetRedraw(TRUE);
        m_Services.Invalidate();
        m_Services.UpdateWindow();
    }
}

//---------------------------------------------------------------------
void CServiceControlWnd::SetCaption()
{
    CString Caption;
    Caption.Format( _T("Service Control Manager on \\\\%s"), m_Computer );
    SetWindowText(Caption);
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnSelectComputer() 
{
    CComputerNameDlg Dlg(this);
    Dlg.m_Computer = m_Computer;
    if (Dlg.DoModal() == IDOK)
    {
        m_Computer = Dlg.m_Computer;
        if (m_Computer.IsEmpty() || (m_Computer == _T(".")))
            m_Computer = GetComputerName();

        SetCaption();
        OnRefresh();
    };
}

//---------------------------------------------------------------------
int CServiceControlWnd::GetCurrentService( BOOL Beep_If_Not_Found )
{
    CSingleLock Lock(&m_ListCtrlAccess);

    // retrieve currently selected item
    int nCurrentIdx = m_Services.GetNextItem(-1, LVNI_SELECTED);

    if (nCurrentIdx == -1)
    {
        // nothing was selected
        if (Beep_If_Not_Found)
            MessageBeep( MB_ICONASTERISK );
        return -1;
    };

    // retrieve number of service that corresponds to selected item
    DWORD nService = m_Services.GetItemData(nCurrentIdx);
    ASSERT( nService >= 0 );
    ASSERT( nService < m_nServices );

    return nService;
};
    
//---------------------------------------------------------------------
HANDLE CServiceControlWnd::OpenServiceByName( LPCTSTR Name, DWORD Rights )
{
    Rights |= SERVICE_QUERY_STATUS;

    // try to open service control manager
    KServiceHandle hSCManager = OpenSCManager( 
                                                m_Computer,
                                                NULL,
                                                0 // no special rights required
                                             );

    if (!hSCManager)
    {
        DWORD Error = GetLastError();
        CString Msg;
        Msg.Format( 
                     _T("Cannot open service control manager on \\\\%s.\n%s"), 
                     m_Computer, 
                     GetSystemMessage(Error)
                  );

        MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return NULL;
    };

    // try to open requested service
    HANDLE hService = OpenService(
                                    hSCManager,
                                    Name,
                                    Rights
                                 );

    if (!hService)
    {
        DWORD Error = GetLastError();

        CString Msg;
        Msg.Format( 
                     _T("Cannot open service %s on \\\\%s.\n%s"), 
                     Name,
                     m_Computer, 
                     GetSystemMessage(Error)
                  );

        MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return NULL;
    };


    return hService;
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceStart()
{
    CSingleLock Lock(&m_ListCtrlAccess);

    int nService = GetCurrentService();
    if (nService == -1) 
        return;

    CString ServiceName = m_pStatus[nService].lpServiceName;
    HANDLE  hService = OpenServiceByName(ServiceName, SERVICE_START);

    if (!hService)
        return;

    BOOL Result;
    DWORD Error = 0;

    // display wait cursor and try to start service
    {
        CWaitCursor Wait;
        Result = StartService(hService, 0, NULL);

        // we must retrieve error here, because destructor of CWaitCursor
        // will clear the error code
        if (!Result)
            Error = GetLastError(); 
    };

    if (Result)
    {
        SERVICE_STATUS Status = m_pStatus[nService].ServiceStatus;
        Status.dwCurrentState = SERVICE_START_PENDING;
        Status.dwWaitHint = 2000;
        m_ServiceTracker.TrackService( nService, hService, SERVICE_RUNNING, &Status, this );
    }
    else
    {
        CloseServiceHandle( hService );
        CString Msg;
        Msg.Format( 
                     _T("Cannot start service %s on \\\\%s.\n%s"), 
                     ServiceName,
                     m_Computer, 
                     GetSystemMessage(Error)
                  );

        MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
    }
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceControl( DWORD ControlCode )
{
    CSingleLock Lock(&m_ListCtrlAccess);

    int nService = GetCurrentService();
    if (nService == -1) 
        return;

    DWORD Rights = 0;
    DWORD TargetState;
    CString OpName;

    switch (ControlCode)
    {
        case SERVICE_CONTROL_STOP:
            Rights = SERVICE_STOP;
            TargetState = SERVICE_STOPPED;
            OpName = "stop";
            break;

        case SERVICE_CONTROL_PAUSE:
            Rights = SERVICE_PAUSE_CONTINUE;
            TargetState = SERVICE_PAUSED;
            OpName = "pause";
            break;

        case SERVICE_CONTROL_CONTINUE:
            Rights = SERVICE_PAUSE_CONTINUE;
            TargetState = SERVICE_RUNNING;
            OpName = "continue";
            break;

        default:
            ASSERT(FALSE); // operation not supported
    };
            

    CString ServiceName = m_pStatus[nService].lpServiceName;
    HANDLE  hService = OpenServiceByName(ServiceName, Rights);

    if (!hService)
        return;

    BOOL Result;
    DWORD Error = 0;
    SERVICE_STATUS Status;

    // display wait cursor and try to stop service
    {
        CWaitCursor Wait;
        Result = ControlService(hService, ControlCode, &Status);

        // we must retrieve error here, because destructor of CWaitCursor
        // will clear the error code
        if (!Result)
            Error = GetLastError(); 
    };

    if (Result)
    {
        m_ServiceTracker.TrackService( nService, hService, TargetState, &Status, this );
    }
    else
    {
        CloseServiceHandle(hService);
        CString Msg;
        Msg.Format( 
                     _T("Cannot %s service %s on \\\\%s.\n%s"), 
                     OpName,
                     ServiceName,
                     m_Computer, 
                     GetSystemMessage(Error)
                  );

        MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
    }
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceStop()
{
    OnServiceControl(SERVICE_CONTROL_STOP);
};


//---------------------------------------------------------------------
void CServiceControlWnd::OnServicePause()
{
    OnServiceControl(SERVICE_CONTROL_PAUSE);
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceContinue()
{
    OnServiceControl(SERVICE_CONTROL_CONTINUE);
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceStatusChange( int nService, SERVICE_STATUS const* Status )
{
    // this function is called from context of tracker thread
    CSingleLock Lock(&m_ListCtrlAccess);
     
    m_pStatus[nService].ServiceStatus = *Status;
    CString Properties[SVC_LAST];
    FillServiceProperties( m_pStatus[nService], m_pConfig[nService], Properties );

    int nItem = GetServiceNumberOnScreen(nService);
    int nColumns = m_Columns.GetSize();
    for (int Col = 1; Col<nColumns; Col++)
    {
        m_Services.SetItem( nItem, Col, LVIF_TEXT, Properties[m_Columns[Col]], 0,0,0,0 );
    }
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceTargetState( int nService )
{
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceError( int nService, DWORD ErrorCode )
{
    CSingleLock Lock(&m_ListCtrlAccess);

    CString Msg;
    Msg.Format( 
                 _T("An error occured while tracking status of service %s on \\\\%s.\n%s"), 
                 m_pStatus[nService].lpServiceName,
                 m_Computer, 
                 GetSystemMessage(ErrorCode)
              );

    MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceTimeout( int nService )
{
    CSingleLock Lock(&m_ListCtrlAccess);

    CString Msg;
    Msg.Format( 
                 _T("Operation with service %s on \\\\%s timed out."), 
                 m_pStatus[nService].lpServiceName,
                 m_Computer
              );

    MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceAdd() 
{
    CConfigServiceDlg Dlg(this, FALSE, m_Computer, NULL);

    if (Dlg.DoModal() == IDOK)
        OnRefresh();
}

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceConfigure() 
{
    CSingleLock Lock(&m_ListCtrlAccess);

    int nService = GetCurrentService();
    if (nService == -1) 
        return;

    CConfigServiceDlg Dlg(this, TRUE, m_Computer, m_pStatus[nService].lpServiceName);

    if (Dlg.DoModal() == IDOK)
        OnRefresh();
}

//---------------------------------------------------------------------
int CServiceControlWnd::GetServiceNumberOnScreen( int nService )
{
    LV_FINDINFO FindInfo;
    FindInfo.flags = LVFI_PARAM; // search by lParam
    FindInfo.psz = NULL;
    FindInfo.lParam = nService;

    int nItem = m_Services.FindItem( &FindInfo );

    return nItem;
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceRemove()
{
    CSingleLock Lock(&m_ListCtrlAccess);

    int nService = GetCurrentService();
    if (nService == -1) 
        return;

    CString ServiceName = m_pStatus[nService].lpServiceName;
    
    // service handle will be closed in the end of the block
    {
        KServiceHandle hService = OpenServiceByName(ServiceName, DELETE);

        if (!hService)
            return;

        // ask user's confirmation
        {
            CString Msg;
            Msg.Format( 
                        _T("Are you sure you want to remove service %s on \\\\%s?"),    
                        ServiceName,
                        m_Computer
                      );

            if (MessageBox( Msg, NULL, MB_ICONQUESTION | MB_YESNO ) != IDYES)
                return;
        };

        CWaitCursor Wait;
        BOOL bResult = DeleteService( hService );

        if (!bResult)
        {
            DWORD Error = GetLastError();

            CString Msg;
            Msg.Format( 
                         _T("Cannot remove service %s on \\\\%s.\n%s"), 
                         ServiceName,
                         m_Computer, 
                         GetSystemMessage(Error)
                      );

            MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
            return;
        };
    } // handle to deleted service closed here

    OnRefresh();
};

//---------------------------------------------------------------------
void CServiceControlWnd::OnSetFocus(CWnd* pOldWnd) 
{
	if (IsWindow(m_Services))
    {
        m_Services.SetFocus();
    }
    else
    {
        CFrameWnd::OnSetFocus(pOldWnd);
    }
}

//---------------------------------------------------------------------
void CServiceControlWnd::OnServiceDblClk( NMHDR *pNotifyStruct, LRESULT *result )
{
    OnServiceConfigure();
    *result = 0;
}

//---------------------------------------------------------------------
void CServiceControlWnd::OnHelpAbout() 
{
    CDialog Dlg( IDD_ABOUT_BOX, this );
    Dlg.DoModal();
}

//---------------------------------------------------------------------
void CServiceControlWnd::OnDestroy()
{
    // main window is destroyed before children
    int nColumns = m_Columns.GetSize();
    for (int i=0; i<nColumns; ++i)
        m_Widths[ m_Columns[i] ] = m_Services.GetColumnWidth(i);
};
