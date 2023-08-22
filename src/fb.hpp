//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: fb.hpp         Thu, 06 Apr 2006 16:49:11 +0800  mhfan $ *
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
#ifndef FB_HPP
#define FB_HPP

extern "C" {

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

};

typedef uint16_t                        pixel_t;

struct FrameBuffer {
    enum {
        SWAP_XY = (0x01 << 0),
        FLIP_WX = (0x01 << 1),
        FLIP_HY = (0x01 << 2),
        CENTERX = (0x01 << 3),
        CENTERY = (0x01 << 4),
        CENTER_MASK     = (CENTERX | CENTERY),
        TRANSFORM_MASK  = (SWAP_XY | FLIP_WX | FLIP_HY),
    };

#define DEFAULT_FBDEV_PATH              "/dev/fb0"

    void* tty;

    pixel_t* pixl;
    int width, height, depth, ppl, bpp;

    ~FrameBuffer() {
        if (scrn && munmap(scrn, size) < 0)
            fprintf(stderr, "Unmap frame buffer: %s\n", strerror(errno));
        if (flag & TRANSFORM_MASK) free(pixl);
        if (0 < fd) close(fd);
    }

     FrameBuffer(const char* path = DEFAULT_FBDEV_PATH) {
        struct fb_fix_screeninfo finfo;
        struct fb_var_screeninfo vinfo;

        if ((fd = open(path, O_RDWR)) < 0 ||
                ioctl(fd, FBIOGET_FSCREENINFO, &finfo) ||
                ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) ||
                (scrn = (unsigned char*)mmap(0, (size = finfo.smem_len),
                        (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0)) ==
                                (void*)-1) {
            fprintf(stderr, "%s: %s\n", path, strerror(errno));
            return; //-1;
        }

        flag  = 0x00;
        pixl  = (pixel_t*)(base = scrn);
        xres  = width = vinfo.xres;     yres = height = vinfo.yres;
        depth = vinfo.red  .length + vinfo.blue  .length +
                    vinfo.green.length + vinfo.transp.length;
        bpp   =(vinfo.bits_per_pixel + 7) / 8;  // XXX:
        bpl   = finfo.line_length; //vinfo.xres * bpp;

        fprintf(stdout, "FB: %dx%d@%d; ", xres, yres, depth);
    }

    int viewport(int x, int y, int w, int h, unsigned long s) {
        int reset = (flag & TRANSFORM_MASK);     flag = s;

        if (w < 0) w = -w, flag =      (flag & FLIP_WX) ?
                (flag & ~FLIP_WX) : (flag | FLIP_WX);
        if (h < 0) h = -h, flag =      (flag & FLIP_HY) ?
                (flag & ~FLIP_HY) : (flag | FLIP_HY);
        
        //x = (xres + x) % xres;        y = (yres + y) % yres;
        if (x < 0) x = xres + x;        if (y < 0) y = yres + y;

        if (xres < x + w || yres < y + h)       return -1;

        if (s & SWAP_XY) {
            if (s & CENTERX) x = (xres - h) / 2;
            if (s & CENTERY) y = (yres - w) / 2;
        } else {
            if (s & CENTERX) x = (xres - w) / 2;
            if (s & CENTERY) y = (yres - h) / 2;
        }       width = w;            height = h;

        if (reset) free(pixl);
        if (flag & TRANSFORM_MASK) {
            pixl = (pixel_t*)malloc((ppl = w) * bpp * h);
            base = scrn + bpl * y + bpp * x;
        } else {
            base = scrn + bpl * y + bpp * x;
            pixl = (pixel_t*)base;
            ppl  = bpl / bpp;
        }

        fprintf(stdout, "%#lx; X: %d, Y: %d, W: %d, H: %d; PPL: %d\n",
                flag, x, y, w, h, ppl);

        return 0;
    }

    void refresh() {    // TODO: implement with templete
        switch ((flag & TRANSFORM_MASK)){
        case FLIP_WX | FLIP_HY | SWAP_XY: {
            int w, h, dgap = bpl / bpp  - height,
                    sgap = width * height - 1;
            pixel_t *dst = (pixel_t*)base,
                    *src = (pixel_t*)pixl + width * height - 1;
            for (h=0; h++ < width; ) {
                for (w=0; w++ < height; ) {
                    *dst++ = *src;
                    src -= width;
                }       src += sgap, dst += dgap;
            }
        }       break;

        case FLIP_WX | SWAP_XY: {
            int w, h, dgap = bpl / bpp  - height,
                    sgap = width * height + 1;
            pixel_t *dst = (pixel_t*)base,
                    *src = (pixel_t*)pixl - width +
                            width * height;
            for (h=0; h++ < width; ) {
                for (w=0; w++ < height; ) {
                    *dst++ = *src;
                    src -= width;
                }       src += sgap, dst += dgap;
            }
        }       break;

        case SWAP_XY | FLIP_HY: {
            int w, h, dgap = bpl / bpp  - height,
                    sgap = width * height + 1;
            pixel_t *dst = (pixel_t*)base,
                    *src = (pixel_t*)pixl + width - 1;
            for (h=0; h++ < width; ) {
                for (w=0; w++ < height; ) {
                    *dst++ = *src;
                    src += width;
                }       src -= sgap, dst += dgap;
            }
        }       break;

        case FLIP_WX | FLIP_HY: {
            int w, h, dgap = bpl / bpp  - width;
            pixel_t *dst = (pixel_t*)base,
                    *src = (pixel_t*)pixl + width * height;
            for (h=0; h++ < height; ) {
                for (w=0; w++ < width; ) *dst++ = *(--src);
                dst += dgap;
            }
        }       break;

        case FLIP_HY: {
            int w, h, dgap = bpl / bpp  - width;
            pixel_t *dst = (pixel_t*)base,
                    *src = (pixel_t*)pixl - width +
                            width * height;
            for (h=0; h++ < height; ) {
                for (w=0; w++ < width; ) *dst++ = *src++;
                src -= width * 2;          dst += dgap;
            }
        }       break;

        case FLIP_WX: {
            int w, h, dgap = bpl / bpp  - width;
            pixel_t *dst = (pixel_t*)base,
                    *src = (pixel_t*)pixl + width;
            for (h=0; h++ < height; ) {
                for (w=0; w++ < width; ) *dst++ = *(--src);
                src += width * 2;         dst += dgap;
            }
        }       break;
        case SWAP_XY: {
            int w, h, dgap = bpl / bpp - height,
                    sgap = width * height - 1;
            pixel_t *dst = (pixel_t*)base, *src = (pixel_t*)pixl;
            for (h=0; h++ < width; ) {
                for (w=0; w++ < height; ) {
                    *dst++ = *src;
                    src += width;
                }       src -= sgap, dst += dgap;
            }
        }       break;

        default: fprintf(stderr, "Unhandled flag: %lx\n", flag);
        }
    }

    // XXX: static
    int fd;
    int size;
    int xres, yres, bpl;
    unsigned char *scrn, *base;

    unsigned long flag;
};

#endif//FB_HPP
// vim:sts=4:ts=8:
