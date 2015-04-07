#include "stdafx.h"
#include "GpxSonar.h"
#include "CTypeAndContainerDlg.h"
#include "CGpxParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTypeAndContainerDlg::CTypeAndContainerDlg(CWnd* pParent): CNonFSDialog(CTypeAndContainerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTypeAndContainerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CTypeAndContainerDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTypeAndContainerDlg)
	DDX_Control(pDX, IDC_LIST, m_Data);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTypeAndContainerDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CTypeAndContainerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTypeAndContainerDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	switch (m_FillerType)
	{
	case CacheTypes:
		{
		m_Data.AddString(TYPE_TRADITIONAL);
		m_Data.AddString(TYPE_MULTI);
		m_Data.AddString(TYPE_VIRTUAL);
		m_Data.AddString(TYPE_WEBCAM);
		m_Data.AddString(TYPE_UNKNOWN);
		m_Data.AddString(TYPE_LETTERBOXHYBRID);
		m_Data.AddString(TYPE_EVENT);
		m_Data.AddString(TYPE_PROJECTAPE);
		m_Data.AddString(TYPE_LOCATIONLESS);
		m_Data.AddString(TYPE_CITO);
		}
		break;
	case CacheContainers:
		{
		m_Data.AddString(CONT_UNKNOWN);
		m_Data.AddString(CONT_MICRO);
		m_Data.AddString(CONT_SMALL);
		m_Data.AddString(CONT_REGULAR);
		m_Data.AddString(CONT_LARGE);
		m_Data.AddString(CONT_VIRTUAL);
		m_Data.AddString(CONT_NOTLISTED);
		}
		break;
	}

	int Idx = m_Data.FindStringExact(-1, (LPCTSTR) m_Selection);

	if (Idx != CB_ERR)
	{
		m_Data.SetCurSel(Idx);
	}
	else
	{
		m_Data.SetCurSel(0);
	}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTypeAndContainerDlg::OnOK() 
{
	UpdateData(true);
	
	m_Data.GetLBText(m_Data.GetCurSel(), m_Selection);

	CNonFSDialog::OnOK();
}
