#ifndef _INC_CXmlMap
	#define _INC_CXmlMap

#include "CommonDefs.h"

#define TIME_CONV				"TIME"
#define BOOL_CONV				"BOOL"
#define MEM_MISER_CANARY		_T("{@|~|#}")
#define MEM_MISER_CANARY_SIZE	7

class CXmlBase
{
protected:
	string	m_Elem;
	string	m_Format;
	void*	m_pVar;
	bool	m_bStoreToDisk;

public:
	CXmlBase();

	// True if the variable is to be stored to disk
	bool		StoredToDisk()
	{
		return m_bStoreToDisk;
	}

	// Converts and store the data of a value
	void		Store(const char* pCData, long Offset, long Length);

	// Sets the address
	void		SetAddr(void* pAddr, bool bStoreToDisk = false);

protected:

	SYSTEMTIME	ParseTime(const char* pCData);
	bool		ParseBool(const char* pCData);
};

class CXmlAttr : public CXmlBase
{
public:
	string	m_Attr;

public:
	CXmlAttr(const char* pElem, const char* pAttr, const char* pFormat, void* pVar);

	// Returns 'true' if the member element matches the argument
	bool	MatchElem(const char* pElem);

	// Returns 'true' if the arguments match the attribute object
	bool	Match(const char* pElem, const char* pAttr);
};

typedef vector<CXmlAttr*> AttrCont;
typedef vector<CXmlAttr*>::iterator itAttr;

class CXmlVal : public CXmlBase
{
public:
	string	m_Val;

public:
	CXmlVal(const char* pElem, const char* pVal, const char* pFormat, void* pVar, bool bStoreToDisk = false);

	// Returns 'true' if the name of the value matches the arguments
	bool	Match(const char* pElem, const char* pVal);
};

typedef vector<CXmlVal*> ValCont;
typedef vector<CXmlVal*>::iterator itVal;

// Standard prototype for functions to be called for OnAttr() registrations with non-NULL function pointers.
typedef void (*NoArgFunc) ();

class CXmlElem
{
private:
	string		m_Elem;
	NoArgFunc	m_pNoArgFunc;

public:
	CXmlElem(const char* Elem, NoArgFunc pNoArgFunc);

	// Returns 'true' if a function was invoked for the element.
	bool CallOnMatch(const char* pElem);
};

typedef vector<CXmlElem*> ElemCont;
typedef vector<CXmlElem*>::iterator itElem;

#endif
