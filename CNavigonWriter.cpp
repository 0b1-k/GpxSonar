#include "Literals.h"
#include "CNavigonWriter.h"
#include "CGpxParser.h"
#include <stdio.h>

CNavigonWriter::CNavigonWriter()
{
}

CNavigonWriter::~CNavigonWriter()
{
}

// Called for each cache to write out the corresponding waypoint to be used by the GPS
void CNavigonWriter::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

	#define MAX_DESCRIPTION_BUFFER	128

	char Description[MAX_DESCRIPTION_BUFFER];

	_snprintf(
		Description, MAX_DESCRIPTION_BUFFER - 1,
		"%s/%s %s", 
		RatingToString(Cache.m_GsCacheDifficulty).c_str(), 
		RatingToString(Cache.m_GsCacheTerrain).c_str(),
		w2a((TCHAR*)Cache.m_GsCacheName.c_str()));

	fprintf(
		m_fd, 
		"-|-|-|-|%s|%s|%s|%s|%s|-|-|%.5f|%.5f|-|-|\r\n",
		"-", // zipc
		w2a((TCHAR*)Cache.m_Shortname.c_str()), // city
		"-", // zipc
		Description, // street, 
		"-", //number,
		Cache.m_Long, 
		Cache.m_Lat);
}
