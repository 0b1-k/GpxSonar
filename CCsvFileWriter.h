#ifndef _INC_CCsvFileWriter
	#define _INC_CCsvFileWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CCsvFileWriter : public CGPSWriter
{
public:
	CCsvFileWriter();
	~CCsvFileWriter();

	virtual	void	Open(const TCHAR* pFilename);
	virtual void	OnCache(CGeoCache& Cache);

protected:
	void Write(char* pStr);
};

#endif
