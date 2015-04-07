#include "CFilterCacheTypes.h"

//------------------------------------------------------------------------------------------------------------------------

CFiltCacheTypes::CFiltCacheTypes()
{
	m_Type = GT_Unknown;
	m_Enabled = false;
}

CFiltCacheTypes::CFiltCacheTypes(GcType Type, bool Enabled)
{
	m_Type = Type;
	m_Enabled = Enabled;
}

void CFiltCacheTypes::Serialize(CStream& ar)
{
	#define CFiltCacheTypesVersion	100

	if (ar.IsStoring())
	{
		ar << CFiltCacheTypesVersion;
		ar << m_Type;
		ar << m_Enabled;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			int Type;
			ar >> Type;
			m_Type = (GcType) Type;

			ar >> m_Enabled;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------

CFilterCacheTypes::CFilterCacheTypes(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
	InitDefaultFilters();
}

void CFilterCacheTypes::InitDefaultFilters()
{
	m_Types.push_back(new CFiltCacheTypes(GT_Traditional, true));
	m_Types.push_back(new CFiltCacheTypes(GT_Multi, true));
	m_Types.push_back(new CFiltCacheTypes(GT_Virtual, true));
	m_Types.push_back(new CFiltCacheTypes(GT_Webcam, true));
	m_Types.push_back(new CFiltCacheTypes(GT_Unknown, true));
	m_Types.push_back(new CFiltCacheTypes(GT_LetterboxHybrid, true));
	m_Types.push_back(new CFiltCacheTypes(GT_Event, true));
	m_Types.push_back(new CFiltCacheTypes(GT_ProjectAPE, true));
	m_Types.push_back(new CFiltCacheTypes(GT_Locationless, true));
	m_Types.push_back(new CFiltCacheTypes(GT_CITO, true));
	m_Types.push_back(new CFiltCacheTypes(GT_Earthcache, true));
}

CFilterCacheTypes::~CFilterCacheTypes()
{
	Reset();
}

bool CFilterCacheTypes::OnFilterCache(CGeoCache* pCache)
{
	for (itFiltCacheTypes it = m_Types.begin(); it != m_Types.end(); it++)
	{
		GcType CacheType = pCache->TypeLookup();

		// Is this cache type what we want?
		if ((CacheType == (*it)->m_Type) && !(*it)->m_Enabled)
		{
			// Nope
			return false;
		}
	}

	// Acceptable cache type
	return true;
}

void CFilterCacheTypes::Serialize(CStream& ar)
{
	#define	CFilterCacheTypesVersion 102

	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterCacheTypesVersion;
		ar << m_Types.size();

		for (itFiltCacheTypes it = m_Types.begin(); it != m_Types.end(); it++)
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
			long Count;

			ar >> Count;

			while (Count--)
			{
				CFiltCacheTypes* pFilt = new CFiltCacheTypes;

				pFilt->Serialize(ar);

				m_Types.push_back(pFilt);
			}

			// Version 101 has one less cache type filter. Remove the version 100 set and replace with the latest set
			if (Version == 100 || Version == 101 || (m_Types.size() != GT_MUST_BE_LAST))
			{
				Reset();
				InitDefaultFilters();
			}
		}
	}
}

void CFilterCacheTypes::Reset()
{
	for (itFiltCacheTypes it = m_Types.begin(); it != m_Types.end(); it++)
	{
		delete *it;
	}

	m_Types.clear();
}
