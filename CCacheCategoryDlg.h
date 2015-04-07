#if !defined(AFX_CCACHECATEGORYDLG_H__B7B422BE_4E00_4ED9_A8B4_4DDDE690ED32__INCLUDED_)
#define AFX_CCACHECATEGORYDLG_H__B7B422BE_4E00_4ED9_A8B4_4DDDE690ED32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CCacheCategoryDlg : public CNonFSDialog
{
// Construction
public:
	CCacheCategoryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCacheCategoryDlg)
	enum { IDD = IDD_CACHECATEGORY };
	CComboBox	m_Data;
	//}}AFX_DATA
	CString		m_Selection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCacheCategoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCacheCategoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCACHECATEGORYDLG_H__B7B422BE_4E00_4ED9_A8B4_4DDDE690ED32__INCLUDED_)
