#include "CBrowserLauncher.h"
#include "CPath.h"

CBrowserLauncher::CBrowserLauncher()
{
}

void CBrowserLauncher::Go(const String& Url, const String& Args, int ShowCmd)
{
	SHELLEXECUTEINFO SEI;

	memset((void*) &SEI, 0, sizeof(SEI));

	CPath P;

	SEI.cbSize = sizeof(SHELLEXECUTEINFO);
	SEI.hwnd = NULL;
	SEI.fMask = SEE_MASK_NOCLOSEPROCESS;
	SEI.lpVerb = _T("open");

	if (!Args.empty())
	{
		SEI.lpParameters = Args.c_str();
	}
	
	SEI.lpDirectory = _T("");
	SEI.lpFile = Url.c_str();
	SEI.nShow = ShowCmd;

	ShellExecuteEx(&SEI);
}
