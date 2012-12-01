#include <math.h>
#include <stdio.h>
#include <io.h>
#include <string.h>



//  32-bit types  -----------------------------------------------------------//
#if defined(_MSC_VER) && (_MSC_VER < 1500) /*vc6 and vc7.0 and vc8.0 and vc9.0*/
	# if UINT_MAX == 0xffffffff
		 typedef int             int32_t;
		 typedef int             int_least32_t;
		 typedef int             int_fast32_t;
		 typedef unsigned int    uint32_t;
		 typedef unsigned int    uint_least32_t;
		 typedef unsigned int    uint_fast32_t;
	# elif (USHRT_MAX == 0xffffffff)
		 typedef short             int32_t;
		 typedef short             int_least32_t;
		 typedef short             int_fast32_t;
		 typedef unsigned short    uint32_t;
		 typedef unsigned short    uint_least32_t;
		 typedef unsigned short    uint_fast32_t;
	# elif ULONG_MAX == 0xffffffff
		 typedef long            int32_t;
		 typedef long            int_least32_t;
		 typedef long            int_fast32_t;
		 typedef unsigned long   uint32_t;
		 typedef unsigned long   uint_least32_t;
		 typedef unsigned long   uint_fast32_t;
	# elif (UINT_MAX == 0xffffffffffffffff) && defined(__MTA__) 
		  // Integers are 64 bits on the MTA / XMT 
		  typedef __int32           int32_t; 
		  typedef __int32           int_least32_t; 
		  typedef __int32           int_fast32_t; 
		  typedef unsigned __int32  uint32_t; 
		  typedef unsigned __int32  uint_least32_t; 
		  typedef unsigned __int32  uint_fast32_t; 
	# else
	#    error defaults not correct; you must hand modify boost/cstdint.hpp
	# endif
#endif

void sincos(const double x, double *sinval, double *cosval);
/*******************************************************************************************************************************/
// i/o interface
//_Check_return_ _CRTIMP FILE * __cdecl open(_In_z_ const char * _Command, _In_z_ const char * _Mode){return _open(_Command, _Mode);};
//FILE *popen( const char *command, const char *mode ){return _open(command, mode);};
int popen( const char *command, int mode );
FILE *popen( const char *command, const char *mode );
int pclose( FILE *stream );
//int close(int fd ) {return _close(fd);};
//int dup(int fd ) {return _dup(fd);};
//int dup2(int fd1, int fd2 ) {return _dup2(fd1, fd2);};

//_CRTIMP int __cdecl close(_In_ int _FileHandle);
//_CRTIMP int __cdecl open(_In_z_ const char * _Filename, _In_ int _Openflag, _In_ int _PermissionMode = 0);
//_CRTIMP long __cdecl lseek(_In_ int _FileHandle, _In_ long _Offset, _In_ int _Origin);

const char * index ( const char * str, int character );
//#define open(x, y) _open(x, y)
//#define close(x) _close(x)
//#define lseek(x, y, z) _lseek(x, y, z)
struct iovec
{
  void*   iov_base;
  size_t  iov_len;
};
/******************************************************************************************************************************/
/* getpagesize for windows */
//
//typedef unsigned short WORD;
//typedef unsigned long DWORD;
//#if defined(_WIN64)
// typedef unsigned __int64 ULONG_PTR;
//#else
// typedef unsigned long ULONG_PTR;
//#endif
// typedef ULONG_PTR DWORD_PTR;
//typedef void *LPVOID;
//
//typedef struct _SYSTEM_INFO {
//  union {
//    DWORD  dwOemId;
//    struct {
//      WORD wProcessorArchitecture;
//      WORD wReserved;
//    };
//  };
//  DWORD     dwPageSize;
//  LPVOID    lpMinimumApplicationAddress;
//  LPVOID    lpMaximumApplicationAddress;
//  DWORD_PTR dwActiveProcessorMask;
//  DWORD     dwNumberOfProcessors;
//  DWORD     dwProcessorType;
//  DWORD     dwAllocationGranularity;
//  WORD      wProcessorLevel;
//  WORD      wProcessorRevision;
//} SYSTEM_INFO;
long getpagesize (void);
long getregionsize (void);




