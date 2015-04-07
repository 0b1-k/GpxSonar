#include "stdafx.h"
#include "GpxSonar.h"
#include "CTextEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTextEditDlg::CTextEditDlg(CWnd* pParent): CNonFSDialog(CTextEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextEditDlg)
	m_Text = _T("");
	m_Html = FALSE;
	//}}AFX_DATA_INIT
}

void CTextEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextEditDlg)
	DDX_Control(pDX, IDC_HTML, m_HtmlCtl);
	DDX_Text(pDX, IDC_EDIT, m_Text);
	DDX_Check(pDX, IDC_HTML, m_Html);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextEditDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CTextEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTextEditDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	SetWindowText((LPCTSTR) m_Title);

	m_HtmlCtl.EnableWindow((BOOL)m_EnableHtml);

	UpdateData(false);
	
	::SHSipPreference(m_hWnd, SIP_UP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextEditDlg::OnOK() 
{
	UpdateData(true);
	
	::SHSipPreference(m_hWnd, SIP_DOWN);

	CNonFSDialog::OnOK();
}

void CTextEditDlg::OnCancel() 
{
	::SHSipPreference(m_hWnd, SIP_DOWN);
	
	CNonFSDialog::OnCancel();
}
