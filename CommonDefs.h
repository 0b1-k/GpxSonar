#ifndef _INC_CommonDefs
	#define _INC_CommonDefs

#pragma warning(disable:4786)
#pragma warning(disable:4800)
#pragma warning(disable:4531)

#include "windows.h"

#include <vector>
#include <map>
#include <list>
#include <string>

#ifdef PPC2K2
	#include "StringEx.h"
#endif

using namespace std;

// Unicode > Ansi & Ansi > Unicode conversions

#define AW_CONVERSION	int	_nStrLength = 0; 

// Helper function for a2w()
wchar_t* _a2wHelper(wchar_t* pW, const char* pA, int nChars);

// Helper function for w2a)
char* _w2aHelper(char* pA, wchar_t* pW, int nChars);

// Converts an ASCII string to Unicode
#define a2w(pA) (\
    ((char*)pA == NULL) ? NULL : (_nStrLength = (strlen(pA)+1),\
        _a2wHelper((wchar_t*) alloca(_nStrLength*2), pA, _nStrLength)\
    )\
)

// Converts a Unicode string to ASCII
#define w2a(pW) (\
    ((wchar_t*)pW == NULL) ? NULL : (_nStrLength = WideCharToMultiByte(CP_UTF8, 0, pW, -1, NULL, 0, NULL, NULL),\
        _w2aHelper((char*) alloca(_nStrLength), pW, _nStrLength)\
    )\
)

#define THROWx

#ifdef PPC2K2
	typedef StringEx String;
	// Define a string list container + iterator
	typedef vector<StringEx> StringListCont;
	typedef vector<StringEx>::iterator itStr;

	class StringList : public StringListCont
	{
	public:
		StringList()
		{}

		~StringList()
		{}
	};

#else
	typedef wstring String;
	// Define a string list container + iterator
	typedef vector<String> StringList;
	typedef vector<String>::iterator itStr;
#endif


// Macros to use when placing reminders inline in the text of the code
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __NOTE__ __FILE__ "("__STR1__(__LINE__)") : *** NOTE: "

#endif