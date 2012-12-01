
#include <CVG.h>
#include <Windows.h>
#include <Winbase.h>
#include <WinDef.h>
#include <CVGtypes.h>

void sincos(const double x, double *sinval, double *cosval)
{
*sinval = sin(x);
*cosval = cos(x);
};
//_Check_return_ _CRTIMP FILE * __cdecl open(_In_z_ const char * _Command, _In_z_ const char * _Mode){return _open(_Command, _Mode);};
//FILE *popen( const char *command, const char *mode ){return _open(command, mode);};
int popen( const char *command, int mode )
	{return _open(command, mode);};
FILE *popen( const char *command, const char *mode )
	{return _popen(command, mode);};
int pclose( FILE *stream )
	{return _pclose(stream);};

//int open(_In_z_ const char * _Filename, _In_ int _Openflag, _In_ int _PermissionMode = 0)
//	{return _open(_Filename, _Openflag, _PermissionMode);};
//int close(_In_ int _FileHandle)
//	{return _close(_FileHandle);};
//long lseek(_In_ int _FileHandle, _In_ long _Offset, _In_ int _Origin)
//	{return _lseek(FileHandle, _Offset, _Origin);};

//int close(int fd ) {return _close(fd);};
//int dup(int fd ) {return _dup(fd);};
//int dup2(int fd1, int fd2 ) {return _dup2(fd1, fd2);};
const char * index ( const char * str, int character )
	{return strchr(str, character);};
/* getpagesize for windows */
long getpagesize (void) {
    static long g_pagesize = 0;
    if (! g_pagesize) {
        SYSTEM_INFO system_info;
        GetSystemInfo (&system_info);
        g_pagesize = system_info.dwPageSize;
    }
    return g_pagesize;
}
long getregionsize (void) {
    static long g_regionsize = 0;
    if (! g_regionsize) {
        SYSTEM_INFO system_info;
        GetSystemInfo (&system_info);
        g_regionsize = system_info.dwAllocationGranularity;
    }
    return g_regionsize;
}
