#include "stdafx.h"
#include "GpxSonar.h"
#include "CDistanceUnitsDlg.h"
#include "CCoords.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {
	DistUnitsKm = 0,
	DistUnitsStatute,
	DistUnitsNautical
} DistUnits;


CDistanceUnitsDlg::CDistanceUnitsDlg(CWnd* pParent) : CNonFSDialog(CDistanceUnitsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDistanceUnitsDlg)
	m_DistUnits = -1;
	//}}AFX_DATA_INIT
}


void CDistanceUnitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDistanceUnitsDlg)
	DDX_Radio(pDX, IDC_UNITS1, m_DistUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDistanceUnitsDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CDistanceUnitsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDistanceUnitsDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDistanceUnitsDlg::SetDistanceUnits(double Units)
{
	if (Units == DIST_UNITS_IN_KM)
	{
		m_DistUnits = DistUnitsKm;
	}
	else if (Units == DIST_UNITS_IN_STATUTE_MILES)
	{
		m_DistUnits = DistUnitsStatute;
	}
	else if (Units == DIST_UNITS_IN_NAUTICAL_MILES)
	{
		m_DistUnits = DistUnitsNautical;
	}
}

double CDistanceUnitsDlg::GetDistanceUnits(CString& Label)
{
	switch (m_DistUnits)
	{
	case DistUnitsKm:
		Label = KILOMETERS;
		return DIST_UNITS_IN_KM;
		break;
	case DistUnitsStatute:
		Label = STATUTE_MILES;
		return DIST_UNITS_IN_STATUTE_MILES;
		break;
	case DistUnitsNautical:
		Label = NAUTICAL_MILES;
		return DIST_UNITS_IN_NAUTICAL_MILES;
		break;
	}

	Label = _T("?");
	return 0;
}

void CDistanceUnitsDlg::OnOK() 
{
	UpdateData(true);

	CNonFSDialog::OnOK();
}
