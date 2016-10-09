// ColumnsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServiceControl.h"
#include "ColumnsDlg.h"
#include "ServiceProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColumnsDlg dialog


CColumnsDlg::CColumnsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColumnsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumnsDlg)
	//}}AFX_DATA_INIT
}


void CColumnsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnsDlg)
	DDX_Control(pDX, IDC_DISPLAYED_LIST, m_Displayed);
	DDX_Control(pDX, IDC_HIDDEN_LIST, m_Hidden);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnsDlg, CDialog)
	//{{AFX_MSG_MAP(CColumnsDlg)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumnsDlg message handlers

BOOL CColumnsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BOOL ColumnDisplayed[SVC_LAST];

    int i;
    for (i=0; i<SVC_LAST; i++)
        ColumnDisplayed[i] = FALSE;

    int DispSize = DisplayedArr.GetSize();
    
    for (i=0; i<DispSize; i++)
    {
        int nProp = DisplayedArr[i];
        int Idx = m_Displayed.AddString( PropertyName[nProp] );
        m_Displayed.SetItemData( Idx, nProp );
        ColumnDisplayed[nProp] = TRUE;
    };

    for (i=0; i<SVC_LAST; i++)
    {
        if (!ColumnDisplayed[i])
        {
            int Idx = m_Hidden.AddString( PropertyName[i] );
            m_Hidden.SetItemData( Idx, i );
        }
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------
void CColumnsDlg::OnOK() 
{
    DisplayedArr.RemoveAll();

    int nItems = m_Displayed.GetCount();
    for (int i=0; i<nItems; i++)
    {
        DisplayedArr.Add( m_Displayed.GetItemData(i) );
    };

	CDialog::OnOK();
}

//-----------------------------------------------
static void MoveItem( CListBox& A, CListBox& B )
{
    int nItem = A.GetCurSel();
    if (nItem == -1)
    {
        MessageBeep(0);
        return;
    };

    int nProp = A.GetItemData(nItem);
    A.DeleteString(nItem);

    int NewSel = nItem;
    if (nItem > A.GetCount())
        NewSel = A.GetCount()-1;
        
    A.SetCurSel(NewSel); 

    int nItemNew = B.AddString( PropertyName[nProp] );
    B.SetItemData(nItemNew, nProp);
    B.SetCurSel(nItemNew);
};

//-----------------------------------------------
static BOOL SwapItems( CListBox& L, int ItemA, int ItemB )
{
    if (ItemA>ItemB) 
    {
        int Temp = ItemA;
        ItemA = ItemB;
        ItemB = Temp;
    }

    int Count = L.GetCount();
    if ((ItemA<0) || (ItemA>=Count) || (ItemB<0) || (ItemB>=Count))
    {
        MessageBeep(0);
        return FALSE;
    };

    DWORD ItemAData = L.GetItemData(ItemA);
    CString ItemAString;
    L.GetText(ItemA,ItemAString);

    DWORD ItemBData = L.GetItemData(ItemB);
    CString ItemBString;
    L.GetText(ItemB,ItemBString);

    L.DeleteString(ItemA);
    L.DeleteString(ItemB-1);

    L.InsertString(ItemA, ItemBString);
    L.SetItemData(ItemA, ItemBData);

    L.InsertString(ItemB, ItemAString);
    L.SetItemData(ItemB, ItemAData);
    return TRUE;
};

//-----------------------------------------------
void CColumnsDlg::OnHide() 
{
    MoveItem(m_Displayed, m_Hidden);
}

//-----------------------------------------------
void CColumnsDlg::OnShow() 
{
    MoveItem(m_Hidden, m_Displayed);
}

//-----------------------------------------------
void CColumnsDlg::OnUp() 
{
    int CurSel = m_Displayed.GetCurSel();
    if (SwapItems( m_Displayed, CurSel-1, CurSel ));
        m_Displayed.SetCurSel(CurSel-1);
}

//-----------------------------------------------
void CColumnsDlg::OnDown() 
{
    int CurSel = m_Displayed.GetCurSel();
    if (SwapItems( m_Displayed, CurSel, CurSel+1 ))
        m_Displayed.SetCurSel(CurSel+1);
}
