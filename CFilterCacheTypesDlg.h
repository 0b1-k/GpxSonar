#if !defined(AFX_CFILTERCACHETYPESDLG_H__9D6AF0C5_95E0_43C4_A716_90B8ACD3A1F5__INCLUDED_)
#define AFX_CFILTERCACHETYPESDLG_H__9D6AF0C5_95E0_43C4_A716_90B8ACD3A1F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"
#include "CGpxParser.h"

class CFilterCacheTypes;

class CFilterCacheTypesDlg : public CNonFSDialog
{
// Construction
public:
	CFilterCacheTypesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterCacheTypesDlg();

// Dialog Data
	//{{AFX_DATA(CFilterCacheTypesDlg)
	enum { IDD = IDD_FILTER_TYPES };
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	CFilterCacheTypes*	m_pTypes;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterCacheTypesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CFilterCacheTypesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HTREEITEM		InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image);
	const TCHAR*	Lookup(GcType Type);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILTERCACHETYPESDLG_H__9D6AF0C5_95E0_43C4_A716_90B8ACD3A1F5__INCLUDED_)
