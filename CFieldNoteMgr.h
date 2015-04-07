#ifndef _INC_CFieldNote
	#define _INC_CFieldNote

#include "CommonDefs.h"
#include "CStream.h"

typedef enum {
	NoteStatusFoundIt = 0,
	NoteStatusDNF,
	NoteStatusNote,
	NoteStatusArchiveIt,
} GcNoteStatus;

class CGeoCache;

class CFieldNote
{
public:
	CGeoCache*		m_pCache;
	String			m_Notes;
	GcNoteStatus	m_Status;
	double			m_Lat;
	double			m_Long;
	SYSTEMTIME		m_Date;

public:
	CFieldNote();
	CFieldNote(const TCHAR* pNotes, GcNoteStatus Status);

	const TCHAR*	GetStatusText();

	void SaveYourself(const String& CacheWpt);
	void LoadYourself(const String& CacheWpt);
	void DeleteYourself(const String& CacheWpt);

	void Serialize(CStream& ar);
};

typedef map<String, CFieldNote*> FieldNoteCont;
typedef map<String, CFieldNote*>::iterator itFieldNote;

class CFieldNoteMgr
{
private:
	FieldNoteCont		m_Notes;

public:
	CFieldNoteMgr();
	~CFieldNoteMgr();

	CFieldNote*	Add(const String& Id, const TCHAR* pNotes, GcNoteStatus Status);
	CFieldNote* Find(const String& Id);

	void		Delete(const String& Id);
	void		Delete(CFieldNote* pNote);

	void		Serialize(CStream& ar);

	void		SaveConfig();
	void		LoadConfig();
	void		DeleteConfig();

	void		Reset();

	int			ExportToGSAK(TCHAR* Filename);
	int			ExportToBoulterExpressLogger(TCHAR* Filename);

	long		Size();

	vector<CFieldNote*>	SortByTimestamp();

protected:
	void		WriteToFile(String& Text, FILE* pFile);
	static bool	SortByTimestampImpl(CFieldNote* pFN1, CFieldNote* pFN2);
	void		CleanNotes(TCHAR* pExtension);
};

#endif
