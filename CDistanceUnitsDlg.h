#if !defined(AFX_CDISTANCEUNITSDLG_H__7E5F845C_E9D4_4C1A_9757_0C33A90DF7C6__INCLUDED_)
#define AFX_CDISTANCEUNITSDLG_H__7E5F845C_E9D4_4C1A_9757_0C33A90DF7C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CDistanceUnitsDlg : public CNonFSDialog
{
// Construction
public:
	CDistanceUnitsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDistanceUnitsDlg)
	enum { IDD = IDD_DIST_UNITS };
	int		m_DistUnits;
	//}}AFX_DATA

	void	SetDistanceUnits(double Units);
	double	GetDistanceUnits(CString& Label);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDistanceUnitsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDistanceUnitsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDISTANCEUNITSDLG_H__7E5F845C_E9D4_4C1A_9757_0C33A90DF7C6__INCLUDED_)
