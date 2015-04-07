#include "CMeridianWriter.h"
#include "CGpxParser.h"
#include "math.h"
#include <stdio.h>

#define CACHE_ICON		"a"
#define ALTITUDE_ZERO	0.0

CMeridianWriter::CMeridianWriter()
{
}

CMeridianWriter::~CMeridianWriter()
{
}

// Called for each cache to write out the corresponding waypoint to be used by the GPS
void CMeridianWriter::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

	double	lon, lat;
	double	ilon, ilat;
	int		lon_deg, lat_deg;

	char Buffer[MAX_OUTPUT_BUFFER];
	char TmpBuffer[MAX_TMP_BUFFER];

	ilat = Cache.m_Lat;
	ilon = Cache.m_Long;

	lon = fabs(ilon);
	lat = fabs(ilat);

	lon_deg = lon;
	lat_deg = lat;

	lon = (lon - lon_deg) * 60.0;
	lat = (lat - lat_deg) * 60.0;

	lon = (lon_deg * 100.0 + lon);
	lat = (lat_deg * 100.0 + lat);
	
	_snprintf(
		TmpBuffer, MAX_TMP_BUFFER - 1,
		"%s/%s %s", 
		RatingToString(Cache.m_GsCacheDifficulty).c_str(), 
		RatingToString(Cache.m_GsCacheTerrain).c_str(),
		w2a((TCHAR*)Cache.m_GsCacheName.c_str()));

	string CleanedDesc = Clean(TmpBuffer);

	_snprintf(
		Buffer, MAX_OUTPUT_BUFFER - 1,
		"$PMGNWPL,%4.3f,%c,%09.3f,%c,%07.lf,M,%-.8s,%-.30s,%s",
		lat, ilat < 0 ? 'S' : 'N',
		lon, ilon < 0 ? 'W' : 'E',
		ALTITUDE_ZERO,
		w2a((TCHAR*)Cache.m_Shortname.c_str()),
		CleanedDesc.c_str(),
		CACHE_ICON);

	unsigned int ChkSum = Checksum(Buffer);

	_snprintf(TmpBuffer, MAX_TMP_BUFFER - 1, "*%02X\r\n", ChkSum);

	strncat(Buffer, TmpBuffer, 5);

	fwrite(Buffer, 1, strlen(Buffer), m_fd);
}

unsigned int CMeridianWriter::Checksum(const char* pStr)
{
	int csum = 0;
	
	for (const char* p = pStr; *p; p++) 
	{
		csum  ^= *p;
	}
	
	return csum;
}
