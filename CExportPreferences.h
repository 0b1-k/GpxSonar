#if !defined(AFX_CEXPORTPREFERENCES_H__1C75114F_F568_4A43_AEA7_FB27F7C4D3EE__INCLUDED_)
#define AFX_CEXPORTPREFERENCES_H__1C75114F_F568_4A43_AEA7_FB27F7C4D3EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CExportPreferences : public CNonFSDialog
{
// Construction
public:
	CExportPreferences(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportPreferences)
	enum { IDD = IDD_EXPORT_PREFS };
	BOOL	m_ExportsNamesAsGpxName;
	UINT	m_ExportMaxLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportPreferences)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEXPORTPREFERENCES_H__1C75114F_F568_4A43_AEA7_FB27F7C4D3EE__INCLUDED_)
