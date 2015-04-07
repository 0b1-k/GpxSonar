#ifndef _INC_CExportLocationMgr
	#define _INC_CExportLocationMgr

#include "CommonDefs.h"
#include "CStream.h"
#include <vector>

class CExportLocationMgr;

class CExportLoc
{
public:
	String			m_Name;
	String			m_FullPath;
	String			m_UnUsed;

public:
	void	Serialize(CStream& ar);
};

typedef vector<CExportLoc*> ExportLocations;
typedef vector<CExportLoc*>::iterator itExportLoc;

class CGpxParser;
class CFieldNoteMgr;

class CExportLocationMgr
{
protected:
	String				m_GpxFilename;
	CFieldNoteMgr*		m_pFieldNoteMgr;
	long				m_Count;
	bool				m_Limited;
	CGpxParser*			m_pGpxParser;

public:

	ExportLocations		m_ExpLocs;
	bool				m_ExportsNamesAsGpxName;
	int					m_ExportMaxLimit;

public:
	CExportLocationMgr();
	~CExportLocationMgr();

	void		SetGpxParser(CGpxParser*);
	CGpxParser*	GetGpxParser();

	void		SetFieldNoteMgr(CFieldNoteMgr*);

	void		SetGpxFilename(const String& GpxFilename);

	String		GetFilename(TCHAR* Name);

	CExportLoc*	FindLocation(TCHAR* Name);

	void		Serialize(CStream& ar);

	long		GetExportedCount();
	bool		GetWasExportLimited();

	void		Export(CExportLoc*);

protected:
	String		ProcessFilename(CExportLoc* pEL);
	void		AddDefaultLocationsByType();
	void		AddLocation(TCHAR* Name, wstring& Path, TCHAR* Filename);
	void		Reset();

	void		ShowExportMsg(bool Limited, long Count, String& File);
	void		CommonMagellanExports(CExportLoc* pEL);

	// Export functions
	void		MagellanMeridian(CExportLoc* pEL);
	void		MagellanExplorist(CExportLoc* pEL);
	void		Loc(CExportLoc* pEL);
	void		GSAK(CExportLoc* pEL);
	void		PharosOstia6(CExportLoc* pEL);
	void		MSFTSTPushpins(CExportLoc* pEL);
	void		MaptechMXF(CExportLoc* pEL);
	void		CSV(CExportLoc* pEL);
	void		LowranceiFinder(CExportLoc* pEL);
	void		OziExplorer(CExportLoc* pEL);
	void		NavigonMobileNavigator4(CExportLoc* pEL);
	void		BoulterExpressLogger(CExportLoc* pEL);
};

#endif
