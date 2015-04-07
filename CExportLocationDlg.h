#if !defined(AFX_CEXPORTLOCATIONDLG_H__E475349F_E8DB_45A3_ACBC_E664F2C002F5__INCLUDED_)
#define AFX_CEXPORTLOCATIONDLG_H__E475349F_E8DB_45A3_ACBC_E664F2C002F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CExportLocationMgr;

class CExportLocationDlg : public CNonFSDialog
{
// Construction
public:
	CExportLocationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportLocationDlg)
	enum { IDD = IDD_EXPORT_LOCS_SELECTION };
	CListBox	m_ExportTypesList;
	CString	m_Location;
	//}}AFX_DATA

	CExportLocationMgr*	m_pExpLocMgr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportLocationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportLocationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetLoc();
	afx_msg void OnSelchangeExportTypes();
	afx_msg void OnExport();
	afx_msg void OnExportOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEXPORTLOCATIONDLG_H__E475349F_E8DB_45A3_ACBC_E664F2C002F5__INCLUDED_)
