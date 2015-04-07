#if !defined(AFX_CLISTPREFERENCESDLG_H__6683ECF9_044F_4355_BDAE_D6E479EE6C29__INCLUDED_)
#define AFX_CLISTPREFERENCESDLG_H__6683ECF9_044F_4355_BDAE_D6E479EE6C29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CListPreferencesDlg : public CNonFSDialog
{
// Construction
public:
	CListPreferencesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CListPreferencesDlg();

// Dialog Data
	//{{AFX_DATA(CListPreferencesDlg)
	enum { IDD = IDD_LIST_PREFERENCES };
	CTreeCtrl	m_Cols;
	//}}AFX_DATA

	// Pointer to the 
	HeadingCont*	m_pHeadings;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListPreferencesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CListPreferencesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HTREEITEM	InsertCol(HTREEITEM ParentItem, const TCHAR* pText, LPARAM pData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLISTPREFERENCESDLG_H__6683ECF9_044F_4355_BDAE_D6E479EE6C29__INCLUDED_)
