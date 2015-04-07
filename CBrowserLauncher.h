#ifndef _INC_CBrowserLauncher
	#define _INC_CBrowserLauncher

#include "CommonDefs.h"

class CBrowserLauncher
{
public:
	CBrowserLauncher();
	
	void	Go(const String& Url, const String& Args = _T(""), int ShowCmd = SW_SHOW);
};

#endif
