#include "stdafx.h"
#include "GpxSonar.h"
#include "CRot13ConvertDlg.h"
#include "CTextTrx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRot13ConvertDlg::CRot13ConvertDlg(CWnd* pParent /*=NULL*/)
	: CNonFSDialog(CRot13ConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRot13ConvertDlg)
	m_Text = _T("");
	//}}AFX_DATA_INIT
}


void CRot13ConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRot13ConvertDlg)
	DDX_Text(pDX, IDC_ROT13, m_Text);
	DDV_MaxChars(pDX, m_Text, 1024);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRot13ConvertDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CRot13ConvertDlg)
	ON_BN_CLICKED(IDC_CONVERT, OnConvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRot13ConvertDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	// Convert any HTML breaks into CRLF
	CTextTrx	TT;

	String	Hints = (LPCTSTR) m_Text;

	TT.BRToCRLF(Hints);

	m_Text = Hints.c_str();

	UpdateData(false);
	
	::SHSipPreference(m_hWnd, SIP_UP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRot13ConvertDlg::OnConvert() 
{
	UpdateData(true);

	if (m_Text.IsEmpty())
	{
		return;
	}

	CTextTrx TT;

	String Text = (LPCTSTR) m_Text;

	TT.Rot13(Text);

	m_Text = Text.c_str();

	UpdateData(false);
}

void CRot13ConvertDlg::OnCancel() 
{
	::SHSipPreference(m_hWnd, SIP_DOWN);
	
	CNonFSDialog::OnCancel();
}
