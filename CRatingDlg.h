#if !defined(AFX_CRATINGDLG_H__3B1D2447_7039_4CA6_A25E_1147ED00E494__INCLUDED_)
#define AFX_CRATINGDLG_H__3B1D2447_7039_4CA6_A25E_1147ED00E494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CRatingDlg : public CNonFSDialog
{
// Construction
public:
	CRatingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRatingDlg)
	enum { IDD = IDD_RATING };
	CComboBox	m_Rating;
	//}}AFX_DATA

	CString		m_Title;
	double		m_RatingVal;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRatingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	double		ToNumerical(CString& Rating);
	CString		ToText(double Rating);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRATINGDLG_H__3B1D2447_7039_4CA6_A25E_1147ED00E494__INCLUDED_)
