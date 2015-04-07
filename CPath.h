#ifndef _INC_CPath
	#define _INC_CPath

#include "CommonDefs.h"

class CPath
{
protected:
	String	m_szProcPath;
	String	m_szProcName;

public:
	CPath();
	~CPath();

	// Returns a complete path using a relative path to the directory where the process was started from
	String	BuildPath(const String& RelPath);

	// Returns a path to a directory where log files can be written
	// By default, the name of the log file is that of the calling process. This can be changed by passing an optional filename.
	String	GetLogsDir(const String& szOptFilename = _T(""));

	// Returns the path to the \My Documents\Personal directory
	String	GetPersonalDir();

	// Returns the process name
	String	GetProcess();

	// Returns the path to the directory where the process was started from
	String	GetProcDir();

	// Returns the path to the directory used for temporary files
	String	GetTempDir();

	// Returns the portion of a full path containing the filename
	String	ExtractFilename(const String& szFullPath, bool StripExtension = false);

	// Returns the extension part of a filename
	String	ExtractExtension(const String& Filename);

	// Returns the portion of a full path w/o the filename
	String	ExtractPathname(const String& szFullPath);
};

#endif