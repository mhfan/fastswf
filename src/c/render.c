//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: render.c       Thu, 30 Mar 2006 09:41:47 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer: 范美辉 (MeiHui FAN)  <mhfan@ustc.edu>            *
 *                                                              *
 * CopyRight (c)  2006  M.H.Fan                                 *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it   	        *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/

#include <cairo.h>

#include "type.h"
#include "common.h"
#include "render.h"

static struct render_cairo {
    struct Framebuffer* fb;
    cairo_surface_t* sf;
    cairo_t* cr;
}   render_cairo;

static int render_cairo_init(struct swf_render* render, struct swf_rect* rect)
{
    int w, h;
    cairo_format_t fmt;
    struct Framebuffer* fb;
    assert(render && rect);
    fb = malloc(sizeof(*fb));		assert(fb);
    if (fbinit(fb, DEFAULT_FBDEV_PATH)) return 1;

    if (fb->xres < (w = TWIPS2PIXELS(rect->x1 - rect->x0))) {
	render->sx = (fb->xres << 16) / w;
	w = fb->xres;
    }
    if (fb->yres < (h = TWIPS2PIXELS(rect->y1 - rect->y0))) {
	render->sy = (fb->yres << 16) / h;
	h = fb->yres;
    }
    fbsetp(fb, 0, 0, w, h, FB_CENTER_MASK);

    switch (fb->depth) {
    case  1: fmt = CAIRO_FORMAT_A1;     break;
    case  8: fmt = CAIRO_FORMAT_A8;     break;
    case 16: fmt = CAIRO_FORMAT_RGB16_565;  break;
    case 24: fmt = CAIRO_FORMAT_RGB24;  break;
    case 32: fmt = CAIRO_FORMAT_ARGB32; break;
    default: fmt = CAIRO_FORMAT_RGB16_565;  break;
    }

    render_cairo.fb = fb;
    render_cairo.cr = cairo_create(render_cairo.sf =
	    cairo_image_surface_create_for_data((unsigned char*)fb->pixels
		    , fmt, fb->width, fb->height, fb->ppl * fb->bpp));

    render->priv = &render_cairo;	return 0;
}

static void render_cairo_setbgc(struct swf_rgb* rgb)
{
    cairo_t* cr = render_cairo.cr;
    struct Framebuffer* fb = render_cairo.fb;
    assert(rgb && cr && fb);
    cairo_rectangle(cr, 0, 0, fb->width, fb->height);
    cairo_set_source_rgb(cr, rgb->r/256.0, rgb->g/256.0, rgb->b/256.0);
    cairo_fill(cr);
}

static void render_cairo_unin(struct swf_render* render)
{
    cairo_surface_destroy(render_cairo.sf);
    cairo_destroy(render_cairo.cr);
    fbunin(render_cairo.fb);
    free(render_cairo.fb);
}

void swf_render_init(struct swf_render* render)
{
    assert(render);
    render->init    = render_cairo_init;
    render->setbgc  = render_cairo_setbgc;
    render->unin    = render_cairo_unin;
}

// vim:sts=4:ts=8:
