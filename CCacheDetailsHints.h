#if !defined(AFX_CCACHEDETAILSHINTS_H__9B2C6BC7_788D_4E0C_9D3C_F501CA04A26D__INCLUDED_)
#define AFX_CCACHEDETAILSHINTS_H__9B2C6BC7_788D_4E0C_9D3C_F501CA04A26D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NonFSDialog.h"

class CCacheDetailsHints : public CNonFSDialog
{
// Construction
public:
	CCacheDetailsHints(CWnd* pParent = NULL);
	~CCacheDetailsHints();

// Dialog Data
	//{{AFX_DATA(CCacheDetailsHints)
	enum { IDD = IDD_CACHE_HINTS };
	CEdit	m_HintsCtrl;
	CString	m_Hints;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCacheDetailsHints)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCacheDetailsHints)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCACHEDETAILSHINTS_H__9B2C6BC7_788D_4E0C_9D3C_F501CA04A26D__INCLUDED_)
