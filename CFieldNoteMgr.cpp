#include "CFieldNoteMgr.h"
#include "Literals.h"
#include "CPath.h"
#include "CTimeHelper.h"
#include "CCoords.h"
#include "CTextTrx.h"
#include "CGpxParser.h"

CFieldNote::CFieldNote()
{
	m_Status = NoteStatusFoundIt;
	m_Lat = 0.0;
	m_Long = 0.0;
	m_pCache = 0;
	GetLocalTime(&m_Date);
}

CFieldNote::CFieldNote(const TCHAR* pNotes, GcNoteStatus Status)
{
	m_Notes = pNotes;
	m_Status = Status;

	GetLocalTime(&m_Date);
}

void CFieldNote::Serialize(CStream& ar)
{
	#define CFieldNoteVersion	101

	if (ar.IsStoring())
	{
		ar << CFieldNoteVersion;
		ar << m_Notes;
		ar << m_Status;
		ar << m_Lat;
		ar << m_Long;
		ar << m_Date;

	}
	else
	{
		int Version = 0;

		ar >> Version;

		if (Version >= 100)
		{
			int Status;

			ar >> m_Notes;
			ar >> Status;
			ar >> m_Lat;
			ar >> m_Long;

			m_Status = (GcNoteStatus) Status;
		}

		if (Version >= 101)
		{
			ar >> m_Date;
		}
	}
}

const TCHAR* CFieldNote::GetStatusText()
{
	switch(m_Status)
	{
	case NoteStatusFoundIt:
		return _T("Found it.");
		break;
	case NoteStatusDNF:
		return _T("Did not find it.");
		break;
	case NoteStatusNote:
		return _T("Note.");
		break;
	case NoteStatusArchiveIt:
		return _T("Needs Archiving.");
		break;
	}

	return _T("?");
}

void CFieldNote::SaveYourself(const String& CacheWpt)
{
	CPath	Path;
	
	String	CacheFname = GPXSONAR_FIELD_NOTES_DIR;
			CacheFname += CacheWpt;
			CacheFname += _T(".fld");

	CacheFname = Path.BuildPath(CacheFname);

	CStream	ar;

	ar.SetStoring(true);

	Serialize(ar);

	ar.Save(CacheFname);
}

void CFieldNote::LoadYourself(const String& CacheWpt)
{
	CPath	Path;
	
	String	CacheFname = GPXSONAR_FIELD_NOTES_DIR;
			CacheFname += CacheWpt;

	CacheFname = Path.BuildPath(CacheFname);

	CStream	ar;

	ar.SetStoring(false);

	ar.Load(CacheFname);

	Serialize(ar);
}

void CFieldNote::DeleteYourself(const String& CacheWpt)
{
	CPath	Path;
	
	String	CacheFname = GPXSONAR_FIELD_NOTES_DIR;
			CacheFname += CacheWpt;
			CacheFname += _T(".fld");

	CacheFname = Path.BuildPath(CacheFname);

	// Delete the field note
	DeleteFile((LPCTSTR) CacheFname.c_str());

	CacheFname = GPXSONAR_FIELD_NOTES_DIR;
	CacheFname += CacheWpt;
	CacheFname += _T(".wav");

	CacheFname = Path.BuildPath(CacheFname);

	// Delete the voice note that may exist as well
	DeleteFile((LPCTSTR) CacheFname.c_str());
}

//--------------------------------------------------------------------------------------------------------

CFieldNoteMgr::CFieldNoteMgr()
{
}

CFieldNoteMgr::~CFieldNoteMgr()
{
	Reset();
}

CFieldNote* CFieldNoteMgr::Add(const String& Id, const TCHAR* pNotes, GcNoteStatus Status)
{
	CFieldNote* pFN = Find(Id);

	if (!pFN)
	{
		pFN = new CFieldNote(pNotes, Status);

		if (!pFN)
		{
			//assert(0);
		}

		m_Notes[Id] = pFN;
	}
	else
	{
		pFN->m_Notes = pNotes;
		pFN->m_Status = Status;
	}

	return pFN; 
}

CFieldNote* CFieldNoteMgr::Find(const String& Id)
{
	itFieldNote it = m_Notes.find(Id);

	if (it != m_Notes.end())
	{
		return (CFieldNote*) (*it).second;
	}
	else
	{
		return 0;
	}
}

void CFieldNoteMgr::Delete(const String& Id)
{
	itFieldNote it = m_Notes.find(Id);

	if (it != m_Notes.end())
	{
		delete (*it).second;
		
		m_Notes.erase(it);
	}
}

void CFieldNoteMgr::Delete(CFieldNote* pNote)
{
	itFieldNote it;

	for (it = m_Notes.begin(); it != m_Notes.end(); it++)
	{
		if ((*it).second == pNote)
		{
			delete (*it).second;
			m_Notes.erase(it);
			break;
		}
	}
}
void CFieldNoteMgr::Reset()
{
	itFieldNote it;

	for (it = m_Notes.begin(); it != m_Notes.end(); it++)
	{
		delete (*it).second;
	}

	m_Notes.clear();
}

void CFieldNoteMgr::Serialize(CStream& ar)
{
	#define CFieldNoteMgrVersion	100

	if (ar.IsStoring())
	{
		ar << CFieldNoteMgrVersion;

		ar << m_Notes.size();

		itFieldNote it;

		for (it = m_Notes.begin(); it != m_Notes.end(); it++)
		{
			ar << (*it).first;

			(*it).second->Serialize(ar);
		}
	}
	else
	{
		Reset();

		int		Version;
		long	Count = 0;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> Count;

			while (Count--)
			{
				String Id;

				ar >> Id;

				CFieldNote* pFN = new CFieldNote();

				if (!pFN)
				{
					//assert(0);
				}

				pFN->Serialize(ar);

				m_Notes[Id] = pFN;
			}
		}
	}
}

int CFieldNoteMgr::ExportToGSAK(TCHAR* pFilename)
{
	FILE* pFile = _tfopen(pFilename, _T("wb"));

	if (pFile)
	{
		itFieldNote it;

		for (it = m_Notes.begin(); it != m_Notes.end(); it++)
		{
			WriteToFile(String((*it).first), pFile);

			WriteToFile(String(_T("\t")), pFile);

			CFieldNote* pFN = (*it).second;

			CTimeHelper	TH(pFN->m_Date);

			WriteToFile(String(_T("Date: ")), pFile);
			WriteToFile(TH.Date(), pFile);
			WriteToFile(String(_T("<br>")), pFile);

			WriteToFile(String(_T("Type: ")), pFile);
			WriteToFile(String(pFN->GetStatusText()), pFile);
			WriteToFile(String(_T("<br>")), pFile);

			CCoords	Coords;

			if (pFN->m_Lat && pFN->m_Long)
			{
				Coords.SetDecimal(pFN->m_Lat, pFN->m_Long);

				WriteToFile(String(_T("Actual Coords: ")), pFile);
				WriteToFile(Coords.Format(false), pFile);
				WriteToFile(String(_T("<br>")), pFile);
			}

			CTextTrx TTrx;

			WriteToFile(String(_T("Text: ")), pFile);
			WriteToFile(TTrx.CRToBR(pFN->m_Notes), pFile);
			WriteToFile(String(_T("\r")), pFile);
		}

		fclose(pFile);

		return m_Notes.size();
	}

	return -1;
}

int	CFieldNoteMgr::ExportToBoulterExpressLogger(TCHAR* Filename)
{
	AW_CONVERSION;

	FILE* pFile = _tfopen(Filename, _T("wb"));

	if (pFile)
	{
		vector<CFieldNote*> Notes;
		vector<CFieldNote*>::iterator Nit;

		// Return a sorted list of notes
		Notes = SortByTimestamp();

		fprintf(pFile, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
		fprintf(pFile, "<gpx>\r\n");

		// Run through the notes and write the body of the report
		for (Nit = Notes.begin(); Nit != Notes.end(); Nit++)
		{
			CFieldNote* pFN = (*Nit);

			if (!pFN->m_pCache)
			{
				continue;
			}

			CTimeHelper EndTime(pFN->m_Date);

			fprintf(
				pFile,
				"\t<wpt>\r\n\t\t<name>%s</name>\r\n\t\t<groundspeak:cache>\r\n\t\t\t<groundspeak:name>%s</groundspeak:name>\r\n\t\t\t<groundspeak:owner>%s</groundspeak:owner>\r\n\t\t</groundspeak:cache>\r\n\t\t<cmlog:log version=\"1.0\">\r\n\t\t\t<cmlog:end_time>%sT%s</cmlog:end_time>\r\n\t\t\t<cmlog:notes>%s</cmlog:notes>\t\t</cmlog:log>\t</wpt>",
				w2a((TCHAR*)pFN->m_pCache->m_Shortname.c_str()),
				w2a((TCHAR*)pFN->m_pCache->m_GsCacheName.c_str()),
				w2a((TCHAR*)pFN->m_pCache->m_GsCacheOwnerName.c_str()),
				w2a((TCHAR*)EndTime.Date().c_str()),
				w2a((TCHAR*)EndTime.Time().c_str()),
				w2a((TCHAR*)pFN->m_Notes.c_str())
				);
		}

		fprintf(pFile, "</gpx>\r\n");

		fclose(pFile);

		return m_Notes.size();
	}

	return -1;
}

void CFieldNoteMgr::WriteToFile(String& Str, FILE* pFile)
{
	AW_CONVERSION

	char* pStr = w2a((TCHAR*)Str.c_str());

	fwrite((void*) pStr, sizeof(char), strlen(pStr), pFile);
}

long CFieldNoteMgr::Size()
{
	return m_Notes.size();
}

// Saves each individual field note into its own file
void CFieldNoteMgr::SaveConfig()
{
	itFieldNote it;

	for (it = m_Notes.begin(); it != m_Notes.end(); it++)
	{
		(*it).second->SaveYourself((*it).first);
	}
}

// Enumerates the cache files under \Config and loads the field notes
void CFieldNoteMgr::LoadConfig()
{
	WIN32_FIND_DATA		FFD;
	BOOL				MoreData = true;
	CPath				Path;

	String	NotePath = GPXSONAR_FIELD_NOTES_DIR;
			NotePath += _T("*.fld");

	NotePath = Path.BuildPath(NotePath);

	HANDLE hFieldNote = FindFirstFile(NotePath.c_str(), &FFD);

	while (INVALID_HANDLE_VALUE != hFieldNote && MoreData)
	{
		String Id = FFD.cFileName;

		CFieldNote* pFN = new CFieldNote();

		if (!pFN)
		{
			// throw up;
		}

		pFN->LoadYourself(Id);

		// Store the cache waypoint name w/o the extension
		m_Notes[Id.substr(0, Id.size() - 4)] = pFN;

		MoreData = FindNextFile(hFieldNote, &FFD);
	}

	FindClose(hFieldNote);
}

// Enumerates the field notes files under \Config and deletes them
void CFieldNoteMgr::DeleteConfig()
{
	CleanNotes(_T("*.fld"));
	CleanNotes(_T("*.wav"));
}

void CFieldNoteMgr::CleanNotes(TCHAR* pExtension)
{
	WIN32_FIND_DATA		FFD;
	BOOL				MoreData = true;
	CPath				Path;

	String	NotePath = GPXSONAR_FIELD_NOTES_DIR;
			NotePath += pExtension;

	NotePath = Path.BuildPath(NotePath);

	HANDLE hFieldNote = FindFirstFile(NotePath.c_str(), &FFD);

	while (INVALID_HANDLE_VALUE != hFieldNote && MoreData)
	{
		NotePath = GPXSONAR_FIELD_NOTES_DIR;
		NotePath += FFD.cFileName;

		NotePath = Path.BuildPath(NotePath);

		DeleteFile((LPCTSTR) NotePath.c_str());

		// Next...
		MoreData = FindNextFile(hFieldNote, &FFD);
	}

	FindClose(hFieldNote);

	Reset();
}

vector<CFieldNote*> CFieldNoteMgr::SortByTimestamp()
{
	vector<CFieldNote*> FieldNotesVec;

	// Transfer the map into the vector
	itFieldNote it;

	for (it = m_Notes.begin(); it != m_Notes.end(); it++)
	{
		FieldNotesVec.push_back((*it).second);

	}

	sort( FieldNotesVec.begin(), FieldNotesVec.end(), SortByTimestampImpl);

	return FieldNotesVec;
}

bool CFieldNoteMgr::SortByTimestampImpl(CFieldNote* pFN1, CFieldNote* pFN2)
{
	FILETIME		FT1, FT2;
	ULARGE_INTEGER	UL1, UL2;

	SystemTimeToFileTime(&pFN1->m_Date, &FT1);
	SystemTimeToFileTime(&pFN2->m_Date, &FT2);

	UL1.LowPart = FT1.dwLowDateTime;
	UL1.HighPart = FT1.dwHighDateTime;

	UL2.LowPart = FT2.dwLowDateTime;
	UL2.HighPart = FT2.dwHighDateTime;

	return (UL1.QuadPart > UL2.QuadPart);
}
