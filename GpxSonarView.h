#if !defined(AFX_GPXSONARVIEW_H__90845E00_DC9C_4E74_B772_434D9A8508F5__INCLUDED_)
#define AFX_GPXSONARVIEW_H__90845E00_DC9C_4E74_B772_434D9A8508F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Afxcview.h"

#include "Literals.h"

#include "CGpxParser.h"
#include "CCachePageWriter.h"
#include "CCoords.h"
#include "CFieldNoteMgr.h"
#include "CTBMgr.h"
#include "CFilterMgr.h"
#include "CCacheReportsPref.h"
#include "CCacheMgr.h"
#include "CExportLocationMgr.h"
#include "IDB_CACHES.h"

#include "CHeading.h"

typedef enum {
	ColType = 0,
	ColWp,
	ColTB,
	ColNote,
	ColFound,
	ColName,
	ColDifficultyRating,
	ColTerrainRating,
	ColDist,
	ColBearing,
	ColOwner,
	ColLastFound,
	ColEndOfList
} GcColDef;

class CSearchDlg;

class CGpxSonarView : public CListView
{
protected: // create from serialization only
	CGpxSonarView();
	DECLARE_DYNCREATE(CGpxSonarView)

// Attributes
public:
	CGpxSonarDoc* GetDocument();

// Operations
public:
	CImageList				m_ImageList;
	CFilterMgr				m_FilterMgr;
	CWPMgr					m_Bookmarks;
	CGpxParser				m_GpxParser;
	CGeoCache*				m_pCurrCache;
	CSearchDlg*				m_pSearchDlg;
	CWnd*					m_pWndMenu;
	CCoords					m_CenterCoords;
	CCachePageWriter		m_CachePageWriter;
	CFieldNoteMgr			m_NotesMgr;
	CTBMgr					m_TBMgr;
	CCacheReportsPref		m_CacheReportPref;
	CExportLocationMgr		m_ExportLocationMgr;
	CString					m_SavedGpxFilename;
	CString					m_LastCacheDetails;
	CString					m_LastFieldNotesReport;
	String					m_Alias;
	String					m_SpoilerPicsPath;
	HeadingCont				m_Headings;
	int						m_CurrSortCol;
	UINT					m_AutoSaveTimer;
	bool					m_NeedToSaveChanges;
	bool					m_InputEnabled;
	bool					m_bTapAndHold;
	bool					m_bUseDblckForCacheDetails;
	bool					m_bReloadLastGpxFile;
	int						m_iItemOnMenu;
	int						m_nLstMenu;
	CPoint					m_ClientCursorPos;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpxSonarView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGpxSonarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void	LoadGpxFile(CString& GpxFilename);

	void	SetupListControl();
	void	UpdateCacheList();
	void	SelectCurrentCache();
	void	ResizeListControl();

	void	ComputeDistanceBearing();

	// Upon loading a GPX file, remove the travel bugs from caches according to their location.
	void	SynchronizeTravelBugs();

	void	LoadConfig();
	void	SaveConfig();

	void	LoadHeadings(CStream& ar);
	void	SaveHeadings(CStream& ar);

	void	ConnectNotes();

	void	ReconnectIgnoredCaches();

	int		CacheNoteBitmapLookup(CGeoCache* pCache);

	// Retrieve the index of the column whose header was clicked on.
	// This function identifies the proper column even if it's no longer visible in the grid
	int		GetClickedColumn(NM_LISTVIEW* pNMListView);

	// Given a column name, returns its index
	int		GetColumnByName(const TCHAR* pName);

	// Returns the actual position of a column given its Id. If the column is not visible, returns -1
	int		GetColumnById(GcColDef Id);

	// Deallocate all the heading definitions
	void	ResetHeadings();

	// Sort the cache list by increasing distance
	void	SortByIncreasingDistance();

	static int CALLBACK CompareType(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareWaypoint(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareTB(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareFound(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareNote(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDifficultyRating(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareTerrainRating(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDistance(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareBearing(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareOwner(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareLastFound(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	PFNLVCOMPARE GetSortFunctionByColId(int Id);

	void ShowCacheDetails();

// Generated message map functions
protected:
	//{{AFX_MSG(CGpxSonarView)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuCacheoptionsShowhints();
	afx_msg void OnLogCache();
	afx_msg void OnMenuSetascenter();
	afx_msg void OnMenuCacheoptionsIgnorecache();
	afx_msg void OnMenuFileMytravelbugs();
	afx_msg void OnMenuFileMycaches();
	afx_msg void OnMenuFileMyalias();
	afx_msg void OnGpxfileinfo();
	afx_msg void OnMenuFileReportsFieldnotes();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuFilters();
	afx_msg void OnMenuListoptionsCentercoords();
	afx_msg void OnDistanceUnits();
	afx_msg void OnMenuListoptionsPreferences();
	afx_msg void OnHelp();
	afx_msg void OnMenuFileRot13helper();
	afx_msg void OnFilters();
	afx_msg void OnUpdateFilters(CCmdUI* pCmdUI);
	afx_msg void OnQuickSearch();
	afx_msg void OnFileExit();
	afx_msg void OnToolsExportExportlocations();
	afx_msg void OnMenuIgnorecachesbythisowner();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileAppconfig();
	afx_msg void OnToolsFieldnotescleaner();
	afx_msg void OnMenuExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	OnContextMenu(CWnd* pWnd, CPoint point) ;

	void	EnableInput();
	void	DisableInput();
	bool	AcceptInput();

	void	RecordColumnSizes();
	void	Cleanup();
	void	DeleteAutoLoadGpxFile();
	void	RememberLastGpxFile();
};

#ifndef _DEBUG  // debug version in GpxSonarView.cpp
inline CGpxSonarDoc* CGpxSonarView::GetDocument()
   { return (CGpxSonarDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPXSONARVIEW_H__90845E00_DC9C_4E74_B772_434D9A8508F5__INCLUDED_)
