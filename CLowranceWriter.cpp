#include "Literals.h"
#include "CLowranceWriter.h"
#include "CGpxParser.h"
#include "math.h"
#include <stdio.h>


#define MAXUSRSTRINGSIZE	256
#define SEMIMINOR			6356752.3142
#define DEGREESTORADIANS	0.017453292
#define SECSTO2000			946713600

#define DEF_ICON 10001

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

typedef struct lowranceusr_icon_mapping {
		int value; 
		TCHAR		*icon;
} lowranceusr_icon_mapping_t;

const lowranceusr_icon_mapping_t lowranceusr_icon_value_table[] = {
/* Taken from iFinder 1.8

	{ 10000, "diamond 1" },
	{ 10001, "diamond 2" },
	{ 10002, "diamond 3" },
	{ 10003, "x 1" },
	{ 10004, "x 2" },
	{ 10005, "x 3" },
	{ 10006, "cross" },
	{ 10007, "house" },
	{ 10008, "car" },
	{ 10009, "store" },
	{ 10010, "gas station" },
	{ 10011, "fork and spoon" },
	{ 10012, "telephone" },
	{ 10013, "airplane" },
	{ 10014, "exit sign" },
	{ 10015, "stop sign" },
	{ 10016, "exclamation" },
	{ 10017, "traffic light" },
	{ 10018, "american flag" },
	{ 10019, "person" },
	{ 10020, "restrooms" },
	{ 10021, "tree" },
	{ 10022, "mountains" },
	{ 10023, "campsite" },
	{ 10024, "picnic table" },
	{ 10025, "deer" },
	{ 10026, "deer tracks" },
	{ 10027, "turkey tracks" },
	{ 10028, "tree stand" },
	{ 10029, "bridge" },
	{ 10030, "skull and crossbones" },
	{ 10031, "fish" },
	{ 10032, "two fish" },
	{ 10033, "dive flag" },
	{ 10034, "wreck" },
	{ 10035, "anchor" },
	{ 10036, "boat" },
	{ 10037, "boat ramp" },
	{ 10038, "flag buoy" },
	{ 10039, "dam" },
	{ 10040, "swimmer" },
	{ 10041, "pier"},

	{ 10038, "Micro-Cache" },   	// icon for "flag buoy" 
	{ 10030, "Virtual cache" }, 	// icon for "skull and crossbones" 
	{ 10032, "Multi-Cache" },   	// icon for "two fish" 
	{ 10003, "Unknown Cache" },   	// icon for "x 1"
	{ 10018, "Locationless (Reverse) Cache" }, // Icon for "american flag"
	{ 10007, "Post Office" },  	// Icon for "house"
	{ 10019, "Event Cache" }, 	// Icon for "person"
	{ 10020, "Webcam Cache" }, 	// Icon for "restrooms"
*/
	{ 10038, _T("Traditional Cache") },	// flag buoy
	{ 10032, _T("Multi-cache") },		// 2 fish
	{ 10030, _T("Virtual Cache") },		// cross and bone
	{ 10017, _T("Webcam Cache") },		// traffic light
	{ 10003, _T("Unknown Cache") },		// x 1
	{ 10007, _T("Letterbox Hybrid") },	// house
	{ 10024, _T("Event Cache") },		// picnic table
	{ 10019, _T("Project APE Cache") },	// person
	{ 10018, _T("Locationless (Reverse) Cache") }, // american flag
	{ 10024, _T("Cache In Trash Out Event") },	// picnic table 
	{ 10022, _T("Earthcache") },		// mountains

	{ -1, NULL }
};

CLowranceWriter::CLowranceWriter()
{
}

CLowranceWriter::~CLowranceWriter()
{
}

#define FEET_TO_METERS(feetsies) ((feetsies) * 0.3048)
#define METERS_TO_FEET(meetsies) ((meetsies) * 3.2808399)


size_t CLowranceWriter::fwrite4(int Val)
{
        return fwrite(&Val, 4, 1, m_fd);
}

size_t CLowranceWriter::fwrite2(short Val)
{
        return fwrite(&Val, 2, 1, m_fd);
}

/**
 * Latitude and longitude for USR coords are in the lowrance mercator meter
 * format in WGS84.  The below code converts them to degrees.
 */
double CLowranceWriter::lon_mm_to_deg(double x) 
{
	return x / (DEGREESTORADIANS * SEMIMINOR);
}

long CLowranceWriter::lon_deg_to_mm(double x) 
{
	return (long)(x * SEMIMINOR * DEGREESTORADIANS);
}

double CLowranceWriter::lat_mm_to_deg(double x)
{
	return (2 * atan(exp(x / SEMIMINOR)) - M_PI / 2) / DEGREESTORADIANS;
}

long CLowranceWriter::lat_deg_to_mm(double x)
{
	return (long)(SEMIMINOR * log(tan((x * DEGREESTORADIANS + M_PI / 2) / 2)));
}

void CLowranceWriter::Open(const TCHAR* pFilename)
{
	CGPSWriter::Open(pFilename);

	fwrite2(2);			// MajorVersion
	fwrite2(0);			// MinorVersion
	fwrite2(m_Total);	// Waypoint count

	m_Count = 0;
}

void CLowranceWriter::Close()
{
	// 0 routes
	fwrite2(0);

	// 0 icons
	fwrite2(0);

	// 0 tracks
	fwrite2(0);

	CGPSWriter::Close();
}

// Called for each cache to write out the corresponding waypoint to be used by the GPS
void CLowranceWriter::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

	int Lat, Lon, SymbolId;
	int alt = 0;

	Lat = lat_deg_to_mm(Cache.m_Lat);
	Lon = lon_deg_to_mm(Cache.m_Long);
	
	// Object counter
	fwrite2(m_Count);

	m_Count++;

	fwrite4(Lat);
	fwrite4(Lon);
	fwrite4(alt);

	char* pShortName =  w2a((TCHAR*)Cache.m_Shortname.c_str());

	int TextLen = strlen(pShortName);
	
	fwrite4(TextLen);
	fwrite(pShortName, 1, TextLen, m_fd);

	// Comments aren't used by the iFinder yet so they just take up space...
	fwrite4(0); // Comment

	// Don't care about the time
	fwrite4(0); // Timestamp

	if (Cache.m_Sym == GEOCACHE_FOUND)
	{
		SymbolId = 10016; // exclamation point
	}
	else
	{
		SymbolId = lowranceusr_find_icon_number_from_desc(Cache.m_GsCacheType);
	}

	// symbol (icon) for the waypoint
	fwrite4(SymbolId);

	// USER waypoint type
	fwrite2(0);
}

long int CLowranceWriter::lowranceusr_find_icon_number_from_desc(String& CacheType)
{
	const lowranceusr_icon_mapping_t *i;

	for (i = lowranceusr_icon_value_table; i->icon; i++) 
	{
		if (CacheType == i->icon) 
		{
			return i->value;
		}
	}

	return DEF_ICON;
}