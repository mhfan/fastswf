//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: bs.hpp         Wed, 05 Apr 2006 14:20:59 +0800  mhfan $ *
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
#ifndef BS_HPP
#define BS_HPP

#include <fstream>

#define HAVE_BYTESWAP_H	1	// XXX:

#include "common.h"
//#include "bswap.h"

#ifndef	UNSTREAMING_READ
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#endif

const bool SIGNED = true, UNSIGN = false;

// TODO: implement non-streaming buffered IO

struct BitStream {
    uint32_t pos;

    ~BitStream() { close(); }

    void close() { is.reset(); }
    void init(std::fstream& fs, bool c) {
#ifndef	UNSTREAMING_READ
	if (c) is.push(boost::iostreams::zlib_decompressor());
	is.push(fs);
#else
#endif
	align();	pos = 0u;
    }

    bool eof() { return is.eof() || is.fail(); }
    void align() { curr.bmsk = 0x00; }		// FIXME:
    void ignore(int32_t n) { is.ignore(n);	pos += n;	align(); }
    uint32_t read(char* buf, uint32_t n) {	// XXX:
	is.read(buf, n);			pos += n;	return n;
    }
    uint32_t read(uint8_t n, bool s) {
	uint32_t val = 0x00;	assert(n < 33u);
#if 0
	for (uint32_t bit = (0x01 << (n - 1u)); bit; bit >>= 1) {
	    if (curr.bmsk == 0x00)  {	++pos;
		curr.byte = is.get();
		curr.bmsk = (0x01 << 7);
	    }
	    if (curr.bmsk & curr.byte) val |= bit;
		curr.bmsk >>= 1;
	}
#else// big-endian bit order
	for (uint8_t bits = n; 0u < bits;) {
	    if (curr.bits == 0u) {	curr.bits =  8u;
		curr.byte = is.get();	++pos;	// XXX:
	    }
	    if (curr.bits < bits) {
		val |= ((uint32_t)curr.byte << (bits -= curr.bits));
		curr.bits = 0u;
	    } else {
		val |= (curr.byte >> (curr.bits -= bits));
		curr.byte &= (0x01 << curr.bits) - 1u;	bits = 0u;
	    }
	}
#endif
	return (s ? signext(val, n) : val);
    }

    BitStream& operator>>( uint8_t& v) {
	align();	pos += sizeof(v);
	is.read((char*)&v, sizeof(v));	return *this;
    }
    BitStream& operator>>(  int8_t& v) {
	align();	pos += sizeof(v);
	is.read((char*)&v, sizeof(v));	return *this;
    }
    BitStream& operator>>(uint16_t& v) {
	align();	pos += sizeof(v);
	is.read((char*)&v, sizeof(v));
#if	__BYTE_ORDER == __BIG_ENDIAN
	v = bswap_16(v); // XXX: le2ne_16()
#endif
	return *this;
    }
    BitStream& operator>>( int16_t& v) {
	align();	pos += sizeof(v);
	is.read((char*)&v, sizeof(v));
#if	__BYTE_ORDER == __BIG_ENDIAN
	v = bswap_16(v); // XXX: le2ne_16()
#endif
	return *this;
    }
    BitStream& operator>>(uint32_t& v) {
	align();	pos += sizeof(v);
	is.read((char*)&v, sizeof(v));
#if	__BYTE_ORDER == __BIG_ENDIAN
	v = bswap_32(v); // XXX: le2ne_32()
#endif
	return *this;
    }
    BitStream& operator>>( int32_t& v) {
	align();	pos += sizeof(v);
	is.read((char*)&v, sizeof(v));
#if	__BYTE_ORDER == __BIG_ENDIAN
	v = bswap_32(v); // XXX: le2ne_32()
#endif
	return *this;
    }
    BitStream& operator>>(   float& v) {
	align();	pos += sizeof(v);
	is.read((char*)&v, sizeof(v));
#if	__BYTE_ORDER == __BIG_ENDIAN
	v = bswap_32(v); // XXX: le2ne_32()
#endif
	return *this;
    }

    BitStream& operator>>(std::string& str) {	align();
	for (++pos; (curr.byte = is.get()); ++pos)
	    str.push_back((char)curr.byte);	return *this;
    }

private:
    uint32_t signext(uint32_t val, uint8_t nb) {
	return ((val & (1 << (nb - 1))) ? (val | (-1 << nb)) : val);
    }
#define	EXTEND_SIGN(val, nb) \
	(val = ((val & (1 << (nb - 1))) ? (val | (-1 << nb)) : val))

#ifndef	UNSTREAMING_READ
    boost::iostreams::filtering_istream is;
#else
#endif

    struct { uint8_t byte; union { uint8_t bmsk, bits; }; } curr;
};

#if 1//def	ZIP_WRAP
#include <zlib.h>

inline int inflate_wrapper(char* src, uint32_t len, void* dst, uint32_t siz)
{
    int ret;	z_stream zs;		zs.opaque = (voidpf)0;
    zs.zalloc = (alloc_func)0;		zs.zfree  =  (free_func)0;

    zs.avail_in  = (uInt)len;		zs.next_in   = (Byte*)src;
    zs.avail_out = (uInt)siz;		zs.next_out  = (Byte*)dst;

    if ((ret = inflateInit(&zs)) != Z_OK) return ret;
    for (;;) {
	if ((ret = inflate(&zs, Z_SYNC_FLUSH)) == Z_OK) break;
	if (ret != Z_OK) break;
    }

    if ((ret = inflateEnd (&zs)) != Z_OK) return ret;

    return Z_OK;
}

inline int inflate_wrapper(BitStream& bs, void* buf, uint32_t size)
{
    int ret;	char byte[1];		z_stream zs;

    zs.opaque = (voidpf)0;
    zs.zalloc = (alloc_func)0;		zs.next_out  = (Byte*)buf;
    zs.zfree  =  (free_func)0;		zs.avail_out = (uInt)size;

    if ((ret = inflateInit(&zs)) != Z_OK) return ret;
    for (;;) {
	bs.read((char*)(zs.next_in  = (Byte*)byte),
			zs.avail_in = sizeof(byte));
	if ((ret = inflate(&zs, Z_SYNC_FLUSH)) == Z_OK) break;
	if (ret != Z_OK) break;
    }
    if ((ret = inflateEnd (&zs)) != Z_OK) return ret;

    return Z_OK;
}
#endif

#endif//BS_HPP
// vim:sts=4:ts=8:
