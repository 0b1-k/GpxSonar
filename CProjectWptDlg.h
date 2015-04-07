#if !defined(AFX_CPROJECTWPTDLG_H__6560F5D3_1071_42AF_9AAA_0D4F2183E299__INCLUDED_)
#define AFX_CPROJECTWPTDLG_H__6560F5D3_1071_42AF_9AAA_0D4F2183E299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"
#include "CCoords.h"

class CProjectWptDlg : public CNonFSDialog
{
// Construction
public:
	CProjectWptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectWptDlg)
	enum { IDD = IDD_PROJECT };
	CComboBox	m_Units;
	CComboBox	m_Dir;
	double	m_Bearing;
	double	m_Dist;
	//}}AFX_DATA

	// Coords resulting from the projection
	CCoords	m_Coords;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectWptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectWptDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeDirection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPROJECTWPTDLG_H__6560F5D3_1071_42AF_9AAA_0D4F2183E299__INCLUDED_)
