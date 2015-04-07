#include "stdafx.h"
#include "GpxSonar.h"
#include "CCachePropertiesDlg.h"
#include "CGpxParser.h"
#include "CBrowserLauncher.h"
#include "CCachePageWriter.h"
#include "CMd5.h"
#include "Literals.h"
#include "CTimeHelper.h"
#include "CCoords.h"
#include "CDatePickerDlg.h"
#include "CLineEditDlg.h"
#include "CTextEditDlg.h"
#include "CRatingDlg.h"
#include "CTypeAndContainerDlg.h"
#include "CCenterCoordsDlg.h"
#include "CWaypointsDlg.h"
#include "CCacheMgr.h"
#include "CFieldNoteMgr.h"
#include "CPath.h"
#include "CCacheCategoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCachePropertiesDlg::CCachePropertiesDlg(CWnd* pParent): CNonFSDialog(CCachePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCachePropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGc = 0;
}

CCachePropertiesDlg::~CCachePropertiesDlg()
{
	DeleteFile((LPCTSTR) m_Temp);

	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		delete (*I);
	}

	m_Headings.clear();
}

void CCachePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCachePropertiesDlg)
	DDX_Control(pDX, IDC_PROPERTIES, m_Props);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCachePropertiesDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CCachePropertiesDlg)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_NOTIFY(NM_DBLCLK, IDC_PROPERTIES, OnDblclkProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCachePropertiesDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	// Default column widths expressed in pixels
	#define PROPERTY_COL_WIDTH 240
	
	m_Headings.push_back(new CHeading(0, _T("Properties"), PROPERTY_COL_WIDTH, 0));

	m_Props.SetExtendedStyle(m_Props.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	int Col = 0;

	// Add the headers to the list control
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		m_Props.InsertColumn(Col++, (*I)->m_Name.c_str(), LVCFMT_LEFT, (*I)->m_Length);
	}

	UpdateList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCachePropertiesDlg::UpdateList()
{
	LVITEM	Item;

	CString Text, Label, Property;

	SetWindowText(m_pGc->m_GsCacheName.c_str());

	m_Props.DeleteAllItems();

	// Fill the list with the properties
	for (int P = 0; P < CacheEndOfList; P++)
	{

		PropertyFunction(P, LabelLookup, Label);
		PropertyFunction(P, FormatProperty, Property);

		Text = Label + _T(": ") + Property;

		// Image of cache type & cache name
		Item.mask =  LVIF_TEXT;
		Item.iItem = P;
		Item.iSubItem = 0;
		Item.pszText = (TCHAR*) (LPCTSTR) Text;

		// Preserve the index of the cache object in the list
		int Idx = m_Props.InsertItem(&Item);

		m_Props.SetItemData(Idx, P);
	}
}

void CCachePropertiesDlg::OnEdit() 
{
	POSITION Pos = m_Props.GetFirstSelectedItemPosition();

	long Item = m_Props.GetNextSelectedItem(Pos);

	if (Item != -1)
	{
		CString Text;

		PropertyFunction(Item, EditProperty, Text);
	}
}

void CCachePropertiesDlg::OnPreview()
{
	CMd5	MD5;

	SYSTEMTIME ST;

	GetLocalTime(&ST);

	DeleteFile((LPCTSTR) m_Temp);

	CPath	Path;

	m_Temp = Path.BuildPath(CACHE_PAGE_LOCATION).c_str();
	m_Temp += MD5.Hash((BYTE*)&ST, sizeof(ST)).c_str();
	m_Temp += _T(".html");

	((CGpxSonarApp*) AfxGetApp())->m_pCachePageWriter->Write(
														*((CGpxParser*) ((CGpxSonarApp*) AfxGetApp())->m_pGpxParser),
														*m_pGc, 
														(TCHAR*)(LPCTSTR) m_Temp);
	
	// Show the resulting page in a web browser
	CBrowserLauncher	BL;

	BL.Go((TCHAR*)(LPCTSTR)m_Temp);
}

void CCachePropertiesDlg::PropertyFunction(int Idx, Function Func, CString& Text)
{
	#define MAX_SMALL_BUFFER_SIZE	40

	if (Func == LabelLookup)
	{
		static const TCHAR* Labels[] = {
			_T("Category"),
			_T("Type"),
			_T("Size"),
			_T("Name"),
			_T("Waypoint"),
			_T("Placed"),
			_T("Coords"),
			_T("Country"),
			_T("State"),
			_T("Difficulty"),
			_T("Terrain"),
			_T("Short Desc"),
			_T("Long Desc"),
			_T("Hints"),
			_T("Waypoints"),
			_T("Notes"),
			_T("")
		};

		if (Idx < CacheEndOfList)
		{
			Text = Labels[Idx];
		}
		else
		{
			Text = _T("?");
		}

		return;
	}

	switch (Idx)
	{
	case CacheCategory:
		if (Func == EditProperty)
		{
			CCacheCategoryDlg Dlg;

			Dlg.m_Selection = m_pGc->m_Category.c_str();

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_Category = (LPCTSTR) Dlg.m_Selection;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_Category.c_str();
		}
		break;
	case CacheType:
		if (Func == EditProperty)
		{
			CTypeAndContainerDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_FillerType = CacheTypes;
			Dlg.m_Selection = m_pGc->m_GsCacheType.c_str();

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheType = (LPCTSTR) Dlg.m_Selection;

				// Fixup! The type gets cached after the first lookup, therefore, it needs to be 'reset' once the type of the cache changes
				m_pGc->m_GcType = GT_NotInitialized;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_GsCacheType.c_str();
		}
		break;
	case CacheSize:
		if (Func == EditProperty)
		{
			CTypeAndContainerDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_FillerType = CacheContainers;
			Dlg.m_Selection = m_pGc->m_GsCacheContainer.c_str();

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheContainer = (LPCTSTR) Dlg.m_Selection;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_GsCacheContainer.c_str();
		}
		break;
	case CacheNickname:
		if (Func == EditProperty)
		{
			CLineEditDlg	Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_GsCacheName.c_str();

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheName = (LPCTSTR) Dlg.m_Text;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_GsCacheName.c_str();
		}
		break;
	case CacheWaypoint:
		if (Func == EditProperty)
		{
			CLineEditDlg	Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_Shortname.c_str();

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_Shortname = (LPCTSTR) Dlg.m_Text;
				
				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_Shortname.c_str();
		}
		break;
	case CacheDatePlaced:
		if (Func == EditProperty)
		{
			CDatePickerDlg	Dlg;

			CopyMemory((void*) &Dlg.m_Date, (void*) &m_pGc->m_CreationTime, sizeof(SYSTEMTIME));

			if (Dlg.DoModal() == IDOK)
			{
				CopyMemory((void*) &m_pGc->m_CreationTime, (void*) &Dlg.m_Date, sizeof(SYSTEMTIME));

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			CTimeHelper	TH(m_pGc->m_CreationTime);
			Text = TH.Date().c_str();
		}
		break;
	case CacheCoords:
		if (Func == EditProperty)
		{
			CCenterCoordsDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.SetTitle(Text);

			CCoords Coords;

			Coords.SetDecimal(m_pGc->m_Lat, m_pGc->m_Long);

			Coords.GetDegMinMmm(
							Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
							Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);

			if (Dlg.DoModal() == IDOK)
			{
				Coords.SetDegMinMmm(
						Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
						Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);

				Coords.GetDecimal(m_pGc->m_Lat, m_pGc->m_Long);

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			CCoords	Coords;
			Coords.SetDecimal(m_pGc->m_Lat, m_pGc->m_Long);
			Text = Coords.Format().c_str();
		}
		break;
	case CacheCountry:
		if (Func == EditProperty)
		{
			CLineEditDlg	Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_GsCacheCountry.c_str();

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheCountry = (LPCTSTR) Dlg.m_Text;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{			
			Text = m_pGc->m_GsCacheCountry.c_str();
		}
		break;
	case CacheState:
		if (Func == EditProperty)
		{
			CLineEditDlg	Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_GsCacheState.c_str();

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheState = (LPCTSTR) Dlg.m_Text;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{			
			Text = m_pGc->m_GsCacheState.c_str();
		}
		break;
	case CacheDifficulty:
		if (Func == EditProperty)
		{
			CRatingDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_RatingVal = m_pGc->m_GsCacheDifficulty;

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheDifficulty = Dlg.m_RatingVal;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			TCHAR Buffer[MAX_SMALL_BUFFER_SIZE];

			_sntprintf(Buffer, MAX_SMALL_BUFFER_SIZE - 1, _T("%.1f"), m_pGc->m_GsCacheDifficulty);

			Text = Buffer;
		}
		break;
	case CacheTerrain:
		if (Func == EditProperty)
		{
			CRatingDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_RatingVal = m_pGc->m_GsCacheTerrain;

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheTerrain = Dlg.m_RatingVal;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			#define MAX_SMALL_BUFFER_SIZE	40

			TCHAR Buffer[MAX_SMALL_BUFFER_SIZE];

			_sntprintf(Buffer, MAX_SMALL_BUFFER_SIZE - 1, _T("%.1f"), m_pGc->m_GsCacheTerrain);

			Text = Buffer;
		}
		break;
	case CacheShortDesc:
		if (Func == EditProperty)
		{
			CTextEditDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_GsCacheShortDesc.c_str();
			Dlg.m_Html = (BOOL) m_pGc->m_GsCacheShortDescIsHtml;
			Dlg.m_EnableHtml = true;

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheShortDesc = (LPCTSTR) Dlg.m_Text;
				m_pGc->m_GsCacheShortDescIsHtml = (bool) Dlg.m_Html;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_GsCacheShortDesc.c_str();
		}
		break;
	case CacheLongDesc:
		if (Func == EditProperty)
		{
			CTextEditDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_GsCacheLongDesc.c_str();
			Dlg.m_Html = (BOOL) m_pGc->m_GsCacheLongDescIsHtml;
			Dlg.m_EnableHtml = true;

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheLongDesc = (LPCTSTR) Dlg.m_Text;
				m_pGc->m_GsCacheLongDescIsHtml = (bool) Dlg.m_Html;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_GsCacheLongDesc.c_str();
		}
		break;
	case CacheHint:
		if (Func == EditProperty)
		{
			CTextEditDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_GsCacheEncodedHints.c_str();
			Dlg.m_EnableHtml = false;

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_GsCacheEncodedHints = (LPCTSTR) Dlg.m_Text;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_GsCacheEncodedHints.c_str();
		}
		break;
	case CacheWaypoints:
		if (Func == EditProperty)
		{
			CWaypointsDlg Dlg;

			Dlg.m_pWptsMgr = m_pGc->m_WpMgr;

			Dlg.DoModal();

			UpdateList();
		}
		else if (Func == FormatProperty && m_pGc->m_WpMgr)
		{
			TCHAR Buffer[MAX_SMALL_BUFFER_SIZE];

			_sntprintf(Buffer, MAX_SMALL_BUFFER_SIZE - 1, _T("%ld"), m_pGc->m_WpMgr->m_Wps.size());

			Text = Buffer;
		}
		break;
	case CacheNotes:
		if (Func == EditProperty)
		{
			CTextEditDlg Dlg;

			PropertyFunction(Idx, LabelLookup, Text);

			Dlg.m_Title = Text;
			Dlg.m_Text = m_pGc->m_pFieldNote->m_Notes.c_str();
			Dlg.m_EnableHtml = false;

			if (Dlg.DoModal() == IDOK)
			{
				m_pGc->m_pFieldNote->m_Notes = (LPCTSTR) Dlg.m_Text;

				UpdateList();
			}
		}
		else if (Func == FormatProperty)
		{
			Text = m_pGc->m_pFieldNote->m_Notes.c_str();
		}
		break;
	}
}

void CCachePropertiesDlg::OnDblclkProperties(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	
	*pResult = 0;
}
