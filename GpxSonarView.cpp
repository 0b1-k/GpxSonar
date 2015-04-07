#include "stdafx.h"
#include "GpxSonar.h"
#include "GpxSonarDoc.h"
#include "GpxSonarView.h"
#include "CCenterCoordsDlg.h"
#include "CDistanceUnitsDlg.h"
#include "CBrowserLauncher.h"
#include "CTextTrx.h"
#include "CCacheDetailsHints.h"
#include "CCacheDetailsFieldNotes.h"
#include "CRot13ConvertDlg.h"
#include "CCacheDetailsFieldNotes.h"
#include "CMyTravelBugsDlg.h"
#include "CAboutDlg.h"
#include "CGpxFileInfoDlg.h"
#include "CFilterMgrDlg.h"
#include "CFilterCacheTypes.h"
#include "CFilterCacheContainers.h"
#include "CFilterBearingDistance.h"
#include "CFilterCacheLists.h"
#include "CFilterOnStrings.h"
#include "CFilterSearch.h"
#include "CFilterCacheRatings.h"
#include "CFilterCacheTB.h"
#include "CListPreferencesDlg.h"
#include "CMyAliasDlg.h"
#include "CFieldNotesReportPrefDlg.h"
#include "CMyCachesDlg.h"
#include "CPath.h"
#include "CSearchDlg.h"
#include "CExportLocationDlg.h"
#include "CMiscPreferences.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGpxSonarView, CListView)

BEGIN_MESSAGE_MAP(CGpxSonarView, CListView)
	//{{AFX_MSG_MAP(CGpxSonarView)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENU_CACHEOPTIONS_SHOWHINTS, OnMenuCacheoptionsShowhints)
	ON_COMMAND(ID_LOG_CACHE, OnLogCache)
	ON_COMMAND(ID_MENU_SETASCENTER, OnMenuSetascenter)
	ON_COMMAND(ID_MENU_CACHEOPTIONS_IGNORECACHE, OnMenuCacheoptionsIgnorecache)
	ON_COMMAND(ID_MENU_FILE_MYTRAVELBUGS, OnMenuFileMytravelbugs)
	ON_COMMAND(ID_MENU_FILE_MYCACHES, OnMenuFileMycaches)
	ON_COMMAND(ID_MENU_FILE_MYALIAS, OnMenuFileMyalias)
	ON_COMMAND(ID_GPXFILEINFO, OnGpxfileinfo)
	ON_COMMAND(ID_MENU_FILE_REPORTS_FIELDNOTES, OnMenuFileReportsFieldnotes)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_FILTERS, OnMenuFilters)
	ON_COMMAND(ID_MENU_LISTOPTIONS_CENTERCOORDS, OnMenuListoptionsCentercoords)
	ON_COMMAND(ID_DISTANCE_UNITS, OnDistanceUnits)
	ON_COMMAND(ID_MENU_LISTOPTIONS_PREFERENCES, OnMenuListoptionsPreferences)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_MENU_FILE_ROT13HELPER, OnMenuFileRot13helper)
	ON_COMMAND(ID_FILTERS, OnFilters)
	ON_UPDATE_COMMAND_UI(ID_FILTERS, OnUpdateFilters)
	ON_COMMAND(ID_QUICK_SEARCH, OnQuickSearch)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_TOOLS_EXPORT_EXPORTLOCATIONS, OnToolsExportExportlocations)
	ON_COMMAND(ID_MENU_IGNORECACHESBYTHISOWNER, OnMenuIgnorecachesbythisowner)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_FILE_APPCONFIG, OnFileAppconfig)
	ON_COMMAND(ID_TOOLS_FIELDNOTESCLEANER, OnToolsFieldnotescleaner)
	ON_NOTIFY_REFLECT(GN_CONTEXTMENU, OnContextMenu)
	ON_COMMAND(ID_MENU_EXPORT, OnMenuExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGpxSonarView::CGpxSonarView()
{
	m_InputEnabled = true;
	m_bTapAndHold = false;
	m_iItemOnMenu = -1;
	m_pWndMenu = 0;

	m_NeedToSaveChanges = false;

	m_pSearchDlg = 0;

	m_bUseDblckForCacheDetails = false;
	m_bReloadLastGpxFile = true;

	CFilterCacheTypes*		pFilterCacheTypes = new CFilterCacheTypes(_T("Cache Types"), FilterCacheTypes);
	CFilterCacheContainers* pFilterCacheContainers = new CFilterCacheContainers(_T("Cache Containers"), FilterContainerTypes);
	CFilterBearingDistance*	pFilterBearingDistance = new CFilterBearingDistance(_T("Bearing & Distance"), FilterBearingDistance);
	CFilterCacheLists*		pFilterCacheLists = new CFilterCacheLists(_T("Cache Attributes"), FilterCacheLists);
	CFilterOnStrings*		pFilterStateList = new CFilterOnStrings(_T("States"), FilterStateList);
	CFilterOnStrings*		pFilterCountryList = new CFilterOnStrings(_T("Countries"), FilterCountryList);
	CFilterSearch*			pFilterSearch = new CFilterSearch(_T("Search"), FilterSearch);
	CFilterCacheRatings*	pFilterRatings = new CFilterCacheRatings(_T("Cache Ratings"), FilterRatings);
	CFilterCacheTB*			pFilterCacheTB = new CFilterCacheTB(_T("Cache With TB"), FilterTravelBugs);

	m_FilterMgr.Add(pFilterCacheTypes);
	m_FilterMgr.Add(pFilterCacheContainers);
	m_FilterMgr.Add(pFilterBearingDistance);
	m_FilterMgr.Add(pFilterCacheLists);
	m_FilterMgr.Add(pFilterStateList);
	m_FilterMgr.Add(pFilterCountryList);
	m_FilterMgr.Add(pFilterSearch);
	m_FilterMgr.Add(pFilterRatings);
	m_FilterMgr.Add(pFilterCacheTB);

	// Default column widths expressed in pixels
	enum ColWidths { 
		CACHE_TYPE_COL_WIDTH = 12,
		CACHE_WAYPOINT_COL_WIDTH = 72,
		CACHE_TB_COL_WIDTH = 10,
		CACHE_NOTE_COL_WIDTH = 10,
		CACHE_CACHE_FOUND_COL_WIDTH = 10,
		CACHE_NAME_COL_WIDTH = 200,
		CACHE_RATING_COL_WIDTH = 48,
		CACHE_DIST_COL_WIDTH = 56,
		CACHE_BEARING_COL_WIDTH = 48,
		CACHE_OWNER_COL_WIDTH = 100,
		CACHE_LAST_FOUND_COL_WIDTH = 100
		};

	#define MINIMUM_COL_WIDTH	10
	#define	DISTANCE_COLUMN		_T("Dist.")
	#define	SEARCHABLE			true
	#define LAST_LOG_COL_LABEL	_T("Last Log")

	m_Headings.push_back(new CHeading(ColType, _T("Type"), CACHE_TYPE_COL_WIDTH, GetSortFunctionByColId(ColType)));
	m_Headings.push_back(new CHeading(ColWp, _T("Waypoint"), CACHE_WAYPOINT_COL_WIDTH, GetSortFunctionByColId(ColWp), SEARCHABLE));
	m_Headings.push_back(new CHeading(ColTB, _T("TB"),	CACHE_TB_COL_WIDTH, GetSortFunctionByColId(ColTB)));
	m_Headings.push_back(new CHeading(ColNote, _T("Note"),	CACHE_TB_COL_WIDTH, GetSortFunctionByColId(ColNote)));
	m_Headings.push_back(new CHeading(ColFound, _T("Found"),	CACHE_CACHE_FOUND_COL_WIDTH, GetSortFunctionByColId(ColFound)));
	m_Headings.push_back(new CHeading(ColName, _T("Name"), CACHE_NAME_COL_WIDTH, GetSortFunctionByColId(ColName), SEARCHABLE));
	m_Headings.push_back(new CHeading(ColDifficultyRating, _T("Diff."), CACHE_RATING_COL_WIDTH, GetSortFunctionByColId(ColDifficultyRating)));
	m_Headings.push_back(new CHeading(ColTerrainRating, _T("Terr."), CACHE_RATING_COL_WIDTH, GetSortFunctionByColId(ColTerrainRating)));
	m_Headings.push_back(new CHeading(ColDist, DISTANCE_COLUMN, CACHE_DIST_COL_WIDTH, GetSortFunctionByColId(ColDist)));
	m_Headings.push_back(new CHeading(ColBearing, _T("Bearing"), CACHE_BEARING_COL_WIDTH, GetSortFunctionByColId(ColBearing)));
	m_Headings.push_back(new CHeading(ColOwner, _T("Owner"), CACHE_OWNER_COL_WIDTH, GetSortFunctionByColId(ColOwner), SEARCHABLE));
	m_Headings.push_back(new CHeading(ColLastFound, LAST_LOG_COL_LABEL, CACHE_LAST_FOUND_COL_WIDTH, GetSortFunctionByColId(ColLastFound)));
}

PFNLVCOMPARE CGpxSonarView::GetSortFunctionByColId(int Id)
{
	switch (Id)
	{
	case ColType:
		return CompareType;
		break;
	case ColWp:
		return CompareWaypoint;
		break;
	case ColTB:
		return CompareTB;
		break;
	case ColNote:
		return CompareNote;
		break;
	case ColFound:
		return CompareFound;
		break;
	case ColName:
		return CompareName;
		break;
	case ColDifficultyRating:
		return CompareDifficultyRating;
		break;
	case ColTerrainRating:
		return CompareTerrainRating;
		break;
	case ColDist:
		return CompareDistance;
		break;
	case ColBearing:
		return CompareBearing;
		break;
	case ColOwner:
		return CompareOwner;
		break;
	case ColLastFound:
		return CompareLastFound;
		break;
	}

	return CompareType;
}

CGpxSonarView::~CGpxSonarView()
{
	SaveConfig();

	Cleanup();
}

void CGpxSonarView::Cleanup()
{
	m_ImageList.DeleteImageList();

	ResetHeadings();

	if (!m_LastCacheDetails.IsEmpty())
	{
		DeleteFile((LPCTSTR) m_LastCacheDetails);
	}

	if (!m_LastFieldNotesReport.IsEmpty())
	{
		DeleteFile((LPCTSTR) m_LastFieldNotesReport);
	}
}

void CGpxSonarView::DeleteAutoLoadGpxFile()
{
	String	GpxFile;
	
	CPath Path;

	GpxFile = Path.BuildPath(DEFAULT_AUTOLOAD_FILE);

	// By default, kill the autoload file (in case some error occurs)
	DeleteFile(GpxFile.c_str());
}

void CGpxSonarView::RememberLastGpxFile()
{
	CFile	F;

	bool FailedToWrite = false;

	String	GpxFile;
	
	CPath Path;

	GpxFile = Path.BuildPath(DEFAULT_AUTOLOAD_FILE);

	if (F.Open(GpxFile.c_str(), CFile::modeCreate | CFile::modeWrite, NULL))
	{
		F.Write((LPCTSTR) m_SavedGpxFilename, m_SavedGpxFilename.GetLength() * sizeof(TCHAR));
		F.Close();
	}

	if (FailedToWrite)
	{
		DeleteFile(GpxFile.c_str());
	}
}

void CGpxSonarView::ResetHeadings()
{
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		delete (*I);
	}

	m_Headings.clear();
}

BOOL CGpxSonarView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = cs.style | LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

void CGpxSonarView::OnDraw(CDC* pDC)
{
	CGpxSonarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

#ifdef _DEBUG
void CGpxSonarView::AssertValid() const
{
	CListView::AssertValid();
}

void CGpxSonarView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CGpxSonarDoc* CGpxSonarView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGpxSonarDoc)));
	return (CGpxSonarDoc*)m_pDocument;
}
#endif //_DEBUG

int CGpxSonarView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	int iResult = CListView::OnCreate(lpCreateStruct);

	if(iResult == 0)
		ShowDoneButton(TRUE);

	return iResult;
}
void CGpxSonarView::OnSetFocus(CWnd* pOldWnd)
{
	CListView::OnSetFocus(pOldWnd);
}

void CGpxSonarView::OnDestroy()
{
	RecordColumnSizes();

	CListView::OnDestroy();
}

void CGpxSonarView::RecordColumnSizes()
{
	KillTimer(m_AutoSaveTimer);

	CListCtrl& ListCtrl = GetListCtrl();

	int Col = 0;

	// Save the width of the columns
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		if ((*I)->m_Visible)
		{
			(*I)->m_Length = ListCtrl.GetColumnWidth(Col);

			Col++;
		}
	}
}

BOOL CGpxSonarView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CGpxSonarView::OnInitialUpdate() 
{
	GetListCtrl().SetExtendedStyle(GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);

	((CGpxSonarApp*) AfxGetApp())->m_pTBMgr = &m_TBMgr;
	((CGpxSonarApp*) AfxGetApp())->m_pGpxParser = &m_GpxParser;
	((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr = &m_FilterMgr;
	((CGpxSonarApp*) AfxGetApp())->m_pCachePageWriter = &m_CachePageWriter;
	((CGpxSonarApp*) AfxGetApp())->m_pFieldNoteMgr = &m_NotesMgr;

	m_pCurrCache = 0;
	m_CurrSortCol = 0;

	CListView::OnInitialUpdate();
	
	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	// Setup the icon list for the list control
	m_ImageList.Create(IDB_CACHES, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	GetListCtrl().SetImageList(&m_ImageList, LVSIL_SMALL);

	// Load the current configuration
	LoadConfig();

	// Initialize the list control
	SetupListControl();

	CString GpxFilename = GetDocument()->GetPathName();

	LoadGpxFile(GpxFilename);

	CPath Path;

	String TargetFname = Path.BuildPath(CACHE_PAGE_TEMPLATE);

	// Load the HTML template used to create cache pages
	m_CachePageWriter.LoadTemplate(TargetFname.c_str());

	#define	AUTO_SAVE_TIMER	1967
	#define	THIRTY_SECS		30000

	// Create the autosave timer
	m_AutoSaveTimer = SetTimer(AUTO_SAVE_TIMER, THIRTY_SECS, 0);

	BringWindowToTop();
}

void CGpxSonarView::OnTimer(UINT nIDEvent) 
{
	if (m_NeedToSaveChanges && nIDEvent == AUTO_SAVE_TIMER)
	{
		SaveConfig();
	}
		
	CListView::OnTimer(nIDEvent);
}

#define GpxSonarConfigVersion	200

// Load the current configuration
void CGpxSonarView::LoadConfig()
{
	CPath	Path;

	String	Fname = Path.BuildPath(GPXSONAR_CONFIG_FILENAME_LEGACY);

	// Always initialize the distance units
	m_CenterCoords.SetDistanceUnits(DIST_UNITS_IN_STATUTE_MILES);

	// LEGACY - Check if there's a legacy (Version < 2.0) GpxSonar.dat file
	if (GetFileAttributes(Fname.c_str()) != -1)
	{
		CCacheMgr	CacheMgr;

		CStream	ar;

		ar.Load(Fname);

		m_CenterCoords.Serialize(ar);
		m_NotesMgr.Serialize(ar);
		m_TBMgr.Serialize(ar);
		m_FilterMgr.Serialize(ar);
		m_GpxParser.Serialize(ar);

		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			LoadHeadings(ar);
		}

		if (Version >= 101)
		{
			ar >> m_Alias;
		}

		if (Version >= 102)
		{
			m_CacheReportPref.Serialize(ar);
		}

		if (Version >= 103)
		{
			CacheMgr.Serialize(ar);
		}

		if (Version >= 104)
		{
			m_Bookmarks.Serialize(ar);
		}

		if (Version >= 105)
		{
			m_ExportLocationMgr.Serialize(ar);
		}

		if (Version >= 106)
		{
			ar >> m_bUseDblckForCacheDetails;
			ar >> m_bReloadLastGpxFile;
		}

		if (Version >= 107)
		{
			ar >> m_SpoilerPicsPath;
		}

		// As of v2.0: major change in the way the configuration of the application works!
		// Field notes are no longer embedded in GpxSonar.dat
		// Caches from the cache manager are no longer embedded in GpxSonar.dat
		// The configuration file gets renamed to reflect the version #.
		// There are more than one config files...
		if (Version < 200)
		{
			// By calling SaveConfig() here, it forces a conversion process to take place to the new format
			SaveConfig();

			// Flush the configuration of the notes mgr and of the cache mgr
			m_NotesMgr.SaveConfig();

			// Save the content of the cache manager
			CacheMgr.SaveConfig();

			String	ArchiveFname = Path.BuildPath(GPXSONAR_CONFIG_FILENAME_LEGACY_ARCHIVE);

			// Rename the legacy configuration file so that it does not get loaded again
			MoveFile((LPCTSTR) Fname.c_str(), (LPCTSTR) ArchiveFname.c_str());
		}
	}

	Fname = Path.BuildPath(GPXSONAR_CONFIG_FILENAME_2);

	// Check if there's a 2.x configuration filename
	if (GetFileAttributes(Fname.c_str()) != -1)
	{
		CStream	ar;

		ar.Load(Fname);

		int Version;

		ar >> Version;

		if (Version >= 200)
		{
			ar >> m_Alias;
			ar >> m_bUseDblckForCacheDetails;
			ar >> m_bReloadLastGpxFile;
			ar >> m_SpoilerPicsPath;

			// Placeholders for future settings
			int Temp;

			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
			ar >> Temp;
	
			LoadHeadings(ar);

			m_CenterCoords.Serialize(ar);
			m_TBMgr.Serialize(ar);
			m_FilterMgr.Serialize(ar);
			m_GpxParser.Serialize(ar);
			m_CacheReportPref.Serialize(ar);
			m_Bookmarks.Serialize(ar);
			m_ExportLocationMgr.Serialize(ar);

			m_NotesMgr.LoadConfig();
		}
	}
}

void CGpxSonarView::LoadHeadings(CStream& ar)
{
	ResetHeadings();

	int Count;

	ar >> Count;

	while (Count--)
	{
		CHeading* pHeading = new CHeading;

		pHeading->Serialize(ar);

		// Set the sort function pointer based on the column Id
		pHeading->m_pSortFunc = GetSortFunctionByColId(pHeading->m_Id);

		m_Headings.push_back(pHeading);
	}

	// Fixup
	m_Headings[ColLastFound]->m_Name = LAST_LOG_COL_LABEL;
}

void CGpxSonarView::SaveHeadings(CStream& ar)
{
	// Save the configuration of the columns
	ar << m_Headings.size();

	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		(*I)->Serialize(ar);
	}
}

void CGpxSonarView::SaveConfig()
{
	KillTimer(m_AutoSaveTimer);

	CPath	Path;

	String	TargetFname = Path.BuildPath(GPXSONAR_CONFIG_FILENAME_2);

	CStream	ar;

	ar.SetStoring(true);

	ar << GpxSonarConfigVersion;

	ar << m_Alias;
	ar << m_bUseDblckForCacheDetails;
	ar << m_bReloadLastGpxFile;
	ar << m_SpoilerPicsPath;

	// Placeholders for future settings
	ar << 0;
	ar << 0;
	ar << 0;
	ar << 0;
	ar << 0;
	ar << 0;
	ar << 0;
	ar << 0;
	ar << 0;
	ar << 0;

	SaveHeadings(ar);

	m_CenterCoords.Serialize(ar);
	m_TBMgr.Serialize(ar);
	m_FilterMgr.Serialize(ar);
	m_GpxParser.Serialize(ar);
	m_CacheReportPref.Serialize(ar);
	m_Bookmarks.Serialize(ar);
	m_ExportLocationMgr.Serialize(ar);

	ar.Save(TargetFname);

	m_NeedToSaveChanges = false;

	// Restore the auto-save timer
	m_AutoSaveTimer = SetTimer(AUTO_SAVE_TIMER, THIRTY_SECS, 0);
}

void CGpxSonarView::SetupListControl()
{
	int Col = 0;

	// Add the headers to the list control
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		if ((*I)->m_Visible)
		{
			if ((*I)->m_Length < MINIMUM_COL_WIDTH)
			{
				(*I)->m_Length = MINIMUM_COL_WIDTH;
			}

			GetListCtrl().InsertColumn(Col++, (*I)->m_Name.c_str(), LVCFMT_LEFT, (*I)->m_Length);
		}
	}
}

void CGpxSonarView::UpdateCacheList()
{
	LVITEM		Item;
	itGC		C;
	long		Count = 0;
	TCHAR		Buffer[20];
	int			OwnerSize = m_Alias.size();
	TCHAR*		pOwner = (TCHAR*) m_Alias.c_str();

	BeginWaitCursor();

	CListCtrl& CacheList = GetListCtrl();

	CacheList.DeleteAllItems();

	// Apply the filters on the caches before refreshing the list
	m_FilterMgr.Filter(m_GpxParser);

	CGeoCache* pCache = m_GpxParser.First(C);

	while (!m_GpxParser.EndOfCacheList(C))
	{
		if (pCache->m_InScope)
		{
			int	Col = 0;

			// Image of cache type
			Item.mask =  LVIF_IMAGE;
			Item.iItem = Count;
			Item.iSubItem = 0;
			Item.iImage = pCache->TypeLookup();

			// Preserve the index of the cache object in the list
			pCache->m_ListIndex = CacheList.InsertItem(&Item);

			CacheList.SetItemData(pCache->m_ListIndex, (DWORD) pCache->This());
			
			if (m_Headings[++Col]->m_Visible)
			{
				// Waypoint name
				Item.mask =  LVIF_TEXT;
				Item.iSubItem++;
				Item.pszText = (TCHAR*) pCache->m_Shortname.c_str();

				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// TB(s) in the cache?
				Item.mask =  LVIF_IMAGE;
				Item.iSubItem++;

				if (pCache->GetTBCount())
				{
					Item.iImage = TB_PRESENT;
				}
				else
				{
					Item.iImage = EMPTY_BITMAP;
				}
				
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Field note?
				Item.mask =  LVIF_IMAGE;
				Item.iSubItem++;

				if (pCache->m_pFieldNote)
				{
					Item.iImage = CacheNoteBitmapLookup(pCache);
				}
				else
				{
					Item.iImage = EMPTY_BITMAP;
				}
				
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Found the cache already?
				Item.mask =  LVIF_IMAGE;
				Item.iSubItem++;

				if (pCache->m_GsCacheArchived)
				{
					Item.iImage = CACHE_ARCHIVED;
				}
				else if (!pCache->m_GsCacheAvailable)
				{
					Item.iImage = CACHE_DISABLED;
				}
				else if (pCache->m_Sym.empty())
				{
					if (OwnerSize && !_tcsnicmp(pCache->m_GsCacheOwnerName.c_str(), pOwner, OwnerSize))
					{
						Item.iImage = MY_CACHE;
					}
					else
					{
						Item.iImage = EMPTY_BITMAP;
					}
				}
				else
				{
					Item.iImage = FOUND_CACHE;
				}
				
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Cache name
				Item.mask =  LVIF_TEXT;
				if (pCache->m_GsCacheName.empty() || pCache->m_GsCacheName[0] == _T('\r') || pCache->m_GsCacheName[0] == _T('\n'))
				{
					pCache->m_GsCacheName = DEFAULT_CACHE_NAME;
				}

				Item.iSubItem++;
				Item.pszText = (TCHAR*) pCache->m_GsCacheName.c_str();
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Difficulty Rating
				Item.iSubItem++;
				Item.mask =  LVIF_TEXT;
				_stprintf(Buffer, _T("%.1f"), pCache->m_GsCacheDifficulty);
				Item.pszText = Buffer;
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Terrain Rating
				Item.iSubItem++;
				Item.mask =  LVIF_TEXT;
				_stprintf(Buffer, _T("%.1f"), pCache->m_GsCacheTerrain);
				Item.pszText = Buffer;
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Distance
				Item.iSubItem++;
				_stprintf(Buffer, _T("%.02f"), pCache->m_Distance);
				Item.pszText = Buffer;
				CacheList.SetItem(&Item);
			}
			
			if (m_Headings[++Col]->m_Visible)
			{
				// Bearing
				Item.iSubItem++;
				Item.pszText = (TCHAR*) pCache->m_Bearing.c_str();
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Owner
				Item.iSubItem++;
				Item.pszText = (TCHAR*) pCache->m_GsCacheOwnerName.c_str();
				CacheList.SetItem(&Item);
			}

			if (m_Headings[++Col]->m_Visible)
			{
				// Last Found date
				pCache->LastFoundText(Buffer);
				Item.iSubItem++;
				Item.pszText = Buffer;
				CacheList.SetItem(&Item);
			}

			Count++;
		}

		pCache = m_GpxParser.Next(C);
	}
	
	if (!m_GpxParser.CacheCount())
	{
		MessageBox(_T("Empty GPX File."), _T("No Data"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		if (!CacheList.GetItemCount())
		{
			MessageBox(_T("No Cache Matches Your Criteria."), _T("No Data"), MB_OK | MB_ICONINFORMATION);
		}
	}

	EndWaitCursor();
}

int CGpxSonarView::CacheNoteBitmapLookup(CGeoCache* pCache)
{
	if (pCache->m_pFieldNote)
	{
		switch(pCache->m_pFieldNote->m_Status)
		{
		case NoteStatusFoundIt:
			return FIELD_NOTE_FOUND;
			break;
		case NoteStatusDNF:
			return FIELD_NOTE_NOTFOUND;
			break;
		case NoteStatusNote:
			return FIELD_NOTE;
			break;
		case NoteStatusArchiveIt:
			return FIELD_NOTE_ARCHIVE;
			break;
		}
	}

	return EMPTY_BITMAP;
}

void CGpxSonarView::LoadGpxFile(CString& GpxFilename)
{
	// Delete the file used to recall the last opened GPX file
	DeleteAutoLoadGpxFile();

	if (!GpxFilename.GetLength())
	{
		return;
	}

	BeginWaitCursor();

	GpxLoadStatus Status;
	
	Status = m_GpxParser.Load((LPCTSTR)GpxFilename);

	switch (Status)
	{
	case GpxLoadStatusFailed:

		MessageBox(m_GpxParser.GetErrorMsg().c_str(), _T("GPX File Loading Failure"), MB_OK | MB_ICONWARNING);

		break;

	case GpxLoadStatusParserException:
		{
			int Answer = MessageBox(m_GpxParser.GetErrorMsg().c_str(), _T("Malformed GPX File"), MB_OKCANCEL | MB_ICONWARNING);
		}
		break;

	case GpxLoadStatusOk:

		ConnectNotes();

		m_SavedGpxFilename = GpxFilename;

		// Save the name of the GPX file for later use by the export functions
		m_ExportLocationMgr.SetGpxFilename((LPCTSTR) m_SavedGpxFilename);

		SynchronizeTravelBugs();

		ReconnectIgnoredCaches();

		{ // These 3 calls must be together
			ComputeDistanceBearing();
			UpdateCacheList();
			SortByIncreasingDistance();
		}

		// Save the name of the GPX file that was last opened successfully
		if (m_bReloadLastGpxFile)
		{
			RememberLastGpxFile();
		}

		break;
	};

	EndWaitCursor();
}

void CGpxSonarView::ConnectNotes()
{
	itGC it;

	BeginWaitCursor();

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		// If this cache was not found, kill the symbol
		if (pCache->m_Sym != GEOCACHE_FOUND)
		{
			pCache->m_Sym.erase();
		}

		pCache->m_pFieldNote = m_NotesMgr.Find(pCache->m_Shortname);

		if (pCache->m_pFieldNote)
		{
			pCache->m_pFieldNote->m_pCache = pCache;
		}

		pCache = m_GpxParser.Next(it);
	}	

	EndWaitCursor();
}

// Upon loading a GPX file, remove the travel bugs from caches according to their location.
void CGpxSonarView::SynchronizeTravelBugs()
{
	itGC it;

	BeginWaitCursor();

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		itTB	TB;
		TBCont	TmpList;

		long Size = 0;

		// Cache has TBs?
		if (pCache->GetTBCount())
		{
			// Make a list of them
			CTravelBug*	pTB = pCache->FirstTB(TB);

			while (!pCache->EndOfTBList(TB))
			{
				TmpList.push_back(pTB);

				pTB = pCache->NextTB(TB);
			}

			Size = TmpList.size();

			// If a TB is in the 'inventory' or is in a different cache, remove it from the cache
			for (long I = 0; I < Size; I++)
			{
				pTB = TmpList[I];

				CTB* pInvTB = m_TBMgr.Find(pTB->m_Ref);

				if (pInvTB)
				{
					// remove the TB from the cache if it's listed somewhere else.
					if (pInvTB->m_CacheShortName != pCache->m_Shortname)
					{
						pCache->RemoveTravelBug(pTB);
					}
				}
			}
		}

		itTB2 TB2;

		// Finally, put any travel bug in the cache which was placed into it from the inventory
		CTB* pInvTB = m_TBMgr.First(TB2);

		while (!m_TBMgr.EndOfList(TB2))
		{
			if (pInvTB->m_CacheShortName == pCache->m_Shortname)
			{
				bool AlreadyInCache = false;

				// Place the TB only if it's not already in the cache
				for (long I = 0; I < Size; I++)
				{
					CTravelBug*	pTB = TmpList[I];

					if (pTB->m_Ref == pInvTB->m_Ref)
					{
						AlreadyInCache = true;
						break;
					}
				}

				if (!AlreadyInCache)
				{
					CTravelBug*	pTB = new CTravelBug;

					if (!pTB)
					{
						return;
					}

					pTB->m_Id = pInvTB->m_Id;
					pTB->m_Name = pInvTB->m_Name;
					pTB->m_Ref = pInvTB->m_Ref;

					pCache->AddTravelBug(pTB);
				}
			}

			pInvTB = m_TBMgr.Next(TB2);
		}

		// Empty the list
		TmpList.clear();

		pCache = m_GpxParser.Next(it);
	}	

	EndWaitCursor();
}

void CGpxSonarView::ReconnectIgnoredCaches()
{
	CFilterCacheLists* pFCL = (CFilterCacheLists*) m_FilterMgr.Find(FilterCacheLists);

	itGC it;

	BeginWaitCursor();

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		// Lookup the Id of the cache...
		if (pFCL->Find(pCache->m_GsCacheId))
		{
			// Flag the cache as ignored if found
			pCache->m_Ignored = true;
		}

		// Run the cache through the filter to determine if it's still in scope
		pFCL->OnFilterCache(pCache);

		pCache = m_GpxParser.Next(it);
	}	

	EndWaitCursor();
}

void CGpxSonarView::ComputeDistanceBearing()
{
	CCoords	CachePoint;

	double		Units = m_CenterCoords.GetDistanceUnits();
	
	double		ForwardAzimuth, ReverseAzimuth;

	BeginWaitCursor();

	itGC it;

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		// Distance
		CachePoint.SetDecimal(pCache->m_Lat, pCache->m_Long);

		pCache->m_Distance = (m_CenterCoords.VincentyDistance(CachePoint, &ForwardAzimuth, &ReverseAzimuth)) / Units;

		// Bearing
		if ((ForwardAzimuth >= 338.0 && ForwardAzimuth <= 360.0) || (ForwardAzimuth >= 0.0 && ForwardAzimuth < 24.0))
		{
			pCache->m_Bearing = BEARING_NORTH;
		}
		else if (ForwardAzimuth >= 24.0 && ForwardAzimuth < 70.0)
		{
			pCache->m_Bearing = BEARING_NORTHEAST;
		}
		else if (ForwardAzimuth >= 70.0 && ForwardAzimuth < 116.0)
		{
			pCache->m_Bearing = BEARING_EAST;
		}
		else if (ForwardAzimuth >= 116.0 && ForwardAzimuth < 162.0)
		{
			pCache->m_Bearing = BEARING_SOUTHEAST;
		}
		else if (ForwardAzimuth >= 162.0 && ForwardAzimuth < 208.0)
		{
			pCache->m_Bearing = BEARING_SOUTH;
		}
		else if (ForwardAzimuth >= 208.0 && ForwardAzimuth < 254.0)
		{
			pCache->m_Bearing = BEARING_SOUTHWEST;
		}
		else if (ForwardAzimuth >= 254.0 && ForwardAzimuth < 300.0)
		{
			pCache->m_Bearing = BEARING_WEST;
		}
		else if (ForwardAzimuth >= 300.0 && ForwardAzimuth < 338.0)
		{
			pCache->m_Bearing = BEARING_NORTHWEST;
		}
		else
		{
			pCache->m_Bearing = _T("?");
		}

		pCache = m_GpxParser.Next(it);
	}

	// Sort the actual cache container according to their distance from the center
	m_GpxParser.SortByDistance();

	EndWaitCursor();
}

// Sort the cache list by increasing distance
void CGpxSonarView::SortByIncreasingDistance()
{
	GetColumnByName(DISTANCE_COLUMN);

	// Sort by ascending distance
	m_Headings[m_CurrSortCol]->m_SortToggle = 1;

	GetListCtrl().SortItems(m_Headings[m_CurrSortCol]->m_pSortFunc, (LPARAM) this);
}

void CGpxSonarView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	GetClickedColumn(pNMListView);

	m_Headings[m_CurrSortCol]->m_SortToggle *= -1;

	GetListCtrl().SortItems(m_Headings[m_CurrSortCol]->m_pSortFunc, (LPARAM) this);

	*pResult = 0;
}

// Retrieve the index of the column whose header was clicked on.
// This function identifies the proper column even if it's no longer visible in the grid
int CGpxSonarView::GetClickedColumn(NM_LISTVIEW* pNMListView)
{
	LVCOLUMN	LvCol;

	#define	MAX_COLUMN_TEXT	100

	TCHAR	Buffer[MAX_COLUMN_TEXT];

	LvCol.mask = LVCF_TEXT; 
	LvCol.pszText = Buffer; 
	LvCol.cchTextMax = MAX_COLUMN_TEXT; 
	
	GetListCtrl().GetColumn(pNMListView->iSubItem, &LvCol);

	return GetColumnByName(Buffer);
}

// Returns the actual position of a column given its Id. If the column is not visible, returns -1
int CGpxSonarView::GetColumnById(GcColDef Id)
{
	int Pos = 0;
	
	// Using the name of the column, locate the matching heading
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		if ((*I)->m_Visible)
		{
			if ((*I)->m_Id == Id)
			{
				return Pos;
			}

			Pos++;
		}
	}	

	return -1;
}

// Given a column name, returns its index
int CGpxSonarView::GetColumnByName(const TCHAR* pName)
{
	m_CurrSortCol = 0;

	// Using the name of the column, locate the matching heading
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		if ((*I)->m_Name == pName)
		{
			return m_CurrSortCol;
		}

		m_CurrSortCol++;
	}	

	return 0;
}

int CALLBACK CGpxSonarView::CompareType(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;

	if (pGC1->TypeLookup() < pGC2->TypeLookup())
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->TypeLookup() > pGC2->TypeLookup())
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}

	return 0;
}

int CALLBACK CGpxSonarView::CompareWaypoint(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;

	int Rc = _tcsicmp(pGC1->m_Shortname.c_str(), pGC2->m_Shortname.c_str());

	if (Rc < 0)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (Rc > 0)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}

	return 0;
}

int CALLBACK CGpxSonarView::CompareTB(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	if (pGC1->GetTBCount() < pGC2->GetTBCount())
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->GetTBCount() > pGC2->GetTBCount())
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareFound(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	if (pGC1->m_Sym.empty() < pGC2->m_Sym.empty())
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->m_Sym.empty() > pGC2->m_Sym.empty())
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareNote(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	bool FN1 = false;
	bool FN2 = false;

	if (pGC1->m_pFieldNote)
	{
		FN1 = true;
	}

	if (pGC2->m_pFieldNote)
	{
		FN2 = true;
	}

	if (FN1 < FN2)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (FN1 > FN2)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	int Rc = _tcsicmp(pGC1->m_GsCacheName.c_str(), pGC2->m_GsCacheName.c_str());

	if (Rc < 0)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (Rc > 0)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareDifficultyRating(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	if (pGC1->m_GsCacheDifficulty < pGC2->m_GsCacheDifficulty)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->m_GsCacheDifficulty > pGC2->m_GsCacheDifficulty)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareTerrainRating(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	if (pGC1->m_GsCacheTerrain < pGC2->m_GsCacheTerrain)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->m_GsCacheTerrain > pGC2->m_GsCacheTerrain)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareDistance(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	if (pGC1->m_Distance < pGC2->m_Distance)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->m_Distance > pGC2->m_Distance)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareBearing(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	if (pGC1->m_Bearing < pGC2->m_Bearing)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->m_Bearing > pGC2->m_Bearing)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareOwner(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	int Rc = _tcsicmp(pGC1->m_GsCacheOwnerName.c_str(), pGC2->m_GsCacheOwnerName.c_str());

	if (Rc < 0)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (Rc > 0)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CGpxSonarView::CompareLastFound(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CGpxSonarView* pThis = (CGpxSonarView*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	TCHAR	Date1[20];
	TCHAR	Date2[20];

	pGC1->LastFoundText(Date1);
	pGC2->LastFoundText(Date2);

	int Res = _tcscmp(Date1, Date2);

	if (Res < 0)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (Res > 0)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

void CGpxSonarView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (!m_bUseDblckForCacheDetails)
	{
		if (AcceptInput())
		{
			if (m_iItemOnMenu > -1)
			{
				GetListCtrl().SetItemState(m_iItemOnMenu, LVIS_SELECTED, LVIS_SELECTED);

				ShowCacheDetails();
			}
		}
	}

	*pResult = 0;
}

void CGpxSonarView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bUseDblckForCacheDetails)
	{
		NM_LISTVIEW* pLV = (NM_LISTVIEW*) pNMHDR;

		// Convert the NM_LISTVIEW.ptAction POINT into a CPoint
		CPoint Click(pLV->ptAction);

		UINT HitTestFlag = LVHT_ONITEM | LVHT_ONITEMICON | LVHT_ONITEMLABEL | LVHT_ONITEMSTATEICON;

		// the return value of listcontrol.HitTest(CPoint, NULL) will be the item clicked if not -1.
		int iItem = GetListCtrl().HitTest(Click, &HitTestFlag);

		if (iItem != -1)
		{
			m_pCurrCache = (CGeoCache*) GetListCtrl().GetItemData(iItem);
		}
		else
		{
			m_pCurrCache = 0;
		}

		((CGpxSonarApp*) AfxGetApp())->m_pCurrCache = m_pCurrCache;

		ShowCacheDetails();
	}

	*pResult = 0;
}

void CGpxSonarView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (AcceptInput())
	{
		m_bTapAndHold = false;		
	}

	CListView::OnLButtonUp(nFlags, point);
}

void CGpxSonarView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (AcceptInput())
	{
		m_ClientCursorPos = point;

		m_bTapAndHold = false;
		
		UINT HitTestFlag = LVHT_ONITEM | LVHT_ONITEMICON | LVHT_ONITEMLABEL | LVHT_ONITEMSTATEICON;

		// the return value of listcontrol.HitTest(CPoint, NULL) will be the item clicked if not -1.
		m_iItemOnMenu = GetListCtrl().HitTest(point, &HitTestFlag);

		if (m_iItemOnMenu != -1)
		{
			m_pCurrCache = (CGeoCache*) GetListCtrl().GetItemData(m_iItemOnMenu);
		}
		else
		{
			m_pCurrCache = 0;
		}

		((CGpxSonarApp*) AfxGetApp())->m_pCurrCache = m_pCurrCache;
	}

	CListView::OnLButtonDown(nFlags, point);
}

void CGpxSonarView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    CMenu        mnuCtxt;
    CMenu*       pMenu;

    if(AcceptInput() && m_pCurrCache && mnuCtxt.LoadMenu(IDR_OPTIONS))
    {
        pMenu = mnuCtxt.GetSubMenu(0);

		point = m_ClientCursorPos;

		ClientToScreen(&point);

		// Signal this is a tap and hold operation              
		m_bTapAndHold = true;

		CListCtrl& ListCtrl = GetListCtrl();

		// Deselect all items
		POSITION pos = ListCtrl.GetFirstSelectedItemPosition();

		while (pos)
		{
			ListCtrl.SetItemState(ListCtrl.GetNextSelectedItem(pos), 0, LVIS_SELECTED);
		}
		
		ListCtrl.SetItemState(m_iItemOnMenu, LVIS_SELECTED, LVIS_SELECTED);

		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
    }
}

void CGpxSonarView::EnableInput()
{
	m_InputEnabled = true;
}

void CGpxSonarView::DisableInput()
{
	m_InputEnabled = false;
}

bool CGpxSonarView::AcceptInput()
{
	return m_InputEnabled;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Context Menu Options
/////////////////////////////////////////////////////////////////////////////////////////
void CGpxSonarView::ShowCacheDetails() 
{
	if (m_pCurrCache)
	{
		if (!m_LastCacheDetails.IsEmpty())
		{
			DeleteFile((LPCTSTR)m_LastCacheDetails);
		}

		CMd5	MD5;

		SYSTEMTIME ST;

		GetLocalTime(&ST);

		CString Temp;

		CPath	Path;

		m_LastCacheDetails = Path.BuildPath(CACHE_PAGE_LOCATION).c_str();
		m_LastCacheDetails += MD5.Hash((BYTE*)&ST, sizeof(ST)).c_str();
		m_LastCacheDetails += _T(".html");

		BeginWaitCursor();

		wstring	SpoilerPicsLocation;

		// Build the path the location of the potential html page referencing the spoiler pics
		SpoilerPicsLocation = m_SpoilerPicsPath;
		SpoilerPicsLocation += _T("\\");
		SpoilerPicsLocation += m_pCurrCache->m_Shortname;
		SpoilerPicsLocation += _T(".html");

		if (GetFileAttributes((LPCTSTR) SpoilerPicsLocation.c_str()) == 0xFFFFFFFF)
		{
			m_CachePageWriter.m_SpoilerPicsWriter.SetSpoilerPicsPath(_T(""));
		}
		else
		{
			m_CachePageWriter.m_SpoilerPicsWriter.SetSpoilerPicsPath(SpoilerPicsLocation);
		}

		m_CachePageWriter.Write(m_GpxParser, *m_pCurrCache, (TCHAR*)(LPCTSTR) m_LastCacheDetails);

		EndWaitCursor();
		
		// Show the resulting page in a web browser
		CBrowserLauncher	BL;

		BL.Go((TCHAR*)(LPCTSTR)m_LastCacheDetails);
	}
}

void CGpxSonarView::OnMenuCacheoptionsShowhints() 
{
	if (m_pCurrCache)
	{
		CTextTrx TT;

		String Hints = TT.Trim(m_pCurrCache->m_GsCacheEncodedHints);

		if (Hints.empty())
		{
			MessageBox(_T("The hider did not provide any hints."), _T("Hummm..."), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			CCacheDetailsHints		HintsDlg;
			
			Hints = TT.CRToCRLF(Hints);

			HintsDlg.m_Hints = Hints.c_str();

			HintsDlg.DoModal();
		}
	}
}

void CGpxSonarView::OnLogCache() 
{
	if (m_pCurrCache)
	{
		DisableInput();

		CCacheDetailsFieldNotes	Dlg;

		Dlg.m_Shortname = m_pCurrCache->m_Shortname.c_str();

		CFieldNote* pFN = m_NotesMgr.Find(m_pCurrCache->m_Shortname);

		if (pFN)
		{
			Dlg.m_Notes = pFN->m_Notes.c_str();
			Dlg.m_Status = pFN->m_Status;
			Dlg.m_Coords.SetDecimal(pFN->m_Lat, pFN->m_Long);
		}

		if (Dlg.DoModal() == IDOK)
		{
			LVITEM	Item;

			switch (Dlg.m_Action)
			{
			case NOTE_ACTION_SAVE:
				m_pCurrCache->m_pFieldNote = m_NotesMgr.Add(m_pCurrCache->m_Shortname, (LPCTSTR) Dlg.m_Notes, (GcNoteStatus) Dlg.m_Status);
				m_pCurrCache->m_pFieldNote->m_Lat = Dlg.m_Coords.m_Latitude;
				m_pCurrCache->m_pFieldNote->m_Long = Dlg.m_Coords.m_Longitude;
				m_pCurrCache->m_pFieldNote->m_pCache = m_pCurrCache;

				// Save the new field note under the \Config directory
				m_pCurrCache->m_pFieldNote->SaveYourself(m_pCurrCache->m_Shortname);

				Item.iImage = CacheNoteBitmapLookup(m_pCurrCache);
				break;

			case NOTE_ACTION_DELETE:
				m_NotesMgr.Delete(m_pCurrCache->m_Shortname);

				// Delete the field note from the \Config directory
				m_pCurrCache->m_pFieldNote->DeleteYourself(m_pCurrCache->m_Shortname);

				m_pCurrCache->m_pFieldNote = 0;

				Item.iImage = EMPTY_BITMAP;
				break;
			}

			int ColPos = GetColumnById(ColNote);

			// Update the 'Notes' bitmap in the list (if that column if visible)
			if (ColPos != -1)
			{
				Item.mask =  LVIF_IMAGE;
				Item.iItem = m_iItemOnMenu;
				Item.iSubItem = ColPos;

				GetListCtrl().SetItem(&Item);
			}

			ColPos = GetColumnById(ColTB);

			// Update the TB bitmap in the list (if that column if visible)
			if (ColPos != -1)
			{
				Item.mask =  LVIF_IMAGE;
				Item.iItem = m_iItemOnMenu;
				Item.iSubItem = ColPos;
				
				if (m_pCurrCache->GetTBCount())
				{
					Item.iImage = TB_PRESENT;
				}
				else
				{
					Item.iImage = EMPTY_BITMAP;
				}

				GetListCtrl().SetItem(&Item);
			}

			// Run the cache through the 'cache list' filter to determine if it should still be in scope
			CFilterCacheLists* pFCL = (CFilterCacheLists*) m_FilterMgr.Find(FilterCacheLists);
			
			if (pFCL->IsEnabled() && !pFCL->OnFilterCache(m_pCurrCache))
			{
				// Take the cache out of the list
				GetListCtrl().DeleteItem(m_iItemOnMenu);
				m_pCurrCache = 0;
			}
		}

		EnableInput();
	}
}

void CGpxSonarView::OnMenuSetascenter() 
{
	if (m_pCurrCache)
	{
		m_CenterCoords.SetDecimal(m_pCurrCache->m_Lat, m_pCurrCache->m_Long);
		
		{ // These 3 calls must be together
			ComputeDistanceBearing();
			UpdateCacheList();
			SortByIncreasingDistance();

			m_NeedToSaveChanges = true;
		}
	}	
}

void CGpxSonarView::OnMenuExport() 
{
	if (m_pCurrCache)
	{
		CExportLocationDlg	Dlg;

		m_GpxParser.MarkAllAsOutOfScopeExceptOne(m_pCurrCache);

		Dlg.m_pExpLocMgr = &m_ExportLocationMgr;
		Dlg.m_pExpLocMgr->SetGpxParser(&m_GpxParser);
		Dlg.m_pExpLocMgr->SetFieldNoteMgr(&m_NotesMgr);

		Dlg.DoModal();

		m_GpxParser.RestoreScope();

		m_NeedToSaveChanges = true;
	}	
}

void CGpxSonarView::OnMenuCacheoptionsIgnorecache() 
{
	if (m_pCurrCache)
	{
		CFilterCacheLists* pFCL = (CFilterCacheLists*) m_FilterMgr.Find(FilterCacheLists);
		
		pFCL->Ignore(m_pCurrCache);

		// If the cache is no longer in scope, remove it from the list
		if (!m_pCurrCache->m_InScope)
		{
			POSITION Pos = GetListCtrl().GetFirstSelectedItemPosition();
			int ItemNum = GetListCtrl().GetNextSelectedItem(Pos);
			GetListCtrl().DeleteItem(ItemNum);
		}

		m_pCurrCache = 0;

		m_NeedToSaveChanges = true;
	}	
}

void CGpxSonarView::OnMenuIgnorecachesbythisowner() 
{
	if (m_pCurrCache)
	{
		CString Msg;

		Msg.Format(_T("Ignore all caches by: %s ?"), m_pCurrCache->m_GsCacheOwnerName.c_str());

		int Answer = MessageBox(Msg, _T("Confirm Operation"), MB_OKCANCEL | MB_ICONQUESTION);

		if (Answer == IDOK)
		{
			CFilterCacheLists* pFCL = (CFilterCacheLists*) m_FilterMgr.Find(FilterCacheLists);
			
			itGC C;

			CGeoCache* pCache = m_GpxParser.First(C);

			while (!m_GpxParser.EndOfCacheList(C))
			{
				if (m_pCurrCache->m_GsCacheOwnerName == pCache->m_GsCacheOwnerName)
				{
					pFCL->Ignore(pCache);
				}

				pCache = m_GpxParser.Next(C);
			}

			m_pCurrCache = 0;

			UpdateCacheList();
			SortByIncreasingDistance();

			m_NeedToSaveChanges = true;
		}
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////
// File Menu Options
/////////////////////////////////////////////////////////////////////////////////////////
void CGpxSonarView::OnMenuFileMytravelbugs() 
{
	CMyTravelBugsDlg Dlg;

	Dlg.DoModal();

	m_NeedToSaveChanges = true;
}

void CGpxSonarView::OnMenuFileMycaches() 
{
	CMyCachesDlg	Dlg;

	CCacheMgr	CacheMgr;
	
	((CGpxSonarApp*) AfxGetApp())->m_pCacheMgr = &CacheMgr;

	Dlg.m_pCacheMgr = &CacheMgr;
	Dlg.m_pExpLocMgr = &m_ExportLocationMgr;
	Dlg.m_Owner = m_Alias;

	BeginWaitCursor();
	{
		CacheMgr.LoadConfig();
	}
	EndWaitCursor();

	Dlg.DoModal();

	BeginWaitCursor();
	{
		CacheMgr.SaveConfig();
	}
	EndWaitCursor();

	((CGpxSonarApp*) AfxGetApp())->m_pCacheMgr = 0;
}

void CGpxSonarView::OnMenuFileMyalias() 
{
	CMyAliasDlg	Dlg;

	Dlg.m_Alias = m_Alias.c_str();

	if (Dlg.DoModal() == IDOK)
	{
		m_Alias = (LPCTSTR) Dlg.m_Alias;

		UpdateCacheList();
		SortByIncreasingDistance();
			
		m_NeedToSaveChanges = true;
	}

	::SHSipPreference(m_hWnd, SIP_FORCEDOWN);
}

void CGpxSonarView::OnGpxfileinfo() 
{
	CGpxFileInfoDlg	Dlg;
	
	Dlg.m_SavedGpxFilename = m_SavedGpxFilename;
	Dlg.m_pNotesMgr = &m_NotesMgr;

	Dlg.DoModal();
}

void CGpxSonarView::OnMenuFileReportsFieldnotes() 
{
	if (!m_LastFieldNotesReport.IsEmpty())
	{
		DeleteFile((LPCTSTR) m_LastFieldNotesReport);
	}

	CMd5	MD5;

	SYSTEMTIME ST;

	GetLocalTime(&ST);

	CPath	Path;

	m_LastFieldNotesReport = Path.BuildPath(CACHE_PAGE_LOCATION).c_str();
	m_LastFieldNotesReport += (TCHAR*) MD5.Hash((BYTE*)&ST, sizeof(ST)).c_str();
	m_LastFieldNotesReport += _T(".html");

	CCachePageWriter		ReportWriter;

	CFieldNotesReportPrefDlg	Dlg;

	// Bind the field notes preferences and let the user pick what he wants
	Dlg.m_ReportsPref = &m_CacheReportPref;

	if (Dlg.DoModal() == IDOK)
	{
		ReportWriter.LoadTemplate(Path.BuildPath(FIELD_NOTES_REPORT_TEMPLATE).c_str());

		ReportWriter.WriteReport(&m_CacheReportPref, &m_NotesMgr, m_GpxParser, (TCHAR*) (LPCTSTR) m_LastFieldNotesReport);

		// Save whatever changes were done
		m_NeedToSaveChanges = true;

		// Show the resulting page in a web browser
		CBrowserLauncher	BL;

		String Temp;

		Temp = _T("\"");
		Temp += (LPCTSTR) m_LastFieldNotesReport;
		Temp += _T("\"");

		BL.Go(Temp);

		if (m_CacheReportPref.m_Reset)
		{
			// No need to reconnect the notes since the report writer took care of deleting / disconnecting the notes appropriately
			// ConnectNotes();

			// Update the list
			UpdateCacheList();
			SortByIncreasingDistance();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// List Menu Options
/////////////////////////////////////////////////////////////////////////////////////////
void CGpxSonarView::OnMenuFilters() 
{
	CFilterMgrDlg Dlg;

	if (Dlg.DoModal() == IDOK)
	{
		UpdateCacheList();
		SortByIncreasingDistance();

		m_NeedToSaveChanges = true;
	}	
}

void CGpxSonarView::OnMenuListoptionsCentercoords() 
{
	CCenterCoordsDlg Dlg;

	Dlg.m_BookmarkMode = true;
	Dlg.m_pWptsMgr = &m_Bookmarks;
	Dlg.SetTitle(_T("Center Coordinates"));

	m_CenterCoords.GetDegMinMmm(
					Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
					Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);

	if (Dlg.DoModal() == IDOK)
	{
		m_CenterCoords.SetDegMinMmm(
				Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
				Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);

		{ // These 3 calls must be together
			ComputeDistanceBearing();
			UpdateCacheList();
			SortByIncreasingDistance();

			m_NeedToSaveChanges = true;
		}
	}

	::SHSipPreference(m_hWnd, SIP_FORCEDOWN);
}

void CGpxSonarView::OnDistanceUnits() 
{
	CDistanceUnitsDlg	Dlg;

	Dlg.SetDistanceUnits(m_CenterCoords.GetDistanceUnits());

	if (Dlg.DoModal() == IDOK)
	{
		CString	Label;

		m_CenterCoords.SetDistanceUnits(Dlg.GetDistanceUnits(Label));

		ComputeDistanceBearing();
		UpdateCacheList();
		SortByIncreasingDistance();

		m_NeedToSaveChanges = true;
	}	
}

void CGpxSonarView::OnMenuListoptionsPreferences() 
{
	CListPreferencesDlg	Dlg;

	Dlg.m_pHeadings = &m_Headings;

	if (Dlg.DoModal() == IDOK)
	{
		CListCtrl& CacheList = GetListCtrl();
			
		CacheList.DeleteAllItems();

		int nColumnCount = CacheList.GetHeaderCtrl()->GetItemCount();

		// Delete all of the columns.
		for (int i = 0; i < nColumnCount; i++)
		{
			CacheList.DeleteColumn(0);
		}

		SetupListControl();

		UpdateCacheList();
		SortByIncreasingDistance();

		m_NeedToSaveChanges = true;
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////
// Menu Options
/////////////////////////////////////////////////////////////////////////////////////////
void CGpxSonarView::OnHelp() 
{
	CBrowserLauncher	BL;

	CPath	Path;

	String	TargetFname = Path.BuildPath(HELP_PAGE);

	BL.Go(TargetFname);	
}

void CGpxSonarView::OnToolsExportExportlocations() 
{
	CExportLocationDlg	Dlg;

	Dlg.m_pExpLocMgr = &m_ExportLocationMgr;
	Dlg.m_pExpLocMgr->SetGpxParser(&m_GpxParser);
	Dlg.m_pExpLocMgr->SetFieldNoteMgr(&m_NotesMgr);

	Dlg.DoModal();

	m_NeedToSaveChanges = true;
}

void CGpxSonarView::OnToolsFieldnotescleaner() 
{
	if (MessageBox(_T("All field notes will be permanently deleted\r\nDo you want to continue?"),_T("Field Notes Cleaner"), MB_OKCANCEL | MB_ICONWARNING) == IDOK)
	{
		m_NotesMgr.DeleteConfig();

		// Reconnect the caches with the notes
		ConnectNotes();
		// Update the main cache list
		UpdateCacheList();
	}
}

void CGpxSonarView::OnMenuFileRot13helper() 
{
	CRot13ConvertDlg Dlg;

	if (m_pCurrCache)
	{
		CTextTrx TT;

		String Hints = TT.Trim(m_pCurrCache->m_GsCacheEncodedHints);

		if (!Hints.empty())
		{
			Hints = TT.CRToCRLF(Hints);

			Dlg.m_Text = Hints.c_str();
		}
	}

	Dlg.DoModal();

	::SHSipPreference(m_hWnd, SIP_FORCEDOWN);
}

void CGpxSonarView::OnFilters() 
{
	OnMenuFilters();
}

void CGpxSonarView::OnUpdateFilters(CCmdUI* pCmdUI) 
{
	// Update the window title with the filter count
	if (m_FilterMgr.GetEnabledFilterCount())
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CGpxSonarView::OnQuickSearch() 
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	CSearchDlg	Dlg;

	Dlg.m_pSearch = (CFilterSearch*) pFilterMgr->Find(FilterSearch);
	Dlg.m_ShowEnableSearch = true;
	Dlg.m_EnableSearch = Dlg.m_pSearch->IsEnabled();

	if (Dlg.DoModal() == IDOK)
	{
		Dlg.m_pSearch->Enable((bool) Dlg.m_EnableSearch);

		UpdateCacheList();
		SortByIncreasingDistance();

		m_NeedToSaveChanges = true;
	}	
}

void CGpxSonarView::OnFileExit() 
{
	RecordColumnSizes();

	SaveConfig();

	Cleanup();

	PostMessage(WM_QUIT);
}

void CGpxSonarView::OnFileAppconfig() 
{
	CMiscPreferences Dlg;

	Dlg.m_DblClickCacheDetails = (BOOL) m_bUseDblckForCacheDetails;
	Dlg.m_ReloadLastGpxFile = (BOOL) m_bReloadLastGpxFile;
	Dlg.m_SpoilerPicsPath = m_SpoilerPicsPath.c_str();
	Dlg.m_StripImgTags = (BOOL) m_GpxParser.GetStripImgTags();

	Dlg.DoModal();

	m_bUseDblckForCacheDetails = (bool) Dlg.m_DblClickCacheDetails;
	m_bReloadLastGpxFile = (bool) Dlg.m_ReloadLastGpxFile;
	m_SpoilerPicsPath = (LPCTSTR) Dlg.m_SpoilerPicsPath;
	m_GpxParser.SetStripImgTags((bool) Dlg.m_StripImgTags);

	if (m_bReloadLastGpxFile)
	{
		RememberLastGpxFile();
	}
	else
	{
		DeleteAutoLoadGpxFile();
	}

	m_NeedToSaveChanges = true;
}
