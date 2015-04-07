#include "CDynStr.h"

CDynStr::CDynStr()
{
	InitInternalState();
}

CDynStr::CDynStr(const char* pStr)
{
	InitInternalState();

	Cat(pStr);
}

CDynStr::CDynStr(char* pStr)
{
	InitInternalState();

	Cat((const char*)pStr);
}

CDynStr::CDynStr(const string& Str)
{
	InitInternalState();

	Cat(Str.c_str());
}

CDynStr::~CDynStr()
{
	Reset();
}

// Return a pointer to the internal string;
const char* CDynStr::operator*()
{
	return (const char*) m_pStr;
}

// Concatenates a string to the internal buffer and returns a pointer to the complete string
const char* CDynStr::Cat(const char* pStr)
{
	if (pStr)
	{
		return Cat(pStr, strlen(pStr));
	}

	return 0;
}

// Concatenates a string to the internal buffer and returns a pointer to the complete string
const char* CDynStr::Cat(const char* pStr, long Len)
{
	if ((m_StrLen + Len + 1) > m_BufferLen)
	{
		return Grow(pStr, Len);
	}
	else
	{
		m_StrLen += Len;

		return strncat(m_pStr, pStr, Len);
	}
}

// Returns the length of the string
long CDynStr::Size()
{
	return m_StrLen;
}

// Returns the string to a 0 length w/o de-allocating the buffer
void CDynStr::Clear()
{
	memset(m_pStr, 0, m_StrLen * sizeof(char));
	m_StrLen = 0;
}

// Deallocates the internal buffer and cleans up
void CDynStr::Reset()
{
	if (m_pStr)
	{
		delete m_pStr;
	}

	InitInternalState();
}

// Initializes the internal state of the class
void CDynStr::InitInternalState()
{
	m_pStr = 0;
	m_StrLen = 0;
	m_BufferLen = 0;
}

// Grows the internal buffer in place and copies the argument string to it
const char* CDynStr::Grow(const char* pStr, long Len)
{
	#define EXTRA_ROOM_IN_CHARS	1024

	if (!m_pStr)
	{
		// Buffer has never been allocated before, so just copy the string into a new one with an extra 1024 characters
		// for future additions.
		m_BufferLen = Len + EXTRA_ROOM_IN_CHARS;
		m_StrLen = Len;
		m_pStr = new char[m_BufferLen];
		strncpy(m_pStr, pStr, Len);
	}
	else
	{
		// Calculate the length of the final 'grown' buffer
		m_BufferLen = m_StrLen + Len + EXTRA_ROOM_IN_CHARS;

		// Allocate a new buffer l
		char* pTmp = new char[m_BufferLen];

		// Zero out the new buffer
		memset(pTmp, 0, m_BufferLen * sizeof(char));

		// Copy the content of the existing string into the new one
		strncpy(pTmp, m_pStr, m_StrLen);

		// Copy the content of the new string into the new buffer
		strncat(pTmp, pStr, Len);

		// Record the length of the new string
		m_StrLen += Len;

		// Delete the old buffer
		delete m_pStr;

		// Make the temporary buffer the current one
		m_pStr = pTmp;
	}

	return (const char*) m_pStr;
}