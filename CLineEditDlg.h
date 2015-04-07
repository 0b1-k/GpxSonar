#if !defined(AFX_CLINEEDITDLG_H__B02AFD0E_85FE_4429_BF73_5C5E473E1A4C__INCLUDED_)
#define AFX_CLINEEDITDLG_H__B02AFD0E_85FE_4429_BF73_5C5E473E1A4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CLineEditDlg : public CNonFSDialog
{
// Construction
public:
	CLineEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineEditDlg)
	enum { IDD = IDD_LINE_EDIT };
	CString	m_Text;
	//}}AFX_DATA

	CString	m_Title;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLINEEDITDLG_H__B02AFD0E_85FE_4429_BF73_5C5E473E1A4C__INCLUDED_)
