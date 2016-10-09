#ifndef __LISTCTRLEX_H__
#define __LISTCTRLEX_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXDISP_H__
    #include "afxdisp.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx window

class CHeaderCtrlEx : public CHeaderCtrl
{
	// Construction
public:	
	CHeaderCtrlEx();
	CHeaderCtrlEx(CWnd* pWnd, void (CWnd::*fpDragCol)(int, int));
	
	// Attributes
	BOOL EnableDrag(BOOL bDrag=TRUE);

public:
	// Operations

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderCtrlEx)
	//}}AFX_VIRTUAL
	// Implementation

public:
	virtual ~CHeaderCtrlEx();
	void SetCallback(CWnd* pWnd, void (CWnd::*fpDragCol)(int, int));

protected:
	BOOL	m_bDragEnable;

	BOOL	m_bCheckForDrag;
	BOOL	m_bDragging;
	int		*m_pWidth;
	int		m_nDragCol;
	int		m_nDropPos;
	CRect	marker_rect;
	void	(CWnd::*m_fpDragCol)(int, int);
	CWnd	*m_pOwnerWnd;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderCtrlEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

#define HIGHLIGHT_NORMAL		0
#define HIGHLIGHT_ALLCOLUMNS	1
#define HIGHLIGHT_ROW			2

#define LVEX_CASE				0
#define LVEX_NOCASE				1
#define LVEX_NUM				2	//integer
#define LVEX_TIME				3
#define LVEX_NOSORT				4
#define LVEX_NUM_FLOAT			5
#define LVEX_TIME_BASE			6	//probably will never use this
#define LVEX_LAST				6	//increment this if you add more types


class CListCtrlEx : public CListCtrl
{
// Construction
public:	
	CListCtrlEx();
	
	// Attributes
	int SetHighlightType(int hilite);
	BOOL EnableSort(BOOL bSort=TRUE);
	BOOL SetColumnTypes(unsigned char* pColTypes, unsigned nColCnt);
	BOOL EnableHeaderDrag(BOOL bDrag=TRUE);
	BOOL SetBaseTime(CString strTime);

    int  GetSortedColumn() { return nSortedCol; }; // -1 if none
    BOOL IsSortAscending() { return bSortAscending; };

public:
	// Operations	

    // sort by new column
    void SortByColumn( int nColumn, BOOL bAscending = TRUE );
    
    // restore sort order, if it was violated by new items, etc.
    void DefaultSort(); 
	
protected:	
	int  m_nHighlight;		// Indicate type of selection highlighting
	BOOL m_bSort;			// set to FALSE if you don't want to sort
	unsigned char* m_pTypes;//column sort types
	int m_nNumCols;	//number of columns to sort
	COleDateTime m_ctStart;	//base time for sorting on 24hr clock
	
	int  nSortedCol;	
	BOOL bSortAscending;
	CHeaderCtrlEx	m_headerctrl;
	
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void RepaintSelectedItems();
	void DragColumn(int source, int dest);

    union KSortInfo
    {
        struct
        {
            unsigned char SortType;
            unsigned char Ascending;
        };
        LPARAM lParam;
    };
	
	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	//data structure for setdata
	struct ListData
	{
		DWORD dwOldData;
		CString strText;
		COleDateTime BaseTime;
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif 
