#if !defined(AFX_CCACHEPROPERTIESDLG_H__60C630EB_1010_416B_82BC_BC2A85C49A99__INCLUDED_)
#define AFX_CCACHEPROPERTIESDLG_H__60C630EB_1010_416B_82BC_BC2A85C49A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "NonFSDialog.h"
#include "CHeading.h"

class CGeoCache;

typedef enum CacheProperty {
	CacheCategory = 0,
	CacheType,
	CacheSize,
	CacheNickname,
	CacheWaypoint,
	CacheDatePlaced,
	CacheCoords,
	CacheCountry,
	CacheState,
	CacheDifficulty,
	CacheTerrain,
	CacheShortDesc,
	CacheLongDesc,
	CacheHint,
	CacheWaypoints,
	CacheNotes,
	CacheEndOfList
	};

typedef enum Function {
	LabelLookup = 0,
	EditProperty,
	FormatProperty
};

class CCachePropertiesDlg : public CNonFSDialog
{
// Construction
public:
	CCachePropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCachePropertiesDlg();

// Dialog Data
	//{{AFX_DATA(CCachePropertiesDlg)
	enum { IDD = IDD_MYCACHE };
	CListCtrl	m_Props;
	//}}AFX_DATA

	CGeoCache*	m_pGc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCachePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HeadingCont	m_Headings;

	CString	m_Temp;

	// Generated message map functions
	//{{AFX_MSG(CCachePropertiesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnPreview();
	afx_msg void OnDblclkProperties(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void PropertyFunction(int Idx, Function Func, CString& Text);
	void UpdateList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCACHEPROPERTIESDLG_H__60C630EB_1010_416B_82BC_BC2A85C49A99__INCLUDED_)
