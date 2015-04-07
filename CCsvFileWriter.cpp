#include "CCsvFileWriter.h"
#include "CGpxParser.h"
#include <stdio.h>

CCsvFileWriter::CCsvFileWriter()
{
}

CCsvFileWriter::~CCsvFileWriter()
{
}

void CCsvFileWriter::Open(const TCHAR* pFilename)
{
	CGPSWriter::Open(pFilename);

	char Header[] = "\"Name\",\"Lat\",\"Lon\"\r\n";

	Write(Header);	
}

void CCsvFileWriter::OnCache(CGeoCache& Cache)
{
	AW_CONVERSION;

	fprintf(
		m_fd, 
		"\"%s :: %s\",\"%4.10f\",\"%4.10f\"\r\n",
		w2a((TCHAR*) Cache.m_Shortname.c_str()),
		w2a((TCHAR*) Cache.m_GsCacheName.c_str()),
		Cache.m_Lat,
		Cache.m_Long);
}

void CCsvFileWriter::Write(char* pStr)
{
	fwrite(pStr, sizeof(char), strlen(pStr), m_fd);
}