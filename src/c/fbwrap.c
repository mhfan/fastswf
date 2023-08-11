/****************************************************************
 * $ID: fbwrap.c       Thu, 30 Mar 2006 22:56:13 +0800  mhfan $ *
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

#include "fbwrap.h"

void fbunin(struct Framebuffer* fb)
{
    if (fb->scr && munmap(fb->scr, fb->size) < 0)
        fprintf(stderr, "Unmap frame buffer: %s\n", strerror(errno));
    if (fb->flag & FB_TRANSFORM_MASK) free(fb->pixels);
    if (0 < fb->fd) close(fb->fd);
}

int  fbinit(struct Framebuffer* fb, char* path)
{
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;

    if ((fb->fd = open(path, O_RDWR)) < 0 ||
            ioctl(fb->fd, FBIOGET_FSCREENINFO, &finfo) ||
            ioctl(fb->fd, FBIOGET_VSCREENINFO, &vinfo) ||
            (fb->scr = mmap(0, (fb->size = finfo.smem_len),
                    (PROT_READ | PROT_WRITE), MAP_SHARED, fb->fd, 0)) ==
                            (void*)-1) {
        fprintf(stderr, "%s: %s\n", path, strerror(errno));     return -1;
    }

    fb->flag  = 0x00;
    fb->xres  = vinfo.xres;
    fb->yres  = vinfo.yres;
    fb->depth = vinfo.red  .length + vinfo.blue  .length +
                vinfo.green.length + vinfo.transp.length;
    fb->bpp   =(vinfo.bits_per_pixel + 7) / 8;  // XXX
    fb->bpl   = finfo.line_length; //vinfo.xres * fb->bpp;

    fprintf(stdout, "FB: %dx%d@%d; ", fb->xres, fb->yres, fb->depth);

    return 0;
}

int  fbsetp(struct Framebuffer* fb, int x, int y, int w, int h, int s)
{
    int reset = (fb->flag & FB_TRANSFORM_MASK);  fb->flag = s;

    if (w < 0) w = -w, fb->flag =      (fb->flag & FB_FLIP_WX) ?
            (fb->flag & ~FB_FLIP_WX) : (fb->flag | FB_FLIP_WX);
    if (h < 0) h = -h, fb->flag =      (fb->flag & FB_FLIP_HY) ?
            (fb->flag & ~FB_FLIP_HY) : (fb->flag | FB_FLIP_HY);

    //x = (fb->xres + x) % fb->xres;    y = (fb->yres + y) % fb->yres;
    if (x < 0) x = fb->xres + x;        if (y < 0) y = fb->yres + y;

    if (fb->xres < x + w || fb->yres < y + h)   return -1;

    if (s & FB_SWAP_XY) {
        if (s & FB_CENTERX) x = (fb->xres - h) / 2;
        if (s & FB_CENTERY) y = (fb->yres - w) / 2;
    } else {
        if (s & FB_CENTERX) x = (fb->xres - w) / 2;
        if (s & FB_CENTERY) y = (fb->yres - h) / 2;
    }   fb->width = w;        fb->height = h;

    if (reset) free(fb->pixels);
    if (fb->flag & FB_TRANSFORM_MASK) {
        fb->pixels = malloc((fb->ppl = w)  * fb->bpp * h);
        fb->base   = fb->scr + fb->bpl * y + fb->bpp * x;
    } else {
        fb->base   = fb->scr + fb->bpl * y + fb->bpp * x;
        fb->pixels = (void*)fb->base;
        fb->ppl    = fb->bpl / fb->bpp;
    }

    fprintf(stdout, "%#lx; X: %d, Y: %d, W: %d, H: %d; PPL: %d\n",
            fb->flag, x, y, w, h, fb->ppl);

    return 0;
}

void fbrefr(struct Framebuffer* fb)
{
    switch ((fb->flag & FB_TRANSFORM_MASK)){
    case FB_FLIP_WX | FB_FLIP_HY | FB_SWAP_XY: {
        int w, h, dgap = fb->bpl / fb->bpp  - fb->height,
                  sgap = fb->width * fb->height - 1;
        pixel_t *dst = (pixel_t*)fb->base,
                *src = (pixel_t*)fb->pixels + fb->width * fb->height - 1;
        for (h=0; h++ < fb->width; ) {
            for (w=0; w++ < fb->height; ) {
                *dst++ = *src;
                src -= fb->width;
            }   src += sgap, dst += dgap;
        }
    }   break;
    case FB_FLIP_WX | FB_SWAP_XY: {
        int w, h, dgap = fb->bpl / fb->bpp  - fb->height,
                  sgap = fb->width * fb->height + 1;
        pixel_t *dst = (pixel_t*)fb->base,
                *src = (pixel_t*)fb->pixels - fb->width +
                        fb->width * fb->height;
        for (h=0; h++ < fb->width; ) {
            for (w=0; w++ < fb->height; ) {
                *dst++ = *src;
                src -= fb->width;
            }   src += sgap, dst += dgap;
        }
    }   break;
    case FB_SWAP_XY | FB_FLIP_HY: {
        int w, h, dgap = fb->bpl / fb->bpp  - fb->height,
                  sgap = fb->width * fb->height + 1;
        pixel_t *dst = (pixel_t*)fb->base,
                *src = (pixel_t*)fb->pixels + fb->width - 1;
        for (h=0; h++ < fb->width; ) {
            for (w=0; w++ < fb->height; ) {
                *dst++ = *src;
                src += fb->width;
            }   src -= sgap, dst += dgap;
        }
    }   break;
    case FB_FLIP_WX | FB_FLIP_HY: {
        int w, h, dgap = fb->bpl / fb->bpp  - fb->width;
        pixel_t *dst = (pixel_t*)fb->base,
                *src = (pixel_t*)fb->pixels + fb->width * fb->height;
        for (h=0; h++ < fb->height; ) {
            for (w=0; w++ < fb->width; ) *dst++ = *(--src);
            dst += dgap;
        }
    }   break;
    case FB_FLIP_HY: {
        int w, h, dgap = fb->bpl / fb->bpp  - fb->width;
        pixel_t *dst = (pixel_t*)fb->base,
                *src = (pixel_t*)fb->pixels - fb->width +
                        fb->width * fb->height;
        for (h=0; h++ < fb->height; ) {
            for (w=0; w++ < fb->width; ) *dst++ = *src++;
            src -= fb->width * 2;          dst += dgap;
        }
    }   break;
    case FB_FLIP_WX: {
        int w, h, dgap = fb->bpl / fb->bpp  - fb->width;
        pixel_t *dst = (pixel_t*)fb->base,
                *src = (pixel_t*)fb->pixels + fb->width;
        for (h=0; h++ < fb->height; ) {
            for (w=0; w++ < fb->width; ) *dst++ = *(--src);
            src += fb->width * 2;         dst += dgap;
        }
    }   break;
    case FB_SWAP_XY: {
        int w, h, dgap = fb->bpl / fb->bpp - fb->height,
                  sgap = fb->width * fb->height - 1;
        pixel_t *dst = (pixel_t*)fb->base, *src = (pixel_t*)fb->pixels;
        for (h=0; h++ < fb->width; ) {
            for (w=0; w++ < fb->height; ) {
                *dst++ = *src;
                src += fb->width;
            }   src -= sgap, dst += dgap;
        }
    }   break;
    default: ;
    }
}

// vim:sts=4:ts=8:
