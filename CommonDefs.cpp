#include "CommonDefs.h"
#include "winnls.h"

// Helper function for a2w()
wchar_t* _a2wHelper(wchar_t* pW, const char* pA, int nChars)
{
	if (!pA)
	{
		return NULL;
	}

	// verify that no illegal character present
	// since lpw was allocated based on the size of lpa
	// don't worry about the number of chars
	pW[0] = '\0';
	
	MultiByteToWideChar(CP_UTF8, 0, pA, -1, pW, nChars);
	
	return pW;
}

// Helper function for w2a)
char* _w2aHelper(char* pA, wchar_t* pW, int nChars)
{
	if (pW == NULL)
	{
		return NULL;
	}

	// verify that no illegal character present
	// since lpa was allocated based on the size of lpw
	// don't worry about the number of chars
	pA[0] = '\0';

	WideCharToMultiByte(CP_UTF8, 0, pW, -1, pA, nChars, NULL, NULL);
	
	return pA;
}