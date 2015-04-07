#include "CXmlMap.h"

CXmlBase::CXmlBase()
{
	m_bStoreToDisk = false;
	m_pVar = 0;
}

// Sets the address
void CXmlBase::SetAddr(void* pAddr, bool bStoreToDisk)
{
	m_pVar = pAddr;
	m_bStoreToDisk = bStoreToDisk;
}

// Converts and store the data of a value
void CXmlBase::Store(const char* pCData, long Offset, long Length)
{
	// Conversion using a format string?
	if (m_Format[0] == '%')
	{
		if (m_Format == "%s")
		{
			if (pCData)
			{
				// The data is being stored to disk
				if (m_bStoreToDisk)
				{
					TCHAR	Buffer[50];

					// Build a 'canary' value indicating the location of the data in the file storage
					_stprintf(Buffer,_T("%s&%ld&%ld"), MEM_MISER_CANARY, Offset, Length);

					((String*) m_pVar)->assign(Buffer);
				}
				else
				{
					// The data of the variable is being stored in memory
					long Len = Length + 1;
        
					TCHAR* pTemp = new TCHAR[Len*2];

					_a2wHelper((wchar_t*) pTemp, pCData, Len);

					((String*) m_pVar)->assign(pTemp);

					delete pTemp;
				}
			}
		}
		else if (m_Format == "%lf")
		{
			sscanf(pCData, m_Format.c_str(), (double*) m_pVar);
		}
		else if (m_Format == "%ld")
		{
			sscanf(pCData, m_Format.c_str(), (long*) m_pVar);
		}
		else if (m_Format == "%i")
		{
			sscanf(pCData, m_Format.c_str(), (int*) m_pVar);
		}
		else
		{
			//assert(0);
		}
	}
	else if (m_Format == TIME_CONV)
	{
		(*((SYSTEMTIME*) m_pVar)) = ParseTime(pCData);
	}
	else if (m_Format == BOOL_CONV)
	{
		(*((bool*) m_pVar)) = ParseBool(pCData);
	}
	else
	{
		//assert(0);
	}
}

bool CXmlBase::ParseBool(const char* pCData)
{
	if (!_stricmp(pCData,"True"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

SYSTEMTIME CXmlBase::ParseTime(const char* timestr)
{
	SYSTEMTIME ST;

	sscanf(timestr, "%d-%d-%d", 
		&ST.wYear,
		&ST.wMonth,
		&ST.wDay);

	return ST;
}

//------------------------------------------------------------------------------------------------------------------------
CXmlAttr::CXmlAttr(const char* pElem, const char* pAttr, const char* pFormat, void* pVar)
{
	m_Elem = pElem;
	m_Attr = pAttr;
	m_Format = pFormat;
	m_pVar = pVar;
}

// Returns 'true' if the member element matches the argument
bool CXmlAttr::MatchElem(const char* pElem)
{
	if (!strcmp(m_Elem.c_str(), pElem))
	{
		return true;
	}

	return false;
}

// Returns 'true' if the arguments match the attribute object
bool CXmlAttr::Match(const char* pElem, const char* pAttr)
{
	if (!strcmp(m_Elem.c_str(), pElem) && !strcmp(m_Attr.c_str(), pAttr))
	{
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------------------------------------------
CXmlVal::CXmlVal(const char* pElem, const char* pVal, const char* pFormat, void* pVar, bool bStoreToDisk)
{
	m_Val = pVal;
	m_Format = pFormat;
	m_Elem = pElem;
	m_pVar = pVar;
	m_bStoreToDisk = bStoreToDisk;
}

// Returns 'true' if the name of the value matches the argument
bool CXmlVal::Match(const char* pElem, const char* pVal)
{
	if (!strcmp(m_Elem.c_str(), pElem) && !strcmp(m_Val.c_str(), pVal))
	{
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------------------------------------------
CXmlElem::CXmlElem(const char* pElem, NoArgFunc pNoArgFunc)
{
	m_Elem = pElem;
	m_pNoArgFunc = pNoArgFunc;
}

// Returns 'true' if a function was invoked for the element.
bool CXmlElem::CallOnMatch(const char* pElem)
{
	if (!strcmp(m_Elem.c_str(), pElem))
	{
		m_pNoArgFunc();

		return true;
	}

	return false;
}
