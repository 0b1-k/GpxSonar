#include "stdafx.h"
#include "GpxSonar.h"
#include "CCenterCoordsDlg.h"
#include "CProjectWptDlg.h"
#include "CWaypointsDlg.h"
#include "CCacheMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCenterCoordsDlg::CCenterCoordsDlg(CWnd* pParent): CNonFSDialog(CCenterCoordsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCenterCoordsDlg)
	m_LatDeg = 0;
	m_LatMinMmm = 0.0f;
	m_LongDeg = 0;
	m_LongMinMmm = 0.0f;
	//}}AFX_DATA_INIT

	m_pTitle = 0;
	m_BookmarkMode = false;
	m_pWptsMgr = 0;
}

void CCenterCoordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCenterCoordsDlg)
	DDX_Control(pDX, IDC_BOOKMARK, m_BookmarksCtl);
	DDX_Control(pDX, IDC_LONG_COMBO, m_Long);
	DDX_Control(pDX, IDC_LAT_COMBO, m_Lat);
	DDX_Text(pDX, IDC_LAT_DEG, m_LatDeg);
	DDX_Text(pDX, IDC_LAT_MIN_MMM, m_LatMinMmm);
	DDX_Text(pDX, IDC_LONG_DEG, m_LongDeg);
	DDX_Text(pDX, IDC_LONG_MIN_MMM, m_LongMinMmm);
	//}}AFX_DATA_MAP
}

void CCenterCoordsDlg::SetTitle(const TCHAR* pTitle)
{
	m_pTitle = (TCHAR*) pTitle;
}

BEGIN_MESSAGE_MAP(CCenterCoordsDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CCenterCoordsDlg)
	ON_BN_CLICKED(IDC_PROJECT, OnProject)
	ON_BN_CLICKED(IDC_BOOKMARK, OnBookmark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCenterCoordsDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	if (m_pTitle)
	{
		SetWindowText(m_pTitle);
	}

	if (m_BookmarkMode)
	{
		m_BookmarksCtl.EnableWindow();
	}

	m_Lat.SetCurSel(m_Lat.FindStringExact(0, m_LatChar.c_str()));
	m_Long.SetCurSel(m_Long.FindStringExact(0, m_LongChar.c_str()));

	::SHSipPreference(m_hWnd, SIP_UP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCenterCoordsDlg::OnOK() 
{
	UpdateData(true);
	
	CString Char;

	m_Lat.GetLBText(m_Lat.GetCurSel(), Char);
	m_LatChar = (LPCTSTR) Char;

	m_Long.GetLBText(m_Long.GetCurSel(), Char);
	m_LongChar = (LPCTSTR) Char;

	::SHSipPreference(m_hWnd, SIP_DOWN);

	CNonFSDialog::OnOK();
}

void CCenterCoordsDlg::OnCancel()
{
	::SHSipPreference(m_hWnd, SIP_DOWN);

	CNonFSDialog::OnCancel();
}

void CCenterCoordsDlg::OnProject() 
{
	UpdateData(true);
	
	CProjectWptDlg Dlg;

	Dlg.m_Coords.SetDegMinMmm(m_LatDeg, m_LatMinMmm, m_LatChar, m_LongDeg, m_LongMinMmm, m_LongChar);

	if (Dlg.DoModal() == IDOK)
	{
		Dlg.m_Coords.GetDegMinMmm(m_LatDeg, m_LatMinMmm, m_LatChar, m_LongDeg, m_LongMinMmm, m_LongChar);

		UpdateData(false);
	}
}

void CCenterCoordsDlg::OnBookmark() 
{
	CWaypointsDlg	Dlg;

	Dlg.m_pWptsMgr = m_pWptsMgr;
	Dlg.m_BookmarkMode = m_BookmarkMode;
	
	if (Dlg.DoModal() == IDOK)
	{
		CCoords	Coords;

		Coords.SetDecimal(Dlg.m_pSelectedWp->m_Lat, Dlg.m_pSelectedWp->m_Long);

		Coords.GetDegMinMmm(m_LatDeg, m_LatMinMmm, m_LatChar, m_LongDeg, m_LongMinMmm, m_LongChar);

		UpdateData(false);
	}
}
