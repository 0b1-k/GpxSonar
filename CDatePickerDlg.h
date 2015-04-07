#if !defined(AFX_CDATEPICKERDLG_H__51382F76_405D_475E_BDFC_804C04CEC4F4__INCLUDED_)
#define AFX_CDATEPICKERDLG_H__51382F76_405D_475E_BDFC_804C04CEC4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CDatePickerDlg : public CNonFSDialog
{
// Construction
public:
	CDatePickerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDatePickerDlg)
	enum { IDD = IDD_DATE_PICKER };
	CComboBox	m_Year;
	CComboBox	m_Month;
	CComboBox	m_Day;
	//}}AFX_DATA

	SYSTEMTIME	m_Date;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatePickerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDatePickerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDATEPICKERDLG_H__51382F76_405D_475E_BDFC_804C04CEC4F4__INCLUDED_)
