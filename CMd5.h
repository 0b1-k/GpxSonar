#ifndef _INC_CMd5
	#define _INC_CMd5

#include "CommonDefs.h"
#include "md5.h"

class CMd5
{
	MD5_CTX			m_Context;
	unsigned char	m_Digest[16];

public:
	CMd5();

	// Initialize a new incremental hash sequence
	void	Init();
	
	// Update the hash with a new chunk of data
	void	Update(BYTE* pDataIn, DWORD DataInSize);

	// Finalize the hash and return the MD5 as a string
	String	Final();

	// Generates a MD5 hash of a buffer given its size and return a string version of the hash
	String	Hash(BYTE* pDataIn, DWORD DataInSize);

protected:
	String Md5AsString();
};

#endif
