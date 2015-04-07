#if !defined(AFX_CMYCACHESDLG_H__1DC69AF7_4DAD_4E27_AA32_9489642CD5D8__INCLUDED_)
#define AFX_CMYCACHESDLG_H__1DC69AF7_4DAD_4E27_AA32_9489642CD5D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"
#include "CHeading.h"

class CCacheMgr;
class CExportLocationMgr;

class CMyCachesDlg : public CNonFSDialog
{
// Construction
public:
	CMyCachesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyCachesDlg();

// Dialog Data
	//{{AFX_DATA(CMyCachesDlg)
	enum { IDD = IDD_MYCACHES };
	CComboBox	m_CommandList;
	CComboBox	m_Categories;
	CListCtrl	m_CacheList;
	//}}AFX_DATA

	int					m_CurrSortCol;
	bool				m_NeedToSaveChanges;
	CExportLocationMgr*	m_pExpLocMgr;
	CCacheMgr*			m_pCacheMgr;
	HeadingCont			m_Headings;
	CImageList			m_ImageList;
	String				m_Owner;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCachesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyCachesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickCaches(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCaches(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCatEdit();
	afx_msg void OnSelchangeCategory();
	afx_msg void OnGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static int CALLBACK CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	void		UpdateCacheList();
	int			ShowCacheProperties(CGeoCache* pGc);
	CGeoCache*	GetSelectedItem();

	// Enable / disable the cache category selection if the list of selections is empty
	void		EnableCategories();

	void		OnNew();
	void		OnEdit();
	void		OnDelete();
	void		OnImport();
	void		OnExport();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMYCACHESDLG_H__1DC69AF7_4DAD_4E27_AA32_9489642CD5D8__INCLUDED_)
