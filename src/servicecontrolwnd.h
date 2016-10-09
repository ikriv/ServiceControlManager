// ServiceControlWnd.h : header file
//
#ifndef _WINSVC_
    #include <winsvc.h>
#endif

#ifndef __LISTCTRLEX_H__
    #include "ListCtrlEx.h"
#endif

#ifndef __SERVICE_TRACKER_H__
    #include "ServiceTracker.h"
#endif

#ifndef __SERVICE_PROPERTY_H__
    #include "ServiceProperty.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CServiceControlDlg dialog

class CServiceControlWnd : public CFrameWnd, 
                           public IServiceStatusNotifySink
{
// Construction
public:
    CServiceControlWnd();
    virtual ~CServiceControlWnd();

// Implementation
protected:
    HICON            m_hIcon;
	CString	         m_Computer;
    CListCtrlEx      m_Services;
    CCriticalSection m_ListCtrlAccess;
    KServiceTracker  m_ServiceTracker;
    CToolBar         m_Toolbar;


    void InitListControl();
    void PopulateListControl();

    // array of visible columns;
    // m_Columns[n] contains number of property displayed in column n
    // See ServiceProperty.h for list of service property numbers
    CArray<int,int> m_Columns;

    // contains width for each type of column; note that index in the array
    // is column TYPE not column number in the list
    int m_Widths[SVC_LAST]; 

    // number of known services
    DWORD m_nServices;

    // m_pStatus and m_pConfig contain information on system services
    // See IMPLEMENTATION NOTES in ServiceControlWnd.cpp for more information
    //
    ENUM_SERVICE_STATUS*    m_pStatus;       
    LPQUERY_SERVICE_CONFIG* m_pConfig;    

    static ENUM_SERVICE_STATUS*    AllocStatusInfo( DWORD nBytes );
    static void FreeStatusInfo( ENUM_SERVICE_STATUS* );

    static LPQUERY_SERVICE_CONFIG* AllocConfigInfo( DWORD nServices );
    static void FreeConfigInfo( LPQUERY_SERVICE_CONFIG*, DWORD nServices );

    void FreeStatusAndConfigInfo();

    void SetCaption();

    int GetCurrentService( BOOL Beep_If_Not_Found = TRUE );
    HANDLE  OpenServiceByName( LPCTSTR Name, DWORD Rights );
    int GetServiceNumberOnScreen( int nService );

    void SaveRegistry();
    void LoadRegistry();

    // Generated message map functions
    //{{AFX_MSG(CServiceControlWnd)
	afx_msg void OnRefresh();
	afx_msg void OnOptionsColumns();
	afx_msg void OnSelectComputer();
    afx_msg void OnServiceStart();
    afx_msg void OnServiceStop();
    afx_msg void OnServicePause();
    afx_msg void OnServiceContinue();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnServiceAdd();
	afx_msg void OnServiceConfigure();
    afx_msg void OnServiceRemove();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnHelpAbout();
    afx_msg void OnDestroy();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    afx_msg void OnServiceDblClk( NMHDR *pNotifyStruct, LRESULT *result );

    // this function is used by OnServiceStop, OnServicePause and OnServiceContinue
    void OnServiceControl( DWORD ControlCode );

    // implementation of functions from IServiceStatusNotifySink
    virtual void OnServiceStatusChange( int nService, SERVICE_STATUS const* Status );
    virtual void OnServiceTargetState( int nService );
    virtual void OnServiceError( int nService, DWORD ErrorCode );
    virtual void OnServiceTimeout( int nService );

};
