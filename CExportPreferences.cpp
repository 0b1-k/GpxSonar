#include "stdafx.h"
#include "gpxsonar.h"
#include "CExportPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExportPreferences::CExportPreferences(CWnd* pParent): CNonFSDialog(CExportPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportPreferences)
	m_ExportsNamesAsGpxName = FALSE;
	m_ExportMaxLimit = 0;
	//}}AFX_DATA_INIT
}

void CExportPreferences::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportPreferences)
	DDX_Check(pDX, IDC_EXPORT_NAME, m_ExportsNamesAsGpxName);
	DDX_Text(pDX, IDC_ExportMaxLimit, m_ExportMaxLimit);
	DDV_MinMaxUInt(pDX, m_ExportMaxLimit, 1, 999);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExportPreferences, CNonFSDialog)
	//{{AFX_MSG_MAP(CExportPreferences)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CExportPreferences::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportPreferences::OnCancel() 
{
	UpdateData(true);

	if (m_ExportMaxLimit < 1)
	{
		m_ExportMaxLimit = 1;
	}

	if (m_ExportMaxLimit > 999)
	{
		m_ExportMaxLimit = 999;
	}

	CNonFSDialog::OnCancel();
}
