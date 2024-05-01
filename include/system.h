/*	SCCS Id: @(#)system.h	3.4	2001/12/07	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef SYSTEM_H
#define SYSTEM_H

#if !defined(__cplusplus)

#define E extern

/* some old <sys/types.h> may not define off_t and size_t; if your system is
 * one of these, define them by hand below
 */
#include <sys/types.h>


#if defined(ULTRIX) && !(defined(ULTRIX_PROTO) || defined(NHSTDC))
/* The Ultrix v3.0 <sys/types.h> seems to be very wrong. */
# define time_t long
#endif

#if defined(ULTRIX)
# define off_t long
#endif

#endif /* !__cplusplus */

/* You may want to change this to fit your system, as this is almost
 * impossible to get right automatically.
 * This is the type of signal handling functions.
 */
#ifndef SIG_RET_TYPE
# if defined(NHSTDC) || defined(POSIX_TYPES)
#  define SIG_RET_TYPE void (*)()
# endif
#endif
#ifndef SIG_RET_TYPE
# if defined(ULTRIX) || defined(SUNOS4) || defined(SVR3) || defined(SVR4)
	/* SVR3 is defined automatically by some systems */
#  define SIG_RET_TYPE void (*)()
# endif
#endif
#ifndef SIG_RET_TYPE	/* BSD, SIII, SVR2 and earlier, Sun3.5 and earlier */
# define SIG_RET_TYPE int (*)()
#endif

#if !defined(__cplusplus)

#if defined(BSD) || defined(ULTRIX) || defined(RANDOM)
# ifdef random
# undef random
# endif
# if !defined(LINUX)
E  long NDECL(random);
# endif
# if (!defined(SUNOS4) && !defined(bsdi) && !defined(__FreeBSD__)) || defined(RANDOM)
E void FDECL(srandom, (unsigned int));
# else
#  if !defined(bsdi) && !defined(__FreeBSD__)
E int FDECL(srandom, (unsigned int));
#  endif
# endif
#else
E long lrand48();
E void srand48();
#endif /* BSD || ULTRIX || RANDOM */

#if !defined(BSD) || defined(ultrix)
			/* real BSD wants all these to return int */
E void FDECL(exit, (int));
/* compensate for some CSet/2 bogosities */
/* If flex thinks that we're not __STDC__ it declares free() to return
   int and we die.  We must use __STDC__ instead of NHSTDC because
   the former is naturally what flex tests for. */
# if defined(__STDC__) || !defined(FLEX_SCANNER)
#   ifndef MONITOR_HEAP
E void FDECL(free, (genericptr_t));
#   endif
# endif
#  if !(defined(ULTRIX_PROTO) && defined(__GNUC__))
E void FDECL(perror, (const char *));
#  endif
#endif
#ifndef NeXT
#ifdef POSIX_TYPES
E void FDECL(qsort, (genericptr_t,size_t,size_t,
		     int(*)(const genericptr,const genericptr)));
#else
# if defined(BSD) || defined(ULTRIX)
E  int qsort();
# else
# if !defined(LATTICE)
E   void FDECL(qsort, (genericptr_t,size_t,size_t,
		       int(*)(const genericptr,const genericptr)));
#  endif
# endif
#endif
#endif /* NeXT */

#if !defined(__GNUC__)
/* may already be defined */

# ifdef ULTRIX
#  ifdef ULTRIX_PROTO
E int FDECL(lseek, (int,off_t,int));
#  else
E long FDECL(lseek, (int,off_t,int));
#  endif
  /* Ultrix 3.0 man page mistakenly says it returns an int. */
E int FDECL(write, (int,char *,int));
E int FDECL(link, (const char *, const char*));
# else
# ifndef bsdi
E long FDECL(lseek, (int,long,int));
# endif
# if defined(POSIX_TYPES)
#   ifndef bsdi
E int FDECL(write, (int, const void *,unsigned));
#   endif
#  else
E int FDECL(write, (int,genericptr_t,unsigned));
#  endif
# endif /* ULTRIX */

E int FDECL(unlink, (const char *));

#endif /* __GNUC__ */



#ifdef ULTRIX
E int FDECL(close, (int));
E int FDECL(atoi, (const char *));
E int FDECL(chdir, (const char *));
# if !defined(ULTRIX_CC20) && !defined(__GNUC__)
E int FDECL(chmod, (const char *,int));
E mode_t FDECL(umask, (int));
# endif
E int FDECL(read, (int,genericptr_t,unsigned));
/* these aren't quite right, but this saves including lots of system files */
E int FDECL(stty, (int,genericptr_t));
E int FDECL(gtty, (int,genericptr_t));
E int FDECL(ioctl, (int, int, char*));
E int FDECL(isatty, (int));	/* 1==yes, 0==no, -1==error */
#include <sys/file.h>
# if defined(ULTRIX_PROTO) || defined(__GNUC__)
E int NDECL(fork);
# else
E long NDECL(fork);
# endif
#endif /* ULTRIX */



/* both old & new versions of Ultrix want these, but real BSD does not */
#ifdef ultrix
E void abort();
E void bcopy();
# ifdef ULTRIX
E int FDECL(system, (const char *));
#  ifndef _UNISTD_H_
E int FDECL(execl, (const char *, ...));
#  endif
# endif
#endif
#if defined(HPUX) && !defined(_POSIX_SOURCE)
E long NDECL(fork);
#endif

#ifdef POSIX_TYPES
/* The POSIX string.h is required to define all the mem* and str* functions */
#include <string.h>
#else
#if defined(SYSV) || defined(SUNOS4)
# if defined(NHSTDC)
#  if !defined(_AIX32) && !(defined(SUNOS4) && defined(__STDC__))
				/* Solaris unbundled cc (acc) */
E int FDECL(memcmp, (const void *,const void *,size_t));
E void *FDECL(memcpy, (void *, const void *, size_t));
E void *FDECL(memset, (void *, int, size_t));
#  endif
# else
#  ifndef memcmp	/* some systems seem to macro these back to b*() */
E int memcmp();
#  endif
#  ifndef memcpy
E char *memcpy();
#  endif
#  ifndef memset
E char *memset();
#  endif
# endif
#else
# ifdef HPUX
E int FDECL(memcmp, (char *,char *,int));
E void *FDECL(memcpy, (char *,char *,int));
E void *FDECL(memset, (char*,int,int));
# endif
#endif
#endif /* POSIX_TYPES */


#if defined(BSD) && defined(ultrix)	/* i.e., old versions of Ultrix */
E void sleep();
#endif
#if defined(ULTRIX) || defined(SYSV)
E unsigned sleep();
#endif
#if defined(HPUX)
E unsigned int FDECL(sleep, (unsigned int));
#endif

E char *FDECL(getenv, (const char *));
E char *getlogin();
#if defined(HPUX) && !defined(_POSIX_SOURCE)
E long NDECL(getuid);
E long NDECL(getgid);
E long NDECL(getpid);
#else
# ifdef POSIX_TYPES
E pid_t NDECL(getpid);
E uid_t NDECL(getuid);
E gid_t NDECL(getgid);
# else	/*!POSIX_TYPES*/
#  ifndef getpid		/* Borland C defines getpid() as a macro */
E int NDECL(getpid);
#  endif
#  if defined(ULTRIX) && !defined(_UNISTD_H_)
E unsigned NDECL(getuid);
E unsigned NDECL(getgid);
E int FDECL(setgid, (int));
E int FDECL(setuid, (int));
#  endif
# endif	/*?POSIX_TYPES*/
#endif	/*?(HPUX && !_POSIX_SOURCE)*/

/* add more architectures as needed */
#if defined(HPUX)
#define seteuid(x) setreuid(-1, (x));
#endif

/*# string(s).h #*/
#if !defined(_XtIntrinsic_h) && !defined(POSIX_TYPES)
/* <X11/Intrinsic.h> #includes <string[s].h>; so does defining POSIX_TYPES */

#if (defined(ULTRIX) || defined(NeXT)) && defined(__GNUC__)
#include <strings.h>
#else
E char	*FDECL(strcpy, (char *,const char *));
E char	*FDECL(strncpy, (char *,const char *,size_t));
E char	*FDECL(strcat, (char *,const char *));
E char	*FDECL(strncat, (char *,const char *,size_t));
E char	*FDECL(strpbrk, (const char *,const char *));

# if defined(SYSV) || defined(HPUX)
E char	*FDECL(strchr, (const char *,int));
E char	*FDECL(strrchr, (const char *,int));
# else /* BSD */
E char	*FDECL(index, (const char *,int));
E char	*FDECL(rindex, (const char *,int));
# endif

E int	FDECL(strcmp, (const char *,const char *));
E int	FDECL(strncmp, (const char *,const char *,size_t));
# ifdef HPUX
E unsigned int	FDECL(strlen, (char *));
#  else
#   if !(defined(ULTRIX_PROTO) && defined(__GNUC__))
E int	FDECL(strlen, (const char *));
#   endif
#  endif /* HPUX */
#endif /* ULTRIX */

#endif	/* !_XtIntrinsic_h_ && !POSIX_TYPES */

#if defined(ULTRIX) && defined(__GNUC__)
E char	*FDECL(index, (const char *,int));
E char	*FDECL(rindex, (const char *,int));
#endif

/* Old varieties of BSD have char *sprintf().
 * Newer varieties of BSD have int sprintf() but allow for the old char *.
 * Several varieties of SYSV and PC systems also have int sprintf().
 * If your system doesn't agree with this breakdown, you may want to change
 * this declaration, especially if your machine treats the types differently.
 * If your system defines sprintf, et al, in stdio.h, add to the initial
 * #if.
 */
#if defined(ULTRIX)
#define SPRINTF_PROTO
#endif
#if (defined(SUNOS4) && defined(__STDC__)) || defined(_AIX32)
#define SPRINTF_PROTO
#endif
#if defined(__sgi) || defined(__GNUC__)
	/* problem with prototype mismatches */
#define SPRINTF_PROTO
#endif

#ifndef SPRINTF_PROTO
# if defined(POSIX_TYPES) || defined(DGUX) || defined(NeXT) || !defined(BSD)
E  int FDECL(sprintf, (char *,const char *,...));
# else
#  define OLD_SPRINTF
E  char *sprintf();
# endif
#endif
#ifdef SPRINTF_PROTO
# undef SPRINTF_PROTO
#endif

#ifdef NEED_VARARGS
# if !defined(SVR4)
#  if !(defined(ULTRIX_PROTO) && defined(__GNUC__))
#   if !(defined(SUNOS4) && defined(__STDC__)) /* Solaris unbundled cc (acc) */

E int FDECL(vfprintf, (FILE *, const char *, va_list));
E int FDECL(vprintf, (const char *, va_list));

#if defined(__GNUC__) && !defined(__APPLE__)
	E int FDECL(vsprintf, (char *, const char *, va_list));
#endif

#   endif
#  endif
# endif
#endif /* NEED_VARARGS */


# if ! (defined(HPUX) && defined(_POSIX_SOURCE))
E int FDECL(tgetent, (char *,const char *));
E void FDECL(tputs, (const char *,int,int (*)()));
# endif
E int FDECL(tgetnum, (const char *));
E int FDECL(tgetflag, (const char *));
E char *FDECL(tgetstr, (const char *,char **));
E char *FDECL(tgoto, (const char *,int,int));

#ifdef ALLOC_C
E genericptr_t FDECL(malloc, (size_t));
#endif

/* time functions */

# ifndef LATTICE
#  if !(defined(ULTRIX_PROTO) && defined(__GNUC__))
E struct tm *FDECL(localtime, (const time_t *));
#  endif
# endif

# if defined(ULTRIX) || (defined(BSD) && defined(POSIX_TYPES)) || defined(SYSV) || (defined(HPUX) && defined(_POSIX_SOURCE))
E time_t FDECL(time, (time_t *));
# else
E long FDECL(time, (time_t *));
# endif /* ULTRIX */




#undef E

#endif /*  !__cplusplus */

#endif /* SYSTEM_H */
