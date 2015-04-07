#if !defined(AFX_CGPXFILEINFODLG_H__DA5E0A1E_C455_46AA_B1B8_CC225B836E8A__INCLUDED_)
#define AFX_CGPXFILEINFODLG_H__DA5E0A1E_C455_46AA_B1B8_CC225B836E8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CGpxParser.h"

class CFieldNoteMgr;

class CGpxFileInfoDlg : public CDialog
{
// Construction
public:
	CGpxFileInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGpxFileInfoDlg();

// Dialog Data
	//{{AFX_DATA(CGpxFileInfoDlg)
	enum { IDD = IDD_GPX_FILE_INFO };
	CListCtrl	m_CacheList;
	//}}AFX_DATA

	CString			m_SavedGpxFilename;
	CImageList		m_ImageList;
	CFieldNoteMgr*	m_pNotesMgr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpxFileInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGpxFileInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void InsertStatLine(int& Line, GcType Type, const TCHAR* pText, int Count);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGPXFILEINFODLG_H__DA5E0A1E_C455_46AA_B1B8_CC225B836E8A__INCLUDED_)
