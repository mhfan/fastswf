/****************************************************************
 * $ID: errmsg.h          Thu Oct 30 11:07:30 2003 CST  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Author:      范美辉  <mhfan@ustc.edu>                        *
 *                                                              *
 * Maintainer:  M.H. Fan  <mhfan@ustc.edu>                      *
 *              Laboratory of Structural Biology                *
 *              School of Life Science                          *
 *              Univ. of Sci.& Tech. of China (USTC)	        *
 *              People's Republic of China (PRC)                *
 *                                                              *
 * CopyRight (c)  2005  MeiHui FAN                              *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it  	        *
 *   under the terms of the GNU General Public Licence.         *
 *                                                              *
 * No warranty, no liability; use this at your own risk!        *
 ****************************************************************/
#ifndef	ERRMSG_H
#define ERRMSG_H

#ifdef	__linux__
#include <unistd.h>   // __unix__
#endif

#ifdef	__STDC__
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#else//__cplusplus
#include <ctype>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <climits>

using std::strerror;
using std::fprintf;
using std::stderr;
using std::errno;

//using std::__FILE__;
//using std::__LINE__;
//using std::__TIME__;
//using std::__DATE__;
#endif//__STDC__

/*
 * NOTE: the "do {" ... "} while (0);" bracketing around the macros
 * allows the err_abort and errno_abort macros to be used as if they
 * were function calls, even in contexts where a trailing ";" would
 * generate a null statement. For example,
 *
 *      if (status != 0) err_abort (status, "message");
 *      else return status;
 *
 * will not compile if err_abort is a macro ending with "}", because
 * C does not expect a ";" to follow the "}". Because C does expect
 * a ";" following the ")" in the do...while construct, err_abort and
 * errno_abort can be used as if they were function calls.
 */

#ifdef	DEBUG
/*
 * Define a macro that can be used for diagnostic output from
 * examples. When compiled -DDEBUG, it results in calling printf
 * with the specified argument list. When DEBUG is not defined, it
 * expands to nothing.
 */

#if 1 // for tracing
#define dtrace	do { fprintf(stderr, "\033[36mTRACE"      \
			"\033[1;34m==>\033[33m%16s"       \
			"\033[36m: \033[32m%4d\033[36m: " \
			"\033[35m%-24s \033[34m"          \
			"[\033[0;37m%s\033[1;34m,"        \
			" \033[0;36m%s\033[1;34m]"        \
			"\033[0m\n", __FILE__, __LINE__,  \
			__FUNCTION__ /* __func__ */,      \
			__TIME__, __DATE__);              \
		} while (0)

#define dprintp(a,n) do { int i, m=sizeof((a)[0]);        \
	fprintf(stderr, "\033[33m" #a ": \033[36m"	  \
		"%p\033[0m ==>\n", a);			  \
		m = (m<2 ? 24 : (m<4 ? 16 : 8));	  \
		for (i=0; i < n; ) {			  \
		    int j=(i+m > n ? n-i : m);		  \
		    for (; j--; ++i)			  \
			if (m > 16) fprintf(stderr,	  \
				"%02x ", (a)[i]); else	  \
			if (m > 8) fprintf(stderr,	  \
				"%04x ", (a)[i]); else	  \
			fprintf(stderr, "%08x ", (a)[i]); \
		    fprintf(stderr, "\n"); }		  \
		} while (0)

#define dprintn(a) do { fprintf(stderr, "\033[33m" #a     \
			": \033[36m%#x, %d, %g\033[0m\n"  \
			, a, a, a);                       \
		} while (0)

#define dprints(a) do { fprintf(stderr, "\033[33m" #a     \
			": \033[36m%s\033[0m\n", a);      \
		} while (0)
#else
#define dtrace do { printk("\033[36mTRACE"		  \
			"\033[1;34m==>\033[33m%16s"       \
			"\033[36m: \033[32m%4d\033[36m: " \
			"\033[35m%-24s \033[34m"          \
			"[\033[0;37m%s\033[1;34m,"        \
			" \033[0;36m%s\033[1;34m]"        \
			"\033[0m\n", __FILE__, __LINE__,  \
			__FUNCTION__ /* __func__ */,      \
			__TIME__, __DATE__);              \
		} while (0)

#define dprintp(a,n) do { int i, m=sizeof((a)[0]);        \
	printk( "\033[33m" #a ": \033[36m"		  \
		"%p\033[0m ==>\n", a);			  \
	    m = (m<2 ? 24 : (m<4 ? 16 : 8));		  \
	    for (i=0; i < n; ) {			  \
		int j=(i+m > n ? n-i : m);		  \
		for (; j--; ++i)			  \
		    if (m > 16) printk("%02x "		  \
			    , (a)[i]); else		  \
		    if (m > 8)  printk("%04x "		  \
			    , (a)[i]); else		  \
		    printk("%08x ", (a)[i]);		  \
		printk("\n"); }				  \
	} while (0)

#define dprintn(a) do { printk("\033[33m" #a		  \
			": \033[36m%#x, %d, %g\033[0m\n"  \
			, a, a, a);                       \
		} while (0)

#define dprints(a) do { printk("\033[33m" #a		  \
			": \033[36m%s\033[0m\n", a);      \
		} while (0)
#endif

// for debuging
#define dmsg(msg) do { \
    fprintf(stderr, "[0;1;33mDEBUG[34m at \"[4;37m%s[0;1;34m\":" \
	    "[36m%d[34m: [32m%s[0;37m\n",  \
	    __FILE__, __LINE__, msg); \
} while (0)

#define debug() do { \
    fprintf(stderr, "[0;1;33mDEBUG[34m at \"[4;37m%s[0;1;34m\":" \
	    "[36m%d[34m: [32m%s[0;37m ([1;35m%d[0;37m)\n",  \
	    __FILE__, __LINE__, strerror(errno), errno); \
} while (0)

#ifdef	SANITY_WRAP
#define	sanity_fswrap(func, ...) \
	( { void tmp; \
	    if ((tmp = func(__VA_ARGS__)) < 0) \
		fprintf(stderr, "Fail to" ##func ": %s: %s\n", \
			__LINE__, strerror(errno)); \
	tmp; } )

#define	sanity_iowrap(func, fd, buf, len) \
	( { int n, m; /* assert(-1 < fd && buf && 0 < len); */ \
	    for (m=0; (n = func(fd, buf + m, len - m)) < (len - m) && 0 < n; \
		m += n) ; \
	    if (m < len)) fprintf(stderr, "Fail to " ##func ": %s: %s\n", \
		    __LINE__, strerror(errno)); \
	m; } )

#define	sanity_opwrap(func, fd, ...) \
	if ((fd = func(__VA_ARGS__)) < 0) \
	    fprintf(stderr, "Fail to" ##func ": %s: %s\n", \
		    __LINE__, strerror(errno)); \

#define	sanity_mkdir(p_, m_) \
	if ((n = mkdir(p_, m_)) && errno != EEXIST) \
	    fprintf(stderr, "Fail to mkdir `%s': %s\n", p_, strerror(errno));
#else
#define	sanity_fswrap(func, ...)	func(__VA_ARGS__)
#define	sanity_iowrap(func, ...)	func(__VA_ARGS__)
#define	sanity_opwrap(func, fd, ...)	(fd = func(__VA_ARGS__))
#define	sanity_mkdir			mkdir
#endif//SANITY_WRAP

#else
#define	dprint(...)
#define	dmsg(...)
#define	dtrace
#define	debug
#endif//DEBUG

// for the standard Ptheads errors.
#define err_abort(code,text) do { \
    fprintf(stderr, "[0;1;33m%s[34m at \"[4;37m%s[0;1;34m\":" \
	    "[36m%d[34m: [31m%s[0;37m ([1;35m%s[0;37m)\n", \
	    text, __FILE__, __LINE__, strerror(code), code); \
    abort(); \
} while (0)

// for the standard classic errors.
#define errno_abort(text) do { \
    fprintf(stderr, "[0;1;33m%s[34m at \"[4;37m%s[0;1;34m\":" \
	    "[36m%d[34m: [31m%s[0;37m ([1;35m%s[0;37m)\n", \
	    text, __FILE__, __LINE__, strerror(errno), errno); \
    abort(); \
} while (0)

#ifdef	__cplusplus
#include <new>

#if 0
void err_new_handl()
{
    fprintf(stderr, "[0;1;35mError[34m at \"[4;37m%s[0;1;34m\":"
		    "[36m%d[34m: [31mNo enough memory![0;37m\n",
	    __FILE__, __LINE__);
    abort();
}
#endif

//std::set_new_handler(err_new_handl);
#endif

#endif//ERRMSG_H
// vim:sts=4:ts=8:
