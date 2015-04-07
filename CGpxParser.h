#ifndef _CGpxParser
	#define _CGpxParser

#include "CommonDefs.h"
#include "CXmlMap.h"
#include ".\Expat\expat.h"
#include "CDynStr.h"

typedef enum {
	GT_NotInitialized = -1,
	GT_Traditional = 0,
	GT_Multi,
	GT_Virtual,
	GT_Webcam,
	GT_Unknown,
	GT_LetterboxHybrid,
	GT_Event,
	GT_ProjectAPE,
	GT_Locationless,
	GT_CITO,
	GT_Earthcache,
	GT_MUST_BE_LAST
} GcType;

#define NO_CATEGORY					_T("<None>")

#define TYPE_MULTI_DISP				_T("Multi-Stage Cache")
#define TYPE_UNKNOWN_DISP			_T("Mystery/Puzzle Cache")
#define TYPE_LETTERBOXHYBRID_DISP	_T("Letterbox (Hybrid)")
#define TYPE_EVENT_DISP				_T("Geocaching Event Cache")

#define TYPE_TRADITIONAL			_T("Traditional Cache")
#define TYPE_MULTI					_T("Multi-cache")
#define TYPE_VIRTUAL				_T("Virtual Cache")
#define TYPE_WEBCAM					_T("Webcam Cache")
#define TYPE_UNKNOWN				_T("Unknown Cache")
#define TYPE_LETTERBOXHYBRID		_T("Letterbox Hybrid")
#define TYPE_EVENT					_T("Event Cache")
#define TYPE_PROJECTAPE				_T("Project APE Cache")
#define TYPE_LOCATIONLESS			_T("Locationless (Reverse) Cache")
#define TYPE_CITO					_T("Cache In Trash Out Event")
#define TYPE_EARTHCACHE				_T("Earthcache")

typedef enum {
	GC_Unknown = 0,
	GC_Micro,
	GC_Small,
	GC_Regular,
	GC_Large,
	GC_Virtual,
	GC_NotListed
} GcContainer;

#define CONT_UNKNOWN	_T("Unknown")
#define CONT_MICRO		_T("Micro")
#define CONT_SMALL		_T("Small")
#define CONT_REGULAR	_T("Regular")
#define CONT_LARGE		_T("Large")
#define CONT_VIRTUAL	_T("Virtual")
#define CONT_NOTLISTED	_T("Not Listed")

typedef enum {
	LOG_Unknown = 0,
	LOG_FoundIt,
	LOG_DidntFindIt,
	LOG_NeedsArchived,
	LOG_ArchivedNoShow,
	LOG_ArchivedShow,
	LOG_Other,
	LOG_Camera,
	LOG_EnableListing,
	LOG_TemporarilyDisableListing
} GcLogType;

class CTravelBug
{
public:
	long	m_Id;
	String	m_Ref;
	String	m_Name;

public:
	CTravelBug();
};

typedef vector<CTravelBug*> TBCont;
typedef vector<CTravelBug*>::iterator itTB;

typedef map<String, GcLogType> LogTypeMap;
typedef map<String, GcLogType>::iterator itLogTypeMap;

class CGeoCacheLogEntry
{
	static LogTypeMap	m_LogTypeMap;

public:
	double		m_Lat;
	double		m_Long;
	long		m_Id;
	SYSTEMTIME	m_Date;
	String		m_Type;
	String		m_FinderName;
	String		m_Text;
	bool		m_TextEncoded;

public:
	CGeoCacheLogEntry();

	// Returns an enumerated value for the string type of the log
	GcLogType TypeLookup();
};

typedef vector<CGeoCacheLogEntry*> GCLogEntryCont;
typedef vector<CGeoCacheLogEntry*>::iterator itGCLogEntry;

typedef map<String, GcType> CacheTypeMap;
typedef map<String, GcType>::iterator itCacheTypeMap;

typedef map<String, GcContainer> CacheContainerMap;
typedef map<String, GcContainer>::iterator itCacheContainerMap;

class CFieldNote;
class CWPMgr;
class CStream;

class CGeoCache
{
public:
	// Index of the cache in the list
	int			m_ListIndex;

	double		m_Lat;
	double		m_Long;
	SYSTEMTIME	m_CreationTime;
	String		m_Shortname;

	// 'true' when the object falls within the scope of the filter(s)
	bool		m_InScope;
	// Backs up the In-scope state
	bool		m_InScopeBackup;

	// 'true' when the cache is ignored
	bool		m_Ignored;

	// GroundSpeak Extensions
	GcType		m_GcType;
	bool		m_GsCacheAvailable;
	bool		m_GsCacheArchived;
	bool		m_GsCacheShortDescIsHtml;
	bool		m_GsCacheLongDescIsHtml;
	long		m_GsCacheId;
	double		m_Distance;
	double		m_GsCacheDifficulty;
	double		m_GsCacheTerrain;
	String		m_Sym;
	String		m_GsCacheType;
	String		m_GsCacheContainer;
	String		m_GsCacheName;
	String		m_GsCachePlacedBy;
	String		m_GsCacheOwnerName;
	String		m_GsCacheCountry;
	String		m_GsCacheState;
	String		m_GsCacheShortDesc;
	String		m_GsCacheLongDesc;
	String		m_GsCacheEncodedHints;
	String		m_Bearing;
	String		m_Category;

	// !0 if a note has been associated with the cache
	CFieldNote*	m_pFieldNote;

	// Pointer to the latest log entry
	CGeoCacheLogEntry*	m_pCurrCLE;

	// Pointer to the latest travel bug
	CTravelBug*			m_pCurrTB;

	// Pointer to a list of waypoints associated with the cache (if owned only)
	CWPMgr*				m_WpMgr;

private:
	GCLogEntryCont				m_Logs;
	TBCont						m_TBs;
	static CacheTypeMap			m_CacheTypeMap;
	static CacheContainerMap	m_CacheContainerMap;

public:
	CGeoCache();
	virtual ~CGeoCache();

	// Add a new log entry to the cache. The last log entry to be added is made the current one and it can be accessed through CurrentLogEntry().
	void						AddLogEntry(CGeoCacheLogEntry* pLogEntry);

	// Add a new travel bug to the cache. The last travel bug entry to be added is made the current one and it can be accessed through CurrentTravelBug().
	void						AddTravelBug(CTravelBug* pTB);

	// Remove a travel bug from the cache
	void						RemoveTravelBug(CTravelBug* pTB);

	// Return a pointer to the current log entry or 0 if none were added.
	CGeoCacheLogEntry*			CurrentLogEntry();

	// Return a pointer to the current travel bug or 0 if none were added
	CTravelBug*					CurrentTravelBug();


	// Returns a pointer to the first log entry in the list. The iterator is set to end() at the end of the list.
	CGeoCacheLogEntry*			FirstLog(itGCLogEntry& it);

	// Returns a pointer to the next log entry in the list. The iterator is set to end() at the end of the list.
	CGeoCacheLogEntry*			NextLog(itGCLogEntry& it);

	// Returns 'true' if the iterator is at the end() of the list
	bool						EndOfLogList(itGCLogEntry& it);

	// Return a text version of the date when the cache was last found
	void						LastFoundText(TCHAR* pBuffer);

	// Returns a pointer to the first travel bug in the list. The iterator is set to end() at the end of the list.
	CTravelBug*					FirstTB(itTB& it);

	// Returns a pointer to the next travel bug in the list. The iterator is set to end() at the end of the list.
	CTravelBug*					NextTB(itTB& it);

	// Returns 'true' if the iterator is at the end() of the list
	bool						EndOfTBList(itTB& it);

	// Returns the # of TBs in the cache
	long						GetTBCount();

	// Returns an enumerated value for the string type of the cache
	GcType						TypeLookup();

	// Returns an enumerated value for the string container of the cache
	GcContainer					ContainerLookup();

	// Returns the 'this' pointer for the object
	CGeoCache*					This();

	// Serialize the cache to a stream
	void						Serialize(CStream& ar);

	// Resets the cache content
	void						Reset();

	// Empties the list of pointers to TBs and Logs
	void						ForgetTBsAndLogs();

private:
	void InitializeInternalState();

	// Removes all log entries
	void						LogReset();

	// Removes all Travel Bug entries
	void						TBReset();
};

typedef vector<CGeoCache*> GCCont;
typedef vector<CGeoCache*>::iterator itGC;

typedef enum {
	GpxLoadStatusFailed = 0,
	GpxLoadStatusOk,
	GpxLoadStatusParserException
} GpxLoadStatus;

class CGpxParser
{
private:
	GCCont		m_Caches;

	GCCont*		m_pCaches;

	// This file is used to store text which would normally be in memory. It is accessed based on an offset from the beginning of the file + a length
	FILE*		m_pTextStore;
	long		m_WriteTextStoreOffset;
	CGeoCache*	m_pCurCache;
	CDynStr		m_CData;
	double		m_Version;
	String		m_GpxFilename;
	String		m_Creator;
	String		m_Error;
	string		m_MappedElem;
	SYSTEMTIME	m_CreationTime;

	// This switch controls the storage to the TextStore.
	bool		m_MemMiser;
	bool		m_StripImgTags;
	bool		m_AlterGlobalState;

	static AttrCont	m_AttrMap;
	static ValCont	m_ValMap;
	static ElemCont	m_ElemMap;

public:
	StringList	m_CountryList;
	StringList	m_StateList;

public:
	CGpxParser();
	virtual		~CGpxParser();

	// Attempts to load and parse a GPX file. Throws an exception on failure.
	GpxLoadStatus	Load(const String& GpxFile, bool AlterGlobalState = true);

	// Returns a ptr to the first CGeoCache in the list. The iterator is set to end() at the end of the list.
	CGeoCache*	First(itGC& it);

	// Returns a ptr to the next CGeoCache in the list. The iterator is set to end() at the end of the list.
	CGeoCache*	Next(itGC& it);

	// Returns 'true' if the iterator is at the end() of the list
	bool		EndOfCacheList(itGC& it);

	// Returns the # of caches loaded
	long		CacheCount();

	void		Reset();

	// Serialize some state information used by the parser to handle the text store
	void		Serialize(CStream& ar);

	// Read a block of text from the store based on the content of the location passed as a string
	char*		ReadFromTextStore(String& Location);

	// Returns the text msg of the last error
	String		GetErrorMsg()
	{
		return m_Error;
	}

	CGpxParser*	GetInstance();
	void		SetInstance(CGpxParser* pNewInst);

	// Set the pointer to the current cache container. Returns the pointer to the old one.
	GCCont*		SetCacheContainer(GCCont* pCont);

	// Sort the caches according to their distance from the center
	void		SortByDistance();

	// Enables / Disables stripping <IMG SRC=""> tags
	void		SetStripImgTags(bool StripImgTags);
	bool		GetStripImgTags();

	// Marks all caches as "out-of-scope" except one
	void		MarkAllAsOutOfScopeExceptOne(CGeoCache* pException);
	// Restores the original scope of the caches after a call to MarkAllAsOutOfScopeExceptOne()
	void		RestoreScope();

private:
	void ResetMaps();

	void InitInternalState();

	// Build the Country / State maps
	void UpdateBuiltInMaps();

	void BuildXmlMap();

	void MapAttr(const char* pElem, const char* pAttr, const char* pFormat, void* pVar);
	void MapElem(const char* pElem, NoArgFunc pNoArgFunc);
	void MapVal(const char* pElem, const char* pVal, const char* pFormat, void* pVar, bool bStoreToDisk = false);

	static	void	OnGpx();
	static	void	OnWaypoint();
	static	void	OnLogEntry();
	static	void	OnTravelBug();

	static	void	StartElement(void *data, const char *el, const char **attr);
	static	void	EndElement(void *data, const char *el);
	static	void	CData(void *dta, const XML_Char *s, int len);

	// Initialize the variables related to the Text Store
	void			InitTextStore();
	// Open the text store file
	void			OpenTextStore();
	// Close the text store
	void			CloseTextStore();
	// Get the position where the next write I/O will take place
	long			GetTextStoreOffset();
	// Write a block of text to the store
	void			WriteTextStore(BYTE* pData, long Length);

	// Function used to sort caches by proximity
	static	bool	SortByDistanceImpl(CGeoCache* pFirst, CGeoCache* pSecond);
};

#endif