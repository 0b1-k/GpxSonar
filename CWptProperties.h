#if !defined(AFX_CWPTPROPERTIES_H__909D1EC4_B26B_45F4_A3A0_24E7A8280A45__INCLUDED_)
#define AFX_CWPTPROPERTIES_H__909D1EC4_B26B_45F4_A3A0_24E7A8280A45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CWaypoint;

class CWptProperties : public CNonFSDialog
{
// Construction
public:
	CWptProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWptProperties)
	enum { IDD = IDD_WPT_PROPERTIES };
	CString	m_Comment;
	CString	m_CoordsText;
	CString	m_Name;
	//}}AFX_DATA

	CWaypoint*	m_pWpt;

	CString		m_Title;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWptProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWptProperties)
	virtual BOOL OnInitDialog();
	afx_msg void OnCoords();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWPTPROPERTIES_H__909D1EC4_B26B_45F4_A3A0_24E7A8280A45__INCLUDED_)
