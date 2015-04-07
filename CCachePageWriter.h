#ifndef _INC_CCachePageWriter
	#define _INC_CCachePageWriter

#include <vector>

using namespace std;

class CGpxParser;
class CGeoCache;
class CCachePageWriterHandler;

typedef enum {
	GcRatingDifficulty = 0,
	GcRatingTerrain
} GcRatingType;

// This class describes a portion of an HTML template. Once all the parts have been used and all the writer handlers called, 
// the HTML page is complete and can be displayed.
class CHtmlSeg
{
public:
	string						m_HtmlSeg;
	CCachePageWriterHandler*	m_pH;

public:
	CHtmlSeg(char* pHtmlSeg, CCachePageWriterHandler* pH);

	// Format text according to the segment definition
	string	Format(CGpxParser& Parser, CGeoCache& Cache);

	// Format integers according to the segment definition
	string	FormatStats(int NotesFound, int NotesDNF, int NotesNote, int NotesArchive);

protected:
	// Creates a string of star bitmaps matching the given rating
	string	WriteStars(double Rating, GcRatingType Type);

	// Returns the filename of the icon matching the log type
	string	LogIconLookup(int LogType);

	// Returns the filename of the icon matching the status of the field note
	string	FieldNoteStatusLookup(int Status);

	// Searches for markup markers and returns a complete string with substitutions
	string	ScanForMarkup(char* pText, int Length);

	// Performs lookups on commonly used tags in logs found on GC.com
	string	MarkupLookup(char* pMarkup);

	// Replace spaces with '_' in cache type bitmaps
	wstring	SpaceToUnderscore(wstring& Str);

	// Strip <IMG SRC=""> tags from HTML cache descriptions
	void	StripImgTag(char* pTag);
};

typedef vector<CHtmlSeg*> HtmlSegCont;
typedef vector<CHtmlSeg*>::iterator itHtmlSeg;

// This class is used as a specialized handler for particular template segments which require more processing
class CCachePageWriterHandler
{
public:
	CCachePageWriterHandler();

	// Virtual function called for each tag in the template associated with a writer handler
	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

// Writes out travel bug label if travel bugs exist in the cache
class CTBLabelWriter : public CCachePageWriterHandler
{
public:
	CTBLabelWriter()
	{}

	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

// Writes out travel bug records
class CTBWriter : public CCachePageWriterHandler
{
public:
	CTBWriter()
	{}

	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

class CWarningWriter : public CCachePageWriterHandler
{
public:
	CWarningWriter()
	{}

	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

class CLogWriter : public CCachePageWriterHandler
{
public:
	CLogWriter()
	{}

	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

class CHintsWriter : public CCachePageWriterHandler
{
public:
	CHintsWriter()
	{}

	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

class CSpoilerPicsWriter : public CCachePageWriterHandler
{
protected:
	wstring			m_SpoilerPicsPath;

public:
	CSpoilerPicsWriter()
	{}

	void			SetSpoilerPicsPath(const wstring& SpoilerPicsPath);
	virtual string	OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};


class CHintsLabelWriter : public CCachePageWriterHandler
{
public:
	CHintsLabelWriter()
	{}

	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

class CReportWriter : public CCachePageWriterHandler
{
public:
	CReportWriter()
	{}

	virtual string OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg);
};

class CCacheReportsPref;
class CFieldNoteMgr;

// Produces an HTML page describing a cache using a templatized document broken down into segments associated with data members of a 
// cache object.
class CCachePageWriter
{
private:
	HtmlSegCont	m_Segs;
	
	CTBLabelWriter		m_TBLabelWriter;
	CTBWriter			m_TBWriter;
	CLogWriter			m_LogWriter;
	CHintsWriter		m_HintsWriter;
	CHintsLabelWriter	m_HintsLabelWriter;
	CReportWriter		m_ReportWriter;
	CWarningWriter		m_WarningWriter;

public:
	CSpoilerPicsWriter	m_SpoilerPicsWriter;

public:
	CCachePageWriter();
	~CCachePageWriter();

	void	LoadTemplate(const TCHAR* pTplPath);

	// Register the various parts of the HTML page to be generated
	void	AddSeg(char* pHtmlSeg, CCachePageWriterHandler* pH = 0);

	// Write the cache page out
	void	Write(CGpxParser& Parser, CGeoCache& Cache, TCHAR* pFilePath);

	// Write a report page out
	void	WriteReport(CCacheReportsPref* pPref, CFieldNoteMgr* pFNM, CGpxParser& Parser, TCHAR* pFilePath);

	// General cleanup
	void	Reset();
};

#endif
