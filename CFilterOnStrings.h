#ifndef _INC_CFilterOnStrings
	#define _INC_CFilterOnStrings

#include "CommonDefs.h"
#include "CFilterMgr.h"

using namespace std;

class CFilteredString
{
public:
	String	m_Str;
	bool	m_Enabled;

public:
	CFilteredString();

	void	Serialize(CStream& ar);
};

typedef vector<CFilteredString*> FiltStrCont;
typedef vector<CFilteredString*>::iterator itFiltStr;

class CFilterOnStrings : public CFilterBase
{
public:
	FiltStrCont	m_Strs;

public:
	CFilterOnStrings(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterOnStrings();

	// Update the internal string list from a new one
	void	Update(StringList& StrList);

	// Delete a string object according to its pointer
	void	Delete(CFilteredString* pFS);

	virtual bool OnFilterCache(CGeoCache* pCache);

	virtual void Serialize(CStream& ar);

protected:
	void	Reset();
};

#endif