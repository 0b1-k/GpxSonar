#if !defined(AFX_CPERFORMANCEDLG_H__67D54E72_7B1F_48CB_8043_E3C03A32AD7F__INCLUDED_)
#define AFX_CPERFORMANCEDLG_H__67D54E72_7B1F_48CB_8043_E3C03A32AD7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CPerformanceDlg : public CNonFSDialog
{
public:
	bool	m_MemMiser;

// Construction
public:
	CPerformanceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPerformanceDlg)
	enum { IDD = IDD_PERFORMANCE };
	int		m_MemMiserCtl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPerformanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPerformanceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPERFORMANCEDLG_H__67D54E72_7B1F_48CB_8043_E3C03A32AD7F__INCLUDED_)
