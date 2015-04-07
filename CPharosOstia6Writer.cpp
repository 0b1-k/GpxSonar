#include "CPharosOstia6Writer.h"
#include "CGpxParser.h"
#include "math.h"
#include <stdio.h>

CPharosOstia6Writer::CPharosOstia6Writer()
{
}

CPharosOstia6Writer::~CPharosOstia6Writer()
{
}

void CPharosOstia6Writer::Open(const TCHAR* pFilename)
{
	CGPSWriter::Open(pFilename);

	fprintf(m_fd, "$$$PHAROS OSTIA WAYPOINT BOOK$$$");
}

// Called for each cache to write out the corresponding waypoint to be used by the GPS
void CPharosOstia6Writer::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

	//*GCA661 : Act Your Age by lucyandrickie, Traditional Cache (1.5/1)!35552287!-91064231

	string	CacheOwnerName = Clean(w2a((TCHAR*)Cache.m_GsCacheOwnerName.c_str()));
	
	fprintf(
		m_fd, 
		"*%s : %s by %s, %s (%.1f/%.1f)!%ld!%ld", 
		w2a((TCHAR*)Cache.m_Shortname.c_str()),
		w2a((TCHAR*)Cache.m_GsCacheName.c_str()),
		CacheOwnerName.c_str(),
		w2a((TCHAR*)Cache.m_GsCacheType.c_str()),
		Cache.m_GsCacheDifficulty,
		Cache.m_GsCacheTerrain,
		DecimalCoordToOsti6Coord(Cache.m_Lat),
		DecimalCoordToOsti6Coord(Cache.m_Long));
}

long CPharosOstia6Writer::DecimalCoordToOsti6Coord(double Value)
{
	// dividing by 180 and multiplying by 134217728
	double OstiaVal = (Value / 180) * 134217728;

	return (long) OstiaVal;
}
