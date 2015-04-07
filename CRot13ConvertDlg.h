#if !defined(AFX_CROT13CONVERTDLG_H__31890B83_B58F_4CBA_B320_F54229646B83__INCLUDED_)
#define AFX_CROT13CONVERTDLG_H__31890B83_B58F_4CBA_B320_F54229646B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CRot13ConvertDlg : public CNonFSDialog
{
// Construction
public:
	CRot13ConvertDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRot13ConvertDlg)
	enum { IDD = IDD_ROT13CONVERT };
	CString	m_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRot13ConvertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRot13ConvertDlg)
	afx_msg void OnConvert();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROT13CONVERTDLG_H__31890B83_B58F_4CBA_B320_F54229646B83__INCLUDED_)
