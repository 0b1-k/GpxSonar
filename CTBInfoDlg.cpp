#include "stdafx.h"
#include "GpxSonar.h"
#include "CTBInfoDlg.h"
#include "CTBMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTBInfoDlg::CTBInfoDlg(CWnd* pParent): CNonFSDialog(CTBInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTBInfoDlg)
	m_Name = _T("");
	m_Serial = _T("");
	m_Location = _T("");
	m_Ref = _T("");
	//}}AFX_DATA_INIT

	m_pTB = 0;
}

void CTBInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTBInfoDlg)
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 100);
	DDX_Text(pDX, IDC_SERIAL, m_Serial);
	DDV_MaxChars(pDX, m_Serial, 15);
	DDX_Text(pDX, IDC_LOCATION, m_Location);
	DDX_Text(pDX, IDC_REF, m_Ref);
	DDV_MaxChars(pDX, m_Ref, 20);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTBInfoDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CTBInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTBInfoDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	m_Name = m_pTB->m_Name.c_str();
	m_Serial = m_pTB->m_Serial.c_str();
	m_Location = m_pTB->m_CacheShortName.c_str();
	m_Ref = m_pTB->m_Ref.c_str();
	
	UpdateData(false);

	::SHSipPreference(m_hWnd, SIP_UP);

	return true;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTBInfoDlg::OnOK() 
{
	UpdateData(true);
	
	if (m_Name.IsEmpty() || m_Serial.IsEmpty() || m_Ref.IsEmpty())
	{
		MessageBox(_T("Please supply all fields"), _T("Missing TB Info"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	m_Ref.MakeUpper();

	m_pTB->m_Name = (LPCTSTR) m_Name;
	m_pTB->m_Serial = (LPCTSTR) m_Serial;
	m_pTB->m_CacheShortName = (LPCTSTR) m_Location;
	m_pTB->m_Ref = (LPCTSTR) m_Ref;

	::SHSipPreference(m_hWnd, SIP_DOWN);

	CNonFSDialog::OnOK();
}

void CTBInfoDlg::OnCancel() 
{
	::SHSipPreference(m_hWnd, SIP_DOWN);
	
	CNonFSDialog::OnCancel();
}
