/* Copyright (C) 1992,94,1996-2000,2002,2004,2012
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef	_SYS_RESOURCE_H
#define	_SYS_RESOURCE_H	1

//#include <features.h>

/* Get the system-dependent definitions of structures and bit values.  */
//#include <bits/types.h>

/* These are the values for 4.4 BSD and GNU.  Earlier BSD systems have a
   subset of these kinds of resource limit.  In systems where `getrlimit'
   and `setrlimit' are not system calls, these are the values used by the C
   library to emulate them.  */
#define __THROW
/* Kinds of resource limit.  */
enum __rlimit_resource
  {
    /* Per-process CPU limit, in seconds.  */
    RLIMIT_CPU,
#define	RLIMIT_CPU	RLIMIT_CPU
    /* Largest file that can be created, in bytes.  */
    RLIMIT_FSIZE,
#define	RLIMIT_FSIZE	RLIMIT_FSIZE
    /* Maximum size of data segment, in bytes.  */
    RLIMIT_DATA,
#define	RLIMIT_DATA	RLIMIT_DATA
    /* Maximum size of stack segment, in bytes.  */
    RLIMIT_STACK,
#define	RLIMIT_STACK	RLIMIT_STACK
    /* Largest core file that can be created, in bytes.  */
    RLIMIT_CORE,
#define	RLIMIT_CORE	RLIMIT_CORE
    /* Largest resident set size, in bytes.
       This affects swapping; processes that are exceeding their
       resident set size will be more likely to have physical memory
       taken from them.  */
    RLIMIT_RSS,
#define	RLIMIT_RSS	RLIMIT_RSS
    /* Locked-in-memory address space.  */
    RLIMIT_MEMLOCK,
#define	RLIMIT_MEMLOCK	RLIMIT_MEMLOCK
    /* Number of processes.  */
    RLIMIT_NPROC,
#define	RLIMIT_NPROC	RLIMIT_NPROC
    /* Number of open files.  */
    RLIMIT_OFILE,
    RLIMIT_NOFILE = RLIMIT_OFILE, /* Another name for the same thing.  */
#define	RLIMIT_OFILE	RLIMIT_OFILE
#define	RLIMIT_NOFILE	RLIMIT_NOFILE
    /* Maximum size of all socket buffers.  */
    RLIMIT_SBSIZE,
#define RLIMIT_SBSIZE	RLIMIT_SBSIZE
    /* Maximum size in bytes of the process address space.  */
    RLIMIT_AS,
    RLIMIT_VMEM = RLIMIT_AS,	/* Another name for the same thing.  */
#define RLIMIT_AS	RLIMIT_AS
#define RLIMIT_VMEM	RLIMIT_AS

    RLIMIT_NLIMITS,		/* Number of limit flavors.  */
    RLIM_NLIMITS = RLIMIT_NLIMITS /* Traditional name for same.  */
  };

/* Value to indicate that there is no limit.  */
#ifndef __USE_FILE_OFFSET64
# define RLIM_INFINITY 0x7fffffff
#else
# define RLIM_INFINITY 0x7fffffffffffffffLL
#endif

#ifdef __USE_LARGEFILE64
# define RLIM64_INFINITY 0x7fffffffffffffffLL
#endif


/* Type for resource quantity measurement.  */
#ifndef __USE_FILE_OFFSET64
typedef __rlim_t rlim_t;
#else
typedef __rlim64_t rlim_t;
#endif
#ifdef __USE_LARGEFILE64
typedef __rlim64_t rlim64_t;
#endif

/* Type for resource quantity measurement.  */
//#define __ULONGWORD_TYPE	unsigned long int
#define __RLIM_T_TYPE		__ULONGWORD_TYPE
#define __rlim_t			__RLIM_T_TYPE
#define __RLIM64_T_TYPE		__UQUAD_TYPE
#define  __rlim64_t			__RLIM64_T_TYPE
#ifndef __USE_FILE_OFFSET64
struct rlimit
  {
    /* The current (soft) limit.  */
    rlim_t rlim_cur;
    /* The hard limit.  */
    rlim_t rlim_max;
  };

#ifdef __USE_LARGEFILE64
struct rlimit64
  {
    /* The current (soft) limit.  */
    rlim64_t rlim_cur;
    /* The hard limit.  */
    rlim64_t rlim_max;
 };
#endif

/* Whose usage statistics do you want?  */
enum __rusage_who
/* The macro definitions are necessary because some programs want
   to test for operating system features with #ifdef RUSAGE_SELF.
   In ISO C the reflexive definition is a no-op.  */
  {
    /* The calling process.  */
    RUSAGE_SELF = 0,
#define RUSAGE_SELF     RUSAGE_SELF
    /* All of its terminated child processes.  */
    RUSAGE_CHILDREN = -1
#define RUSAGE_CHILDREN RUSAGE_CHILDREN
  };
#ifdef vaffanculo
#define __need_timeval
#include <CVGtime.h>           /* For `struct timeval'.  */

/* Structure which says how much of each resource has been used.  */
struct rusage
  {
    /* Total amount of user time used.  */
    struct timeval ru_utime;
    /* Total amount of system time used.  */
    struct timeval ru_stime;
    /* Maximum resident set size (in kilobytes).  */
    long int ru_maxrss;
    /* Amount of sharing of text segment memory
       with other processes (kilobyte-seconds).  */
    long int ru_ixrss;
    /* Amount of data segment memory used (kilobyte-seconds).  */
    long int ru_idrss;
    /* Amount of stack memory used (kilobyte-seconds).  */
    long int ru_isrss;
    /* Number of soft page faults (i.e. those serviced by reclaiming
       a page from the list of pages awaiting reallocation.  */
    long int ru_minflt;
    /* Number of hard page faults (i.e. those that required I/O).  */
    long int ru_majflt;
    /* Number of times a process was swapped out of physical memory.  */
    long int ru_nswap;
    /* Number of input operations via the file system.  Note: This
       and `ru_oublock' do not include operations with the cache.  */
    long int ru_inblock;
    /* Number of output operations via the file system.  */
    long int ru_oublock;
    /* Number of IPC messages sent.  */
    long int ru_msgsnd;
    /* Number of IPC messages received.  */
    long int ru_msgrcv;
    /* Number of signals delivered.  */
    long int ru_nsignals;
    /* Number of voluntary context switches, i.e. because the process
       gave up the process before it had to (usually to wait for some
       resource to be available).  */
    long int ru_nvcsw;
    /* Number of involuntary context switches, i.e. a higher priority process
       became runnable or the current process used up its time slice.  */
    long int ru_nivcsw;
  };

/* Priority limits.  */
#define PRIO_MIN        -20     /* Minimum priority a process can have.  */
#define PRIO_MAX        20      /* Maximum priority a process can have.  */

/* The type of the WHICH argument to `getpriority' and `setpriority',
   indicating what flavor of entity the WHO argument specifies.  */
enum __priority_which
  {
    PRIO_PROCESS = 0,           /* WHO is a process ID.  */
#define PRIO_PROCESS PRIO_PROCESS
    PRIO_PGRP = 1,              /* WHO is a process group ID.  */
#define PRIO_PGRP PRIO_PGRP
    PRIO_USER = 2               /* WHO is a user ID.  */
#define PRIO_USER PRIO_USER

#ifndef __id_t_defined
typedef __id_t id_t;
# define __id_t_defined
#endif
#endif // vaffanculo
__BEGIN_DECLS

/* The X/Open standard defines that all the functions below must use
   `int' as the type for the first argument.  When we are compiling with
   GNU extensions we change this slightly to provide better error
   checking.  */
#if defined __USE_GNU && !defined __cplusplus
typedef enum __rlimit_resource __rlimit_resource_t;
typedef enum __rusage_who __rusage_who_t;
typedef enum __priority_which __priority_which_t;
#else
typedef int __rlimit_resource_t;
typedef int __rusage_who_t;
typedef int __priority_which_t;
#endif

/* Put the soft and hard limits for RESOURCE in *RLIMITS.
   Returns 0 if successful, -1 if not (and sets errno).  */
#ifndef __USE_FILE_OFFSET64
extern int getrlimit (__rlimit_resource_t __resource,
		      struct rlimit *__rlimits) __THROW;
#else
# ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH (getrlimit, (__rlimit_resource_t __resource,
				       struct rlimit *__rlimits), getrlimit64);
# else
#  define getrlimit getrlimit64
# endif
#endif
#ifdef __USE_LARGEFILE64
extern int getrlimit64 (__rlimit_resource_t __resource,
			struct rlimit64 *__rlimits) __THROW;
#endif

/* Set the soft and hard limits for RESOURCE to *RLIMITS.
   Only the super-user can increase hard limits.
   Return 0 if successful, -1 if not (and sets errno).  */
#ifndef __USE_FILE_OFFSET64
extern int setrlimit (__rlimit_resource_t __resource,
		      const struct rlimit *__rlimits) __THROW;
#else
# ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH (setrlimit, (__rlimit_resource_t __resource,
				       const struct rlimit *__rlimits),
			   setrlimit64);
# else
#  define setrlimit setrlimit64
# endif
#endif
#ifdef __USE_LARGEFILE64
extern int setrlimit64 (__rlimit_resource_t __resource,
			const struct rlimit64 *__rlimits) __THROW;
#endif

/* Return resource usage information on process indicated by WHO
   and put it in *USAGE.  Returns 0 for success, -1 for failure.  */
extern int getrusage (__rusage_who_t __who, struct rusage *__usage) __THROW;

/* Return the highest priority of any process specified by WHICH and WHO
   (see above); if WHO is zero, the current process, process group, or user
   (as specified by WHO) is used.  A lower priority number means higher
   priority.  Priorities range from PRIO_MIN to PRIO_MAX (above).  */
extern int getpriority (__priority_which_t __which, id_t __who) __THROW;

/* Set the priority of all processes specified by WHICH and WHO (see above)
   to PRIO.  Returns 0 on success, -1 on errors.  */
extern int setpriority (__priority_which_t __which, id_t __who, int __prio)
     __THROW;

__END_DECLS

#endif	/* sys/resource.h  */
