<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: Zlib - Win32 (WCE emulator) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP24B.tmp" with contents
[
/nologo /W3 /D "_i386_" /D "_X86_" /D "x86" /D "NDEBUG" /D "NO_ERRNO_H" /D "_WIN32_WCE" /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "_LIB" /Fp"emulatorRel/Zlib.pch" /YX /Fo"emulatorRel/" /Gs8192 /GF /O2  /c 
"D:\Projects\PPC2003\GpxSonar\Zlib\adler32.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\compress.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\crc32.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\deflate.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\gzio.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\infback.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\inffast.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\inflate.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\inftrees.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\trees.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\uncompr.c"
"D:\Projects\PPC2003\GpxSonar\Zlib\zutil.c"
]
Creating command line "cl.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP24B.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP24C.tmp" with contents
[
/nologo /out:"emulatorRel\Zlib.lib" 
.\emulatorRel\adler32.obj
.\emulatorRel\compress.obj
.\emulatorRel\crc32.obj
.\emulatorRel\deflate.obj
.\emulatorRel\gzio.obj
.\emulatorRel\infback.obj
.\emulatorRel\inffast.obj
.\emulatorRel\inflate.obj
.\emulatorRel\inftrees.obj
.\emulatorRel\trees.obj
.\emulatorRel\uncompr.obj
.\emulatorRel\zutil.obj
]
Creating command line "link.exe -lib @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP24C.tmp"
<h3>Output Window</h3>
Compiling...
adler32.c
compress.c
crc32.c
deflate.c
gzio.c
D:\Projects\PPC2003\GpxSonar\Zlib\gzio.c(1002) : warning C4013: 'strerror' undefined; assuming extern returning int
D:\Projects\PPC2003\GpxSonar\Zlib\gzio.c(1002) : warning C4047: ':' : 'int ' differs in levels of indirection from 'char *'
infback.c
inffast.c
inflate.c
inftrees.c
trees.c
uncompr.c
zutil.c
Creating library...




<h3>Results</h3>
Zlib.lib - 0 error(s), 2 warning(s)
</pre>
</body>
</html>
