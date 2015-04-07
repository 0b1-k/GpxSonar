#ifndef _INC_CNavigonWriter
	#define _INC_CNavigonWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CNavigonWriter : public CGPSWriter
{
public:
	CNavigonWriter();
	~CNavigonWriter();

	virtual void	OnCache(CGeoCache& Cache);
};

#endif
