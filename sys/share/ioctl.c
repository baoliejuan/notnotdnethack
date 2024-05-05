/*	SCCS Id: @(#)ioctl.c	3.4	1990/22/02 */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/* This cannot be part of hack.tty.c (as it was earlier) since on some
   systems (e.g. MUNIX) the include files <termio.h> and <sgtty.h>
   define the same constants, and the C preprocessor complains. */

#include "hack.h"

#if defined(BSD_JOB_CONTROL)
#    include <bsd/sgtty.h>
struct ltchars ltchars;
struct ltchars ltchars0 = { -1, -1, -1, -1, -1, -1 }; /* turn all off */
#else

#include <termios.h>
struct termios termio;
#endif

#ifdef SUSPEND	/* BSD isn't alone anymore... */
#include	<signal.h>
#endif

#ifdef __linux__
extern void NDECL(linux_mapon);
extern void NDECL(linux_mapoff);
#endif

void
getwindowsz(void)
{
#ifdef USE_WIN_IOCTL
    /*
     * ttysize is found on Suns and BSD
     * winsize is found on Suns, BSD, and Ultrix
     */
    struct winsize ttsz;

    if (ioctl(fileno(stdin), (int)TIOCGWINSZ, (char *)&ttsz) != -1) {
	/*
	 * Use the kernel's values for lines and columns if it has
	 * any idea.
	 */
	if (ttsz.ws_row)
	    LI = ttsz.ws_row;
	if (ttsz.ws_col)
	    CO = ttsz.ws_col;
    }
#endif
}

void
getioctls(void)
{
#ifdef BSD_JOB_CONTROL
	(void) ioctl(fileno(stdin), (int) TIOCGLTC, (char *) &ltchars);
	(void) ioctl(fileno(stdin), (int) TIOCSLTC, (char *) &ltchars0);
#else
	(void) tcgetattr(fileno(stdin), &termio);
#endif
	getwindowsz();
}

void
setioctls(void)
{
#ifdef BSD_JOB_CONTROL
	(void) ioctl(fileno(stdin), (int) TIOCSLTC, (char *) &ltchars);
#else
	(void) tcsetattr(fileno(stdin), TCSADRAIN, &termio);
#endif
}

#ifdef SUSPEND		/* No longer implies BSD */
int
dosuspend(void)
{
# ifdef SIGTSTP
	if(signal(SIGTSTP, SIG_IGN) == SIG_DFL) {
		suspend_nhwindows((char *)0);
#  ifdef __linux__
		linux_mapon();
#  endif
		(void) signal(SIGTSTP, SIG_DFL);
		(void) kill(0, SIGTSTP);
#  ifdef __linux__
		linux_mapoff();
#  endif
		resume_nhwindows();
	} else {
		pline("I don't think your shell has job control.");
	}
# else
	pline("Sorry, it seems we have no SIGTSTP here.  Try ! or S.");
# endif
	return(0);
}
#endif /* SUSPEND */
