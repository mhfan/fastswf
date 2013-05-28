//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: swf.hpp        Wed, 05 Apr 2006 08:38:08 +0800  mhfan $ *
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
#ifndef SWF_HPP
#define SWF_HPP

#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <vector>
#include <map>

#include <sys/time.h>
#include <stdint.h>
#include <time.h>

#include "bs.hpp"
#include "tag.hpp"
#include "type.hpp"
#include "jpeg.hpp"

#include "sound.hpp"
#include "action.hpp"
#include "render.hpp"

namespace SWF {

/*
 * Alexis' SWF Reference:
 *  http://www.m2osw.com/swf_alexref.html
 *  http://sswf.sourceforge.net/SWFalexref.html
 *
 * Macromedia Flash File Format (SWF):
 *  http://www.half-serious.com/swf/format/index.html
 *
 * SWF File Reference:
 *  http://homepages.tig.com.au/~dkl/swf/SWFfileformat.html
 *  http://homepages.tig.com.au/~dkl/swf/SWFfilereference.html
 *
 * SWF File Format Specification:
 *  http://www.the-labs.com/MacromediaFlash/SWF-Spec/SWFfileformat.html
 *  http://www.adobe.com/devnet/swf.html
 *  http://www.adobe.com/devnet/f4v.html
 *  http://www.adobe.com/devnet/rtmp.html
 *
 * ActionScript Virtual Machine 2 (AVM2) overview
 *  http://www.adobe.com/devnet/actionscript.html
 *
 *  http://www.adobe.com/support/documentation/en/flash/
 */

/*
 * Introduction:
 *
 * SWF: ShockWave Flash
 *
 * The SWF (pronounced 'swiff') file format delivers vector graphics, text,
 * video, and sound over the Internet and is supported by Adobe(R) Flash(R)
 * Player software. The SWF file format is designed to be an efficient
 * delivery format, not a format for exchanging graphics between
 * graphics editors. It is designed to meet the following goals:
 *
 *  * On-screen display - The format is primarily intended for on-screen
 *  display and supports anti-aliasing, fast rendering to a bitmap of any color
 *  format, animation, and interactive buttons.
 *
 *  * Extensibility - The format is a tagged format, so it can be evolved with
 *  new features while maintaining backward compatibility with earlier versions
 *  of Flash Player.
 *
 *  * Network delivery - The format can travel over a network with limited and
 *  unpredictable bandwidth. The files are compressed to be small and support
 *  incremental rendering through streaming. The SWF file format is a binary
 *  format and is not human readable like HTML. The SWF file format uses
 *  techniques such as bit-packing and structures with optional fields to
 *  minimize file size.
 *
 *  * Simplicity - The format is simple so that Flash Player is small and
 *  easily ported. Also, Flash Player depends upon a limited set of operating
 *  system features only.
 *
 *  * File independence - The files display with minimal dependence on external
 *  resources such as fonts.
 *
 *  * Scalability - The files work well on limited hardware, and can take
 *  advantage of better hardware when it is available. This ability is
 *  important because computers have different monitor resolutions and bit
 *  depths.
 *
 *  * Speed - The graphics described by SWF files render quickly.
 *
 *  * Scriptability - The format includes tags that provide sequences of byte
 *  codes to be interpreted by a stack machine. The byte codes support the
 *  ActionScript(R) language. Flash Player provides a runtime ActionScript
 *  object model that allows interaction with drawing primitives, servers, and
 *  features of Flash Player.
 *
 * SWF files have the extension .swf and a MIME type of
 * application/x-shockwave-flash.
 *
 * The SWF format has evolved through several versions. Through SWF 5,
 * substantial additions were made to the SWF tag set. Starting with SWF 6 and
 * later, the SWF format changes less, as more new features are implemented
 * partly or entirely at the ActionScript level. Starting with SWF 9, the
 * ActionScript 3.0 language, which employs the new ActionScript Virtual
 * Machine 2 (AVM2) can be used. Anyone planning to generate SWF file content
 * that uses newer features should become familiar with the ActionScript object
 * model that Flash Player exposes. Some references for this information are
 * Programming ActionScript 3.0
 * (see www.adobe.com/go/learn_fl_cs4_programmingAS3_en),
 * ActionScript 3.0 Language Reference
 * (see www.adobe.com/go/learn_flashcs4_langref_en), and the Adobe ActionScript
 * Virtual Machine 2 Overview (PDF file) at www.adobe.com/go/avm2overview.
 *
 */

/*
 * A SWF file is made up of a header, followed by a number of tags. The two
 * types of tags are definition tags and control tags.
 *
 * Definition tags define the objects known as characters, which are stored in
 * the dictionary.
 *
 * Control tags manipulate characters, and control the flow of the file.
 */

struct FileHeader {
    union {
	struct {
	    uint32_t mgc:24, ver:8;	// XXX: regarding endianess
	};  uint32_t __;	uint8_t _[4];	// { 'C', 'W', 'S', 8 }
    };	    uint32_t len;

    /* The FileLength field is the total length of the SWF file, including the
     * header. If this is an uncompressed SWF file (FWS signature), the
     * FileLength field should exactly match the file size. If this is a
     * compressed SWF file (CWS signature), the FileLength field indicates the
     * total length of the file after decompression, and thus generally does
     * not match the file size. Having the uncompressed size available can
     * make the decompression process more efficient.
     */

    enum {  MAX_SWF_VERSION		= 10u,
	    MAGIC_SIGNATURE_SWC		= 'SWC',
	    MAGIC_SIGNATURE_SWF		= 'SWF',
	    MAGIC_SIGNATURE_COMPRESSED  = MAGIC_SIGNATURE_SWC,
	    MAGIC_SIGNATURE		= MAGIC_SIGNATURE_SWF,
    };

    std::fstream& load(std::fstream& fs) {
	fs.read((char*)_, sizeof(_));
	fs.read((char*)&len, sizeof(len));
#if	__BYTE_ORDER == __BIG_ENDIAN
	len = bswap_32(len);	// XXX: le2ne_32()
#endif
	return fs;
    }
};

struct MovieHeader {
    Rect fs;

    union {
	uf16_8_t fr;
	uint16_t f_;
    };  uint16_t fc;

    BitStream& load(BitStream& bs) { return bs >> fs >> f_ >> fc; }

    //void exec() { render.SetViewport(fs); }

    void dump(std::ostream& os) {
	os  << "Movie:  " << fs << " twips; "	 << std::setw(5) << f_
	    << "/256=" << f_ / 256.f << " fps; " << std::setw(6) << fc
	    << " frames" << std::endl;
    }

    friend BitStream& operator>>(BitStream& bs, MovieHeader& mh)
	    { return mh.load(bs); }
};

/*
 * Following the header is a series of tagged data blocks. All tags share a
 * common format, so any program parsing a SWF file can skip over blocks it
 * does not understand. Data inside the block can point to offsets within the
 * block, but can never point to an offset in another block. This ability
 * enables tags to be removed, inserted, or modified by tools that process a
 * SWF file. The FileAttributes tag is only required for SWF 8 and later.
 */

/* Tag ordering in SWF files:
 *
 * Generally speaking, tags in a SWF can occur in any order. However, you must
 * observe the following rules:
 *
 *  * The FileAttributes tag must be the first tag in the SWF file for SWF 8
 *  and later.
 *
 *  * A tag should only depend on tags that come before it. A tag should never
 *  depend on a tag that comes later in the file.
 *
 *  * A definition tag that defines a character must occur before any control
 *  tag that refers to that character.
 *
 *  * Streaming sound tags must be in order. Out-of-order streaming sound tags
 *  result in the sound being played out of order.
 *
 *  * The End tag is always the last tag in the SWF file.
 */

/* Processing a SWF file:
 *
 * Flash Player processes all of the tags in a SWF file until a ShowFrame tag
 * is encountered. At this point, the display list is copied to the screen and
 * Flash Player is idle until it is time to process the next frame. The
 * contents of the first frame are the cumulative effect of performing all of
 * the control tag operations before the first ShowFrame tag. The contents of
 * the second frame are the cumulative effect of performing all of the control
 * tag operations from the beginning of the file to the second ShowFrame tag,
 * and so on.
 */

/* File compression strategy:
 *
 * Since SWF files are frequently delivered over a network connection, they
 * should be as compact as possible. Several techniques are used to accomplish
 * this, including the following items:
 *
 *  * Reuse - The structure of the character dictionary makes it easy to reuse
 *  elements in a SWF file. For example, a shape, button, sound, font, or
 *  bitmap can be stored in a file once and referenced many times.
 *
 *  * Compression - Shapes are compressed by using an efficient delta encoding
 *  scheme; often the first coordinate of a line is assumed to be the last
 *  coordinate of the previous line. Distances are also often expressed
 *  relative to the last position.
 *
 *  * Default values - Some structures, like matrixes and color transforms,
 *  have common fields that are used more often than others. For example, for
 *  a matrix, the most common field is the translation field. Scaling and
 *  rotation are less common. Therefore, if the scaling field is not present,
 *  it is assumed to be 100%. If the rotation field is not present, it is
 *  assumed that there is no rotation. This use of default values helps to
 *  minimize file sizes.
 *
 *  * Change Encoding - As a rule, SWF files only store the changes between
 *  states. This is reflected in shape data structures and in the
 *  place-move-remove model that the display list uses.
 *
 *  * Shape Data Structure - The shape data structure uses a unique structure
 *  to minimize the size of shapes and to render anti-aliased shapes
 *  efficiently on the screen.
 */

/* Displaying a frame of a SWF file is a three-stage process:
 *
 *  1. Objects are defined with definition tags such as DefineShape,
 *  DefineSprite, and so on. Each object is given a unique ID called a
 *  character, and is stored in a repository called the dictionary.
 *
 *  2. Selected characters are copied from the dictionary and placed on the
 *  display list, which is the list of the characters that will be displayed
 *  in the next frame.
 *
 *  3. Once complete, the contents of the display list are rendered to the
 *  screen with ShowFrame.
 *
 * A depth value is assigned to each character on the display list. The depth
 * determines the stacking order of the character. Characters with lower depth
 * values are displayed underneath characters with higher depth values. A
 * character with a depth value of 1 is displayed at the bottom of the stack.
 * A character can appear more than once in the display list, but at different
 * depths. Only one character can be at any given depth.
 *
 * In SWF 1 and 2, the display list was a flat list of the objects that are
 * present on the screen at any given time. In SWF 3 and later versions, the
 * display list is a hierarchical list where an element on the display can
 * have a list of child elements. For more information, see DefineSprite.
 *
 * The following six tags are used to control the display list:
 *
 *  * PlaceObject Adds a character to the display list.
 *
 *  * PlaceObject2 & PlaceObject3 Adds a character to the display list,
 *  or modifies the character at the specified depth.
 *
 *  * RemoveObject Removes the specified character from the display list.
 *
 *  * RemoveObject2 Removes the character at the specified depth.
 *
 *  * ShowFrame Renders the contents of the display list to the display.
 */

/*
 * The following is a step-by-step procedure for creating and displaying a
 * Macromedia Flash animation:
 *
 * 1. Define each object with a DefineShape , DefineText , DefineSprite or
 * DefineMorphShape tag. Each object is given a unique character ID, and added
 * to the dictionary.
 *
 * 2. Add each character to the display list with a PlaceObject2 tag. Each
 * PlaceObject2 tag specifies the character to be displayed, plus the
 * following attributes:
 *
 *   1). A depth value. This controls the stacking order of the character
 *   being placed. Characters with lower depth values appear to be underneath
 *   characters with higher depth values. A depth value of 1 means the
 *   character is displayed at the bottom of the stack. There can be only one
 *   character at any given depth.
 *
 *   2). A transformation matrix. This determines the position, scale, factor,
 *   and angle of rotation of the character being placed. The same character
 *   may be placed more than once (at different depths) with a different
 *   transformation matrix.
 *
 *   3). An optional color transform. This specifies the color effect applied
 *   to the character being placed. Color effects include transparency and
 *   color shifts.
 *
 *   4). An optional name string. This identifies the character being placed
 *   for SetTarget actions. SetTarget is used to perform actions inside sprite
 *   objects.
 *
 *   5). An optional ClipDepth value. This specifies the top-most depth that
 *   will be masked by the character being placed.
 *
 *   6). An optional ratio value. This controls how a morph character is
 *   displayed when placed. A ratio of zero displays the character at the
 *   start of the morph. A ratio of 65535 displays the character at the end of
 *   the morph.
 *
 * 3. Render the contents of the display list to the screen with a ShowFrame
 * tag.
 *
 * 4. Modify each character on the Display List with a PlaceObject2 tag. Each
 * PlaceObject2 assigns a new transformation matrix to the character at a
 * given depth. (The character ID is not specified because there can be only
 * one character for each depth).
 *
 * 5. Display the characters in their new positions with a ShowFrame tag.
 * Repeat steps 4 and 5 for each frame of the animation. Note: if a character
 * does not change from frame to frame, there is no need to re-place the
 * unchanged character after each frame.
 *
 * 6. Remove each character from the display list with a RemoveObject2 tag.
 * Only the depth value is required to identify the character being removed.
 */

struct TimeLine {
    uint32_t cu;
    //uint32_t ct;	    // = fn * 1000000u * 256u / fr;
    //uint32_t tl;	    // = fc * 1000000u * 256u / fr;
    struct timeval tb;

    uint32_t fresh() {
	static struct timeval tn;	gettimeofday(&tn, NULL);
	cu = (tn.tv_sec - tb.tv_sec) * 1000000u + tn.tv_usec - tb.tv_usec;
	return cu;
    }

    void start() { gettimeofday(&tb, NULL);	cu = 0u; }

    void delay(uint32_t ct) { fresh(); if (cu < ct) usleep(ct - cu); }
};

struct EventHandler { };


struct Flash {
    enum {  LOAD_BY_TAG, LOAD_BY_FRAME, LOAD_WHOLE,
	    PLAY_DEFLAUT = 0u, PLAY_ONCE, SKIP_FRAME, };

    FrameRender* fr;	// XXX:
    SoundMixer*  sm;	// XXX:

     Flash();
    ~Flash() { delete fr; }

    void cloz() {
	bs.close();		fs.close();
	for (std::vector<Tag*>::iterator it = ts.begin();
		it != ts.end(); ++it) delete *it;
	ts.clear();		fs.clear();
	dl.clear();		di.clear();
	fl.clear();
    }

    void advf() {
	assert(ControlTag::fl->pf < ControlTag::fl->size());
	std::vector<ControlTag*>& fm =
		(*ControlTag::fl)[ControlTag::fl->pf++];
	for (std::vector<ControlTag*>::iterator it = fm.begin();
		it != fm.end(); ++it) (*it)->exec();
    }

    void show() {
	for (DispList::iterator it = dl.begin();
		it != dl.end(); ++it) it->first->show(it->second);
    }

    void dump(std::ostream& os) {
	os  << "Frame #" << std::left << std::setw(4) << ControlTag::fl->pf
	    << " with display list("  << std::setw(4)
	    << dl.size() - 1 << " objects): \n";
	for (DispList::iterator it = dl.begin();
		++it != dl.end(); ) {
	    os  << "  Object("  << it->first->code << ")\t#"
		<< std::setw(4) << it->first->id;
	    if (it->second) os  << " on depth #"
		<< std::setw(4) << it->second->depth;
	    os  << std::endl;
	}   os  << std::right;
    }

    bool open(const char* fn);
    void load(uint8_t ht = LOAD_BY_TAG);
    void play(uint8_t flag = PLAY_ONCE, int8_t sp = 1);

private:
     FileHeader fh;
    MovieHeader mh;

    EventHandler eh;	// XXX:
    ActionScript as;	// XXX:
    StackMachine SM;	// XXX:

    std::vector<Tag*> ts;

    Dictionary di;
    FrameList  fl;
    DispList   dl;

    TimeLine  tl;
    BitStream bs;
    std::fstream fs;
    std::string  rl;
};

};

#endif//SWF_HPP
// vim:sts=4:ts=8:
