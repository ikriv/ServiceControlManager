// ConfigServiceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigServiceDlg dialog

class CConfigServiceDlg : public CDialog
{
// Construction
public:
	CConfigServiceDlg(
                        CWnd*   pParent,
                        BOOL    bConfigure, // TRUE = configure, FALSE = install
                        LPCTSTR ComputerName,
                        LPCTSTR ServiceName // Ignored if bConfigure == FALSE
                     );   

// Dialog Data
	//{{AFX_DATA(CConfigServiceDlg)
	enum { IDD = IDD_CONFIG_SERVICE };
	CString	m_ServiceName;
	CString	m_DisplayName;
	CString	m_Path;
	int		m_ServiceType;
	int		m_Startup;
	int		m_ErrorControl;
	BOOL	m_Interactive;
	CString	m_GroupName;
	CString	m_DependsOn;
	CString	m_Account;
	CString	m_Password;
	CString	m_ConfirmPassword;
	//}}AFX_DATA

    BOOL m_bConfigure;
    CString m_ComputerName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigServiceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigServiceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    BOOL FillDialogFromServiceData();
};
