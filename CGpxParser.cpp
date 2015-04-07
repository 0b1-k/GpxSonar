#include "CGpxParser.h"
#include "windows.h"
#include "CBaseException.h"
#include "CFieldNoteMgr.h"
#include "CStream.h"
#include "CCacheMgr.h"
#include "CPath.h"
#include "Literals.h"
#include <algorithm>
#include ".\Zlib\unzip.h"

//------------------------------------------------------------------------------------------------------------------------
CTravelBug::CTravelBug()
{
	m_Id = 0;
}

//------------------------------------------------------------------------------------------------------------------------
LogTypeMap	CGeoCacheLogEntry::m_LogTypeMap;

CGeoCacheLogEntry::CGeoCacheLogEntry()
{

	m_Lat = 0;
	m_Long = 0;
	m_Id = 0;
	m_TextEncoded = false;
	memset(&m_Date,sizeof(m_Date), 0);
}


// Returns an enumerated value for the string type of the log
GcLogType CGeoCacheLogEntry::TypeLookup()
{
	if (m_LogTypeMap.empty())
	{
		m_LogTypeMap[_T("Found it")] = LOG_FoundIt;
		m_LogTypeMap[_T("Didn't find it")] = LOG_DidntFindIt;
		m_LogTypeMap[_T("Needs Archived")] = LOG_NeedsArchived;
		m_LogTypeMap[_T("Archive (no show)")] = LOG_ArchivedNoShow;
		m_LogTypeMap[_T("Archive (show)")] = LOG_ArchivedShow;
		m_LogTypeMap[_T("Camera")] = LOG_Camera;
		m_LogTypeMap[_T("Enable Listing")] = LOG_EnableListing;
		m_LogTypeMap[_T("Temporarily Disable Listing")] = LOG_TemporarilyDisableListing;
		m_LogTypeMap[_T("Other")] = LOG_Other;
	}

	itLogTypeMap it = m_LogTypeMap.find(m_Type);

	if (it != m_LogTypeMap.end())
	{
		return (*it).second;
	}
	else
	{
		return LOG_Other;
	}
}

//------------------------------------------------------------------------------------------------------------------------
CacheTypeMap		CGeoCache::m_CacheTypeMap;
CacheContainerMap	CGeoCache::m_CacheContainerMap;

CGeoCache::CGeoCache()
{
	InitializeInternalState();
}

CGeoCache::~CGeoCache()
{
	Reset();
}

// Resets the cache content
void CGeoCache::Reset()
{
	LogReset();
	TBReset();
}

// Empties the list of pointers to TBs and Logs
void CGeoCache::ForgetTBsAndLogs()
{
	m_TBs.clear();
	m_Logs.clear();
}

// Returns the 'this' pointer for the object
CGeoCache* CGeoCache::This()
{
	return this;
}

void CGeoCache::InitializeInternalState()
{
	m_ListIndex = -1;
	m_InScope = false;
	m_Ignored = false;

	m_Lat = 0.0;
	m_Long = 0.0;
	memset(&m_CreationTime,sizeof(m_CreationTime), 0);
	m_GsCacheId = 0;
	m_pFieldNote = 0;
	m_GsCacheAvailable = true;
	m_GsCacheArchived = false;
	m_GsCacheDifficulty = 0.0;
	m_GsCacheTerrain  = 0.0;
	m_pCurrCLE = 0;
	m_pCurrTB = 0;
	m_WpMgr = 0;
	m_Distance = 0;

	m_GcType = GT_NotInitialized;

	m_Category = NO_CATEGORY;
}

// Add a new log entry to the cache
void CGeoCache::AddLogEntry(CGeoCacheLogEntry* pLogEntry)
{
	m_Logs.push_back(pLogEntry);

	m_pCurrCLE = pLogEntry;
}

CGeoCacheLogEntry* CGeoCache::CurrentLogEntry()
{
	return m_pCurrCLE;
}

// Add a new travel bug to the cache. The last travel bug entry to be added is made the current one and it can be accessed through CurrentTravelBug().
void CGeoCache::AddTravelBug(CTravelBug* pTB)
{
	m_TBs.push_back(pTB);

	m_pCurrTB = pTB;
}

// Remove a travel bug from the cache
void CGeoCache::RemoveTravelBug(CTravelBug* pTB)
{
	for (itTB I = m_TBs.begin(); I != m_TBs.end(); I++)
	{
		if ((*I)->m_Ref == pTB->m_Ref)
		{
			delete (*I);
			m_TBs.erase(I);
			return;
		}
	}
}

// Return a pointer to the current travel bug or 0 if none were added
CTravelBug*	CGeoCache::CurrentTravelBug()
{
	return m_pCurrTB;
}

// Returns a pointer to the first log entry in the list. The iterator is set to end() at the end of the list.
CGeoCacheLogEntry* CGeoCache::FirstLog(itGCLogEntry& it)
{
	it = m_Logs.begin();

	if (it != m_Logs.end())
	{
		m_pCurrCLE = *it;

		return m_pCurrCLE;
	}

	m_pCurrCLE = 0;

	return m_pCurrCLE;
}

// Returns a pointer to the next log entry in the list. The iterator is set to end() at the end of the list.
CGeoCacheLogEntry* CGeoCache::NextLog(itGCLogEntry& it)
{
	it++;

	if (it != m_Logs.end())
	{
		m_pCurrCLE = *it;

		return m_pCurrCLE;
	}

	m_pCurrCLE = 0;

	return m_pCurrCLE;
}

// Returns 'true' if the iterator is at the end() of the list
bool CGeoCache::EndOfLogList(itGCLogEntry& it)
{
	if (it != m_Logs.end())
	{
		return false;
	}

	return true;
}

// Return a text version of the date when the cache was last found
void CGeoCache::LastFoundText(TCHAR* pBuffer)
{
	itGCLogEntry L;
	
	L = m_Logs.begin();

	if (L != m_Logs.end())
	{
		CGeoCacheLogEntry* pLog = *L;

		_stprintf(pBuffer, _T("%d-%02d-%02d"), pLog->m_Date.wYear, pLog->m_Date.wMonth, pLog->m_Date.wDay);

		return;
	}

	_tcscpy(pBuffer, _T("Never"));
}

// Returns a pointer to the first travel bug in the list. The iterator is set to end() at the end of the list.
CTravelBug* CGeoCache::FirstTB(itTB& it)
{
	it = m_TBs.begin();

	if (it != m_TBs.end())
	{
		m_pCurrTB = *it;

		return m_pCurrTB;
	}

	m_pCurrTB = 0;

	return m_pCurrTB;
}

// Returns a pointer to the next travel bug in the list. The iterator is set to end() at the end of the list.
CTravelBug* CGeoCache::NextTB(itTB& it)
{
	it++;

	if (it != m_TBs.end())
	{
		m_pCurrTB = *it;

		return m_pCurrTB;
	}

	m_pCurrTB = 0;

	return m_pCurrTB;
}

// Returns 'true' if the iterator is at the end() of the list
bool CGeoCache::EndOfTBList(itTB& it)
{
	if (it != m_TBs.end())
	{
		return false;
	}

	return true;
}

// Returns the # of TBs in the cache
long CGeoCache::GetTBCount()
{
	return m_TBs.size();
}

// Returns an enumerated value for the string type of the cache
GcType CGeoCache::TypeLookup()
{
	if (m_GcType != GT_NotInitialized)
	{
		return m_GcType;
	}

	if (m_CacheTypeMap.empty())
	{
		m_CacheTypeMap[TYPE_TRADITIONAL] = GT_Traditional;
		m_CacheTypeMap[TYPE_MULTI] = GT_Multi;
		m_CacheTypeMap[TYPE_VIRTUAL] = GT_Virtual;
		m_CacheTypeMap[TYPE_WEBCAM] = GT_Webcam;
		m_CacheTypeMap[TYPE_UNKNOWN] = GT_Unknown;
		m_CacheTypeMap[TYPE_LETTERBOXHYBRID] = GT_LetterboxHybrid;
		m_CacheTypeMap[TYPE_EVENT] = GT_Event;
		m_CacheTypeMap[TYPE_PROJECTAPE] = GT_ProjectAPE;
		m_CacheTypeMap[TYPE_LOCATIONLESS] = GT_Locationless;
		m_CacheTypeMap[TYPE_CITO] = GT_CITO;
		m_CacheTypeMap[TYPE_EARTHCACHE] = GT_Earthcache;
	}

	itCacheTypeMap it = m_CacheTypeMap.find(m_GsCacheType.c_str());

	if (it != m_CacheTypeMap.end())
	{
		m_GcType = (*it).second;
	}
	else
	{
		m_GcType = GT_Unknown;
	}

	return m_GcType;
}

// Returns an enumerated value for the string container of the cache
GcContainer	CGeoCache::ContainerLookup()
{
	if (m_CacheContainerMap.empty())
	{
		m_CacheContainerMap[CONT_UNKNOWN] = GC_Unknown;
		m_CacheContainerMap[CONT_MICRO] = GC_Micro;
		m_CacheContainerMap[CONT_SMALL] = GC_Small;
		m_CacheContainerMap[CONT_REGULAR] = GC_Regular;
		m_CacheContainerMap[CONT_LARGE] = GC_Large;
		m_CacheContainerMap[CONT_VIRTUAL] = GC_Virtual;
		m_CacheContainerMap[CONT_NOTLISTED] = GC_NotListed;
	}

	itCacheContainerMap it = m_CacheContainerMap.find(m_GsCacheContainer.c_str());

	if (it != m_CacheContainerMap.end())
	{
		return (*it).second;
	}
	else
	{
		return GC_Unknown;
	}
}

// Removes all log entries
void CGeoCache::LogReset()
{
	for (itGCLogEntry I = m_Logs.begin(); I != m_Logs.end(); I++)
	{
		delete *I;
	}

	m_Logs.clear();
}

void CGeoCache::TBReset()
{
	for(itTB T = m_TBs.begin(); T != m_TBs.end(); T++)
	{
		delete *T;
	}

	m_TBs.clear();
}

#define CGeoCacheVersion 102

// Serialize the cache to a stream
void CGeoCache::Serialize(CStream& ar)
{
	if (ar.IsStoring())
	{
		ar << CGeoCacheVersion;

		ar << m_Lat;
		ar << m_Long;
		ar << m_CreationTime;
		ar << m_Shortname;
		ar << m_GsCacheAvailable;
		ar << m_GsCacheArchived;
		ar << m_GsCacheShortDescIsHtml;
		ar << m_GsCacheLongDescIsHtml;
		ar << m_GsCacheId;
		ar << m_GsCacheDifficulty;
		ar << m_GsCacheTerrain;
		ar << m_GsCacheType;
		ar << m_GsCacheContainer;
		ar << m_GsCacheName;
		ar << m_GsCachePlacedBy;
		ar << m_GsCacheOwnerName;
		ar << m_GsCacheCountry;
		ar << m_GsCacheState;
		ar << m_GsCacheShortDesc;
		ar << m_GsCacheLongDesc;
		ar << m_GsCacheEncodedHints;

		if (m_WpMgr)
		{
			ar << true;

			m_WpMgr->Serialize(ar);
		}
		else
		{
			ar << false;
		}

		if (m_pFieldNote)
		{
			ar << true;

			m_pFieldNote->Serialize(ar);
		}
		else
		{
			ar << false;
		}

		ar << m_Category;
	}
	else
	{
		int Version;
		bool HasObj;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Lat;
			ar >> m_Long;
			ar >> m_CreationTime;
			ar >> m_Shortname;
			ar >> m_GsCacheAvailable;
			ar >> m_GsCacheArchived;
			ar >> m_GsCacheShortDescIsHtml;
			ar >> m_GsCacheLongDescIsHtml;
			ar >> m_GsCacheId;
			ar >> m_GsCacheDifficulty;
			ar >> m_GsCacheTerrain;
			ar >> m_GsCacheType;
			ar >> m_GsCacheContainer;
			ar >> m_GsCacheName;
			ar >> m_GsCachePlacedBy;
			ar >> m_GsCacheOwnerName;
			ar >> m_GsCacheCountry;
			ar >> m_GsCacheState;
			ar >> m_GsCacheShortDesc;
			ar >> m_GsCacheLongDesc;
			ar >> m_GsCacheEncodedHints;

			ar >> HasObj;

			if (HasObj)
			{
				m_WpMgr = new CWPMgr;

				if (m_WpMgr)
				{
					m_WpMgr->Serialize(ar);
				}
			}
		}

		if (Version >= 101)
		{
			ar >> HasObj;

			if (HasObj)
			{
				m_pFieldNote = new CFieldNote;

				if (m_pFieldNote)
				{
					m_pFieldNote->Serialize(ar);
				}
			}
		}

		if (Version >= 102)
		{
			ar >> m_Category;
		}
		else
		{
			m_Category = NO_CATEGORY;
		}
	}
}

static CGpxParser* pInst = 0;

AttrCont	CGpxParser::m_AttrMap;
ValCont		CGpxParser::m_ValMap;
ElemCont	CGpxParser::m_ElemMap;

//------------------------------------------------------------------------------------------------------------------------
CGpxParser::CGpxParser()
{
	m_AlterGlobalState = true;
	m_MemMiser = true;
	m_StripImgTags = false;
	m_pTextStore = 0;

	m_pCaches = &m_Caches;

	if (!pInst)
	{
		pInst = this;

		BuildXmlMap();
	}

	InitInternalState();
}

CGpxParser::~CGpxParser()
{
	CloseTextStore();

	Reset();

	if (pInst == this)
	{
		ResetMaps();

		pInst = 0;
	}
}

CGpxParser*	CGpxParser::GetInstance()
{
	return pInst;
}

void CGpxParser::SetInstance(CGpxParser* pNewInst)
{
	pInst = pNewInst;
}

GCCont* CGpxParser::SetCacheContainer(GCCont* pCont)
{
	GCCont* pTmp = m_pCaches;

	m_pCaches = pCont;

	return pTmp;
}

void CGpxParser::BuildXmlMap()
{
	MapElem("gpx", OnGpx);
	MapElem("wpt", OnWaypoint);
	MapElem("groundspeak:log", OnLogEntry);
	MapElem("groundspeak:travelbug", OnTravelBug);
}

void CGpxParser::MapAttr(const char* pElem, const char* pAttr, const char* pFormat, void* pVar)
{
	for (itAttr A = m_AttrMap.begin(); A != m_AttrMap.end(); A++)
	{
		if ((*A)->Match(pElem, pAttr))
		{
			(*A)->SetAddr(pVar);
			return;
		}
	}

	m_AttrMap.push_back(new CXmlAttr(pElem, pAttr, pFormat, pVar));
}

void CGpxParser::MapElem(const char* pElem, NoArgFunc pNoArgFunc)
{
	m_ElemMap.push_back(new CXmlElem(pElem, pNoArgFunc));
}

void CGpxParser::MapVal(const char* pElem, const char* pVal, const char* pFormat, void* pVar, bool bStoreToDisk)
{
	m_MappedElem = pElem;

	for (itVal V = m_ValMap.begin(); V != m_ValMap.end(); V++)
	{
		if ((*V)->Match(pElem, pVal))
		{
			(*V)->SetAddr(pVar, bStoreToDisk);
			return;
		}
	}

	m_ValMap.push_back(new CXmlVal(pElem, pVal, pFormat, pVar, bStoreToDisk));
}

void CGpxParser::OnGpx()
{
	pInst->MapAttr("gpx", "version", "%lf", (void*) &pInst->m_Version);
	pInst->MapAttr("gpx", "creator", "%s", (void*) &pInst->m_Creator);
	pInst->MapVal("gpx", "time", TIME_CONV, (void*) &pInst->m_CreationTime);
}

void CGpxParser::OnWaypoint()
{
	if (pInst->m_pTextStore == 0)
	{
		pInst->OpenTextStore();
	}

	pInst->m_pCurCache = new CGeoCache;

	if (!pInst->m_pCurCache)
	{
		CBaseException	Up;
		//throw Up;
	}

	pInst->m_pCaches->push_back(pInst->m_pCurCache);

	pInst->MapAttr("wpt", "lat", "%lf", (void*) &pInst->m_pCurCache->m_Lat);
	pInst->MapAttr("wpt", "lon", "%lf", (void*) &pInst->m_pCurCache->m_Long);

	pInst->MapVal("wpt", "time", TIME_CONV, (void*) &pInst->m_pCurCache->m_CreationTime);
	pInst->MapVal("wpt", "name", "%s", (void*) &pInst->m_pCurCache->m_Shortname);
	pInst->MapVal("wpt", "sym", "%s", (void*) &pInst->m_pCurCache->m_Sym);
	
	pInst->MapAttr("groundspeak:cache", "id", "%ld", (void*) &pInst->m_pCurCache->m_GsCacheId);
	pInst->MapAttr("groundspeak:cache", "available", BOOL_CONV, (void*) &pInst->m_pCurCache->m_GsCacheAvailable);
	pInst->MapAttr("groundspeak:cache", "archived", BOOL_CONV, (void*) &pInst->m_pCurCache->m_GsCacheArchived);

	pInst->MapVal("wpt", "groundspeak:name", "%s", (void*) &pInst->m_pCurCache->m_GsCacheName);
	
	// Alias for the cache name when using EasyGPS
	pInst->MapVal("wpt", "desc", "%s", (void*) &pInst->m_pCurCache->m_GsCacheName);
	
	pInst->MapVal("wpt", "groundspeak:placed_by", "%s", (void*) &pInst->m_pCurCache->m_GsCachePlacedBy);
	pInst->MapVal("wpt", "groundspeak:owner", "%s", (void*) &pInst->m_pCurCache->m_GsCacheOwnerName);
	pInst->MapVal("wpt", "groundspeak:type", "%s", (void*) &pInst->m_pCurCache->m_GsCacheType);
	pInst->MapVal("wpt", "groundspeak:container", "%s", (void*) &pInst->m_pCurCache->m_GsCacheContainer);
	pInst->MapVal("wpt", "groundspeak:difficulty", "%lf", (void*) &pInst->m_pCurCache->m_GsCacheDifficulty);
	pInst->MapVal("wpt", "groundspeak:terrain", "%lf", (void*) &pInst->m_pCurCache->m_GsCacheTerrain);
	pInst->MapVal("wpt", "groundspeak:country", "%s", (void*) &pInst->m_pCurCache->m_GsCacheCountry);
	pInst->MapVal("wpt", "groundspeak:state", "%s", (void*) &pInst->m_pCurCache->m_GsCacheState);

	pInst->MapAttr("groundspeak:short_description", "html", BOOL_CONV, (void*) &pInst->m_pCurCache->m_GsCacheShortDescIsHtml);
	pInst->MapVal("wpt", "groundspeak:short_description", "%s", (void*) &pInst->m_pCurCache->m_GsCacheShortDesc, pInst->m_MemMiser);

	pInst->MapAttr("groundspeak:long_description", "html", BOOL_CONV, (void*) &pInst->m_pCurCache->m_GsCacheLongDescIsHtml);
	pInst->MapVal("wpt", "groundspeak:long_description", "%s", (void*) &pInst->m_pCurCache->m_GsCacheLongDesc, pInst->m_MemMiser);

	pInst->MapVal("wpt", "groundspeak:encoded_hints", "%s", (void*) &pInst->m_pCurCache->m_GsCacheEncodedHints);
}

void CGpxParser::OnLogEntry()
{
	pInst->m_pCurCache->AddLogEntry(new CGeoCacheLogEntry);

	pInst->MapAttr("groundspeak:log", "id", "%ld", (void*) &pInst->m_pCurCache->m_pCurrCLE->m_Id);
	pInst->MapVal("log", "groundspeak:date", TIME_CONV, (void*) &pInst->m_pCurCache->m_pCurrCLE->m_Date);
	pInst->MapVal("log", "groundspeak:type", "%s", (void*) &pInst->m_pCurCache->m_pCurrCLE->m_Type);
	pInst->MapVal("log", "groundspeak:finder", "%s", (void*) &pInst->m_pCurCache->m_pCurrCLE->m_FinderName);
	pInst->MapAttr("groundspeak:text", "encoded", BOOL_CONV, (void*) &pInst->m_pCurCache->m_pCurrCLE->m_TextEncoded);
	pInst->MapVal("log", "groundspeak:text", "%s", (void*) &pInst->m_pCurCache->m_pCurrCLE->m_Text, pInst->m_MemMiser);
	pInst->MapAttr("groundspeak:log_wpt", "lat", "%lf", (void*) &pInst->m_pCurCache->m_pCurrCLE->m_Lat);
	pInst->MapAttr("groundspeak:log_wpt", "lon", "%lf", (void*) &pInst->m_pCurCache->m_pCurrCLE->m_Long);
}

void CGpxParser::OnTravelBug()
{
	pInst->m_pCurCache->AddTravelBug(new CTravelBug);

	pInst->MapAttr("groundspeak:travelbug", "id", "%ld", (void*) &pInst->m_pCurCache->m_pCurrTB->m_Id);
	pInst->MapAttr("groundspeak:travelbug", "ref", "%s", (void*) &pInst->m_pCurCache->m_pCurrTB->m_Ref);
	pInst->MapVal("tb", "groundspeak:travelbug", "%s", (void*) &pInst->m_pCurCache->m_pCurrTB->m_Name);
}

void CGpxParser::InitInternalState()
{
	m_pCurCache = 0;
	memset(&m_CreationTime,sizeof(m_CreationTime), 0);
}

// Attempts to load and parse a GPX file. Throws an exception on failure.
// Returns an enumerated type indicating the status of the load.
GpxLoadStatus CGpxParser::Load(const String& GpxFile, bool AlterGlobalState)
{
	AW_CONVERSION;

	GpxLoadStatus	Status = GpxLoadStatusOk;
	XML_Parser		XP;
	TCHAR			Err[10];
	FILE*			fd = NULL;
	unzFile			zfd = NULL;
	unz_file_info	FileInfo;
	char			Filename[256];
	bool			IsZip = false;
	int				rc = 0;

	// Flag protecting global resources 
	m_AlterGlobalState = AlterGlobalState;

	if (GpxFile.rfind(_T(".zip"), GpxFile.size()) != -1 || GpxFile.rfind(_T(".ZIP"), GpxFile.size()) != -1)
	{
		IsZip = true;
	}

	InitInternalState();

	Reset();

	if (IsZip)
	{
		char* pFN = w2a((TCHAR*)GpxFile.c_str());

		// Open the ZIP file
		zfd = unzOpen(pFN);

		if (zfd == NULL) 
		{
			m_Error = _T("Cannot open ZIP file: ");
			m_Error += GpxFile;
			m_Error += _T(" for reading\n");

			return GpxLoadStatusFailed;
		}

		// Find the first compressed file inside of the ZIP file
		rc = unzGoToFirstFile(zfd);

		if (UNZ_OK != rc)
		{
			m_Error = _T("Couldn't find first compressed file in ");
			m_Error += GpxFile;
			m_Error += _T("\n");

			unzClose(zfd);

			return GpxLoadStatusFailed;
		}

		rc = unzGetCurrentFileInfo(zfd,&FileInfo,Filename,sizeof(Filename),NULL,0,NULL,0);

		if (UNZ_OK != rc)
		{
			TCHAR Err[10];

			_stprintf(Err,_T("%d\n"),rc);

			m_Error = _T("Couldn't get file info on file \'");
			m_Error += a2w(Filename);
			m_Error += _T("\' in ");
			m_Error += GpxFile;
			m_Error += _T(". Error # ");
			m_Error += Err;

			unzClose(zfd);

			return GpxLoadStatusFailed;
		}

		rc = unzOpenCurrentFile(zfd);

		if (UNZ_OK != rc)
		{
			TCHAR Err[10];

			_stprintf(Err,_T("%d\n"),rc);

			m_Error = _T("Couldn't open file \'");
			m_Error += a2w(Filename);
			m_Error += _T("\' in ");
			m_Error += GpxFile;
			m_Error += _T(". Error # ");
			m_Error += Err;

			unzClose(zfd);

			return GpxLoadStatusFailed;
		}
	}
	else
	{
		fd = _tfopen(GpxFile.c_str(), _T("rb"));

		if (fd == NULL) 
		{
			m_Error = _T("Cannot open ");
			m_Error += GpxFile;
			m_Error += _T(" for reading\n");

			return GpxLoadStatusFailed;
		}
	}

	XP = XML_ParserCreate(NULL);

	if (!XP) 
	{
		m_Error = _T("Cannot create XML Parser\n");

		if (IsZip)
		{
			unzClose(zfd);
		}
		else
		{
			fclose(fd);
		}

		return GpxLoadStatusFailed;
	}

	XML_SetElementHandler(XP, StartElement, EndElement);
	XML_SetCharacterDataHandler(XP, CData);

	#define CHAR_BUFFER				1024 * 32
	#define	SIZEOF_BUFFER_MINUS_1	CHAR_BUFFER - 1
	#define	MAX_READ_LENGTH			SIZEOF_BUFFER_MINUS_1 - 1
	
	long	LineCount = 0;
	int		len = 0;
	int		done = 0;
	char	PrecedingChar = 0;
	char	buf[CHAR_BUFFER];

	char*	pEnd = buf + MAX_READ_LENGTH;

	// Terminate the buffer with a null
	buf[SIZEOF_BUFFER_MINUS_1] = 0;

	while (!done) 
	{
		if (IsZip)
		{
            len = unzReadCurrentFile(zfd,buf,MAX_READ_LENGTH);

			if (!len)
			{
				done = true;
				continue;
			}

            if (len < 0)
            {
				_stprintf(Err, _T("%d\n"), len);

				m_Error = _T("Failed to read from \'");
				m_Error += a2w(Filename);
				m_Error += _T("\' in ");
				m_Error += GpxFile;
				m_Error += _T(". Error # ");
				m_Error += Err;

				Status = GpxLoadStatusFailed;

				done = true;
				continue;
            }
		}
		else
		{
			len = fread(buf, 1, MAX_READ_LENGTH, fd);
			done = feof(fd) || !len; 

			if (!len)
			{
				done = true;
				continue;
			}

			// TO TO : add file read error handler
		}

		char* pBad = buf;

		// Filter out bad characters
		while (true)
		{
			pBad = strstr(pBad, "&#");

			if (pBad)
			{
				pBad[0] = ' ';
				pBad[1] = ' ';

				pBad += 2;

				if (pBad >= pEnd)
				{
					break;
				}

				continue;
			}

			break;
		}

		// Check for a '#' symbol following a '&' symbol within 2 consecutive reads
		if (buf[0] == '#' && PrecedingChar == '&')
		{
			buf[0] = 0;
		}

		// Remember the last character of the buffer.
		PrecedingChar = buf[MAX_READ_LENGTH];

		if (!XML_Parse(XP, buf, len, done)) 
		{
			#define	MAX_XML_ERROR_BUFFER 200
			
			TCHAR Buffer[MAX_XML_ERROR_BUFFER];
			
			_sntprintf(Buffer, MAX_XML_ERROR_BUFFER, _T("XML parser error at %ld: %s\n"), XML_GetCurrentLineNumber(XP) + LineCount, a2w(XML_ErrorString(XML_GetErrorCode(XP))));
			
			m_Error = Buffer;
			
			Status = GpxLoadStatusParserException;

			done = true;
			continue;
		}

		LineCount += XML_GetCurrentLineNumber(XP);
	}

	if (zfd)
	{
		unzCloseCurrentFile (zfd);
		unzClose(zfd);
	}

	if (fd)
	{
		fclose(fd);
	}

	if (XP)
	{
		XML_ParserFree(XP);
	}

	if (Status == GpxLoadStatusOk)
	{
		// Build the Country / State maps
		UpdateBuiltInMaps();
	}

	return Status;
}

// Build the Country / State maps
void CGpxParser::UpdateBuiltInMaps()
{
	if (!m_AlterGlobalState)
	{
		return;
	}

	m_CountryList.clear();
	m_StateList.clear();

	// Rebuild the Country map
	for (itGC I = m_pCaches->begin(); I != m_pCaches->end(); I++)
	{
		CGeoCache* pGc = (*I);

		bool Found = false;

		itStr C;

		for (C = m_CountryList.begin(); C != m_CountryList.end(); C++)
		{
			if ((*C) == pGc->m_GsCacheCountry)
			{
				Found = true;
				break;
			}
		}

		if (!Found)
		{
			m_CountryList.push_back(pGc->m_GsCacheCountry);
		}

		Found = false;

		for (C = m_StateList.begin(); C != m_StateList.end(); C++)
		{
			if ((*C) == pGc->m_GsCacheState)
			{
				Found = true;
				break;
			}
		}

		if (!Found)
		{
			m_StateList.push_back(pGc->m_GsCacheState);
		}
	}
}

static CGeoCache DummyCache;

// Returns a ptr to the first cache in the list. The iterator is set to end() at the end of the list.
CGeoCache* CGpxParser::First(itGC& it)
{
	it = m_pCaches->begin();

	if (it != m_pCaches->end())
	{
		return (*it);
	}

	return &DummyCache;
}

// Returns a ptr to the next cache in the list. The iterator is set to end() at the end of the list.
CGeoCache* CGpxParser::Next(itGC& it)
{
	it++;

	if (it != m_pCaches->end())
	{
		return (*it);
	}

	return &DummyCache;
}

// Returns 'true' if the iterator is at the end() of the list
bool CGpxParser::EndOfCacheList(itGC& it)
{
	if (it != m_pCaches->end())
	{
		return false;
	}

	return true;
}

// Returns the # of caches loaded
long CGpxParser::CacheCount()
{
	return m_pCaches->size();
}

// Cleanup all allocated caches
void CGpxParser::Reset()
{
	for (itGC I = m_pCaches->begin(); I != m_pCaches->end(); I++)
	{
		delete *I;
	}

	m_pCaches->clear();
}

void CGpxParser::ResetMaps()
{
	for (itAttr A = m_AttrMap.begin(); A != m_AttrMap.end(); A++)
	{
		delete *A;
	}

	m_AttrMap.clear();

	for (itVal V = m_ValMap.begin(); V != m_ValMap.end(); V++)
	{
		delete *V;
	}

	m_ValMap.clear();

	for (itElem E = m_ElemMap.begin(); E != m_ElemMap.end(); E++)
	{
		delete *E;
	}

	m_ElemMap.clear();
}

void CGpxParser::StartElement(void *data, const char *el, const char **attr)
{
	#define MIN_LENGTH	512;

	pInst->m_CData.Clear();

	// For each new element, check against the Element map
	for (itElem E = m_ElemMap.begin(); E != m_ElemMap.end(); E++)
	{
		// Call any necessary function associated with the element
		if ((*E)->CallOnMatch(el))
		{
			// If a call took place, get out of the loop
			break;
		}
	}

	data = (void*) &pInst->m_Version;

	// Check if there are attributes of interest for this element
	for (itAttr A = m_AttrMap.begin(); A != m_AttrMap.end(); A++)
	{
		if ((*A)->MatchElem(el))
		{
			// Yes, attribute(s) need to be stored for this element
			
			// For each attribute check if its value needs to be stored
			const char **avp = &attr[0];
			
			while (*avp) 
			{
				// Run through the attributes map
				for (A = m_AttrMap.begin(); A != m_AttrMap.end(); A++)
				{
					// looking for the object matching the element/attribute combo
					if ((*A)->Match(el, avp[0]))
					{
						// got it! store the value
						(*A)->Store(avp[1],0,strlen(avp[1]));

						// no need to continue looking, so bail...
						break;
					}
				}
				avp+=2;
			}

			// No need to continue, so bail...
			break;
		}
	}
}

void CGpxParser::EndElement(void *data, const char *el)
{
	// Check if we have any mapped values for this element
	for (itVal V = m_ValMap.begin(); V != m_ValMap.end(); V++)
	{
		if ((*V)->Match(pInst->m_MappedElem.c_str(),el))
		{
			(*V)->Store(*pInst->m_CData, pInst->GetTextStoreOffset(), pInst->m_CData.Size());

			if ((*V)->StoredToDisk())
			{
				pInst->WriteTextStore((BYTE*) *pInst->m_CData, pInst->m_CData.Size());
			}

			break;
		}
	}
}

void CGpxParser::CData(void *data, const XML_Char *s, int len)
{
	pInst->m_CData.Cat(s, len);
}

// Open the text store file
void CGpxParser::OpenTextStore()
{
	if (!m_AlterGlobalState)
	{
		return;
	}

	InitTextStore();

	CloseTextStore();

	CPath	Path;

	m_pTextStore = _tfopen(Path.BuildPath(TEXT_STORE).c_str(), _T("w+b"));
}

// Initialize the variables related to the Text Store
void CGpxParser::InitTextStore()
{
	m_WriteTextStoreOffset = 0;
}

// Close the text store
void CGpxParser::CloseTextStore()
{
	if (!m_AlterGlobalState)
	{
		return;
	}

	if (m_pTextStore)
	{
		fclose(m_pTextStore);
		m_pTextStore = 0;
	}
}

// Get the position where the next write I/O will take place
long CGpxParser::GetTextStoreOffset()
{
	return m_WriteTextStoreOffset;
}

// Write a block of text to the store
void CGpxParser::WriteTextStore(BYTE* pData, long Length)
{
	if (!m_AlterGlobalState)
	{
		return;
	}

	if (m_pTextStore)
	{
		fwrite(pData, sizeof(char), Length, m_pTextStore);

		m_WriteTextStoreOffset += Length;
	}
}

// Read a block of text from the store based on the content of the location passed as a string
char* CGpxParser::ReadFromTextStore(String& Location)
{
	if (m_pTextStore)
	{
		#define	MAX_LOCATION_SIZE 50

		TCHAR Loc[MAX_LOCATION_SIZE];

		// Make a copy of the location string before parsing it (strtok would corrupt the STL string)
		_tcsncpy(Loc, Location.c_str(), sizeof(Loc)/sizeof(TCHAR));

		//
		// Parse the content of the location to extract the offset and the length
		//

		// Skip the canary value
		_tcstok(Loc, _T("&"));

		// Extract the Offset value
		long Offset = _ttol(_tcstok(NULL, _T("&")));
		// Extract the Length value
		long Length = _ttol(_tcstok(NULL, _T("&")));

		if (!Length)
		{
			return "";
		}

		char* pText = new char[Length+1];

		if (!fseek(m_pTextStore, Offset, SEEK_SET))
		{
			long Read = fread(pText, sizeof(char), Length, m_pTextStore);

			pText[Read] = 0;
		}

		return pText;
	}

	return "Failed";
}

// Serialize some state information used by the parser to handle the text store
void CGpxParser::Serialize(CStream& ar)
{
	#define CGpxParserVersion	101

	if (ar.IsStoring())
	{
		ar << CGpxParserVersion;
		ar << m_MemMiser;
		ar << m_StripImgTags;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_MemMiser;

			// Override whatever was retrieved from the config stream!!
			m_MemMiser = true;
		}

		if (Version >= 101)
		{
			ar >> m_StripImgTags;
		}
	}
}

// Sort the caches according to their distance from the center
void CGpxParser::SortByDistance()
{
	sort( m_pCaches->begin(), m_pCaches->end(), SortByDistanceImpl);
}

// Function used to sort caches by proximity
bool CGpxParser::SortByDistanceImpl(CGeoCache* pFirst, CGeoCache* pSecond)
{
	if (pFirst->m_Distance < pSecond->m_Distance)
	{
		return true;
	}

	return false;
}

// Enables / Disables stripping <IMG SRC=""> tags
void CGpxParser::SetStripImgTags(bool StripImgTags)
{
	m_StripImgTags = StripImgTags;
}

bool CGpxParser::GetStripImgTags()
{
	return m_StripImgTags;
}

// Marks all caches as "out-of-scope" except one
void CGpxParser::MarkAllAsOutOfScopeExceptOne(CGeoCache* pException)
{
	for (itGC I = m_pCaches->begin(); I != m_pCaches->end(); I++)
	{
		CGeoCache* pC = (*I);
		
		// Backup the scope
		pC->m_InScopeBackup = pC->m_InScope;

		// Change the scope of the cache to 'out' except if it's the one we're passing as the exception
		if (pC == pException)
		{
			pC->m_InScope = true;
		}
		else
		{
			pC->m_InScope = false;
		}
	}
}

// Restores the original scope of the caches after a call to MarkAllAsOutOfScopeExceptOne()
void CGpxParser::RestoreScope()
{
	for (itGC I = m_pCaches->begin(); I != m_pCaches->end(); I++)
	{
		CGeoCache* pC = (*I);
		
		// Restore the scope
		pC->m_InScope = pC->m_InScopeBackup;
	}
}
