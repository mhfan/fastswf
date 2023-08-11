/****************************************************************
 * $ID: zs.h           Fri, 31 Mar 2006 22:08:31 +0800  mhfan $ *
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
#ifndef ZS_H
#define ZS_H

#include <zlib.h>
#include <stdio.h>

#define ZS_DEFAULT_CHUNK_SIZE           8192

struct zstream {
    FILE* strm;
    long  poff;
#if 0
    struct buffer {
        unsigned char* base;
        long size, head, tail;
    }   obuf;
#endif
    struct {
        z_stream* zstr;
        unsigned char* chnk;
    };
    unsigned long flag;
#define ZS_EOF                          (0x01 << 7)
#define ZS_MODE_READ                    (0x01 << 0)
#define ZS_MODE_WRITE                   (0x01 << 1)
};

int zs_eof  (struct zstream* zs);
int zs_close(struct zstream* zs);
struct zstream* zs_init(FILE* fs, unsigned m);
struct zstream* zs_open(char* fn, char* m);
size_t zs_read (void* buf, size_t size, size_t memb, struct zstream* zs);
size_t zs_write(void* buf, size_t size, size_t memb, struct zstream* zs);
int zs_seeko(struct zstream* zs, off_t off, int wh);
int zs_seek (struct zstream* zs, long  off, int wh);
int zs_getpos(struct zstream* zs, fpos_t *pos);
int zs_setpos(struct zstream* zs, fpos_t *pos);
void zs_rewind(struct zstream* zs);
off_t zs_tello(struct zstream* zs);
long zs_tell(struct zstream* zs);
int zs_error(struct zstream* zs);
void zs_clearerr(struct zstream* zs);

#endif//ZS_H
// vim:sts=4:ts=8:
