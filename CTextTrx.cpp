#include "CTextTrx.h"

CTextTrx::CTextTrx()
{
}

// Strips unacceptable characters at the right of a string
String CTextTrx::TrimRight(const String& Src)
{
	String Dest;

	Dest.reserve(Src.size());

	long Size = Src.size();

	// Skip to the first valid character if any
	for (long I = 0; I < Size; I++)
	{
		if (_tcschr(EXCLUDED_CHARS, Src[I]) == 0)
		{
			Dest += Src.substr(I);
			break;
		}
		
	}

	return Dest;
}

// Strips unacceptable characters at the left of a string
String CTextTrx::TrimLeft(const String& Src)
{
	String Dest;

	Dest.reserve(Src.size());

	long Size = Src.size();

	// Skip to the first valid character if any
	while(Size--)
	{
		if (_tcschr(EXCLUDED_CHARS, Src[Size]) == 0)
		{
			Dest += Src.substr(0, Size + 1);
			break;
		}
		
	}

	return Dest;
}

// Strips unacceptable characters at the right and the left of a string
String CTextTrx::Trim(const String& Src)
{
	String Dest = TrimRight(Src);

	Dest = TrimLeft(Dest);

	return Dest;
}

// Replaces ZEROA (CR) to <BR> when creating HTML output
string CTextTrx::CRToBR(const char* pText)
{
	return Convert(pText, HTML_BREAK_ASCII);
}

string CTextTrx::Convert(const char* pSrc, const char* pSubst)
{
	string	Dest;
	long	ZeroACount = 0;
	long	Size = strlen(pSrc);

	// Run through the source and count the ZEROA characters
	for (long I = 0; I < Size; I++)
	{
		if (pSrc[I] == ZEROA)
		{
			ZeroACount++;
		}
	}

	// Reserve enough space for the replacement to take place efficiently
	Dest.reserve(Size + ZeroACount * strlen(pSubst));

	// Replace all ZEROAs with the substitution string
	for (I = 0; I < Size; I++)
	{
		if (pSrc[I] == ZEROD)
		{
			continue;
		}
		else if (pSrc[I] == ZEROA)
		{
			Dest += pSubst;
		}
		else
		{
			Dest += pSrc[I];
		}
	}

	return Dest;
}

// Replaces ZEROA (CR) to <BR> when creating HTML output
String CTextTrx::CRToBR(const String& Src)
{
	return Convert(Src, HTML_BREAK_LOWERCASE);
}

// Replaces ZEROA (CR) with CR+LF when displaying text
String CTextTrx::CRToCRLF(const String& Src)
{
	return Convert(Src, CRLF);
}

String CTextTrx::Convert(const String& Src, const String& Subst)
{
	String	Dest;
	long	ZeroACount = 0;

	// Run through the source and count the ZEROA characters
	for (long I = 0; I < Src.size(); I++)
	{
		if (Src[I] == ZEROA)
		{
			ZeroACount++;
		}
	}

	// Reserve enough space for the replacement to take place efficiently
	Dest.reserve(Src.size() + ZeroACount * Subst.size());

	// Replace all ZEROAs with the substitution string
	for (I = 0; I < Src.size(); I++)
	{
		if (Src[I] == ZEROD)
		{
			continue;
		}
		else if (Src[I] == ZEROA)
		{
			Dest += Subst;
		}
		else
		{
			Dest += Src[I];
		}
	}

	return Dest;
}

// Replaces all <BR> with CR+LF
void CTextTrx::BRToCRLF(String& Src)
{
	long i;

	String	Find = HTML_BREAK_LOWERCASE;

	// Look for the lower case version of the HTML breaks
	while ((i = Src.find(HTML_BREAK_LOWERCASE)) != std::string::npos) 
	{
		Src.replace(i, Find.length(), CRLF);
	}

	Find = HTML_BREAK;

	// Look for the uupercase case version of the HTML breaks
	while ((i = Src.find(HTML_BREAK)) != std::string::npos) 
	{
		Src.replace(i, Find.length(), CRLF);
	}
}

void CTextTrx::Rot13(String& Str)
{
	long Size = Str.size();

	static int Rot13Tbl[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,78,79,80,81,82,83,84,85,86,87,88,89,90,65,66,67,68,69,70,71,72,73,74,75,76,77,91,92,93,94,95,96,110,111,112,113,114,115,116,117,118,119,120,121,122,97,98,99,100,101,102,103,104,105,106,107,108,109,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};

	for (long I = 0; I < Size; I++)
	{
		Str[I] = Rot13Tbl[Str[I]];
	}
}
