#if !defined(AFX_CMYALIASDLG_H__C035BADA_2007_4FA5_BDAE_C9A015475DC8__INCLUDED_)
#define AFX_CMYALIASDLG_H__C035BADA_2007_4FA5_BDAE_C9A015475DC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CMyAliasDlg : public CNonFSDialog
{
// Construction
public:
	CMyAliasDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyAliasDlg)
	enum { IDD = IDD_ALIAS };
	CString	m_Alias;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAliasDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyAliasDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMYALIASDLG_H__C035BADA_2007_4FA5_BDAE_C9A015475DC8__INCLUDED_)
