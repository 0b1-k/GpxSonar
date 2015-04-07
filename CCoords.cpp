#include "CCoords.h"
#include <math.h>

CCoords::CCoords()
{
}

void CCoords::SetDistanceUnits(double Units)
{
	m_DegDist = Units;
}

double CCoords::GetDistanceUnits() const
{
	return m_DegDist;
}


void CCoords::GetDegMinMmm(
					int& LatDeg, float& LatMinMmm, String& LatChar, 
					int& LongDeg, float& LongMinMmm, String& LongChar)
{
	double lat_decimal = m_Latitude;

	if (lat_decimal < 0)
	{
		LatChar = _T("S");
		lat_decimal *= -1;
	}
	else
	{
		LatChar = _T("N");
	}

	LatDeg = (int) fabs(lat_decimal);
	lat_decimal -= LatDeg;
	LatMinMmm = (lat_decimal * 60);


	double long_decimal = m_Longitude;

	if (long_decimal < 0)
	{
		LongChar = _T("W");
		long_decimal *= -1;
	}
	else
	{
		LongChar = _T("E");
	}

	LongDeg = (int) fabs(long_decimal);
	long_decimal -= LongDeg;
	LongMinMmm = (long_decimal * 60);
}

void CCoords::SetDegMinMmm(
					int LatDeg, float LatMinMmm, String& LatChar, 
					int LongDeg, float LongMinMmm, String& LongChar)
{
	double lat_decimal = abs(LatDeg);

	lat_decimal += (LatMinMmm / 60.0);

	if (LatChar == _T("S"))
	{
		lat_decimal *= -1;
	}

	double long_decimal = abs(LongDeg);

	long_decimal += (LongMinMmm / 60.0);

	if (LongChar == _T("W"))
	{
		long_decimal *= -1;
	}

	SetDecimal(lat_decimal, long_decimal);
}

void CCoords::SetDecimal(double lat, double lon)
{
	m_Latitude = lat;
	m_Longitude = lon;
}

void CCoords::GetDecimal(double& lat, double& lon)
{
	lat = m_Latitude;
	lon = m_Longitude;
}

String CCoords::Format(bool Split)
{
	int		LatDeg;
	float	LatMinMmm;
	String	LatChar;
	int		LongDeg;
	float	LongMinMmm;
	String	LongChar;

	GetDegMinMmm(LatDeg, LatMinMmm, LatChar, LongDeg, LongMinMmm, LongChar);

	#define MAX_BUFFER 100
	
	TCHAR Buffer[MAX_BUFFER];

	if (Split)
	{
		_sntprintf(Buffer, MAX_BUFFER, _T("%S %i° %.3f\r\n%S %i° %.3f"), LatChar.c_str(), LatDeg, LatMinMmm, LongChar.c_str(), LongDeg, LongMinMmm);
	}
	else
	{
		_sntprintf(Buffer, MAX_BUFFER, _T("%S %i° %.3f %S %i° %.3f"), LatChar.c_str(), LatDeg, LatMinMmm, LongChar.c_str(), LongDeg, LongMinMmm);
	}

	return Buffer;
}

void CCoords::Serialize(CStream& ar)
{
	#define CCoordsVersion	100

	if (ar.IsStoring())
	{
		ar << CCoordsVersion;
		ar << m_Latitude;
		ar << m_Longitude;
		ar << m_DegDist;
	}
	else
	{
		m_DegDist = DIST_UNITS_IN_STATUTE_MILES;
		m_Longitude = 0;
		m_Latitude = 0;

		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Latitude;
			ar >> m_Longitude;
			ar >> m_DegDist;

			// Fixup for the units required ever since the switch to FizzyMagic's geodesy code.
			if (m_DegDist == DIST_IN_KM)
			{
				m_DegDist = DIST_UNITS_IN_KM;
			}
			else if (m_DegDist == DIST_IN_STATUTE_MILES)
			{
				m_DegDist = DIST_UNITS_IN_STATUTE_MILES;
			}
			else if(m_DegDist == DIST_IN_NAUTICAL_MILES)
			{
				m_DegDist = DIST_UNITS_IN_NAUTICAL_MILES;
			}
		}
	}
}
