#if !defined(AFX_CFILTERBEARINGDISTANCEDLG_H__756DA26F_C35E_4D19_81A6_6CF3ED97B2EA__INCLUDED_)
#define AFX_CFILTERBEARINGDISTANCEDLG_H__756DA26F_C35E_4D19_81A6_6CF3ED97B2EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CFilterBearingDistance;

class CFilterBearingDistanceDlg : public CNonFSDialog
{
// Construction
public:
	CFilterBearingDistanceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterBearingDistanceDlg();

// Dialog Data
	//{{AFX_DATA(CFilterBearingDistanceDlg)
	enum { IDD = IDD_FILTER_DIST_BEARING };
	CTreeCtrl	m_Tree;
	double	m_Dist;
	//}}AFX_DATA

	CFilterBearingDistance*	m_pBearDist;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterBearingDistanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CFilterBearingDistanceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HTREEITEM	InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILTERBEARINGDISTANCEDLG_H__756DA26F_C35E_4D19_81A6_6CF3ED97B2EA__INCLUDED_)
