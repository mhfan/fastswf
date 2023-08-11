/****************************************************************
 * $ID: shape.h        Thu, 30 Mar 2006 09:38:32 +0800  mhfan $ *
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
#ifndef SHAPE_H
#define SHAPE_H

#include "type.h"

/*
 * A shape is comprised of the following elements:
 *
 *  * CharacterId — A 16-bit value that uniquely identifies this shape as a
 *  ‘character’ in the dictionary. The CharacterId can be referred to in
 *  control tags such as PlaceObject. Characters can be re-used and combined
 *  with other characters to make more complex shapes.
 *  * Bounding box — The rectangle that completely encloses the shape.
 *  * Fill style array — A list of all the fill styles used in a shape.
 *  * Line style array — A list of all the line styles used in a shape.
 *  * Shape-record array — A list of shape-records. Shape-records can define
 *  straight or curved edges, style changes, or move the drawing position.
 *
 * Note: Line and fill styles are defined once only, and may be used (and
 * re-used) by any of the edges in the shape.
 */

struct swf_fill_style_solid {
    union {
        struct swf_rgb  rgb;            // for DefineShape/DefineShape2
        struct swf_rgba rgba;           // for DefineShape3
    };  struct swf_rgba rgba_morph;     // for DefineMorphShape
};

struct swf_gradient_record {
    f8_8_t pos;
    union {
        struct swf_rgb  rgb;            // for DefineShape/DefineShape2
        struct swf_rgba rgba;           // for DefineShape3
    };
    struct {                            // for DefineMorphShape
        f8_8_t pos_morph;
        struct swf_rgba rgba_morph;
    };
};


struct swf_fill_style_gradient {
    struct swf_matrix matrix;
    struct swf_matrix matrix_morph;     // for DefineMorphShape
    struct {
        uint8_t count;
        struct swf_gradient_record* records;
    };
};

struct swf_fill_style_bitmap {
    uint16_t refid;
    struct swf_matrix matrix;
    struct swf_matrix matrix_morph;     // for DefineMorphShape
};

struct swf_fill_style {
    uint8_t type;
#define SWF_FILL_STYLE_SOLID            0x00
#define SWF_FILL_STYLE_GRADIENT_LINEAR  0x10
#define SWF_FILL_STYLE_GRADIENT_RADIAL  0x12
#define SWF_FILL_STYLE_BITMAP_CLIPPED_S 0x41
#define SWF_FILL_STYLE_BITMAP_TILLED_H  0x42
#define SWF_FILL_STYLE_BITMAP_CLIPPED_H 0x43
    union {
        struct swf_fill_style_solid     solid;
        struct swf_fill_style_gradient  gradient;
        struct swf_fill_style_bitmap    bitmap;
    };
};

struct swf_line_style {
    uint16_t width;                     // TWIPS
    union {
        struct swf_rgb  rgb;            // for DefineShape/DefineShape2
        struct swf_rgba rgba;           // for DefineShape3
    };  struct swf_rgba rgba_morph;     // for DefineMorphShape
};

struct /*__attribute__((packed)) */swf_styles_nbit {
#define SWF_STYLE_FILL_NBIT             4
#define SWF_STYLE_LINE_NBIT             4
    uint8_t fill : SWF_STYLE_FILL_NBIT, line : SWF_STYLE_LINE_NBIT;
};

struct __attribute__((packed)) swf_shape_record {
    uint8_t type : 1;
    union {
        uint8_t _;
#define SWF_SHAPE_RECORD_END_NBIT       5
        uint8_t end : SWF_SHAPE_RECORD_END_NBIT;// == 0x00      // type == 0

        struct __attribute__((packed)) {
            uint8_t has_new_styles  : 1;// 0 when DefineShape
            uint8_t has_line_style  : 1;
            uint8_t has_fill_style1 : 1;
            uint8_t has_fill_style0 : 1;
            uint8_t has_moveto      : 1;
            struct {
#define SWF_SHAPE_RECORD_MOVETO_NBIT    5
                uint8_t nbit : SWF_SHAPE_RECORD_MOVETO_NBIT;
#define SWF_SHAPE_RECORD_MOVETO_XY      0x00
                int32_t x, y;           // : nbit, : nbit // TWIPS
                // NOTE: the first is relative to the current shape origin
                // subsequent's are relative to the current drawing postion
            }   moveto;                 // : snbit.fill, : snbit.line
            uint32_t fill_style0, fill_style1, line_style;
        }   setup;                                              // type == 0

        struct __attribute__((packed)) {    // type == 1
            uint8_t type : 1;
#define SWF_SHAPE_RECORD_EDGECOORD_NBIT 4
            uint8_t nbit : SWF_SHAPE_RECORD_EDGECOORD_NBIT;
            // XXX: nbit += 2;
            union {
                struct /*__attribute__((packed)) */{
                    int32_t cp_dx, cp_dy;   // : nbit, : nbit   // TWIPS
                    int32_t ap_dx, ap_dy;   // : nbit, : nbit   // TWIPS
                }   curve;              // type == 0
                struct /*__attribute__((packed)) */{
                    uint8_t has_xay : 1, has_xoy : 1;   // when has_xay == 0
#define SWF_SHAPE_RECORD_EDGECOORD_DEFAULT  0x00
                    int32_t dx, dy;         // : nbit, : nbit   // TWIPS
                }   sline;
            };
        }   edge;                                               // type == 1
    };
};

struct swf_shape_with_style {
    struct {
        struct {
            union {
                uint8_t  coun_;         // only for DefineShape
                uint16_t count;         // when coun_ == 0xff
            };
            struct swf_fill_style* styles;
        }   fill;
        struct {
            union {
                uint8_t  coun_;         // only for DefineShape
                uint16_t count;         // when coun_ == 0xff
            };
            struct swf_line_style* styles;
        }   line;
        struct swf_styles_nbit snbit;
    }   styles;
    struct swf_shape_record* records;
};

struct swf_defineshape {
    //struct swf_tag_hdr tag;           // 2, 22, 32, 46
    uint16_t id;
    struct swf_rect br;
    union {
        struct swf_shape_with_style shape;
        struct {
            struct swf_rect br;
            uint32_t offset;
            struct {
                struct swf_shape_with_style shape;
                // XXX: aligned by char
                struct swf_styles_nbit snbit;
                struct swf_shape_record* records;
            };
        }   morph;                      // for DefineMorphShape
    };
};

struct swf_shape {
    struct swf_styles_nbit snbit;
    struct swf_shape_record* records;
};

#endif//SHAPE_H
// vim:sts=4:ts=8:
