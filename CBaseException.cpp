#include "CBaseException.h"
#include "CPath.h"

// Static member so that all instances work with the same value
static long	m_lQuotaInBytes = MAX_LOG_FILE_SIZE_IN_BYTES;

CBaseException::CBaseException()
{
	m_dwError = GetLastError();
}

CBaseException::~CBaseException()
{
}

// Change the quota on the log file
void CBaseException::SetMaxLogSizeInBytes(long lMaxSize)
{
	m_lQuotaInBytes = lMaxSize;
}

// Convert the current m_dwError to a text msg assuming that it's a Win32 error
void CBaseException::Win32Error()
{
	void* pMsg = NULL;

#ifdef _WIN32_
	::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			m_dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &pMsg,
			0,
			NULL);
#endif

	if (pMsg)
	{
		// Save any text already assigned to the message.
		if (m_szMsg.empty())
		{
			m_szMsg = (TCHAR*) pMsg;
		}
		else
		{
			m_szMsg += _T("\n");
			m_szMsg += (TCHAR*) pMsg;
		}

		LocalFree(pMsg);
	}
	else
	{
		TCHAR Buffer[30];

		_sntprintf(Buffer, 30, _T("%lu"), m_dwError);

		m_szMsg = _T("Failed to resolve error: ");
		m_szMsg += Buffer;
		
		Log(_T("FtsUnresolvedErrors"));
	}
}

// Log an error to disk to a log file named after the calling process.
// The log file is located in a "\Logs" directory at the level of the calling process.
// Optionally, a log file name can be specified if the default is not good.
void CBaseException::Log(const String& szOptFilename, bool bReset)
{
	// open a handle to the log file
	FILE* hFile = NULL;
	
	CPath	Path;

	if (bReset)
	{
		hFile = _tfopen((TCHAR*) Path.GetLogsDir(szOptFilename).c_str(), _T("w"));
	}
	else
	{
		hFile = _tfopen((TCHAR*) Path.GetLogsDir(szOptFilename).c_str(), _T("a+"));

		fseek(hFile, 0, SEEK_END);

		// delete the file if it's too large
		if (ftell(hFile) > m_lQuotaInBytes)
		{
			// Close the file
			fclose(hFile);
			// Then re-open it but this time truncate it.
			hFile = _tfopen((TCHAR*) Path.GetLogsDir(szOptFilename).c_str(), _T("w"));
		}
	}

	if (hFile == NULL)
	{
		CBaseException Up;
		
		Up.Win32Error();

		_tprintf(
			_T("CBaseException::Log() Failed to create log file: %s. The error was: %lu. %s"),
			(TCHAR*) Path.GetLogsDir(szOptFilename).c_str(),
			Up.m_dwError,
			Up.m_szMsg.c_str());

		return;
	}

	m_szSrc = StripCRLF(m_szSrc);
	m_szMsg = StripCRLF(m_szMsg);
	
	SYSTEMTIME ST;

	GetLocalTime(&ST);

	// Write the content of the exception to disk
	_ftprintf(
		hFile, 
		_T("\"%d-%02d-%02d, %02d:%02d:%02d\",\"%s\",\"%s\",\"%lu\",\"%s\"\n"), 
		ST.wYear,
		ST.wMonth,
		ST.wDay,
		ST.wHour,
		ST.wMinute,
		ST.wSecond,
		(TCHAR*) Path.GetProcess().c_str(),
		m_szSrc.c_str(),
		m_dwError,
		m_szMsg.c_str());

	#ifdef _DEBUG
	Print();
	#endif

	// Done writing to the log file. Close it
	fclose(hFile);
}

// strip all CR LF from a string and return the cleaned up string
String CBaseException::StripCRLF(String& Str)
{
	String szStr;

	szStr.reserve(Str.size());

	long lLen = Str.size();

	for (long lPos = 0; lPos < lLen; lPos++)
	{
		TCHAR cChar = Str.at(lPos);

		if ( cChar == _T('\r') || cChar == _T('\n'))
		{
			szStr += _T(' ');
		}
		else
		{
			szStr += cChar;
		}
	}

	return szStr;
}

void CBaseException::Print()
{
	_tprintf(
		_T("Src: %s, Error: %lu, Msg: %s\r\n"),
		m_szSrc.c_str(),
		m_dwError,
		m_szMsg.c_str());
}
