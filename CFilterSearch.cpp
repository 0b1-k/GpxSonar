#include "CFilterSearch.h"
#include "CGpxParser.h"

CFilterSearch::CFilterSearch(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
	m_Match = PartialMatch;
}

CFilterSearch::~CFilterSearch()
{
}

bool CFilterSearch::OnFilterCache(CGeoCache* pCache)
{
	const TCHAR* pText = m_Text.c_str();

	if (ExactMatch == m_Match)
	{
		if (!_tcsicmp(pText, (TCHAR*) pCache->m_Shortname.c_str()) || 
			!_tcsicmp(pText, (TCHAR*) pCache->m_GsCacheName.c_str()) || 
			!_tcsicmp(pText, (TCHAR*) pCache->m_GsCacheOwnerName.c_str()))
		{
			return true;
		}
	}
	else
	{
		// Allocate a buffer large enough to handle any of the strings
		long Size = pCache->m_Shortname.size() + pCache->m_GsCacheName.size() + pCache->m_GsCacheOwnerName.size();

		TCHAR* pBuffer = new TCHAR[Size];

		if (FindSubstr(pBuffer, Size, pText, pCache->m_Shortname))
		{
			delete pBuffer;

			return true;
		}

		if (FindSubstr(pBuffer, Size, pText, pCache->m_GsCacheName))
		{
			delete pBuffer;

			return true;
		}

		if (FindSubstr(pBuffer, Size, pText, pCache->m_GsCacheOwnerName))
		{
			delete pBuffer;

			return true;
		}

		delete pBuffer;
	}

	return false;
}

// Attempt to find a substring in another after making lowercase conversions. Returns 'true' if the substring is found.
bool CFilterSearch::FindSubstr(TCHAR* pBuffer, long BuffSize, const TCHAR* pSubstr, String& Text)
{
	long TextSize = Text.size();

	// Copy the Substring into the provided buffer and convert it to lower case before the comparison
	for (long I = 0; I < TextSize; I++)
	{
#ifdef PPC2K2
		pBuffer[I] = tolower(Text[I]);
#else
		pBuffer[I] = _tolower(Text[I]);
#endif
	}

	// Terminate the buffer
	pBuffer[I] = 0;

	// Attempt to find the string in the text
	if (_tcsstr(pBuffer, pSubstr))
	{
		return true;
	}

	return false;
}

void CFilterSearch::Serialize(CStream& ar)
{
	#define	CFilterSearchVersion 100

	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterSearchVersion;
		ar << m_Match;
		ar << m_Text;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			int Temp;

			ar >> Temp;
			ar >> m_Text;

			m_Match = (StringMatchType) Temp;
		}
	}
}
