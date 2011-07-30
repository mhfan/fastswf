//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: bs.h           Thu, 30 Mar 2006 09:25:34 +0800  mhfan $ *
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
#ifndef BS_H
#define BS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef	SIGN_EXTEND
#define	SIGN_EXTEND(val, nb) \
	(val = (val & (1 << (nb - 1))) ? val | (-1 << nb) : val);
#endif//SIGN_EXTEND

struct bstream {
    void* strm;
    struct { uint8_t byte, bmsk; } curr;
    unsigned long flag;
#define	BS_EOF				(0x01 << 7)
#define	BS_MODE_READ			(0x01 << 0)
#define	BS_MODE_WRITE			(0x01 << 1);
};

struct bstream* bs_init(void* strm, unsigned m);
int bs_eof  (struct bstream* bs);
int bs_close(struct bstream* bs);
struct bstream* bs_open(char* fn, char* m);
//long bs_read(void* buf, long size, long memb, void* strm);
size_t bs_read (void* buf, size_t size, size_t memb, struct bstream* bs);
size_t bs_write(void* buf, size_t size, size_t memb, struct bstream* bs);
int bs_seeko(struct bstream* bs, off_t off, int wh);
int bs_seek (struct bstream* bs, long off, int wh);
int bs_getpos(struct bstream* bs, fpos_t *pos);
int bs_setpos(struct bstream* bs, fpos_t *pos);
void bs_rewind(struct bstream* bs);
off_t bs_tello(struct bstream* bs);
long bs_tell(struct bstream* bs);
int bs_error(struct bstream* bs);
void bs_clearerr(struct bstream* bs);


void    bs_align_byte(struct bstream* bs);
uint8_t  bs_peek_byte(struct bstream* bs);
void     bs_read_byte(struct bstream* bs);
uint32_t bs_peek_bits(struct bstream* bs, unsigned nb);
uint32_t bs_read_bits(struct bstream* bs, unsigned nb);
 int32_t bs_read_snb (struct bstream* bs, unsigned nb);
uint32_t bs_read_unb (struct bstream* bs, unsigned nb);
uint8_t  bs_read_u8  (struct bstream* bs);
 int8_t  bs_read_s8  (struct bstream* bs);
uint16_t bs_read_u16 (struct bstream* bs);
 int16_t bs_read_s16 (struct bstream* bs);
uint32_t bs_read_u32 (struct bstream* bs);
 int32_t bs_read_s32 (struct bstream* bs);

size_t (* bs_read_hook)(void *ptr, size_t size, size_t memb, void* strm);

#endif//BS_H
// vim:sts=4:ts=8:
