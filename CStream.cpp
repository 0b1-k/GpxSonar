#include "CStream.h"
#include "CBaseException.h"

CStream::CStream()
{
	m_pBuffer = NULL;
	m_pUnpack = NULL;
	m_lBufferSizeInChars = 0;
	m_lLastValLen = 0;
	m_bIsStoring = false;
	m_cSaved = 0;

	#ifdef _UNICODE
	m_lUnicode = 0xFFFE;
	#else
	m_lUnicode = 0x0000;
	#endif
}

CStream::~CStream()
{
	Reset();
}

// Set to 'true' when inserting data into the stream. Set to 'false' when extracting data from the stream.
void CStream::SetStoring(bool bIsStoring)
{
	m_bIsStoring = bIsStoring;

	m_Md5.Init();
}

// Returns 'true' when storing to the stream and false when extracting from it.
bool CStream::IsStoring()
{
	return m_bIsStoring;
}

// Add a string to the list
void CStream::Push(const String& szStr, int iDataLen)
{
	m_szTemp.reserve(_tcslen(m_Header) + iDataLen);

	// Concatenate the header + the data into one string
	m_szTemp = m_Header;
	m_szTemp += szStr;

	// Update the Md5 with each successive string that gets stored
	m_Md5.Update((BYTE*) m_szTemp.c_str(), m_szTemp.size() * sizeof(TCHAR));

	// Store the string into the list
	m_List.push_back(m_szTemp);

	// Add the size of the new string to the total length of the buffer required to hold all the strings when packing.
	m_lBufferSizeInChars += m_szTemp.size();
}

// Used by the extraction operators to move to the next value to return
TCHAR* CStream::GetNext()
{
	static TCHAR szNothing[] = _T("");

	if (!m_pUnpack)
	{
		return szNothing;
	}

	// Retrieve the size of the next value in characters
	m_lLastValLen = _tcstol(m_pUnpack, NULL, 10);

	if (!m_lLastValLen && (m_pUnpack >= (m_pBuffer + m_lBufferSizeInChars)))
	{
		m_cSaved = 0;
		return szNothing;
	}

	// Move to the '='
	while (*m_pUnpack != _T('=')) m_pUnpack++;

	// skip the '='
	m_pUnpack++;

	// Save the character after the last character of the current value
	m_cSaved = m_pUnpack[m_lLastValLen];

	// Replace the character with a NULL
	m_pUnpack[m_lLastValLen] = _T('\0');

	// return the pointer to the current value
	return m_pUnpack;
}

// Used to synch pointers and restore data altered by GetNext()
void CStream::Synch()
{
	if (m_lLastValLen || m_cSaved)
	{
		// Restore the character that was replaced by a NULL in GetNext()
		m_pUnpack[m_lLastValLen] = m_cSaved;
		// Move to the next value to be retrieved
		m_pUnpack += m_lLastValLen;
	}
}

// Size is expressed in characters
void CStream::GetNextPointer(TCHAR** ppString, DWORD& Size)
{
	if(!m_pUnpack)
	{
		*ppString = NULL;
		Size = 0;
		return;
	}

	m_lLastValLen = _tcstol(m_pUnpack, NULL, 10);

	Size = m_lLastValLen;

	if (m_lLastValLen)
	{
		while (*m_pUnpack != _T('=')) m_pUnpack++;

		// skip the '='
		m_pUnpack++;

		*ppString = m_pUnpack;

		m_pUnpack += m_lLastValLen;
	}
	else
	{
		*ppString = NULL;
	}
}

// returns the size of the CStream buffer in characters
long CStream::GetSize()
{
	return m_lBufferSizeInChars;
}

// Saves the stream from memory to disk
// Throws a CBaseException in case of error
THROWx void CStream::Save(const String& szFilename)
{
	FILE* hFile = _tfopen((TCHAR*) szFilename.c_str(), _T("w"));

	if (hFile == NULL)
	{
		CBaseException Up;

		Up.m_szSrc = _T("CStream::Save()");
		Up.m_szMsg = _T("Failed to create file: ") + szFilename;
		Up.Win32Error();
		Up.Log();

		//throw Up;
	}
	
	//try
	{
		WriteHeader(hFile);

		for (itStr it = m_List.begin(); it != m_List.end(); it++)
		{
			Write((void*) (*it).c_str(), (*it).size() * sizeof(TCHAR), hFile);
		}
	}
	/*
	catch(CBaseException& e)
	{
		fclose(hFile);

		e.m_szMsg = _T("Failed while writting to: ") + szFilename;
		e.Win32Error();
		e.Log();
		//throw e;
	}
	*/
	fclose(hFile);
}

// Writes the header preceding the data
// Throws a CBaseException in case of error
THROWx void CStream::WriteHeader(FILE* hFile)
{
	// Write the Unicode identifier
	Write((void*) &m_lUnicode, sizeof(m_lUnicode), hFile);

	// Write the MD5 string
	String szMd5 = m_Md5.Final();
	int iMd5Size = szMd5.size();
	Write((void*) &iMd5Size, sizeof(iMd5Size), hFile);
	Write((void*) szMd5.c_str(), iMd5Size * sizeof(TCHAR), hFile);

	// Write the metadata info
	int iDescriptorSize = m_szDescriptor.size();
	Write((void*) &iDescriptorSize, sizeof(iDescriptorSize), hFile);
	Write((void*) m_szDescriptor.c_str(), m_szDescriptor.size() * sizeof(TCHAR), hFile);

	// Write the size of the data itself (in characters)
	Write((void*) &m_lBufferSizeInChars, sizeof(m_lBufferSizeInChars), hFile);
}

// Writes a chunk of data to disk and throws an exception in case of a problem
THROWx void CStream::Write(void* pData, long lSize, FILE* hFile)
{
	long lWritten = fwrite(pData, 1, lSize, hFile);

	if (lWritten != lSize)
	{
		CBaseException Up;
		Up.m_szSrc = _T("CStream::Write()");
	}
}

// Loads a stream from disk back into memory
// Throws a CBaseException in case of error
THROWx void CStream::Load(const String& szFilename)
{
	FILE* hFile = _tfopen((TCHAR*) szFilename.c_str(), _T("r"));

	if (hFile == NULL)
	{
		CBaseException Up;

		Up.m_szSrc = _T("CStream::Load()");
		Up.m_szMsg = _T("Failed to open file: ") + szFilename;
		Up.Win32Error();

		//throw Up;
	}
	
	//try
	{
		// Clean everything up before loading the data from disk
		Reset();

		// Read the header preceding the data. Returns the size of the buffer required to read the data in one chunk.
		String szMd5 = ReadHeader(hFile);

		// Allocate a buffer large enough to hold the data section in the file.
		// The buffer needs to be a bit larger due to the extraction process
		if (m_lBufferSizeInChars)
		{
			// allocate a target character buffer large enough to receive all the packed data
			m_pBuffer = new TCHAR[m_lBufferSizeInChars + 2];

			if (!m_pBuffer)
			{
				CBaseException Up;

				Up.m_szSrc = _T("CStream::Load()");
				Up.m_szMsg = _T("Memory allocation failure! File: ") + szFilename;
				Up.Log();

				fclose(hFile);

				//throw Up;
			}

			// Read the whole chunk of data into the buffer.
			Read((void*) m_pBuffer, m_lBufferSizeInChars * sizeof(TCHAR), hFile);

			// Terminate the buffer with a null
			m_pBuffer[m_lBufferSizeInChars] = _T('\0');

			// Calculate the MD5 on the data and compare with the header
			if (m_Md5.Hash((BYTE*) m_pBuffer, m_lBufferSizeInChars * sizeof(TCHAR)) != szMd5)
			{
				CBaseException Up;

				Up.m_szSrc = _T("CStream::Load()");
				Up.m_szMsg = _T("Md5 mismatch! The file: ") + szFilename + _T(" is corrupt!");
				Up.Log();

				fclose(hFile);

				//throw Up;
			}

			// Set the data extraction buffer on the data
			m_pUnpack = m_pBuffer;
		}
	}
	/*
	catch(CBaseException& e)
	{
		fclose(hFile);

		e.m_szMsg = _T("Failed while reading from: ") + szFilename;
		e.Win32Error();
		e.Log();
		//throw e;
	}
*/
	fclose(hFile);
}

// Reads the header preceding the data. Returns the MD5 saved in the header
// Throws a CBaseException in case of error
THROWx String CStream::ReadHeader(FILE* hFile)
{
	// Read the Unicode identifier
	Read((void*) &m_lUnicode, sizeof(m_lUnicode), hFile);

	// Read the MD5 string
	int iMd5Size = 0;
	
	Read((void*) &iMd5Size, sizeof(iMd5Size), hFile);
	
	// Allocate a buffer for the Md5 string
	TCHAR* pMd5 = new TCHAR[iMd5Size+1];

	if (!pMd5)
	{
		CBaseException Up;
		Up.m_szSrc = _T("CStream::ReadHeader()");
		Up.m_szMsg = _T("Failed to allocate buffer for Md5!");
		Up.Log();
		//throw Up;
	}

	// Read the MD5 back in
	Read((void*) pMd5, iMd5Size * sizeof(TCHAR), hFile);

	// Save the Md5 and destroy the buffer
	pMd5[iMd5Size] = _T('\0');
	String szMd5 = pMd5;
	delete [] pMd5;

	// Read the metadata info
	int iDescriptorSize = 0;

	Read((void*) &iDescriptorSize, sizeof(iDescriptorSize), hFile);

	if (iDescriptorSize)
	{
		// Allocate a buffer for the descriptor
		TCHAR* pDesc = new TCHAR[iDescriptorSize+1];

		if (!pDesc)
		{
			CBaseException Up;
			Up.m_szSrc = _T("CStream::ReadHeader()");
			Up.m_szMsg = _T("Failed to allocate buffer for descriptor!");
			Up.Log();
			//throw Up;
		}
		
		// Read the descriptor back in
		Read((void*) pDesc, iDescriptorSize * sizeof(TCHAR), hFile);

		// Save the descriptor and destroy the buffer
		pDesc[iDescriptorSize] = _T('\0');
		m_szDescriptor = pDesc;
		delete [] pDesc;
	}
	else
	{
		m_szDescriptor = _T("");
	}

	// Read the size of the data itself (in characters)
	Read((void*) &m_lBufferSizeInChars, sizeof(m_lBufferSizeInChars), hFile);

	return szMd5;
}

// Reads a chunk of data from disk and throws an exception in case of a problem
THROWx void CStream::Read(void* pData, long lSize, FILE* hFile)
{
	long lRead = fread(pData, 1, lSize, hFile);

	if (lRead != lSize)
	{
		CBaseException Up;
		Up.m_szSrc = _T("CStream::Read()");
	}
}

// Returns the current position of the pointer 
TCHAR *CStream::GetUnpackPointer()
{
	return m_pUnpack;
}

// Creates a string buffer containing the data inserted with the overloaded operators.
// Throws a CBaseException in case of memory allocation failure.
THROWx void CStream::Pack(DWORD& dwSizeInBytes, TCHAR** Ptr, bool bAddNullTerminator)
{
	DestroyBuffer();

	if(!m_List.size())
	{
		*Ptr = NULL;
		dwSizeInBytes = 0;
		return;
	}

	if (bAddNullTerminator)
	{
		// provide the length of the buffer in bytes
		dwSizeInBytes = (m_lBufferSizeInChars + 2) * sizeof(TCHAR);
	}
	else
	{
		// provide the length of the buffer in bytes
		dwSizeInBytes = m_lBufferSizeInChars * sizeof(TCHAR);
	}

	// allocate a target character buffer large enough to receive all the packed data
	m_pBuffer = AllocateBuffer(dwSizeInBytes);

	TCHAR* pDest = m_pBuffer;

	// Concatenate the strings into the target buffer
	for (itStr it = m_List.begin(); it != m_List.end(); it++)
	{
		CopyMemory((void*) pDest, (const void*) (*it).c_str(), (*it).size() * sizeof(TCHAR));

		// Move the pointer to the next copy location
		pDest += (*it).size();
	}

	// NULL terminate the string if needed
	if (bAddNullTerminator)
	{
		*pDest = 0;
		pDest++;
		*pDest = 0;
	}

	// Return the pointer
	*(Ptr) = m_pBuffer;
}

// Assign a buffer to the stream before using the extraction operators
void CStream::Unpack(TCHAR* pBuffer)
{
	Reset();

	m_pUnpack = pBuffer;

	SetStoring(false);
}

// Resets the stream object
void CStream::Reset()
{
	DestroyBuffer();

	m_List.clear();
}

// Allocate a buffer and throws an exception if it fails.
THROWx TCHAR* CStream::AllocateBuffer(DWORD dwSize)
{
	TCHAR* pBuffer = new TCHAR[dwSize];

	if (!pBuffer)
	{
		CBaseException Up;
		Up.m_dwError = 0;
		Up.m_szSrc = _T("CStream::Pack()");
		Up.m_szMsg = _T("Memory allocation failure!");
		Up.Log();
		//throw Up;
	}

	return pBuffer;
}

// Destroys the buffer allocated with Pack()
void CStream::DestroyBuffer()
{
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
		m_pUnpack = NULL;
		m_lBufferSizeInChars = 0;
	}
}

//
// Overloaded insertion operators
//

void CStream::operator<<(TCHAR* pszStr)
{
	long  lLen = _tcslen(pszStr);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%li="), lLen);

	Push(pszStr, lLen);
}

void CStream::operator<<(const String& str)
{
	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), str.size());

	Push(str.c_str(), str.size());
}

void CStream::operator<<(DWORD dword)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%lu"), dword);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(float Float)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%.5f"), Float);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(double Double)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%.5f"), Double);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(LONGLONG& LongLong)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%I64d"), LongLong);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(WORD word)
{
	PutWord(word);
}

void CStream::PutWord(WORD word)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%i"), word);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(short Short)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%i"), Short);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(long Long)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%i"), Long);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(int Int)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%i"), Int);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(UINT UInt)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%u"), UInt);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(bool Boolean)
{
	_sntprintf(m_DataBuffer, sizeof(m_DataBuffer) - sizeof(TCHAR), _T("%i"), Boolean);

	int iDataLength = _tcslen(m_DataBuffer);

	_sntprintf(m_Header, sizeof(m_Header) - sizeof(TCHAR), _T("%i="), iDataLength);

	Push(m_DataBuffer, iDataLength);
}

void CStream::operator<<(SYSTEMTIME& ST)
{
	PutWord(ST.wYear);
	PutWord(ST.wMonth);
	PutWord(ST.wDayOfWeek);
	PutWord(ST.wDay);
	PutWord(ST.wHour);
	PutWord(ST.wMinute);
	PutWord(ST.wSecond);
	PutWord(ST.wMilliseconds);
}

//
// Overloaded extraction operators
//

void CStream::operator>>(String& szStr)
{
	szStr = GetNext();
	Synch();
}

void CStream::operator>>(DWORD& Dword)
{
	Dword = _tcstoul(GetNext(),NULL,10);
	Synch();
}

void CStream::operator>>(float& Float)
{
	Float = _tcstod(GetNext(),NULL);
	Synch();
}

void CStream::operator>>(double& Double)
{
	Double = _tcstod(GetNext(),NULL);
	Synch();
}

void CStream::operator>>(LONGLONG& LongLong)
{
	LongLong = _ttol(GetNext());
	Synch();
}

void CStream::operator>>(long& Long)
{
	Long = _tcstol(GetNext(),NULL,10);
	Synch();
}

void CStream::operator>>(WORD& Word)
{
	GetWord(Word);
}

void CStream::GetWord(WORD& Word)
{
	long LTemp = _tcstol(GetNext(),NULL,10);
	Word = (WORD) LTemp;
	Synch();
}

void CStream::operator>>(int& Int)
{
	Int = _ttoi(GetNext());
	Synch();
}

void CStream::operator>>(short& Short)
{
	long LTemp = _tcstol(GetNext(),NULL,10);
	Short = LTemp;
	Synch();
}

void CStream::operator>>(UINT& UInt)
{
	long LTemp = _tcstol(GetNext(),NULL,10);
	UInt = LTemp;
	Synch();
}

void CStream::operator>>(bool& Boolean)
{
	long LTemp = _tcstol(GetNext(),NULL,10);
	Synch();

	if (LTemp)
	{
		Boolean = true;
	}
	else
	{
		Boolean = false;
	}
}

void CStream::operator>>(SYSTEMTIME& ST)
{
	GetWord(ST.wYear);
	GetWord(ST.wMonth);
	GetWord(ST.wDayOfWeek);
	GetWord(ST.wDay);
	GetWord(ST.wHour);
	GetWord(ST.wMinute);
	GetWord(ST.wSecond);
	GetWord(ST.wMilliseconds);
}

