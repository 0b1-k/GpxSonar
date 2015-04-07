#include "CPath.h"
#include "CBaseException.h"
#include "Literals.h"

CPath::CPath()
{
	TCHAR szModuleFilename[MAX_PATH+2];

	// Get the full path of the process using this object
	if (!GetModuleFileName(NULL, szModuleFilename, MAX_PATH+1))
	{
		CBaseException	Up;

		Up.Win32Error();

		_tprintf(
			_T("CPath::CPath() Failed on GetModuleFileName(). Error: %lu, Msg: %s\n"),
			Up.m_dwError,
			Up.m_szMsg);
	}
	else
	{
		int iLen = _tcslen(szModuleFilename);
		bool bExtGone = false;

		// Run through the path of the module and extract the path and the module name w/o extension
		while (iLen--)
		{
			if (!bExtGone)
			{
				if (szModuleFilename[iLen] == _T('.'))
				{
					// Chop the extension off
					szModuleFilename[iLen] = _T('\0');
					bExtGone = true;
				}
			}
			else
			{
				// Look for the first '\' preceding the module name
				if (szModuleFilename[iLen] == _T('\\'))
				{
					// Retrieve the module name
					TCHAR* pszProcName = _tcsdup(&szModuleFilename[iLen+1]);
					m_szProcName = pszProcName;
					free(pszProcName);

					// Chop off the module name
					szModuleFilename[iLen] = _T('\0');

					// Retrieve the path
					m_szProcPath = szModuleFilename;

					// Bail from the loop
					break;
				}
			}
		}		
	}
}

CPath::~CPath()
{
}

// Returns the process name
String	CPath::GetProcess()
{
	return m_szProcName;
}

// Returns a path to a directory where log files can be written
// By default, the name of the log file is that of the calling process. This can be changed by passing an optional filename.
String CPath::GetLogsDir(const String& szOptFilename)
{
	String	szPath = m_szProcPath;
			szPath += _T("\\Logs");

	// Make sure the directory exists
	CreateDirectory(szPath.c_str(), NULL);

	// Add the log filename to the path...
	if (szOptFilename.empty())
	{
		// Use the default
		szPath += _T("\\") + m_szProcName + _T(".log");
	}
	else
	{
		// Use the filename provided by the caller
		szPath += _T("\\") + szOptFilename + _T(".log");
	}

	return szPath;
}

// Returns the path to the directory where the process was started from
String CPath::GetProcDir()
{
	return m_szProcPath;
}

// Returns a complete path using a relative path to the directory where the process was started from
String CPath::BuildPath(const String& RelPath)
{
	return m_szProcPath + RelPath;
}

// Returns the path to the directory used for temporary files
String CPath::GetTempDir()
{
	String	szPath = m_szProcPath;
			szPath += _T("\\Temp");

	// Make sure the directory exists
	CreateDirectory(szPath.c_str(), NULL);

	return szPath;
}

// Returns the portion of a full path containing the filename
String CPath::ExtractFilename(const String& szFullPath, bool StripExtension)
{
	String	Filename;

	int iPos = szFullPath.rfind('\\');
	
	if (iPos != string::npos)
	{
		Filename = szFullPath.substr(iPos+1);
	}
	else
	{
		Filename = szFullPath;
	}

	if (StripExtension)
	{
		iPos = Filename.rfind('.');

		if (iPos != string::npos)
		{
			Filename = Filename.substr(0, iPos);
		}
	}

	return Filename;
}

String CPath::ExtractExtension(const String& Filename)
{
	String	Ext;
		
	int iPos = Filename.rfind('.');

	if (iPos != string::npos)
	{
		return Filename.substr(iPos);
	}

	return _T("");
}

// Returns the portion of a full path w/o the filename
String CPath::ExtractPathname(const String& szFullPath)
{
	int iPos = szFullPath.rfind('\\');

	if (iPos != string::npos)
	{
		return szFullPath.substr(0,iPos);
	}
	else
	{
		return szFullPath;
	}
}

// Returns the path to the \My Documents\Personal directory
String	CPath::GetPersonalDir()
{
	TCHAR	Path[MAX_PATH];

	SHGetSpecialFolderPath(0, Path, CSIDL_PERSONAL, true);

	if (!_tcslen(Path))
	{
		// The call failed. Provide some default.
		return BuildPath(EXPORT_LOCATION);
	}

	return Path;
}
