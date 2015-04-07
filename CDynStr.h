#ifndef _INC_CDynStr
	#define _INC_CDynStr

#include "CommonDefs.h"

class CDynStr
{
private:
	char*	m_pStr;
	// Length of the string in characters
	long	m_StrLen;
	// Length of the buffer in characters
	long	m_BufferLen;

public:
	CDynStr();
	CDynStr(const char* pStr);
	CDynStr(char* pStr);
	CDynStr(const string& Str);

	virtual ~CDynStr();

	// Return a pointer to the internal string;
	const	char*	operator*();

	// Concatenates a string to the internal buffer and returns a pointer to the complete string
	const	char*	Cat(const char* pStr);

	// Concatenates a string to the internal buffer and returns a pointer to the complete string
	const	char*	Cat(const char* pStr, long Len);

	// Returns the length of the string
	long	Size();

	// Returns the string to a 0 length w/o de-allocating the buffer
	void	Clear();

	// Deallocates the internal buffer and cleans up
	void	Reset();

private:
	// Initializes the internal state of the class
	void			InitInternalState();

	// Grows the internal buffer in place and copies the argument string to it
	const char*	Grow(const char* pStr, long Len);
};

#endif