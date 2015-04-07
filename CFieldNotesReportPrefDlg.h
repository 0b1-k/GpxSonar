#if !defined(AFX_CFIELDNOTESREPORTPREFDLG_H__3FEB4FF5_03E0_49A3_A755_F7C5FCFB131A__INCLUDED_)
#define AFX_CFIELDNOTESREPORTPREFDLG_H__3FEB4FF5_03E0_49A3_A755_F7C5FCFB131A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CCacheReportsPref;

class CFieldNotesReportPrefDlg : public CNonFSDialog
{
// Construction
public:
	CFieldNotesReportPrefDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFieldNotesReportPrefDlg)
	enum { IDD = IDD_FIELDNOTESREPORTPREFS };
	BOOL	m_Archive;
	BOOL	m_Delete;
	BOOL	m_Found;
	BOOL	m_Notes;
	BOOL	m_NotFound;
	//}}AFX_DATA

	CCacheReportsPref*	m_ReportsPref;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldNotesReportPrefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFieldNotesReportPrefDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFIELDNOTESREPORTPREFDLG_H__3FEB4FF5_03E0_49A3_A755_F7C5FCFB131A__INCLUDED_)
