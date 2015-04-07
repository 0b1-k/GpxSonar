#if !defined(AFX_NONFSDIALOG_H__494B0C5A_1359_4E0D_932C_5DDB1619A744__INCLUDED_)
#define AFX_NONFSDIALOG_H__494B0C5A_1359_4E0D_932C_5DDB1619A744__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NonFSDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//	created:		2002/09/03
//	created:		3:9:2002   17:20
//	filename:		E:\AfriGIS\WinCE\PPC2002\PocketMapCE\Source\PocketMap\NonFSDialog.h
//	file path:	E:\AfriGIS\WinCE\PPC2002\PocketMapCE\Source\PocketMap
//	file base:	NonFSDialog
//	file ext:		h
//	author:			Tanzim Husain [tanzim@mail.com]
//	
//	purpose:		A non full screen dialog with SIP support
/////////////////////////////////////////////////////////////////////////////

// CNonFSDialog dialog
class CNonFSDialog : public CDialog
{
// Construction
public:
	CNonFSDialog();	
	CNonFSDialog(UINT nIDTemplate, CWnd* pParent);
	CNonFSDialog(LPCTSTR lpszTemplateName, CWnd* pParent);
	virtual ~CNonFSDialog();

	//{{AFX_DATA(CNonFSDialog)	
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNonFSDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CNonFSDialog)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); 
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()
private:
	void init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NONFSDIALOG_H__494B0C5A_1359_4E0D_932C_5DDB1619A744__INCLUDED_)