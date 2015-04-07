#include "stdafx.h"
#include "GpxSonar.h"
#include "CCacheDetailsHints.h"
#include "CTextTrx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCacheDetailsHints::CCacheDetailsHints(CWnd* pParent) : CNonFSDialog(CCacheDetailsHints::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCacheDetailsHints)
	m_Hints = _T("");
	//}}AFX_DATA_INIT
}

CCacheDetailsHints::~CCacheDetailsHints()
{
}

void CCacheDetailsHints::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCacheDetailsHints)
	DDX_Control(pDX, IDC_HINTS, m_HintsCtrl);
	DDX_Text(pDX, IDC_HINTS, m_Hints);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCacheDetailsHints, CNonFSDialog)
	//{{AFX_MSG_MAP(CCacheDetailsHints)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCacheDetailsHints::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	// Convert any HTML breaks into CRLF
	CTextTrx	TT;

	String	Hints = (LPCTSTR) m_Hints;

	TT.BRToCRLF(Hints);

	m_Hints = Hints.c_str();

	CRect	Rect;
	GetClientRect(&Rect);
	m_HintsCtrl.MoveWindow(&Rect);

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
