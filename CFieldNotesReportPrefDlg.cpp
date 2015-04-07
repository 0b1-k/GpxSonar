#include "stdafx.h"
#include "GpxSonar.h"
#include "CCacheReportsPref.h"
#include "CFieldNotesReportPrefDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFieldNotesReportPrefDlg::CFieldNotesReportPrefDlg(CWnd* pParent) : CNonFSDialog(CFieldNotesReportPrefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFieldNotesReportPrefDlg)
	m_Archive = FALSE;
	m_Delete = FALSE;
	m_Found = FALSE;
	m_Notes = FALSE;
	m_NotFound = FALSE;
	//}}AFX_DATA_INIT

	m_ReportsPref = 0;
}

void CFieldNotesReportPrefDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFieldNotesReportPrefDlg)
	DDX_Check(pDX, IDC_ARCHIVE, m_Archive);
	DDX_Check(pDX, IDC_DELETE, m_Delete);
	DDX_Check(pDX, IDC_FOUNDIT, m_Found);
	DDX_Check(pDX, IDC_NOTES, m_Notes);
	DDX_Check(pDX, IDC_NOTFOUND, m_NotFound);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFieldNotesReportPrefDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFieldNotesReportPrefDlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFieldNotesReportPrefDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	m_Archive = (BOOL) m_ReportsPref->m_NoteStatusArchiveIt;
	m_NotFound = (BOOL) m_ReportsPref->m_NoteStatusDNF;
	m_Found = (BOOL) m_ReportsPref->m_NoteStatusFoundIt;
	m_Notes = (BOOL) m_ReportsPref->m_NoteStatusNote;
	m_Delete = (BOOL) m_ReportsPref->m_Reset;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFieldNotesReportPrefDlg::OnOK() 
{
	UpdateData(true);
	
	m_ReportsPref->m_NoteStatusArchiveIt = (bool) m_Archive;
	m_ReportsPref->m_NoteStatusDNF = (bool) m_NotFound;
	m_ReportsPref->m_NoteStatusFoundIt = (bool) m_Found;
	m_ReportsPref->m_NoteStatusNote = (bool) m_Notes;
	m_ReportsPref->m_Reset = (bool) m_Delete;

	CNonFSDialog::OnOK();
}

void CFieldNotesReportPrefDlg::OnDelete() 
{
	UpdateData(true);

	if (m_Delete)
	{
		int Answer = MessageBox(
			_T("By checking this option, the field notes will be permanently deleted after the report is done. Is that what you want to do?"), 
			_T("Field Notes Deletion"), 
			MB_OKCANCEL | MB_ICONWARNING);

		if (Answer == IDCANCEL)
		{
			m_Delete = false;

			UpdateData(false);
		}
	}
}
