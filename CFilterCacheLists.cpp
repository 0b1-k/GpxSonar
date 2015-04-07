#include "CFilterCacheLists.h"
#include "CGpxParser.h"
#include "CFieldNoteMgr.h"

CFilterCacheLists::CFilterCacheLists(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
	m_ShowIgnoredCaches = false;
	m_HideFoundCaches = false;
	m_HideDisabledCaches = false;
	m_UseShowOnly = false;
	m_ExclusiveOptions = ShowFoundCachesOnly;

	// This filter is enabled by default
	Enable(true);
}

CFilterCacheLists::~CFilterCacheLists()
{
	m_CacheIds.clear();
}

void CFilterCacheLists::Ignore(CGeoCache* pCache)
{
	// Flag the cache as ignored
	pCache->m_Ignored = true;

	// Add its cache Id to the list of ignored caches
	if (!Find(pCache->m_GsCacheId))
	{
		m_CacheIds.push_back(pCache->m_GsCacheId);
	}

	// run the cache through the filter to determine if it's in scope according to the user settings
	pCache->m_InScope = OnFilterCache(pCache);
}

void CFilterCacheLists::UnIgnore(CGeoCache* pCache)
{
	pCache->m_Ignored = false;

	for (itCacheId it = m_CacheIds.begin(); it != m_CacheIds.end(); it++)
	{
		if ((*it) == pCache->m_GsCacheId)
		{
			m_CacheIds.erase(it);
			break;
		}
	}

	pCache->m_InScope = OnFilterCache(pCache);
}

bool CFilterCacheLists::Find(long Id)
{
	for (itCacheId it = m_CacheIds.begin(); it != m_CacheIds.end(); it++)
	{
		if ((*it) == Id)
		{
			return true;
		}
	}

	return false;
}

bool CFilterCacheLists::OnFilterCache(CGeoCache* pCache)
{
	if (m_UseShowOnly)
	{
		// Show the 'Found Caches' Only?
		if (m_ExclusiveOptions == ShowFoundCachesOnly)
		{
			// If the cache is marked with a green checkmark, it's a match
			if (!pCache->m_Sym.empty())
			{
				return true;
			}

			// The cache has a 'Found It' field note?
			if (pCache->m_pFieldNote && pCache->m_pFieldNote->m_Status == NoteStatusFoundIt)
			{
				return true;
			}
		}
		// Show any cache with a field note?
		else if (pCache->m_pFieldNote)
		{
			return true;
		}

		return false;
	}
	else
	{
		// Is the cache 'ignored' and the user allows showing ignored caches?
		if (!m_ShowIgnoredCaches && pCache->m_Ignored)
		{
			// The cache needs to be visible
			return false;
		}

		// Are 'Found Caches' supposed to be hidden?
		if (m_HideFoundCaches)
		{
			// Is this cache a 'Found' cache?
			if ((pCache->m_pFieldNote && pCache->m_pFieldNote->m_Status == NoteStatusFoundIt) || !pCache->m_Sym.empty())
			{
				// Hide this cache
				return false;
			}
		}

		// Are 'Disabled/Archived' caches supposed to be hidden?
		if (m_HideDisabledCaches)
		{
			// Is this cache disabled / archived?
			if ((pCache->m_GsCacheArchived || !pCache->m_GsCacheAvailable))
			{
				// Hide this cache
				return false;
			}
		}
	}

	// If we get here, the cache is supposed to be visible since it does not match any of the other conditions.
	return true;
}

void CFilterCacheLists::Serialize(CStream& ar)
{
	#define	CFilterCacheListsVersion 101

	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterCacheListsVersion;

		ar << m_ShowIgnoredCaches;
		ar << m_HideFoundCaches;
		ar << m_UseShowOnly;
		ar << m_ExclusiveOptions;

		ar << m_CacheIds.size();

		for (itCacheId it = m_CacheIds.begin(); it != m_CacheIds.end(); it++)
		{
			ar << (*it);
		}

		ar << m_HideDisabledCaches;
	}
	else
	{
		m_CacheIds.clear();

		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_ShowIgnoredCaches;
			ar >> m_HideFoundCaches;
			ar >> m_UseShowOnly;
			ar >> m_ExclusiveOptions;

			long Count;

			ar >> Count;

			while (Count--)
			{
				long Id;

				ar >> Id;

				m_CacheIds.push_back(Id);
			}
		}

		if (Version >= 101)
		{
			ar >> m_HideDisabledCaches;
		}
	}
}
