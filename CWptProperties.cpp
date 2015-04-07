#include "stdafx.h"
#include "GpxSonar.h"
#include "CWptProperties.h"
#include "CCenterCoordsDlg.h"
#include "CCoords.h"
#include "CCacheMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWptProperties::CWptProperties(CWnd* pParent): CNonFSDialog(CWptProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWptProperties)
	m_Comment = _T("");
	m_CoordsText = _T("");
	m_Name = _T("");
	//}}AFX_DATA_INIT

	m_pWpt = 0;
}

void CWptProperties::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWptProperties)
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	DDV_MaxChars(pDX, m_Comment, 100);
	DDX_Text(pDX, IDC_COORDS_TEXT, m_CoordsText);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 8);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWptProperties, CNonFSDialog)
	//{{AFX_MSG_MAP(CWptProperties)
	ON_BN_CLICKED(IDC_COORDS, OnCoords)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWptProperties::OnInitDialog() 
{
	CCoords Coords;

	CNonFSDialog::OnInitDialog();
	
	if (!m_Title.IsEmpty())
	{
		SetWindowText((LPCTSTR) m_Title);
	}

	m_Name = m_pWpt->m_Name.c_str();
	m_Comment = m_pWpt->m_Desc.c_str();
	
	Coords.SetDecimal(m_pWpt->m_Lat, m_pWpt->m_Long);
	
	m_CoordsText = Coords.Format(true).c_str();

	UpdateData(false);

	::SHSipPreference(m_hWnd, SIP_UP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWptProperties::OnCoords() 
{
	UpdateData(true);

	CCenterCoordsDlg Dlg;

	CCoords Coords;

	Coords.SetDecimal(m_pWpt->m_Lat, m_pWpt->m_Long);

	Coords.GetDegMinMmm(
					Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
					Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);

	if (Dlg.DoModal() == IDOK)
	{
		Coords.SetDegMinMmm(
				Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
				Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);

		Coords.GetDecimal(m_pWpt->m_Lat, m_pWpt->m_Long);

		m_CoordsText = Coords.Format(true).c_str();

		UpdateData(false);
	}
}

void CWptProperties::OnCancel() 
{
	UpdateData(true);

	m_pWpt->m_Name = (LPCTSTR) m_Name;
	m_pWpt->m_Desc = (LPCTSTR) m_Comment;

	::SHSipPreference(m_hWnd, SIP_DOWN);

	CNonFSDialog::OnCancel();	
}
