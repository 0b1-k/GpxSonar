#include "stdafx.h"
#include "GpxSonar.h"
#include "CDatePickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDatePickerDlg::CDatePickerDlg(CWnd* pParent): CNonFSDialog(CDatePickerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDatePickerDlg)
	//}}AFX_DATA_INIT
}

void CDatePickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDatePickerDlg)
	DDX_Control(pDX, IDC_YEAR, m_Year);
	DDX_Control(pDX, IDC_MONTH, m_Month);
	DDX_Control(pDX, IDC_DAY, m_Day);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDatePickerDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CDatePickerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDatePickerDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
		
	m_Day.SetCurSel(m_Date.wDay - 1);
	m_Month.SetCurSel(m_Date.wMonth - 1);
	m_Year.SetCurSel(m_Date.wYear - 2001);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDatePickerDlg::OnOK() 
{
	UpdateData(true);

	m_Date.wDay = m_Day.GetCurSel() + 1;
	m_Date.wMonth = m_Month.GetCurSel() + 1;
	m_Date.wYear = m_Year.GetCurSel() + 2001;
	
	CNonFSDialog::OnOK();
}
