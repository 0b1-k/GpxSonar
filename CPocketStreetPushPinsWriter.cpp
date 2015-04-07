#include "CPocketStreetPushPinsWriter.h"
#include "CGpxParser.h"
#include "math.h"
#include <stdio.h>

CPocketStreetPushPinsWriter::CPocketStreetPushPinsWriter()
{
	m_TotalPushPins = 0;
	m_PIndex = 0;
}

CPocketStreetPushPinsWriter::~CPocketStreetPushPinsWriter()
{
}

// Set the total number of push pins to be written to the file header
void CPocketStreetPushPinsWriter::SetTotalPushPins(short int Count)
{
	m_TotalPushPins = Count;
}

void CPocketStreetPushPinsWriter::Open(const TCHAR* pFilename)
{
	CGPSWriter::Open(pFilename);

	unsigned char header_bytes[] = { 0x31, 0x6E, 0x69, 0x50, 0x20, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    /* the header: */
    /* 31 6E 69 50 20 00 00 00 08 00 00 00 11 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 */
    /* offset 0x0C - 0x0D = 2 byte pin count */

	/* insert waypoint count into header */
	le_write16(&header_bytes[12], m_TotalPushPins);

	fwrite(header_bytes, 1, 32, m_fd);
}

// Called for each cache to write out the corresponding waypoint to be used by the GPS
void CPocketStreetPushPinsWriter::OnCache(CGeoCache& Cache)
{
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

	AW_CONVERSION;

	char tbuf[64];
	int i;

	/* convert lat/long back to radians */
	double lat = (Cache.m_Lat * M_PI) / 180.0;
    double lon = (Cache.m_Long * M_PI) / 180.0;
        
	m_PIndex++;

	le_write16(tbuf, m_PIndex);
        
	/* 2 bytes - pin index */
    fwrite(tbuf, 1, 2, m_fd);
        
    /* 2 bytes - null bytes */
    memset(tbuf, '\0', sizeof(tbuf));
    fwrite(tbuf, 1, 2, m_fd);
        
	/* set the grid byte */
	char c = grid_byte(Cache.m_Lat, Cache.m_Long);

	/* since the grid byte matches with what pocketstreets does to   */
	/* input files, our output appears identical to a pin file that  */
    /* has already been processed and corrected by pocketstreets.    */
    /* Due to the grid and signs, it'll look different than one that */
    /* comes straight from S&T.                                      */
	
    /* the grid byte */
    fwrite(&c, 1, 1, m_fd);

    /* 8 bytes - latitude/radians */
	psp_fwrite_double(lat, m_fd);

    /* 8 bytes - longitude/radians */
	psp_fwrite_double(lon, m_fd);

    /* 1 byte - pin properties */
    c = 0x14; /* display pin name on, display notes on. 0x04 = no notes */
    fwrite(&c, 1, 1, m_fd);

    memset(tbuf, '\0', sizeof(tbuf));

    /* 3 unknown bytes */
    fwrite(tbuf, 1, 3, m_fd);

	char Icon[2] = {0,0};

	/*
	Traditional is a red box with a flag
	Mystery Cache is the pushpin.
	Multi is the Flags
	Event is the bubble text icon.
	Virtual is the arrow pointing right.
	*/

	#define	IconPin				0
	#define	IconBubble			1
	#define	IconRedBoxFlag		4
	#define	IconArrowToLeft		8
	#define	IconArrowToRight	9
	#define	IconArrowDown		10
	#define	IconFlags			18
	#define	IconLetter			29

	switch (Cache.TypeLookup())
	{
	case GT_Traditional:
		Icon[0] = IconRedBoxFlag;
		break;
	case GT_Multi:
		Icon[0] = IconFlags;
		break;
	case GT_Virtual:
		Icon[0] = IconArrowToRight;
		break;
	case GT_Webcam:
		Icon[0] = IconArrowDown;
		break;
	case GT_Unknown:
		Icon[0] = IconPin;
		break;
	case GT_LetterboxHybrid:
		Icon[0] = IconLetter;
		break;
	case GT_Event:
		Icon[0] = IconBubble;
		break;
	case GT_ProjectAPE:
		Icon[0] = IconRedBoxFlag;
		break;
	case GT_Locationless:
		Icon[0] = IconArrowToLeft;
		break;
	case GT_CITO:
		Icon[0] = IconBubble;
		break;
	case GT_Earthcache:
		Icon[0] = IconArrowToRight;
		break;
	}

    /* 1 icon byte 0x00 = PIN */
    fwrite(&Icon, 1, 1, m_fd);

    /* 3 unknown bytes */
    fwrite(tbuf, 1, 3, m_fd); /* 3 junk */

	//---------------------------------

	String Desc;

	Desc = Cache.m_Shortname;

	char* pDesc = w2a((TCHAR*) Desc.c_str());

    c = strlen(pDesc);

    /* 1 string size */
    fwrite(&c, 1, 1, m_fd);

    for (i = 0; pDesc[i]; i++) 
	{
		fwrite(&pDesc[i], 1, 1, m_fd);	/* char */
		fwrite(&tbuf[0], 1, 1, m_fd);		/* null */
    }

	//---------------------------------

	Desc = Cache.m_GsCacheType;
	Desc += _T(". ");
	Desc += Cache.m_GsCacheName;
	Desc += _T(" by ");
	Desc += Cache.m_GsCacheOwnerName;
	Desc += _T(". Rating: ");
	Desc += RatingToStringW(Cache.m_GsCacheDifficulty);
	Desc += _T("/");
	Desc += RatingToStringW(Cache.m_GsCacheTerrain);

	TCHAR		Buffer[20];

	Cache.LastFoundText(Buffer);

	Desc += _T(". Last found: ");
	Desc += Buffer;

	pDesc = w2a((TCHAR*) Desc.c_str());

	c = strlen(pDesc);

    /*  1 byte string size */
    fwrite(&c, 1, 1, m_fd);

    for (i = 0; pDesc[i]; i++)
	{
		fwrite(&pDesc[i], 1, 1, m_fd);	/* char */
        fwrite(&tbuf[0], 1, 1, m_fd);	/* null */
    }

	//---------------------------------

	// Scrap the last string

    c = strlen(tbuf);

    /* 1 byte string size */
    fwrite(&c, 1, 1, m_fd);

	for (i = 0; tbuf[i]; i++) 
	{
		fwrite(&tbuf[i], 1, 1, m_fd);              /* char */
		fwrite(&tbuf[0], 1, 1, m_fd);              /* null */
	}
}

void CPocketStreetPushPinsWriter::le_write16(void *addr, unsigned value)
{
	unsigned char *p = (unsigned char *) addr;
	p[0] = value;
	p[1] = value >> 8;	
}

/* Implement the grid in ascii art... This makes a bit of sense if you stand
   on a point over the north pole and look down on the earth.

-180   -90        0       90      180 
------------------------------------    /\
| 0x03  U|S 0x02 U|k 0x00  | 0x01   |   90
|--------|--------|--------|--------|   0
| 0x07   |  0x06  |  0x04  | 0x05   |   -90
------------------------------------    \/
*/    
char CPocketStreetPushPinsWriter::grid_byte(double lat, double lon)
{
    char c = 0x00;
    
    if ((lon >= 0.0) && (lon < 90.0)) {
        if (lat >= 0.0) {
            c = 0x00;
        } else {
            c = 0x04;
        }
    } else
    if (lon >= 90.0) {
        if (lat >= 0.0) {
            c = 0x01;
        } else {
            c = 0x05;
        }
    } else
    if ((lon < 0.0) && (lon >= -90.0)) {
        if (lat >= 0.0) {
            c = 0x02;
        } else {
            c = 0x06;
        }
    } else 
    if (lon < -90.0) {
        if (lat >= 0.0) {
            c = 0x03;
        } else {
            c = 0x07;
        }
    }
        
    return (c);
}

void CPocketStreetPushPinsWriter::psp_fwrite_double(double x, FILE *fp)
{
	unsigned char *cptr = (unsigned char *)&x;
	unsigned char cbuf[8];

	if (!m_endianness_tested) 
	{
		test_endianness();
	}
	
	if (m_i_am_little_endian) 
	{
		fwrite(&x, 8, 1, fp);
	} 
	else 
	{
		cbuf[0] = cptr[7];
		cbuf[1] = cptr[6];
		cbuf[2] = cptr[5];
		cbuf[3] = cptr[4];
		cbuf[4] = cptr[3];
		cbuf[5] = cptr[2];
		cbuf[6] = cptr[1];
		cbuf[7] = cptr[0];

		fwrite(cbuf, 8, 1, fp);
	}
}

void CPocketStreetPushPinsWriter::test_endianness()
{
	union {
		long l;
		unsigned char uc[sizeof (long)];
	} u;

	u.l = 1;

	m_i_am_little_endian = u.uc[0];

	m_endianness_tested = 1;
}