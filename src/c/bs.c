/****************************************************************
 * $ID: bitstream.c    Thu, 30 Mar 2006 09:25:15 +0800  mhfan $ *
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

#include "bs.h"
#include "common.h"

struct bstream* bs_init(void* strm, unsigned m) {
    struct bstream* bs;
    bs = malloc(sizeof(*bs));           assert(bs);
    bs->strm = strm;                    bs->flag = m;
    bs_read_hook = fread;               return bs;
}

int bs_close(struct bstream* bs) {
    free(bs);
    return 0;
}

int bs_eof(struct bstream* bs) { return (bs->flag & BS_EOF); }

#if 0
// TODO: implement all the rest standard stream manipulate operation

struct bstream* bs_open(char* fn, char* m) {
dtrace;
    return 0;
}

long bs_read(void* buf, long size, long memb, void* strm) {
dtrace;
    return 0;
}

size_t bs_read (void* buf, size_t size, size_t memb, struct bstream* bs) {
dtrace;
    return 0;
}

size_t bs_write(void* buf, size_t size, size_t memb, struct bstream* bs) {
dtrace;
    return 0;
}

int bs_seeko(struct bstream* bs, off_t off, int wh) {
dtrace;
    return 0;
}

int bs_seek (struct bstream* bs, long  off, int wh) {
dtrace;
    return 0;
}

int bs_getpos(struct bstream* bs, fpos_t *pos) {
dtrace;
    return 0;
}

int bs_setpos(struct bstream* bs, fpos_t *pos) {
dtrace;
    return 0;
}

void bs_rewind(struct bstream* bs) {
dtrace;
}

off_t bs_tello(struct bstream* bs) {
dtrace;
    return 0;
}

long bs_tell(struct bstream* bs) {
dtrace;
    return 0;
}

int bs_error(struct bstream* bs) {
dtrace;
    return 0;
}

void bs_clearerr(struct bstream* bs) {
dtrace;
}

uint8_t  bs_peek_byte(struct bstream* bs) {
dtrace;
    return 0;
}

uint32_t bs_peek_bits(struct bstream* bs, unsigned nb) {
dtrace;
    return 0;
}
#endif

uint32_t bs_read_unb(struct bstream* bs, unsigned nb) {
    uint32_t bit, val=0x00;
    assert(bs && nb < 32);
    for (bit = (0x01 << (nb - 1)); bit; bit >>= 1) {
        if (!bs->curr.bmsk) {
             bs->curr.bmsk = (0x01 << 7);
             bs_read_byte(bs);
        }
        if ( bs->curr.bmsk & bs->curr.byte) val |= bit;
             bs->curr.bmsk >>= 1;
    }   return val;
}

 int32_t bs_read_snb(struct bstream* bs, unsigned nb) {
    int32_t val = bs_read_unb(bs, nb);
    return SIGN_EXTEND(val, nb);
}

void    bs_align_byte(struct bstream* bs) {
    assert(bs);                         bs->curr.bmsk = 0x00;
}

void     bs_read_byte(struct bstream* bs) {
    if (!bs_read_hook(&bs->curr.byte, 1, 1, bs->strm)) bs->flag |= BS_EOF;
}

uint32_t bs_read_bits(struct bstream* bs, unsigned nb) {
    return bs_read_unb(bs, nb);
}

uint8_t  bs_read_u8  (struct bstream* bs) {
    uint8_t val;
    assert(bs);
    bs_align_byte(bs);
    if (!bs_read_hook(&val, 1, 1, bs->strm)) bs->flag |= BS_EOF;
    return val;
}

 int8_t  bs_read_s8  (struct bstream* bs) { return bs_read_u8(bs); }

uint16_t bs_read_u16 (struct bstream* bs) {
    uint16_t val;
    assert(bs);
    bs_align_byte(bs);
    if (!bs_read_hook(&val, 2, 1, bs->strm)) bs->flag |= BS_EOF;
    return val;
}

 int16_t bs_read_s16 (struct bstream* bs) { return bs_read_u16(bs); }

uint32_t bs_read_u32 (struct bstream* bs) {
    uint32_t val;
    assert(bs);
    bs_align_byte(bs);
    if (!bs_read_hook(&val, 4, 1, bs->strm)) bs->flag |= BS_EOF;
    return val;
}

 int32_t bs_read_s32 (struct bstream* bs) { return bs_read_u32(bs); }

// vim:sts=4:ts=8:
