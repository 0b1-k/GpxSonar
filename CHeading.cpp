#include "StdAfx.h"
#include "CHeading.h"

CHeading::CHeading()
{
}

CHeading::CHeading(int Id, const String& Name, int Length, PFNLVCOMPARE pSortFunc, bool Searchable)
{
	m_Id = Id;
	m_Name = Name;
	m_Length = Length;
	m_SortToggle = 1;
	m_pSortFunc = pSortFunc;
	m_Visible = true;
	m_Searchable = Searchable;
}

void CHeading::Serialize(CStream& ar)
{
	#define CHeadingVersion	100

	if (ar.IsStoring())
	{
		ar << CHeadingVersion;
		ar << m_Id;
		ar << m_Name;
		ar << m_Length;
		ar << m_SortToggle;
		ar << m_Visible;
		ar << m_Searchable;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Id;
			ar >> m_Name;
			ar >> m_Length;
			ar >> m_SortToggle;
			ar >> m_Visible;
			ar >> m_Searchable;
		}
	}
}
