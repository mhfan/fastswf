//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: jpeg.hpp       Fri, 07 Apr 2006 15:25:44 +0800  mhfan $ *
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
#ifndef JPEG_HPP
#define JPEG_HPP

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <setjmp.h>
#include <jpeglib.h>

#ifdef  __cplusplus
}
#endif

#include "bs.hpp"

// TODO: wrap into a class

struct JPEGClient {
    long size;
    char* data;
    jmp_buf jbuf;
};

void j_error_exit(struct jpeg_common_struct* cinfo);
void js_init_ource(struct jpeg_decompress_struct* cinfo);
void js_term_source(struct jpeg_decompress_struct* cinfo);
boolean js_fill_input_buffer(struct jpeg_decompress_struct* cinfo);
void js_skip_input_data(struct jpeg_decompress_struct* cinfo, long count);
boolean js_resync_to_restart(struct jpeg_decompress_struct* cinfo, int desired);

#endif//JPEG_HPP
// vim:sts=4:ts=8:
