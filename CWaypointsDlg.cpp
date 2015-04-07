#include "stdafx.h"
#include "GpxSonar.h"
#include "CWaypointsDlg.h"
#include "CCacheMgr.h"
#include "IDB_CACHES.h"
#include "CWptProperties.h"
#include "CGpxParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWaypointsDlg::CWaypointsDlg(CWnd* pParent): CNonFSDialog(CWaypointsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaypointsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pWptsMgr = 0;
	m_CurrSortCol = 0;
	m_BookmarkMode = false;
}

CWaypointsDlg::~CWaypointsDlg()
{
	m_ImageList.DeleteImageList();

	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		delete (*I);
	}

	m_Headings.clear();
}

void CWaypointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaypointsDlg)
	DDX_Control(pDX, IDC_SELECT, m_SelectCtl);
	DDX_Control(pDX, IDC_WPTS, m_Wpts);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWaypointsDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CWaypointsDlg)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_NOTIFY(NM_DBLCLK, IDC_WPTS, OnDblclkWpts)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_WPTS, OnColumnclickWpts)
	ON_BN_CLICKED(IDC_SELECT, OnSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWaypointsDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	if (m_BookmarkMode)
	{
		m_SelectCtl.EnableWindow();

		SetWindowText(_T("Coordinates Bookmarks"));
	}

	// Default column widths expressed in pixels
	#define WPT_NAME_COL_WIDTH 222
	//define WPT_COMMENT_COL_WIDTH 200
	
	m_Headings.push_back(new CHeading(0, _T("Name"), WPT_NAME_COL_WIDTH, CompareName));
	//m_Headings.push_back(new CHeading(1, _T("Comment"), WPT_COMMENT_COL_WIDTH, CompareComment));

	m_Wpts.SetExtendedStyle(m_Wpts.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);

	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	// Setup the icon list for the list control
	m_ImageList.Create(IDB_WAYPOINT, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	m_Wpts.SetImageList(&m_ImageList, LVSIL_SMALL);

	int Col = 0;

	// Add the headers to the list control
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		m_Wpts.InsertColumn(Col++, (*I)->m_Name.c_str(), LVCFMT_LEFT, (*I)->m_Length);
	}

	UpdateWptList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaypointsDlg::UpdateWptList() 
{
	LVITEM		Item;
	int			Count = 0;

	m_Wpts.DeleteAllItems();

	for (itWP W = m_pWptsMgr->m_Wps.begin(); W != m_pWptsMgr->m_Wps.end(); W++)
	{
		CWaypoint* pWp = *W;

		// Bitmap + Name of WP
		Item.mask =  LVIF_TEXT | LVIF_IMAGE;
		Item.iItem = Count;
		Item.iSubItem = 0;
		Item.iImage = WAYPOINT_IMAGE;
		Item.pszText = (TCHAR*) pWp->m_Name.c_str();
		m_Wpts.SetItemData(m_Wpts.InsertItem(&Item), (DWORD) pWp);
		
		// Comment of Wpt
		/*
		Item.mask =  LVIF_TEXT;
		Item.iSubItem++;
		Item.pszText = (TCHAR*) pWp->m_Desc.c_str();
		m_Wpts.SetItem(&Item);
		*/

		Count++;
	}	
}

void CWaypointsDlg::OnNew() 
{
	CWaypoint* pWp = new CWaypoint;

	CString	Name;

	Name.Format(_T("Wp#%0ld"), m_pWptsMgr->m_Wps.size());

	pWp->m_Name = (LPCTSTR) Name;

	if (ShowWptProperties(pWp) == IDOK)
	{
		m_pWptsMgr->m_Wps.push_back(pWp);

		UpdateWptList();
	}
	else
	{
		delete pWp;
	}
}

void CWaypointsDlg::OnEdit() 
{
	CWaypoint* pWp = GetSelectedItem();

	if (pWp && ShowWptProperties(pWp) == IDOK)
	{
		UpdateWptList();
	}
}

int CWaypointsDlg::ShowWptProperties(CWaypoint* pWp)
{
	CWptProperties	Dlg;

	if (m_BookmarkMode)
	{
		Dlg.m_Title = _T("Coords. Properties");
	}

	Dlg.m_pWpt = pWp;

	Dlg.DoModal();

	return IDOK;
}

void CWaypointsDlg::OnDelete() 
{
	CWaypoint* pWp = GetSelectedItem();

	if (pWp)
	{
		if ( MessageBox( _T("Do you really want to\r\ndelete this waypoint?"), _T("Waypoint Killer"), MB_OKCANCEL | MB_ICONWARNING ) == IDOK )
		{
			for (itWP W = m_pWptsMgr->m_Wps.begin(); W != m_pWptsMgr->m_Wps.end(); W++)
			{
				if ((*W) == pWp)
				{
					delete pWp;
					m_pWptsMgr->m_Wps.erase(W);
					break;
				}
			}

			UpdateWptList();
		}
	}
}

void CWaypointsDlg::OnImport() 
{
	CFileDialog	FD(true, _T("zip"), NULL, 0, _T("Zipped GC.com GPX Files (*.zip)|*.zip||"), NULL );

	if (FD.DoModal() == IDOK)
	{
		BeginWaitCursor();

		GpxLoadStatus Status;
		
		CGpxParser	Parser;

		// Backup the global pointer to the current instance of the parser
		CGpxParser* pBackupInst = Parser.GetInstance();

		// Make the new parser the current global instance
		Parser.SetInstance(&Parser);

		BeginWaitCursor();
		{
			Status = Parser.Load((LPCTSTR) FD.GetPathName(), false);
		}
		EndWaitCursor();

		switch (Status)
		{
		case GpxLoadStatusFailed:
			{
				MessageBox(Parser.GetErrorMsg().c_str(), _T("Zipped GC.com GPX File Loading Failure"), MB_OK | MB_ICONWARNING);
			}
			break;

		case GpxLoadStatusParserException:
			{
				MessageBox(Parser.GetErrorMsg().c_str(), _T("Malformed Zipped GC.com GPX File"), MB_OK | MB_ICONWARNING);
			}
			break;

		case GpxLoadStatusOk:
			{
				itGC C;

				m_pWptsMgr->Reset();

				CGeoCache* pCache = Parser.First(C);

				while (!Parser.EndOfCacheList(C))
				{
					CWaypoint* pWp = new CWaypoint;

					if (pWp)
					{
						pWp->m_Lat = pCache->m_Lat;
						pWp->m_Long = pCache->m_Long;
						pWp->m_Name = pCache->m_Shortname;
						pWp->m_Desc = pCache->m_GsCacheName;

						m_pWptsMgr->m_Wps.push_back(pWp);
					}

					pCache = Parser.Next(C);
				}
			}
			break;
		};

		// Restore the pointer to the original instance of the parser
		Parser.SetInstance(pBackupInst);

		EndWaitCursor();

		UpdateWptList();
	}
}

void CWaypointsDlg::OnDblclkWpts(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	
	*pResult = 0;
}

CWaypoint* CWaypointsDlg::GetSelectedItem()
{
	POSITION Pos = m_Wpts.GetFirstSelectedItemPosition();

	long Item = m_Wpts.GetNextSelectedItem(Pos);

	if (Item != -1)
	{
		return (CWaypoint*) m_Wpts.GetItemData(Item);
	}

	return 0;
}

int CALLBACK CWaypointsDlg::CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CWaypointsDlg* pThis = (CWaypointsDlg*) lParamSort;
	CWaypoint* pWp1 = (CWaypoint*) lParam1;
	CWaypoint* pWp2 = (CWaypoint*) lParam2;
	
	if (pWp1->m_Name < pWp2->m_Name)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pWp1->m_Name > pWp2->m_Name)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

/*
int CALLBACK CWaypointsDlg::CompareComment(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CWaypointsDlg* pThis = (CWaypointsDlg*) lParamSort;
	CWaypoint* pWp1 = (CWaypoint*) lParam1;
	CWaypoint* pWp2 = (CWaypoint*) lParam2;
	
	if (pWp1->m_Desc < pWp2->m_Desc)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pWp1->m_Desc > pWp2->m_Desc)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}
*/
void CWaypointsDlg::OnColumnclickWpts(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_CurrSortCol = pNMListView->iSubItem;

	m_Headings[m_CurrSortCol]->m_SortToggle *= -1;

	m_Wpts.SortItems(m_Headings[m_CurrSortCol]->m_pSortFunc, (LPARAM) this);

	*pResult = 0;
}

void CWaypointsDlg::OnSelect() 
{
	m_pSelectedWp = GetSelectedItem();

	if (!m_pSelectedWp)
	{
		MessageBox(_T("Please select a bookmark"), _T("Ooops"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		CNonFSDialog::OnOK();
	}
}
