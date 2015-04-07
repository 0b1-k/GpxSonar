#ifndef _INC_CCoords
	#define _INC_CCoords

#include "Coord.h"
#include "CommonDefs.h"
#include "CStream.h"

/*use: 110.4768 for distances in km,
       69.048 for statute miles,
       60.0 for nautical miles,
       5524 for surveyors chains*/

#define	KILOMETERS		"Km"
#define	STATUTE_MILES	"Miles"
#define	NAUTICAL_MILES	"Naut. Mi."

#define DIST_IN_KM				110.4768
#define DIST_IN_STATUTE_MILES	69.048
#define DIST_IN_NAUTICAL_MILES	60.0

#define DIST_UNITS_IN_KM				1000.0
#define DIST_UNITS_IN_STATUTE_MILES		(1000.0 * 1.6213712)
#define DIST_UNITS_IN_NAUTICAL_MILES	(1000.0 * 1.1799856)

class CCoords : public CLatLon
{
public:
	CCoords();
	
	void			SetDistanceUnits(double Units);
	double			GetDistanceUnits() const;

	void			SetDecimal(double lat, double lon);
	void			GetDecimal(double& lat, double& lon);

	const char*		GetDistanceLabel();

	void			GetDegMinMmm(int& LatDeg, float& LatMinMmm, String& LatChar, int& LongDeg, float& LongMinMmm, String& LongChar);
	void			SetDegMinMmm(int LatDeg, float LatMinMmm, String& LatChar, int LongDeg, float LongMinMmm, String& LongChar);

	String			Format(bool Split = false);

	void			Serialize(CStream& ar);
};

#endif
