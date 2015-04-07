#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterSearch.h"
#include "CSearchDlg.h"
#include "CMd5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSearchDlg::CSearchDlg(CWnd* pParent): CNonFSDialog(CSearchDlg::IDD, pParent)
{
	m_pSearch = 0;
	m_ShowEnableSearch = false;

	//{{AFX_DATA_INIT(CSearchDlg)
	m_Match = ExactMatch;
	m_Text = _T("");
	m_EnableSearch = FALSE;
	//}}AFX_DATA_INIT
}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchDlg)
	DDX_Control(pDX, IDC_ENABLE_SEARCH, m_EnableSearchCtl);
	DDX_Control(pDX, IDC_TEXT, m_TextCtl);
	DDX_Radio(pDX, IDC_MATCH, m_Match);
	DDX_Text(pDX, IDC_TEXT, m_Text);
	DDV_MaxChars(pDX, m_Text, 40);
	DDX_Check(pDX, IDC_ENABLE_SEARCH, m_EnableSearch);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CSearchDlg)
	ON_BN_CLICKED(IDC_ENABLE_SEARCH, OnEnableSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSearchDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	m_Match = m_pSearch->m_Match;

	// Ensures that the search string is blank everytime the dialog shows up
	//m_Text = m_pSearch->m_Text.c_str();
	
	m_EnableSearch = (BOOL) m_pSearch->IsEnabled();

	if (m_ShowEnableSearch)
	{
		m_EnableSearchCtl.ShowWindow(SW_SHOW);
	}

	m_ModHash = HashValues();

	UpdateData(false);
	
	m_TextCtl.SetFocus();

	::SHSipPreference(m_hWnd, SIP_UP);

	return false;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

String CSearchDlg::HashValues()
{
	CMd5	Hash;
	
	Hash.Init();

	Hash.Update((BYTE*) (LPCTSTR) m_Text, m_Text.GetLength());
	Hash.Update((BYTE*) &m_Match, sizeof(m_Match));
	Hash.Update((BYTE*) &m_EnableSearch, sizeof(m_EnableSearch));

	return Hash.Final();
}

void CSearchDlg::OnCancel() 
{
	UpdateData(true);

	m_Text.MakeLower();

	m_pSearch->m_Match = (StringMatchType) m_Match;
	m_pSearch->m_Text = (LPCTSTR) m_Text;
	m_pSearch->Enable((bool) m_EnableSearch);

	::SHSipPreference(m_hWnd, SIP_DOWN);

	if (HashValues() != m_ModHash)
	{
		CNonFSDialog::OnOK();
	}
	else
	{
		CNonFSDialog::OnCancel();
	}
}

void CSearchDlg::OnEnableSearch() 
{
	UpdateData(true);
}

BOOL CSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
    switch (pMsg->message)
    {
		case WM_KEYDOWN:
		{
			switch (pMsg->wParam)
			{
				case VK_RETURN:
				{
					// Don't respond to the user pressing the ENTER key!
					return true;
				}
				break;
			}
		}
		break;
    }
	
	return CNonFSDialog::PreTranslateMessage(pMsg);
}
