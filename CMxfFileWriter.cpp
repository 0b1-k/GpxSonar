#include "CMxfFileWriter.h"
#include "CGpxParser.h"
#include <stdio.h>

CMxfFileWriter::CMxfFileWriter()
{
}

CMxfFileWriter::~CMxfFileWriter()
{
}

void CMxfFileWriter::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

	// 35.97203, -87.13470, "Mountain Bike Heaven by susy1313", "GCEBB", "Mountain Bike Heaven by susy1313", ff0000, 47

	String Desc;
	
	Desc = Cache.m_GsCacheName;
	Desc += _T(" by ");
	Desc += Cache.m_GsCacheOwnerName;

	long Length = Desc.size();

	for (long Pos = 0; Pos < Length; Pos++)
	{
		if (Desc[Pos] == _T('\"'))
		{
			Desc.at(Pos) = _T('\'');
		}
		if (Desc[Pos] == _T(','))
		{
			Desc.at(Pos) = _T(';');
		}
	}

	fprintf(
		m_fd, 
		"%4.20f, %4.20f, \"%s\", \"%s\", \"%s\", ff0000, 47\r\n",
		Cache.m_Lat,
		Cache.m_Long,
		w2a((TCHAR*) Desc.c_str()),
		w2a((TCHAR*) Cache.m_Shortname.c_str()),
		w2a((TCHAR*) Desc.c_str()));
}