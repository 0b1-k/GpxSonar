#if !defined(AFX_CCACHEDETAILSFIELDNOTES_H__69DB6BDF_83F8_4EDB_8B64_6EA3AAD98D91__INCLUDED_)
#define AFX_CCACHEDETAILSFIELDNOTES_H__69DB6BDF_83F8_4EDB_8B64_6EA3AAD98D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	NOTE_ACTION_SAVE	1
#define	NOTE_ACTION_DELETE	0

#include "NonFSDialog.h"
#include "CCoords.h"
#include "CMenuEdit.h"

class CCacheDetailsFieldNotes : public CNonFSDialog
{
// Construction
public:
	CCacheDetailsFieldNotes(CWnd* pParent = NULL);
	~CCacheDetailsFieldNotes();

// Dialog Data
	//{{AFX_DATA(CCacheDetailsFieldNotes)
	enum { IDD = IDD_CACHE_FIELDNOTES };
	CMenuEdit	m_NotesCtrl;
	CComboBox	m_LogStatus;
	CString	m_Notes;
	//}}AFX_DATA

	int		m_Status;
	int		m_Action;

	CString	m_Shortname;
	CCoords	m_Coords;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCacheDetailsFieldNotes)
	protected:
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCacheDetailsFieldNotes)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnTb();
	afx_msg void OnCoords();
	afx_msg void OnTimestamp();
	afx_msg void OnRecord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	String	m_ModHash;

protected:
	String	HashValues();

	int		m_RecStatus;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCACHEDETAILSFIELDNOTES_H__69DB6BDF_83F8_4EDB_8B64_6EA3AAD98D91__INCLUDED_)
