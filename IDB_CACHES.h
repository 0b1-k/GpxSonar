#ifndef _INC_IDB_CACHES
	#define _INC_IDB_CACHES

#define BMP_START						10

#define TB_PRESENT						BMP_START + 1
#define EMPTY_BITMAP					BMP_START + 2
#define FIELD_NOTE						BMP_START + 3
#define FIELD_NOTE_FOUND				BMP_START + 4
#define FIELD_NOTE_NOTFOUND				BMP_START + 5
#define FIELD_NOTE_ARCHIVE				BMP_START + 6
#define GENERIC_CONTAINER				BMP_START + 7
#define FOUND_CACHE						BMP_START + 8
#define MY_CACHE						BMP_START + 9
#define CACHE_ARCHIVED					BMP_START + 10
#define CACHE_DISABLED					BMP_START + 11

#define WAYPOINT_IMAGE					0

#endif