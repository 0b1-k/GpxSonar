#if !defined(AFX_CTBINFODLG_H__760A2D34_B543_4667_9F43_3E30B8BEC1DB__INCLUDED_)
#define AFX_CTBINFODLG_H__760A2D34_B543_4667_9F43_3E30B8BEC1DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CTB;

class CTBInfoDlg : public CNonFSDialog
{
// Construction
public:
	CTBInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTBInfoDlg)
	enum { IDD = IDD_TB_PROPERTIES };
	CString	m_Name;
	CString	m_Serial;
	CString	m_Location;
	CString	m_Ref;
	//}}AFX_DATA

	CTB* m_pTB;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTBInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTBInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTBINFODLG_H__760A2D34_B543_4667_9F43_3E30B8BEC1DB__INCLUDED_)
