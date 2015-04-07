#if !defined(AFX_CCENTERCOORDSDLG_H__2744F849_AB48_4906_BE6C_62FA02022ECE__INCLUDED_)
#define AFX_CCENTERCOORDSDLG_H__2744F849_AB48_4906_BE6C_62FA02022ECE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonDefs.h"
#include "NonFSDialog.h"

class CWPMgr;

class CCenterCoordsDlg : public CNonFSDialog
{
// Construction
public:
	CCenterCoordsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCenterCoordsDlg)
	enum { IDD = IDD_CENTER };
	CButton	m_BookmarksCtl;
	CComboBox	m_Long;
	CComboBox	m_Lat;
	int		m_LatDeg;
	float	m_LatMinMmm;
	int		m_LongDeg;
	float	m_LongMinMmm;
	//}}AFX_DATA

	String	m_LatChar;
	String	m_LongChar;
	CWPMgr*	m_pWptsMgr;
	bool	m_BookmarkMode;

	void	SetTitle(const TCHAR* pTitle);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCenterCoordsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCenterCoordsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnProject();
	afx_msg void OnBookmark();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	TCHAR* m_pTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCENTERCOORDSDLG_H__2744F849_AB48_4906_BE6C_62FA02022ECE__INCLUDED_)
