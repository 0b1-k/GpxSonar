#if !defined(AFX_CFILTERSTRINGSDLG_H__EC1328B5_8B20_4664_B08B_D6E5247F19E4__INCLUDED_)
#define AFX_CFILTERSTRINGSDLG_H__EC1328B5_8B20_4664_B08B_D6E5247F19E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CFilterOnStrings;

class CFilterStringsDlg : public CNonFSDialog
{
// Construction
public:
	CFilterStringsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterStringsDlg();

// Dialog Data
	//{{AFX_DATA(CFilterStringsDlg)
	enum { IDD = IDD_FILTER_STRINGS };
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	CFilterOnStrings*	m_pFiltStrs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterStringsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CFilterStringsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HTREEITEM InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILTERSTRINGSDLG_H__EC1328B5_8B20_4664_B08B_D6E5247F19E4__INCLUDED_)
