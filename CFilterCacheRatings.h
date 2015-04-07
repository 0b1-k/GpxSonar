#ifndef _INC_CFilterCacheRatings
	#define _INC_CFilterCacheRatings

#include "CommonDefs.h"
#include "CFilterMgr.h"

typedef enum {
	OperGreaterEqual = 0,
	OperEqual,
	OperLessEqual
	} OperType;

class CFilterCacheRatings : public CFilterBase
{
public:
	OperType	m_TerrOper;
	double		m_TerrLvl;
	OperType	m_DiffOper;
	double		m_DiffLvl;
	bool		m_DiffEnabled;
	bool		m_TerrEnabled;

public:
	CFilterCacheRatings(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterCacheRatings();

	virtual bool OnFilterCache(CGeoCache* pCache);
	virtual void Serialize(CStream& ar);
};

#endif