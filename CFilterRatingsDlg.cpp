#include "stdafx.h"
#include "gpxsonar.h"
#include "CFilterRatingsDlg.h"
#include "CFilterCacheRatings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterRatingsDlg::CFilterRatingsDlg(CWnd* pParent): CNonFSDialog(CFilterRatingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterRatingsDlg)
	m_DiffEnabled = FALSE;
	m_TerrEnabled = FALSE;
	//}}AFX_DATA_INIT

	m_pRatings = 0;
}

void CFilterRatingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterRatingsDlg)
	DDX_Control(pDX, IDC_TERR_OPER, m_TerrOper);
	DDX_Control(pDX, IDC_TERR_LVL, m_TerrLvl);
	DDX_Control(pDX, IDC_DIFF_OPER, m_DiffOper);
	DDX_Control(pDX, IDC_DIFF_LVL, m_DiffLvl);
	DDX_Check(pDX, IDC_DIFFICULTY, m_DiffEnabled);
	DDX_Check(pDX, IDC_TERRAIN, m_TerrEnabled);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterRatingsDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterRatingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterRatingsDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	int Idx = m_TerrLvl.FindStringExact(-1, (LPCTSTR) ToText(m_pRatings->m_TerrLvl));

	if (Idx != CB_ERR)
	{
		m_TerrLvl.SetCurSel(Idx);
	}
	else
	{
		m_TerrLvl.SetCurSel(0);
	}

	Idx = m_DiffLvl.FindStringExact(-1, (LPCTSTR) ToText(m_pRatings->m_DiffLvl));

	if (Idx != CB_ERR)
	{
		m_DiffLvl.SetCurSel(Idx);
	}
	else
	{
		m_DiffLvl.SetCurSel(0);
	}

	m_TerrOper.SetCurSel(m_pRatings->m_TerrOper);
	m_DiffOper.SetCurSel(m_pRatings->m_DiffOper);

	m_DiffEnabled = (BOOL) m_pRatings->m_DiffEnabled;
	m_TerrEnabled = (BOOL) m_pRatings->m_TerrEnabled;

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterRatingsDlg::OnCancel() 
{
	UpdateData(true);

	CString Rating;

	m_TerrLvl.GetLBText(m_TerrLvl.GetCurSel(), Rating);
	m_pRatings->m_TerrLvl = ToNumerical(Rating);
	m_pRatings->m_TerrOper = (OperType) m_TerrOper.GetCurSel();

	m_DiffLvl.GetLBText(m_DiffLvl.GetCurSel(), Rating);
	m_pRatings->m_DiffLvl = ToNumerical(Rating);
	m_pRatings->m_DiffOper = (OperType) m_DiffOper.GetCurSel();

	m_pRatings->m_DiffEnabled = (bool) m_DiffEnabled;
	m_pRatings->m_TerrEnabled = (bool) m_TerrEnabled;

	CNonFSDialog::OnCancel();
}

double CFilterRatingsDlg::ToNumerical(CString& Rating)
{
	double Val;

	swscanf((LPCTSTR) Rating, _T("%lf"), &Val);

	return Val;
}

CString CFilterRatingsDlg::ToText(double RatingVal)
{
	CString Rating;

	Rating.Format(_T("%.1f"), RatingVal);

	return Rating;
}
