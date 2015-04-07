#include "CGpxParser.h"
#include "CCachePageWriter.h"
#include "CTimeHelper.h"
#include "CCoords.h"
#include "CTextTrx.h"
#include "CFieldNoteMgr.h"
#include "CCacheReportsPref.h"
#include <math.h>
#include "windows.h"
#include "CPath.h"
#include "Literals.h"

//-------------------------------------------------------------------------------------------------------------------------------------
CHtmlSeg::CHtmlSeg(char* pHtmlSeg, CCachePageWriterHandler* pH)
{
	m_HtmlSeg = pHtmlSeg;
	m_pH = pH;
}

// Format integers according to the segment definition
string	CHtmlSeg::FormatStats(int NotesFound, int NotesDNF, int NotesNote, int NotesArchive)
{
	#define	EXTRA_SPACE_FOR_SUBSTITUTIONS	200

	string		Out;
	CTextTrx	TT;
	char		Buffer[10];

	Out.reserve(m_HtmlSeg.size() + EXTRA_SPACE_FOR_SUBSTITUTIONS);

	// Run though the HTML segment looking for tags to replace
	for (long I = 0; I < m_HtmlSeg.size();)
	{
		// Is this a '#'?
		if (m_HtmlSeg[I] == '#')
		{
			char*	pTag = "###NOTES_FOUND###";
			long	Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				sprintf(Buffer,"%i",NotesFound);
				Out += Buffer;
				continue;
			}

			pTag = "###NOTES_DNF###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				sprintf(Buffer,"%i",NotesDNF);
				Out += Buffer;
				continue;
			}

			pTag = "###NOTES_NOTE###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				sprintf(Buffer,"%i",NotesNote);
				Out += Buffer;
				continue;
			}

			pTag = "###NOTES_ARCHIVE###";
			Size = strlen(pTag);

			// Found a #, attempt to match it with the ones that we recognize
			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				sprintf(Buffer,"%i",NotesArchive);
				Out += Buffer;
				continue;
			}
		}

		Out += m_HtmlSeg[I];
		I++;
	}

	return Out;
}

// Format text according to the segment definition
string CHtmlSeg::Format(CGpxParser& Parser, CGeoCache& Cache)
{
	AW_CONVERSION;

	#define	EXTRA_SPACE_FOR_SUBSTITUTIONS	200

	string Out;

	CTextTrx TT;

	Out.reserve(m_HtmlSeg.size() + EXTRA_SPACE_FOR_SUBSTITUTIONS);

	// Run though the HTML segment looking for tags to replace
	for (long I = 0; I < m_HtmlSeg.size();)
	{
		// Is this a '#'?
		if (m_HtmlSeg[I] == '#')
		{
			char*	pTag = "###WAYPOINT###";
			long	Size = strlen(pTag);

			// Found a #, attempt to match it with the ones that we recognize
			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				char* pShortname = w2a((TCHAR*)Cache.m_Shortname.c_str());

				Out += "<a href=\"http://www.geocaching.com/seek/cache_details.aspx?wp=";
				Out += pShortname;
				Out += "\">";
				Out += pShortname;
				Out += "</a>";
				continue;
			}
			
			pTag = "###CACHE_TYPE###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				wstring CacheType = SpaceToUnderscore(Cache.m_GsCacheType);

				Out += w2a((TCHAR*)CacheType.c_str());
				continue;
			}

			pTag = "###CACHE_NAME###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;
				Out += w2a((TCHAR*)Cache.m_GsCacheName.c_str());
				continue;
			}
			
			pTag = "###OWNER###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;
				Out += w2a((TCHAR*)Cache.m_GsCacheOwnerName.c_str());
				continue;
			}
			
			pTag = "###COORDS###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				CCoords	Coords;
				Coords.SetDecimal(Cache.m_Lat, Cache.m_Long);
				Out += w2a((TCHAR*)Coords.Format().c_str());
				continue;
			}
			
			pTag = "###CONTAINER###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				Out += w2a((TCHAR*)Cache.m_GsCacheContainer.c_str());
				continue;
			}

			pTag = "###STATE###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;
				Out += w2a((TCHAR*)Cache.m_GsCacheState.c_str());
				continue;
			}
			
			pTag = "###COUNTRY###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;
				Out += w2a((TCHAR*)Cache.m_GsCacheCountry.c_str());
				continue;
			}
			
			pTag = "###HIDDEN###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				CTimeHelper	TH(Cache.m_CreationTime);
				Out += w2a((TCHAR*)TH.Date().c_str());
				continue;
			}
			
			pTag = "###DIFFICULTY###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;
				Out += WriteStars(Cache.m_GsCacheDifficulty, GcRatingDifficulty);
				continue;
			}
			
			pTag = "###TERRAIN###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				Out += WriteStars(Cache.m_GsCacheTerrain, GcRatingTerrain);
				continue;
			}
			
			pTag = "###TB_NAME###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;
				
				Out += w2a((TCHAR*)Cache.m_pCurrTB->m_Name.c_str());
				continue;
			}
			
			pTag = "###SHORT_DESC###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				// Is the string stored in memory or on disk?
				if (Cache.m_GsCacheShortDesc.substr(0,MEM_MISER_CANARY_SIZE) == MEM_MISER_CANARY)
				{
					// Read the data back from disk
					char* pText = Parser.ReadFromTextStore(Cache.m_GsCacheShortDesc);

					if (!Cache.m_GsCacheShortDescIsHtml)
					{
						Out += TT.CRToBR(pText);
					}
					else
					{
						if (Parser.GetStripImgTags())
						{
							StripImgTag(pText);
						}

						Out += pText;
					}

					delete pText;
				}
				else
				{
					if (!Cache.m_GsCacheShortDescIsHtml)
					{
						char* pText = w2a((TCHAR*) TT.CRToBR(Cache.m_GsCacheShortDesc).c_str());

						Out += pText;
					}
					else
					{
						char* pText = w2a((TCHAR*)Cache.m_GsCacheShortDesc.c_str());

						if (Parser.GetStripImgTags())
						{
							StripImgTag(pText);
						}

						Out += pText;
					}
				}
				continue;
			}
			
			pTag = "###LONG_DESC###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				// Is the string stored in memory or on disk?
				if (Cache.m_GsCacheLongDesc.substr(0,MEM_MISER_CANARY_SIZE) == MEM_MISER_CANARY)
				{
					// Read the data back from disk
					char* pText = Parser.ReadFromTextStore(Cache.m_GsCacheLongDesc);

					if (!Cache.m_GsCacheLongDescIsHtml)
					{
						Out += TT.CRToBR(pText);
					}
					else
					{
						if (Parser.GetStripImgTags())
						{
							StripImgTag(pText);
						}

						Out += pText;
					}

					delete pText;
				}
				else
				{
					if (!Cache.m_GsCacheLongDescIsHtml)
					{
						char* pText = w2a((TCHAR*)TT.CRToBR(Cache.m_GsCacheLongDesc).c_str());

						Out += pText;
					}
					else
					{
						char* pText = w2a((TCHAR*)Cache.m_GsCacheLongDesc.c_str()); 

						if (Parser.GetStripImgTags())
						{
							StripImgTag(pText);
						}

						Out += pText;
					}
				}
				continue;
			}
			
			pTag = "###HINTS###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;
				Out += w2a((TCHAR*) Cache.m_GsCacheEncodedHints.c_str());
				continue;
			}

			pTag = "###SPOILER_PICS###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				char* pShortname = w2a((TCHAR*)Cache.m_Shortname.c_str());
				Out += pShortname;
				continue;
			}
			
			pTag = "###LOG_ICON###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				Out += LogIconLookup(Cache.m_pCurrCLE->TypeLookup());

				continue;
			}
			
			pTag = "###LOG_DATE###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				CTimeHelper	TH(Cache.m_pCurrCLE->m_Date);
				
				Out += w2a((TCHAR*)TH.Date().c_str());
				continue;
			}
			
			pTag = "###FINDER###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				Out += w2a((TCHAR*)Cache.m_pCurrCLE->m_FinderName.c_str());
				continue;
			}
			
			pTag = "###LOG_COORDS###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				if ((Cache.m_pCurrCLE->m_Lat != 0.0) && (Cache.m_pCurrCLE->m_Long != 0.0))
				{
					CCoords	Coords;
					Coords.SetDecimal(Cache.m_pCurrCLE->m_Lat, Cache.m_pCurrCLE->m_Long);
					Out += w2a((TCHAR*)Coords.Format().c_str());
				}
				continue;
			}

			pTag = "###LOG_ENTRY###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				// Is the string stored in memory or on disk?
				if (Cache.m_pCurrCLE->m_Text.substr(0,MEM_MISER_CANARY_SIZE) == MEM_MISER_CANARY)
				{
					// Read the data back from disk
					char* pText = Parser.ReadFromTextStore(Cache.m_pCurrCLE->m_Text);

					string Tmp = ScanForMarkup(pText, strlen(pText));

					Out += TT.CRToBR(Tmp.c_str());

					delete pText;
				}
				else
				{
					TCHAR* pText = (TCHAR*)Cache.m_pCurrCLE->m_Text.c_str();

					char* pAsc = w2a(pText);

					string Tmp = ScanForMarkup(pAsc, _nStrLength);

					Out += TT.CRToBR(Tmp.c_str());
				}
				continue;
			}

			//--------------------------------------------------------------------------------------
			// This section handles the tags related to the production of field notes reports
			//--------------------------------------------------------------------------------------

			pTag = "###REPORT_ICON###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				Out += FieldNoteStatusLookup(Cache.m_pFieldNote->m_Status);

				continue;
			}

			pTag = "###REPORT_DATE###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				CTimeHelper	TH(Cache.m_pFieldNote->m_Date);
				
				Out += w2a((TCHAR*)TH.DateTime().c_str());

				continue;
			}

			pTag = "###REPORT_COORDS###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				if ((Cache.m_pFieldNote->m_Lat != 0.0) && (Cache.m_pFieldNote->m_Long != 0.0))
				{
					CCoords	Coords;
					Coords.SetDecimal(Cache.m_pFieldNote->m_Lat, Cache.m_pFieldNote->m_Long);
					Out += w2a((TCHAR*)Coords.Format().c_str());
				}
				continue;
			}

			pTag = "###REPORT_ENTRY###";
			Size = strlen(pTag);

			if (!m_HtmlSeg.compare(I, Size, pTag))
			{
				I += Size;

				if (Cache.m_pFieldNote->m_Notes.size())
				{
					Out += TT.CRToBR(w2a((TCHAR*)Cache.m_pFieldNote->m_Notes.c_str()));
				}

				CPath Path;

				String	VoiceNote = GPXSONAR_FIELD_NOTES_DIR;
						VoiceNote += Cache.m_Shortname;
						VoiceNote += _T(".wav");

				VoiceNote = Path.BuildPath(VoiceNote);

				// Check if there's a voice note with it...
				if (GetFileAttributes((LPCTSTR) VoiceNote.c_str()) != -1)
				{
					// Create a link to the recording and add it to the log entry
					Out += " <a href=\"";
					Out += w2a((TCHAR*)VoiceNote.c_str());
					Out += "\"><IMG SRC='./images/icons/voice_record.gif'></a>";
				}

				continue;
			}
		}

		Out += m_HtmlSeg[I];
		I++;
	}

	return Out;
}

void CHtmlSeg::StripImgTag(char* pText)
{
	char* pTmp = pText;

	if (!pTmp)
	{
		return;
	}

	while (true)
	{
		static char* Tags[] = {
			"<img",
			"<IMG",
			"<Img",
			""
		};

		char* pTag = 0;

		// Try to find an IMG tag in the text...
		for (int T = 0; *Tags[T]; T++)
		{
			pTag = strstr(pTmp,Tags[T]);

			if (pTag)
			{
				break;
			}
		}

		if (!pTag)
		{
			return;
		}


		// Found one! Replace the content of '<IMG' with beginning HTML comment '<!--'
		pTag[1] = '!';
		pTag[2] = '-';
		pTag[3] = '-';

		// Locate the end '>' and replace with a terminating HTML comment: '-->'
		for (int I = 4; pTag[I]; I++)
		{
			if (pTag[I] == '>')
			{
				pTag[I-1] = '-';
				pTag[I-2] = '-';

				pTmp = pTag;
				break;
			}
		}
	}
}

// Replace spaces with '_' in cache type bitmaps
wstring	CHtmlSeg::SpaceToUnderscore(wstring& Str)
{
	wstring Subst = Str;

	long Size = Str.size();

	for (long C = 0; C < Size; C++)
	{
		if (Subst[C] == _T(' '))
		{
			Subst.at(C) = _T('_');
		}
	}

	return Subst;
}

// Returns the filename of the icon matching the log type
string CHtmlSeg::LogIconLookup(int LogType)
{
	switch(LogType)
	{
	case LOG_FoundIt:
		return "icon_smile.gif";
		break;
	case LOG_DidntFindIt:
		return "icon_sad.gif";
		break;
	case LOG_NeedsArchived:
		return "icon_remove.gif";
		break;
	case LOG_ArchivedNoShow:
	case LOG_ArchivedShow:
		return "traffic_cone.gif";
		break;
	case LOG_Camera:
		return "icon_camera.gif";
		break;
	case LOG_EnableListing:
		return "icon_enabled.gif";
		break;
	case LOG_TemporarilyDisableListing:
		return "icon_disabled.gif";
		break;
	case LOG_Other:
		return "icon_note.gif";
		break;
	}

	return "icon_note.gif";
}

// Searches for markup markers and returns a complete string with substitutions
string CHtmlSeg::ScanForMarkup(char* pText, int Length)
{
	#define MAX_VALID_TAG_LENGTH	6

	typedef vector<string> strList;
	typedef vector<string>::iterator itstr;

	strList	Strs;
	string	Temp;

	Temp.reserve(6);

	char* pI = pText;
	char* pCopyStart = pText;

	int	BeginPos = -1;
	int	EndPos = -1;
	int CopyIdx = 0;

	long TotalSize = 0;

	// Run through the text looking for [ and ] markers
	for (int I = 0; I < Length; I++)
	{
		if (BeginPos == -1)
		{
			// Is this an opening marker?
			if (pText[I] == '[')
			{
				// Store the string preceding the marker
				Temp.assign(pText + CopyIdx, (int) ((pText + I) - (pText + CopyIdx)));

				Strs.push_back(Temp);

				TotalSize += Temp.size();

				// Set the start of the next copy operation
				CopyIdx = I;

				// Store the position of the beginning marker
				BeginPos = I;
			}
		}
		else if (EndPos == -1)
		{
			// Is this an ending marker?
			if (pText[I] == ']')
			{
				bool Stored = false;

				// Store the position of the end marker
				EndPos = I;

				// Compute the size of the tag between [ & ]
				int TagSize = (int) ((pText + EndPos + 1) - (pText + BeginPos));

				// Are we dealing with a potentially interesting tag?
				if (TagSize < MAX_VALID_TAG_LENGTH)
				{
					// Yes, look it up!

					// Make the string upper case
					char Buffer[MAX_VALID_TAG_LENGTH];

					memset(Buffer, 0, MAX_VALID_TAG_LENGTH);

					// Copy the tag into a temporary buffer
					strncpy(Buffer, pText + BeginPos, TagSize);

					// Convert the buffer to uppercase
					for (int U = 0; U < TagSize; U++)
					{
#ifdef PPC2K2
						Buffer[U] = toupper(Buffer[U]);
#else
						Buffer[U] = _toupper(Buffer[U]);
#endif
					}

					// Look it up
					string Out = MarkupLookup(Buffer);

					// Did the lookup operation return a match?
					if (!Out.empty())
					{
						if (Out.size() > MAX_VALID_TAG_LENGTH)
						{
							Temp = "<img src=\"./Images/Icons/";
							Temp += Out;
							Temp += "\">";
						}
						else
						{
							Temp = Out;
						}

						TotalSize += Temp.size();

						// Yes, store the replacement string for the tag
						Strs.push_back(Temp);

						// Set the start of the next copy operation
						CopyIdx = EndPos + 1;

						Stored = true;
					}
				}
				
				if (!Stored)
				{
					// Store the string in between the markers
					Temp.assign(pText + BeginPos, TagSize);

					Strs.push_back(Temp);

					TotalSize += Temp.size();

					// Set the start of the next copy operation
					CopyIdx = EndPos + 1;
				}

				// Nope, this is not a tag that needs substitution
				// Reset the markers
				BeginPos = -1;
				EndPos = -1;
			}
		}
	}

	// At the end of the loop, we can deal with the following case
	// #1: There were no tags in the text at all
	if (!Strs.size())
	{
		return pText;
	}

	// #2: There was a beginning tag ([) but no end tag (])
	if ((BeginPos != -1 && EndPos == -1) || CopyIdx < Length)
	{
		// Copy everything left after the [ marker
		Temp.assign(pText + CopyIdx);

		TotalSize += Temp.size();

		Strs.push_back(Temp);
	}
	
	Temp.erase();

	Temp.reserve(TotalSize);

	// Finally, make one big string with all the substitutions
	for (itstr S = Strs.begin(); S != Strs.end(); S++)
	{
		Temp += *S;
	}

	return Temp;
}

// Performs lookups on commonly used tags in logs found on GC.com
string CHtmlSeg::MarkupLookup(char* pMarkup)
{
	if (!_stricmp(pMarkup,"[:D]"))
	{
		return "icon_smile_big.gif";
	}
	else if (!_stricmp(pMarkup,"[:O]") || !strcmp(pMarkup,"[:0]"))
	{
		return "icon_smile_shock.gif";
	}
	else if (!_stricmp(pMarkup,"[:I]"))
	{
		return "icon_smile_blush.gif";
	}
	else if (!_stricmp(pMarkup,"[:P]"))
	{
		return "icon_smile_tongue.gif";
	}
	else if (!_stricmp(pMarkup,"[:)]"))
	{
		return "icon_smile.gif";
	}
	else if (!_stricmp(pMarkup,"[:)]"))
	{
		return "icon_smile.gif";
	}
	else if (!_stricmp(pMarkup,"[;)]"))
	{
		return "icon_smile_wink.gif";
	}
	else if (!_stricmp(pMarkup,"[^]"))
	{
		return "icon_smile_approve.gif";
	}
	else if (!_stricmp(pMarkup,"[:(]"))
	{
		return "icon_smile_sad.gif";
	}
	else if (!_stricmp(pMarkup,"[}:)]"))
	{
		return "icon_smile_evil.gif";
	}
	else if (!_stricmp(pMarkup,"[V]"))
	{
		return "icon_smile_dissapprove.gif";
	}
	else if (!_stricmp(pMarkup,"[8D]"))
	{
		return "icon_smile_cool.gif";
	}
	else if (!_stricmp(pMarkup,"[8]"))
	{
		return "icon_smile_8ball.gif";
	}
	else if (!_stricmp(pMarkup,"[B)]"))
	{
		return "icon_smile_blackeye.gif";
	}
	else if (!_stricmp(pMarkup,"[:(!]"))
	{
		return "icon_smile_angry.gif";
	}
	else if (!_stricmp(pMarkup,"[:X]"))
	{
		return "icon_smile_kisses.gif";
	}
	else if (!_stricmp(pMarkup,"[XX(]"))
	{
		return "icon_smile_dead.gif";
	}
	else if (!_stricmp(pMarkup,"[?]"))
	{
		return "icon_smile_question.gif";
	}
	else if (!_stricmp(pMarkup,"[:o)]"))
	{
		return "icon_smile_clown.gif";
	}
	else if (!_stricmp(pMarkup,"[8)]"))
	{
		return "icon_smile_shy.gif";
	}
	else if (!_stricmp(pMarkup,"[|)]"))
	{
		return "icon_smile_sleepy.gif";
	}

	//------------------------------------------------------------------

	else if (!_stricmp(pMarkup,"[B]"))
	{
		return "<B>";
	}
	else if (!_stricmp(pMarkup,"[/B]"))
	{
		return "</B>";
	}
	else if (!_stricmp(pMarkup,"[I]"))
	{
		return "<I>";
	}
	else if (!_stricmp(pMarkup,"[/I]"))
	{
		return "</I>";
	}
	else if (!_stricmp(pMarkup,"[U]"))
	{
		return "<U>";
	}
	else if (!_stricmp(pMarkup,"[/U]"))
	{
		return "</U>";
	}

	//------------------------------------------------------------------

	return pMarkup;
}

// Returns the filename of the icon matching the status of the field note
string CHtmlSeg::FieldNoteStatusLookup(int Status)
{
	switch(Status)
	{
	case NoteStatusFoundIt:
		return LogIconLookup(LOG_FoundIt);
		break;
	case NoteStatusDNF:
		return LogIconLookup(LOG_DidntFindIt);
		break;
	case NoteStatusNote:
		return LogIconLookup(LOG_Other);
		break;
	case NoteStatusArchiveIt:
		return LogIconLookup(LOG_NeedsArchived);
		break;
	}

	return LogIconLookup(LOG_Other);
}

string CHtmlSeg::WriteStars(double Rating, GcRatingType Type)
{
	string Out;

	Out.reserve(EXTRA_SPACE_FOR_SUBSTITUTIONS);

	while (Rating >= 1)
	{
		Rating--;

		Out +="<img src=\"./Images/";

		switch (Type)
		{
		case GcRatingDifficulty:
			Out += "staryellow.gif";
			break;
		case GcRatingTerrain:
			Out += "stargreen.gif";
			break;
		}

		Out += "\">";
	}

	if (Rating > 0.0)
	{
		Out += "<img src=\"./Images/";

		switch (Type)
		{
		case GcRatingDifficulty:
			Out += "halfstaryellow.gif";
			break;
		case GcRatingTerrain:
			Out += "halfstargreen.gif";
			break;
		}

		Out += "\">";
	}

	return Out;
}

//-------------------------------------------------------------------------------------------------------------------------------------

CCachePageWriterHandler::CCachePageWriterHandler()
{}

// Virtual function called for each tag in the template associated with a writer handler
string CCachePageWriterHandler::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	return "";
}

//-------------------------------------------------------------------------------------------------------------------------------------
// Only return a string if travel bugs actually exist in the cache
string CTBLabelWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;
	
	if (Cache.GetTBCount())
	{
		Out = pHtmlSeg->Format(Parser, Cache);
	}

	return Out;
}

// Run through the travel bugs in the cache and generate the HTML for each one
string CTBWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;
	
	itTB it;
	
	Cache.FirstTB(it);

	while (!Cache.EndOfTBList(it))
	{
		Out += pHtmlSeg->Format(Parser, Cache);

		Cache.NextTB(it);
	}

	return Out;
}

// Only return a string if the cache is disabled or archived
string CWarningWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;
	
	if (Cache.m_GsCacheArchived || !Cache.m_GsCacheAvailable)
	{
		Out = pHtmlSeg->Format(Parser, Cache);
	}

	return Out;
}

// Run through the log entries for the cache and generate the HTML for each one
string CLogWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;

	itGCLogEntry it;

	Cache.FirstLog(it);

	while (!Cache.EndOfLogList(it))
	{
		Out += pHtmlSeg->Format(Parser, Cache);

		Cache.NextLog(it);
	}

	return Out;
}

string CHintsLabelWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;
	
	CTextTrx TT;

	Cache.m_GsCacheEncodedHints = TT.Trim(Cache.m_GsCacheEncodedHints);

	if (!Cache.m_GsCacheEncodedHints.empty())
	{
		Out = pHtmlSeg->Format(Parser, Cache);
	}

	return Out;
}

string CHintsWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;
	
	CTextTrx TT;

	Cache.m_GsCacheEncodedHints = TT.Trim(Cache.m_GsCacheEncodedHints);

	if (!Cache.m_GsCacheEncodedHints.empty())
	{
		Out = pHtmlSeg->Format(Parser, Cache);
	}

	return Out;

}

void CSpoilerPicsWriter::SetSpoilerPicsPath(const wstring& SpoilerPicsPath)
{
	m_SpoilerPicsPath = SpoilerPicsPath;
}

string CSpoilerPicsWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;
	
	// Test for the existence of spoiler pics for this cache
	if (!m_SpoilerPicsPath.empty())
	{
		// Save the shortname of the cache
		wstring ShortNameBkp = Cache.m_Shortname;

		// Replace the shortname with the path to the spoiler pics for it
		Cache.m_Shortname = m_SpoilerPicsPath;

		// It has spoiler pics! Add a link to the html page created by Spoiler Sync for it
		Out = pHtmlSeg->Format(Parser, Cache);

		// Restore the actual short name of the cache
		Cache.m_Shortname = ShortNameBkp;
	}

	return Out;
}

string CReportWriter::OnWrite(CGpxParser& Parser, CGeoCache& Cache, CHtmlSeg* pHtmlSeg)
{
	string Out;
	
	if (Cache.m_pFieldNote)
	{
		Out = pHtmlSeg->Format(Parser, Cache);
	}

	return Out;

}

//-------------------------------------------------------------------------------------------------------------------------------------
CCachePageWriter::CCachePageWriter()
{
}

CCachePageWriter::~CCachePageWriter()
{
	Reset();
}

void CCachePageWriter::LoadTemplate(const TCHAR* pTplPath)
{
	#define MAX_TPL_SIZE 5120

	char	Buffer[MAX_TPL_SIZE];
	char*	pBuff = Buffer;

	FILE* fd = _tfopen(pTplPath, _T("r"));

	if (fd)
	{
		// Read the whole html template into the buffer
		long Size = fread(Buffer, sizeof(char), (MAX_TPL_SIZE * sizeof(char)) - 1, fd);

		Buffer[Size] = 0;

		// Mark the beginning of the current segment
		char* pSeg = 0;

		// Run through the buffer looking for <!--BEGINSEG--> / <!--ENDSEG--> tags
		for (long I = 0; I < Size; I++, pBuff++)
		{
			// Is this the beginning of a segment?
			if (!strncmp(pBuff,"<!--BEGINSEG-->", 15))
			{
				I += 15;
				pBuff += 15;

				// Yes, skip the tag and record the beginning of the segment
				pSeg = pBuff;
			}
			// Is this the end of a segment?
			else if (!strncmp(pBuff, "<!--ENDSEG-->", 13))
			{
				// Terminate the string
				*pBuff = 0;

				// Determine if the segment needs to be processed through a handler
				if (strstr(pSeg, "###TB_LABEL###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_TBLabelWriter);
				}
				else if (strstr(pSeg, "###UNAVAILABLE_LABEL###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_WarningWriter);
				}
				else if (strstr(pSeg, "###TB_NAME###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_TBWriter);
				}
				else if (strstr(pSeg, "###SPOILER_PICS###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_SpoilerPicsWriter);
				}
				else if (strstr(pSeg, "###HINTS###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_HintsWriter);
				}
				else if (strstr(pSeg, "###HINTS_LABEL###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_HintsLabelWriter);
				}
				else if (strstr(pSeg, "###LOG_ENTRY###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_LogWriter);
				}
				else if (strstr(pSeg, "###REPORT###"))
				{
					// Connect the segment with its processing code.
					AddSeg(pSeg, &m_ReportWriter);
				}
				else
				{
					// Regular segment w/o processing code
					AddSeg(pSeg, 0);
				}

				// Skip the end tag and record the beginning of the segment
				I += 13;
				pBuff += 13;

				pSeg = pBuff;
			}
		}

		fclose(fd);
	}
}

// Register the various parts of the HTML page to be generated
void CCachePageWriter::AddSeg(char* pHtmlSeg, CCachePageWriterHandler* pH)
{
	CHtmlSeg* pHS = new CHtmlSeg(pHtmlSeg, pH);

	if (!pHS)
	{
		//assert(0);
	}
	else
	{
		m_Segs.push_back(pHS);
	}
}

// Write the cache page out
void CCachePageWriter::Write(CGpxParser& Parser, CGeoCache& Cache, TCHAR* pFilePath)
{
	FILE* fd = _tfopen(pFilePath, _T("w"));

	if (fd)
	{
		for (itHtmlSeg I = m_Segs.begin(); I < m_Segs.end(); I++)
		{
			CHtmlSeg*	pHS = *I;

			string	Out;

			if (pHS->m_pH)
			{
				Out = pHS->m_pH->OnWrite(Parser, Cache, pHS);
			}
			else
			{
				// Simple formatting of the output
				Out = pHS->Format(Parser, Cache);
			}

			// Write out to the file
			fwrite(Out.c_str(), sizeof(char), Out.size(), fd);
		}

		fclose(fd);
	}
}

// Write a report page out
void CCachePageWriter::WriteReport(CCacheReportsPref* pPref, CFieldNoteMgr* pFNM, CGpxParser& Parser, TCHAR* pFilePath)
{
	FILE* fd = _tfopen(pFilePath, _T("w"));

	if (fd)
	{
		string		Out;
		CHtmlSeg*	pHS;

		// Get the report header
		itHtmlSeg I = m_Segs.begin();

		pHS = *I;

		// Write the report header
		fwrite(pHS->m_HtmlSeg.c_str(), sizeof(char), pHS->m_HtmlSeg.size(), fd);

		vector<CFieldNote*> Notes;
		vector<CFieldNote*>::iterator Nit;

		// Return a sorted list of notes
		Notes = pFNM->SortByTimestamp();

		int NotesFound = 0;
		int NotesDNF = 0;
		int NotesNote = 0;
		int NotesArchive = 0;

		// Accumulate the report stats
		for (Nit = Notes.begin(); Nit != Notes.end(); Nit++)
		{
			CFieldNote* pFN = (*Nit);

			if (!pFN->m_pCache)
			{
				continue;
			}
			
			switch(pFN->m_Status)
			{
			case NoteStatusFoundIt:
				NotesFound++;
				break;
			case NoteStatusDNF:
				NotesDNF++;
				break;
			case NoteStatusNote:
				NotesNote++;
				break;
			case NoteStatusArchiveIt:
				NotesArchive++;
				break;
			default:
				break;
			}
		}

		// Get the stats segment
		I++;

		pHS = *I;		
		
		// Write the report stats
		Out = pHS->FormatStats(NotesFound, NotesDNF, NotesNote, NotesArchive);
		fwrite(Out.c_str(), sizeof(char), Out.size(), fd);

		// Get the body of the report
		I++;

		pHS = *I;

		// Run through the notes and write the body of the report
		for (Nit = Notes.begin(); Nit != Notes.end(); Nit++)
		{
			CFieldNote* pFN = (*Nit);

			if (!pFN->m_pCache)
			{
				continue;
			}

			// Check if field note needs to be included in the report
			if (pPref->IncludeInReport(pFN->m_Status))
			{

				if (pHS->m_pH)
				{
					Out = pHS->m_pH->OnWrite(Parser, *pFN->m_pCache, pHS);
				}
				else
				{
					// Simple formatting of the output
					Out = pHS->Format(Parser, *pFN->m_pCache);
				}

				// Write out to the file
				fwrite(Out.c_str(), sizeof(char), Out.size(), fd);

				// Check if the field note needs to be removed
				if (pPref->m_Reset)
				{
					pFN->DeleteYourself(pFN->m_pCache->m_Shortname);
					pFN->m_pCache->m_pFieldNote = 0;
					pFNM->Delete(pFN);
				}
			}
		}

		// Get the footer of the report
		I++;

		pHS = *I;

		// Write the report footer
		fwrite(pHS->m_HtmlSeg.c_str(), sizeof(char), pHS->m_HtmlSeg.size(), fd);

		fclose(fd);
	}
}

// General cleanup
void CCachePageWriter::Reset()
{
	for (itHtmlSeg I = m_Segs.begin(); I < m_Segs.end(); I++)
	{
		delete *I;
	}

	m_Segs.clear();
}
