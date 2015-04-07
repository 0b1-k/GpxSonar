#ifndef _INC_CCacheReportsPref
	#define _INC_CCacheReportsPref

#include "CStream.h"
#include "CFieldNoteMgr.h"

class CCacheReportsPref
{
public:
	bool	m_NoteStatusFoundIt;
	bool	m_NoteStatusDNF;
	bool	m_NoteStatusNote;
	bool	m_NoteStatusArchiveIt;
	bool	m_Reset;

public:
	CCacheReportsPref();
	
	bool	IncludeInReport(GcNoteStatus Status);

	void	Serialize(CStream& ar);
};

#endif
