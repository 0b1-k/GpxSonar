#ifndef _INC_CMenuEdit
	#define _INC_CMenuEdit 

#if _MSC_VER > 1000
#pragma once
#endif 

class CMenuEdit : public CEdit
{
public:
    CMenuEdit();

protected:
  	//{{AFX_MSG_MAP(CMenuEdit)
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG_MAP
    
	DECLARE_MESSAGE_MAP()

	void	ContextMenu(CPoint point);

	bool	m_bTapAndHold;
};

#endif
