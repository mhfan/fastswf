//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: sound.hpp      Thu, 06 Apr 2006 14:43:44 +0800  mhfan $ *
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
#ifndef SOUND_HPP
#define SOUND_HPP

extern "C" {

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/soundcard.h>

#include <mad.h>
};

namespace SWF {

#define DEFAULT_DSP_PATH		"/dev/dsp"

struct SoundMixer {
    uint32_t fmt;
    uint16_t rate;

    ~SoundMixer() { close(fd); }
     SoundMixer(const char* path = DEFAULT_DSP_PATH) {
	if (fd < 0 && (fd = open(path, O_WRONLY)) < 0)
	    fprintf(stderr, "%s: %s\n", path, strerror(errno));
	else ioctl(fd, SNDCTL_DSP_SYNC, 0);	// XXX:
    }

    void config(uint32_t r = 44100u, bool s16le = true) {
	uint32_t f = (s16le ? AFMT_S16_LE : AFMT_U8);
	if (fmt  != f && (ioctl(fd, SNDCTL_DSP_SETFMT, &f) < 0 || fmt  != f))
	    fprintf(stderr, "To set sound format(%x): %s\n"
			  , f, strerror(errno));
	if (rate != r && (ioctl(fd, SNDCTL_DSP_SPEED , &r) < 0 || rate != r))
	    fprintf(stderr, "To set sound speed (%d): %s\n"
			  , r, strerror(errno));
	fprintf(stdout, "Sound device configured as %dHz stereo %s\n",
		(rate = r), ((fmt  = f) == AFMT_U8 ? "U8" : "S16LE"));
    }

    uint32_t play(const uint8_t* buf, uint32_t len) {
	uint32_t m = 0u;
	for (int n = 0; 0u < len; ) {
	    if ((n = write(fd, buf, len)) < 0) {
		if (errno == EINTR) continue; else {
		    fprintf(stderr, "Write failed: %s\n"
				  , strerror(errno));	break;
		}
	    }	buf += n;	len -= n;   m += n;
	}   return m;
    }

    void stop() {
	if (ioctl(fd, SNDCTL_DSP_RESET, 0) < 0)
	    fprintf(stderr, "To stop sound: %s\n", strerror(errno));
    }

private:
    static int fd;
};

struct Audio {
    uint8_t* ibuf;
    uint32_t ilen, olen;

    Audio(): ibuf(NULL), ilen(0u), olen(0u) { }

    void decode(uint8_t flag);

    static const uint8_t STEREO = 0x01, S16LE = 0x02;
    static const uint16_t OUTPUT_BURST = 8192u,
		 ADPCM_BLOCK_SIZE      = 4096u,
		 MP3_FRAME_PCM_MAX_LEN = 4608u,	// XXX:
		 OUTPUT_BUFFER_SIZE    = OUTPUT_BURST + MP3_FRAME_PCM_MAX_LEN;
    static int16_t obuf[OUTPUT_BUFFER_SIZE];	// XXX:
};

struct AudioPCM: public Audio {
    void decode(uint8_t flag) {
	int16_t* pd = obuf + olen;
	if (flag & S16LE) {
	    if (flag & STEREO); else
	    for (int16_t*ps = (int16_t*)ibuf; ilen--; )
		*pd++ = *ps, *pd++ = *ps++;
	} else { int8_t* ps = (int8_t *)ibuf;
	    if (flag & STEREO) while (ilen--) *pd++ = (*ps++ << 8); else
	    while (ilen--) {
		int16_t smpl = (*ps++ << 8);
		*pd++ = smpl,   *pd++ = smpl;
	    }
	}
    }
};

struct AudioADPCM: public Audio {
    bool eod() { return (!(bitn && (uint32_t)(bytp - ibuf) < ilen)); }

    uint32_t get_bits(uint8_t n, bool s) {
	uint32_t val = 0x00;	assert(0u < n && n < 33u);
	for (uint8_t m = n; 0u < m;) {
	    if (bitn == 0u) {	bitn = 8u;	++bytp; }
	    if (bitn < m) {
		val |= ((uint32_t)*bytp << (m -= bitn));     bitn = 0u;
	    } else {
		val |= (*bytp >> (bitn -= m));
		*bytp &= (0x01 << bitn) - 1u;			m = 0u;
	    }
	}   return (s ? EXTEND_SIGN(val, n) : val);
    }

    void decode(uint8_t flag) {
    static const   int8_t it5[16] = { -1, -1, -1, -1, -1, -1, -1, -1,
				       1,  2,  4,  6,  8, 10, 13, 16 };
    static const   int8_t it4[ 8] = { -1, -1, -1, -1,  2,  4,  6,  8 };
    static const   int8_t it3[ 4] = { -1, -1,  2,  4 };
    static const   int8_t it2[ 2] = { -1,  2 };
    static const uint16_t sst[89] = {
	    7,     8,     9,    10,    11,    12,    13,    14,    16,    17,
	   19,    21,    23,    25,    28,    31,    34,    37,    41,    45,
	   50,    55,    60,    66,    73,    80,    88,    97,   107,   118,
	  130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
	  337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
	  876,   963,  1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066,
	 2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
	 5894,  6484,  7132,  7845,  8630,  9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
    };	static const int8_t* its[] = { it2, it3, it4, it5 };

    extern const bool SIGNED, UNSIGN;

    uint8_t nb = get_bits(2u, UNSIGN), k0 = (0x01 << nb)
	    , sm = (0x01 << (nb + 1)), st = 1u + ((flag & STEREO) ? 1u : 0u);
    const int8_t* ixt = its[nb];
    int16_t* optr = obuf + olen;

    if (st < 2u) for (nb += 2; !eod(); ) {
	if (((olen += 2) & (0xff << 1)) == 1u) {
	    // wrap around at every 4096 samples...
	    valp[0] = get_bits(16u, SIGNED);
	    sidx[0] = get_bits( 6u, UNSIGN);
	} else {
	     uint8_t delt = get_bits(nb, UNSIGN), k = k0,
		     step = sst[sidx[0]], vpdf = 0u;
	     do {   if (delt & k) vpdf += step;	step >>= 1;
	     } while ((k >>= 1)); vpdf += step;
	     // vpdf = (delt + 0.5) * step / 4

	     if (delt & sm) valp[0] -= vpdf; else valp[0] += vpdf;

	     sidx[0] += ixt[(delt & (~sm))];
	     //if (sidx[0] <  0u) sidx[0] =  0u; else		// XXX:
	     if (88u < sidx[0]) sidx[0] = 88u;

	     //if (valp[0] < SHRT_MIN) valp[0] = SHRT_MIN; else
	     //if (SHRT_MAX < valp[0]) valp[0] = SHRT_MAX;	// XXX:
	}   *optr++ = valp[0], *optr++ = valp[0];
    } else for (nb += 2; !eod(); ) {
	if (((olen += 2) & (0xff << 1)) == 1u) {
	    // wrap around at every 4096 samples...
	    for (uint8_t i=0u; i < st; ++i) {
		*optr++ = valp[i] = get_bits(16u, SIGNED);
			  sidx[i] = get_bits( 6u, UNSIGN);
	    }
	} else	// XXX:
	for (uint8_t i=0u; i < st; ++i) {
	     uint8_t delt = get_bits(nb, UNSIGN), k = k0,
		     step = sst[sidx[i]], vpdf = 0u;
	     do {   if (delt & k) vpdf += step;
		    step >>= 1;
	     } while ((k >>= 1)); vpdf += step;
	     // vpdf = (delt + 0.5) * step / 4

	     if (delt & sm) valp[i] -= vpdf; else valp[i] += vpdf;

	     sidx[i] += ixt[(delt & (~sm))];
	     //if (sidx[i] <  0u) sidx[i] =  0u; else		// XXX:
	     if (88u < sidx[i]) sidx[i] = 88u;

	     //if (valp[i] < SHRT_MIN) valp[i] = SHRT_MIN; else
	     //if (SHRT_MAX < valp[i]) valp[i] = SHRT_MAX;	// XXX:

	     *optr++ = valp[i];
	}
    }
    }

private:
    uint8_t* bytp;
    uint8_t  bitn;

    uint8_t  sidx[2];
    int16_t  valp[2];
};

struct AudioMP3: public Audio {
    ~AudioMP3() {
	mad_synth_finish (&synth);
	mad_frame_finish (&frame);
	mad_stream_finish(&stream);
    }
     AudioMP3() {
	mad_synth_init (&synth);
	mad_frame_init (&frame);
	mad_stream_init(&stream);
    }

    inline int16_t scale(mad_fixed_t smpl) {
	smpl += (1L << (MAD_F_FRACBITS - 16));			// round

	if (smpl >= MAD_F_ONE) smpl =  MAD_F_ONE - 1; else
	if (smpl < -MAD_F_ONE) smpl = -MAD_F_ONE;		// clip

	return (smpl >> (MAD_F_FRACBITS + 1 - 16));		// quantize
    }

    void decode(uint8_t flag) {
	    mad_stream_buffer(&stream, ibuf, ilen);
    for (int16_t* optr = obuf + olen; olen < OUTPUT_BURST; ) {
	if (mad_frame_decode (&frame, &stream)) {
	    switch (stream.error) {
	    case MAD_ERROR_BUFLEN:	break;
	    default: std::cerr << "MP3: "	// XXX:
		    << mad_stream_errorstr(&stream) << std::endl;
	    }
	} else {
	    mad_synth_frame  (&synth, &frame);

	    struct mad_pcm    *pcm = &synth.pcm;
	    mad_fixed_t const *lch = pcm->samples[0],
			      *rch = pcm->samples[1];

	    if (1u < pcm->channels)
		for (uint32_t i = 0; i < pcm->length; ++i)
		    *optr++ = scale(*lch++), *optr++ = scale(*rch++); else
		for (uint32_t i = 0; i < pcm->length; ++i) {	// XXX:
		    int16_t smpl = scale(*lch++);
		    *optr++ = smpl, *optr++ = smpl;
		}

	    olen += (pcm->length << 2);	// 2ch, s16le
	}
    }	    if (stream.next_frame)
		memmove(ibuf, stream.next_frame, (ilen =
			ibuf + ilen - (uint8_t*)stream.next_frame));
    }

private:
#if 0
    struct mad_decoder decoder;
#else
    struct mad_frame  frame;
    struct mad_synth  synth;
    struct mad_stream stream;
#endif
};

};

#endif//SOUND_HPP
// vim:sts=4:ts=8:
