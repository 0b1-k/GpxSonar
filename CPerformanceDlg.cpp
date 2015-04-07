#include "stdafx.h"
#include "GpxSonar.h"
#include "CPerformanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4800)

CPerformanceDlg::CPerformanceDlg(CWnd* pParent): CNonFSDialog(CPerformanceDlg::IDD, pParent)
{
	m_MemMiser = false;

	//{{AFX_DATA_INIT(CPerformanceDlg)
	m_MemMiserCtl = 0;
	//}}AFX_DATA_INIT
}

void CPerformanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPerformanceDlg)
	DDX_Radio(pDX, IDC_MemUsage, m_MemMiserCtl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPerformanceDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CPerformanceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPerformanceDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	m_MemMiserCtl = (int) m_MemMiser;

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPerformanceDlg::OnOK() 
{
	UpdateData(true);
	
	m_MemMiser = (bool) m_MemMiserCtl;

	CNonFSDialog::OnOK();
}
