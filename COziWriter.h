#ifndef _INC_COziWriter
	#define _INC_COziWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class COziWriter : public CGPSWriter
{
protected:
	int		m_Count;

public:
	COziWriter();
	~COziWriter();

	virtual	void	Open(const TCHAR* pFilename);
	virtual void	OnCache(CGeoCache& Cache);
};

#endif
