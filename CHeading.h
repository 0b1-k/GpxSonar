#ifndef _INC_CHeading
	#define _INC_CHeading

#include "CommonDefs.h"
#include "CStream.h"

// Describes a column header in the list control
class CHeading
{
public:
	String			m_Name;
	int				m_Length;
	int				m_SortToggle;
	int				m_Id;
	bool			m_Visible;
	bool			m_Searchable;
	PFNLVCOMPARE	m_pSortFunc;

public:
	CHeading();
	CHeading(int Id, const String& Name, int Length, PFNLVCOMPARE pSortFunc, bool Searchable = false);

	void	Serialize(CStream& ar);
};

// Array of column headers
typedef vector<CHeading*> HeadingCont;
typedef vector<CHeading*>::iterator itHeading;

#endif
