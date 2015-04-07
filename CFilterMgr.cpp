#include "CFilterMgr.h"
#include "CGpxParser.h"

//------------------------------------------------------------------------------------------------------------------------
// Base class for filters. Derived classes must implement the OnFilterCache() function.
CFilterBase::CFilterBase(const TCHAR* pText, GcFilter FilterType)
{
	m_pFilterMgr = new CFilterMgr;

	m_Name = pText;
	m_Type = FilterType;
	m_pData = 0;
	m_Enabled = false;
}

CFilterBase::~CFilterBase()
{
	if (m_pFilterMgr)
	{
		delete m_pFilterMgr;
	}
}

// 'Enable' (set to 'true') or 'Disable' the filter.
void CFilterBase::Enable(bool Switch)
{
	m_Enabled = Switch;
}

// Returns 'true' if the filter is enabled.
bool CFilterBase::IsEnabled()
{
	return m_Enabled;
}

// Returns the name of the filter
String CFilterBase::GetName()
{
	return m_Name;
}

// Returns the type of the filter
GcFilter CFilterBase::GetType()
{
	return m_Type;
}

// Set the data pointer
void CFilterBase::SetData(void* pData)
{
	m_pData = pData;
}

// Get the data pointer
void* CFilterBase::GetData()
{
	return m_pData;
}

void CFilterBase::Serialize(CStream& ar)
{
	#define	CFilterBaseVersion 100

	if (ar.IsStoring())
	{
		ar << CFilterBaseVersion;
		ar << m_Name;
		ar << m_Enabled;
		ar << m_Type;

		m_pFilterMgr->Serialize(ar);
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			int Type;

			ar >> m_Name;
			ar >> m_Enabled;
			ar >> Type;

			m_Type = (GcFilter) Type;

			m_pFilterMgr->Serialize(ar);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------

// Manages the cache filters. When filters are applied to caches, the Filter() function
// examines each cache and 'enables' or 'disables' it according to the filtering rules
// implemented in classes derived from CFilterBase
CFilterMgr::CFilterMgr()
{
	m_pFilterResults = 0;
}

CFilterMgr::~CFilterMgr()
{
	Reset();

	if (m_pFilterResults)
	{
		delete m_pFilterResults;
	}
}

// Registers a new filter with the manager
void CFilterMgr::Add(CFilterBase* pFilter)
{
	CFilterBase* pFound = Find(pFilter->GetType());

	if (!pFound)
	{
		m_Filters.push_back(pFilter);
	}
}

// Accessors for the filters
CFilterBase* CFilterMgr::First(itFilter& it)
{
	it = m_Filters.begin();

	if (it != m_Filters.end())
	{
		return *it;
	}

	return 0;
}

CFilterBase* CFilterMgr::Next(itFilter& it)
{
	it++;

	if (it != m_Filters.end())
	{
		return *it;
	}

	return 0;
}

bool CFilterMgr::EndOfFilters(itFilter& it)
{
	if (it != m_Filters.end())
	{
		return false;
	}

	return true;
}

// Find a filter according to its GcFilter Id or NULL if not found.
CFilterBase* CFilterMgr::Find(GcFilter Filter)
{
	for (itFilter I = m_Filters.begin(); I != m_Filters.end(); I++)
	{
		if ((*I)->GetType() == Filter)
		{
			return *I;
		}

		if ((*I)->m_pFilterMgr->Size())
		{
			CFilterBase* pFilt = (*I)->m_pFilterMgr->Find(Filter);

			if (pFilt)
			{
				return pFilt;
			}
		}
	}

	return 0;
}

// Method used to run the caches through the filters.
void CFilterMgr::Filter(CGpxParser& Parser)
{
	itGC C;


	CGeoCache* pCache = Parser.First(C);

	while (!Parser.EndOfCacheList(C))
	{
		int Results = 0;

		// Run the cache through the filters...
		for (itFilter I = m_Filters.begin(); I != m_Filters.end(); I++)
		{
			// If the filter isn't active, just add one to result as if the filter evaluated to 'true'
			if (!(*I)->IsEnabled())
			{
				Results++;
			}
			else
			{
				// If the filter evaluated to 'true' increment the count for the result
				if ((*I)->OnFilterCache(pCache))
				{
					Results++;
				}
			}
		}

		// If the count in the results matches the number of filters, all the filters evaluated to 'true' and the cache should be part of the result set.
		// Otherwise, the cache needs to be excluded.
		if (Results != m_Filters.size())
		{
			pCache->m_InScope = false;
		}
		else
		{
			// Declare the cache as 'in scope (visible)' in the list
			pCache->m_InScope = true;
		}

		pCache = Parser.Next(C);
	}

}

// Returns the # of filters present in this filter manager
long CFilterMgr::Size()
{
	return m_Filters.size();
}

// Cleanup of filters
void CFilterMgr::Reset()
{
	for (itFilter I = m_Filters.begin(); I != m_Filters.end(); I++)
	{
		delete *I;
	}
	
	m_Filters.clear();	
}

void CFilterMgr::Serialize(CStream& ar)
{
	#define	CFilterMgrVersion	100
	
	if (ar.IsStoring())
	{
		ar << CFilterMgrVersion;

		ar << Size();

		for (itFilter I = m_Filters.begin(); I != m_Filters.end(); I++)
		{
			ar << (*I)->GetType();

			(*I)->Serialize(ar);
		}
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			long Count;

			ar >> Count;

			while (Count--)
			{
				GcFilter	FilterType;
				int			Type;

				ar >> Type;

				FilterType = (GcFilter) Type;

				CFilterBase* pFilt = Find(FilterType);

				if (pFilt)
				{
					pFilt->Serialize(ar);
				}
			}
		}
	}
}

// Returns the # of filters that are currently active
int	CFilterMgr::GetEnabledFilterCount()
{
	int Count = 0;

	for (itFilter I = m_Filters.begin(); I != m_Filters.end(); I++)
	{
		if ((*I)->IsEnabled())
		{
			Count++;
		}
	}

	return Count;
}
