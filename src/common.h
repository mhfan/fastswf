//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: common.h       Mon, 27 Mar 2006 21:26:41 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  范美辉(MeiHui FAN)  <mhfan@ustc.edu>            *
 *                                                              *
 * CopyRight (c)  2006  M.H.Fan                                 *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it              *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/
#ifndef COMMON_H
#define COMMON_H

#define DEBUG                           1
#define SANITY_WRAP                     1
#define STREAMING_READ                  1

#include "errmsg.h"

#define MAX(a, b)                       (a < b ? b : a)
#define MIN(a, b)                       (a < b ? a : b)

#define NORMAL_FILE_MODE                (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DATA_DIR_MODE                   (0755)

#define CUBIC_BEZIER                    1 // for cairo/twin render

#define ARRAY_SIZE(x)   ((unsigned)(sizeof(x) / sizeof((x)[0])))
#define BITSOF(t)                       (sizeof(t) << 3)

#endif//COMMON_H
// vim:sts=4:ts=8:
