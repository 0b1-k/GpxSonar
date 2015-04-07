#if !defined(AFX_CTYPEANDCONTAINERDLG_H__68AFB9BA_1746_4EB7_BDDE_D4DB5055215A__INCLUDED_)
#define AFX_CTYPEANDCONTAINERDLG_H__68AFB9BA_1746_4EB7_BDDE_D4DB5055215A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

typedef enum FillerType {
	CacheTypes = 0,
	CacheContainers
};

class CTypeAndContainerDlg : public CNonFSDialog
{
// Construction
public:
	CTypeAndContainerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTypeAndContainerDlg)
	enum { IDD = IDD_TYPE_AND_CONTAINER };
	CComboBox	m_Data;
	//}}AFX_DATA

	CString		m_Title;
	CString		m_Selection;
	FillerType	m_FillerType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeAndContainerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTypeAndContainerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	UpdateList();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTYPEANDCONTAINERDLG_H__68AFB9BA_1746_4EB7_BDDE_D4DB5055215A__INCLUDED_)
