// ComputerNameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComputerNameDlg dialog

class CComputerNameDlg : public CDialog
{
// Construction
public:
	CComputerNameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComputerNameDlg)
	enum { IDD = IDD_COMPUTER_NAME_DLG };
	CString	m_Computer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComputerNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComputerNameDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
