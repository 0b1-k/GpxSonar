#ifndef _INC_CMeridianWriter
	#define _INC_CMeridianWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CMeridianWriter : public CGPSWriter
{
public:
	CMeridianWriter();
	~CMeridianWriter();

	virtual void OnCache(CGeoCache& Cache);

protected:
	unsigned int Checksum(const char* pStr);
};

#endif
