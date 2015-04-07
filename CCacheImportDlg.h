#if !defined(AFX_CCACHEIMPORTDLG_H__C03BFD8E_9383_4295_9EF0_A407A0DA42C7__INCLUDED_)
#define AFX_CCACHEIMPORTDLG_H__C03BFD8E_9383_4295_9EF0_A407A0DA42C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonDefs.h"
#include "NonFSDialog.h"

class CGeoCache;

class CCacheImportDlg : public CNonFSDialog
{
// Construction
public:
	CCacheImportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCacheImportDlg)
	enum { IDD = IDD_CACHE_IMPORT };
	CButton	m_ShowMine;
	CListBox	m_Caches;
	//}}AFX_DATA

	String		m_Owner;
	CGeoCache*	m_pCache;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCacheImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	m_ShowMineOnly;

	// Generated message map functions
	//{{AFX_MSG(CCacheImportDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnShowMine();
	afx_msg void OnDblclkCaches();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void PopulateList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCACHEIMPORTDLG_H__C03BFD8E_9383_4295_9EF0_A407A0DA42C7__INCLUDED_)
