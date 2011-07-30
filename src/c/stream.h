//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: stream.h       Sun, 02 Apr 2006 13:05:54 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  ∑∂√¿ª‘(MeiHui FAN)  <mhfan@ustc.edu>            *
 *                                                              *
 * CopyRight (c)  2006  M.H.Fan                                 *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it   	        *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/
#ifndef STREAM_H
#define STREAM_H

struct stream {
    void* strm;
    long (* read) (void* buf, long size, long memb, void* strm);
    long (* write)(void* buf, long size, long memb, void* strm);
    int  (* seek) (void* strm, long off, int wh);
    int  (* close)(void* strm);
    int  (* eof)  (void* strm);
};

#endif//STREAM_H
// vim:sts=4:ts=8:
