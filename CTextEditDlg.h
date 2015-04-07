#if !defined(AFX_CTEXTEDITDLG_H__1876DFFC_71B4_481D_B2D9_D043E5745A35__INCLUDED_)
#define AFX_CTEXTEDITDLG_H__1876DFFC_71B4_481D_B2D9_D043E5745A35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CTextEditDlg : public CNonFSDialog
{
// Construction
public:
	CTextEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextEditDlg)
	enum { IDD = IDD_TEXT_EDIT };
	CButton	m_HtmlCtl;
	CString	m_Text;
	BOOL	m_Html;
	//}}AFX_DATA

	bool	m_EnableHtml;

	CString	m_Title;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTEXTEDITDLG_H__1876DFFC_71B4_481D_B2D9_D043E5745A35__INCLUDED_)
