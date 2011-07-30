//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: type.c         Tue, 04 Apr 2006 01:56:58 +0800  mhfan $ *
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

#include "bs.h"
#include "type.h"
#include "common.h"

void swf_read_rect(struct bstream* bs, struct swf_rect* rect)
{
    unsigned nbit;
    assert(bs && rect);
    bs_align_byte(bs);
    nbit = bs_read_unb(bs, SWF_RECT_NBIT);
    rect->x0 = bs_read_snb(bs, nbit);
    rect->x1 = bs_read_snb(bs, nbit);
    rect->y0 = bs_read_snb(bs, nbit);
    rect->y1 = bs_read_snb(bs, nbit);
}

void swf_read_matrix(struct bstream* bs, struct swf_matrix* mat)
{
    unsigned has, nbit;
    assert(bs && mat);
    bs_align_byte(bs);
    if ((has = bs_read_unb(bs, 1))) {
	nbit = bs_read_unb(bs, SWF_MATRIX_SNBIT);
	mat->sx = bs_read_snb(bs, nbit);
	mat->sy = bs_read_snb(bs, nbit);
    } else mat->sx = mat->sy = SWF_MATRIX_DEFAULT_SCALE;
    if ((has = bs_read_unb(bs, 1))) {
	nbit = bs_read_unb(bs, SWF_MATRIX_RNBIT);
	mat->r0 = bs_read_snb(bs, nbit);
	mat->r1 = bs_read_snb(bs, nbit);
    } else mat->r0 = mat->r1 = SWF_MATRIX_DEFAULT_ROTATE;
    nbit = bs_read_unb(bs, SWF_MATRIX_TNBIT);
    mat->tx = bs_read_snb(bs, nbit);
    mat->ty = bs_read_snb(bs, nbit);
}

void swf_read_rgba(struct bstream* bs, struct swf_rgba* rgba)
{
    rgba->r = bs_read_u8(bs);
    rgba->g = bs_read_u8(bs);
    rgba->b = bs_read_u8(bs);
    rgba->a = bs_read_u8(bs);
}

void swf_read_rgb(struct bstream* bs, struct swf_rgb* rgb)
{
    rgb->r = bs_read_u8(bs);
    rgb->g = bs_read_u8(bs);
    rgb->b = bs_read_u8(bs);
}

void swf_read_cxform(struct bstream* bs, struct swf_cxform* cxf)
{
    uint8_t nbit;
    assert(bs && cxf);
    bs_align_byte(bs);
    cxf->ar = bs_read_unb(bs, 1);
    cxf->mr = bs_read_unb(bs, 1);
    nbit = bs_read_unb(bs, SWF_CXFORM_NBIT);
    if (cxf->mr) {
	cxf->mr = bs_read_snb(bs, nbit);
	cxf->mg = bs_read_snb(bs, nbit);
	cxf->mb = bs_read_snb(bs, nbit);
	cxf->ma = cxf->ma == SHRT_MAX ? bs_read_snb(bs, nbit)
		: SWF_CXFORM_DEFAULT_MUL;
    } else cxf->mr = cxf->mg = cxf->mb = cxf->ma = SWF_CXFORM_DEFAULT_MUL;
    if (cxf->ar) {
	cxf->ar = bs_read_snb(bs, nbit);
	cxf->ag = bs_read_snb(bs, nbit);
	cxf->ab = bs_read_snb(bs, nbit);
	cxf->aa = cxf->aa == SHRT_MAX ? bs_read_snb(bs, nbit)
		: SWF_CXFORM_DEFAULT_ADD;
    } else cxf->ar = cxf->ag = cxf->ab = cxf->aa = SWF_CXFORM_DEFAULT_ADD;
}

// vim:sts=4:ts=8:
