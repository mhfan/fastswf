//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: fb.h           Thu, 30 Mar 2006 22:57:43 +0800  mhfan $ *
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
#ifndef FB_H
#define FB_H

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

#define	DEFAULT_FBDEV_PATH		"/dev/fb0"
#define	pixel_t				uint16_t

struct Framebuffer {
    void* pixels;
    int width, height, ppl, bpp, depth;
    int size;

//private:
    int fd;
    int xres, yres, bpl;
    unsigned char *scr, *base;

#define	FB_SWAP_XY			(0x01 << 0)
#define	FB_FLIP_WX			(0x01 << 1)
#define	FB_FLIP_HY			(0x01 << 2)
#define	FB_CENTERX			(0x01 << 3)
#define	FB_CENTERY			(0x01 << 4)
#define	FB_CENTER_MASK			(FB_CENTERX | FB_CENTERY)
#define	FB_TRANSFORM_MASK		(FB_SWAP_XY | FB_FLIP_WX | FB_FLIP_HY)
    unsigned long flag;
};

void fbunin(struct Framebuffer* fb);
void fbrefr(struct Framebuffer* fb);
int  fbinit(struct Framebuffer* fb, char* path);
int  fbsetp(struct Framebuffer* fb, int x, int y, int w, int h, int s);

#endif//FB_H
// vim:sts=4:ts=8:
