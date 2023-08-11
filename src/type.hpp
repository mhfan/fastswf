//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: type.hpp       Wed, 05 Apr 2006 14:25:54 +0800  mhfan $ *
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
#ifndef TYPE_HPP
#define TYPE_HPP

#include <iomanip>
#include <fstream>

#include <string>
#include <vector>

#include "bs.hpp"
//#include "bswap.h"
#include "common.h"
#include "fixed.hpp"
#include "action.hpp"

namespace SWF {

/* Coordinates and twips:
 *
 * The SWF file format stores all x-y coordinates as integers, usually in a
 * unit of measurement called a twip. In the SWF format, a twip is 1/20th of
 * a logical pixel. A logical pixel is the same as a screen pixel when the
 * file is played at 100% - that is, without scaling.
 *
 * Coordinates in the SWF file format use the traditional graphics axes: x is
 * horizontal and proceeds from minimum values at the left to maximum values
 * at the right, and y is vertical and proceeds from minimum values at the top
 * to maximum values at the bottom.
 */

#ifdef	OPTIMIZE_SPEED

#ifdef	OPTIMIZE_ACCURACY
const uint8_t TWIPS_SHIFT = 4u;
#else
const uint8_t TWIPS_SHIFT = 5u;
#endif
const uint8_t TWIPS = (0x01 <<  TWIPS_SHIFT);

static inline int32_t twips2pixels(int32_t twips) {
    return  ((twips + (0x01 << (TWIPS_SHIFT - 1))) >> TWIPS_SHIFT);
}

#define	TWIPS2PIXELS(twips) \
    (twips + (0x01 << (TWIPS_SHIFT - 1)) >> TWIPS_SHIFT)

static inline int32_t pixels2twips(int32_t pixels) {
    return (pixels << TWIPS_SHIFT);
}

#define	PIXELS2TWIPS(pixels)		(pixels << TWIPS_SHIFT)

#else// TWIP: Twentieth of a Point; 1/1440 inch, or 1/20 pixel

const uint8_t TWIPS = 20u;

static inline int32_t twips2pixels(int32_t twips) {
    return ((twips + (TWIPS >> 1)) / TWIPS);
}

#define	TWIPS2PIXELS(twips)		((twips + (TWIPS >> 1)) / TWIPS)

static inline int32_t pixels2twips(int32_t pixels) {
    return (pixels * TWIPS);
}

#define	PIXELS2TWIPS(pixels)		(pixels * TWIPS)

#endif//OPTIMIZE_SPEED

#if 0
static inline float twips2pixels(int32_t twips) {
    return  ((float)twips / TWIPS);
}
#endif

#if 0
struct Pixel {
    Pixel(int32_t val): pixels(val) { }
    int32_t topixels() { return (pixels * TWIPS); }
    operator Twip() { return totwips(); }
    operator int32_t() { return pixels; }

private:
    int32_t pixels;
};

struct Twip {
    Twip(int32_t val): twips(val) { }
    float   topixels() { return (float)twips / TWIPS; }
    int32_t topixels() { return ((twips + (TWIPS >> 1)) / TWIPS); }
    operator Pixel() { return topixels(); }
    operator int32_t() { return twips; }

private:
    int32_t twips;
};

struct Point {
    union {
	struct { Twip x, y; };
	int32_t _[2];
    };  Point(int32_t x_, int32_t y_): x(x_), y(y_) { }
};
#else
typedef int32_t Twip;
#endif

/* Integer types and byte order:
 *
 * The SWF file format uses 8-bit, 16-bit, 32-bit, 64-bit, signed, and
 * unsigned integer types. All integer values are stored in the SWF file by
 * using little-endian byte order: the least significant byte is stored first,
 * and the most significant byte is stored last, in the same way as the Intel
 * x86 architecture. The bit order within bytes in the SWF file format is
 * big-endian: the most significant bit is stored first, and the least
 * significant bit is stored last.
 *
 * All integer types must be byte-aligned. That is, the first bit of an
 * integer value must be stored in the first bit of a byte in the SWF file.
 * Signed integers are represented by using traditional 2's-complement bit
 * patterns. These are the signed integer representations used on most modern
 * computer platforms. In the 2's complement system, negative numbers have 1
 * as the first bit, and zero and positive numbers have 0 as the first bit.
 * A negative number, -n, is represented as the bitwise opposite of the
 * positive-zero number n-1.
 */

typedef  int8_t  SI8;
typedef  int16_t SI16;
typedef  int32_t SI32;
typedef uint8_t  UI8;
typedef uint16_t UI16;
typedef uint32_t UI24;	// XXX:
typedef uint32_t UI32;
typedef uint64_t UI64;

/* Fixed-point numbers:
 *
 * The SWF file format supports two types of fixed-point numbers: 32-bit and
 * 16-bit. The 32-bit fixed-point numbers are 16.16. That is, the high 16 bits
 * represent the number FIXED values are stored like 32-bit integers in the
 * SWF file (using little-endian byte order) and must be byte aligned.
 *
 * SWF 8 and later supports 16-bit 8.8 signed, fixed-point numbers. The high 8
 * bits represent the number before the decimal point, and the low 8 bits
 * represent the number after the decimal point. FIXED8 values are stored like
 * 16-bit integers in the SWF file (using littleendian byte order) and must be
 * byte aligned.
 */

typedef f32_16_t FIXED;
typedef f16_8_t  FIXED8;

/* Floating-point numbers:
 *
 * SWF 8 and later supports the use of IEEE Standard 754 compatible
 * floating-point types. Three types of floating-point numbers are supported.
 */

//typedef double  DOUBLE;
struct DOUBLE {
    union {	  double d;
	struct { uint64_t mantissa:52, exponent:11, sign:1; };
    };
};

//typedef float   FLOAT;
struct FLOAT {
    union {	  float f;
	struct { uint32_t mantissa:23, exponent:8, sign:1; };
    };
};

struct FLOAT16 {
    /* FLOAT16 is identical to the characteristics of FLOAT except for changes
     * to the number of bits allocated to the exponent and mantissa:
     *  -  1 bit  for the sign
     *  -  5 bits for the exponent, with an exponent bias of 16
     *  - 10 bits for the mantissa
     *
     * FLOAT:
     *  -  1 bit  for the sign
     *  -  8 bits for the exponent
     *  - 23 bits for the mantissa
     *
     * DOUBLE:
     *  -  1 bit  for the sign
     *  - 11 bits for the exponent
     *  - 52 bits for the mantissa
     */
    union {	  int16_t _;	float f;
	struct { uint16_t mantissa:10, exponent:5, sign:1; };
    };

    BitStream& load(BitStream& bs) {
	return bs;
    }

    operator float() const { return f; }
    // TODO: convert to float?
};

/* Encoded integers:
 *
 * SWF 9 and later supports the use of integers encoded with a variable number
 * of bytes. One type of encoded integer is supported.
 *
 * This is a 32-bit unsigned integer value encoded with a variable number of
 * bytes to save space. All EncodedU32's are encoded as 1-5 bytes depending on
 * the value (larger values need more space). The encoding method is if the hi
 * bit in the current byte is set, then the next byte is also part of the
 * value. Each bit in a byte contributes 7 bits to the value, with the hi bit
 * telling us whether to use the next byte, or if this is the last byte for
 * the value.
 */

struct EncodedU32 {
    UI32 v;

    BitStream& load(BitStream& bs) {
	UI8 b;	      bs >> b, v  = b;
	if (v & (0x01 <<  7)) bs >> b, v |= b <<  7; else return bs;
	if (v & (0x01 << 14)) bs >> b, v |= b << 14; else return bs;
	if (v & (0x01 << 21)) bs >> b, v |= b << 21; else return bs;
	if (v & (0x01 << 28)) bs >> b, v |= b << 28;	  return bs;
    }

    friend BitStream& operator>>(BitStream& bs, EncodedU32& eu32)
	    { return eu32.load(bs); }
};

/* Bit values:
 *
 * Bit values are variable-length bit fields that can represent three types of
 * numbers:
 *  1. Unsigned integers
 *  2. Signed integers
 *  3. Signed 16.16 fixed-point values.
 *
 * Bit values do not have to be byte aligned. Other types (such as UI8 and
 * UI16) are always byte aligned. If a byte-aligned type follows a bit value,
 * the last byte that contains the bit value is padded with zeros.
 *
 * This expansion is called sign extension.
 */

/* String values:
 *
 * A string value represents a null-terminated character string. The format
 * for a string value is a sequential list of bytes terminated by the null
 * character byte.
 *
 * In SWF 5 or earlier, STRING values are encoded using either ANSI (which is
 * a superset of ASCII) or shift-JIS (a Japanese encoding). You cannot
 * indicate the encoding that is used; instead, the decoding choice is made
 * according to the locale in which Flash Player is running. This means that
 * text content in SWF 5 or earlier can only be encoded in ANSI or shift-JIS,
 * and the target audience must be known during authoring—otherwise garbled
 * text results.
 *
 * In SWF 6 or later, STRING values are always encoded by using the Unicode
 * UTF-8 standard. This is a multibyte encoding; each character is composed of
 * between one and four bytes. UTF-8 is a superset of ASCII; the byte range 0
 * to 127 in UTF-8 exactly matches the ASCII mapping, and all ASCII characters
 * 0 to 127 are represented by just one byte. UTF-8 guarantees that whenever a
 * character other than character 0 (the null character) is encoded by using
 * more than one byte, none of those bytes are zero. This avoids the
 * appearance of internal null characters in UTF-8 strings, meaning that it
 * remains safe to treat null bytes as string terminators, just as for ASCII
 * strings.
 */

typedef char* STRING;

/* Language code:
 *
 * A language code identifies a spoken language that applies to text. Language
 * codes are associated with font specifications in the SWF file format.
 *
 * Flash Player uses language codes to determine line-breaking rules for
 * dynamic text, and to choose backup fonts when a specified device font is
 * unavailable. Other uses for language codes may be found in the future.
 * A language code of zero means no language. This code results in behavior
 * that is dependent on the locale in which Flash Player is running.
 */

enum LanguageCode {
    Latin = 1,
    /* the western languages covered by Latin-1:
     *	English, French, German, and so on.
     */
    Japanese, Korean,
    SimplifiedChinese,
    TraditionalChinese,
};

struct Rect {
    /* Rectangle record:
     *
     * A rectangle value represents a rectangular region defined by a minimum
     * x- and y-coordinate position and a maximum x- and y-coordinate position.
     * The RECT record must be byte aligned.
     */

    enum { NBITS = 5u, };

    //uint8_t nbits:NBITS;	// Nbits
    union {
	struct {
	    Twip x0, x1, y0, y1;	//:nbits;
	};  int32_t __[4];
	    int32_t _[2][2];	// [[x0 x1], [y0 y1]]
	//struct { struct Point tlp, brp; };
	//struct { Twip Xmin, Xmax, Ymin, Ymax; };
	//int32_t _[2][2];		// [[tlp.x tlp.y], [brp.x brp.y]]
    };

    //Rect(Rect& r): x0(r.x0), x1(r.x1), y0(r.y0), y1(r.y1) {}

    //Point tlp() const { return Point(x0, y0); }
    //Point brp() const { return Point(x1, y1); }
    Twip width () const { return abs(x1 - x0); }
    Twip height() const { return abs(y1 - y0); }

    BitStream& load(BitStream& bs) {
	uint8_t nbits = NBITS;		bs.align();
	nbits = bs.read(nbits, UNSIGN);
	__[0] = bs.read(nbits, SIGNED);
	__[1] = bs.read(nbits, SIGNED);
	__[2] = bs.read(nbits, SIGNED);
	__[3] = bs.read(nbits, SIGNED);	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, Rect& r)
	    { return r.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, const Rect& r) {
	return (os  << "[" << r.x0 << " " << r.y0 << "; "
		    << r.x1 << " " << r.y1 << "]");
    }
};

struct Matrix {
    /* MATRIX record:
     *
     * The MATRIX record represents a standard 2x3 transformation matrix of
     * the sort commonly used in 2D graphics. It is used to describe the
     * scale, rotation, and translation of a graphic object. The MATRIX record
     * must be byte aligned.
     */

    enum { SNBIT = 5u, RNBIT = 5u, TNBIT = 5u, };

    struct {
	// X' = (X * xx + Y * xy + 32768) / 65536 + x0
	// Y' = (X * yx + Y * yy + 32768) / 65536 + y0

	// X' = (X * sx + Y * r1 + 32768) / 65536 + tx
	// Y' = (X * r0 + Y * sy + 32768) / 65536 + ty

	union {	int32_t __[4];	//int32_t _[2][2];
	    struct {
		//uint32_t has_scale:1,	snbit:SNBIT;
		FIXED sx, sy;	   //:snbit;
		//uint32_t has_rotat:1,	rnbit:RNBIT;
		FIXED r0, r1;	   //:rnbit;
	    };

	    struct { FIXED xx, yy, yx, xy; };
#ifdef	CTM_FLOAT_POINT
	    struct { float    xx, yy, yx, xy; } f; float _f[4];
#endif
	};

	union {	//int32_t ___[2];
		//uint32_t		tnbit:TNBIT;
	    struct { Twip tx, ty; };	   //:tnbit;
	    struct { Twip x0, y0; };
	};
    };

    //Matrix(Matrix& m): sx(m.sx), sy(m.sy), r0(m.r0),
    //		    r1(m.r1), tx(m.tx), ty(m.ty) { }

#ifdef	FIXED_POINT
    Matrix& concatenate(Matrix& m) {
	f64_32_t sx_ = sx, sy_ = sy, r0_ = r0, r1_ = r1;
	sx = ((sx_ * m.sx + r0_ * m.r1 + 0x8000) >> 16);
	r1 = ((r1_ * m.sx + sy_ * m.r1 + 0x8000) >> 16);
	r0 = ((r0_ * m.sy + sx_ * m.r0 + 0x8000) >> 16);
	sy = ((sy_ * m.sy + r1_ * m.r0 + 0x8000) >> 16);
	m.transform(tx, ty);		return *this;
    }

    Matrix& invert() {
	FIXED det = ((f64_32_t)sx * sy -
		     (f64_32_t)r0 * r1 + 0x8000) >> 16;
	if (det == 0) sx = sy = r0 = r1 = ~0ul; else {	// XXX:
	    f64_32_t sx_ = sx, sy_ = sy, r0_ = r0, r1_ = r1;
	    sx =  ((r1_ << 16) + (det >> 1)) / det;
	    sy = -((sy_ << 16) + (det >> 1)) / det;
	    r0 = -((r0_ << 16) + (det >> 1)) / det;
	    r1 =  ((sx_ << 16) + (det >> 1)) / det;
	}				return *this;
    }

    Matrix& operator*(Matrix& m) {	return concatenate(m); }

    //Matrix& transform(Point& p) {	return transform(p.x, p.y); }
    Matrix& transform(int32_t& x, int32_t& y) {
	f64_32_t x_ = x, y_ = y;
	x = ((x_ * sx + y_ * r1 + 0x8000) >> 16) + tx;
	y = ((x_ * r0 + y_ * sy + 0x8000) >> 16) + ty;	return *this;
    }
#endif

    BitStream& load(BitStream& bs) {	uint8_t nbit;	bs.align();
	if ((nbit = bs.read(1, UNSIGN)) && (nbit = bs.read(SNBIT, UNSIGN))) {
	       sx = bs.read(nbit, SIGNED);    sy = bs.read(nbit, SIGNED);
	} else sx = sy = DEFAULT_SCALE;
	if ((nbit = bs.read(1, UNSIGN)) && (nbit = bs.read(RNBIT, UNSIGN))) {
	       r0 = bs.read(nbit, SIGNED);    r1 = bs.read(nbit, SIGNED);
	} else r0 = r1 = DEFAULT_ROTATE;
	if ((nbit = bs.read(TNBIT, UNSIGN))) {
	       tx = bs.read(nbit, SIGNED);    ty = bs.read(nbit, SIGNED);
	} else tx = ty = DEFAULT_TRANSLATE;
#ifdef	CTM_FLOAT_POINT
	_f[3] = __[3] / 65535.f, _f[2] = __[2] / 65535.f,
	_f[1] = __[1] / 65535.f, _f[0] = __[0] / 65535.f;
#endif
	return bs;
    }

    static const FIXED	 DEFAULT_SCALE     = (0x01 << 16);
    static const FIXED	 DEFAULT_ROTATE    = 0x000000;
    static const int32_t DEFAULT_TRANSLATE = 0x000000;

    friend BitStream& operator>>(BitStream& bs, Matrix& mtx)
	    { return mtx.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
	return (os  << "[" << m.sx << " " << m.sy << "; "
			   << m.r0 << " " << m.r1 << "; "
			   << m.tx << " " << m.ty << "]");
    }
};

struct __attribute__((packed)) RGB {
    // The RGB record represents a color as a 24-bit red, green, and blue value.
    union {	 uint8_t _[3];
#if	__BYTE_ORDER == __BIG_ENDIAN
	struct { uint8_t    b, g, r; };
#else// XXX:
	struct { uint8_t    r, g, b; };
#endif
#ifdef	NORMALIZED_COLOR
	struct { float      r, g, b; } f;	float _f[3];
#endif
    };

#if 0
    RGB() { }
    RGB(uint32_t c) {
	b = (c & 0xff);
	g = ((c >>  8) & 0xff);
	r = ((c >> 16) & 0xff);
    }
    RGB(RGB& c): r(c.r), g(c.g), b(c.b) { }
#endif

    BitStream& load(BitStream& bs) {
	//return (bs >> r >> g >> b);
	bs.read((char*)_, 3);
#ifdef	NORMALIZED_COLOR
	_f[2] = _[2] / 255.f, _f[1] = _[1] / 255.f, _f[0] = _[0] / 255.f;
#endif
	return bs;
    }

    operator uint32_t() {
	return ((0xff << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
    }

    friend BitStream& operator>>(BitStream& bs, RGB& rgb)
	    { return rgb.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, const RGB& c) {
	return (os  << "#" << std::setw(8) << std::setfill('0') << std::hex
		    << (((uint32_t)c.r << 24) | ((uint32_t)c.g << 16) |
			((uint32_t)c.b <<  8) | 0xff) << std::setfill(' ')
		    << std::dec << " [" << (int)c.r << " "
		    << (int)c.g << " "  << (int)c.b << "]");
    }
};

struct RGBA {
    union {	 uint8_t _[4];	uint32_t __;
#if	__BYTE_ORDER == __BIG_ENDIAN
	struct { uint8_t    a, b, g, r; };
#else// XXX:
	struct { uint8_t    r, g, b, a; };
#endif
#ifdef	NORMALIZED_COLOR
	struct { float      r, g, b, a; } f;	float _f[4];
#endif
    };

#if 0
    RGBA(RGB&  c): r(c.r), g(c.g), b(c.b), a(0xff) { }
    RGBA(RGBA& c): r(c.r), g(c.g), b(c.b), a(c.a ) { }
    RGBA(XRGB& c): r(c.r), g(c.g), b(c.b), a(0xff) { }
    RGBA(ARGB& c) { r = c.r, g = c.g, b = c.b, a = c.a ; }	// XXX:
#endif

    BitStream& load(BitStream& bs) {
	//return (bs >> r >> g >> b >> a);
	bs.read((char*)_, 4);
#ifdef	NORMALIZED_COLOR
	_f[3] = _[3] / 255.f, _f[2] = _[2] / 255.f,
	_f[1] = _[1] / 255.f, _f[0] = _[0] / 255.f;
#endif
	return bs;
    }

    operator uint32_t() { return __; }

    friend BitStream& operator>>(BitStream& bs, RGBA& rgba)
	    { return rgba.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, const RGBA& c) {
	return (os  << "#"  << std::hex << std::setfill('0') << std::setw(8)
		    << c.__ << std::dec << std::setfill(' ') << " ["
		    << (int)c.r << " " << (int)c.g << " "
		    << (int)c.b << " " << (int)c.a << "]");
    }
};

struct ARGB {
    union {	 uint8_t _[4];	uint32_t __;
#if	__BYTE_ORDER == __BIG_ENDIAN
	struct { uint8_t    b, g, r, a; };
#else// XXX:
	struct { uint8_t a, r, g, b; };
#endif
#ifdef	NORMALIZED_COLOR
	struct { float   a, r, g, b; } f;	float _f[4];
#endif
    };

    //ARGB(RGB&  c): a(0xff), r(c.r), g(c.g), b(c.b) { }
    //ARGB(RGBA& c) { a = c.a , r = c.r, g = c.g, b = c.b; }	// XXX:

    BitStream& load(BitStream& bs) {
	//return (bs >> a >> r >> g >> b);
	bs.read((char*)_, 4);
#ifdef	NORMALIZED_COLOR
	_f[3] = _[3] / 255.f, _f[2] = _[2] / 255.f,
	_f[1] = _[1] / 255.f, _f[0] = _[0] / 255.f;
#endif
	return bs;
    }

    operator uint32_t() { return __; }

    friend BitStream& operator>>(BitStream& bs, ARGB& argb)
	    { return argb.load(bs); }
};

struct XRGB {
    union {	 uint8_t _[4];	uint32_t __;
#if	__BYTE_ORDER == __BIG_ENDIAN
	struct { uint8_t b, g, r, x; };
#else// XXX:
	struct { uint8_t x, r, g, b; };
#endif
#ifdef	NORMALIZED_COLOR
	struct { float   x, r, g, b; } f;	float _f[4];
#endif
    };

    BitStream& load(BitStream& bs) {
	//bs >> x;    x = 0xff;		return (bs >> r >> g >> b);
	bs.read((char*)_, 4);		x = 0xff;
#ifdef	NORMALIZED_COLOR
	_f[3] = _[3] / 255.f, _f[2] = _[2] / 255.f,
	_f[1] = _[1] / 255.f, _f[0] = _[0] / 255.f;
#endif
	return bs;
    }

    operator uint32_t() { return __; }

    friend BitStream& operator>>(BitStream& bs, XRGB& xrgb)
	    { return xrgb.load(bs); }
};

struct CXForm {
    enum { NBIT = 4u, };

    struct {	//uint8_t has_add:1, has_mul:1, nbit:NBIT;
	union {	     int16_t _[4];
	    struct { FIXED8 mr, mg, mb, ma; };
#if 0//def	FLOAT_POINT
	    struct { float mr, mg, mb, ma; } f; float _f[4];
#endif
	};
	union {	     int8_t __[4];
	    struct { int8_t  ar, ag, ab, aa; };	// XXX: int16_t
#ifdef	NORMALIZED_COLOR
	    struct { float ar, ag, ab, aa; } f_; float __f[4];
#endif
	};
    };

    CXForm(): ma(DEFAULT_MUL), aa(DEFAULT_ADD) { }
    //CXForm(int): ma(SHRT_MAX), aa(CHAR_MAX) { }

    // C' = max(0, min((C * mc + 128) / 256 + ac, 255))
    // C' = clamp((C * mc + 128) / 256 + ac, 0, 255)
    CXForm& transform(RGB& c) {
#ifdef	NORMALIZED_COLOR
	c.f.r = std::max(0.f, std::min(((c.f.r * mr) / 255.f) + f_.ar, 1.f));
	c.f.g = std::max(0.f, std::min(((c.f.g * mg) / 255.f) + f_.ag, 1.f));
	c.f.b = std::max(0.f, std::min(((c.f.b * mb) / 255.f) + f_.ab, 1.f));
#else
	c.r = std::max(0, std::min(((c.r * mr + 0x80) >> 8) + ar, 0xff));
	c.g = std::max(0, std::min(((c.g * mg + 0x80) >> 8) + ag, 0xff));
	c.b = std::max(0, std::min(((c.b * mb + 0x80) >> 8) + ab, 0xff));
#endif
	return *this;
    }

    CXForm& transform(RGBA& c) {
#ifdef	NORMALIZED_COLOR
	c.f.r = std::max(0, std::min(((c.f.r * mr) / 255.f) + f_.ar, 1.f));
	c.f.g = std::max(0, std::min(((c.f.g * mg) / 255.f) + f_.ag, 1.f));
	c.f.b = std::max(0, std::min(((c.f.b * mb) / 255.f) + f_.ab, 1.f));
	c.f.a = std::max(0, std::min(((c.f.a * mb) / 255.f) + f_.aa, 1.f));
#else
	c.r = std::max(0, std::min(((c.r * mr + 0x80) >> 8) + ar, 0xff));
	c.g = std::max(0, std::min(((c.g * mg + 0x80) >> 8) + ag, 0xff));
	c.b = std::max(0, std::min(((c.b * mb + 0x80) >> 8) + ab, 0xff));
	c.a = std::max(0, std::min(((c.a * ma + 0x80) >> 8) + aa, 0xff));
#endif
	return *this;
    }

    BitStream& load(BitStream& bs) {	bs.align();
#if 0
	union {	     uint8_t _;
	    struct { uint8_t hasa:1, hasm:1, nbit:NBIT; };
	};
#else
	uint8_t nbit;
#endif
	ar   = bs.read(1, UNSIGN);
	mr   = bs.read(1, UNSIGN);
	nbit = bs.read(NBIT, UNSIGN);

	if (mr) {
	    mr = bs.read(nbit, SIGNED);
	    mg = bs.read(nbit, SIGNED);
	    mb = bs.read(nbit, SIGNED);
	    if (ma == SHRT_MAX) ma = bs.read(nbit, UNSIGN);
	}   else mr = mg = mb = ma = DEFAULT_MUL;

#if 0//def	FLOAT_POINT
	_f[3] = _[3] / 255.f, _f[2] = _[2] / 255.f,
	_f[1] = _[1] / 255.f, _f[0] = _[0] / 255.f;
#endif

	if (ar) {
	    ar = bs.read(nbit, SIGNED);
	    ag = bs.read(nbit, SIGNED);
	    ab = bs.read(nbit, SIGNED);
	    if (aa == CHAR_MAX) aa = bs.read(nbit, UNSIGN);
	}   else ar = ag = ab = aa = DEFAULT_ADD;
	
#ifdef	NORMALIZED_COLOR
	__f[3] = __[3] / 255.f, __f[2] = _[2] / 255.f,
	__f[1] = __[1] / 255.f, __f[0] = _[0] / 255.f;
#endif

	return bs;
    }

    static const int16_t DEFAULT_MUL = (0x01 << 8);
    static const int16_t DEFAULT_ADD =  0x0000;

    friend BitStream& operator>>(BitStream& bs, CXForm& cxf)
	    { return cxf.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, const CXForm& f) {
	return (os  << "[" << f.mr << " "  << f.mg << " " << f.mb
		    << " " << f.ma << "; " << (int)f.ar << " " << (int)f.ag
		    << " " << (int)f.ab << " "  << (int)f.aa << "]");
    }
};

struct FillStyle {
    enum {
	SOLID = 0x00, GRADIENT_LINEAR = 0x10,
	GRADIENT_RADIAL = 0x12, GRADIENT_RADIAL_FOCAL,
	BITMAP_TILLED_SMOOTH = 0x40, BITMAP_CLIPPED_SMOOTH,
	BITMAP_TILLED_HARD, BITMAP_CLIPPED_HARD,
	BITMAP_TILLED = 0x40, BITMAP_CLIPPED,

	Solid = 0x00, LinearGradient = 0x10,
	RadialGradient = 0x12, FocalRadialGradient,
	RepeatingBitmap = 0x40, ClippedBitmap,
	HardRepeatingBitmap, HardClippedBitmap,
	TilledBitmap = 0x40, HardTilledBitmap = 0x42,
    };

    uint8_t type;

    FillStyle() { }
    FillStyle(uint8_t t): type(t) { }

    virtual ~FillStyle() { }

    friend BitStream& operator>>(BitStream& bs, FillStyle& fs);
    friend std::ostream& operator<<(std::ostream& os, FillStyle& fs);
};

struct FillStyleSolid: public FillStyle {	RGBA rgba;
    FillStyleSolid(uint8_t t, BitStream& bs, bool hasa): FillStyle(t) {
	rgba.a = (hasa ? 0x00 : 0xff);		load(bs);
    }

    BitStream& load(BitStream& bs) {
	return (rgba.a ? (bs >> rgba.r >> rgba.g >> rgba.b) : (bs >> rgba));
    }

    friend BitStream& operator>>(BitStream& bs, FillStyleSolid& fss)
	    { return fss.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, FillStyleSolid& fss) {
	return (os  << "  Solid: " << fss.rgba << std::endl);
    }
};

struct FillStyleSolidMorph: public FillStyle {
    RGBA rgba0, rgba1;

    FillStyleSolidMorph(uint8_t t, BitStream& bs): FillStyle(t) { load(bs); }

    BitStream& load(BitStream& bs) { return (bs >> rgba0 >> rgba1); }

    friend BitStream& operator>>(BitStream& bs,
	    FillStyleSolidMorph& fss) { return fss.load(bs); }
    friend std::ostream& operator<<(std::ostream& os,
	    FillStyleSolidMorph& fss) {
	return (os  << "  Solid morph: " << fss.rgba0
		    << " --> " << fss.rgba1 << std::endl);
    }
};


struct GradientRecord {
#ifdef	NORMALIZED_COLOR//CTM_FLOAT_POINT
    union {
	uint8_t pos;
	float fpos;
    };
#else
    uint8_t pos;
#endif
    RGBA rgba;

    GradientRecord() { rgba.a = 0x00; }		// XXX:

    BitStream& load(BitStream& bs) {	bs >> pos;
#ifdef	NORMALIZED_COLOR//CTM_FLOAT_POINT
	fpos = pos / 255.f;	
#endif
	return (rgba.a ? (bs >> rgba.r >> rgba.g >> rgba.b) : (bs >> rgba));
    }

    friend BitStream& operator>>(BitStream& bs, GradientRecord& gr)
	    { return gr.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, GradientRecord& gr) {
#ifndef	NORMALIZED_COLOR//CTM_FLOAT_POINT
	return (os  << "\t    " << std::setw(4) << (int)gr.pos
		    << ": " << gr.rgba << std::endl);
#else
	return (os  << "\t    " << gr.fpos << ": " << gr.rgba << std::endl);
#endif
    }
};

struct GradientStructure {
    union {	 uint8_t _;
	struct { uint8_t cnt:4, InterpolationMode:2, SpreadMode:2; };
    };
    std::vector<GradientRecord> grVec;

    enum {
	SPREAD_PAD = 0, SPREAD_REFLECT, SPREAD_REPEAT,
	SpreadPad  = 0, SpreadReflect, SpreadRepeat,

	INTERPOLATION_NORMAL = 0, INTERPOLATION_LINEAR,
	InterpolationNormal  = 0, InterpolationLinear,
    };
};

struct FillStyleGradient: public FillStyle, public GradientStructure {
    Matrix mtx;
    FIXED8 FocalPoint;

    /*
     * All gradients are defined in a standard space called the gradient
     * square. The gradient square is centered at (0,0), and extends from
     * (-16384,-16384) to (16384,16384).  Each gradient is mapped from the
     * gradient square to the display surface using a standard
     *
     * A linear gradient is defined from left to right.  A radial from inside
     * to outside(from center to periphery).  The gradients are always drawn
     * in a square with coordinates [-819.2, -819.2] to [819.2 819.2] pixels
     * (that's 16384 in TWIPs).  The usual is to scale the gradient square
     * down, translate to the proper position and rotate as necessary. There
     * is no point in rotating a radial gradient.
     */

    FillStyleGradient(uint8_t t, BitStream& bs, bool hasa): FillStyle(t) {
	alpha = (hasa ? 0x00 : 0xff);	load(bs);
    }

    BitStream& load(BitStream& bs) {
	bs >> mtx >> _;		grVec.resize(cnt);
	for (uint8_t i = 0; i < cnt; ++i) {
	    grVec[i].rgba.a = alpha;	bs >> grVec[i];
	}
	if (type == FillStyle::GRADIENT_RADIAL_FOCAL) bs >> FocalPoint;
	return bs;
    }

    static uint8_t alpha;

    friend BitStream& operator>>(BitStream& bs, FillStyleGradient& fsg)
	    { return fsg.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, FillStyleGradient& fsg) {
	os  << "  Gradient(" << (fsg.type == FillStyle::GRADIENT_RADIAL ?
		"linear" : "radial") << "):" << fsg.mtx << std::endl;
	for (std::vector<GradientRecord>::iterator it = fsg.grVec.begin();
		it != fsg.grVec.end(); ++it) os  << *it;	return os;
    }
};

struct FillStyleGradientMorph: public FillStyle, public GradientStructure {
    Matrix mtx0, mtx1;

    FillStyleGradientMorph(uint8_t t, BitStream& bs):
	    FillStyle(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	bs >> mtx0 >> mtx1 >> _;
	grVec.resize(cnt * 2);	// beg/end
	for (uint8_t i = 0; i < grVec.size(); ++i) bs >> grVec[i];
	return bs;
    }

    friend BitStream& operator>>(BitStream& bs,
	    FillStyleGradientMorph& fsg) { return fsg.load(bs); }
    friend std::ostream& operator<<(std::ostream& os,
	    FillStyleGradientMorph& fsg) {
	os  << " Gradient(" << (fsg.type == FillStyle::GRADIENT_RADIAL ?
		"linear" : "radial") << ") morph: \n\t  "
	    << fsg.mtx0 << " --> " << fsg.mtx1 << std::endl;
	for (std::vector<GradientRecord>::iterator it = fsg.grVec.begin();
		it != fsg.grVec.end(); ++it) os  << *it;	return os;
    }
};


struct FillStyleBitmap: public FillStyle {
    uint16_t rfid;
    Matrix mtx;

    FillStyleBitmap(uint8_t t, BitStream& bs): FillStyle(t) { load(bs); }

    BitStream& load(BitStream& bs) { return (bs >> rfid >> mtx); }

    friend BitStream& operator>>(BitStream& bs, FillStyleBitmap& fsb)
	    { return fsb.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, FillStyleBitmap& fsb) {
	return (os  << "  Bitmap " << (fsb.type % 2 ? "clipped" : "tilled")
		    << (fsb.type < FillStyle::BITMAP_TILLED_HARD ?  "(smooth)"
			: "(hard)") << ": #" << std::left
		    << std::setw(6) << fsb.rfid << std::right
		    << " " << fsb.mtx << std::endl);
    }
};

struct FillStyleBitmapMorph: public FillStyle {
    uint16_t rfid;
    Matrix mtx0, mtx1;

    FillStyleBitmapMorph(uint8_t t, BitStream& bs): FillStyle(t) { load(bs); }

    BitStream& load(BitStream& bs) { return (bs >> rfid >> mtx0 >> mtx1); }

    friend BitStream& operator>>(BitStream& bs, FillStyleBitmapMorph& fsb)
	    { return fsb.load(bs); }
    friend std::ostream& operator<<(std::ostream& os,
	    FillStyleBitmapMorph& fsb) {
	return (os  << "  Bitmap " << (fsb.type % 2 ? "clipped" : "tilled")
		    << (fsb.type < FillStyle::BITMAP_TILLED_HARD ?  "(smooth)"
			: "clipped(hard)") << " morph: #" << fsb.rfid
		    << "\n\t  " << fsb.mtx0 << " --> " << fsb.mtx1
		    << std::endl);
    }
};

inline BitStream& operator>>(BitStream& bs, FillStyle& fs)
{
    return bs;
}

inline std::ostream& operator<<(std::ostream& os, FillStyle& fs)
{
    return os;
}

struct LineStyle2 {
    RGBA Color;
    UI16 Width;
    union {
	struct {
	    uint16_t PixelHintingFlag:1, NoVScaleFlag:1, NoHScaleFlag:1,
		     HasFillFlag:1, JoinStyle:2, StartCapStyle:2,
		     EndCapStyle:2, NoClose:1, Reserved:5;
	};  uint16_t _;
    };
    FIXED8 MiterLimitFactor;
    FillStyle* FillType;

    enum {
	ROUND_CAP = 0, NO_CAP, SQUARE_CAP,
	RoundCap  = 0, NoCap, SquareCap,

	ROUND_JOIN = 0, BEVEL_JOIN, MITER_JOIN,
	RoundJoin  = 0, BevelJoin, MiterJoin,
    };

    ~LineStyle2() { delete FillType; }

    BitStream& load(BitStream& bs) {
	uint8_t type;

	bs >> Width >> _;
	if (JoinStyle == MITER_JOIN) bs >> MiterLimitFactor; else
	    MiterLimitFactor = 0;
	if (HasFillFlag) {
	    bs >> type;
	    switch (type) {
	    case FillStyle::SOLID:
		FillType = new FillStyleSolid(type, bs, true);		break;

	    case FillStyle::GRADIENT_LINEAR:
	    case FillStyle::GRADIENT_RADIAL:
	    case FillStyle::GRADIENT_RADIAL_FOCAL:
		FillType = new FillStyleGradient(type, bs, true); 	break;

	    case FillStyle::BITMAP_TILLED_SMOOTH:
	    case FillStyle::BITMAP_CLIPPED_SMOOTH:
	    case FillStyle::BITMAP_TILLED_HARD:
	    case FillStyle::BITMAP_CLIPPED_HARD:
		FillType = new FillStyleBitmap(type, bs);		break;
	    default:			FillType = NULL;
	    }
	} else bs >> Color;
	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, LineStyle2& ls)
	    { return ls.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, LineStyle2& ls) {
	return os;
    }
};

struct LineStyle {
    uint16_t width;
    RGBA rgba;

    BitStream& load(BitStream& bs) {	bs >> width;
	return (rgba.a ? (bs >> rgba.r >> rgba.g >> rgba.b)
		       : (bs >> rgba));
    }

    friend BitStream& operator>>(BitStream& bs, LineStyle& ls)
	    { return ls.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, LineStyle& ls) {
	return (os  << "  " << std::setw(4) << ls.width << " "
		    << ls.rgba << std::endl);
    }
};

struct MorphLineStyle {
    uint16_t width0, width1;
    RGBA rgba0, rgba1;

    BitStream& load(BitStream& bs)
	    { return (bs >> width0 >> width1 >> rgba0 >> rgba1); }

    friend BitStream& operator>>(BitStream& bs, MorphLineStyle& ls)
	    { return ls.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, MorphLineStyle& ls) {
	return (os  << "\t" << std::setw(4) << ls.width0 << "  "
		    << ls.rgba0 << " --> " << std::setw(4) << ls.width1
		    << "  " << ls.rgba1 << std::endl);
    }
};


struct StyleBits {
    union {	 uint8_t _;	// XXX:
	struct { uint8_t line:4, fill:4; };
    };

    BitStream& load(BitStream& bs) { return (bs >> _); }

    friend BitStream& operator>>(BitStream& bs, StyleBits& sb)
	    { return sb.load(bs); }
};

struct ShapeRecord {
    enum { EDGE_NBIT = 4u,  END_NBIT, };

    union {	 uint8_t _;//: END_NBIT + 1;
	struct { uint8_t endr: END_NBIT, type:1; }; // 0x00; nbit += 2
	struct { uint8_t nbit:EDGE_NBIT, edgt:1, typ_:1; };
	struct { uint8_t has_mv2:1, has_f0s:1 , has_f1s:1,
			 has_lns:1, has_news:1, ty__:1; };
    };

    ShapeRecord() { }
    ShapeRecord(ShapeRecord& sr): _(sr._) { }

    BitStream& load(BitStream& bs) {
	_ = bs.read(END_NBIT + 1, UNSIGN);	return bs;
    }

#ifdef	ABSOLUTE_COORDINATE
    static Twip x0, y0;
#endif

    virtual ~ShapeRecord() { }

    friend BitStream& operator>>(BitStream& bs, ShapeRecord& sr)
	    { return sr.load(bs); }
};

struct ShapeRecordSetup: public ShapeRecord {
    enum { MV2_NBIT = 5u, };

#if 1
    uint16_t f0s, f1s, lns;
#else// FIXME: how about using pointer instead?
    LineStyle *plns;
    FillStyle *pf0s, *pf1s;
#endif
    struct { Twip dx, dy; } mv2;
    // The very _first_ entry are not deltas from the previous point,
    // they are relative to shape origin

    ShapeRecordSetup(ShapeRecord& sr, BitStream& bs):
	    ShapeRecord(sr) { load(bs); }

    BitStream& load(BitStream& bs) {
	if (has_mv2) {
	    uint8_t nbit = bs.read(MV2_NBIT, UNSIGN);
#ifndef	ABSOLUTE_COORDINATE
	    mv2.dx = bs.read(nbit, SIGNED);
	    mv2.dy = bs.read(nbit, SIGNED);
#else
	    mv2.dx = (x0 += bs.read(nbit, SIGNED));
	    mv2.dy = (y0 += bs.read(nbit, SIGNED));
#endif
	} else
#ifndef	ABSOLUTE_COORDINATE
	    mv2.dx = mv2.dy = 0;
#else
	    mv2.dx = x0, mv2.dy = y0;
#endif
	f0s = (has_f0s ? (bs.read(snbit.fill, UNSIGN) + fcnt) : 0u) - 1;
	f1s = (has_f1s ? (bs.read(snbit.fill, UNSIGN) + fcnt) : 0u) - 1;
	lns = (has_lns ? (bs.read(snbit.line, UNSIGN) + lcnt) : 0u) - 1;
	return bs;
    }

    static uint16_t fcnt, lcnt;
    static StyleBits snbit;

    friend BitStream& operator>>(BitStream& bs, ShapeRecordSetup& sr)
	    { return sr.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, ShapeRecordSetup& sr) {
	os  <<  "    Change style:";
	if (sr.f0s < (uint16_t)-1) os << " F0S#" << (int)sr.f0s;
	if (sr.f1s < (uint16_t)-1) os << " F1S#" << (int)sr.f1s;
	if (sr.lns < (uint16_t)-1) os << " LNS#" << (int)sr.lns;
	os  << "\n\tMove to " <<
#ifndef	ABSOLUTE_COORDINATE
		'+'
#else// XXX:
		' '
#endif
	    << "[" << sr.mv2.dx << ' ' << sr.mv2.dy << "]\n";
	return os;
    }
};

struct ShapeRecordEdgeCurve: public ShapeRecord {
    struct { Twip dx, dy; } ctrl, anch;	//: nbit + 2;

    ShapeRecordEdgeCurve(ShapeRecord& sr, BitStream& bs):
	ShapeRecord(sr) { load(bs); }

    BitStream& load(BitStream& bs) {
	uint8_t nb = nbit + 2u;
#ifndef	ABSOLUTE_COORDINATE

#ifndef	CUBIC_BEZIER
	ctrl.dx = bs.read(nb, SIGNED);
	ctrl.dy = bs.read(nb, SIGNED);
	anch.dx = bs.read(nb, SIGNED);
	anch.dy = bs.read(nb, SIGNED);
#else
	ctrl.dx = bs.read(nb, SIGNED);
	ctrl.dy = bs.read(nb, SIGNED);
	anch.dx = bs.read(nb, SIGNED);
	anch.dy = bs.read(nb, SIGNED);

	cubi.dx = ctrl.dx + anch.dx / 3;
	cubi.dy = ctrl.dy + anch.dy / 3;
	anch.dx = ctrl.dx + anch.dx;
	anch.dy = ctrl.dy + anch.dy;
	ctrl.dx =(ctrl.dx << 1) / 3;
	ctrl.dy =(ctrl.dy << 1) / 3;
#endif

#else

#ifndef	CUBIC_BEZIER
	ctrl.dx = (x0 += bs.read(nb, SIGNED));
	ctrl.dy = (y0 += bs.read(nb, SIGNED));
	anch.dx = (x0 += bs.read(nb, SIGNED));
	anch.dy = (y0 += bs.read(nb, SIGNED));
#else
	ctrl.dx = bs.read(nb, SIGNED);
	ctrl.dy = bs.read(nb, SIGNED);
	anch.dx = bs.read(nb, SIGNED);
	anch.dy = bs.read(nb, SIGNED);

	cubi.dx = ctrl.dx + anch.dx/3 + x0;
	cubi.dy = ctrl.dy + anch.dy/3 + y0;
	anch.dx = ctrl.dx + anch.dx   + x0;
	anch.dy = ctrl.dy + anch.dy   + y0;
	ctrl.dx =(ctrl.dx << 1) / 3   + x0;
	ctrl.dy =(ctrl.dy << 1) / 3   + y0;

	x0 = anch.dx, y0 = anch.dy;
#endif

#endif//ABSOLUTE_COORDINATE
	return bs;
    }

#ifdef	CUBIC_BEZIER
    struct { Twip dx, dy; } cubi;
#endif

    friend BitStream& operator>>(BitStream& bs, ShapeRecordEdgeCurve& sr)
	    { return sr.load(bs); }
    friend std::ostream& operator<<(std::ostream& os,
	    ShapeRecordEdgeCurve& sr) {
#ifndef	ABSOLUTE_COORDINATE

	return (os  << "\tCurve   +[" << sr.ctrl.dx << " " << sr.ctrl.dy
#ifdef	CUBIC_BEZIER
		    <<      "], \t+[" << sr.cubi.dx << " " << sr.cubi.dy
#endif
		    <<    "] \tto +[" << sr.anch.dx << " " << sr.anch.dy
		    << "]\n");

#else

	return (os  << "\tCurve    [" << sr.ctrl.dx << " " << sr.ctrl.dy
#ifdef	CUBIC_BEZIER
		    <<      "], \t [" << sr.cubi.dx << " " << sr.cubi.dy
#endif
		    <<    "] \tto  [" << sr.anch.dx << " " << sr.anch.dy
		    << "]\n");

#endif//ABSOLUTE_COORDINATE
    }
};

struct ShapeRecordEdgeLine : public ShapeRecord {
    Twip dx, dy;	//: nbit + 2;

    ShapeRecordEdgeLine (ShapeRecord& sr, BitStream& bs):
	    ShapeRecord(sr) { load(bs); }

    BitStream& load(BitStream& bs) {
	uint8_t nb = nbit + 2u;
	bool has_xy = bs.read(1u, UNSIGN);
	if (has_xy) {
#ifndef	ABSOLUTE_COORDINATE
	    dx = bs.read(nb, SIGNED);
	    dy = bs.read(nb, SIGNED);
#else
	    dx = (x0 += bs.read(nb, SIGNED));
	    dy = (x0 += bs.read(nb, SIGNED));
#endif
	} else {    has_xy = bs.read(1u, UNSIGN);
#ifndef	ABSOLUTE_COORDINATE
	    if (has_xy) dy = bs.read(nb, SIGNED), dx = 0;
	    else	dx = bs.read(nb, SIGNED), dy = 0;
#else
	    if (has_xy) dy = (y0 += bs.read(nb, SIGNED)), dx = x0;
	    else	dx = (x0 += bs.read(nb, SIGNED)), dy = y0;
#endif
	}   return bs;
    }

    friend BitStream& operator>>(BitStream& bs, ShapeRecordEdgeLine& sr)
	    { return sr.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, ShapeRecordEdgeLine& sr) {
	os  << "\tLine to " <<
#ifndef	ABSOLUTE_COORDINATE
		'+'
#else
		' '
#endif
	    << "[" << sr.dx << ' ' << sr.dy << "]\n";
	return os;
    }
};

struct FontShape {
    //StyleBits snbit;	// used only when loading/parsing
    std::vector<ShapeRecord*> srVec;
    // Each shape is assumed to be defined within a 1024x1024 EM Square.

    ~FontShape() {
	for (std::vector<ShapeRecord*>::iterator it = srVec.begin();
		it != srVec.end(); ++it) delete *it;
    }

    BitStream& load(BitStream& bs) {
	bs >> ShapeRecordSetup::snbit;

#ifdef	ABSOLUTE_COORDINATE
	ShapeRecord::x0 = 0, ShapeRecord::y0 = 0;
#endif

	for (ShapeRecord sr, *psr; ;) { bs >> sr;
	    if (sr.type) {
		if (sr.edgt) psr = new ShapeRecordEdgeLine (sr, bs);
		else	     psr = new ShapeRecordEdgeCurve(sr, bs);
	    } else {	     if (!sr.endr) break;
			     psr = new ShapeRecordSetup    (sr, bs);
		// XXX: add some sanity check here for f0s/f1s/lns
	    }   srVec.push_back(psr);
	}	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, FontShape& fs)
	    { return fs.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, FontShape& fs) {
	uint16_t i, cnt = fs.srVec.size();
	os  << std::setw(4) << cnt << " records\n";
	for (i = 0u; i < cnt; ++i) {
	    ShapeRecord* psr = fs.srVec[i];
	    if (psr->type) {
		if (psr->edgt)
		     os  << *reinterpret_cast<ShapeRecordEdgeLine* >(psr);
		else os  << *reinterpret_cast<ShapeRecordEdgeCurve*>(psr);
	    } else   os  << *reinterpret_cast<ShapeRecordSetup*    >(psr);
	}   return os;
    }
};

struct ShapeWithStyle {
    //uint16_t fcnt, lcnt;
    std::vector<FillStyle*>   fsVec;
    std::vector<LineStyle>    lsVec;  // StyleBits snbit;
    std::vector<LineStyle2>   l2Vec;  // StyleBits snbit;
    std::vector<ShapeRecord*> srVec;

     //ShapeWithStyle(): fcnt(0u), lcnt(0u) { }
    ~ShapeWithStyle() {
	for (std::vector<ShapeRecord*>::iterator it = srVec.begin();
		it != srVec.end(); ++it) delete *it;
	for (std::vector<FillStyle*>::iterator it = fsVec.begin();
		it != fsVec.end(); ++it) delete *it;
    }

    BitStream& load(BitStream& bs) {
	union { uint8_t coun_, alpha, type; };
	uint16_t i, fcnt = 0u, lcnt = 0u;

#ifdef	ABSOLUTE_COORDINATE
	ShapeRecord::x0 = 0, ShapeRecord::y0 = 0;
#endif

NEWS:	bs >> coun_;	     ShapeRecordSetup::fcnt = fcnt;
	if (coun_ == 0xff && lots) bs >> fcnt; else fcnt = coun_;
	fsVec.resize(fcnt += ShapeRecordSetup::fcnt);

	for (i = ShapeRecordSetup::fcnt; i < fcnt; ++i) {
	    FillStyle* pfs;		bs >> type;
	    switch (type) {
	    case FillStyle::SOLID:
		pfs = new FillStyleSolid(type, bs, hasa);	break;
	    case FillStyle::GRADIENT_LINEAR:
	    case FillStyle::GRADIENT_RADIAL:
	    case FillStyle::GRADIENT_RADIAL_FOCAL:
		pfs = new FillStyleGradient(type, bs, hasa); 	break;
	    case FillStyle::BITMAP_TILLED_SMOOTH:
	    case FillStyle::BITMAP_CLIPPED_SMOOTH:
	    case FillStyle::BITMAP_TILLED_HARD:
	    case FillStyle::BITMAP_CLIPPED_HARD:
		pfs = new FillStyleBitmap(type, bs);		break;
	    default:			pfs = NULL;
	    }				fsVec[i] = pfs;
	}

	bs >> coun_;	     ShapeRecordSetup::lcnt = lcnt;
	if (coun_ == 0xff && lots) bs >> lcnt; else lcnt = coun_;
	lsVec.resize(lcnt += ShapeRecordSetup::lcnt);
	alpha = (hasa ? 0x00 : 0xff);
	for (i = ShapeRecordSetup::lcnt; i < lcnt; ++i) {
	    lsVec[i].rgba.a = alpha;	bs >> lsVec[i];
	}

	   bs >> ShapeRecordSetup::snbit;
	for (ShapeRecord sr, *psr; ;) { bs >> sr;
	    if (sr.type) {
		if (sr.edgt) psr = new ShapeRecordEdgeLine (sr, bs);
		else	     psr = new ShapeRecordEdgeCurve(sr, bs);
	    } else {	     if (!sr.endr) break;
			     psr = new ShapeRecordSetup    (sr, bs);
		// XXX: add some sanity check here for f0s/f1s/lns
		if (//news &&
		    reinterpret_cast  <ShapeRecordSetup*>(psr)->has_news) {
		    srVec.push_back(psr);	goto NEWS;
		}
	    }	    srVec.push_back(psr);
	}	    return bs;
    }

    static bool lots, hasa, ls2;//, news;

    friend BitStream& operator>>(BitStream& bs, ShapeWithStyle& sws)
	    { return sws.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, ShapeWithStyle& sws) {
	uint16_t i, cnt = sws.fsVec.size();
	os  << "  Fill styles(" << cnt << "): \n";
	for (i = 0u; i < cnt; ++i) {
	    FillStyle* pfs = sws.fsVec[i];
	    os  << "    #" << std::setw(4) << std::left << i << std::right;
	    switch (pfs->type) {
	    case FillStyle::SOLID:
		os  << *(FillStyleSolid*)pfs;			break;
	    case FillStyle::GRADIENT_LINEAR:
	    case FillStyle::GRADIENT_RADIAL:
	    case FillStyle::GRADIENT_RADIAL_FOCAL:
		os  << *(FillStyleGradient*)pfs;		break;
	    case FillStyle::BITMAP_TILLED_SMOOTH:
	    case FillStyle::BITMAP_CLIPPED_SMOOTH:
	    case FillStyle::BITMAP_TILLED_HARD:
	    case FillStyle::BITMAP_CLIPPED_HARD:
		os  << *(FillStyleBitmap*)pfs;			break;
	    default:
		os  << "  Unknown(0x" << std::setfill('0') << std::hex
		    << std::setw(2) << pfs->type << ")\n"  << std::dec
		    << std::setfill(' ');
	    }
	}

	os  << "  Line styles(" << (cnt = sws.lsVec.size()) << "): \n";
	for (i = 0u; i < cnt; ++i)
	    os  << "    #" << std::setw(4) << std::left << i
		<< ": " << std::right << sws.lsVec[i];
#if 0
	os  << "  Style bits: F-" << (int)ShapeRecordSetup::snbit.fill
	    <<		   ", L-" << (int)ShapeRecordSetup::snbit.line
	    << std::endl;
#endif
	os  << "  Shape records(" << (cnt = sws.srVec.size()) << "): \n";
	for (i = 0u; i < cnt; ++i) {
	    ShapeRecord* psr = sws.srVec[i];
	    if (psr->type) {
		if (psr->edgt)
		     os  << *reinterpret_cast<ShapeRecordEdgeLine* >(psr);
		else os  << *reinterpret_cast<ShapeRecordEdgeCurve*>(psr);
	    } else   os  << *reinterpret_cast<ShapeRecordSetup*    >(psr);
	}   return os;
    }
};

struct MorphShapeWithStyle {
    //uint16_t fcnt, lcnt;
    std::vector<FillStyle*> fsVec;
    std::vector<MorphLineStyle>  lsVec;  // StyleBits snbit;
    //std::vector<MorphLineStyle2> l2Vec;  // StyleBits snbit;
    std::vector<ShapeRecord*> srVec0, srVec1;

/*
 * Shape `morphing' is the metamorphosis of one shape into another over
 * time. Macromedia Flash supports a flexible morphing model, which allows a
 * number of shape attributes to vary during the morph. Macromedia Flash (SWF)
 * defines only the start and end states of the morph. The Macromedia Flash
 * Player is responsible for interpolating between the end-points and
 * generating the ‘in-between’ states.
 *
 * The shape attributes that can be varied during the morph are:
 * * The position of each edge in the shape.
 * * The color and thickness of the outline.
 * * The fill color of the shape (if filled with a color)
 * * The bitmap transform (if filled with a bitmap)
 * * The gradient transform (if filled with a gradient)
 * * The color and position of each point in the gradient.
 *
 * A number of restrictions apply to morphing. These restrictions are:
 * * The start and end shapes must have the same number of edges
 * * The start and end shapes must have the same type of fill
 *	(i.e. solid, gradient or bitmap)
 * * The style change records must be the same for the start and end shapes
 * * If filled with a bitmap, both shapes must be filled with the same bitmap.
 * * If filled with a gradient, both gradients must have
 *	the same number of color points.
 */

    ~MorphShapeWithStyle() {
	for (std::vector<ShapeRecord*>::iterator it = srVec1.begin();
		it != srVec1.end(); ++it) delete *it;
	for (std::vector<ShapeRecord*>::iterator it = srVec0.begin();
		it != srVec0.end(); ++it) delete *it;
	for (std::vector<FillStyle*>::iterator it = fsVec.begin();
		it != fsVec .end(); ++it) delete *it;
    }

    BitStream& load(BitStream& bs) {
	bool start = true;
	union { uint8_t coun_, type; };
	uint16_t i, fcnt = 0u, lcnt = 0u;

#ifdef	ABSOLUTE_COORDINATE
	ShapeRecord::x0 = 0, ShapeRecord::y0 = 0;
#endif

NEWS:	bs >> coun_;	     ShapeRecordSetup::fcnt = fcnt;
	if (coun_ == 0xff) bs >> fcnt; else fcnt = coun_;
	fsVec.resize(fcnt += ShapeRecordSetup::fcnt);
	for (i = ShapeRecordSetup::fcnt; i < fcnt; ++i) {
	    FillStyle* pfs;		bs >> type;
	    switch (type) {
	    case FillStyle::SOLID:
		pfs = new FillStyleSolidMorph(type, bs); 	break;
	    case FillStyle::GRADIENT_LINEAR:
	    case FillStyle::GRADIENT_RADIAL:
	    //case FillStyle::GRADIENT_RADIAL_FOCAL:
		pfs = new FillStyleGradientMorph(type, bs); 	break;
	    case FillStyle::BITMAP_TILLED_SMOOTH:
	    case FillStyle::BITMAP_CLIPPED_SMOOTH:
	    case FillStyle::BITMAP_TILLED_HARD:
	    case FillStyle::BITMAP_CLIPPED_HARD:
		pfs = new FillStyleBitmapMorph(type, bs); 	break;
	    default:			pfs = NULL;
	    }				fsVec[i] = pfs;
	}

	bs >> coun_;	     ShapeRecordSetup::lcnt = lcnt;
	if (coun_ == 0xff) bs >> lcnt;  else lcnt = coun_;
	lsVec.resize(lcnt += ShapeRecordSetup::lcnt);
	for (i = ShapeRecordSetup::lcnt; i < lcnt; ++i) bs >> lsVec[i];

    if (start) {
	   bs >> ShapeRecordSetup::snbit;
	for (ShapeRecord sr, *psr; ;) { bs >> sr;
	    if (sr.type) {
		if (sr.edgt) psr = new ShapeRecordEdgeLine (sr, bs);
		else	     psr = new ShapeRecordEdgeCurve(sr, bs);
	    } else {	     if (!sr.endr) break;
			     psr = new ShapeRecordSetup    (sr, bs);
		// XXX: add some sanity check here for f0s/f1s/lns
		if (reinterpret_cast  <ShapeRecordSetup*>(psr)->has_news) {
		    srVec0.push_back(psr);	goto NEWS;
		}
	    }	    srVec0.push_back(psr);
	}

#if 0//def	ABSOLUTE_COORDINATE
	ShapeRecord::x0 = 0, ShapeRecord::y0 = 0;	// XXX:
#endif
    }	start = false;

	   bs >> ShapeRecordSetup::snbit;
	for (ShapeRecord sr, *psr; ;) { bs >> sr;
	    if (sr.type) {
		if (sr.edgt) psr = new ShapeRecordEdgeLine (sr, bs);
		else	     psr = new ShapeRecordEdgeCurve(sr, bs);
	    } else {	     if (!sr.endr) break;
			     psr = new ShapeRecordSetup    (sr, bs);
		// XXX: add some sanity check here for f0s/f1s/lns
		if (reinterpret_cast  <ShapeRecordSetup*>(psr)->has_news) {
		    srVec1.push_back(psr);	goto NEWS;
		}
	    }	    srVec1.push_back(psr);
	}	    return bs;
    }

    friend BitStream& operator>>(BitStream& bs, MorphShapeWithStyle& sws)
	    { return sws.load(bs); }
    friend std::ostream& operator<<(std::ostream& os,
	    MorphShapeWithStyle& sws) {
	uint16_t i, cnt = sws.fsVec.size();
	os  << "  Fill styles(" << cnt << "): \n";
	for (i = 0u; i < cnt; ++i) {
	    FillStyle* pfs = sws.fsVec[i];
	    os  << "    #" << std::setw(4) << std::left << i << std::right;
	    switch (pfs->type) {
	    case FillStyle::SOLID:
		os  << *(FillStyleSolidMorph*)pfs;		break;
	    case FillStyle::GRADIENT_LINEAR:
	    case FillStyle::GRADIENT_RADIAL:
		os  << *(FillStyleGradientMorph*)pfs;		break;
	    case FillStyle::BITMAP_TILLED_SMOOTH:
	    case FillStyle::BITMAP_CLIPPED_SMOOTH:
	    case FillStyle::BITMAP_TILLED_HARD:
	    case FillStyle::BITMAP_CLIPPED_HARD:
		os  << *(FillStyleBitmapMorph*)pfs;		break;
	    default:
		os  << "  Unknown(0x" << std::setfill('0') << std::hex
		    << std::setw(2) << pfs->type << ")\n"  << std::dec
		    << std::setfill(' ');
	    }
	}

	os  << "  Line styles(" << (cnt = sws.lsVec.size()) << "): \n";
	for (i = 0; i < cnt; ++i)
	    os  << "    #" << std::setw(4) << std::left << i
		<< ": " << std::right << sws.lsVec[i];
#if 0
	os  << "  Style bits: F-" << (int)ShapeRecordSetup::snbit.fill
	    <<		   ", L-" << (int)ShapeRecordSetup::snbit.line
	    << std::endl;
#endif
	os  << "  Shape records(" << (cnt = sws.srVec0.size()) << "): start\n";
	for (i = 0; i < cnt; ++i) {
	    ShapeRecord* psr = sws.srVec0[i];
	    if (psr->type) {
		if (psr->edgt)
		     os  << *reinterpret_cast<ShapeRecordEdgeLine* >(psr);
		else os  << *reinterpret_cast<ShapeRecordEdgeCurve*>(psr);
	    } else   os  << *reinterpret_cast<ShapeRecordSetup*    >(psr);
	}

	os  << "  Shape records(" << (cnt = sws.srVec1.size()) << "):   end\n";
	for (i = 0; i < cnt; ++i) {
	    ShapeRecord* psr = sws.srVec1[i];
	    if (psr->type) {
		if (psr->edgt)
		     os  << *reinterpret_cast<ShapeRecordEdgeLine* >(psr);
		else os  << *reinterpret_cast<ShapeRecordEdgeCurve*>(psr);
	    } else   os  << *reinterpret_cast<ShapeRecordSetup*    >(psr);
	}   return os;
    }
};

struct TextRecord {
    union {	 uint8_t endr;
	struct { uint8_t glyf:7, type:1; };
	struct { uint8_t has_movx:1, has_movy:1, has_colr:1, has_font:1,
			 xxx:3, typ_:1;
	};
    };

    BitStream& load(BitStream& bs) { return (bs >> endr); }

    friend BitStream& operator>>(BitStream& bs, TextRecord& tr)
	    { return tr.load(bs); }
};

struct TextEntry {
    uint32_t gidx;
    int32_t  advs;

    BitStream& load(BitStream& bs) {
	gidx = bs.read(gnb, UNSIGN);
	advs = bs.read(anb, SIGNED);	return bs;
    }

    static uint8_t gnb, anb;

    friend BitStream& operator>>(BitStream& bs, TextEntry& te)
	    { return te.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, TextEntry& te) {
	return (os  << "\tGlyph #" << std::setfill('0') << std::setw(4)
		<< te.gidx << ", advanced by " << std::setfill(' ')
		<< std::setw(4) << te.advs << " twips\n");
    }
};

#if 0// XXX:
struct TextRecordSetup: public TextRecord {
    RGBA rgba;		// out of order regarding alignment
    uint16_t fref;
    uint16_t height;
    struct { int16_t dx, dy; } mv2;

    TextRecordSetup(TextRecord& tr, bool hasa): TextRecord(tr)
	    { rgba.a = (hasa ? 0x00 : 0xff); }

    BitStream& load(BitStream& bs) {	bs >> fref;
	if (has_colr) {
	    if (!rgba.a) bs >> rgba; else bs >> rgba.r >> rgba.g >> rgba.b;
	}
	if (has_movx)  bs >> mv2.dx;
	if (has_movy)  bs >> mv2.dy;
	if (has_font)  bs >> height;	return bs;
    }
};

struct TextRecordGlyph: public TextRecord {
    std::vector<TextEntry> teVec;

    TextRecordGlyph(TextRecord& tr): TextRecord(tr) { }

    BitStream& load(BitStream& bs) {	teVec.resize(glyf);
	for (uint8_t i = 0; i < glyf; ++i) bs >> teVec[i];
	return bs;
    }
};

struct TextRecordString: public TextRecordSetup {
    std::vector<TextEntry> teVec;

    TextRecordString(TextRecordSetup& tr): TextRecordSetup(tr) { }

    BitStream& load(BitStream& bs) {
	uint8_t count;		TextRecordSetup::load(bs);
	bs >> count;		teVec.resize(count);
	for (uint8_t i = 0; i < glyf; ++i) bs >> teVec[i];
	return bs;
    }
};

#else

struct TextRecordString: public TextRecord {
    RGBA rgba;		// out of order regarding alignment
    uint16_t fref;
    uint16_t height;
    struct { int16_t dx, dy; } mv2;
    std::vector<TextEntry> teVec;

    TextRecordString(TextRecord& tr, bool hasa): TextRecord(tr)
	    { rgba.a = (hasa ? 0x00 : 0xff); }
    BitStream& load(BitStream& bs) {	uint8_t count;
	if (has_font) bs >> fref; else fref = 0u;
	if (has_colr) {
	    if (!rgba.a) bs >> rgba; else bs >> rgba.r >> rgba.g >> rgba.b;
	}   else rgba.r = rgba.g = rgba.b = 0x00, rgba.a = 0xff;
	if (has_movx) bs >> mv2.dx; else mv2.dx = 0;
	if (has_movy) bs >> mv2.dy; else mv2.dy = 0;
	if (has_font) bs >> height; else height = 0u;
	bs >> count;		teVec.resize(count);
	for (uint8_t i = 0; i < count; ++i) bs >> teVec[i];
	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, TextRecordString& trs)
	    { return trs.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, TextRecordString& trs) {
	os  << "\tFont #" << trs.fref << ", height " << trs.height
	    << ", color " << trs.rgba << ", offset [" << trs.mv2.dx
	    << " " << trs.mv2.dy << "]\n";
	for (std::vector<TextEntry>::iterator it = trs.teVec.begin();
		it != trs.teVec.end(); ++it) os << *it;		return os;
    }
};
#endif

struct Kerning {
    union {
	struct { uint8_t c1, c2; };
	uint16_t code;
    };   int16_t adjs;

    BitStream& load(BitStream& bs) { return (bs >> code >> adjs); }

    friend BitStream& operator>>(BitStream& bs, Kerning& kn)
	    { return kn.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Kerning& kn) {
	return (os  << "\t(#" << std::setfill('0') << std::setw(4)
		    << kn.c1 << ", #" << std::setw(4) << kn.c2
		    << std::setfill(' ') << ") adjuct "
		    << kn.adjs << " twips\n");
    }
};

struct KerningWide {
    union {
	struct { uint16_t c1, c2; };
	uint32_t code;
    };   int16_t adjs;

    BitStream& load(BitStream& bs) { return (bs >> code >> adjs); }

    friend BitStream& operator>>(BitStream& bs, KerningWide& kn)
	    { return kn.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, KerningWide& kn) {
	return (os  << "\t(#" << std::setfill('0') << std::setw(4)
		    << kn.c1 << ", #" << std::setw(4) << kn.c2
		    << std::setfill(' ') << ") adjuct "
		    << kn.adjs << " twips\n");
    }
};

struct Envelope {
    uint32_t p44k;		// unit in samples
    uint16_t vll, vlr;		// XXX: prescaled by 256
    BitStream& load(BitStream& bs) { return (bs >> p44k >> vll >> vlr); }

    friend BitStream& operator>>(BitStream& bs, Envelope& ev)
	    { return ev.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Envelope& ev) {
	return (os  << "\t" << std::setw(5) << ev.p44k
		    << ": L-" << ev.vll << ", R-" << ev.vlr << std::endl);
    }
};

struct SoundInfo {
    uint16_t rfid;
    union {
	struct {
	    uint8_t has_ip:1, has_op:1, has_lp:1, has_en:1,
		    nomul:1, stop:1, xx:2;
	};  uint8_t _;
    };
    //uint8_t  ecount;	// out of order regarding alignment
    uint16_t lcount;	// out of order regarding alignment
    uint32_t ipoint, opoint;
    std::vector<Envelope> enVec;

    BitStream& load(BitStream& bs) {
	uint8_t  ecount;		bs >> rfid >> _;
	if (has_ip) bs >> ipoint; else ipoint = 0;
	if (has_op) bs >> opoint; else opoint = UINT_MAX;
	if (has_lp) bs >> lcount; else lcount = 1;
	if (has_en){bs >> ecount; enVec.resize(ecount);
	    for (uint8_t i = 0; i < ecount; ++i) bs >> enVec[i];
	} else ecount = 0;		return bs;
    }

    friend BitStream& operator>>(BitStream& bs, SoundInfo& si)
	    { return si.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, SoundInfo& si) {
	os  << "  sound #" << si.rfid << " loop " << si.lcount
	    << " times from " << si.ipoint << " to " << si.opoint << std::endl;
	if (!si.enVec.empty()) os  << "    Envelopes:\n";
	for (std::vector<Envelope>::iterator it = si.enVec.begin();
		it != si.enVec.end(); ++it) os << *it;		return os;
    }
};

struct External {
    uint16_t rfid;
    std::string smbl;

    BitStream& load(BitStream& bs) { return (bs >> rfid >> smbl); }

    friend BitStream& operator>>(BitStream& bs, External& ex)
	    { return ex.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, External& ex) {
	return (os  << "  #" << std::left << std::setw(5) << ex.rfid
		    << ": " << ex.smbl.data() << std::right << std::endl);
    }
};

struct Params {
    uint8_t regs;
    std::string name;

    BitStream& load(BitStream& bs) { return (bs >> regs >> name); }

    friend BitStream& operator>>(BitStream& bs, Params& pm)
	    { return pm.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Params& pm) {
	return (os  << "  0x" << std::left	  << std::setfill('0')
		    << std::setw(4)   << std::hex << pm.regs << ": "
		    << pm.name.data() << std::dec << std::setfill(' ')
		    << std::endl);
    }
};

struct Button {
    //enum ButtonState { UPON, OVER, DOWN, HITT, NMAX };

    union {	 uint8_t _;
	struct { uint8_t upon:1, over:1, down:1, hitt:1, xxxx:4; };
    };
    uint16_t btid, layer;
    Matrix mtx;

    BitStream& load(BitStream& bs) { return (bs >> btid >> layer >> mtx); }

    friend BitStream& operator>>(BitStream& bs, Button& bt)
	    { return bt.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Button& btn) {
	os << "    State(s):";
	if (btn.upon) os << " UPON";	if (btn.over) os << " OVER";
	if (btn.down) os << " DOWN";	if (btn.hitt) os << " HITT";
	return (os  << "\n      Button #" << btn.btid << " at depth #"
		    << btn.layer << ": \n\t Coord Matrix: "
		    << btn.mtx << std::endl);
    }
};

struct Button2: public Button {
    CXForm cxf;

    BitStream& load(BitStream& bs) { return (Button::load(bs) >> cxf); }

    friend BitStream& operator>>(BitStream& bs, Button2& bt)
	    { return bt.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Button2& btn) {
	os << "    State(s):";
	if (btn.upon) os << " UPON";	if (btn.over) os << " OVER";
	if (btn.down) os << " DOWN";	if (btn.hitt) os << " HITT";
	return (os  << "\n      Button #" << btn.btid << " at depth #"
		    << btn.layer << ": \n\tCoord Matrix: "
		    << btn.mtx << "\n\tColor Matrix: "
		    << btn.cxf << std::endl);
    }
};

struct Event {
    union {
	struct {
	    uint32_t onload:1, enter_frame:1, unload:1, mouse_down:1,
		     mouse_move:1, mouse_up:1, key_down:1, key_up:1,
		     data:1, init:1, press:1, release:1, release_outside:1,
		     roll_over:1, roll_out:1, drag_over:1, drag_out:1,
		     key_press:1, construct:1, x11x:13;
	};  uint32_t _;
    };	//uint32_t length;
    std::vector<Action*> axVec;

     Event(uint32_t f): _(f) { }
    ~Event() {
	for (std::vector<Action*>::iterator it = axVec.begin();
		it != axVec.end(); ++it) delete *it;
    }

    BitStream& load(BitStream& bs) {	uint32_t length;
	Action* ax;			bs >> length;
	do {	ax = new Action;	bs >> *ax;
	    axVec.push_back(ax);
	} while (ax->id != ACTION::CODE::End);   return bs;
    }

    friend BitStream& operator>>(BitStream& bs, Event& ev)
	    { return ev.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Event& ev) {
	os  << "    Events: 0x"	    << std::setfill('0') << std::hex
	    << std::setw(8) << ev._ << std::setfill(' ') << std::dec
	    << std::endl;
	for (std::vector<Action*>::iterator it = ev.axVec.begin();
		it != ev.axVec.end(); ++it) os << **it;		return os;
    }
};

struct Condition {
    enum {
	KEY_LEFT = 1u, KEY_RIGHT, KEY_HOME, KEY_END, KEY_INSERT,
	KEY_DELETE = 6u, KEY_BACKSPACE = 8u, KEY_ENTER = 13u,
	KEY_UP, KEY_DOWN, KEY_PAGEUP, KEY_PAGEDOWN, KEY_TAB, KEY_SPACE,
    };

    //uint16_t len;
    union {
	struct {
	    uint16_t pointer_release_enter:1,	pointer_release_leave:1,
		     pointer_over_down:1,	pointer_over_up:1,
		     pointer_drag_leave:1,	pointer_drag_enter:1,
		     pointer_outdown_overup:1,
		     menu_enter:1, menu_leave:1, key:7;
	};  uint16_t _;
    };  std::vector<Action*> axVec;

    ~Condition() {
	for (std::vector<Action*>::iterator it = axVec.begin();
		it != axVec.end(); ++it) {
	    delete *it;
	}
    }

    BitStream& load(BitStream& bs) {	//uint16_t len;
	Action* ax;			bs >> /*len >> */_;
	do {	ax = new Action;	bs >> *ax;
	    axVec.push_back(ax);
	} while (ax->id != ACTION::CODE::End);	    return bs;
    }

    friend BitStream& operator>>(BitStream& bs, Condition& cx)
	    { return cx.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Condition& cx) {
	os  << "    Conditions: 0x" << std::setfill('0') << std::hex
	    << std::setw(4) << cx._ << std::setfill(' ') << std::dec
	    << std::endl;
	for (std::vector<Action*>::iterator it = cx.axVec.begin();
		it != cx.axVec.end(); ++it) os << **it;
	return os;
    }
};

struct Filter {
    enum FilterCode {
	DropShadow = 0, Blur, Grow, Bevel, GradientGrow,
	Convolution, AdjustColor, ColorMatrix = 6, GradientBevel,
	DROPSHADOW = 0, BLUR, GROW, BEVEL, GRADIENTGROW,
	CONVOLUTION, ADJUSTCOLOR, COLORMATRIX = 6, GRADIENTBEVEL,
    };

    typedef FilterCode FilterType;
    typedef FilterCode FilterID;

    uint8_t type;

    Filter(uint8_t t): type(t) { }
    //Filter(Filter& f): type(f.type) { }

    BitStream& load(BitStream& bs) { return (bs >> type); }

    friend BitStream& operator>>(BitStream& bs, Filter& fl)
	    { return fl.load(bs); }
};

struct FilterGrow: public Filter {
    RGBA ShadowColor;
    FIXED BlurX, BlurY;
    FIXED8 Strength;
    union {
	struct {
	    uint8_t Passes:5, CompositeSource:1, Knockout:1, InnerShadow:1;
	};  uint8_t _;
    };

    FilterGrow(uint8_t t): Filter(t) { }
    //FilterGrow(Filter& f): Filter(f) { }
    FilterGrow(uint8_t t, BitStream& bs): Filter(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	return bs >> ShadowColor >> BlurX >> BlurY >> Strength >> _;
    }

    friend BitStream& operator>>(BitStream& bs, FilterGrow& fl)
	    { return fl.load(bs); }
};

struct FilterDropShadow: public FilterGrow {
    FIXED Angle, Distance;

    //FilterDropShadow(uint8_t t): Filter(t) { }
    //FilterDropShadow(Filter& f): Filter(f) { }
    FilterDropShadow(uint8_t t, BitStream& bs): FilterGrow(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	bs  >> ShadowColor >> BlurX >> BlurY >> Angle >> Distance
	    >> Strength >> _;	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, FilterDropShadow& fl)
	    { return fl.load(bs); }
};

struct FilterBlur: public Filter {
    FIXED BlurX, BlurY;
    union {	 uint8_t _;
	struct { uint8_t Passes:5, Reserved:3; };
    };

    //FilterBlur(uint8_t t): Filter(t) { }
    //FilterBlur(Filter& f): Filter(f) { }
    FilterBlur(uint8_t t, BitStream& bs): Filter(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	bs >> BlurX >> BlurY >> _;	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, FilterBlur& fl)
	    { return fl.load(bs); }
};

struct FilterBevel: public Filter {
    RGBA ShadowColor, HighlightColor;
    FIXED BlurX, BlurY, Angle, Distance;
    FIXED8 Strength;
    union {	 uint8_t _;
	struct { uint8_t Passes:4, OnTop:1, CompositeSource:1,
			 Knockout:1, InnerShadow:1;
	};
    };

    //FilterBevel(uint8_t t): Filter(t) { }
    //FilterBevel(Filter& f): Filter(f) { }
    FilterBevel(uint8_t t, BitStream& bs): Filter(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	return bs >> ShadowColor >> HighlightColor >> BlurX >> BlurY
		  >> Angle >> Distance >> Strength >> _;
    }

    friend BitStream& operator>>(BitStream& bs, FilterBevel& fl)
	    { return fl.load(bs); }
};

struct FilterConvolution: public Filter {
    union {	 uint8_t _;
	struct { uint8_t PreserveAlpha:1, Clamp:1, Reserved:6; };
    };	// out of order regarding alignment
    RGBA DefaultColor;
    FIXED Divisor, Bias;
    std::vector<std::vector<FIXED> > matVec;	// XXX: float

    // FilterConvolution(uint8_t t): Filter(t) { }
    // FilterConvolution(Filter& f): Filter(f) { }
     FilterConvolution(uint8_t t, BitStream& bs): Filter(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	float val;
	UI8 MatrixX, MatrixY;
	bs >> MatrixX >> MatrixY;
	bs >> val; Divisor = val * 65535.f;
	bs >> val; Bias	   = val * 65535.f;
	matVec.resize(MatrixY);
	for (uint8_t j = 0; j < MatrixY; ++j) {
	    matVec[j].resize(MatrixX);
	    for (uint8_t i = 0; i < MatrixX; ++i) {
		bs >> val; matVec[j][i] = val * 65535.f;
	    }
	}
	bs >> DefaultColor >> _;	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, FilterConvolution& fl)
	    { return fl.load(bs); }
};

struct FilterAdjustColor: public Filter {
    /* A Color Matrix filter applies a color transformation on the pixels of a
     * display list object. Given an input RGBA pixel in a display list
     * object, the color transformation is calculated in the following way:
     *
     * The resulting RGBA values are saturated. The matrix values are stored
     * from left to right and each row from top to bottom. The last row is
     * always assumed to be (0,0,0,0,1) and does not need to be stored.
     *
     *	|R'|   |r0 r1 r2 r3 r4| |R|
     *	|G'|   |g0 g1 g2 g3 g4| |G|
     *	|B'| = |b0 b1 b2 b3 b4| |B|
     *	|A'|   |a0 a1 a2 a3 a4| |A|
     *	|1 |   | 0  0  0  0  1| |1|
     */

    union {
	float _f[5][5], __f[25];
	FIXED _F[5][5], __F[25];
	struct { FIXED R[5], G[5], B[5], A[5], O[5]; };
    };

    //FilterAdjustColor(uint8_t t): Filter(t) { }
    //FilterAdjustColor(Filter& f): Filter(f) { }
    FilterAdjustColor(uint8_t t, BitStream& bs): Filter(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	for (uint8_t i = 0; i < 4 * 5; ++i) {
	    bs >> __f[i];	__F[i] = __f[i] * 65535.f;
	}
	__F[23] = __F[22] = __F[21] = __F[20] = 0, __F[24] = 1;	// XXX:
	return bs;
    }

    friend BitStream& operator>>(BitStream& bs, FilterAdjustColor& fl)
	    { return fl.load(bs); }
};
typedef FilterAdjustColor FilterColorMatrix;

struct FilterGradientGrow: public Filter {
    //uint8_t NumColors;
    std::vector<RGBA> GradientColors;
    std::vector<UI8>  GradientRatio;
    FIXED BlurX, BlurY, Angle, Distance;
    FIXED8 Strength;
    union {	 uint8_t _;
	struct { uint8_t Passes:4, OnTop:1, CompositeSource:1,
			 Knockout:1, InnerShadow:1;
	};
    };

    FilterGradientGrow(uint8_t t): Filter(t) { }
    //FilterGradientGrow(Filter& f): Filter(f) { }
    FilterGradientGrow(uint8_t t, BitStream& bs): Filter(t) { load(bs); }

    BitStream& load(BitStream& bs) {
	uint8_t NumColors;
	bs >> NumColors;
	GradientColors.resize(NumColors);
	GradientRatio .resize(NumColors);
	for (uint8_t i = 0; i < NumColors; ++i) bs >> GradientColors[i];
	for (uint8_t i = 0; i < NumColors; ++i) bs >> GradientRatio[i];
	return bs >> BlurX >> BlurY >> Angle >> Distance >> Strength >> _;
    }

    friend BitStream& operator>>(BitStream& bs, FilterGradientGrow& fl)
	    { return fl.load(bs); }
};

typedef FilterGradientGrow FilterGradientBevel;

struct ClipActions {
    //UI16 Reserved;
    union ClipEventFlags {
	struct {
	    union {	 uint16_t _;
		struct { uint16_t Load:1, EnterFrame:1, Unload:1, MouseMove:1,
				  MouseDown:1, MouseUp:1, KeyDown:1, KeyUp:1,
				  Data:1, Initialize:1, Press:1, Release:1,
			ReleaseOutside:1, RollOver:1, RollOut:1, DragOver:1;
		};
	    };
	    struct { uint16_t DragOut:1, KeyPress:1, Construct:1,
			      Reserved:5, KeyCode:8;	// XXX:
	    };
	};  uint32_t __;
    };

};

inline uint32_t bitn2mask(uint8_t  n) { return (0x01 << n); }
inline uint8_t  mask2bitn(uint32_t m) {
    uint8_t n = 0;	while ((m >>= 1)) ++n;		return n;
}

inline void dump_data(std::ostream& os, uint8_t* data, uint16_t size) {
    uint16_t i, j;

    os  << std::setfill('0') << std::hex;

    for (i = 0u; i < size; ++i) {
	if (!(j = i % 16u)) os << "\n    "
		    << std::setw(4) << i << ':' << ' ';

	os  << ' '  << std::setw(2) << (unsigned)data[i];

	if ((i % 8u) == 7u) os << ' ';

	if (j == 15u) {
	    os << ' ' << '|';

	    for (j = i - 15u; j <= i; ++j)
		os << char(std::isprint(data[j]) ? data[j] : '.');

	    os << '|';
	}
    }

    if ((j = i % 16u)) {
	for (; j < 16u; ++j) os << (((j % 8u) == 7u) ? "    " : "   ");

	os << ' ' << '|';

	for (j = i - (i % 16u); j < i; ++j)
	    os << char(std::isprint(data[j]) ? data[j] : '.');

	for (j = i % 16u; j < 16u; ++j) os << ' ';

	os << '|';
    }

    os << std::setfill(' ') << std::dec << std::endl;
}

};

#endif//TYPE_HPP
// vim:sts=4:ts=8:
