#ifndef _INC_CMxfFileWriter
	#define _INC_CMxfFileWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CMxfFileWriter : public CGPSWriter
{
public:
	CMxfFileWriter();
	~CMxfFileWriter();

	virtual void	OnCache(CGeoCache& Cache);
};

#endif
