#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterStringsDlg.h"
#include "CFilterOnStrings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterStringsDlg::CFilterStringsDlg(CWnd* pParent) : CNonFSDialog(CFilterStringsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterStringsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pFiltStrs = 0;
}

CFilterStringsDlg::~CFilterStringsDlg()
{
	m_ImageList.DeleteImageList();
}

void CFilterStringsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterStringsDlg)
	DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterStringsDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterStringsDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterStringsDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)
	#define GENERIC_STR_BMP		0

	SetWindowText(m_pFiltStrs->GetName().c_str());

	// Setup the icon list for the list control
	if (m_ImageList.Create(IDB_STRINGS, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA))
	{
		m_Tree.SetImageList(&m_ImageList, TVSIL_NORMAL);
		
		for (itFiltStr S = m_pFiltStrs->m_Strs.begin(); S != m_pFiltStrs->m_Strs.end(); S++)
		{
			CFilteredString* pFS = *S;

			HTREEITEM Item = InsertItem(m_Tree, TVI_ROOT, pFS->m_Str.c_str(), (void*) pFS, GENERIC_STR_BMP);

			m_Tree.SetCheck(Item, pFS->m_Enabled);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HTREEITEM CFilterStringsDlg::InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image)
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

void CFilterStringsDlg::OnDelete() 
{
	HTREEITEM Item = m_Tree.GetSelectedItem();

	if (Item)
	{	
		CFilteredString* pFS = (CFilteredString*) m_Tree.GetItemData(Item);

		m_pFiltStrs->Delete(pFS);

		m_Tree.DeleteItem(Item);
	}
}

void CFilterStringsDlg::OnCancel() 
{
	HTREEITEM Item = m_Tree.GetRootItem();

	while (Item)
	{
		CFilteredString* pFS = (CFilteredString*) m_Tree.GetItemData(Item);

		pFS->m_Enabled = (bool) m_Tree.GetCheck(Item);

		Item = m_Tree.GetNextItem(Item, TVGN_NEXT);
	}
	
	CNonFSDialog::OnCancel();
}
