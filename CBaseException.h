#ifndef _INC_CBaseException
	#define _INC_CBaseException

#include "CommonDefs.h"

// This class is used as a base class for all exceptions. It's used for all error logging.
class CBaseException
{
	#define MAX_LOG_FILE_SIZE_IN_BYTES	(5 * 1024 * 1024)

public:
	DWORD	m_dwError;
	String	m_szSrc;
	String	m_szMsg;

public:
	CBaseException();
	~CBaseException();

	// Change the quota on the log file
	void SetMaxLogSizeInBytes(long lMaxSize);

	// Convert the current m_dwError to a text msg assuming that it's a Win32 error
	void Win32Error();

	// Log an error to disk to a log file named after the calling process.
	// The log file is located in a "\Logs" directory at the level of the calling process.
	// Optionally, a log file name can be specified if the default is not good.
	void Log(const String& szOptFile = _T(""), bool bReset = false);

	// Prints out the current exception to the console
	void Print();

protected:
	// strip all CR LF from a string and return the cleaned up string
	String StripCRLF(String& Str);
};

#endif
