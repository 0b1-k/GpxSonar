#include "CFilterCacheRatings.h"
#include "CGpxParser.h"

CFilterCacheRatings::CFilterCacheRatings(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
	m_TerrOper = OperGreaterEqual;
	m_TerrLvl = 1.0;
	m_DiffOper = OperGreaterEqual;
	m_DiffLvl = 1.0;
	m_DiffEnabled = false;
	m_TerrEnabled = false;
}

CFilterCacheRatings::~CFilterCacheRatings()
{
}

bool CFilterCacheRatings::OnFilterCache(CGeoCache* pCache)
{
	bool DiffRc = false;

	if (m_DiffEnabled)
	{
		switch(m_DiffOper)
		{
		case OperGreaterEqual:
			if (pCache->m_GsCacheDifficulty >= m_DiffLvl)
			{
				DiffRc = true;
			}
			break;
		case OperEqual:
			if (pCache->m_GsCacheDifficulty == m_DiffLvl)
			{
				DiffRc = true;
			}
			break;
		case OperLessEqual:
			if (pCache->m_GsCacheDifficulty <= m_DiffLvl)
			{
				DiffRc = true;
			}
			break;
		}
	}
	
	bool TerrRc = false;

	if (m_TerrEnabled)
	{
		switch (m_TerrOper)
		{
		case OperGreaterEqual:
			if (pCache->m_GsCacheTerrain >= m_TerrLvl)
			{
				TerrRc = true;
			}
			break;
		case OperEqual:
			if (pCache->m_GsCacheTerrain == m_TerrLvl)
			{
				TerrRc = true;
			}
			break;
		case OperLessEqual:
			if (pCache->m_GsCacheTerrain <= m_TerrLvl)
			{
				TerrRc = true;
			}
			break;
		}
	}

	if (m_DiffEnabled && m_TerrEnabled)
	{
		// Both the Difficulty and the Terrain ratings must have passed the filter
		if (DiffRc && TerrRc)
		{
			return true;
		}

		return false;
	}

	// Only the Difficulty rating is enabled...
	if (m_DiffEnabled && !m_TerrEnabled)
	{
		return DiffRc;
	}

	// Only the Terrain rating is enabled...
	if (!m_DiffEnabled && m_TerrEnabled)
	{
		return TerrRc;
	}

	// Nothing was enabled...
	return true;
}

void CFilterCacheRatings::Serialize(CStream& ar)
{
	#define CFilterCacheRatingsVersion	100

	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterCacheRatingsVersion;

		ar << m_TerrOper;
		ar << m_TerrLvl;
		ar << m_DiffOper;
		ar << m_DiffLvl;
		ar << m_DiffEnabled;
		ar << m_TerrEnabled;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			int Oper;

			ar >> Oper;
			m_TerrOper = (OperType) Oper;
			ar >> m_TerrLvl;
			ar >> Oper;
			m_DiffOper = (OperType) Oper;
			ar >> m_DiffLvl;
			ar >> m_DiffEnabled;
			ar >> m_TerrEnabled;
		}
	}
}
