#include "stdafx.h"
#include "GpxSonar.h"
#include "CLineEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLineEditDlg::CLineEditDlg(CWnd* pParent): CNonFSDialog(CLineEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineEditDlg)
	m_Text = _T("");
	//}}AFX_DATA_INIT
}

void CLineEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineEditDlg)
	DDX_Text(pDX, IDC_EDIT, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineEditDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CLineEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CLineEditDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	SetWindowText((LPCTSTR) m_Title);

	UpdateData(false);
	
	::SHSipPreference(m_hWnd, SIP_UP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLineEditDlg::OnOK() 
{
	UpdateData(true);
	
	::SHSipPreference(m_hWnd, SIP_DOWN);

	CNonFSDialog::OnOK();
}

void CLineEditDlg::OnCancel() 
{
	::SHSipPreference(m_hWnd, SIP_DOWN);
	
	CNonFSDialog::OnCancel();
}
