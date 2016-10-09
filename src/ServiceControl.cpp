// ServiceControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "wincon.h"
#include "io.h"
#include "fcntl.h"
#include "ServiceControl.h"
#include "ServiceControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServiceControlApp

BEGIN_MESSAGE_MAP(CServiceControlApp, CWinApp)
	//{{AFX_MSG_MAP(CServiceControlApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServiceControlApp construction

CServiceControlApp::CServiceControlApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServiceControlApp object

CServiceControlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServiceControlApp initialization

BOOL CServiceControlApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CServiceControlWnd* pWnd = new CServiceControlWnd;
	m_pMainWnd = pWnd;

    pWnd->LoadFrame( IDR_MAINFRAME );
    pWnd->ShowWindow( SW_SHOW );

    return TRUE;
}
