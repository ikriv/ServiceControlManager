// ComputerNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceControl.h"
#include "ComputerNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComputerNameDlg dialog


CComputerNameDlg::CComputerNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComputerNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComputerNameDlg)
	m_Computer = _T("");
	//}}AFX_DATA_INIT
}


void CComputerNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComputerNameDlg)
	DDX_Text(pDX, IDC_COMPUTER_NAME, m_Computer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComputerNameDlg, CDialog)
	//{{AFX_MSG_MAP(CComputerNameDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComputerNameDlg message handlers
