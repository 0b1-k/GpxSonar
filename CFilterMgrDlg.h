#if !defined(AFX_CFILTERMGRDLG_H__873891F6_A007_4136_8867_17621FF1A045__INCLUDED_)
#define AFX_CFILTERMGRDLG_H__873891F6_A007_4136_8867_17621FF1A045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"
#include "CFilterMgr.h"

class CFilterMgrDlg : public CNonFSDialog
{
// Construction
public:
	CFilterMgrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterMgrDlg();

// Dialog Data
	//{{AFX_DATA(CFilterMgrDlg)
	enum { IDD = IDD_FILTER_MGR };
	CTreeCtrl	m_Tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterMgrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CFilterMgrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetupFilteringOptions();

	HTREEITEM	InsertFilter(HTREEITEM ParentItem, const TCHAR* pText, GcFilter FilterType);

	void OnCacheTypes();
	void OnContainerTypes();
	void OnBearingDistance();
	void OnCacheLists();
	void OnStateList();
	void OnCountryList();
	void OnSearch();
	void OnRatings();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILTERMGRDLG_H__873891F6_A007_4136_8867_17621FF1A045__INCLUDED_)
