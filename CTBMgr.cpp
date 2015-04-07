#include "CTBMgr.h"

CTB::CTB()
{
}

CTB::CTB(const String& Serial, const String& Name)
{
	m_Name = Name;
	m_Serial = Serial;
}

// Assign a TB to a particular cache (using the waypoint name) or if left blank, the TB is
// implicitly assigned to the TB inventory
void CTB::AssignToCache(const String& CacheShortName)
{
	m_CacheShortName = CacheShortName;
}

void CTB::Serialize(CStream& ar)
{
	#define CTBVersion 100

	if (ar.IsStoring())
	{
		ar << CTBVersion;
		ar << m_CacheShortName;
		ar << m_Serial;

		// Base class members
		ar << m_Id;
		ar << m_Ref;
		ar << m_Name;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_CacheShortName;
			ar >> m_Serial;

			// Base class members
			ar >> m_Id;
			ar >> m_Ref;
			ar >> m_Name;
		}
	}
}

//----------------------------------------------------------------------------------------------------

CTBMgr::CTBMgr()
{
}

CTBMgr::~CTBMgr()
{
	Reset();
}

// Add a new TB to the list
CTB* CTBMgr::Add(const String& Serial, const String& Name)
{
	for (itTB2 I = m_TBs.begin(); I != m_TBs.end(); I++)
	{
		if ((*I)->m_Serial == Serial && (*I)->m_Name == Name)
		{
			return (*I);
		}
	}

	CTB* pTB = new CTB(Serial, Name);

	if (!pTB)
	{
		//assert(0);
		return 0;
	}

	pTB->m_CacheShortName = TB_INVENTORY;

	m_TBs.push_back(pTB);

	return pTB;
}

// Remove a TB from the list
void CTBMgr::Delete(CTB* pTB)
{
	for (itTB2 I = m_TBs.begin(); I != m_TBs.end(); I++)
	{
		if ((*I) == pTB)
		{
			delete (*I);
			m_TBs.erase(I);
			return;
		}
	}
}

// Find a TB by its reference #
CTB* CTBMgr::Find(const String& Ref)
{
	for (itTB2 I = m_TBs.begin(); I != m_TBs.end(); I++)
	{
		if ((*I)->m_Ref == Ref)
		{
			return (*I);
		}
	}

	return 0;
}

// Find a TB by its reference # or its serial #
CTB* CTBMgr::FindEither(const String& Ref, const String& Serial)
{
	for (itTB2 I = m_TBs.begin(); I != m_TBs.end(); I++)
	{
		if (((*I)->m_Ref == Ref) || ((*I)->m_Serial == Serial))
		{
			return (*I);
		}
	}

	return 0;
}

// TB List iterators
CTB* CTBMgr::First(itTB2& it)
{
	it = m_TBs.begin();

	if (it != m_TBs.end())
	{
		return (*it);
	}

	return 0;
}

CTB* CTBMgr::Next(itTB2& it)
{
	it++;

	if (it != m_TBs.end())
	{
		return (*it);
	}

	return 0;
}

// Returns true at the end of the list
bool CTBMgr::EndOfList(itTB2& it)
{
	if (it != m_TBs.end())
	{
		return false;
	}

	return true;
}

void CTBMgr::Serialize(CStream& ar)
{
	#define CTBMgrVersion	100

	if (ar.IsStoring())
	{
		ar << CTBMgrVersion;

		ar << m_TBs.size();

		for (itTB2 I = m_TBs.begin(); I != m_TBs.end(); I++)
		{
			(*I)->Serialize(ar);
		}
	}
	else
	{
		Reset();

		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			long Count;

			ar >> Count;

			while (Count--)
			{
				CTB* pTB = new CTB;

				if (!pTB)
				{
					//assert(0);
				}

				pTB->Serialize(ar);

				m_TBs.push_back(pTB);
			}
		}
	}
}

void CTBMgr::Reset()
{
	for (itTB2 I = m_TBs.begin(); I != m_TBs.end(); I++)
	{
		delete (*I);
	}

	m_TBs.clear();
}
