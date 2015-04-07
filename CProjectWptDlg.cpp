#include "stdafx.h"
#include "GpxSonar.h"
#include "CProjectWptDlg.h"
#include "CFilterBearingDistance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProjectWptDlg::CProjectWptDlg(CWnd* pParent) : CNonFSDialog(CProjectWptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectWptDlg)
	m_Bearing = 0.0;
	m_Dist = 0.0;
	//}}AFX_DATA_INIT
}

void CProjectWptDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectWptDlg)
	DDX_Control(pDX, IDC_Units, m_Units);
	DDX_Control(pDX, IDC_Direction, m_Dir);
	DDX_Text(pDX, IDC_Bearing, m_Bearing);
	DDV_MinMaxDouble(pDX, m_Bearing, 0., 360.0);
	DDX_Text(pDX, IDC_Distance, m_Dist);
	DDV_MinMaxDouble(pDX, m_Dist, 0., 99999.9);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProjectWptDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CProjectWptDlg)
	ON_CBN_SELCHANGE(IDC_Direction, OnSelchangeDirection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#define	DIST_IN_FEET_STR	_T("Feet")
#define	DIST_IN_METERS_STR	_T("Meters")
#define	DIST_IN_PACES_STR	_T("Paces")

#define	DIST_IN_FEET		3.2808399
#define	DIST_IN_METERS		1
#define	DIST_IN_PACES		1.48

#define DIST_IN_METERS_SELECTION	0
#define DIST_IN_FEET_SELECTION		1
#define DIST_IN_PACES_SELECTION		2

BOOL CProjectWptDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	int Idx = 0;

	// Populate the units combo
	Idx = m_Units.AddString(DIST_IN_METERS_STR);
	m_Units.SetItemData(Idx, DIST_IN_METERS_SELECTION);

	Idx = m_Units.AddString(DIST_IN_FEET_STR);
	m_Units.SetItemData(Idx, DIST_IN_FEET_SELECTION);

	Idx = m_Units.AddString(DIST_IN_PACES_STR);
	m_Units.SetItemData(Idx, DIST_IN_PACES_SELECTION);

	m_Units.SetCurSel(0);

	// Populate the direction combo
	Idx = m_Dir.AddString(BEARING_NORTH);
	m_Dir.SetItemData(Idx, BEARING_NORTH_DEGREE);
	
	Idx = m_Dir.AddString(BEARING_NORTHEAST);
	m_Dir.SetItemData(Idx, BEARING_NORTHEAST_DEGREE);

	Idx = m_Dir.AddString(BEARING_EAST);
	m_Dir.SetItemData(Idx, BEARING_EAST_DEGREE);

	Idx = m_Dir.AddString(BEARING_SOUTHEAST);
	m_Dir.SetItemData(Idx, BEARING_SOUTHEAST_DEGREE);

	Idx = m_Dir.AddString(BEARING_SOUTH);
	m_Dir.SetItemData(Idx, BEARING_SOUTH_DEGREE);

	Idx = m_Dir.AddString(BEARING_SOUTHWEST);
	m_Dir.SetItemData(Idx, BEARING_SOUTHWEST_DEGREE);

	Idx = m_Dir.AddString(BEARING_WEST);
	m_Dir.SetItemData(Idx, BEARING_WEST_DEGREE);

	Idx = m_Dir.AddString(BEARING_NORTHWEST);
	m_Dir.SetItemData(Idx, BEARING_NORTHWEST_DEGREE);

	m_Dir.SetCurSel(0);

	m_Bearing = BEARING_NORTH_DEGREE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProjectWptDlg::OnOK() 
{
	UpdateData(true);

	double	Distance;
	double	Azimuth;
	double	Units;

	switch (m_Units.GetItemData(m_Units.GetCurSel()))
	{
	case DIST_IN_METERS_SELECTION:
		Units = DIST_IN_METERS;
		break;
	case DIST_IN_FEET_SELECTION:
		Units = DIST_IN_FEET;
		break;
	case DIST_IN_PACES_SELECTION:
		Units = DIST_IN_PACES;
		break;
	}

	Distance = m_Dist / Units;

	Azimuth = (double) m_Bearing;

	CLatLon ProjectedCoords;
	
	ProjectedCoords = m_Coords.VincentyProjection(Azimuth, Distance);

	m_Coords.m_Latitude = ProjectedCoords.m_Latitude;
	m_Coords.m_Longitude = ProjectedCoords.m_Longitude;
	
	CNonFSDialog::OnOK();
}

void CProjectWptDlg::OnSelchangeDirection() 
{
	UpdateData(true);
	
	m_Bearing = (double) m_Dir.GetItemData(m_Dir.GetCurSel());

	UpdateData(false);
}
