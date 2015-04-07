#include "stdafx.h"
#include "gpxsonar.h"
#include "CExportLocationDlg.h"
#include "CExportLocationMgr.h"
#include "CExportPreferences.h"
#include "CGpxParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExportLocationDlg::CExportLocationDlg(CWnd* pParent): CNonFSDialog(CExportLocationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportLocationDlg)
	m_Location = _T("");
	//}}AFX_DATA_INIT

	m_pExpLocMgr = 0;
}

void CExportLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportLocationDlg)
	DDX_Control(pDX, IDC_EXPORT_TYPES, m_ExportTypesList);
	DDX_Text(pDX, IDC_LOCATION, m_Location);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExportLocationDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CExportLocationDlg)
	ON_BN_CLICKED(IDC_SET_LOC, OnSetLoc)
	ON_LBN_SELCHANGE(IDC_EXPORT_TYPES, OnSelchangeExportTypes)
	ON_BN_CLICKED(IDC_EXPORT_OPTIONS, OnExportOptions)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CExportLocationDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	// Fill the list of export location types
	for (itExportLoc L = m_pExpLocMgr->m_ExpLocs.begin(); L != m_pExpLocMgr->m_ExpLocs.end(); L++)
	{
		int Item = m_ExportTypesList.AddString((*L)->m_Name.c_str());
		m_ExportTypesList.SetItemDataPtr(Item, (*L));
	}
	
	m_ExportTypesList.SetCurSel(0);

	OnSelchangeExportTypes();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportLocationDlg::OnSetLoc() 
{
	int Item = m_ExportTypesList.GetCurSel();

	if (Item != LB_ERR)
	{
		CExportLoc* pEL = (CExportLoc*) m_ExportTypesList.GetItemDataPtr(Item);

		CFileDialog	Dlg( 
					false,		// File Save As...
					NULL,		// No file extension
					pEL->m_FullPath.c_str(),
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST,
					0,			// No Filter
					0);			// No parent window

		if (Dlg.DoModal() == IDOK)
		{
			pEL->m_FullPath = (LPCTSTR) Dlg.GetPathName();

			m_Location = (LPCTSTR) pEL->m_FullPath.c_str();

			UpdateData(false);
		}
	}
}

void CExportLocationDlg::OnSelchangeExportTypes() 
{
	int Item = m_ExportTypesList.GetCurSel();

	if (Item != LB_ERR)
	{
		CExportLoc* pEL = (CExportLoc*) m_ExportTypesList.GetItemDataPtr(Item);

		m_Location = (LPCTSTR) pEL->m_FullPath.c_str();

		UpdateData(false);
	}	
}

void CExportLocationDlg::OnExportOptions() 
{
	CExportPreferences Dlg;

	Dlg.m_ExportsNamesAsGpxName = (BOOL) m_pExpLocMgr->m_ExportsNamesAsGpxName;
	Dlg.m_ExportMaxLimit = m_pExpLocMgr->m_ExportMaxLimit;

	Dlg.DoModal();

	m_pExpLocMgr->m_ExportsNamesAsGpxName = (bool) Dlg.m_ExportsNamesAsGpxName;
	m_pExpLocMgr->m_ExportMaxLimit = Dlg.m_ExportMaxLimit;
}

void CExportLocationDlg::OnExport() 
{
	int Item = m_ExportTypesList.GetCurSel();

	if (Item != LB_ERR)
	{
		CExportLoc* pEL = (CExportLoc*) m_ExportTypesList.GetItemDataPtr(Item);

		if (pEL)
		{
			try
			{
				m_pExpLocMgr->Export(pEL);

				String	File;

				File = m_pExpLocMgr->GetFilename((TCHAR*)pEL->m_Name.c_str());

				CString Msg;

				if (m_pExpLocMgr->GetWasExportLimited())
				{
					Msg.Format(_T("Successfully wrote %ld out of %ld waypoints to %s file due to the export limit you have set or to a GPS limitation."), m_pExpLocMgr->GetExportedCount() - 1, m_pExpLocMgr->GetGpxParser()->CacheCount(), File.c_str());
				}
				else
				{
					Msg.Format(_T("Successfully wrote %ld waypoints to %s"), m_pExpLocMgr->GetExportedCount(), File.c_str());
				}

				MessageBox(Msg, _T("Export Complete"), MB_OK | MB_ICONINFORMATION);
			}
			catch(...)
			{
				MessageBox(_T("An exception occured while exporting"), _T("Exception!"), MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			MessageBox(_T("Invalid export location!"), _T("NULL Pointer"), MB_OK | MB_ICONINFORMATION);
		}
	}	
}
