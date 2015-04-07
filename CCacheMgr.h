#ifndef _INC_CCacheMgr
	#define _INC_CCacheMgr

#include "CommonDefs.h"
#include "CGpxParser.h"
#include "CStream.h"

class CWaypoint
{
public:
	double		m_Lat;
	double		m_Long;
	String		m_Name;
	String		m_Desc;

public:
	CWaypoint();

	void	Serialize(CStream& ar);
};

typedef list<CWaypoint*> Waypoints;
typedef list<CWaypoint*>::iterator itWP;

class CWPMgr
{
public:
	Waypoints	m_Wps;

public:
	CWPMgr();
	~CWPMgr();

	void	Serialize(CStream& ar);

	void	Reset();
};

class CCacheMgr
{
public:
	GCCont		m_Caches;
	StringList	m_CacheCategories;

public:
	CCacheMgr();
	~CCacheMgr();

	bool		IsAlreadyDefined(CGeoCache* pGc);
	CGeoCache*	New(String& Owner);
	void		Delete(CGeoCache* pGc);
	void		Copy(CGpxParser* pParser, CGeoCache* pGcDest, CGeoCache* pGcSrc);

	void		Serialize(CStream& ar);

	void		SaveConfig();
	void		LoadConfig();

	void		Reset();

	bool		AddCacheCategory(String& Cat);
	bool		DelCacheCategory(String& Cat);

private:
	void		DestroyObjs(CGeoCache* pGc);
};

#endif
