#ifndef _INC_CFilterCacheLists
	#define _INC_CFilterCacheLists

#include "CommonDefs.h"
#include "CFilterMgr.h"

using namespace std;

typedef vector<long> CacheIdCont;
typedef vector<long>::iterator itCacheId;

typedef enum {
	ShowFoundCachesOnly = 0,
	ShowCachesWithNotesOnly
} GcExclusiveShowOpts;

class CFilterCacheLists : public CFilterBase
{
public:
	bool		m_ShowIgnoredCaches;
	bool		m_HideFoundCaches;
	bool		m_HideDisabledCaches;
	bool		m_UseShowOnly;
	int			m_ExclusiveOptions;

	// List of cache IDs to be ignored
	CacheIdCont	m_CacheIds;

public:
	CFilterCacheLists(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterCacheLists();

	void			Ignore(CGeoCache* pCache);
	void			UnIgnore(CGeoCache* pCache);

	bool			Find(long Id);

	virtual bool	OnFilterCache(CGeoCache* pCache);

	virtual void	Serialize(CStream& ar);
};

#endif