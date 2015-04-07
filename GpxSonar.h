#if !defined(AFX_GPXSONAR_H__23CD3C96_58C0_4812_B908_18C18BE2DDD1__INCLUDED_)
#define AFX_GPXSONAR_H__23CD3C96_58C0_4812_B908_18C18BE2DDD1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CCachePageWriter;
class CFilterMgr;
class CGpxParser;
class CGeoCache;
class CTBMgr;
class CCacheMgr;
class CFieldNoteMgr;

#define DEFAULT_AUTOLOAD_FILE	_T("\\DefaultGpxFile.txt")

class CGpxSonarApp : public CWinApp
{
public:
	CCachePageWriter*		m_pCachePageWriter;
	CFilterMgr*				m_pFilterMgr;
	CGpxParser*				m_pGpxParser;
	CGeoCache*				m_pCurrCache;
	CTBMgr*					m_pTBMgr;
	CCacheMgr*				m_pCacheMgr;
	CCeDocListDocTemplate*	m_pDocTemplate;
	CFieldNoteMgr*			m_pFieldNoteMgr;

public:
	CGpxSonarApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpxSonarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGpxSonarApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnLoadGpx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPXSONAR_H__23CD3C96_58C0_4812_B908_18C18BE2DDD1__INCLUDED_)
