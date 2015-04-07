#include "CFilterBearingDistance.h"
#include "CGpxParser.h"

//------------------------------------------------------------------------------------------------------------------------

CFilterBearing::CFilterBearing()
{
	m_Enabled = false;
}

CFilterBearing::CFilterBearing(const TCHAR* pBearing, bool Enabled)
{
	m_Bearing = pBearing;
	m_Enabled = Enabled;
}

void CFilterBearing::Serialize(CStream& ar)
{
	#define CFilterBearingVersion	100

	if (ar.IsStoring())
	{
		ar << CFilterBearingVersion;
		ar << m_Enabled;
		ar << m_Bearing;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Enabled;
			ar >> m_Bearing;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------
CFilterBearingDistance::CFilterBearingDistance(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
	m_Distance = 500.0;

	m_Bearings.push_back(new CFilterBearing(BEARING_NORTH, true));
	m_Bearings.push_back(new CFilterBearing(BEARING_NORTHEAST, true));
	m_Bearings.push_back(new CFilterBearing(BEARING_EAST, true));
	m_Bearings.push_back(new CFilterBearing(BEARING_SOUTHEAST, true));
	m_Bearings.push_back(new CFilterBearing(BEARING_SOUTH, true));
	m_Bearings.push_back(new CFilterBearing(BEARING_SOUTHWEST, true));
	m_Bearings.push_back(new CFilterBearing(BEARING_WEST, true));
	m_Bearings.push_back(new CFilterBearing(BEARING_NORTHWEST, true));
}

CFilterBearingDistance::~CFilterBearingDistance()
{
	Reset();
}

bool CFilterBearingDistance::OnFilterCache(CGeoCache* pCache)
{
	// Is the cache too far?
	if (pCache->m_Distance > m_Distance)
	{
		return false;
	}

	for (itFiltBearing it = m_Bearings.begin(); it != m_Bearings.end(); it++)
	{
		// Is this one of the bearings that we're not interested in?
		if (pCache->m_Bearing == (*it)->m_Bearing && !(*it)->m_Enabled)
		{
			return false;
		}
	}

	// Distance and bearing are OK
	return true;
}

void CFilterBearingDistance::Serialize(CStream& ar)
{
	#define CFilterBearingDistanceVersion	100

	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterBearingDistanceVersion;

		ar << m_Distance;

		ar << m_Bearings.size();

		for (itFiltBearing it = m_Bearings.begin(); it != m_Bearings.end(); it++)
		{
			(*it)->Serialize(ar);
		}
	}
	else
	{
		Reset();

		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Distance;

			long Count;

			ar >> Count;

			while (Count--)
			{
				CFilterBearing* pFilt = new CFilterBearing;

				pFilt->Serialize(ar);

				m_Bearings.push_back(pFilt);
			}
		}
	}
}

void CFilterBearingDistance::Reset()
{
	for (itFiltBearing it = m_Bearings.begin(); it != m_Bearings.end(); it++)
	{
		delete (*it);
	}

	m_Bearings.clear();
}