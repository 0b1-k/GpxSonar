#include "stdafx.h"
#include "GpxSonar.h"
#include "CCacheImportDlg.h"
#include "CGpxParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCacheImportDlg::CCacheImportDlg(CWnd* pParent): CNonFSDialog(CCacheImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCacheImportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_ShowMineOnly = false;
	m_pCache = 0;
}

void CCacheImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCacheImportDlg)
	DDX_Control(pDX, IDC_SHOW_MINE, m_ShowMine);
	DDX_Control(pDX, IDC_CACHES, m_Caches);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCacheImportDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CCacheImportDlg)
	ON_BN_CLICKED(IDC_SHOW_MINE, OnShowMine)
	ON_LBN_DBLCLK(IDC_CACHES, OnDblclkCaches)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCacheImportDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	PopulateList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCacheImportDlg::OnOK() 
{
	int Idx = m_Caches.GetCurSel();

	if (Idx != LB_ERR)
	{
		m_pCache = (CGeoCache*) m_Caches.GetItemDataPtr(Idx);
	}
	else
	{
		m_pCache = 0;
	}
	
	CNonFSDialog::OnOK();
}

void CCacheImportDlg::OnShowMine() 
{
	m_ShowMineOnly = (bool) m_ShowMine.GetCheck();
	
	PopulateList();
}

void CCacheImportDlg::PopulateList() 
{
	itGC	C;
	CString	Temp;
	int		Size = m_Owner.size();
	TCHAR*	pOwner = (TCHAR*) m_Owner.c_str();

	CGpxParser* pParser = ((CGpxSonarApp*) AfxGetApp())->m_pGpxParser;

	m_Caches.ResetContent();

	CGeoCache* pCache = pParser->First(C);

	while (!pParser->EndOfCacheList(C))
	{
		if (pCache->m_InScope)
		{
			bool Include = true;

			if (m_ShowMineOnly)
			{
				if (Size && _tcsnicmp(pCache->m_GsCacheOwnerName.c_str(), pOwner, Size))
				{
					Include = false;
				}
			}

			if (Include)
			{
				Temp = pCache->m_Shortname.c_str();
				Temp += _T(": ");
				Temp += pCache->m_GsCacheName.c_str();

				int Idx = m_Caches.InsertString(0, Temp);

				m_Caches.SetItemDataPtr(Idx, (void*) pCache);
			}
		}

		pCache = pParser->Next(C);
	}

	m_Caches.SetCurSel(0);
}

void CCacheImportDlg::OnDblclkCaches() 
{
	OnOK();
}
