#include "CFilterOnStrings.h"
#include "CGpxParser.h"

//------------------------------------------------------------------------------------------------------------------------
CFilteredString::CFilteredString()
{
	m_Enabled = false;
}

void CFilteredString::Serialize(CStream& ar)
{
	#define CFilteredStringVersion 100

	if (ar.IsStoring())
	{
		ar << CFilteredStringVersion;
		ar << m_Str;
		ar << m_Enabled;
	}
	else
	{
		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			ar >> m_Str;
			ar >> m_Enabled;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------

CFilterOnStrings::CFilterOnStrings(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
}

CFilterOnStrings::~CFilterOnStrings()
{
	Reset();
}

// Update the internal string list from a new one
void CFilterOnStrings::Update(StringList& StrList)
{
	for (itStr I = StrList.begin(); I != StrList.end(); I++)
	{
		bool Found = false;

		String Str = (*I);

		for (itFiltStr S = m_Strs.begin(); S != m_Strs.end(); S++)
		{
			if ((*S)->m_Str == Str)
			{
				Found = true;
				break;
			}
		}

		if (!Found)
		{
			CFilteredString* pFS = new CFilteredString;

			pFS->m_Str = Str;

			m_Strs.push_back(pFS);
		}
	}
}

// Delete a string object according to its pointer
void CFilterOnStrings::Delete(CFilteredString* pFS)
{
	for (itFiltStr S = m_Strs.begin(); S != m_Strs.end(); S++)
	{
		if ((*S) == pFS)
		{
			delete pFS;

			m_Strs.erase(S);

			return;
		}
	}
}

bool CFilterOnStrings::OnFilterCache(CGeoCache* pCache)
{
	for (itFiltStr S = m_Strs.begin(); S != m_Strs.end(); S++)
	{
		CFilteredString* pFS = *S;

		if (GetType() == FilterStateList)
		{
			if ((pFS->m_Str == pCache->m_GsCacheState) && !pFS->m_Enabled)
			{
				return false;
			}
		}
		else
		{
			if ((pFS->m_Str == pCache->m_GsCacheCountry) && !pFS->m_Enabled)
			{
				return false;
			}
		}
	}

	return true;
}

void CFilterOnStrings::Serialize(CStream& ar)
{
	#define CFilterOnStringsVersion	100
	
	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterOnStringsVersion;

		ar << m_Strs.size();

		for (itFiltStr S = m_Strs.begin(); S != m_Strs.end(); S++)
		{
			(*S)->Serialize(ar);
		}
	}
	else
	{
		Reset();

		int Version;

		ar >> Version;

		if (Version >= 100)
		{
			long Count;

			ar >> Count;

			while (Count--)
			{
				CFilteredString* pFS = new CFilteredString;
				
				pFS->Serialize(ar);

				m_Strs.push_back(pFS);
			}
		}
	}
}

void CFilterOnStrings::Reset()
{
	for (itFiltStr S = m_Strs.begin(); S != m_Strs.end(); S++)
	{
		delete *S;
	}

	m_Strs.clear();
}
