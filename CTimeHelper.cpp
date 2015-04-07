#include "CTimeHelper.h"

CTimeHelper::CTimeHelper(SYSTEMTIME& Time)
{
	m_Time = Time;
}

String CTimeHelper::Date()
{
	#define MAX_BUFFER	50
	
	TCHAR Buffer[MAX_BUFFER];

	_sntprintf(
		Buffer, MAX_BUFFER, 
		_T("%d-%02d-%02d"), 
		m_Time.wYear,
		m_Time.wMonth,
		m_Time.wDay);

	ReplaceSpaceWithZero(Buffer);

	return Buffer;
}

String CTimeHelper::Time()
{
	#define MAX_BUFFER	50

	TCHAR Buffer[MAX_BUFFER];

	_sntprintf(
		Buffer, MAX_BUFFER, 
		_T("%02d:%02d:%02d"), 
		m_Time.wHour,
		m_Time.wMinute,
		m_Time.wSecond);

	ReplaceSpaceWithZero(Buffer);

	return Buffer;
}

String CTimeHelper::DateTime()
{
	#define MAX_BUFFER	50

	TCHAR Buffer[MAX_BUFFER];

	String	DateTime;

	DateTime = Date();
	DateTime += _T(", ");

	_sntprintf(
		Buffer, MAX_BUFFER, 
		_T("%02d:%02d:%02d"), 
		m_Time.wHour,
		m_Time.wMinute,
		m_Time.wSecond);

	ReplaceSpaceWithZero(Buffer);

	DateTime += Buffer;

	return DateTime;
}

void CTimeHelper::ReplaceSpaceWithZero(TCHAR* pBuffer)
{
	for (int I = 0; pBuffer[I]; I++)
	{
		if (pBuffer[I] == _T(' '))
		{
			pBuffer[I] = _T('0');
		}
	}
}

