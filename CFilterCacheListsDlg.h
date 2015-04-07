#if !defined(AFX_CFILTERCACHELISTSDLG_H__5855DD55_8F60_46E3_BFDB_E306B621F209__INCLUDED_)
#define AFX_CFILTERCACHELISTSDLG_H__5855DD55_8F60_46E3_BFDB_E306B621F209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CFilterCacheLists;

class CFilterCacheListsDlg : public CNonFSDialog
{
// Construction
public:
	CFilterCacheListsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterCacheListsDlg)
	enum { IDD = IDD_CACHE_LISTS };
	CButton	m_ExclusiveShowCtrl;
	int		m_ExclusiveShow;
	BOOL	m_HideFound;
	BOOL	m_ShowIgnored;
	BOOL	m_UseShowOnly;
	BOOL	m_HideDisabled;
	//}}AFX_DATA

	CFilterCacheLists*	m_pCacheLists;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterCacheListsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterCacheListsDlg)
	afx_msg void OnEditIgnored();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnShowOnly();
	afx_msg void OnHideFound();
	afx_msg void OnHideIgnored();
	afx_msg void OnHideDisabled();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILTERCACHELISTSDLG_H__5855DD55_8F60_46E3_BFDB_E306B621F209__INCLUDED_)
