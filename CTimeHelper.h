#ifndef _INC_CTimeHelper
	#define _INC_CTimeHelper

#include "CommonDefs.h"

class CTimeHelper
{
private:
	SYSTEMTIME	m_Time;

public:
	CTimeHelper(SYSTEMTIME& Time);

	String	Date();
	String	Time();
	String	DateTime();
protected:
	void	ReplaceSpaceWithZero(TCHAR* pBuffer);
};

#endif
