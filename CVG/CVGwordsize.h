/* Determine the wordsize from the preprocessor defines.  */
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
# define __WORDSIZE	64
#else
# define __WORDSIZE	32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
# define __WORDSIZE	64
#else
# define __WORDSIZE	32
#endif
#endif

//#define __GLIBC_HAVE_LONG_LONG 1
