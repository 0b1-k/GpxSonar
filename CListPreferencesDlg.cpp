#include "stdafx.h"
#include "GpxSonar.h"
#include "CHeading.h"
#include "CListPreferencesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListPreferencesDlg::CListPreferencesDlg(CWnd* pParent): CNonFSDialog(CListPreferencesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListPreferencesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CListPreferencesDlg::~CListPreferencesDlg()
{
	m_ImageList.DeleteImageList();
}

void CListPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListPreferencesDlg)
	DDX_Control(pDX, IDC_COLS, m_Cols);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CListPreferencesDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CListPreferencesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CListPreferencesDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	CNonFSDialog::OnInitDialog();
	
	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)


	// Setup the icon list for the tree control
	m_ImageList.Create(IDB_COLUMNS, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	m_Cols.SetImageList(&m_ImageList, TVSIL_NORMAL);

	itHeading I;
	
	I = m_pHeadings->begin();

	// Skip the 1st column. It's always visible.
	I++;

	// Fill list with the column names and check each one if it's visible
	for (; I != m_pHeadings->end(); I++)
	{
		HTREEITEM hItem = InsertCol(TVI_ROOT, (LPCTSTR) (*I)->m_Name.c_str(), (LPARAM) *I);

		m_Cols.SetCheck(hItem, (*I)->m_Visible);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HTREEITEM CListPreferencesDlg::InsertCol(HTREEITEM ParentItem, const TCHAR* pText, LPARAM pData)
{
	#define	BMP_COLUMN			0

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = ParentItem;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = (TCHAR*) pText;
	tvInsert.item.lParam = pData;
	tvInsert.item.iImage = BMP_COLUMN;
	tvInsert.item.iSelectedImage = BMP_COLUMN;

	return m_Cols.InsertItem(&tvInsert);
}

void CListPreferencesDlg::OnOK() 
{
	HTREEITEM Item = m_Cols.GetRootItem();

	while (Item)
	{
		CHeading* pH = (CHeading*) m_Cols.GetItemData(Item);

		pH->m_Visible = (bool) m_Cols.GetCheck(Item);

		Item = m_Cols.GetNextItem(Item, TVGN_NEXT);
	}
	
	CNonFSDialog::OnOK();
}
