#include "CMd5.h"

CMd5::CMd5()
{
}

// Generates a MD5 hash of a buffer given its size and return a string version of the hash
String CMd5::Hash(BYTE* pDataIn, DWORD DataInSize)
{
	MD5Init(&m_Context);
	MD5Update(&m_Context, pDataIn, DataInSize);
	MD5Final(m_Digest, &m_Context);

	return Md5AsString();
}

// Initialize a new incremental hash sequence
void CMd5::Init()
{
	MD5Init(&m_Context);
}

// Update the hash with a new chunk of data
void CMd5::Update(BYTE* pDataIn, DWORD DataInSize)
{
	MD5Update(&m_Context, pDataIn, DataInSize);
}

// Finalize the hash and return the MD5 as a string
String CMd5::Final()
{
	MD5Final(m_Digest, &m_Context);
	return Md5AsString();
}

String CMd5::Md5AsString()
{
	String szHashOut;
	
	// Reserve 34 characters
	szHashOut.reserve(34);

	for( int i = 0; i < 16; i++)
	{
		TCHAR Buffer[5];
		_stprintf(Buffer, _T("%02x"), m_Digest[i] );
		szHashOut += Buffer;
	}

	return szHashOut;
}
