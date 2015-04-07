#ifndef _INC_CTBActionMgr
	#define _INC_CTBActionMgr

#include "CommonDefs.h"

#include <string>

using namespace std;

typedef enum {
	TbActionGrab = 0,
	TbActionDrop
} TbAction;

typedef enum {
	TbFromCache = 0,
	TbFromInventory
} TbSource;

class CTB;

// Class used to record an action on a travel bug
class CTBAction
{
public:
	CTB*		m_pTB;
	TbAction	m_Action;
	TbSource	m_OriginalSource;

public:
	CTBAction(CTB* pTB, TbAction Action, TbSource Source)
	{
		m_pTB = pTB;
		m_Action = Action;
		m_OriginalSource = Source;
	}
};

typedef list<CTBAction*> TBActionCont;
typedef list<CTBAction*>::iterator itTBAction;

// Manage transitions between TB actions
class CTBActionMgr
{
private:
	TBActionCont	m_Actions;

public:
	CTBActionMgr();
	~CTBActionMgr();

	// Record an action on a TB
	void		RecordAction(CTB* pTB, TbAction Action, TbSource Source);

	// Returns 'true' if the action log is empty
	bool		IsEmpty();

	// Generate a text log of the actions and reset all actions.
	String		ActionLog();

protected:
	// Locate a TB and return an iterator to the action object or end()
	itTBAction	Find(CTB* pTB);

	void		Reset();
};

#endif
