#ifndef _INC_CLowranceWriter
	#define _INC_CLowranceWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CLowranceWriter : public CGPSWriter
{
protected:
	int		m_Total;
	int		m_Count;

public:
	CLowranceWriter();
	~CLowranceWriter();

	void SetWaypointCount(int Total)
	{
		m_Total = Total;
	}

	virtual	void	Open(const TCHAR* pFilename);
	virtual	void	Close();

	virtual void OnCache(CGeoCache& Cache);

protected:
	size_t	fwrite4(int Val);
	size_t	fwrite2(short Val);
	double	lon_mm_to_deg(double x);
	long	lon_deg_to_mm(double x);
	double	lat_mm_to_deg(double x);
	long	lat_deg_to_mm(double x);
	long int lowranceusr_find_icon_number_from_desc(String& CacheType);

};

#endif
