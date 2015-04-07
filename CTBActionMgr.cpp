#include "CTBActionMgr.h"
#include "CTBMgr.h"


// Manage transitions between TB actions
CTBActionMgr::CTBActionMgr()
{
}

CTBActionMgr::~CTBActionMgr()
{
	Reset();
}

// Record an action on a TB
void CTBActionMgr::RecordAction(CTB* pTB, TbAction Action, TbSource	Source)
{
	// Attempt to locate a TB with an action associated to it
	itTBAction I = Find(pTB);

	if (I != m_Actions.end())
	{
		CTBAction* pAction = (*I);

		// Found the TB! Figure out if the action is a wash (in which case the action object gets deleted)
		if (Action == TbActionDrop)
		{
			// If dropping (from the inventory) when the source was the cache, it means that the user has put the TB back in the cache: a wash!
			if (pAction->m_OriginalSource == TbFromCache)
			{
				// The user actually put the TB back. Forget the action by deleting it since it's like nothing ever happened.
				delete (*I);
				m_Actions.erase(I);
			}
			else
			{
				// We should never get here!
				//assert(0);
			}
		}
		// Action == TbActionGrab
		else
		{
			// If grabbing (from the cache) when the source was the inventory, it means that the user has taken the TB back into the inventory: a wash!
			if (pAction->m_OriginalSource == TbFromInventory)
			{
				// The user actually took the TB back. Forget the action by deleting it since it's like nothing ever happened.
				delete (*I);
				m_Actions.erase(I);
			}
			else
			{
				// We should never get here!
				//assert(0);
			}
		}
	}
	else
	{
		// First time an action is recorded for this TB...
		CTBAction* pAction = new CTBAction(pTB, Action, Source);

		if (!pAction)
		{
			//assert(0);
			return;
		}

		m_Actions.push_back(pAction);
	}
}

// Locate a TB in the list of existing actions
itTBAction CTBActionMgr::Find(CTB* pTB)
{
	for (itTBAction I = m_Actions.begin(); I != m_Actions.end(); I++)
	{
		if ((*I)->m_pTB == pTB)
		{
			return I;
		}
	}

	return I;
}

// Returns 'true' if the action log is empty
bool CTBActionMgr::IsEmpty()
{
	return m_Actions.empty();
}

// Generate a text log of the actions and reset all actions.
String CTBActionMgr::ActionLog()
{
	#define DEFAULT_LOG_SIZE	256

	String Log;

	Log.reserve(DEFAULT_LOG_SIZE);

	for (itTBAction I = m_Actions.begin(); I != m_Actions.end(); I++)
	{
		switch ((*I)->m_Action)
		{
		case TbActionGrab:
			Log += _T("Took \'") + (*I)->m_pTB->m_Name + _T("\' TB.\r\n");
			break;
		case TbActionDrop:
			Log += _T("Left \'") + (*I)->m_pTB->m_Name + _T("\' TB.\r\n");
			break;
		}
	}

	Reset();

	return Log;
}

void CTBActionMgr::Reset()
{
	for (itTBAction I = m_Actions.begin(); I != m_Actions.end(); I++)
	{
		delete (*I);
	}

	m_Actions.clear();
}
