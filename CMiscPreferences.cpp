#include "stdafx.h"
#include "gpxsonar.h"
#include "CMiscPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMiscPreferences::CMiscPreferences(CWnd* pParent): CNonFSDialog(CMiscPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiscPreferences)
	m_DblClickCacheDetails = FALSE;
	m_ReloadLastGpxFile = FALSE;
	m_SpoilerPicsPath = _T("");
	m_StripImgTags = FALSE;
	//}}AFX_DATA_INIT
}

void CMiscPreferences::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiscPreferences)
	DDX_Check(pDX, IDC_DOUBLECLICK_CACHE_DETAILS, m_DblClickCacheDetails);
	DDX_Check(pDX, IDC_RELOAD_LAST_GPX, m_ReloadLastGpxFile);
	DDX_Text(pDX, IDC_SPOILER_PICS_LOCATION, m_SpoilerPicsPath);
	DDV_MaxChars(pDX, m_SpoilerPicsPath, 128);
	DDX_Check(pDX, IDC_NO_IMG_TAGS, m_StripImgTags);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMiscPreferences, CNonFSDialog)
	//{{AFX_MSG_MAP(CMiscPreferences)
	ON_BN_CLICKED(IDC_SPOILER_BROWSE, OnSpoilerBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMiscPreferences::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMiscPreferences::OnCancel()
{
	UpdateData(true);

	CNonFSDialog::OnCancel();
}

void CMiscPreferences::OnSpoilerBrowse() 
{
	CFileDialog	Dlg( 
				true,								// File Open...
				_T(".html"),						// No file extension
				m_SpoilerPicsPath,
				OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON,
				_T("Spoiler Sync HTML Files (GC*.html)|GC*.html||"), // No Filter
				0);									// No parent window

	if (Dlg.DoModal() == IDOK)
	{
		m_SpoilerPicsPath = (LPCTSTR) Dlg.GetPathName();

		int BackslashPos = m_SpoilerPicsPath.ReverseFind(_T('\\'));

		if (BackslashPos >= 0)
		{
			m_SpoilerPicsPath = m_SpoilerPicsPath.Left(BackslashPos);
		}
		else
		{
			MessageBox(_T("This is not a valid Spoiler Sync picture directory."), _T("Invalid Path"), MB_OK | MB_ICONWARNING);
			
			m_SpoilerPicsPath.Empty();
		}

		UpdateData(false);
	}
}
