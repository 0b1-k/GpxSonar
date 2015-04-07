#include "stdafx.h"
#include "GpxSonar.h"
#include "CMyTravelBugsDlg.h"
#include "CTBInfoDlg.h"
#include "CTBMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyTravelBugsDlg::CMyTravelBugsDlg(CWnd* pParent): CNonFSDialog(CMyTravelBugsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyTravelBugsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_CurrSortCol = 0;
}

CMyTravelBugsDlg::~CMyTravelBugsDlg()
{
	m_ImageList.DeleteImageList();

	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		delete (*I);
	}

	m_Headings.clear();
}

void CMyTravelBugsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyTravelBugsDlg)
	DDX_Control(pDX, IDC_TB_LIST, m_TBs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyTravelBugsDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CMyTravelBugsDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_TB_LIST, OnColumnclickTbList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyTravelBugsDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	SetupListControl();
	
	UpdateList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyTravelBugsDlg::SetupListControl()
{
	m_TBs.SetExtendedStyle(m_TBs.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	// Setup the icon list for the list control
	m_ImageList.Create(IDB_TB, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	m_TBs.SetImageList(&m_ImageList, LVSIL_SMALL);

	// Default column widths expressed in pixels
	enum ColWidths {
		//TB_REF_COL_WIDTH = 72,
		//TB_SERIAL_COL_WIDTH = 72,
		TB_NAME_COL_WIDTH = 200,
		//TB_LOCATION_COL_WIDTH = 72
		};

	//m_Headings.push_back(new CHeading(0, _T("Ref"), TB_REF_COL_WIDTH, CompareRef));
	//m_Headings.push_back(new CHeading(0, _T("Serial"), TB_SERIAL_COL_WIDTH, CompareSerial));
	m_Headings.push_back(new CHeading(0, _T("Name"),	TB_NAME_COL_WIDTH, CompareName));
	//m_Headings.push_back(new CHeading(0, _T("Location"),	TB_LOCATION_COL_WIDTH, CompareLocation));
	
	int Col = 0;

	// Add the headers to the list control
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		m_TBs.InsertColumn(Col++, (*I)->m_Name.c_str(), LVCFMT_LEFT, (*I)->m_Length);
	}
}

void CMyTravelBugsDlg::UpdateList()
{
	#define TRAVEL_BUG_IMAGE	0
	#define	MAX_BUFFER			20

	LVITEM		Item;
	itTB2		T;
	int			Count = 0;
	//TCHAR		Buffer[MAX_BUFFER];

	m_TBs.DeleteAllItems();

	CTBMgr*	pTBMgr = ((CGpxSonarApp*)AfxGetApp())->m_pTBMgr;

	CTB* pTB = pTBMgr->First(T);

	while (!pTBMgr->EndOfList(T))
	{
		// Name of TB + Image of TB
		Item.mask =  LVIF_TEXT | LVIF_IMAGE;
		Item.iItem = Count;
		Item.iSubItem = 0;
		Item.iImage = TRAVEL_BUG_IMAGE;
		Item.pszText = (TCHAR*) pTB->m_Name.c_str();
		m_TBs.SetItemData(m_TBs.InsertItem(&Item), (DWORD) pTB);

		/*
		// Reference + Image of TB
		Item.mask =  LVIF_TEXT | LVIF_IMAGE;
		Item.iItem = Count;
		Item.iSubItem = 0;
		Item.iImage = TRAVEL_BUG_IMAGE;
		Item.pszText = (TCHAR*) pTB->m_Ref.c_str();
		m_TBs.SetItemData(m_TBs.InsertItem(&Item), (DWORD) pTB);
		
		// Serial # of TB
		_sntprintf(Buffer, MAX_BUFFER, _T("%s"), pTB->m_Serial.c_str());
		Item.mask =  LVIF_TEXT;
		Item.iSubItem++;
		Item.pszText = Buffer;
		m_TBs.SetItem(&Item);

		// Name of TB
		Item.mask =  LVIF_TEXT;
		Item.iSubItem++;
		Item.pszText = (TCHAR*) pTB->m_Name.c_str();
		m_TBs.SetItem(&Item);

		// Location of TB
		Item.mask =  LVIF_TEXT;
		Item.iSubItem++;
		Item.pszText = (TCHAR*) pTB->m_CacheShortName.c_str();
		m_TBs.SetItem(&Item);
		*/

		pTB = pTBMgr->Next(T);

		Count++;
	}
}

void CMyTravelBugsDlg::OnAdd() 
{
	CTBInfoDlg	Dlg;
	
	CTB	Tb;

	Tb.m_CacheShortName = TB_INVENTORY;

	Dlg.m_pTB = &Tb;

	if (Dlg.DoModal() == IDOK)
	{
		CTBMgr*	pTBMgr = ((CGpxSonarApp*)AfxGetApp())->m_pTBMgr;

		CTB* pTB = pTBMgr->FindEither(Tb.m_Ref, Tb.m_Serial);

		if (pTB)
		{
			MessageBox(_T("A TB with the same Serial # or the\r\nsame Reference # already exists."), _T("Duplicate Entry"), MB_OK | MB_ICONWARNING);
			return;
		}
			
		pTB = pTBMgr->Add(Tb.m_Serial, Tb.m_Name);

		(*pTB) = Tb;

		UpdateList();
	}
}

void CMyTravelBugsDlg::OnEdit() 
{
	POSITION Pos = m_TBs.GetFirstSelectedItemPosition();

	long Item = m_TBs.GetNextSelectedItem(Pos);

	if (Item != -1)
	{
		CTB* pTB = (CTB*) m_TBs.GetItemData(Item);

		CTBInfoDlg	Dlg;

		Dlg.m_pTB = pTB;

		if (Dlg.DoModal() == IDOK)
		{
			(*pTB) = (*Dlg.m_pTB);

			UpdateList();
		}
	}
}

void CMyTravelBugsDlg::OnRemove() 
{
	POSITION Pos = m_TBs.GetFirstSelectedItemPosition();

	long Item = m_TBs.GetNextSelectedItem(Pos);

	if (Item != -1)
	{
		CTB* pTB = (CTB*) m_TBs.GetItemData(Item);

		if (MessageBox(_T("Exterminate?"), _T("Pest Control"), MB_OKCANCEL) == IDOK)
		{
			CTBMgr*	pTBMgr = ((CGpxSonarApp*)AfxGetApp())->m_pTBMgr;

			pTBMgr->Delete(pTB);

			UpdateList();
		}
	}
}

void CMyTravelBugsDlg::OnColumnclickTbList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_CurrSortCol = pNMListView->iSubItem;

	m_Headings[m_CurrSortCol]->m_SortToggle *= -1;

	m_TBs.SortItems(m_Headings[m_CurrSortCol]->m_pSortFunc, (LPARAM) this);

	*pResult = 0;
}

/*
int CALLBACK CMyTravelBugsDlg::CompareRef(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMyTravelBugsDlg* pThis = (CMyTravelBugsDlg*) lParamSort;
	CTB* pTB1 = (CTB*) lParam1;
	CTB* pTB2 = (CTB*) lParam2;

	if (pTB1->m_Ref < pTB2->m_Ref)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pTB1->m_Ref > pTB2->m_Ref)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

int CALLBACK CMyTravelBugsDlg::CompareSerial(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMyTravelBugsDlg* pThis = (CMyTravelBugsDlg*) lParamSort;
	CTB* pTB1 = (CTB*) lParam1;
	CTB* pTB2 = (CTB*) lParam2;

	if (pTB1->m_Serial < pTB2->m_Serial)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pTB1->m_Serial > pTB2->m_Serial)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}
*/
int CALLBACK CMyTravelBugsDlg::CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMyTravelBugsDlg* pThis = (CMyTravelBugsDlg*) lParamSort;
	CTB* pTB1 = (CTB*) lParam1;
	CTB* pTB2 = (CTB*) lParam2;

	if (pTB1->m_Name < pTB2->m_Name)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pTB1->m_Name > pTB2->m_Name)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}
/*
int CALLBACK CMyTravelBugsDlg::CompareLocation(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMyTravelBugsDlg* pThis = (CMyTravelBugsDlg*) lParamSort;
	CTB* pTB1 = (CTB*) lParam1;
	CTB* pTB2 = (CTB*) lParam2;

	if (pTB1->m_CacheShortName < pTB2->m_CacheShortName)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pTB1->m_CacheShortName > pTB2->m_CacheShortName)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}
*/