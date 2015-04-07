#if !defined(AFX_CMISCPREFERENCES_H__79D3129A_07A1_46FD_86DC_B82186C6C4EC__INCLUDED_)
#define AFX_CMISCPREFERENCES_H__79D3129A_07A1_46FD_86DC_B82186C6C4EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CMiscPreferences : public CNonFSDialog
{
// Construction
public:
	CMiscPreferences(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMiscPreferences)
	enum { IDD = IDD_PREFERENCES };
	BOOL	m_DblClickCacheDetails;
	BOOL	m_ReloadLastGpxFile;
	CString	m_SpoilerPicsPath;
	BOOL	m_StripImgTags;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiscPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMiscPreferences)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpoilerBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMISCPREFERENCES_H__79D3129A_07A1_46FD_86DC_B82186C6C4EC__INCLUDED_)
