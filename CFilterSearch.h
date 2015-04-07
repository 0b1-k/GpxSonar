#ifndef _INC_CFilterSearch
	#define _INC_CFilterSearch

#include "CommonDefs.h"
#include "CFilterMgr.h"

using namespace std;

typedef enum {
	ExactMatch = 0,
	PartialMatch
} StringMatchType;


class CFilterSearch : public CFilterBase
{
public:
	StringMatchType	m_Match;
	String			m_Text;

public:
	CFilterSearch(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterSearch();

	virtual bool	OnFilterCache(CGeoCache* pCache);

	virtual void	Serialize(CStream& ar);

protected:
	// Attempt to find a substring in another after making lowercase conversions. Returns 'true' if the substring is found.
	bool	FindSubstr(TCHAR* pBuffer, long BuffSize, const TCHAR* pSubstr, String& Text);

};

#endif