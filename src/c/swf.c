/****************************************************************
 * $ID: swf.c          Wed, 29 Mar 2006 23:53:55 +0800  mhfan $ *
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

#include "zs.h"
#include "swf.h"
#include "utils.h"
#include "common.h"

int swf_decode_frame(struct swf* swf) {
    assert(swf && swf->bs);
    do {swf_read_taghdr(swf->bs, &swf->th);
        swf->tagpsr[swf->th.code].prsr(swf);
        if (swf->th.code == SWF_TAG_ShowFrame) return 1;
    } while (!(swf->bs->flag & BS_EOF) && swf->th.code);
    return 0;
}

struct swf* swf_open(char* path) {
    FILE* fs;
    struct swf* swf;
    union {
        struct { uint32_t mgc : 24, ver : 8; };
        uint32_t __;            uint8_t _[4];
    }   mv;

    assert(path);
    if (!(fs = fopen(path, "rb"))) {
        fprintf(stderr, "Fail to open `%s': %s\n"
                , path, strerror(errno));                       return NULL;
    } else compact_path(path, 52);      // XXX

    fread(&mv, 4/*sizeof(mv)*/, 1, fs);
    if (mv.mgc != SWF_MAGIC_SIGNATURE &&
        mv.mgc != SWF_COMPRESS_MAGIC_SIGNATURE
        /*|| mv.ver < 0x00 || 0x0f < mv.ver*/) {
        fprintf(stderr, "It's not a valid `%c%c%c': %s\n"
                , mv._[2], mv._[1], mv._[0], path);             return NULL;
    }
    swf = malloc(sizeof(*swf));         assert(swf);
    swf->hdr.__ = mv.__;                swf->path = path;
    fread(&swf->hdr.len, 4/*sizeof(swf->hdr.len)*/, 1, fs);

    fprintf(stdout, "%c%c%c%d with %8d bytes: %s\n", mv._[2]
            , mv._[1], mv._[0], mv.ver, swf->hdr.len, path);

    if (swf->hdr._[0] == 'C') {         // deal with compressed SWF
        swf->bs = bs_init(zs_init(fs, ZS_MODE_READ), BS_MODE_READ);
        bs_read_hook = zs_read;
    } else swf->bs = bs_init(fs, BS_MODE_READ);         // make a bit-stream
    assert(0 < swf->hdr.len && swf->bs);

    swf_read_rect(swf->bs, &swf->mh.fs);// read movie header
    swf->mh.fr = bs_read_u16(swf->bs);
    swf->mh.fc = bs_read_u16(swf->bs);

    fprintf(stdout, "Movie:  [%d, %d; %5d, %5d] twips;"
            " %4u/256=%2g fps; %6u frames\n"
            , swf->mh.fs.x0, swf->mh.fs.y0, swf->mh.fs.x1, swf->mh.fs.y1
            , swf->mh.fr,    swf->mh.fr/256.0            , swf->mh.fc);

    return swf;
}

int swf_close(struct swf* swf) {
    assert(swf && swf->bs);             bs_close(swf->bs);
    free(swf);                          return 0;
}

// vim:sts=4:ts=8:
