#if !defined(AFX_CMYTRAVELBUGSDLG_H__440D3825_AD96_46A7_A8A7_09F712753A7C__INCLUDED_)
#define AFX_CMYTRAVELBUGSDLG_H__440D3825_AD96_46A7_A8A7_09F712753A7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"
#include "CHeading.h"

class CMyTravelBugsDlg : public CNonFSDialog
{
// Construction
public:
	CMyTravelBugsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyTravelBugsDlg();

// Dialog Data
	//{{AFX_DATA(CMyTravelBugsDlg)
	enum { IDD = IDD_MY_TRAVEL_BUGS };
	CListCtrl	m_TBs;
	//}}AFX_DATA

	CImageList	m_ImageList;
	HeadingCont	m_Headings;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTravelBugsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int		m_CurrSortCol;

	// Generated message map functions
	//{{AFX_MSG(CMyTravelBugsDlg)
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickTbList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetupListControl();
	void UpdateList();

	//static int CALLBACK CompareRef(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//static int CALLBACK CompareSerial(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//static int CALLBACK CompareLocation(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMYTRAVELBUGSDLG_H__440D3825_AD96_46A7_A8A7_09F712753A7C__INCLUDED_)
