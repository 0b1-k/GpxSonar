#include "CFilterCacheTB.h"
#include "CGpxParser.h"

CFilterCacheTB::CFilterCacheTB(const TCHAR* pText, GcFilter FilterType) : CFilterBase(pText, FilterType)
{
}

CFilterCacheTB::~CFilterCacheTB()
{
}

bool CFilterCacheTB::OnFilterCache(CGeoCache* pCache)
{
	if (pCache->GetTBCount())
	{
		return true;
	}

	return false;
}

void CFilterCacheTB::Serialize(CStream& ar)
{
	#define CFilterCacheTBVersion	100

	CFilterBase::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << CFilterCacheTBVersion;
	}
	else
	{
		int Version;

		ar >> Version;
	}
}
