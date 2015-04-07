#include "stdafx.h"
#include "gpxsonar.h"
#include "CCacheCategoryEditor.h"
#include "CCacheMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCacheCategoryEditor::CCacheCategoryEditor(CWnd* pParent): CNonFSDialog(CCacheCategoryEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCacheCategoryEditor)
	m_Cat = _T("");
	//}}AFX_DATA_INIT
}

void CCacheCategoryEditor::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCacheCategoryEditor)
	DDX_Control(pDX, IDC_CATLIST, m_CatList);
	DDX_Text(pDX, IDC_CAT, m_Cat);
	DDV_MaxChars(pDX, m_Cat, 30);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCacheCategoryEditor, CNonFSDialog)
	//{{AFX_MSG_MAP(CCacheCategoryEditor)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCacheCategoryEditor::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	CCacheMgr* pCM = ((CGpxSonarApp*) AfxGetApp())->m_pCacheMgr;

	// Fill out the list of categories to pick from
	for (itStr S = pCM->m_CacheCategories.begin(); S != pCM->m_CacheCategories.end(); S++)
	{
		m_CatList.AddString((*S).c_str());
	}
	
	m_CatList.SetCurSel(0);

	::SHSipPreference(m_hWnd, SIP_UP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCacheCategoryEditor::OnCancel() 
{
	::SHSipPreference(m_hWnd, SIP_FORCEDOWN);
	
	CNonFSDialog::OnOK();
}

void CCacheCategoryEditor::OnAdd() 
{
	UpdateData(true);

	CCacheMgr* pCM = ((CGpxSonarApp*) AfxGetApp())->m_pCacheMgr;

	if (pCM->AddCacheCategory(String((LPCTSTR)m_Cat)))
	{
		m_CatList.AddString(m_Cat);
	}

	m_Cat.Empty();

	UpdateData(false);
}

void CCacheCategoryEditor::OnRemove() 
{
	int Idx = m_CatList.GetCurSel();

	if (Idx != LB_ERR)
	{
		CString Category;

		m_CatList.GetText(Idx, Category);

		CCacheMgr* pCM = ((CGpxSonarApp*) AfxGetApp())->m_pCacheMgr;

		if (pCM->DelCacheCategory(String((LPCTSTR)Category)))
		{
			m_CatList.DeleteString(Idx);
		}
	}
}
