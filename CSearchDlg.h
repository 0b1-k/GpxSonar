#if !defined(AFX_CSEARCHDLG_H__2DD570BE_4EE4_43C7_9AEC_041BC25A6A98__INCLUDED_)
#define AFX_CSEARCHDLG_H__2DD570BE_4EE4_43C7_9AEC_041BC25A6A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CFilterSearch;

class CSearchDlg : public CNonFSDialog
{
public:
	CFilterSearch*	m_pSearch;

	bool			m_ShowEnableSearch;

// Construction
public:
	CSearchDlg(CWnd* pParent = NULL);
	virtual ~CSearchDlg();

// Dialog Data
	//{{AFX_DATA(CSearchDlg)
	enum { IDD = IDD_SEARCH };
	CButton	m_EnableSearchCtl;
	CEdit	m_TextCtl;
	int		m_Match;
	CString	m_Text;
	BOOL	m_EnableSearch;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	String	m_ModHash;

	// Generated message map functions
	//{{AFX_MSG(CSearchDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnEnableSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	String	HashValues();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSEARCHDLG_H__2DD570BE_4EE4_43C7_9AEC_041BC25A6A98__INCLUDED_)
