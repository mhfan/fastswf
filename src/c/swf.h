/****************************************************************
 * $ID: swf.h          Wed, 29 Mar 2006 20:10:00 +0800  mhfan $ *
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
#ifndef SWF_H
#define SWF_H

/*
 * Alexis' SWF Reference:
    http://sswf.sourceforge.net/SWFalexref.html
 *
 * Macromedia Flash File Format (SWF):
    http://www.half-serious.com/swf/format/index.html
 *
 * SWF File Reference:
    http://homepages.tig.com.au/~dkl/swf/SWFfilereference.html
    http://homepages.tig.com.au/~dkl/swf/SWFfileformat.html
 */

/*
 * Introduction:
 *
 *  The Macromedia Flash (SWF) (pronounced 'swiff' ) file format was designed
 *  to deliver vector graphics and animation over the Internet. The Macromedia
 *  Flash File Format (SWF) was designed as a very efficient delivery format
 *  and not as a format for exchanging graphics between graphics editors. It
 *  was designed to meet the following goals:
 *
 *  * On-screen Display—The format is primarily intended for on-screen
 *  display and so it supports anti-aliasing, fast rendering to a bitmap of
 *  any color format, animation and interactive buttons.
 *
 *  * Extensibility—The format is a tagged format, so the format can be
 *  evolved with new features while maintaining backward compatibility with
 *  older players.
 *
 *  * Network Delivery—The files can be delivered over a network with limited
 *  and unpredictable bandwidth. The files are compressed to be small and
 *  support incremental rendering through streaming. SWF is a binary format
 *  and is not human readable like HTML. Macromedia Flash (SWF) uses
 *  techniques such as bit-packing and structures with optional fields, to
 *  minimize file size.
 *
 *  * Simplicity—The format is simple so that the player is small and easily
 *  ported. Also, the player depends upon only a very limited set of operating
 *  system functionality.
 *
 *  * File Independence—Files can be displayed without any dependence on
 *  external resources such as fonts.
 *
 *  * Scalability—Different computers have different monitor resolutions and
 *  bit depths. Files work well on limited hardware, while taking advantage of
 *  more expensive hardware when it is available.
 *
 *  * Speed—The files are designed to be rendered at a high quality very
 *  quickly.
 */

/*
 * File Compression Strategy:
 *
 * Since SWF files are frequently delivered over a network connection, it is
 * important that they be as compact as possible. There are several techniques
 * that are used to accomplish this:
 *
 *  * Reuse—The structure of the character dictionary makes it very easy to
 *  reuse elements in a SWF file. For example, a shape, button, sound, font,
 *  or bitmap can be stored in a file once and referenced many times.
 *
 *  * Compression—The contents of tags are compressed. SWF supports a variety
 *  of compression techniques. Bitmaps can be compressed with JPEG or a
 *  PNG-like zlib compression. Sound is compressed with various levels of
 *  ADPCM compression. Shapes are compressed using a very efficient delta
 *  encoding scheme.
 *
 *  * Bit Packing—Whenever possible, numbers are packed into the smallest
 *  possible number of bits for a particular value. Coordinates are commonly
 *  stored using variable-sized bit fields, where a few bits are used to
 *  indicate how many bits are needed by subsequent values. See RECT for an
 *  example.
 *
 *  * Default values—Some structures like matrices and color transforms have
 *  common fields that are used more often than others. For example, for a
 *  matrix, the most common field is the translation field. Scaling and
 *  rotation are less common. Therefore if the scaling field is not present,
 *  it is assumed to be 100%. If the rotation field is not present, it is
 *  assumed that there is no rotation. This use of default values helps to
 *  minimize file sizes.
 *
 *  * Change Encoding—As a rule, SWF files just store the changes between
 *  states. This is reflected in shape data structures and in the
 *  place/move/remove model used by the display list.
 *
 *  * Shape Data Structure—The shape data structure uses a unique structure
 *  to minimize the size of shapes and to render anti-aliased shapes very
 *  efficiently on the screen.
 *
 */

/*
 * A Macromedia Flash (SWF) file is made up of a header, followed by a number
 * of tags. There are two types of tags, Definition Tags and Control Tags.
 * Definition Tags define the objects known as characters, which are stored in
 * the Dictionary. Control Tags manipulate characters, and control the flow of
 * the movie.
 */

#include "type.h"

struct __attribute__((packed)) swf_hdr {        // little-endian
#define SWF_MAGIC_SIGNATURE             'SWF'
#define SWF_COMPRESS_MAGIC_SIGNATURE    'SWC'   // after V6
    union {
        struct {
            uint32_t mgc : 24, ver : 8; // 1~8
        };  uint32_t __;        uint8_t _[4];
    };      uint32_t len;
    // the rest are compressed when 6 < version and magic == 'CWS'
};

struct __attribute__((packed)) swf_movie_hdr {
    struct swf_rect fs;
    uf16_8_t fr;
    uint16_t fc;
};

struct swf;

/*
 * Processing a Macromedia Flash (SWF) File:
 *
 * The player processes all the tags in a Macromedia Flash (SWF) file until a
 * 'ShowFrame' tag is encountered. At this point, the "display list" is copied
 * to the screen and the player is idle until it is time to process the next
 * frame. The contents of the first frame are the cumulative effect of
 * performing all the control tag operations before the first 'ShowFrame' tag.
 * The contents of the second frame are the cumulative effect of performing
 * all the control tag operations from the beginning of the file to the second
 * 'ShowFrame' tag, and so on.
 */

#include "bs.h"

#if 0
struct swf_character {
};

struct swf_dictionary {
};

struct swf_action {
};

struct swf_displist {
};
#endif

#include "tag.h"

#include "render.h"

struct swf {
    char* path;
    struct bstream* bs;

    struct swf_render render;
    struct swf_tagpsr tagpsr[SWF_TAG_MaxTagNumber];

    unsigned frames;
    struct swf_hdr hdr;
    struct swf_movie_hdr mh;
    struct swf_tag_hdr th;
    struct swf_rgb bgc;
};

struct swf* swf_open(char* path);
int swf_decode(struct swf* swf);
int swf_close (struct swf* swf);

#endif//SWF_H
// vim:sts=4:ts=8:
