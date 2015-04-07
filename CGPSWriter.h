#ifndef _INC_CGPSWriter
	#define _INC_CGPSWriter

#include "CommonDefs.h"

class CGeoCache;

#define MAX_SMALL_BUFFER	10
#define MAX_OUTPUT_BUFFER	250
#define MAX_TMP_BUFFER		100

class CGPSWriter
{
protected:
	FILE*	m_fd;

public:
	CGPSWriter();

	virtual ~CGPSWriter();

	virtual	void	Open(const TCHAR* pFilename);
	virtual	void	Close();

	virtual	void	ProcessCache(CGeoCache& Cache);

protected:
	virtual void	OnCache(CGeoCache& Cache) = 0;

	// converts a rating to an UTF-8 string
	string		RatingToString(double Val);

	// converts a rating to an Unicode string
	wstring		RatingToStringW(double Val);

	string		Clean(const char* pStr);
};

#endif
