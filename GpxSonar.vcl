<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: Zlib - Win32 (WCE ARMV4) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE1D.tmp" with contents
[
/nologo /W3 /D "NDEBUG" /D "ARM" /D "_ARM_" /D "ARMV4" /D "NO_ERRNO_H" /D "_WIN32_WCE" /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"ARMV4Rel/Zlib.pch" /YX /Fo"ARMV4Rel/" /O2 /MC /c 
"D:\Projects\PPC2003\GpxSonar\Zlib\adler32.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\compress.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\crc32.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\deflate.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\gzio.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\infback.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\inffast.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\inflate.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\inftrees.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\ioapi.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\iowin32.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\trees.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\uncompr.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\unzip.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\zip.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\zutil.c"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE1D.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE1E.tmp" with contents
[
/nologo /out:"ARMV4Rel\Zlib.lib" 
.\ARMV4Rel\adler32.obj
.\ARMV4Rel\compress.obj
.\ARMV4Rel\crc32.obj
.\ARMV4Rel\deflate.obj
.\ARMV4Rel\gzio.obj
.\ARMV4Rel\infback.obj
.\ARMV4Rel\inffast.obj
.\ARMV4Rel\inflate.obj
.\ARMV4Rel\inftrees.obj
.\ARMV4Rel\ioapi.obj
.\ARMV4Rel\iowin32.obj
.\ARMV4Rel\trees.obj
.\ARMV4Rel\uncompr.obj
.\ARMV4Rel\unzip.obj
.\ARMV4Rel\zip.obj
.\ARMV4Rel\zutil.obj
]
Creating command line "link.exe -lib @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE1E.tmp"
<h3>Output Window</h3>
Compiling...
adler32.c
compress.c
crc32.c
deflate.c
gzio.c
d:\projects\ppc2003\gpxsonar\zlib\gzio.c(986) : warning C4013: 'GetLastError' undefined; assuming extern returning int
infback.c
inffast.c
inflate.c
inftrees.c
ioapi.c
iowin32.c
trees.c
uncompr.c
unzip.c
zip.c
zutil.c
Creating library...

<h3>
--------------------Configuration: GpxSonar - Win32 (WCE ARMV4) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4Rel/GpxSonar.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "NDEBUG" /d "UNICODE" /d "_UNICODE" /d "WIN32_PLATFORM_PSPC=400" /d "ARM" /d "_ARM_" /d "ARMV4" /r "D:\Projects\PPC2003\GpxSonar\GpxSonar.rc"" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE1F.tmp" with contents
[
/nologo /W3 /GX /D "NO_ERRNO_H" /D "_WIN32_WCE" /D "ARM" /D "_ARM_" /D "ARMV4" /D UNDER_CE=420 /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /D "COMPILED_FROM_DSP" /Fo"ARMV4Rel/" /O2 /MC /c 
"D:\Projects\PPC2003\GpxSonar\CAboutDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CBaseException.cpp"
"D:\Projects\PPC2003\GpxSonar\CBrowserLauncher.cpp"
"D:\Projects\PPC2003\GpxSonar\CCacheCategoryDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CCacheCategoryEditor.cpp"
"D:\Projects\PPC2003\GpxSonar\CCacheDetailsFieldNotes.cpp"
"D:\Projects\PPC2003\GpxSonar\CCacheDetailsHints.cpp"
"D:\Projects\PPC2003\GpxSonar\CCacheImportDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CCacheMgr.cpp"
"D:\Projects\PPC2003\GpxSonar\CCachePageWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CCachePropertiesDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CCacheReportsPref.cpp"
"D:\Projects\PPC2003\GpxSonar\CCenterCoordsDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CCoords.cpp"
"D:\Projects\PPC2003\GpxSonar\CCsvFileWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CDatePickerDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CDistanceUnitsDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CDynStr.cpp"
"D:\Projects\PPC2003\GpxSonar\CExportLocationDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CExportLocationMgr.cpp"
"D:\Projects\PPC2003\GpxSonar\CExportPreferences.cpp"
"D:\Projects\PPC2003\GpxSonar\CFieldNoteMgr.cpp"
"D:\Projects\PPC2003\GpxSonar\CFieldNotesReportPrefDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterBearingDistance.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterBearingDistanceDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterCacheContainers.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterCacheLists.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterCacheListsDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterCacheRatings.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterCacheTB.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterCacheTypes.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterCacheTypesDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterContainersDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterIgnoredCachesDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterMgr.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterMgrDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterOnStrings.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterRatingsDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterSearch.cpp"
"D:\Projects\PPC2003\GpxSonar\CFilterStringsDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CGPSWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CGpxFileInfoDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CGpxParser.cpp"
"D:\Projects\PPC2003\GpxSonar\CHeading.cpp"
"D:\Projects\PPC2003\GpxSonar\CLineEditDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CListPreferencesDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CLocFileWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CLowranceWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CMd5.cpp"
"D:\Projects\PPC2003\GpxSonar\CMenuEdit.cpp"
"D:\Projects\PPC2003\GpxSonar\CMeridianWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CMiscPreferences.cpp"
"D:\Projects\PPC2003\GpxSonar\CMxfFileWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CMyAliasDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CMyCachesDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CMyTravelBugsDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CNavigonWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CommonDefs.cpp"
"D:\Projects\PPC2003\GpxSonar\Coord.cpp"
"D:\Projects\PPC2003\GpxSonar\COziWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CPath.cpp"
"D:\Projects\PPC2003\GpxSonar\CPharosOstia6Writer.cpp"
"D:\Projects\PPC2003\GpxSonar\CPocketStreetPushPinsWriter.cpp"
"D:\Projects\PPC2003\GpxSonar\CProjectWptDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CRatingDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CRot13ConvertDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CSearchDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CStream.cpp"
"D:\Projects\PPC2003\GpxSonar\CTBActionMgr.cpp"
"D:\Projects\PPC2003\GpxSonar\CTBExchangeDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CTBInfoDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CTBMgr.cpp"
"D:\Projects\PPC2003\GpxSonar\CTextEditDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CTextTrx.cpp"
"D:\Projects\PPC2003\GpxSonar\CTimeHelper.cpp"
"D:\Projects\PPC2003\GpxSonar\CTypeAndContainerDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CWaypointsDlg.cpp"
"D:\Projects\PPC2003\GpxSonar\CWptProperties.cpp"
"D:\Projects\PPC2003\GpxSonar\CXmlMap.cpp"
"D:\Projects\PPC2003\GpxSonar\GpxSonar.cpp"
"D:\Projects\PPC2003\GpxSonar\GpxSonarDoc.cpp"
"D:\Projects\PPC2003\GpxSonar\GpxSonarView.cpp"
"D:\Projects\PPC2003\GpxSonar\MainFrm.cpp"
"D:\Projects\PPC2003\GpxSonar\md5.cpp"
"D:\Projects\PPC2003\GpxSonar\NonFSDialog.cpp"
"D:\Projects\PPC2003\GpxSonar\Expat\xmlparse.c"
"D:\Projects\PPC2003\GpxSonar\Expat\xmlrole.c"
"D:\Projects\PPC2003\GpxSonar\Expat\xmltok.c"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE1F.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE20.tmp" with contents
[
/nologo /W3 /GX /D "NO_ERRNO_H" /D "_WIN32_WCE" /D "ARM" /D "_ARM_" /D "ARMV4" /D UNDER_CE=420 /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /D "COMPILED_FROM_DSP" /Fp"ARMV4Rel/GpxSonar.pch" /Yc"stdafx.h" /Fo"ARMV4Rel/" /O2 /MC /c 
"D:\Projects\PPC2003\GpxSonar\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE20.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE21.tmp" with contents
[
voicectl.lib Zlib.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:no /pdb:"ARMV4Rel/GpxSonar.pdb" /out:"ARMV4Rel/GpxSonar.exe" /libpath:".\Zlib\ARMV4Rel" /subsystem:windowsce,4.20 /align:"4096" /MACHINE:ARM 
.\ARMV4Rel\CAboutDlg.obj
.\ARMV4Rel\CBaseException.obj
.\ARMV4Rel\CBrowserLauncher.obj
.\ARMV4Rel\CCacheCategoryDlg.obj
.\ARMV4Rel\CCacheCategoryEditor.obj
.\ARMV4Rel\CCacheDetailsFieldNotes.obj
.\ARMV4Rel\CCacheDetailsHints.obj
.\ARMV4Rel\CCacheImportDlg.obj
.\ARMV4Rel\CCacheMgr.obj
.\ARMV4Rel\CCachePageWriter.obj
.\ARMV4Rel\CCachePropertiesDlg.obj
.\ARMV4Rel\CCacheReportsPref.obj
.\ARMV4Rel\CCenterCoordsDlg.obj
.\ARMV4Rel\CCoords.obj
.\ARMV4Rel\CCsvFileWriter.obj
.\ARMV4Rel\CDatePickerDlg.obj
.\ARMV4Rel\CDistanceUnitsDlg.obj
.\ARMV4Rel\CDynStr.obj
.\ARMV4Rel\CExportLocationDlg.obj
.\ARMV4Rel\CExportLocationMgr.obj
.\ARMV4Rel\CExportPreferences.obj
.\ARMV4Rel\CFieldNoteMgr.obj
.\ARMV4Rel\CFieldNotesReportPrefDlg.obj
.\ARMV4Rel\CFilterBearingDistance.obj
.\ARMV4Rel\CFilterBearingDistanceDlg.obj
.\ARMV4Rel\CFilterCacheContainers.obj
.\ARMV4Rel\CFilterCacheLists.obj
.\ARMV4Rel\CFilterCacheListsDlg.obj
.\ARMV4Rel\CFilterCacheRatings.obj
.\ARMV4Rel\CFilterCacheTB.obj
.\ARMV4Rel\CFilterCacheTypes.obj
.\ARMV4Rel\CFilterCacheTypesDlg.obj
.\ARMV4Rel\CFilterContainersDlg.obj
.\ARMV4Rel\CFilterIgnoredCachesDlg.obj
.\ARMV4Rel\CFilterMgr.obj
.\ARMV4Rel\CFilterMgrDlg.obj
.\ARMV4Rel\CFilterOnStrings.obj
.\ARMV4Rel\CFilterRatingsDlg.obj
.\ARMV4Rel\CFilterSearch.obj
.\ARMV4Rel\CFilterStringsDlg.obj
.\ARMV4Rel\CGPSWriter.obj
.\ARMV4Rel\CGpxFileInfoDlg.obj
.\ARMV4Rel\CGpxParser.obj
.\ARMV4Rel\CHeading.obj
.\ARMV4Rel\CLineEditDlg.obj
.\ARMV4Rel\CListPreferencesDlg.obj
.\ARMV4Rel\CLocFileWriter.obj
.\ARMV4Rel\CLowranceWriter.obj
.\ARMV4Rel\CMd5.obj
.\ARMV4Rel\CMenuEdit.obj
.\ARMV4Rel\CMeridianWriter.obj
.\ARMV4Rel\CMiscPreferences.obj
.\ARMV4Rel\CMxfFileWriter.obj
.\ARMV4Rel\CMyAliasDlg.obj
.\ARMV4Rel\CMyCachesDlg.obj
.\ARMV4Rel\CMyTravelBugsDlg.obj
.\ARMV4Rel\CNavigonWriter.obj
.\ARMV4Rel\CommonDefs.obj
.\ARMV4Rel\Coord.obj
.\ARMV4Rel\COziWriter.obj
.\ARMV4Rel\CPath.obj
.\ARMV4Rel\CPharosOstia6Writer.obj
.\ARMV4Rel\CPocketStreetPushPinsWriter.obj
.\ARMV4Rel\CProjectWptDlg.obj
.\ARMV4Rel\CRatingDlg.obj
.\ARMV4Rel\CRot13ConvertDlg.obj
.\ARMV4Rel\CSearchDlg.obj
.\ARMV4Rel\CStream.obj
.\ARMV4Rel\CTBActionMgr.obj
.\ARMV4Rel\CTBExchangeDlg.obj
.\ARMV4Rel\CTBInfoDlg.obj
.\ARMV4Rel\CTBMgr.obj
.\ARMV4Rel\CTextEditDlg.obj
.\ARMV4Rel\CTextTrx.obj
.\ARMV4Rel\CTimeHelper.obj
.\ARMV4Rel\CTypeAndContainerDlg.obj
.\ARMV4Rel\CWaypointsDlg.obj
.\ARMV4Rel\CWptProperties.obj
.\ARMV4Rel\CXmlMap.obj
.\ARMV4Rel\GpxSonar.obj
.\ARMV4Rel\GpxSonarDoc.obj
.\ARMV4Rel\GpxSonarView.obj
.\ARMV4Rel\MainFrm.obj
.\ARMV4Rel\md5.obj
.\ARMV4Rel\NonFSDialog.obj
.\ARMV4Rel\StdAfx.obj
.\ARMV4Rel\xmlparse.obj
.\ARMV4Rel\xmlrole.obj
.\ARMV4Rel\xmltok.obj
.\ARMV4Rel\GpxSonar.res
.\Zlib\ARMV4Rel\Zlib.lib
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE21.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
CAboutDlg.cpp
CBaseException.cpp
CBrowserLauncher.cpp
CCacheCategoryDlg.cpp
CCacheCategoryEditor.cpp
CCacheDetailsFieldNotes.cpp
CCacheDetailsHints.cpp
CCacheImportDlg.cpp
CCacheMgr.cpp
c:\program files\windows ce tools\wce420\pocket pc 2003\include\armv4\xmemory(46) : warning C4291: 'void *__cdecl operator new(unsigned int,void *)' : no matching operator delete found; memory will not be freed if initialization throws an exception
        c:\program files\windows ce tools\wce420\pocket pc 2003\mfc\include\wcealt.h(235) : see declaration of 'new'
        c:\program files\windows ce tools\wce420\pocket pc 2003\include\armv4\xmemory(78) : see reference to function template instantiation 'void __cdecl std::_Construct(class std::basic_string<unsigned short,struct std::char_traits<unsigned short>,class std::allocator<unsigned short> > *,const class std::basic_string<unsigned short,struct std::char_traits<unsigned short>,class std::allocator<unsigned short> > &)' being compiled
CCachePageWriter.cpp
CCachePropertiesDlg.cpp
CCacheReportsPref.cpp
CCenterCoordsDlg.cpp
CCoords.cpp
CCsvFileWriter.cpp
CDatePickerDlg.cpp
CDistanceUnitsDlg.cpp
CDynStr.cpp
CExportLocationDlg.cpp
CExportLocationMgr.cpp
Generating Code...
Compiling...
CExportPreferences.cpp
CFieldNoteMgr.cpp
CFieldNotesReportPrefDlg.cpp
CFilterBearingDistance.cpp
CFilterBearingDistanceDlg.cpp
CFilterCacheContainers.cpp
CFilterCacheLists.cpp
CFilterCacheListsDlg.cpp
CFilterCacheRatings.cpp
CFilterCacheTB.cpp
CFilterCacheTypes.cpp
CFilterCacheTypesDlg.cpp
CFilterContainersDlg.cpp
CFilterIgnoredCachesDlg.cpp
CFilterMgr.cpp
CFilterMgrDlg.cpp
CFilterOnStrings.cpp
CFilterRatingsDlg.cpp
CFilterSearch.cpp
CFilterStringsDlg.cpp
Generating Code...
Compiling...
CGPSWriter.cpp
CGpxFileInfoDlg.cpp
CGpxParser.cpp
CHeading.cpp
CLineEditDlg.cpp
CListPreferencesDlg.cpp
CLocFileWriter.cpp
CLowranceWriter.cpp
CMd5.cpp
CMenuEdit.cpp
CMeridianWriter.cpp
CMiscPreferences.cpp
CMxfFileWriter.cpp
CMyAliasDlg.cpp
CMyCachesDlg.cpp
CMyTravelBugsDlg.cpp
CNavigonWriter.cpp
CommonDefs.cpp
Coord.cpp
COziWriter.cpp
Generating Code...
Compiling...
CPath.cpp
CPharosOstia6Writer.cpp
CPocketStreetPushPinsWriter.cpp
CProjectWptDlg.cpp
CRatingDlg.cpp
CRot13ConvertDlg.cpp
CSearchDlg.cpp
CStream.cpp
CTBActionMgr.cpp
CTBExchangeDlg.cpp
CTBInfoDlg.cpp
CTBMgr.cpp
CTextEditDlg.cpp
CTextTrx.cpp
CTimeHelper.cpp
CTypeAndContainerDlg.cpp
CWaypointsDlg.cpp
CWptProperties.cpp
CXmlMap.cpp
GpxSonar.cpp
Generating Code...
Compiling...
GpxSonarDoc.cpp
GpxSonarView.cpp
MainFrm.cpp
md5.cpp
NonFSDialog.cpp
Generating Code...
Compiling...
xmlparse.c
xmlrole.c
xmltok.c
Generating Code...
Linking...
   Creating library ARMV4Rel/GpxSonar.lib and object ARMV4Rel/GpxSonar.exp
LINK : warning LNK4089: all references to 'WININET.dll' discarded by /OPT:REF




<h3>Results</h3>
GpxSonar.exe - 0 error(s), 3 warning(s)
</pre>
</body>
</html>
