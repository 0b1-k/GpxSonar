#ifndef _INC_CTextTrx
	#define _INC_CTextTrx

#include "CommonDefs.h"

class CTextTrx
{
	#define	ZEROA					0x0A
	#define	ZEROD					0x0D
	#define HTML_BREAK				_T("<BR>")
	#define HTML_BREAK_LOWERCASE	_T("<br>")
	#define HTML_BREAK_ASCII		"<BR>"
	#define CRLF					_T("\r\n")
	#define CRLF_ASCII				"\r\n"
	#define	EXCLUDED_CHARS			_T("\t\r\n\b ")

public:
	CTextTrx();

	// Replaces ZEROA (CR) to <BR> when creating HTML output
	String	CRToBR(const String& Src);

	// Replaces ZEROA (CR) to <BR> when creating HTML output
	string	CRToBR(const char* pText);

	// Replaces ZEROA (CR) with CR+LF when displaying text
	String	CRToCRLF(const String& Src);

	// Replaces all <BR> with CR+LF
	void	BRToCRLF(String& Src);


	// Strips unacceptable characters at the right of a string
	String	TrimRight(const String& Src);

	// Strips unacceptable characters at the left of a string
	String	TrimLeft(const String& Src);

	// Strips unacceptable characters at the right and the left of a string
	String	Trim(const String& Src);

	// In-place ROT13 conversion of a string
	void	Rot13(String& Str);

protected:
	String Convert(const String& Src, const String& Subst);
	string Convert(const char* pSrc, const char* pSubst);
};

#endif
