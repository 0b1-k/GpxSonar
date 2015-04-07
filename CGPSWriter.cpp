#include "CGPSWriter.h"
#include "CGpxParser.h"
#include "CFieldNoteMgr.h"

CGPSWriter::CGPSWriter()
{
	m_fd = 0;
}

CGPSWriter::~CGPSWriter()
{
	Close();
}

void CGPSWriter::Open(const TCHAR* pFilename)
{
	Close();

	m_fd = _tfopen(pFilename, _T("w+b"));
}

void CGPSWriter::Close()
{
	if (m_fd)
	{
		fclose(m_fd);
		m_fd = 0;
	}
}

void CGPSWriter::ProcessCache(CGeoCache& Cache)
{
	if (Cache.m_pFieldNote)
	{
		// If the field note has user-provided actual cache coords then,
		// substitute the coords of the cache with them.
		if (Cache.m_pFieldNote->m_Lat != 0.0 && Cache.m_pFieldNote->m_Long != 0.0)
		{
			// Backup the original coords;
			double OrgLat = Cache.m_Lat;
			double OrgLong = Cache.m_Long;

			// Use the actual coords provided by the user
			Cache.m_Lat = Cache.m_pFieldNote->m_Lat;
			Cache.m_Long = Cache.m_pFieldNote->m_Long;

			// Process the cache normally
			OnCache(Cache);

			// Restore the original coords
			Cache.m_Lat = OrgLat;
			Cache.m_Long = OrgLong;
		}
		else
		{
			// The field note does not have actual coords provided by the user
			OnCache(Cache);
		}
	}
	// No field note
	else
	{
		OnCache(Cache);
	}
}

// converts a rating to a string
string CGPSWriter::RatingToString(double Val)
{
	int Int = (int) (Val * 10.0);

	char Buffer[MAX_SMALL_BUFFER];

	_snprintf(Buffer, MAX_SMALL_BUFFER - 1, "%d", Int);

	return Buffer;
}

// converts a rating to a string
wstring CGPSWriter::RatingToStringW(double Val)
{
	int Int = (int) (Val * 10.0);

	TCHAR Buffer[MAX_SMALL_BUFFER];

	_sntprintf(Buffer, MAX_SMALL_BUFFER - 1, _T("%d"), Int);

	return Buffer;
}

// Removes unwanted characters from a string and return the 'cleansed' string
string CGPSWriter::Clean(const char* pStr)
{
	static char ValidChars[] = 
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ "
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789-.'/";

	string Out;

	long Size = strlen(pStr);

	Out.reserve(Size);

	for (long I = 0; I < Size; I++) 
	{
		if (strchr(ValidChars, pStr[I])) 
		{
			Out += pStr[I];
		}
	}
	
	return Out;
}

