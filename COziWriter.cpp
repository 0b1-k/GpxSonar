#include "Literals.h"
#include "COziWriter.h"
#include "CGpxParser.h"
#include <stdio.h>

COziWriter::COziWriter()
{
}

COziWriter::~COziWriter()
{
}

void COziWriter::Open(const TCHAR* pFilename)
{
	CGPSWriter::Open(pFilename);

    static char *Header = 
        "OziExplorer Waypoint File Version 1.1\r\n"
        "WGS 84\r\n" 
        "Reserved 2\r\n" 
        "Reserved 3\r\n";

	fprintf(m_fd, Header);

	m_Count = 0;
}

// Called for each cache to write out the corresponding waypoint to be used by the GPS
void COziWriter::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

    double AltInFeet = -1.0;
    double Time = 37547.29167;

    m_Count++;

	char Description[MAX_TMP_BUFFER];

	_snprintf(
		Description, MAX_TMP_BUFFER - 1,
		"%s/%s %s", 
		RatingToString(Cache.m_GsCacheDifficulty).c_str(), 
		RatingToString(Cache.m_GsCacheTerrain).c_str(),
		w2a((TCHAR*)Cache.m_GsCacheName.c_str()));

    fprintf(m_fd,
            "%d,%s,%.6f,%.6f,%.5f,%d,%d,%d,%d,%d,%s,%d,%d,%d,%.0f,%d,%d,%d\r\n",
            m_Count, 
			w2a((TCHAR*)Cache.m_Shortname.c_str()),
			Cache.m_Lat,
			Cache.m_Long, 
			Time, 
			0, 1, 3, 0, 65535, 
			Description, 
			0, 0, 0, 
			AltInFeet, 
			6, 0, 17);
}
