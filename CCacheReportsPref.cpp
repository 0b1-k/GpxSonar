#include "CCacheReportsPref.h"

CCacheReportsPref::CCacheReportsPref()
{
	m_NoteStatusFoundIt = true;
	m_NoteStatusDNF = true;
	m_NoteStatusNote = false;
	m_NoteStatusArchiveIt = true;
	m_Reset = false;
}

bool CCacheReportsPref::IncludeInReport(GcNoteStatus Status)
{
	switch (Status)
	{
	case NoteStatusFoundIt:
		return m_NoteStatusFoundIt;
		break;
	case NoteStatusDNF:
		return m_NoteStatusDNF;
		break;
	case NoteStatusNote:
		return m_NoteStatusNote;
		break;
	case NoteStatusArchiveIt:
		return m_NoteStatusArchiveIt;
		break;
	}

	// Should never get here
	return false;
}

#define CCacheReportsPrefVersion 100

void CCacheReportsPref::Serialize(CStream& ar)
{
	if (ar.IsStoring())
	{
		ar << CCacheReportsPrefVersion;

		ar << m_NoteStatusFoundIt;
		ar << m_NoteStatusDNF;
		ar << m_NoteStatusNote;
		ar << m_NoteStatusArchiveIt;
		ar << m_Reset;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_NoteStatusFoundIt;
			ar >> m_NoteStatusDNF;
			ar >> m_NoteStatusNote;
			ar >> m_NoteStatusArchiveIt;
			ar >> m_Reset;
		}
	}
}
