//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: jpeg.cpp       Fri, 07 Apr 2006 15:26:20 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  范美辉(MeiHui FAN)  <mhfan@ustc.edu>            *
 *                                                              *
 * CopyRight (c)  2006  M.H.Fan                                 *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it   	        *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/

#include "jpeg.hpp"

// TODO: wrap into a class

void js_init_ource(struct jpeg_decompress_struct* jpds)
	{ jpds->src->bytes_in_buffer = 0; }

boolean js_fill_input_buffer(struct jpeg_decompress_struct* jpds)
{
    struct JPEGClient* jclt = (struct JPEGClient*)jpds->client_data;
    if (jclt->data) {
	jpds->src->next_input_byte = (JOCTET*)jclt->data;
	jpds->src->bytes_in_buffer =	      jclt->size;
	jclt->data = NULL;
    }	return TRUE;
}

void js_skip_input_data(struct jpeg_decompress_struct* jpds, long count)
{
    jpds->src->next_input_byte += count;
    jpds->src->bytes_in_buffer -= count;
}

#if 0
boolean js_resync_to_restart(struct jpeg_decompress_struct* jpds, int desired)
	{ return jpeg_resync_to_restart(jpds, desired); }
#endif

void js_term_source(struct jpeg_decompress_struct* jpds) { }

void j_error_exit(struct jpeg_common_struct* jpds)
{
    struct JPEGClient* jclt = (struct JPEGClient*)jpds->client_data;
    (jpds->err->output_message)(jpds);
    longjmp(jclt->jbuf, 1);
}

// vim:sts=4:ts=8:
