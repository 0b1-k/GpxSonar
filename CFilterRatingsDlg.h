#if !defined(AFX_CFilterRatingsDlg_H__CFB621CA_750D_4032_AB4E_EC6CED954935__INCLUDED_)
#define AFX_CFilterRatingsDlg_H__CFB621CA_750D_4032_AB4E_EC6CED954935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CFilterCacheRatings;

class CFilterRatingsDlg : public CNonFSDialog
{
// Construction
public:
	CFilterRatingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterRatingsDlg)
	enum { IDD = IDD_FILTER_RATINGS };
	CComboBox	m_TerrOper;
	CComboBox	m_TerrLvl;
	CComboBox	m_DiffOper;
	CComboBox	m_DiffLvl;
	BOOL	m_DiffEnabled;
	BOOL	m_TerrEnabled;
	//}}AFX_DATA

	CFilterCacheRatings* m_pRatings;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterRatingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterRatingsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	double	ToNumerical(CString& Rating);
	CString	ToText(double RatingVal);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFilterRatingsDlg_H__CFB621CA_750D_4032_AB4E_EC6CED954935__INCLUDED_)
