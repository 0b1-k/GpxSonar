#include "stdafx.h"
#include "GpxSonar.h"
#include "CCacheDetailsFieldNotes.h"
#include "CFieldNoteMgr.h"
#include "CTBExchangeDlg.h"
#include "CTBActionMgr.h"
#include "CCenterCoordsDlg.h"
#include "CMd5.h"
#include "CTimeHelper.h"
#include "CPath.h"
#include "Literals.h"
#include "voicectl.h"

#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCacheDetailsFieldNotes::CCacheDetailsFieldNotes(CWnd* pParent /*=NULL*/) : CNonFSDialog(CCacheDetailsFieldNotes::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCacheDetailsFieldNotes)
	m_Notes = _T("");
	//}}AFX_DATA_INIT
	
	m_RecStatus = VRN_OK;
	m_Status = NoteStatusFoundIt;
}

CCacheDetailsFieldNotes::~CCacheDetailsFieldNotes()
{
}

void CCacheDetailsFieldNotes::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCacheDetailsFieldNotes)
	DDX_Control(pDX, IDC_NOTES, m_NotesCtrl);
	DDX_Control(pDX, IDC_STATUS, m_LogStatus);
	DDX_Text(pDX, IDC_NOTES, m_Notes);
	DDV_MaxChars(pDX, m_Notes, 1024);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCacheDetailsFieldNotes, CNonFSDialog)
	//{{AFX_MSG_MAP(CCacheDetailsFieldNotes)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_TB, OnTb)
	ON_BN_CLICKED(IDC_COORDS, OnCoords)
	ON_BN_CLICKED(IDC_TIMESTAMP, OnTimestamp)
	ON_BN_CLICKED(IDC_RECORD, OnRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCacheDetailsFieldNotes::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	CString Title;

	GetWindowText(Title);

	Title += _T(" : ") + m_Shortname;

	SetWindowText(Title);

	m_LogStatus.SetCurSel(m_Status);

	UpdateData(false);

	CWnd* pWnd = GetDlgItem(IDC_NOTES);

	pWnd->SetFocus();

	m_ModHash = HashValues();

	if (m_Notes.IsEmpty())
	{
		OnTimestamp();
	}

	::SHSipPreference(m_hWnd, SIP_UP);

	return false;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

String CCacheDetailsFieldNotes::HashValues()
{
	CMd5	Hash;
	
	Hash.Init();

	Hash.Update((BYTE*) (LPCTSTR) m_Notes, m_Notes.GetLength());
	Hash.Update((BYTE*) &m_Status, sizeof(m_Status));
	Hash.Update((BYTE*) &m_Coords.m_Latitude, sizeof(m_Coords.m_Latitude));
	Hash.Update((BYTE*) &m_Coords.m_Longitude, sizeof(m_Coords.m_Longitude));

	return Hash.Final();
}

void CCacheDetailsFieldNotes::OnCancel()
{
	UpdateData(true);

	m_Status = m_LogStatus.GetCurSel();

	if (HashValues() != m_ModHash)
	{
		m_Action = NOTE_ACTION_SAVE;

		::SHSipPreference(m_hWnd, SIP_DOWN);

		CNonFSDialog::OnOK();
	}
	else
	{
		::SHSipPreference(m_hWnd, SIP_DOWN);

		CNonFSDialog::OnCancel();
	}
}

void CCacheDetailsFieldNotes::OnDelete() 
{
	UpdateData(true);

	m_Action = NOTE_ACTION_DELETE;

	if (MessageBox(_T("Permanently delete this note?"), _T("Confim Deletion"), MB_OKCANCEL | MB_ICONWARNING) == IDOK)
	{
		::SHSipPreference(m_hWnd, SIP_DOWN);

		CNonFSDialog::OnOK();
	}
}

void CCacheDetailsFieldNotes::OnTb() 
{
	UpdateData(true);

	CTBExchangeDlg	Dlg;

	Dlg.DoModal();

	if (!Dlg.m_TBActionMgr.IsEmpty())
	{
		m_Notes += Dlg.m_TBActionMgr.ActionLog().c_str();

		UpdateData(false);
	}
}

void CCacheDetailsFieldNotes::OnCoords() 
{
	CCenterCoordsDlg	Dlg;

	Dlg.SetTitle(_T("Actual Coordinates"));

	m_Coords.GetDegMinMmm(
					Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
					Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);

	if (Dlg.DoModal() == IDOK)
	{
		m_Coords.SetDegMinMmm(
				Dlg.m_LatDeg, Dlg.m_LatMinMmm, Dlg.m_LatChar,
				Dlg.m_LongDeg, Dlg.m_LongMinMmm, Dlg.m_LongChar);
	}
}

void CCacheDetailsFieldNotes::OnTimestamp() 
{
	UpdateData(true);

	SYSTEMTIME	ST;

	GetLocalTime(&ST);

	CTimeHelper	TH(ST);

	CString TS;

	TS = _T("---(");
	TS += (TCHAR*) TH.DateTime().c_str();
	TS += _T(")---\r\n");
	TS += m_Notes;

	m_Notes = TS;

	UpdateData(false);
}

void CCacheDetailsFieldNotes::OnRecord() 
{
	CPath	Path;
	String	RecPath;

	RecPath = GPXSONAR_FIELD_NOTES_DIR;
	RecPath += m_Shortname;
	RecPath += _T(".wav");

	RecPath = Path.BuildPath(RecPath);

	// Initialize the control's data structure.
	CM_VOICE_RECORDER cmvr;

	memset( &cmvr, 0, sizeof(cmvr));
	
	cmvr.cb = sizeof(CM_VOICE_RECORDER);
	cmvr.dwStyle = VRS_NO_MOVE | VRS_MODAL;
	cmvr.xPos = -1; // Use -1 to center the control relative to owner.
	cmvr.yPos = -1;
	cmvr.hwndParent = CCacheDetailsFieldNotes::m_hWndOwner;
	cmvr.lpszRecordFileName = (LPTSTR) RecPath.c_str();

	// Create the recorder control in modal style
	VoiceRecorder_Create(&cmvr);

	// m_RecStatus is set through the NOTIFY msgs sent by the control
	switch (m_RecStatus)
	{
        case VRN_ERROR:
			MessageBox(_T("The Voice Recorder could not be created.\r\nPlease close some applications and try again."), _T("Initialization Failure"), MB_OK | MB_ICONERROR);
            break;
        case VRN_CANCEL:
			// Delete the recording
			DeleteFile(RecPath.c_str());
            break;
	}
}

BOOL CCacheDetailsFieldNotes::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
    LPNMHDR pnmh = (LPNMHDR) lParam;

    switch (pnmh->code)
    {
        case VRN_ERROR:
			m_RecStatus = VRN_ERROR;
            break;
        case VRN_CANCEL:
			m_RecStatus = VRN_CANCEL;
            break;
        case VRN_OK:
			m_RecStatus = VRN_OK;
            break;
    }
	
	return CNonFSDialog::OnNotify(wParam, lParam, pResult);
}
