#include "stdafx.h"
#include "CMenuEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MES_UNDO        _T("&Undo")
#define MES_CUT         _T("Cu&t")
#define MES_COPY        _T("&Copy")
#define MES_PASTE       _T("&Paste")
#define MES_DELETE      _T("&Delete")
#define MES_SELECTALL   _T("Select &All")
#define ME_SELECTALL    WM_USER + 0x7000

BEGIN_MESSAGE_MAP(CMenuEdit, CEdit)
	//{{AFX_MSG_MAP(CMenuEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMenuEdit::CMenuEdit()
{
	m_bTapAndHold = false;
}

void CMenuEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bTapAndHold = false;		

	CEdit::OnLButtonUp(nFlags, point);
}

void CMenuEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bTapAndHold = false;

    SHRGINFO shrgi = {0};

    shrgi.cbSize        = sizeof(SHRGINFO);
    shrgi.hwndClient    = m_hWnd;
    shrgi.ptDown.x      = point.x;
    shrgi.ptDown.y      = point.y;
    shrgi.dwFlags       = SHRG_RETURNCMD;

    if(GN_CONTEXTMENU == ::SHRecognizeGesture(&shrgi))
        ContextMenu(point);
    else
		CEdit::OnLButtonDown(nFlags, point);
        //Default();
}

void CMenuEdit::ContextMenu(CPoint point)
{
    SetFocus();

	// Signal this is a tap and hold operation              
	m_bTapAndHold = true;

    CMenu menu;
    menu.CreatePopupMenu();

    BOOL bReadOnly = GetStyle() & ES_READONLY;
    
	DWORD flags = CanUndo() && !bReadOnly ? 0 : MF_GRAYED;
    
	menu.InsertMenu(0, MF_BYPOSITION | flags, EM_UNDO, MES_UNDO);
    menu.InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR);

    DWORD sel = GetSel();
    flags = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_COPY, MES_COPY);

    flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_CUT, MES_CUT);
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_CLEAR, MES_DELETE);

    flags = IsClipboardFormatAvailable(CF_UNICODETEXT) && !bReadOnly ? 0 : MF_GRAYED;
    
	menu.InsertMenu(4, MF_BYPOSITION | flags, WM_PASTE, MES_PASTE);
    menu.InsertMenu(6, MF_BYPOSITION | MF_SEPARATOR);

    int len = GetWindowTextLength();
    flags = (!len || (LOWORD(sel) == 0 && HIWORD(sel) == len)) ? MF_GRAYED : 0;
    menu.InsertMenu(7, MF_BYPOSITION | flags, ME_SELECTALL, MES_SELECTALL);

	ClientToScreen(&point);

    menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

BOOL CMenuEdit::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case EM_UNDO:
    case WM_CUT:
    case WM_COPY:
    case WM_CLEAR:
    case WM_PASTE:
        return SendMessage(LOWORD(wParam));
    case ME_SELECTALL:
        return SendMessage (EM_SETSEL, 0, -1);
    default:
        return CEdit::OnCommand(wParam, lParam);
    }
}
