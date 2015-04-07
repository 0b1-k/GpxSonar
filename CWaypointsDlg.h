#if !defined(AFX_CWAYPOINTSDLG_H__3AC1DEF7_C173_4D3E_B334_11DC1D65F06C__INCLUDED_)
#define AFX_CWAYPOINTSDLG_H__3AC1DEF7_C173_4D3E_B334_11DC1D65F06C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"
#include "CHeading.h"

class CWPMgr;
class CWaypoint;

class CWaypointsDlg : public CNonFSDialog
{
// Construction
public:
	CWaypointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaypointsDlg();

// Dialog Data
	//{{AFX_DATA(CWaypointsDlg)
	enum { IDD = IDD_WAYPOINTS };
	CButton	m_SelectCtl;
	CListCtrl	m_Wpts;
	//}}AFX_DATA

	CWPMgr*		m_pWptsMgr;
	CWaypoint*	m_pSelectedWp;
	bool		m_BookmarkMode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaypointsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HeadingCont	m_Headings;
	CImageList	m_ImageList;
	int			m_CurrSortCol;

	// Generated message map functions
	//{{AFX_MSG(CWaypointsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNew();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	afx_msg void OnImport();
	afx_msg void OnDblclkWpts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickWpts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static int CALLBACK CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//static int CALLBACK CompareComment(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	void		UpdateWptList();

	CWaypoint*	GetSelectedItem();
	int			ShowWptProperties(CWaypoint* pWp);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWAYPOINTSDLG_H__3AC1DEF7_C173_4D3E_B334_11DC1D65F06C__INCLUDED_)
