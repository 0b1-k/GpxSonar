#ifndef _INC_CStream
	#define _INC_CStream

#include "CommonDefs.h"
#include "CMd5.h"

class CStream
{
	#define CStreamVersion		100

protected:

	StringList	m_List;

	CMd5		m_Md5;

	String		m_szDescriptor;
	String		m_szTemp;
	TCHAR		m_cSaved;
	TCHAR		m_DataBuffer[80];
	TCHAR		m_Header[25];
	TCHAR*		m_pBuffer;
	TCHAR*		m_pUnpack;
	long		m_lBufferSizeInChars;
	long		m_lLastValLen;
	long		m_lUnicode;
	bool		m_bIsStoring;

public:
	CStream();
	virtual ~CStream();

	// Assign a user-defined descriptor (metadata) to the stream
	void			SetDescriptor(const String& szDescriptor);
	// Retrieve the user-defined descriptor (metadata) on the stream
	String			GetDescriptor();

	// Set to 'true' when inserting data into the stream. Set to 'false' when extracting data from the stream.
	void			SetStoring(bool IsStoring);
	// Returns 'true' when storing to the stream and false when extracting from it.
	bool			IsStoring();

	// Stream insertion operators
	void operator<<(TCHAR* pszStr);
	void operator<<(const String& str);
	void operator<<(DWORD dword);
	void operator<<(short Short);
	void operator<<(float Float);
	void operator<<(double Double);
	void operator<<(WORD word);
	void operator<<(long Long);
	void operator<<(int Int);
	void operator<<(UINT UInt);
	void operator<<(bool Boolean);
	void operator<<(SYSTEMTIME& ST);
	void operator<<(LONGLONG& LongLong);

	// Stream extraction operators
	void operator>>(String&);
	void operator>>(DWORD&);
	void operator>>(short& Short);
	void operator>>(float& Float);
	void operator>>(double& Double);
	void operator>>(WORD&);
	void operator>>(long& Long);
	void operator>>(int&);
	void operator>>(UINT&);
	void operator>>(bool&);
	void operator>>(SYSTEMTIME& ST);
	void operator>>(LONGLONG& LongLong);

	// Creates a string buffer containing the data inserted with the overloaded operators.
	// Throws a CBaseException in case of memory allocation failure.
	THROWx	void	Pack(DWORD& dwSizeInBytes, TCHAR** Ptr, bool bAddNullTerminator = false);

	// Assign a buffer to the stream before using the extraction operators
	void			Unpack(TCHAR* pBuffer);

	// returns the size of the buffer in characters
	long			GetSize();
	
	// Saves the stream from memory to disk
	// Throws a CBaseException in case of error
	THROWx	void	Save(const String& szFilename);

	// Loads a stream from disk back into memory
	// Throws a CBaseException in case of error
	THROWx	void	Load(const String& szFilename);

	// Retrieves the pointer to the next value and its size
	void			GetNextPointer(TCHAR** ppString, DWORD& Size);

	// Returns the current position of the pointer 
	TCHAR*			GetUnpackPointer();

	// Resets the stream object
	void			Reset();

protected:
	// Add a string to the list
	virtual	void	Push(const String& szBuffer, int iDataLen);

	// Used by the extraction operators to move to the next value to return
	virtual	TCHAR*	GetNext();

	// Used to synch pointers and restore data altered by GetNext()
	virtual	void	Synch();

	// Helpers dealing with WORD data types
	void			PutWord(WORD Word);
	void			GetWord(WORD& Word);

	// Allocate a buffer and throws an exception if it fails.
	THROWx TCHAR*	AllocateBuffer(DWORD dwSize);

	// Destroys the buffer allocated with Pack()
	void			DestroyBuffer();

	// Writes the header preceding the data
	// Throws a CBaseException in case of error
	THROWx void		WriteHeader(FILE* hFile);
		
	// Writes a chunk of data to disk and throws an exception in case of a problem
	THROWx void		Write(void* pData, long lSize, FILE* hFile);

	// Reads the header preceding the data. Returns the MD5 saved in the header
	// Throws a CBaseException in case of error
	THROWx String	ReadHeader(FILE* hFile);

	// Reads a chunk of data from disk and throws an exception in case of a problem
	THROWx void		Read(void* pData, long lSize, FILE* hFile);
};

#endif
