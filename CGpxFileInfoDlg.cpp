#pragma warning(disable:4786)

#include "stdafx.h"
#include "GpxSonar.h"
#include "CGpxFileInfoDlg.h"
#include "CHeading.h"
#include "IDB_CACHES.h"
#include "CFieldNoteMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGpxFileInfoDlg::CGpxFileInfoDlg(CWnd* pParent /*=NULL*/) : CDialog(CGpxFileInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGpxFileInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pNotesMgr = 0;
}

CGpxFileInfoDlg::~CGpxFileInfoDlg()
{
	m_ImageList.DeleteImageList();
}

void CGpxFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGpxFileInfoDlg)
	DDX_Control(pDX, IDC_CACHE_LIST, m_CacheList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGpxFileInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CGpxFileInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGpxFileInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	Rect;
	GetClientRect(&Rect);
	m_CacheList.MoveWindow(&Rect);

	CGpxParser* pGpxParser = ((CGpxSonarApp*) AfxGetApp())->m_pGpxParser;

	m_CacheList.SetExtendedStyle(m_CacheList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	// Setup the icon list for the list control
	m_ImageList.Create(IDB_CACHES, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	m_CacheList.SetImageList(&m_ImageList, LVSIL_SMALL);

	// Default column widths expressed in pixels
	enum ColWidths { 
		CACHE_TYPE = 140,
		CACHE_COUNT = 90,
		};

	HeadingCont	Headings;

	Headings.push_back(new CHeading(0, _T("Type"), CACHE_TYPE, 0));
	Headings.push_back(new CHeading(0, _T("Count"), CACHE_COUNT, 0));
	
	int Col = 0;

	// Add the headers to the list control
	for (itHeading I = Headings.begin(); I != Headings.end(); I++)
	{
		m_CacheList.InsertColumn(Col++, (*I)->m_Name.c_str(), LVCFMT_LEFT, (*I)->m_Length);
	}
	
	itGC		C;

	int Traditional = 0;
	int Multi = 0;
	int Virtual = 0;
	int Webcam = 0;
	int Unknown = 0;
	int LetterboxHybrid = 0;
	int Event = 0;
	int ProjectAPE = 0;
	int Locationless = 0;
	int CITO = 0;
	int Earth = 0;
	int TBs = 0;
	int Disabled = 0;
	int Archived = 0;
	int Found = 0;
	int Notes = 0;

	// Gather the numbers for each type of cache
	CGeoCache* pCache = pGpxParser->First(C);

	while (!pGpxParser->EndOfCacheList(C))
	{
		TBs += pCache->GetTBCount();

		if (!pCache->m_Sym.empty())
		{
			Found++;
		}

		if (pCache->m_pFieldNote)
		{
			Notes++;
		}

		if (pCache->m_GsCacheArchived)
		{
			Archived++;
		}

		if (!pCache->m_GsCacheAvailable)
		{
			Disabled++;
		}

		switch (pCache->m_GcType)
		{
		case GT_Traditional:
			Traditional++;
			break;
		case GT_Multi:
			Multi++;
			break;
		case GT_Virtual:
			Virtual++;
			break;
		case GT_Webcam:
			Webcam++;
			break;
		case GT_Unknown:
			Unknown++;
			break;
		case GT_LetterboxHybrid:
			LetterboxHybrid++;
			break;
		case GT_Event:
			Event++;
			break;
		case GT_ProjectAPE:
			ProjectAPE++;
			break;
		case GT_Locationless:
			Locationless++;
			break;
		case GT_CITO:
			CITO++;
			break;
		case GT_Earthcache:
			Earth++;
			break;
		}

		pCache = pGpxParser->Next(C);
	}

	int Item = 0;

	
	InsertStatLine(Item, (GcType) (FIELD_NOTE), (LPCTSTR) m_SavedGpxFilename, pGpxParser->CacheCount());

	InsertStatLine(Item, (GcType) (EMPTY_BITMAP), _T("--- Statistics ---"), -1);

	InsertStatLine(Item, (GcType) (TB_PRESENT), _T("Travel Bugs"), TBs);
	InsertStatLine(Item, GT_Traditional, _T("Traditional"), Traditional);
	InsertStatLine(Item, GT_Multi, _T("Multi"), Multi);
	InsertStatLine(Item, GT_Virtual, _T("Virtual"), Virtual);
	InsertStatLine(Item, GT_Webcam, _T("Webcam"), Webcam);
	InsertStatLine(Item, GT_Unknown, _T("Unknown"), Unknown);
	InsertStatLine(Item, GT_LetterboxHybrid, _T("Letterbox Hybrid"), LetterboxHybrid);
	InsertStatLine(Item, GT_Event, _T("Event"), Event);
	InsertStatLine(Item, GT_ProjectAPE, _T("Project APE"), ProjectAPE);
	InsertStatLine(Item, GT_Locationless, _T("Locationless"), Locationless);
	InsertStatLine(Item, GT_CITO, _T("CITO"), CITO);
	InsertStatLine(Item, GT_Earthcache, _T("Earthcache"), Earth);

	InsertStatLine(Item, (GcType) (EMPTY_BITMAP), _T("--- Other ---"), -1);

	InsertStatLine(Item, (GcType) (FOUND_CACHE), _T("Found Caches"), Found);
	InsertStatLine(Item, (GcType) (FIELD_NOTE), _T("Visible Field Notes"), Notes);
	InsertStatLine(Item, (GcType) (FIELD_NOTE), _T("Total Field Notes"), m_pNotesMgr->Size());

	InsertStatLine(Item, (GcType) (CACHE_ARCHIVED), _T("Archived Caches"), Archived);
	InsertStatLine(Item, (GcType) (CACHE_DISABLED), _T("Disabled Caches"), Disabled);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGpxFileInfoDlg::InsertStatLine(int& Line, GcType Type, const TCHAR* pText, int Count)
{
	if (Count)
	{
		#define MAX_BUFFER_SIZE  20

		LVITEM		Item;
		TCHAR		Buffer[MAX_BUFFER_SIZE];

		if (Count >= 0)
		{
			_sntprintf(Buffer, MAX_BUFFER_SIZE, _T("%i"), Count);
		}
		else
		{
			Buffer[0] = 0;
		}

		// Image / Text of cache type
		Item.mask =  LVIF_IMAGE | LVIF_TEXT;
		Item.iItem = Line;
		Item.iSubItem = 0;
		Item.iImage = Type;
		Item.pszText = (TCHAR*) pText;

		m_CacheList.InsertItem(&Item);
		
		// Count for this type
		Item.mask =  LVIF_TEXT;
		Item.iSubItem++;
		Item.pszText = Buffer;

		m_CacheList.SetItem(&Item);

		Line++;
	}
}
