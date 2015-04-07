#include "CFilterCacheContainers.h"

//------------------------------------------------------------------------------------------------------------------------

CFiltContainerTypes::CFiltContainerTypes()
{
	m_Container = GC_Unknown;
	m_Enabled = false;
}

CFiltContainerTypes::CFiltContainerTypes(GcContainer Cont, bool Enabled)
{
	m_Container = Cont;
	m_Enabled = Enabled;
}

void CFiltContainerTypes::Serialize(CStream& ar)
{
	#define CFiltContainerTypesVersion	100

	if (ar.IsStoring())
	{
		ar << CFiltContainerTypesVersion;
		ar << m_Container;
		ar << m_Enabled;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			int Cont;
			ar >> Cont;
			m_Container = (GcContainer) Cont;

			ar >> m_Enabled;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------

CFilterCacheContainers::CFilterCacheContainers(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
	m_Conts.push_back(new CFiltContainerTypes(GC_Unknown, true));
	m_Conts.push_back(new CFiltContainerTypes(GC_Micro, true));
	m_Conts.push_back(new CFiltContainerTypes(GC_Small, true));
	m_Conts.push_back(new CFiltContainerTypes(GC_Regular, true));
	m_Conts.push_back(new CFiltContainerTypes(GC_Large, true));
	m_Conts.push_back(new CFiltContainerTypes(GC_Virtual, true));
	m_Conts.push_back(new CFiltContainerTypes(GC_NotListed, true));
}

CFilterCacheContainers::~CFilterCacheContainers()
{
	Reset();
}

bool CFilterCacheContainers::OnFilterCache(CGeoCache* pCache)
{
	GcContainer	CacheContainer = pCache->ContainerLookup();

	for (itFiltCacheContainer it = m_Conts.begin(); it != m_Conts.end(); it++)
	{
		// Is this a container that we don't want to see?
		if ((CacheContainer == (*it)->m_Container) && !(*it)->m_Enabled)
		{
			// Exclude the cache
			return false;
		}
	}

	// The cache has a desirable container
	return true;
}

void CFilterCacheContainers::Serialize(CStream& ar)
{
	#define	CFilterCacheContainersVersion 101

	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterCacheContainersVersion;
		ar << m_Conts.size();

		for (itFiltCacheContainer it = m_Conts.begin(); it != m_Conts.end(); it++)
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
				CFiltContainerTypes* pFilt = new CFiltContainerTypes;

				pFilt->Serialize(ar);

				m_Conts.push_back(pFilt);
			}
		}
	}
}

void CFilterCacheContainers::Reset()
{
	for (itFiltCacheContainer it = m_Conts.begin(); it != m_Conts.end(); it++)
	{
		delete *it;
	}

	m_Conts.clear();
}
