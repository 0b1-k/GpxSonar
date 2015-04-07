#ifndef _INC_CLocFileWriter
	#define _INC_CLocFileWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CLocFileWriter : public CGPSWriter
{
public:
	CLocFileWriter();
	~CLocFileWriter();

	virtual	void	Open(const TCHAR* pFilename);
	virtual	void	Close();
	virtual void	OnCache(CGeoCache& Cache);

protected:
	void Write(char* pStr);
};

#endif
