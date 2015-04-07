#include "stdafx.h"
#include "gpxsonar.h"
#include "CCacheCategoryDlg.h"
#include "CCacheMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCacheCategoryDlg::CCacheCategoryDlg(CWnd* pParent): CNonFSDialog(CCacheCategoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCacheCategoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCacheCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCacheCategoryDlg)
	DDX_Control(pDX, IDC_CATEGORIES, m_Data);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCacheCategoryDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CCacheCategoryDlg)
	ON_BN_CLICKED(ID_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCacheCategoryDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	CCacheMgr* pCM = ((CGpxSonarApp*) AfxGetApp())->m_pCacheMgr;

	// Fill out the list of categories to pick from
	for (itStr S = pCM->m_CacheCategories.begin(); S != pCM->m_CacheCategories.end(); S++)
	{
		m_Data.AddString((*S).c_str());
	}

	int Idx = m_Data.FindStringExact(-1, (LPCTSTR) m_Selection);

	if (Idx != CB_ERR)
	{
		m_Data.SetCurSel(Idx);
	}
	else
	{
		m_Data.SetCurSel(0);
	}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCacheCategoryDlg::OnOk() 
{
	UpdateData(true);
	
	m_Data.GetLBText(m_Data.GetCurSel(), m_Selection);

	CNonFSDialog::OnOK();	
}
