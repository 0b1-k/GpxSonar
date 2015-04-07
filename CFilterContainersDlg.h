#if !defined(AFX_CFILTERCONTAINERSDLG_H__2B36F82A_AC48_49C5_8BD3_5C665DB715FD__INCLUDED_)
#define AFX_CFILTERCONTAINERSDLG_H__2B36F82A_AC48_49C5_8BD3_5C665DB715FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"
#include "CGpxParser.h"

class CFilterCacheContainers;

class CFilterContainersDlg : public CNonFSDialog
{
// Construction
public:
	CFilterContainersDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterContainersDlg();

// Dialog Data
	//{{AFX_DATA(CFilterContainersDlg)
	enum { IDD = IDD_FILTER_CONTAINERS };
	CTreeCtrl	m_Opts;
	//}}AFX_DATA

	CFilterCacheContainers*	m_pConts;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterContainersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CFilterContainersDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	const TCHAR*	Lookup(GcContainer Cont);
	HTREEITEM		InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFILTERCONTAINERSDLG_H__2B36F82A_AC48_49C5_8BD3_5C665DB715FD__INCLUDED_)
