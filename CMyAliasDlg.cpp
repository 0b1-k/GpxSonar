#include "stdafx.h"
#include "GpxSonar.h"
#include "CMyAliasDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyAliasDlg::CMyAliasDlg(CWnd* pParent) : CNonFSDialog(CMyAliasDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyAliasDlg)
	m_Alias = _T("");
	//}}AFX_DATA_INIT
}

void CMyAliasDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyAliasDlg)
	DDX_Text(pDX, IDC_ALIAS, m_Alias);
	DDV_MaxChars(pDX, m_Alias, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyAliasDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CMyAliasDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyAliasDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	UpdateData(false);
	
	::SHSipPreference(m_hWnd, SIP_UP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyAliasDlg::OnOK() 
{
	UpdateData(true);
	
	::SHSipPreference(m_hWnd, SIP_DOWN);

	CNonFSDialog::OnOK();
}

void CMyAliasDlg::OnCancel() 
{
	::SHSipPreference(m_hWnd, SIP_DOWN);
	
	CNonFSDialog::OnCancel();
}
