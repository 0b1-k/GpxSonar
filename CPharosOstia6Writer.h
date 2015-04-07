#ifndef _INC_CPharosOstia6Writer
	#define _INC_CPharosOstia6Writer

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CPharosOstia6Writer : public CGPSWriter
{
public:
	CPharosOstia6Writer();
	~CPharosOstia6Writer();

	virtual	void Open(const TCHAR* pFilename);

	virtual void OnCache(CGeoCache& Cache);

protected:
	long		DecimalCoordToOsti6Coord(double Value);
};

#endif
