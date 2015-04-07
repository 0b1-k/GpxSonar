#if !defined(AFX_CFILTERIGNOREDCACHESDLG_H__D54462BF_E288_4D60_9FDB_4D53F9D65A96__INCLUDED_)
#define AFX_CFILTERIGNOREDCACHESDLG_H__D54462BF_E288_4D60_9FDB_4D53F9D65A96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CFilterIgnoredCachesDlg : public CNonFSDialog
{
// Construction
public:
	CFilterIgnoredCachesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterIgnoredCachesDlg)
	enum { IDD = IDD_FILTER_IGNORED_LIST };
	CListBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterIgnoredCachesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterIgnoredCachesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnUnignore();
	afx_msg void OnUnignoreall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILTERIGNOREDCACHESDLG_H__D54462BF_E288_4D60_9FDB_4D53F9D65A96__INCLUDED_)
