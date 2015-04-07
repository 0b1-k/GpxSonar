#ifndef _INC_CTBMgr
	#define _INC_CTBMgr

#include "CommonDefs.h"
#include "CGpxParser.h"
#include "CStream.h"

#define TB_INVENTORY	_T("Inventory")

class CTB : public CTravelBug
{
public:
	String	m_CacheShortName;
	String	m_Serial;

public:
	CTB();
	CTB(const String& Serial, const String& Name);

	// Assign a TB to a particular cache (using the waypoint name) or if left blank, the TB is
	// implicitly assigned to the TB inventory
	void AssignToCache(const String& CacheShortName);

	void Serialize(CStream& ar);
};

typedef list<CTB*> TB2Cont;
typedef list<CTB*>::iterator itTB2;

class CTBMgr
{
private:
	TB2Cont		m_TBs;

public:
	CTBMgr();
	~CTBMgr();

	// Add a new TB to the list
	CTB*	Add(const String& Serial, const String& Name);

	// Find a TB by its reference #
	CTB*	Find(const String& Ref);

	// Find a TB by its reference # or by its serial #
	CTB*	FindEither(const String& Ref, const String& Serial);

	// Remove a TB from the list
	void	Delete(CTB* pTB);

	// TB List iterators
	CTB*	First(itTB2& it);
	CTB*	Next(itTB2& it);

	// Returns true at the end of the list
	bool	EndOfList(itTB2& it);

	void	Serialize(CStream& ar);

protected:
	void	Reset();
};

#endif
