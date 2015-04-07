#ifndef _INC_CFilterBearingDistance
	#define _INC_CFilterBearingDistance

#include "CommonDefs.h"
#include "CFilterMgr.h"
#include <vector>

#define BEARING_NORTH		_T("N")
#define BEARING_NORTHEAST	_T("NE")
#define BEARING_EAST		_T("E")
#define BEARING_SOUTHEAST	_T("SE")
#define BEARING_SOUTH		_T("S")
#define BEARING_SOUTHWEST	_T("SW")
#define BEARING_WEST		_T("W")
#define BEARING_NORTHWEST	_T("NW")

#define BEARING_NORTH_DEGREE		0
#define BEARING_NORTHEAST_DEGREE	45
#define BEARING_EAST_DEGREE			90
#define BEARING_SOUTHEAST_DEGREE	135
#define BEARING_SOUTH_DEGREE		180
#define BEARING_SOUTHWEST_DEGREE	225
#define BEARING_WEST_DEGREE			270
#define BEARING_NORTHWEST_DEGREE	315

using namespace std;

class CFilterBearing
{
public:
	bool		m_Enabled;
	String		m_Bearing;

public:
	CFilterBearing();
	CFilterBearing(const TCHAR* pBearing, bool Enabled);

	void Serialize(CStream& ar);
};

typedef vector<CFilterBearing*> FiltBearingCont;
typedef vector<CFilterBearing*>::iterator itFiltBearing;

class CFilterBearingDistance : public CFilterBase
{
public:
	double			m_Distance;
	FiltBearingCont	m_Bearings;

public:
	CFilterBearingDistance(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterBearingDistance();

	virtual bool OnFilterCache(CGeoCache* pCache);

	virtual void Serialize(CStream& ar);

protected:
	void	Reset();
};

#endif