#if !defined(AFX_CTBEXCHANGEDLG_H__63462F14_48F7_43D3_8AD8_B391E6AABE66__INCLUDED_)
#define AFX_CTBEXCHANGEDLG_H__63462F14_48F7_43D3_8AD8_B391E6AABE66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonDefs.h"
#include "CTBActionMgr.h"
#include "NonFSDialog.h"

class CTBExchangeDlg : public CNonFSDialog
{
// Construction
public:
	CTBExchangeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTBExchangeDlg)
	enum { IDD = IDD_CACHE_TRAVEL_BUGS };
	CListBox	m_MyTBs;
	CListBox	m_CacheTBs;
	//}}AFX_DATA

	CTBActionMgr	m_TBActionMgr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTBExchangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTBExchangeDlg)
	afx_msg void OnDrop();
	afx_msg void OnGrab();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateCacheList();
	void UpdateInventoryList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTBEXCHANGEDLG_H__63462F14_48F7_43D3_8AD8_B391E6AABE66__INCLUDED_)
