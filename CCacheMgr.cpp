#include "StdAfx.h"
#include "CCacheMgr.h"
#include "CFieldNoteMgr.h"
#include "Literals.h"
#include "CPath.h"

//----------------------------------------------------------------------------------------------------------------------

CWaypoint::CWaypoint()
{
	m_Lat = 0;
	m_Long = 0;
}

#define CWaypointVersion	100

void CWaypoint::Serialize(CStream& ar)
{
	if (ar.IsStoring())
	{
		ar << CWaypointVersion;
		ar << m_Lat;
		ar << m_Long;
		ar << m_Name;
		ar << m_Desc;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Lat;
			ar >> m_Long;
			ar >> m_Name;
			ar >> m_Desc;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

CWPMgr::CWPMgr()
{
}

CWPMgr::~CWPMgr()
{
	Reset();
}

#define CWPMgrVersion 100

void CWPMgr::Serialize(CStream& ar)
{
	if (ar.IsStoring())
	{
		ar << CWPMgrVersion;
		
		ar << m_Wps.size();

		for (itWP W = m_Wps.begin(); W != m_Wps.end(); W++)
		{
			(*W)->Serialize(ar);
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
				CWaypoint* pWp = new CWaypoint;

				pWp->Serialize(ar);

				m_Wps.push_back(pWp);
			}
		}
	}
}

void CWPMgr::Reset()
{
	for (itWP W = m_Wps.begin(); W != m_Wps.end(); W++)
	{
		delete *W;
	}

	m_Wps.clear();
}

//----------------------------------------------------------------------------------------------------------------------

CCacheMgr::CCacheMgr()
{
}

CCacheMgr::~CCacheMgr()
{
	Reset();
}

bool CCacheMgr::IsAlreadyDefined(CGeoCache* pGc)
{
	for (itGC C = m_Caches.begin(); C != m_Caches.end(); C++)
	{
		if ((*C)->m_GsCacheName == pGc->m_GsCacheName && (*C)->m_Shortname == pGc->m_Shortname)
		{
			return true;
		}
	}

	return false;
}

CGeoCache* CCacheMgr::New(String& Owner)
{
	CGeoCache* pGc = new CGeoCache;

	if (!pGc)
	{
		return 0;
	}

	m_Caches.push_back(pGc);

	// Allocate a waypoint mgr for the new cache
	pGc->m_WpMgr = new CWPMgr;

	// Allocate a field note for the new cache
	pGc->m_pFieldNote = new CFieldNote;

	// Initialize some defaults for the new cache
	GetLocalTime(&pGc->m_CreationTime);

	// Make sure that the cache is exportable
	pGc->m_InScope = true;

	pGc->m_GsCacheName = DEFAULT_CACHE_NAME;
	pGc->m_Shortname = _T("GCxxxx");
	pGc->m_GsCacheType = TYPE_TRADITIONAL;
	pGc->m_GsCacheContainer = CONT_REGULAR;
	pGc->m_GsCacheOwnerName = Owner;
	pGc->m_GsCacheCountry = _T("Country");
	pGc->m_GsCacheState = _T("State");
	pGc->m_GsCacheDifficulty = 1.0;
	pGc->m_GsCacheTerrain = 1.0;
	pGc->m_GsCacheShortDesc = _T("Provide the short description here.");
	pGc->m_GsCacheLongDesc = _T("Provide the long description here.");

	return pGc;
}

void CCacheMgr::Delete(CGeoCache* pGc)
{
	for (itGC C = m_Caches.begin(); C != m_Caches.end(); C++)
	{
		if ((*C) == pGc)
		{
			DestroyObjs(pGc);

			m_Caches.erase(C);

			break;
		}
	}
}

void CCacheMgr::Copy(CGpxParser* pParser, CGeoCache* pGcDest, CGeoCache* pGcSrc)
{
	// Back the pointers 
	CFieldNote* pBackupFN = pGcDest->m_pFieldNote;
	CWPMgr*	pWpMgr = pGcDest->m_WpMgr;

	// Copy (shallow) the selected cache into the new one
	*pGcDest = *pGcSrc;

	// retrieve the text of the descriptions from the file
	AW_CONVERSION;

	pGcDest->m_GsCacheShortDesc = a2w(pParser->ReadFromTextStore(pGcDest->m_GsCacheShortDesc));
	pGcDest->m_GsCacheLongDesc = a2w(pParser->ReadFromTextStore(pGcDest->m_GsCacheLongDesc));

	// Reset some of the pointers
	pGcDest->m_pCurrCLE = 0;
	pGcDest->m_pCurrTB = 0;

	// Remove all TBs & Logs
	pGcDest->ForgetTBsAndLogs();

	// Restore the pointers which have been erased during the copy
	pGcDest->m_pFieldNote = pBackupFN;
	pGcDest->m_WpMgr = pWpMgr;

	// Copy any field note present
	if (pGcDest->m_pFieldNote && pGcSrc->m_pFieldNote)
	{
		*pGcDest->m_pFieldNote = *pGcSrc->m_pFieldNote;
	}

	// Make sure that the cache is exportable
	pGcDest->m_InScope = true;
}

void CCacheMgr::DestroyObjs(CGeoCache* pGc)
{
	if (pGc->m_WpMgr)
	{
		delete pGc->m_WpMgr;
	}

	if (pGc->m_pFieldNote)
	{
		delete pGc->m_pFieldNote;
	}

	delete pGc;
}

#define CCacheMgrVersion	101

void CCacheMgr::Serialize(CStream& ar)
{
	if (ar.IsStoring())
	{
		ar << CCacheMgrVersion;
		
		ar << m_Caches.size();

		for (itGC I = m_Caches.begin(); I != m_Caches.end(); I++)
		{
			(*I)->Serialize(ar);
		}

		ar << m_CacheCategories.size();

		for (itStr S = m_CacheCategories.begin(); S != m_CacheCategories.end(); S++)
		{
			ar << *S;
		}
	}
	else
	{
		Reset();

		int			Version;
		long		Count;
		String		Category;

		ar >> Version;

		if (Version >= 100)
		{

			ar >> Count;

			while (Count--)
			{
				CGeoCache* pGC = new CGeoCache;

				pGC->Serialize(ar);

				// Fix up to make sure that caches previously created in former versions of the app will be exportable
				pGC->m_InScope = true;

				m_Caches.push_back(pGC);
			}
		}

		if (Version >= 101)
		{
			ar >> Count;

			while (Count--)
			{
				ar >> Category;

				m_CacheCategories.push_back(Category);
			}
		}

		if (!m_CacheCategories.size())
		{
			m_CacheCategories.push_back(NO_CATEGORY);
		}
	}
}

void CCacheMgr::Reset()
{
	for (itGC I = m_Caches.begin(); I != m_Caches.end(); I++)
	{
		DestroyObjs(*I);
	}

	m_CacheCategories.clear();
	m_Caches.clear();
}

bool CCacheMgr::AddCacheCategory(String& Cat)
{
	TCHAR* pCat = (TCHAR*) Cat.c_str();

	for (itStr S = m_CacheCategories.begin(); S != m_CacheCategories.end(); S++)
	{
		TCHAR* pCurrentCat = (TCHAR*) (*S).c_str();

		if (!_tcsicmp(pCat, pCurrentCat))
		{
			return false;
		}
	}

	m_CacheCategories.push_back(Cat);

	return true;
}

bool CCacheMgr::DelCacheCategory(String& Cat)
{
	TCHAR* pCat = (TCHAR*) Cat.c_str();

	for (itStr S = m_CacheCategories.begin(); S != m_CacheCategories.end(); S++)
	{
		TCHAR* pCurrentCat = (TCHAR*) (*S).c_str();

		// Found it?
		if (!_tcsicmp(pCat, pCurrentCat))
		{
			// Removing 'NO_CATEGORY' is not allowed
			if(!_tcsicmp(pCat, NO_CATEGORY))
			{
				return false;
			}

			// Yes, remove the category from the caches that have it assigned to them
			for (itGC I = m_Caches.begin(); I != m_Caches.end(); I++)
			{
				CGeoCache* pCache = *I;

				TCHAR* pCacheCat = (TCHAR*) pCache->m_Category.c_str();

				if (!_tcsicmp(pCat, pCacheCat))
				{
					pCache->m_Category = NO_CATEGORY;
				}
			}

			// Remove the category from the list
			m_CacheCategories.erase(S);

			return true;
		}
	}

	return false;
}

void CCacheMgr::SaveConfig()
{
	CPath	Path;
	
	String	CacheMgrFname = GPXSONAR_CACHE_MGR_FILENAME;

	CacheMgrFname = Path.BuildPath(CacheMgrFname);

	CStream	ar;

	ar.SetStoring(true);

	Serialize(ar);

	ar.Save(CacheMgrFname);
}

void CCacheMgr::LoadConfig()
{
	CPath	Path;
	
	String	CacheMgrFname = GPXSONAR_CACHE_MGR_FILENAME;

	CacheMgrFname = Path.BuildPath(CacheMgrFname);

	CStream	ar;

	ar.Load(CacheMgrFname);

	Serialize(ar);
}
