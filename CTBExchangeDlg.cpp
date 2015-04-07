#include "stdafx.h"
#include "GpxSonar.h"
#include "CTBExchangeDlg.h"
#include "CGpxParser.h"
#include "CTBMgr.h"
#include "CTextTrx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTBExchangeDlg::CTBExchangeDlg(CWnd* pParent /*=NULL*/)
	: CNonFSDialog(CTBExchangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTBExchangeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CTBExchangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTBExchangeDlg)
	DDX_Control(pDX, IDC_MY_TB_LIST, m_MyTBs);
	DDX_Control(pDX, IDC_CACHE_TB_LIST, m_CacheTBs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTBExchangeDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CTBExchangeDlg)
	ON_BN_CLICKED(IDC_DROP, OnDrop)
	ON_BN_CLICKED(IDC_GRAB, OnGrab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTBExchangeDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	UpdateCacheList();
	UpdateInventoryList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTBExchangeDlg::UpdateCacheList()
{
	m_CacheTBs.ResetContent();

	CGeoCache*	pGC = ((CGpxSonarApp*)AfxGetApp())->m_pCurrCache;
	CTBMgr*		pTBMgr = ((CGpxSonarApp*)AfxGetApp())->m_pTBMgr;

	CTextTrx	TT;

	itTB TB;

	// Run through the bugs in the cache if any and add them to the list only if they're not in the inventory already
	CTravelBug* pTB = pGC->FirstTB(TB);

	while (!pGC->EndOfTBList(TB))
	{
		CTB* pInvTB = pTBMgr->Find(pTB->m_Ref);

		if ((!pInvTB))
		{
			pTB->m_Name = TT.Trim(pTB->m_Name);

			// Not in the TB inventory so add it to the TBs in the cache
			int Item = m_CacheTBs.AddString(pTB->m_Name.c_str());
			m_CacheTBs.SetItemDataPtr(Item, (void*) pTB);
		}

		pTB = pGC->NextTB(TB);
	}

	// Run through the TBs in the inventory that have been 'dropped' in the cache
	itTB2 TB2;

	CTB* pInvTB = pTBMgr->First(TB2);

	while (!pTBMgr->EndOfList(TB2))
	{
		if (pInvTB->m_CacheShortName == pGC->m_Shortname)
		{
			int Item = m_CacheTBs.AddString(pInvTB->m_Name.c_str());
			m_CacheTBs.SetItemDataPtr(Item, (void*) pInvTB);
		}

		pInvTB = pTBMgr->Next(TB2);
	}
}

void CTBExchangeDlg::UpdateInventoryList()
{
	m_MyTBs.ResetContent();

	CTBMgr* pTBMgr = ((CGpxSonarApp*)AfxGetApp())->m_pTBMgr;

	itTB2 TB;

	CTB* pInvTB = pTBMgr->First(TB);

	while (!pTBMgr->EndOfList(TB))
	{
		if (pInvTB->m_CacheShortName == TB_INVENTORY)
		{
			int Item = m_MyTBs.AddString(pInvTB->m_Name.c_str());
			m_MyTBs.SetItemDataPtr(Item, (void*) pInvTB);
		}

		pInvTB = pTBMgr->Next(TB);
	}
}

void CTBExchangeDlg::OnDrop() 
{
	CGeoCache* pGC = ((CGpxSonarApp*)AfxGetApp())->m_pCurrCache;

	// Run trough the checked TBs in the inventory and assign them to the current cache
	int Count = m_MyTBs.GetCount();

	while (Count--)
	{
		if (m_MyTBs.GetSel(Count))
		{
			CTB* pInvTB = (CTB*) m_MyTBs.GetItemDataPtr(Count);

			pInvTB->AssignToCache(pGC->m_Shortname);

			CTravelBug* pTB = new CTravelBug;

			if (!pTB)
			{
				//assert(0);
				return;
			}

			pTB->m_Id = pInvTB->m_Id;
			pTB->m_Name = pInvTB->m_Name;
			pTB->m_Ref = pInvTB->m_Ref;

			pGC->AddTravelBug(pTB);

			m_TBActionMgr.RecordAction(pInvTB, TbActionDrop, TbFromInventory);
		}
	}

	UpdateCacheList();
	UpdateInventoryList();
}

void CTBExchangeDlg::OnGrab()
{
	CGeoCache* pGC = ((CGpxSonarApp*)AfxGetApp())->m_pCurrCache;
	CTBMgr* pTBMgr = ((CGpxSonarApp*)AfxGetApp())->m_pTBMgr;

	// Run trough the checked TBs in the cache and assign them to the inventory
	int Count = m_CacheTBs.GetCount();

	while (Count--)
	{
		if (m_CacheTBs.GetSel(Count))
		{
			CTravelBug* pTB = (CTravelBug*) m_CacheTBs.GetItemDataPtr(Count);

			// Check if the TB already exists in the inventroy before creating a new one
			CTB* pInvTB = pTBMgr->Find(pTB->m_Ref);

			if (!pInvTB)
			{
				// Create a new TB in the inventory with a serial # set to zero
				pInvTB = pTBMgr->Add(_T("0"), pTB->m_Name);
			}
			else
			{
				// Assign the TB to the inventory
				pInvTB->AssignToCache(TB_INVENTORY);
			}

			// The rest of the data
			pInvTB->m_Id = pTB->m_Id;
			pInvTB->m_Ref = pTB->m_Ref;

			// Remove the TB from the cache
			pGC->RemoveTravelBug(pTB);

			m_TBActionMgr.RecordAction(pInvTB, TbActionGrab, TbFromCache);
		}
	}

	UpdateCacheList();
	UpdateInventoryList();
}
