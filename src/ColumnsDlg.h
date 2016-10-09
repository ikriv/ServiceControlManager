// ColumnsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColumnsDlg dialog

class CColumnsDlg : public CDialog
{
// Construction
public:
	CColumnsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColumnsDlg)
	enum { IDD = IDD_COLUMNS_DLG };
	CListBox	m_Displayed;
	CListBox	m_Hidden;
	//}}AFX_DATA

    CArray<int,int> DisplayedArr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumnsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CColumnsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHide();
	virtual void OnOK();
	afx_msg void OnShow();
	afx_msg void OnUp();
	afx_msg void OnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
