//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: type.h         Fri, 31 Mar 2006 00:04:14 +0800  mhfan $ *
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
#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

#include "bs.h"

typedef int8_t				f8_8_t;
typedef int16_t				f16_8_t;
typedef int16_t				f16_16_t;
typedef int32_t				f32_16_t;
typedef int32_t				f32_24_t;
typedef int32_t				f32_32_t;
typedef int64_t				f64_16_t;
typedef int64_t				f64_24_t;
typedef int64_t				f64_32_t;
typedef int64_t				f64_40_t;
typedef int64_t				f64_48_t;
typedef int64_t				f64_64_t;

typedef uint8_t				uf8_8_t;
typedef uint16_t			uf16_8_t;
typedef uint16_t			uf16_16_t;
typedef uint32_t			uf32_16_t;
typedef uint32_t			uf32_24_t;
typedef uint32_t			uf32_32_t;
typedef uint64_t			uf64_16_t;
typedef uint64_t			uf64_24_t;
typedef uint64_t			uf64_32_t;
typedef uint64_t			uf64_40_t;
typedef uint64_t			uf64_48_t;
typedef uint64_t			uf64_64_t;

#ifdef  OPTIMIZE_SPEED
#ifdef  OPTIMIZE_ACCURACY
#define	TWIPS_SHIFT 			4
#else
#define	TWIPS_SHIFT			5
#endif//OPTIMIZE_ACCURACY
#define	TWIPS  				(0x01 << TWIPS_SHIFT)
#define TWIPS2PIXELS(twips)             (((twips - 1) >> TWIPS_SHIFT) + 1)
#else //TWIP: Twentieth of a Point; 1/1440 inch, or 1/20 pixel
#define	TWIPS 				20
#define TWIPS2PIXELS(twips)             (((twips - 1) /  TWIPS) + 1)
#endif//OPTIMIZE_SPEED

typedef	int32_t				twip_t;

struct swf_point { union { struct { int32_t x, y; }; int32_t _[2]; }; };

struct /*__attribute__((packed)) */swf_rect {
#define	SWF_RECT_NBIT			5
    //uint8_t nbit : SWF_RECT_NBIT;
    union {
	struct /*__attribute__((packed)) */{
	    int32_t x0, x1, y0, y1;	// : nbit;	// TWIPS
	};  int32_t __[4];
	int32_t _[2][2];		// [[x0 x1]; [y0 y1]]
	//struct { struct swf_point tlp, brp; };
	//int32_t _[2][2];		// [[tlp.x tlp.y]; [brp.x brp.y]]
    };
};

struct /*__attribute__((packed)) */swf_matrix {
#define	SWF_MATRIX_SNBIT		5
#define	SWF_MATRIX_RNBIT		5
#define	SWF_MATRIX_TNBIT		5
    union {
	struct /*__attribute__((packed)) */{
	    // X' = (X * sx + Y * r1 - 1) / 65536 + 1 + tx
	    // Y' = (X * r0 + Y * sy - 1) / 65536 + 1 + ty
#define	SWF_MATRIX_DEFAULT_SCALE	(0x01 << 16)
#define	SWF_MATRIX_DEFAULT_ROTATE	0x000000
#define	SWF_MATRIX_DEFAULT_TRANSLATE	0x000000
	    //uint32_t has_scale : 1, snbit : SWF_MATRIX_SNBIT;
	    f32_16_t sx, sy;	 // : rnbit;
	    //uint32_t has_rotat : 1, rnbit : SWF_MATRIX_RNBIT;
	    f32_16_t r0, r1;	 // : snbit;
	    //uint32_t		      tnbit : SWF_MATRIX_TNBIT;
	    twip_t   tx, ty;	 // : tnbit;
	};  int32_t __[6];
	int32_t _[3][2];		// [[sx sy]; [r0 r1]; [tx ty]]
	    // X' = (X * xx + Y * xy - 1) / 65536 + 1 + x0
	    // Y' = (X * yx + Y * yy - 1) / 65536 + 1 + y0
	struct { int32_t xx, yy, yx, xy, x0, y0; };
	//int32_t _[2][3];		// [[xx xy x0]; [yx yy y0]]
    };
};

struct __attribute__((packed)) swf_rgb {
    union { struct { uint8_t	r, g, b; };	uint8_t _[3]; };
};

struct swf_xrgb {
    union { struct { uint8_t p, r, g, b; };	uint8_t _[4]; };
};

struct swf_rgba {
    union { struct { uint8_t    r, g, b, a; };	uint8_t _[4]; };
};

struct swf_argb {
    union { struct { uint8_t a, r, g, b; };	uint8_t _[4]; };
};

#if 0
struct swf_color {
    union {
	struct swf_rgb;
	struct swf_xrgb;
	struct swf_rgba;
	struct swf_argb;
    };
};
#endif

struct /*__attribute__((packed)) */swf_cxform {
#define	SWF_CXFORM_NBIT			4
    //uint8_t has_add : 1, has_mul : 1, nbit : 4;
    // C' = max(0, min((C * mc - 1) / 256 + 1 + ac, 255))
    // C' = clamp((C * mc - 1) / 256 + 1 + ac, 0, 255)
    union {
	struct /*__attribute__((packed)) */{
#define	SWF_CXFORM_DEFAULT_MUL		(0x01 << 8)
#define	SWF_CXFORM_DEFAULT_ADD		0x0000
	    f16_8_t mr, mg, mb, ma;
	    int16_t ar, ag, ab, aa;
	};  int16_t __[8];
	int16_t _[2][4];	// [[mr mg mb ma]; [ar ag ab aa]]
	//int16_t _[4][2];	// [[mr ar]; [mg ag]; [mb ab]; [ma aa]]
    };
};

void swf_read_rgb (struct bstream* bs, struct swf_rgb*  rgb);
void swf_read_rgba(struct bstream* bs, struct swf_rgba* rgba);
void swf_read_rect(struct bstream* bs, struct swf_rect* rect);
void swf_read_matrix(struct bstream* bs, struct swf_matrix* mat);
void swf_read_cxform(struct bstream* bs, struct swf_cxform* cxf);

#endif//TYPE_H
// vim:sts=4:ts=8:
