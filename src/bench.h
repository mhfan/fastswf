/***************************************************************
 * $ID: bench.h         Tue Nov 11 10:49:17 2003 CST  meihui $ *
 *                                                             *
 * Description:                                                *
 *                                                             *
 * Author:      范美辉  <mhfan@ustc.edu>                       *
 *                                                             *
 * Maintainer:  M.H. Fan  <mhfan@ustc.edu>                     *
 *              Laboratory of Structural Biology               *
 *              School of Life Science                         *
 *              Univ. of Sci.& Tech. of China (USTC)	       *
 *              People's Republic of China (PRC)               *
 *                                                             *
 * CopyRight (c)  2003  MeiHui FAN                             *
 *   All rights reserved.                                      *
 *                                                             *
 * This file is free software;                                 *
 *   you are free to modify and/or redistribute it  	       *
 *   under the terms of the GNU General Public Licence (GPL).  *
 *                                                             *
 * No warranty, no liability; use this at your own risk!       *
 ***************************************************************/
#ifndef BENCH_H
#define BENCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define	EPSILON			    0.000001F

#ifndef uint64_t
#define int64_t			    long long int
#define uint64_t		    unsigned long long int
#endif

/*
 * 在Intel Pentium以上级别(及其兼容型)的CPU中，有一个称为时间戳（Time Stamp）
 * 的部件，它以64位无符号整型数的格式，记录了自CPU上电以来所经过的时钟周期数。
 *
 * X86指令集提供了一条机器指令RDTSC（Read Time Stamp Counter）来读取这个时间戳
 * 的数字，并将其保存在EDX:EAX寄存器对中。由于EDX:EAX寄存器对恰好是Win32/Linux
 * 平台下C++语言保存函数返回值的寄存器，所以我们可以把这条指令看成是一个普通的
 * 函数调用。
 */

inline uint64_t rdtsc ()
{
    uint64_t dst;
    asm volatile ("RDTSC\n" : "=A" (dst));	// compatible with ANSI C.
    // asm volatile ("RDTSC");
    // asm ("_emit 0x0F\r\t"	"_emit 0x31");	// for cpp
    // asm ("_emit 0x0F; _emit 0x31");
    return dst;
}

float get_cpu_freq ()
{
    uint64_t freq = rdtsc(); sleep(1); return freq = rdtsc() - freq;
}

float getCpuFreq()
{
    size_t n=64;
    char* lineptr = (char*) malloc (n);
    FILE* fp = fopen ("/proc/cpuinfo", "r");
    do { getdelim (&lineptr, &n, '\n', fp);
    } while (0 != memcmp (lineptr, "cpu MHz", 7));
    n = 0;	while (lineptr[++n] != ':') ;
    fclose (fp);    return atof(lineptr+n+1);
}

float cpu_freq = 0.0F;

double cyc2sec (uint64_t cyc)
{
    if (cpu_freq < EPSILON) cpu_freq = get_cpu_freq ();
    //int64_t call_interval = rdtsc() - rdtsc();
    return (cyc/* - call_interval*/) / (double)cpu_freq;
}

#ifdef	DEBUG
void print (void)
{
    fprintf (stdout, "CPU clock cycle count: %Ld\n", rdtsc ());
    return;
}
#endif

#endif//BENCH_H
// vim:sts=4:ts=8:
