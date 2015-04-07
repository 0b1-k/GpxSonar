#ifndef _INC_CFilterCacheContainers
	#define _INC_CFilterCacheContainers

#include "CommonDefs.h"
#include "CFilterMgr.h"
#include "CGpxParser.h"
#include <vector>

using namespace std;

class CFiltContainerTypes
{
public:
	GcContainer	m_Container;
	bool		m_Enabled;

public:
	CFiltContainerTypes();
	CFiltContainerTypes(GcContainer Cont, bool Enabled);

	void Serialize(CStream& ar);
};

typedef vector<CFiltContainerTypes*>			FiltCacheContainerCont;
typedef vector<CFiltContainerTypes*>::iterator	itFiltCacheContainer;

class CFilterCacheContainers : public CFilterBase
{
public:
	FiltCacheContainerCont	m_Conts;

public:
	CFilterCacheContainers(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterCacheContainers();

	virtual bool OnFilterCache(CGeoCache* pCache);
	virtual void Serialize(CStream& ar);

protected:
	void	Reset();
};

#endif