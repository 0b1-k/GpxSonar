#ifndef _INC_CFilterCacheTB
	#define _INC_CFilterCacheTB

#include "CommonDefs.h"
#include "CFilterMgr.h"

class CFilterCacheTB : public CFilterBase
{
public:
	CFilterCacheTB(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterCacheTB();

	virtual bool OnFilterCache(CGeoCache* pCache);
	virtual void Serialize(CStream& ar);
};

#endif