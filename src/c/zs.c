//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: zs.c           Fri, 31 Mar 2006 22:08:28 +0800  mhfan $ *
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

#include "common.h"
#include "zs.h"

int zs_eof(struct zstream* zs) { return (zs->flag & ZS_EOF); }

#if 0
void zerr(struct zstream* zs, int ret)
{   // report a zlib or i/o error
    fprintf(stderr, "ZS: ");
    switch (ret) {
    case Z_ERRNO:			assert(zs);
        if (ferror(zs->strm)) fprintf(stderr, "Error reading: %s\n"
		, strerror(errno));				break;
    case Z_STREAM_ERROR:
        fprintf(stderr, "Invalid compression level\n");
        break;
    case Z_DATA_ERROR:
        fprintf(stderr, "Invalid/incomplete deflate data\n");	break;
    case Z_MEM_ERROR:
        fprintf(stderr, "Out of memory\n");			break;
    case Z_VERSION_ERROR:
        fprintf(stderr, "zlib version mismatch!\n");		break;
    }
}
#endif

struct zstream* zs_init(FILE* fs, unsigned md)
{
    int ret;
    z_stream* zstr;
    struct zstream* zs=NULL;

    zstr = malloc(sizeof(*zstr));	assert(zstr);
    zstr->zfree	    = Z_NULL;		zstr->zalloc    = Z_NULL;
    zstr->opaque    = Z_NULL;		zstr->next_in   = Z_NULL;
    zstr->avail_in  = 0;
    if ((ret = inflateInit(zstr)) != Z_OK) {
	if (zstr->msg) fprintf(stderr, "ZS init: %s\n", zstr->msg);
	return zs;
    }

    zs = malloc(sizeof(*zs));		assert(zs);
    zs->chnk = malloc(ZS_DEFAULT_CHUNK_SIZE);
    zs->strm = fs;			zs->flag = md;
    zs->zstr = zstr;			assert(zs->chnk);

    return zs;
}

int zs_close(struct zstream* zs)
{
    free(zs->zstr);			free(zs->chnk);
    free(zs);				return 0;
}

size_t zs_read(void* buf, size_t size, size_t memb, struct zstream* zs)
{
    assert(buf && zs && zs->zstr && 0 < size && 0 < memb);
    zs->zstr-> next_out = buf;
    zs->zstr->avail_out = size * memb;
    while (zs->zstr->avail_out) {	int ret;
	if (!zs->zstr->avail_in && !(zs->zstr->avail_in =
		fread((zs->zstr->next_in = zs->chnk), 1
			, ZS_DEFAULT_CHUNK_SIZE, zs->strm))) {
	    zs->flag |= ZS_EOF;		break;
	}
	if ((ret = inflate(zs->zstr, Z_NO_FLUSH)) == Z_STREAM_END) break;
	assert(ret != Z_STREAM_ERROR);	//zerr(zs, ret);	// XXX
	//if (zs->zstr->msg) fprintf(stderr, "ZS: %s\n", zs->zstr->msg);
    }	return (memb -= (zs->zstr->avail_out + size - 1) / size);
}

#if 0
// TODO: implement all the rest standard stream manipulate operation

struct zstream* zs_open(char* fn, char* m)
{
dtrace;
    return NULL;
}

size_t zs_write(void* buf, size_t size, size_t memb, struct zstream* zs)
{
dtrace;
    return 0;
}

int zs_seeko(struct zstream* zs, off_t off, int wh)
{
dtrace;
    return 0;
}

int zs_seek(struct zstream* zs, long off, int wh)
{
dtrace;
    return 0;
}

int zs_getpos(struct zstream* zs, fpos_t *pos)
{
dtrace;
    return 0;
}

int zs_setpos(struct zstream* zs, fpos_t *pos)
{
dtrace;
    return 0;
}

void zs_rewind(struct zstream* zs)
{
dtrace;
}

off_t zs_tello(struct zstream* zs)
{
dtrace;
    return 0;
}

long zs_tell(struct zstream* zs)
{
dtrace;
    return 0;
}

int zs_error(struct zstream* zs)
{
dtrace;
    return 0;
}

void zs_clearerr(struct zstream* zs)
{
dtrace;
}
#endif

// vim:sts=4:ts=8:
