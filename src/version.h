/****************************************************************
 * $ID: version.h      Thu, 30 Mar 2006 13:23:19 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer: 范美辉 (MeiHui FAN) <mhfan@ustc.edu>             *
 *                                                              *
 * Copyright (c) 2006 M.H.Fan, All rights reserved.             *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it              *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/
#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR                   0
#define VERSION_MINOR                   1
#define VERSION_REVIZ                   0

#define PACKAGE_NAME                    "FPlayer/libswf"
#define AUTHOR_EMAIL                    "<mhfan@ustc.edu>"
//#define AUTHOR_EMAIL                  "<meihui.fan@gmail.com>"
#define AUTHOR_NAME                     "MeiHui FAN"

#define VERSION_STRING_CONCAT_REAL(major, minor, reviz) \
        #major "." #minor "." #reviz

#define VERSION_STRING_CONCAT(major, minor, reviz) \
        VERSION_STRING_CONCAT_REAL(major, minor, reviz)

#if 1//def      __STDC__
#define VERSION_NUMBER  \
        (VERSION_MAJOR << 16 | VERSION_MINOR <<  8 | VERSION_REVIZ)

#define VERSION_STRING  \
        VERSION_STRING_CONCAT(VERSION_MAJOR, VERSION_MINOR, VERSION_REVIZ)

#define AUTHOR_STRING                   AUTHOR_NAME " " AUTHOR_EMAIL
#else
const unsigned VERSION_NUMBER =
        (VERSION_MAJOR << 16 | VERSION_MINOR <<  8 | VERSION_REVIZ)

const char VERSION_STRING[] =
        VERSION_STRING_CONCAT(VERSION_MAJOR, VERSION_MINOR, VERSION_REVIZ);
const char  AUTHOR_STRING[] = AUTHOR_NAME " " AUTHOR_EMAIL
#endif

#endif//VERSION_H
// vim:sts=4:ts=8:
