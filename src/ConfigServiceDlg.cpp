// ConfigServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiscUtil.h"
#include "MultiString.h"
#include "ServiceControl.h"
#include "ConfigServiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString const DEFAULT_PASSWORD = "**********";

/////////////////////////////////////////////////////////////////////////////
// CConfigServiceDlg dialog


CConfigServiceDlg::CConfigServiceDlg(
                                        CWnd* pParent, 
                                        BOOL bConfigure, 
                                        LPCTSTR ComputerName, 
                                        LPCTSTR ServiceName 
                                    )
	: CDialog(CConfigServiceDlg::IDD, pParent)
{
	m_bConfigure = bConfigure;
    m_ComputerName = ComputerName;

    //{{AFX_DATA_INIT(CConfigServiceDlg)
	m_ServiceName = ServiceName;
	m_DisplayName = _T("");
	m_Path = _T("");
	m_ServiceType = -1;
	m_Startup = -1;
	m_ErrorControl = -1;
	m_Interactive = FALSE;
	m_GroupName = _T("");
	m_DependsOn = _T("");
	m_Account = _T("");
	m_Password = _T("");
	m_ConfirmPassword = _T("");
	//}}AFX_DATA_INIT

}


void CConfigServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigServiceDlg)
	DDX_Text(pDX, IDC_SERVICE_NAME, m_ServiceName);
	DDX_Text(pDX, IDC_DISPLAY_NAME, m_DisplayName);
	DDX_Text(pDX, IDC_EXE_PATH, m_Path);
	DDX_Radio(pDX, IDC_TYPE_OWN, m_ServiceType);
	DDX_Radio(pDX, IDC_START_BOOT, m_Startup);
	DDX_Radio(pDX, IDC_ERRORS_IGNORE, m_ErrorControl);
	DDX_Check(pDX, IDC_TYPE_INTERACTIVE, m_Interactive);
	DDX_Text(pDX, IDC_GROUP, m_GroupName);
	DDX_Text(pDX, IDC_DEPENDENCIES, m_DependsOn);
	DDX_Text(pDX, IDC_ACCOUNT, m_Account);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_CONFIRM_PASSWORD, m_ConfirmPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigServiceDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigServiceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigServiceDlg message handlers

BOOL CConfigServiceDlg::OnInitDialog() 
{
    // if configuring existing service, fill dialog data
    if (m_bConfigure)
    {
        if (!FillDialogFromServiceData())
            return FALSE;
    };

	CDialog::OnInitDialog();

    if (m_bConfigure)
    {
        CString Msg;
        Msg.Format(_T("Configure service %s on \\\\%s"), m_ServiceName, m_ComputerName );
        SetWindowText(Msg);

        SetDlgItemText( IDOK, _T("&Configure") );
        GetDlgItem(IDC_SERVICE_NAME)->SendMessage( EM_SETREADONLY, 1 );
    }
    else
    {
        CString Msg;
        Msg.Format(_T("Install new service on \\\\%s"), m_ComputerName);
        SetWindowText(Msg);

        SetDlgItemText( IDOK, _T("&Install") );
    };
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//--------------------------------------------------------------------
void CConfigServiceDlg::OnOK() 
{
    // save old account in special variable
    CString OldAccount = m_Account;

    UpdateData(TRUE);

    //********************************************************
    //     determine service characteristics from dialog
    //********************************************************

    //----------------- service type
    DWORD dwServiceType;
    switch (m_ServiceType)
    {
        case 0: dwServiceType = SERVICE_WIN32_OWN_PROCESS; break;
        case 1: dwServiceType = SERVICE_WIN32_SHARE_PROCESS; break;
        case 2: dwServiceType = SERVICE_KERNEL_DRIVER; break;
        case 3: dwServiceType = SERVICE_FILE_SYSTEM_DRIVER; break;
        default:
            MessageBox( _T("Please specify service type"), NULL, MB_ICONSTOP | MB_OK );
            return;
    };

    if (m_Interactive)
        dwServiceType |= SERVICE_INTERACTIVE_PROCESS;

    //----------------- start type
    DWORD dwStartType;
    switch (m_Startup)
    {
        case 0: dwStartType =  SERVICE_BOOT_START; break;
        case 1: dwStartType =  SERVICE_SYSTEM_START; break;
        case 2: dwStartType =  SERVICE_AUTO_START; break;
        case 3: dwStartType =  SERVICE_DEMAND_START; break;
        case 4: dwStartType =  SERVICE_DISABLED; break;
        default:
            MessageBox( _T("Please specify service start type"), NULL, MB_ICONSTOP | MB_OK );
            return;
    };

    //----------------- error control
    DWORD dwErrorControl;

    switch (m_ErrorControl)
    {
        case 0: dwErrorControl = SERVICE_ERROR_IGNORE; break;
        case 1: dwErrorControl = SERVICE_ERROR_NORMAL; break;
        case 2: dwErrorControl = SERVICE_ERROR_SEVERE; break;
        case 3: dwErrorControl = SERVICE_ERROR_CRITICAL; break;
        default:
            MessageBox( _T("Please specify error control type"), NULL, MB_ICONSTOP | MB_OK );
            return;
    };

    //----------------- service name
    if (m_ServiceName.IsEmpty())
    {
        GotoDlgCtrl( GetDlgItem(IDC_SERVICE_NAME) );
        MessageBox( _T("Please enter service name"), NULL, MB_ICONSTOP | MB_OK );
        return;
    };

    //----------------- display name
    if (m_DisplayName.IsEmpty())
    {
        GotoDlgCtrl( GetDlgItem(IDC_DISPLAY_NAME) );
        MessageBox( _T("Please enter service display name"), NULL, MB_ICONSTOP | MB_OK );
        return;
    };

    //----------------- executable path
    LPCTSTR Path = m_Path;

    {
        // check executable path only if installing new service;
        // some system services have empty executable path, and we must let them
        // to be configured
        if (!m_bConfigure && m_Path.IsEmpty())
        {
            GotoDlgCtrl( GetDlgItem(IDC_EXE_PATH) );
            MessageBox( _T("Please enter executable file name"), NULL, MB_ICONSTOP | MB_OK );
            return;
        };

        // Path points to m_Path, or is NULL if m_Path is empty
        if (m_Path.IsEmpty())
            Path = NULL;
    } // end of checking path

    //----------------- account
    BOOL const bAccountChanged = (m_Account != OldAccount);

    LPCTSTR Account;

    // empty account corresponds to local system account
    if (m_Account.IsEmpty())
        m_Account = _T(".\\LocalSystem");
        
    if (!bAccountChanged)
    {
        // account has not changed
        Account = NULL;
    }
    else
    {
        // if account does not contain backslash, add .\ for built-in domain
        if (m_Account.Find(_T('\\')) == -1)
            m_Account = _T(".\\") + m_Account;

        Account = m_Account;
    };

    //----------------- password
    if (m_Password != m_ConfirmPassword)
    {
        GotoDlgCtrl( GetDlgItem(IDC_PASSWORD) );
        MessageBox( 
                    _T("Password and confirmed password do not match. Please re-enter password"), 
                    NULL, 
                    MB_ICONSTOP | MB_OK 
                  );
        return;
    };

    LPCTSTR Password = m_Password;

    if (m_Password == DEFAULT_PASSWORD)
    {
        if (Account != NULL)
        {
            // otherwise ChangeServiceConfig may fail
            // incorrectly stating "account does not exist"            
            Password = _T(""); 
        }
        else
            Password = NULL;
    }

    //----------------- dependencies 
    CString DependsOn; // contains array of null-terminated strings terminated by 
                       // two null characters

    {
        // convert Dependencies from semicolon-separated list stroed in the dialog
        KMultiString Dep;
        Dep.CopyFromList(m_DependsOn); // convert from semicolon-separated list
        DependsOn = Dep.GetBuffer();
    }

    //********************************************************
    //          try to open service control manager
    //********************************************************

    DWORD dwScmAccess = 0;
    if (!m_bConfigure)
        dwScmAccess |= SC_MANAGER_CREATE_SERVICE;

    KServiceHandle hSCManager = OpenSCManager( m_ComputerName, NULL, dwScmAccess );

    if (!hSCManager)
    {
        DWORD Error = GetLastError();

        CString Msg;
        Msg.Format( 
                     _T("Cannot open service control manager on \\\\%s.\n%s"), 
                     m_ComputerName, 
                     GetSystemMessage(Error)
                  );

        MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return;
    };

    // display wait cursor:
    // updating configuration make take some time, especially with remote computers
    CWaitCursor Wait; 

    //********************************************************
    //              install or configure service
    //********************************************************

    if (m_bConfigure)
    {
        // try to open service handle
        KServiceHandle hService = OpenService(hSCManager, m_ServiceName, SERVICE_CHANGE_CONFIG);

        if (!hService)
        {
            DWORD Error = GetLastError();

            CString Msg;
            Msg.Format( 
                         _T("Cannot open service %s on \\\\%s.\n%s"), 
                         m_ServiceName,
                         m_ComputerName, 
                         GetSystemMessage(Error)
                      );

            MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
            return;
        };

        // try to change service configuration
        //
        // NOTE: if account information is changed together with other service
        // characteristics, ChangeServiceConfig may sometimes cause access violation
        // in Services.exe killing Service Control Manager. 
        //
        // This might be a bug somewhere in implementation of ChangeServiceConfig()
        // To eliminate this problem, we change account information in separate call
        // to ChangeServiceConfig(), if needed.
        //
        // I wrote and debugged this program on original Windows NT 4.0 Workstation 
        // without service packs, so this problem might be fixed in future releases
        BOOL bResult = ChangeServiceConfig(
                                            hService,
                                            dwServiceType,
                                            dwStartType,
                                            dwErrorControl,
                                            Path,           // m_Path or NULL if m_Path is empty
                                            m_GroupName,    
                                            NULL,           // tagID: not supported, remains unchanged
                                            DependsOn,      // service dependencies
                                            NULL,           // service account or driver object name
                                            NULL,           // password: remains unchanged if NULL
                                            m_DisplayName
                                          );

        if (!bResult)
        {
            DWORD Error = GetLastError();

            CString Msg;
            Msg.Format( 
                         _T("Cannot change service configuration for %s on \\\\%s.\n%s"), 
                         m_ServiceName,
                         m_ComputerName, 
                         GetSystemMessage(Error)
                      );

            MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
            return;
        };

        if ((Account != NULL) || (Password != NULL))
        {
            // Set account information in separate call to ChangeServiceConfig()
            // this call deals ONLY with account information and does not change
            // anything else
            //
            // There is a little bug here as well: if existing account is specified
            // with NULL password, it incorrectly reports that account does not exist
            // I wrote and debugged this program on original Windows NT 4.0 Workstation 
            // without service packs, so this problem might be fixed in future releases
            bResult = ChangeServiceConfig(
                                            hService,
                                            SERVICE_NO_CHANGE, // service type
                                            SERVICE_NO_CHANGE, // start type
                                            SERVICE_NO_CHANGE, // error control
                                            NULL,              // path - not changed
                                            NULL,              // group - not changed
                                            NULL,              // tagID - not changed
                                            NULL,              // service dependencies - not changed
                                            Account,           // service account or driver object name
                                            Password,          // password; ignored for drivers
                                            NULL               // display name - not changed
                                          );

            if (!bResult)
            {
                DWORD Error = GetLastError();

                CString Msg;
                Msg.Format( 
                             _T("Cannot change account information for %s on \\\\%s.\n%s"), 
                             m_ServiceName,
                             m_ComputerName, 
                             GetSystemMessage(Error)
                          );

                MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
                return;
            }
        }

    }
    else
    {
        // installing new service
        //
        // NOTE: unlike ChangeServiceConfig, here I did not see problems 
        // with specifying all the information in one call
        KServiceHandle hService = 
            CreateService(
                            hSCManager,     // handle to service control manager database  
                            m_ServiceName,  // pointer to name of service to start 
                            m_DisplayName,  // pointer to display name 
                            0,              // type of access to service: no access required 
                            dwServiceType,  // type of service 
                            dwStartType,    // when to start service 
                            dwErrorControl, // severity if service fails to start 
                            m_Path,         // pointer to name of binary file 
                            m_GroupName,    // pointer to name of load ordering group 
                            NULL,           // pointer to variable to get tag identifier 
                            DependsOn,      // pointer to array of dependency names 
                            Account,        // pointer to account name of service 
                            Password        // pointer to password for service account 
                        );  

        if (!hService)
        {
            DWORD Error = GetLastError();

            CString Msg;
            Msg.Format( 
                         _T("Cannot create service %s on \\\\%s.\n%s"), 
                         m_ServiceName,
                         m_ComputerName, 
                         GetSystemMessage(Error)
                      );

            MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
            return;
        }
    };

	// close the dialog
    CDialog::OnOK();
}

//--------------------------------------------------------------------
void CConfigServiceDlg::OnCancel() 
{
	CDialog::OnCancel();
}

//--------------------------------------------------------------------
BOOL CConfigServiceDlg::FillDialogFromServiceData()
{
    KServiceHandle hSCManager = OpenSCManager( m_ComputerName, NULL, 0 );

    if (!hSCManager)
    {
        DWORD Error = GetLastError();

        CString Msg;
        Msg.Format( 
                     _T("Cannot open service control manager on \\\\%s.\n%s"), 
                     m_ComputerName, 
                     GetSystemMessage(Error)
                  );

        GetParent()->MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return FALSE;
    };

    KServiceHandle hService = OpenService(hSCManager, m_ServiceName, SERVICE_QUERY_CONFIG);

    if (!hService)
    {
        DWORD Error = GetLastError();

        CString Msg;
        Msg.Format( 
                     _T("Cannot open service %s on \\\\%s.\n%s"), 
                     m_ServiceName,
                     m_ComputerName, 
                     GetSystemMessage(Error)
                  );

        GetParent()->MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return FALSE;
    };

    //-----------------------------------------
    // query service configuration
    DWORD nBytesNeeded;
    DWORD BufSize = 512; // default buffer size
    QUERY_SERVICE_CONFIG* pConfig = (QUERY_SERVICE_CONFIG*)malloc( BufSize );

    BOOL bSuccess;

    bSuccess = QueryServiceConfig( hService, pConfig, BufSize, &nBytesNeeded );

    if (!bSuccess)
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            // not enough memory - re-allocate it according to demands
            DWORD BufSize = nBytesNeeded;
            free(pConfig);
            pConfig = (QUERY_SERVICE_CONFIG*)malloc( BufSize );

            bSuccess = QueryServiceConfig( hService, pConfig, BufSize, &nBytesNeeded );
        }
    };

    if (!bSuccess)
    {
        DWORD Error = GetLastError();
        free( pConfig );

        CString Msg;
        Msg.Format( 
                     _T("Cannot query service configuration for service %s on \\\\%s.\n%s"), 
                     m_ServiceName,
                     m_ComputerName, 
                     GetSystemMessage(Error)
                  );

        GetParent()->MessageBox( Msg, NULL, MB_ICONSTOP | MB_OK );
        return FALSE;
    };

    //-----------------------------------------
    // fill dialog data
    m_DisplayName  = pConfig->lpDisplayName;

    // service type and interactive flag

    DWORD Type = pConfig->dwServiceType;
    m_Interactive = ((Type & SERVICE_INTERACTIVE_PROCESS) != 0);

    Type &= ~SERVICE_INTERACTIVE_PROCESS;

    switch (Type)
    {
        case SERVICE_WIN32_OWN_PROCESS:   
            m_ServiceType = 0;
            break;

        case SERVICE_WIN32_SHARE_PROCESS: 
            m_ServiceType = 1;
            break;

        case SERVICE_KERNEL_DRIVER:   
            m_ServiceType = 2;
            break;

        case SERVICE_FILE_SYSTEM_DRIVER:  
            m_ServiceType = 3;
            break;

        default:
            m_ServiceType = -1;
    };

    // startup
    switch (pConfig->dwStartType)
    {
        case SERVICE_BOOT_START:    m_Startup = 0; break;
        case SERVICE_SYSTEM_START:  m_Startup = 1; break;
        case SERVICE_AUTO_START:    m_Startup = 2; break;
        case SERVICE_DEMAND_START:  m_Startup = 3; break;
        case SERVICE_DISABLED:      m_Startup = 4; break;
        default:                    m_Startup = -1;
    };

    // error control
    switch (pConfig->dwErrorControl)
    {
        case SERVICE_ERROR_IGNORE:   m_ErrorControl = 0; break;
        case SERVICE_ERROR_NORMAL:   m_ErrorControl = 1; break;
        case SERVICE_ERROR_SEVERE:   m_ErrorControl = 2; break;
        case SERVICE_ERROR_CRITICAL: m_ErrorControl = 3; break;
        default:                     m_ErrorControl = -1; 
    };

    // path
    m_Path = pConfig->lpBinaryPathName;

    // group
    m_GroupName = pConfig->lpLoadOrderGroup;

    // dependencies
    KMultiString DependsOn;
    DependsOn.CopyFromBuffer( pConfig->lpDependencies );
    m_DependsOn = DependsOn.GetList();

    m_Account = pConfig->lpServiceStartName;
    m_Password = m_ConfirmPassword = DEFAULT_PASSWORD;

    // free allocated memory
    free(pConfig);

    return TRUE;
        
};
