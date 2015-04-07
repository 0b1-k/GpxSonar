#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterBearingDistanceDlg.h"
#include "CFilterBearingDistance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterBearingDistanceDlg::CFilterBearingDistanceDlg(CWnd* pParent /*=NULL*/)
	: CNonFSDialog(CFilterBearingDistanceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterBearingDistanceDlg)
	m_Dist = 500.0;
	//}}AFX_DATA_INIT

	m_pBearDist = 0;
}

CFilterBearingDistanceDlg::~CFilterBearingDistanceDlg()
{
	m_ImageList.DeleteImageList();
}

void CFilterBearingDistanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterBearingDistanceDlg)
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Text(pDX, IDC_DIST, m_Dist);
	DDV_MinMaxDouble(pDX, m_Dist, 0., 99999.99);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterBearingDistanceDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterBearingDistanceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterBearingDistanceDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)
	#define	COMPASS_BMP			0

	// Setup the icon list for the list control
	if (m_ImageList.Create(IDB_BEARINGS, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA))
	{
		m_Tree.SetImageList(&m_ImageList, TVSIL_NORMAL);

		for (itFiltBearing it = m_pBearDist->m_Bearings.begin(); it != m_pBearDist->m_Bearings.end(); it++)
		{
			CFilterBearing* pFB = *it;

			HTREEITEM Item = InsertItem(m_Tree, TVI_ROOT, pFB->m_Bearing.c_str(), (void*) pFB, COMPASS_BMP);

			m_Tree.SetCheck(Item, pFB->m_Enabled);
		}
	}
	
	m_Dist = m_pBearDist->m_Distance;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HTREEITEM CFilterBearingDistanceDlg::InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image)
{
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = ParentItem;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = (TCHAR*) pText;
	tvInsert.item.lParam = (LPARAM) pData;
	tvInsert.item.iImage = Image;
	tvInsert.item.iSelectedImage = Image;

	return Tree.InsertItem(&tvInsert);
}

void CFilterBearingDistanceDlg::OnCancel() 
{
	UpdateData(true);

	m_pBearDist->m_Distance = m_Dist; 

	HTREEITEM Item = m_Tree.GetRootItem();

	while (Item)
	{
		CFilterBearing* pFB = (CFilterBearing*) m_Tree.GetItemData(Item);

		pFB->m_Enabled = (bool) m_Tree.GetCheck(Item);

		Item = m_Tree.GetNextItem(Item, TVGN_NEXT);

	}

	CNonFSDialog::OnCancel();
}
