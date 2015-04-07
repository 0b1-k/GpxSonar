#if !defined(AFX_CCACHECATEGORYEDITOR_H__D8CC83BC_CC95_49C8_84B7_B1FF5903AC23__INCLUDED_)
#define AFX_CCACHECATEGORYEDITOR_H__D8CC83BC_CC95_49C8_84B7_B1FF5903AC23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CCacheCategoryEditor : public CNonFSDialog
{
// Construction
public:
	CCacheCategoryEditor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCacheCategoryEditor)
	enum { IDD = IDD_CACHECATEGORYEDITOR };
	CListBox	m_CatList;
	CString	m_Cat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCacheCategoryEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCacheCategoryEditor)
	virtual void OnCancel();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCACHECATEGORYEDITOR_H__D8CC83BC_CC95_49C8_84B7_B1FF5903AC23__INCLUDED_)
