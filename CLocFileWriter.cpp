#include "CLocFileWriter.h"
#include "CGpxParser.h"
#include <stdio.h>

CLocFileWriter::CLocFileWriter()
{
}

CLocFileWriter::~CLocFileWriter()
{
}

void CLocFileWriter::Open(const TCHAR* pFilename)
{
	CGPSWriter::Open(pFilename);

	char Header[] = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\r\n<loc version=\"1.0\" src=\"GPXSonar\">";

	Write(Header);	
}

void CLocFileWriter::Close()
{
	char Footer[] = "</loc>";

	Write(Footer);

	CGPSWriter::Close();
}

void CLocFileWriter::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

	Write("<waypoint>\r\n");

	fprintf(
		m_fd, 
		"<name id=\"%s\"><![CDATA[%s by %s]]></name>\r\n<coord lat=\"%4.20f\" lon=\"%4.20f\"/>\r\n",
		w2a((TCHAR*) Cache.m_Shortname.c_str()),
		w2a((TCHAR*) Cache.m_GsCacheName.c_str()),
		w2a((TCHAR*) Cache.m_GsCacheOwnerName.c_str()),
		Cache.m_Lat,
		Cache.m_Long);

	Write("<type>Geocache</type>\r\n");
	Write("</waypoint>\r\n");
}

void CLocFileWriter::Write(char* pStr)
{
	fwrite(pStr, sizeof(char), strlen(pStr), m_fd);
}