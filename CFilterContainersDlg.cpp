#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterContainersDlg.h"
#include "CFilterCacheContainers.h"
#include "IDB_CACHES.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterContainersDlg::CFilterContainersDlg(CWnd* pParent) : CNonFSDialog(CFilterContainersDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterContainersDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pConts = 0;
}

CFilterContainersDlg::~CFilterContainersDlg()
{
	m_ImageList.DeleteImageList();
}

void CFilterContainersDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterContainersDlg)
	DDX_Control(pDX, IDC_OPTIONS, m_Opts);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterContainersDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterContainersDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterContainersDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	// Setup the icon list for the tree control
	m_ImageList.Create(IDB_CACHES, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	m_Opts.SetImageList(&m_ImageList, TVSIL_NORMAL);

	for (itFiltCacheContainer it = m_pConts->m_Conts.begin(); it != m_pConts->m_Conts.end(); it++)
	{
		CFiltContainerTypes* pFCT = *it;

		// Do not list the unknown container type
		if (GC_Unknown != pFCT->m_Container)
		{
			HTREEITEM Item = InsertItem(m_Opts, TVI_ROOT, Lookup(pFCT->m_Container), (void*) pFCT, GENERIC_CONTAINER);

			m_Opts.SetCheck(Item, pFCT->m_Enabled);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HTREEITEM CFilterContainersDlg::InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image)
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

void CFilterContainersDlg::OnCancel() 
{
	HTREEITEM Item = m_Opts.GetRootItem();

	while (Item)
	{
		CFiltContainerTypes* pFCT = (CFiltContainerTypes*) m_Opts.GetItemData(Item);

		pFCT->m_Enabled = (bool) m_Opts.GetCheck(Item);

		Item = m_Opts.GetNextItem(Item, TVGN_NEXT);
	}

	CNonFSDialog::OnCancel();
}

const TCHAR* CFilterContainersDlg::Lookup(GcContainer Cont)
{
	switch(Cont)
	{
	case GC_Unknown:
		return CONT_UNKNOWN;
		break;
	case GC_Micro:
		return CONT_MICRO;
		break;
	case GC_Small:
		return CONT_SMALL;
		break;
	case GC_Regular:
		return CONT_REGULAR;
		break;
	case GC_Large:
		return CONT_LARGE;
		break;
	case GC_Virtual:
		return CONT_VIRTUAL;
		break;
	case GC_NotListed:
		return CONT_NOTLISTED;
		break;
	}

	return _T("?");
}
