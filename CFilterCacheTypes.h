#ifndef _INC_CFilterCacheTypes
	#define _INC_CFilterCacheTypes

#include "CommonDefs.h"
#include "CFilterMgr.h"
#include "CGpxParser.h"
#include <vector>

using namespace std;

class CFiltCacheTypes
{
public:
	GcType	m_Type;
	bool	m_Enabled;

public:
	CFiltCacheTypes();
	CFiltCacheTypes(GcType Type, bool Enabled);

	void Serialize(CStream& ar);
};

typedef vector<CFiltCacheTypes*>			FiltCacheTypesCont;
typedef vector<CFiltCacheTypes*>::iterator	itFiltCacheTypes;

class CFilterCacheTypes : public CFilterBase
{
public:
	FiltCacheTypesCont	m_Types;

public:
	CFilterCacheTypes(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterCacheTypes();

	virtual bool OnFilterCache(CGeoCache* pCache);
	virtual void Serialize(CStream& ar);

protected:
	void	Reset();
	void	InitDefaultFilters();
};

#endif