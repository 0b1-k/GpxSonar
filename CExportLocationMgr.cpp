#include "CExportLocationMgr.h"
#include "Literals.h"
#include "CPath.h"

#include "CPharosOstia6Writer.h"
#include "CPocketStreetPushPinsWriter.h"
#include "CMxfFileWriter.h"
#include "CCsvFileWriter.h"
#include "CLowranceWriter.h"
#include "COziWriter.h"
#include "CLocFileWriter.h"
#include "CMeridianWriter.h"
#include "CNavigonWriter.h"

#include "CGpxParser.h"
#include "CFieldNoteMgr.h"
#include "CBrowserLauncher.h"

//------------------------------------------------------------------------------------
void CExportLoc::Serialize(CStream& ar)
{
	#define CExportLocVersion 100

	if (ar.IsStoring())
	{
		ar << CExportLocVersion;
		ar << m_Name;
		ar << m_FullPath;
		ar << m_UnUsed;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Name;
			ar >> m_FullPath;
			ar >> m_UnUsed;
		}
	}
}

//------------------------------------------------------------------------------------
CExportLocationMgr::CExportLocationMgr()
{
	m_pFieldNoteMgr = 0;
	m_ExportsNamesAsGpxName = false;
	m_ExportMaxLimit = 500;
	m_Count = 0;
	m_Limited = false;
	m_pGpxParser = 0;

	AddDefaultLocationsByType();
}

CExportLocationMgr::~CExportLocationMgr()
{
	Reset();
}

void CExportLocationMgr::SetFieldNoteMgr(CFieldNoteMgr* pFNM)
{
	m_pFieldNoteMgr = pFNM;
}

void CExportLocationMgr::SetGpxParser(CGpxParser* pGpxParser)
{
	m_pGpxParser = pGpxParser;
}

CGpxParser*	CExportLocationMgr::GetGpxParser()
{
	return m_pGpxParser;
}

long CExportLocationMgr::GetExportedCount()
{
	return m_Count;
}

bool CExportLocationMgr::GetWasExportLimited()
{
	return m_Limited;
}

void CExportLocationMgr::SetGpxFilename(const String& GpxFilename)
{
	m_GpxFilename = GpxFilename;
}

String CExportLocationMgr::GetFilename(TCHAR* Name)
{
	CExportLoc*	pEL = FindLocation(Name);

	if (pEL)
	{
		return ProcessFilename(pEL);
	}

	return _T("");
}

CExportLoc*	CExportLocationMgr::FindLocation(TCHAR* Name)
{
	for (itExportLoc L = m_ExpLocs.begin(); L != m_ExpLocs.end(); L++)
	{
		if ((*L)->m_Name == Name)
		{
			return *L;
		}
	}

	return 0;
}

String CExportLocationMgr::ProcessFilename(CExportLoc* pEL)
{
	if (m_ExportsNamesAsGpxName)
	{
		CPath	Path;
		String	PathToFile;
		String	GpxFilename;
		String	Extension;

		PathToFile = Path.ExtractPathname(pEL->m_FullPath);
		GpxFilename = Path.ExtractFilename(m_GpxFilename, true);
		
		Extension = Path.ExtractFilename(pEL->m_FullPath);
		Extension = Path.ExtractExtension(Extension);

		return (PathToFile + _T("\\") + GpxFilename + Extension);
	}

	return pEL->m_FullPath;
}

void CExportLocationMgr::Serialize(CStream& ar)
{
	#define CExportLocationMgrVersion	102

	if (ar.IsStoring())
	{
		ar << CExportLocationMgrVersion;

		ar << m_ExpLocs.size();

		for (itExportLoc L = m_ExpLocs.begin(); L != m_ExpLocs.end(); L++)
		{
			(*L)->Serialize(ar);
		}

		ar << m_ExportsNamesAsGpxName;
		ar << m_ExportMaxLimit;
	}
	else
	{
		Reset();

		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			int Count;

			ar >> Count;

			while (Count--)
			{
				CExportLoc* pEL = new CExportLoc;

				pEL->Serialize(ar);

				m_ExpLocs.push_back(pEL);
			}
		}

		if (Version >= 101)
		{
			ar >> m_ExportsNamesAsGpxName;
		}

		if (Version >= 102)
		{
			ar >> m_ExportMaxLimit;
		}

		AddDefaultLocationsByType();
	}
}

void CExportLocationMgr::AddDefaultLocationsByType()
{
	CPath Path;

	wstring Pathname = Path.GetPersonalDir();

	AddLocation(MAGELLAN_EXPORT, Pathname, DEFAULT_MERIDIAN_WPTS_FILENAME);
	AddLocation(LOC_EXPORT, Pathname, DEFAULT_LOC_WPTS_FILENAME);
	AddLocation(GSAK_EXPORT, Pathname, DEFAULT_GSAK_NOTES_FILENAME);
	AddLocation(PHAROS_EXPORT, Pathname, DEFAULT_PHAROS_WPTS_FILENAME);
	AddLocation(MS_STREETTRIPS_EXPORT, Pathname, DEFAULT_MS_STREETTRIPS_PUSHPINS_WPTS_FILENAME);
	AddLocation(MAPTECH_MXF_EXPORT, Pathname, DEFAULT_MAPTECH_MXF_WPTS_FILENAME);
	AddLocation(CSV_EXPORT, Pathname, DEFAULT_CSV_WPTS_FILENAME);
	AddLocation(LOWRANCE_EXPORT, Pathname, DEFAULT_LOWRANCE_WPTS_FILENAME);
	AddLocation(MAGELLAN_EXPLORIST_EXPORT, Pathname, DEFAULT_EXPLORIST_WPTS_FILENAME);
	AddLocation(OZI_EXPORT, Pathname, DEFAULT_OZI_WPTS_FILENAME);
	AddLocation(NAVIGON_EXPORT, Pathname, DEFAULT_NAVIGON_WPTS_FILENAME);
	AddLocation(BOULTERLOGGER_EXPORT, Pathname, DEFAULT_BOULTERLOGGER_WPTS_FILENAME);
}

void CExportLocationMgr::AddLocation(TCHAR* Name, wstring& Path, TCHAR* Filename)
{
	if (!FindLocation(Name))
	{
		CExportLoc* pEL = new CExportLoc;

		pEL->m_Name = Name;
		pEL->m_FullPath = Path + _T("\\");
		pEL->m_FullPath += Filename;

		m_ExpLocs.push_back(pEL);
	}
}

void CExportLocationMgr::Reset()
{
	for (itExportLoc L = m_ExpLocs.begin(); L != m_ExpLocs.end(); L++)
	{
		delete *L;
	}

	m_ExpLocs.clear();
}

void CExportLocationMgr::Export(CExportLoc* pEL)
{
	if (pEL)
	{
		if (pEL->m_Name == MAGELLAN_EXPORT || pEL->m_Name == MAGELLAN_EXPLORIST_EXPORT)
		{
			CommonMagellanExports(pEL);
		}
		else if (pEL->m_Name == LOC_EXPORT)
		{
			Loc(pEL);
		}
		else if (pEL->m_Name == GSAK_EXPORT)
		{
			GSAK(pEL);
		}
		else if (pEL->m_Name == PHAROS_EXPORT)
		{
			PharosOstia6(pEL);
		}
		else if (pEL->m_Name == MS_STREETTRIPS_EXPORT)
		{
			MSFTSTPushpins(pEL);
		}
		else if (pEL->m_Name == MAPTECH_MXF_EXPORT)
		{
			MaptechMXF(pEL);
		}
		else if (pEL->m_Name == CSV_EXPORT)
		{
			CSV(pEL);
		}
		else if (pEL->m_Name == LOWRANCE_EXPORT)
		{
			LowranceiFinder(pEL);
		}
		else if (pEL->m_Name == NAVIGON_EXPORT)
		{
			NavigonMobileNavigator4(pEL);
		}
		else if (pEL->m_Name == BOULTERLOGGER_EXPORT)
		{
			BoulterExpressLogger(pEL);
		}
		else // (pEL->m_Name == OZI_EXPORT)
		{
			OziExplorer(pEL);
		}
	}
}

//
// Export functions
//

void CExportLocationMgr::CommonMagellanExports(CExportLoc* pEL)
{
	itGC it;

	m_Count = 0;
	m_Limited = false;

	CMeridianWriter	MW;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	MW.Open(File.c_str());

	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= 500 || m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			MW.ProcessCache(*pCache);

			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	MW.Close();
}

void CExportLocationMgr::Loc(CExportLoc* pEL) 
{
	itGC it;

	m_Count = 0;
	m_Limited = false;

	CLocFileWriter	LFW;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	LFW.Open(File.c_str());

	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			LFW.ProcessCache(*pCache);

			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	LFW.Close();
}

void CExportLocationMgr::GSAK(CExportLoc* pEL) 
{
	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	m_Count = m_pFieldNoteMgr->ExportToGSAK((TCHAR*) File.c_str());
}

void CExportLocationMgr::BoulterExpressLogger(CExportLoc* pEL) 
{
	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	m_Count = m_pFieldNoteMgr->ExportToBoulterExpressLogger((TCHAR*) File.c_str());
}

void CExportLocationMgr::PharosOstia6(CExportLoc* pEL) 
{
	itGC	it;

	m_Count = 0;
	m_Limited = false;

	CPharosOstia6Writer	PO;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	PO.Open(File.c_str());

	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PO.ProcessCache(*pCache);
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	PO.Close();
}

void CExportLocationMgr::MSFTSTPushpins(CExportLoc* pEL) 
{
	itGC	it;

	m_Count = 0;
	m_Limited = false;

	CPocketStreetPushPinsWriter	PSPP;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	// Count the # of waypoints to be written (required before opening the pushpin file)
	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	if (m_Limited)
		m_Count--;

	// Set the total count of pushpins
	PSPP.SetTotalPushPins(m_Count);

	// Open the pushpin file and write the header
	PSPP.Open(File.c_str());

	m_Count = 0;
	m_Limited = false;

	pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PSPP.ProcessCache(*pCache);
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	PSPP.Close();

	basic_string<TCHAR>::size_type ExtPos;

	ExtPos = File.rfind(_T('.'));

	// Check if there's a map corresponding to the pushpins
	if (ExtPos != basic_string<TCHAR>::npos)
	{
		String MapFilename;

		// Extract the filename w/o the extension
		MapFilename = File.substr(0,ExtPos);

		// Add the 'mps' extension to the map filename
		MapFilename += _T(".mps");

		// Launch S+T with the push pins if the corresponding map exists
		if (GetFileAttributes(MapFilename.c_str()) != -1)
		{
			CBrowserLauncher BL;

			BL.Go(MapFilename.c_str());
		}
	}
}

void CExportLocationMgr::MaptechMXF(CExportLoc* pEL) 
{
	itGC	it;

	m_Count = 0;
	m_Limited = false;

	CMxfFileWriter	PO;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	PO.Open(File.c_str());

	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PO.ProcessCache(*pCache);
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	PO.Close();
}

void CExportLocationMgr::CSV(CExportLoc* pEL) 
{
	itGC	it;

	m_Count = 0;
	m_Limited = false;

	CCsvFileWriter	PO;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	PO.Open(File.c_str());

	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PO.ProcessCache(*pCache);
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	PO.Close();
}

void CExportLocationMgr::LowranceiFinder(CExportLoc* pEL) 
{
	itGC it;

	m_Count = 0;
	m_Limited = false;

	CLowranceWriter	LW;

	// Count the # of waypoints that need to be written
	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}
	
	// Set the total # of waypoints
	LW.SetWaypointCount(m_Count);

	m_Count = 0;
	m_Limited = false;
	
	// Write the waypoints out
	String	File;
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	LW.Open(File.c_str());

	pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			LW.ProcessCache(*pCache);

			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	LW.Close();
}

void CExportLocationMgr::OziExplorer(CExportLoc* pEL) 
{
	itGC	it;

	m_Count = 0;
	m_Limited = false;

	COziWriter	OW;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	OW.Open(File.c_str());

	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			OW.ProcessCache(*pCache);
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	OW.Close();
}

void CExportLocationMgr::NavigonMobileNavigator4(CExportLoc* pEL) 
{
	itGC	it;

	m_Count = 0;
	m_Limited = false;

	CNavigonWriter	NW;

	String	File;
	
	File = GetFilename((TCHAR*)pEL->m_Name.c_str());

	NW.Open(File.c_str());

	CGeoCache* pCache = m_pGpxParser->First(it);

	while (!m_pGpxParser->EndOfCacheList(it))
	{
		if (m_Count >= m_ExportMaxLimit)
		{
			m_Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			NW.ProcessCache(*pCache);
			m_Count++;
		}

		pCache = m_pGpxParser->Next(it);
	}

	NW.Close();
}
