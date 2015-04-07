#ifndef _INC_CPocketStreetPushPinsWriter
	#define _INC_CPocketStreetPushPinsWriter

#include "CommonDefs.h"
#include "CGPSWriter.h"

class CPocketStreetPushPinsWriter : public CGPSWriter
{
public:
	CPocketStreetPushPinsWriter();
	~CPocketStreetPushPinsWriter();

	// Set the total number of push pins to be written to the file header
	void			SetTotalPushPins(short int Count);

	virtual	void	Open(const TCHAR* pFilename);

	virtual void	OnCache(CGeoCache& Cache);

protected:
	void	le_write16(void *addr, unsigned value);
	char	grid_byte(double lat, double lon);
	void	test_endianness();
	void	psp_fwrite_double(double x, FILE *fp);

protected:
	short	int	m_TotalPushPins;
	short	int m_PIndex;
	int		m_endianness_tested;
	int		m_i_am_little_endian;
};

#endif
