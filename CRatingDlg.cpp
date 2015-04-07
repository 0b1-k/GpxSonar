#include "stdafx.h"
#include "GpxSonar.h"
#include "CRatingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRatingDlg::CRatingDlg(CWnd* pParent): CNonFSDialog(CRatingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRatingDlg)
	//}}AFX_DATA_INIT
}

void CRatingDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingDlg)
	DDX_Control(pDX, IDC_RATING, m_Rating);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRatingDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CRatingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRatingDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	SetWindowText((LPCTSTR) m_Title);

	int Idx = m_Rating.FindStringExact(-1, (LPCTSTR) ToText(m_RatingVal));

	if (Idx != CB_ERR)
	{
		m_Rating.SetCurSel(Idx);
	}
	else
	{
		m_Rating.SetCurSel(0);
	}

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRatingDlg::OnOK() 
{
	UpdateData(true);

	CString Rating;

	m_Rating.GetLBText(m_Rating.GetCurSel(), Rating);

	m_RatingVal = ToNumerical(Rating);
	
	CNonFSDialog::OnOK();
}

double CRatingDlg::ToNumerical(CString& Rating)
{
	double Val;

	swscanf((LPCTSTR) Rating, _T("%lf"), &Val);

	return Val;
}

CString CRatingDlg::ToText(double RatingVal)
{
	CString Rating;

	Rating.Format(_T("%.1f"), RatingVal);

	return Rating;
}
