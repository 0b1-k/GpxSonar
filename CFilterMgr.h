#ifndef _INC_CFilterMgr
	#define _INC_CFilterMgr

#include "CommonDefs.h"
#include "CStream.h"

// Main filters
typedef enum {
	FilterCacheTypes = 0,
	FilterContainerTypes,
	FilterBearingDistance,
	FilterCacheLists,
	FilterStateList,
	FilterCountryList,
	FilterSearch,
	FilterRatings,
	FilterTravelBugs,
	//FilterSuccessRatio,
	EndOfGcFilter
	} GcFilter;

class CGeoCache;
class CFilterMgr;

// Base class for filters. Derived classes must implement the OnFilterCache() function.
class CFilterBase
{
public:
	CFilterMgr*		m_pFilterMgr;

protected:
	String			m_Name;
	bool			m_Enabled;
	GcFilter		m_Type;
	void*			m_pData;

public:
	CFilterBase(const TCHAR* pText, GcFilter FilterType);
	virtual ~CFilterBase();

	// 'Enable' (set to 'true') or 'Disable' the filter.
	void		Enable(bool Switch);

	// Returns 'true' if the filter is enabled.
	bool		IsEnabled();

	// Returns the name of the filter
	String		GetName();

	// Returns the type of the filter
	GcFilter	GetType();

	// Set the data pointer
	void		SetData(void* pData);

	// Get the data pointer
	void*		GetData();

	// Determines if the cache is being 'disabled' by the filter.
	// Return 'false' to filter a cache out of the list.
	virtual bool OnFilterCache(CGeoCache* pCache) = 0;

	virtual void Serialize(CStream& ar);
};

typedef vector<CFilterBase*> Filters;
typedef vector<CFilterBase*>::iterator itFilter;

class CGpxParser;

// Manages the cache filters. When filters are applied to caches, the Filter() function
// examines each cache and 'enables' or 'disables' it according to the filtering rules
// implemented in classes derived from CFilterBase
class CFilterMgr
{
protected:
	Filters			m_Filters;
	bool*			m_pFilterResults;

public:
	CFilterMgr();
	~CFilterMgr();

	// Registers a new filter with the manager
	void			Add(CFilterBase* pFilter);

	// Accessors for the filters
	CFilterBase*	First(itFilter& it);
	CFilterBase*	Next(itFilter& it);
	bool			EndOfFilters(itFilter& it);

	// Find a filter according to its GcFilter Id or NULL if not found.
	CFilterBase*	Find(GcFilter Filter);

	// Method used to run the caches through the filters.
	void			Filter(CGpxParser& Parser);

	// Returns the # of filters present in this filter manager
	long			Size();

	void			Serialize(CStream& ar);

	// Returns the # of filters that are currently active
	int				GetEnabledFilterCount();

protected:
	// Cleanup of filters
	void			Reset();
};

#endif