//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: render.h       Thu, 30 Mar 2006 09:41:54 +0800  mhfan $ *
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
#ifndef RENDER_H
#define RENDER_H

#include "type.h"
#include "fbwrap.h"

struct swf_render {
    uf16_16_t sx, sy;
    int  (*init)(struct swf_render* render, struct swf_rect* rect);
    void (*unin)(struct swf_render* render);
    void (*setbgc)(struct swf_rgb* rgb);
    void* priv;
};

void swf_render_init(struct swf_render* render);

#endif//RENDER_H
// vim:sts=4:ts=8:
