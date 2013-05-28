//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: tag.hpp        Wed, 05 Apr 2006 14:27:22 +0800  mhfan $ *
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
#ifndef TAG_HPP
#define TAG_HPP

#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <vector>
#include <map>

#include "bs.hpp"
#include "type.hpp"
#include "jpeg.hpp"

#include "sound.hpp"
#include "action.hpp"
#include "render.hpp"

namespace SWF {

namespace TAG {

namespace CODE {

enum TagCode {

    /* References:
     *	http://www.m2osw.com/swf_tags
     *	SWF File Format Specification
     */

#define UnusedTag(n)	UnusedTag_##n = n

    //FileHeader			= -1,
    /* Format:	v1
     * Although it isn't a tag per say, we consider the file header as being
     * a tag because it represents a block in the flash file. Since version 8,
     * it can be complemented by the FileAttributes tag.
     */

    End				=  0,
    /* Format:	v1
     * Mark the end of the file or a Sprite. It can't appear anywhere else but
     * the end of the file or a Sprite.
     */

    ShowFrame			=  1,
    /* Display:	v1
     * Display the current display list and pauses for 1 frame as defined in
     * the file header.
     */

    DefineShape			=  2,
    /* Define:	v1
     * Define a simple geometric shape.
     */

    FreeCharacter		=  3,
    /* Define:	v1, Unknown
     * Release a character which won't be used in this movie anymore.
     */

    PlaceObject			=  4,
    /* Display: v1
     * Place the specified object in the current display list.
     */

    RemoveObject		=  5,
    /* Display: v1
     * Remove the specified object at the specified depth.
     */

    DefineBits			=  6,
    /* Define:	v1
     * Define a JPEG bit stream.
     */

    DefineButton		=  7,
    /* Define:	v1
     * Define an action button.
     */

    JPEGTables			=  8,
    /* Define:	v1
     * Define the tables used to compress/decompress all the SWF 1.0 JPEG
     * images (See also DefineBitsJPEG.)
     */

    SetBackgroundColor		=  9,
    /* Display: v1
     * Change the background color.
     */

    DefineFont			= 10,
    /* Define:	v1
     * List shapes corresponding to glyphs.
     */

    DefineText			= 11,
    /* Define:	v1
     * Defines a text of characters displayed using a font. This definition
     * doesn't support any transparency.
     */

    DoAction			= 12,
    /* Action:	v1
     * Actions to perform at the time the next show frame is reached and
     * before the result is being displayed. It can duplicate sprites,
     * start/stop movie clips, etc.
     *
     * All the actions within a frame are executed sequentially in the order
     * they are defined.
     *
     * Important: many actions are not supported in Adobe Flash version 1.
     * Please, see the reference of actions below in order to know which
     * actions can be used with which version of Adobe Flash.
     */

    DefineFontInfo		= 13,
    /* Define:	v1
     * Information about a previously defined font. Includes the font style, a
     * map and the font name.
     */

    DefineSound			= 14,
    /* Define:	v2
     * Declare a sound effect. This tag defines sound samples that can later
     * be played back using either a StartSound or a DefineButtonSound. Note
     * that the same DefineSound block can actually include multiple sound
     * files and only part of the entire sound can be played back as required.
     */

    StartSound			= 15,
    /* Display: v2
     * Start playing the referenced sound on the next ShowFrame.
     */

    StopSound			= 16,
    /* Display:	v2, Unknown
     * Start playing the referenced sound on the next ShowFrame.
     */

    DefineButtonSound		= 17,
    /* Define:	v2
     * Defines how to play a sound effect for when an event occurs on the
     * referenced button.
     */

    SoundStreamHead		= 18,
    /* Define:	v2
     * Declare a sound effect which will be interleaved with a movie data so
     * as to be loaded over a network connection while being played.
     */

    SoundStreamBlock		= 19,
    /* Define:	v2
     * A block of sound data (i.e. audio samples.) The size of this block of
     * data will be defined in the previous SoundStreamHead tag. It is used to
     * download sound samples on a per frame basis instead of all at once.
     */

    DefineBitsLossless		= 20,
    /* Define:	v2
     * A bitmap compressed using ZLIB (similar to the PNG format).
     */

    DefineBitsJPEG2		= 21,
    /* Define:	v2
     * Defines a complete JPEG image (includes the bit stream and the tables
     * all in one thus enabling multiple tables to be used within the same SWF
     * file).
     *
     * Since Flash version 10, the data can also be set to a valid PNG or
     * GIF89a. There is no need to specify the image format in the tag since
     * the data describing the image includes the necessary information.
     */

    DefineShape2		= 22,
    /* Define:	v2
     * Define a simple geometric shape.
     */

    DefineButtonCXForm		= 23,
    /* Define:	v2
     * Setup a color transformation for a button.
     */

    Protect			= 24,
    /* Format:	v2
     * Disable edition capabilities of the given SWF file. Though this doesn't
     * need to be enforced by an SWF editor, it marks the file as being
     * copyrighted in a way.
     */

    PathsArePostscript		= 25,
    /* Define:	v3, Unknown
     * The shape paths are defined as in postscript?
     *
     * Apparently there was some testing with using Postscript like
     * instructions to render shapes. I support that is close to the time when
     * the ActionScript language was not yet fully functional. The content of
     * this tag is not described anywhere and is more than likely not
     * supported in newer versions.
     */

    PlaceObject2		= 26,
    /* Display: v3
     * Place, replace, remove an object in the current display list.
     */

    UnusedTag(27),

    RemoveObject2		= 28,
    /* Display: v3
     * Remove the object at the specified level. This tag should be used in
     * movies version 3 and over since it compressed better than the standard
     * RemoveObject tag. Note that a PlaceObject2 can also be used for this
     * task.
     */

    SyncFrame			= 29,
    /* Display:	v3, Unknown
     * Tag used to synchronize the animation with the hardware.
     */

    UnusedTag(30),

    FreeAll			= 31,
    /* Define:	v3, Unknown
     * Probably an action that would be used to clear everything out.
     */

    DefineShape3		= 32,
    /* Define:	v3
     * Define a simple geometric shape.
     */

    DefineText2			= 33,
    /* Define:	v3
     * Defines a text of characters displayed using a font. Transparency is
     * supported with this tag.
     */

    DefineButton2		= 34,
    /* Define:	v3
     * Define an action button. Includes a color transformation.
     */

    DefineBitsJPEG3		= 35,
    /* Define:	v3
     * Defines a complete JPEG image, i.e. a verbatim JPEG image file. The
     * JPEG image is then followed by an alpha channel. Note that the alpha
     * channel uses the Z-lib compression. Since Flash 10, the supported image
     * formats are JPEG, PNG and GIF89a.
     */

    DefineBitsLossless2		= 36,
    /* Define:	v3
     * Defines an RGBA bitmap compressed using ZLIB (similar to the PNG format).
     */

    DefineEditText		= 37,
    /* Define:	v4
     * An edit text enables the end users to enter text in a Flash window.
     */

    DefineVideo			= 38,
    /* Define:	v4, Unknown
     * Apparently, Macromedia did have a first attempt in supporting video on
     * their platform. It looks, however, as if they reconsidered at that
     * point in time.
     */

    DefineSprite		= 39,
    /* Define:	v3
     * Declares an animated character. This is similar to a shape with a
     * display list so the character can be changing on its own over time.
     */

    NameCharacter		= 40,
    /* Define:	v3, Unknown
     * Define the name of an object (for buttons, bitmaps, sprites and sounds).
     */

    ProductInfo			= 41,
    /* Define:	v3, Unknown
     * This tag defines information about the product used to generate the
     * animation. The product identifier should be unique among all the
     * products. The info includes a product identifier, a product edition, a
     * major and minor version, a build number and the date of compilation.
     * All of this information is all about the generator, not the output
     * movie.
     */

    DefineTextFormat		= 42,
    /* Define:	v1, Unknown
     * Another tag that Flash ended up not using.
     */

    FrameLabel			= 43,
    /* Define:	v3
     * Names a frame or anchor. This frame can later be referenced using this
     * name.
     */

    UnusedTag(44),

    SoundStreamHead2		= 45,
    /* Define:	v3
     * Declare a sound effect which will be interleaved with a movie data so
     * as to be loaded over a network connection while being played.
     */

    DefineMorphShape		= 46,
    /* Define:	v3
     * This is similar to a sprite with a simple morphing between two shapes.
     */

    GenerateFrame		= 47,
    /* Define:	v3, Unknown
     * This may have been something similar to a New in an action script and
     * thus was removed later.
     */

    DefineFont2			= 48,
    /* Define:	v3
     * Define a list of glyphs using shapes and other font metric information.
     */

    GeneratorCommand		= 49,
    /* Define:	v3, Unknown
     * Gives some information about the tool which generated this SWF file and
     * its version.
     */

    DefineCommandObject		= 50,
    /* Define:	v5, Unknown
     * ?
     */

    CharacterSet		= 51,
    /* Define:	v5, Unknown
     * It looks like this would have been some sort of DefineSprite
     * extension... did not make it out either.
     */

    ExternalFont		= 52,
    /* Define:	v5, Unknown
     * It looks like accessing a system font was going to be another tag, but
     * instead Macromedia made use of a flag in the existing DefineFont2 tag.
     */

    UnusedTag(53),
    UnusedTag(54),
    UnusedTag(55),

    Export			= 56,
    ExportAssets		= 56,
    /* Define:	v5
     * Exports a list of definitions declared external so they can be used in
     * other movies. You can in this way create one or more movies to hold a
     * collection of objects to be reused by other movies without having to
     * duplicate these in each movie. A single export is enough for an entire
     * movie (and you should have just one).
     */

    Import			= 57,
    ImportAssets		= 57,
    /* Define:	v5
     * Imports a list of definitions that are to be loaded from another movie.
     * You can retrieve objects that were exported in the specified movie. You
     * can have as many import as you like, though you should really only have
     * one per referenced movie.
     */

    ProtectDebug		= 58,
    /* Format:	v5
     * The data of this tag is an MD5 password like the Protect tag. When it
     * exists and you know the password, you will be given the right to debug
     * the movie with Flash V5.x.
     *
     * WARNING: this tag is only valid in Flash V5.x, use the ProtectDebug2
     * instead in V6.x and newer movies and Protect in older movies.
     */

    DoInitAction		= 59,
    /* Action:	v6
     * Actions to perform the first time the following Show Frame tag is
     * reached. All the initialization actions are executed before any other
     * actions. You have to specify a sprite to which the actions are applied
     * to. Thus you don't need a set target action. When multiple
     * initialization action blocks are within the same frame, they are
     * executed one after another in the order they appear in that frame.
     */

    DefineVideoStream		= 60,
    /* Define:	v6
     * Defines the necessary information for the player to display a video
     * stream (i.e. size, codec, how to decode the data, etc.). Play the
     * frames with VideoFrame tags.
     */

    VideoFrame			= 61,
    /* Define:	v6
     * Show the specified video frame of a movie.
     */

    DefineFontInfo2		= 62,
    /* Define:	v6
     * Defines information about a font, like the DefineFontInfo tag plus a
     * language reference. To force the use of a given language, this tag
     * should be used in v6.x+ movies instead of the DefineFontInfo tag.
     */

    DebugID			= 63,
    /* Define:	v6, Unknown
     * This tag is used when debugging an SWF movie. It gives information
     * about what debug file to load to match the SWF movie with the source.
     * The identifier is a UUID.
     */

    ProtectDebug2		= 64,
    /* Format:	v6
     * The data of this tag is a 16 bits word followed by an MD5 password like
     * the Protect tag. When it exists and you know the password, you will be
     * given the right to debug the movie with Flash V6.x and over.
     */

    ScriptLimits		= 65,
    /* Define:	v7
     * Change limits used to ensure scripts do not use more resources than
     * what you choose. In version 7, it supports a maximum recursive depth
     * and a maximum amount of time scripts can be run for in seconds.
     */

    SetTabIndex			= 66,
    /* Define:	v7
     * Define the order index so the player knows where to go next when the
     * Tab key is pressed by the user.
     */

    UnusedTag(67),
    UnusedTag(68),

    FileAttributes		= 69,
    /* Format:	v8
     * Since version 8, this tag is required and needs to be the very first
     * tag in the movie. It is used as a way to better handle security within
     * the Flash Player.
     */

    PlaceObject3		= 70,
    /* Display: v8
     * Place an object in the display list. The object can include bitmap
     * caching information, a blend mode and a set of filters.
     */


    Import2			= 71,
    ImportAssets2		= 71,
    /* Define:	v8
     * Imports a list of definitions from another movie. In version 8+, this
     * tag replaces the original ImportAssets tag. You can retrieve objects
     * which were exported in the specified movie. You can have as many import
     * as you like, although you should really only have one per referenced
     * movie.
     */

    DoABCAction			= 72,
    /* Action:	v9, Unknown
     * New container tag for ActionScripts under SWF 9. Includes only actions.
     */

    DefineFontAlignZones	= 73,
    /* Define:	v8
     * Define advanced hints about a font glyphs to place them on a pixel
     * boundary.
     */

    CSMTextSettings		= 74,
    /* Define:	v8
     * Define whether CSM text should be used in a previous DefineText,
     * DefineText2 or DefineEditText.
     */

    DefineFont3			= 75,
    /* Define:	v8
     * Define a list of glyphs using shapes and other font metric information.
     */

    SymbolClass			= 76,
    /* Action:	v9
     * Instantiate objects from a set of classes.
     */

    Metadata			= 77,
    /* Format:	v8
     * This tag includes XML code describing the movie. The format is RDF
     * compliant to the XMP as defined on W3C.
     */

    DefineScalingGrid		= 78,
    /* Define:	v8
     * Define scale factors for a window, a button, or other similar objects.
     */

    UnusedTag(79),
    UnusedTag(80),
    UnusedTag(81),

    DoABCDefine			= 82,
    DoABC			= 82,
    /* Action:	v9
     * New container tag for ActionScripts under SWF 9. Includes an
     * identifier, a name and actions.
     */

    DefineShape4		= 83,
    /* Define:	v8
     * Declare a shape which supports new line caps, scaling and fill options.
     */

    DefineMorphShape2		= 84,
    /* Define:	v8
     * Declare a morphing shape with attributes supported by version 8+.
     */

    UnusedTag(85),

    DefineSceneAndFrameData	= 86,
    DefineSceneAndFrameLabelData= 86,
    /* Define:	v9
     * Define raw data for scenes and frames.
     */

    DefineBinaryData		= 87,
    /* Define:	v9
     * Defines a buffer of any size with any binary user data.
     */

    DefineFontName		= 88,
    /* Define:	v9
     * Define the legal font name and copyright.
     */

    StartSound2			= 89,
    /* Action:	v9
     * ?
     */

    DefineBitsJPEG4		= 90,
    /* Define:	v10
     * Defines a complete JPEG (the image formats supported are JPEG, PNG and
     * GIF89a) and includes a deblocking filter parameter. The JPEG image is
     * then followed by an alpha channel. Note that the alpha channel uses the
     * Z-lib compression.
     */

    DefineFont4			= 91,
    /* Define:	v10
     * ?
     */

    MaxTagNumber,		//= 1024

    DefineBitsPtr		= 1023,
    /* Define:	v5, Unknown
     * Creates a bitmap that references another bitmap that has already been
     * allocated. This special tag is for performance optimization in the
     * editor only. (Macromedia Flash SWFv5)
     */

};

};

};

#if 1
struct TagNameTable: public std::vector<const char*> {
    TagNameTable() {
	reserve(TAG::CODE::MaxTagNumber);
#define	ADD_TAG_NAME(tag) push_back(#tag)
//#define	ADD_TAG_NAME(tag) [TAG::CODE::tag] = #tag	// XXX:
#else
struct TagNameTable: public std::map<uint16_t, const char*> {
    TagNameTable() {
#define	ADD_TAG_NAME(tag) insert(value_type(TAG::CODE::tag, #tag))
#endif
	//ADD_TAG_NAME(FileHeader);
	ADD_TAG_NAME(End);			//  0
	ADD_TAG_NAME(ShowFrame);
	ADD_TAG_NAME(DefineShape);
	ADD_TAG_NAME(FreeCharacter);
	ADD_TAG_NAME(PlaceObject);
	ADD_TAG_NAME(RemoveObject);		//  5
	ADD_TAG_NAME(DefineBits);
	ADD_TAG_NAME(DefineButton);
	ADD_TAG_NAME(JPEGTables);
	ADD_TAG_NAME(SetBackgroundColor);
	ADD_TAG_NAME(DefineFont);		// 10
	ADD_TAG_NAME(DefineText);
	ADD_TAG_NAME(DoAction);
	ADD_TAG_NAME(DefineFontInfo);
	ADD_TAG_NAME(DefineSound);
	ADD_TAG_NAME(StartSound);		// 15
	ADD_TAG_NAME(StopSound);
	ADD_TAG_NAME(DefineButtonSound);
	ADD_TAG_NAME(SoundStreamHead);
	ADD_TAG_NAME(SoundStreamBlock);
	ADD_TAG_NAME(DefineBitsLossless);	// 20
	ADD_TAG_NAME(DefineBitsJPEG2);
	ADD_TAG_NAME(DefineShape2);
	ADD_TAG_NAME(DefineButtonCXForm);
	ADD_TAG_NAME(Protect);
	ADD_TAG_NAME(PathsArePostscript);	// 25
	ADD_TAG_NAME(PlaceObject2);
	ADD_TAG_NAME(UnusedTag_27);
	ADD_TAG_NAME(RemoveObject2);
	ADD_TAG_NAME(SyncFrame);
	ADD_TAG_NAME(UnusedTag_30);		// 30
	ADD_TAG_NAME(FreeAll);
	ADD_TAG_NAME(DefineShape3);
	ADD_TAG_NAME(DefineText2);
	ADD_TAG_NAME(DefineButton2);
	ADD_TAG_NAME(DefineBitsJPEG3);		// 35
	ADD_TAG_NAME(DefineBitsLossless2);
	ADD_TAG_NAME(DefineEditText);
	ADD_TAG_NAME(DefineVideo);
	ADD_TAG_NAME(DefineSprite);
	ADD_TAG_NAME(NameCharacter);		// 40
	ADD_TAG_NAME(ProductInfo);
	ADD_TAG_NAME(DefineTextFormat);
	ADD_TAG_NAME(FrameLabel);
	ADD_TAG_NAME(UnusedTag_44);
	ADD_TAG_NAME(SoundStreamHead2);		// 45
	ADD_TAG_NAME(DefineMorphShape);
	ADD_TAG_NAME(GenerateFrame);
	ADD_TAG_NAME(DefineFont2);
	ADD_TAG_NAME(GeneratorCommand);
	ADD_TAG_NAME(DefineCommandObject);	// 50
	ADD_TAG_NAME(CharacterSet);
	ADD_TAG_NAME(ExternalFont);
	ADD_TAG_NAME(UnusedTag_53);
	ADD_TAG_NAME(UnusedTag_54);
	ADD_TAG_NAME(UnusedTag_55);		// 55
	ADD_TAG_NAME(ExportAssets);
	ADD_TAG_NAME(ImportAssets);
	ADD_TAG_NAME(ProtectDebug);
	ADD_TAG_NAME(DoInitAction);
	ADD_TAG_NAME(DefineVideoStream);	// 60
	ADD_TAG_NAME(VideoFrame);
	ADD_TAG_NAME(DefineFontInfo2);
	ADD_TAG_NAME(DebugID);
	ADD_TAG_NAME(ProtectDebug2);
	ADD_TAG_NAME(ScriptLimits);		// 65
	ADD_TAG_NAME(SetTabIndex);
	ADD_TAG_NAME(UnusedTag_67);
	ADD_TAG_NAME(UnusedTag_68);
	ADD_TAG_NAME(FileAttributes);
	ADD_TAG_NAME(PlaceObject3);		// 70
	ADD_TAG_NAME(ImportAssets2);
	ADD_TAG_NAME(DoABCAction);
	ADD_TAG_NAME(DefineFontAlignZones);
	ADD_TAG_NAME(CSMTextSettings);
	ADD_TAG_NAME(DefineFont3);		// 75
	ADD_TAG_NAME(SymbolClass);
	ADD_TAG_NAME(Metadata);
	ADD_TAG_NAME(DefineScalingGrid);
	ADD_TAG_NAME(UnusedTag_79);
	ADD_TAG_NAME(UnusedTag_80);		// 80
	ADD_TAG_NAME(UnusedTag_81);
	ADD_TAG_NAME(DoABC);
	ADD_TAG_NAME(DefineShape4);
	ADD_TAG_NAME(DefineMorphShape2);
	ADD_TAG_NAME(UnusedTag_85);		// 85
	ADD_TAG_NAME(DefineSceneAndFrameLabelData);
	ADD_TAG_NAME(DefineBinaryData);
	ADD_TAG_NAME(DefineFontName);
	ADD_TAG_NAME(StartSound2);
	ADD_TAG_NAME(DefineBitsJPEG4);		// 90
	ADD_TAG_NAME(DefineFont4);

	//ADD_TAG_NAME(DefineBitsPtr);		// 1023
    }
};

struct Tag {
    /* Tag format:
     *
     * Each tag begins with a tag type and a length. The tag-header formats
     * can be either short or long. Short tag headers are used for tags with
     * 62 bytes of data or less. Long tag headers, with a signed 32-bit length
     * field, can be used for any tag size up to 2GB, far larger than is
     * presently practical.
     *
     * NOTE: The TagCodeAndLength field is a two-byte word, not a bit field of
     * 10 bits followed by a bit field of 6 bits. The little-endian byte
     * ordering of a SWF file makes these two layouts different.
     */

    union {
	struct {
	    uint16_t siz_:6, code:10;	// XXX:
	};  uint16_t TagCodeAndLength;
	    /* 0x000~0x1ff for future use,
	     * 0x200~0x3ff for third party application use
	     */
    };	    //uint32_t size;	// used only when loading/parsing

    Tag(BitStream& bs) { load(bs); }
    Tag(Tag& t): TagCodeAndLength(t.TagCodeAndLength)/*: size(t.size)*/ { }

    void load(BitStream& bs) {
	bs >> TagCodeAndLength;
	if (siz_ == 0x3f) bs >> size; else size = siz_;
	tend = bs.pos + size;
    }

#ifndef	NDEBUG
    virtual void dump(std::ostream& os) {	// XXX:
	uint16_t tlen = sizeof(TagCodeAndLength) + (0x3f == siz_ ? 4 : 0);
	static TagNameTable tnTbl;
#if 1
	const char* tn = tnTbl[code];
	os  << (tn ? : "UnknownTag")
#else
	TagNameTable::iterator it = tnTbl.find(code);
	os  << (it == tnTbl.end() ? "UnknownTag" : it->second)
#endif
	    << '/' << code << ": \t0x" << std::setfill('0') << std::hex
	    << std::setw(8) << (tend - size - tlen) << " +("
	    << std::setfill(' ') << std::dec << tlen << " + "
	    << size << ")\n";
    }
#endif

    virtual ~Tag() { }

    static uint32_t size, tend;
    static uint8_t  swfv; //X, XX;
};

//typedef Tag End;
struct End: public Tag {
    End(Tag& t, BitStream& bs): Tag(t) { }
};

struct TagUnknown: public Tag {
     TagUnknown(Tag& t, BitStream& bs): Tag(t), data(NULL) { load(bs); }

    ~TagUnknown() { delete[] data; }

    void load(BitStream& bs) {
	if (0) bs.ignore(size); else {
	    data = new uint8_t[size];
	    bs.read((char*)data, size);
	}
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	if (data) dump_data(std::clog, data, size); else
	os  << "  (ignoring " << size << " bytes data)\n";
    }

    uint8_t* data;
};
typedef	TagUnknown TagUnused;
typedef	TagUnknown TagDefault;

typedef TagUnknown FreeCharacter;
typedef TagUnknown PathsArePostscript;
typedef TagUnused  UnusedTag_27;
typedef TagUnknown SyncFrame;
typedef TagUnused  UnusedTag_30;
typedef TagUnknown FreeAll;
typedef TagUnknown DefineVideo;
typedef TagUnknown NameCharacter;
typedef TagUnknown ProductInfo;
typedef TagUnknown DefineTextFormat;
typedef TagUnused  UnusedTag_44;
typedef TagUnknown GenerateFrame;
typedef TagUnknown DefineCommandObject;
typedef TagUnknown CharacterSet;
typedef TagUnknown ExternalFont;
typedef TagUnused  UnusedTag_53;
typedef TagUnused  UnusedTag_54;
typedef TagUnused  UnusedTag_55;
typedef TagUnused  UnusedTag_67;
typedef TagUnused  UnusedTag_68;

typedef TagUnknown ImportAssets2;
typedef TagUnknown DoABCAction;
typedef TagUnknown DefineFontAlignZones;
typedef TagUnknown CSMTextSettings;
typedef TagUnknown DefineFont3;
typedef TagUnknown SymbolClass;
typedef TagUnknown Metadata;
typedef TagUnknown DefineScalingGrid;
typedef TagUnknown UnusedTag_79;
typedef TagUnknown UnusedTag_80;
typedef TagUnknown UnusedTag_81;
typedef TagUnknown DoABC;
typedef TagUnknown DefineMorphShape2;
typedef TagUnused  UnusedTag_85;
typedef TagUnknown DefineSceneAndFrameLabelData;
typedef TagUnknown DefineBinaryData;
typedef TagUnknown DefineFontName;
typedef TagUnknown StartSound2;
typedef TagUnknown DefineFont4;

struct FileAttributes: public Tag {
    /* The FileAttributes tag defines characteristics of the SWF file. This
     * tag is required for SWF 8 and later and must be the first tag in the
     * SWF file. Additionally, the FileAttributes tag can optionally be
     * included in all SWF file versions.
     *
     * The HasMetadata flag identifies whether the SWF file contains the
     * Metadata tag. Flash Player does not care about this bit field or the
     * related tag but it is useful for search engines.
     *
     * The UseNetwork flag signifies whether Flash Player should grant the SWF
     * file local or network file access if the SWF file is loaded locally.
     * The default behavior is to allow local SWF files to interact with local
     * files only, and not with the network.  However, by setting the
     * UseNetwork flag, the local SWF can forfeit its local file system access
     * in exchange for access to the network. Any version of SWF can use the
     * UseNetwork flag to set the file access for locally loaded SWF files
     * that are running in Flash Player 8 or later.
     */

    union {
	struct {
	    uint32_t UseNetwork:1, SWFRelativeURL:1,
		     SupressCrossDomainCaching:1,
		     ActionScript3:1, HasMetadata:1,
		     UseGPU:1, UseDirectBlit:1,
		     Reserved:25;
	};  uint32_t _;
    };

    FileAttributes(Tag& t, BitStream& bs): Tag(t), _(0x0) { load(bs); }

    void load(BitStream& bs) { bs >> _; }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Flags:0x"	 << std::setfill('0') << std::hex
	    << std::setw(8) << _ << std::setfill(' ') << std::dec
	    << "; UseDirectBlit:" << UseDirectBlit
	    << ", UseGPU:" << UseGPU << "," << std::endl
	    << "  HasMetadata:" << HasMetadata
	    << ", ActionScript3:" << ActionScript3
	    << ", UseNetwork:" << UseNetwork << std::endl;
    }
};

struct GeneratorCommand: public Tag {
    uint32_t ver;
    std::string info;

    GeneratorCommand(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) {
	info.resize(size - sizeof(ver));
	bs >> ver >> info;
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Generator v" << ver << ": " << info/*.data()*/ << std::endl;
    }
};

struct DebugID: public Tag {
    //std::string uuid;
    uint8_t* uuid;

    /* The DebugID tag is used to match a debug file (.swd) with a Flash
     * animation (.swf). This is used by the Flash environment and is not
     * required to create movies otherwise.
     *
     * The uuid is a universally unique identifier. The size should be 128
     * bytes. It is otherwise defined by the size of the tag. All Unix and
     * MS-Windows OSes offer a library to generate UUIDs. Although, you can
     * very well just use a simple counter, it will work too.
     */

     DebugID(Tag& t, BitStream& bs): Tag(t) { load(bs); }
    ~DebugID() { delete[] uuid; }

    void load(BitStream& bs) {
	uuid = new uint8_t[size + 1];
	bs.read((char*)uuid, size);
	uuid[size] = '\0';
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  UUID: " << uuid << std::endl;
    }
};

/* Definition and control tags:
 *
 * The two categories of tags in a SWF file are as follows:
 *
 * Definition tags define the content of the SWF file - the shapes, text,
 * bitmaps, sounds, and so on. Each definition tag assigns a unique ID called
 * a character ID to the content it defines. Flash Player then stores the
 * character in a repository called the dictionary. Definition tags, by
 * themselves, do not cause anything to be rendered.
 *
 * Control tags create and manipulate rendered instances of characters in the
 * dictionary, and control the flow of the file.
 */

struct  DefineTag;
struct  ControlTag;

struct  PlaceObject;

typedef DefineTag Character;
typedef uint16_t  CharacterID;

//typedef std::vector<Character*> Dictionary;	// better to use map container
typedef std::map<uint16_t, Character*> Dictionary;

/* The dictionary:
 *
 * The dictionary is a repository of characters that are defined, and are
 * available for control tags to use. The process of building and using the
 * dictionary is as follows:
 *
 *  1. The definition tag defines some content, such as a shape, font, bitmap,
 *  or sound.
 *
 *  2. The definition tag assigns a unique CharacterId to the content.
 *
 *  3. The content is saved in the dictionary under the CharacterId.
 *
 *  4. A control tag uses the CharacterId to retrieve the content from the
 *  dictionary, and performs some action on the content, such as displaying a
 *  shape, or playing a sound.
 *
 * Every definition tag must specify a unique ID. Duplicate IDs are not
 * allowed. Typically, the first CharacterId is 1, the second CharacterId is
 * 2, and so on. The number zero (0) is special and is considered a null
 * character.
 *
 * Control tags are not the only tags that reference the dictionary.
 * Definition tags can use characters from the dictionary to define more
 * complex characters. For example, the DefineButton and DefineSprite tags
 * refer to other characters to define their contents. The DefineText tag can
 * refer to font characters to select different fonts for the text.
 */

struct AttrInfo { uint16_t rfid, depth; };

struct DefineTag: public Tag {
#ifdef	NDEBUG
    static
#endif
    uint16_t id;	// useless duplicated in map container

    DefineTag(Tag& t): Tag(t) { id = 0u; }
    DefineTag(Tag& t, BitStream& bs): Tag(t) { load(bs);    add2dict(); }

    void add2dict() { di->insert(Dictionary::value_type(id, this)); }

    void load(BitStream& bs) {	bs >> id; }

    virtual void show(AttrInfo* attr) = 0;
    //virtual int operator() () = 0;

    static FrameRender* fr;
    static SoundMixer* sm;
    static Dictionary* di;
};

/* Shapes:
 *
 * The SWF shape architecture is designed to be compact, flexible and rendered
 * very quickly to the screen. It is similar to most vector formats in that
 * shapes are defined by a list of edges called a path. A path may be closed,
 * where the start and end of the path meet to close the figure, or open,
 * where the path forms an open-ended stroke. A path may contain a mixture of
 * straight edges, curved edges, and `pen up and move' commands. The latter
 * allows multiple disconnected figures to be described by a single shape
 * structure.
 *
 * A fill style defines the appearance of an area enclosed by a path. Fill
 * styles supported by the SWF file format include a color, a gradient, or a
 * bitmap image.
 *
 * A line style defines the appearance of the outline of a path. The line
 * style may be a stroke of any thickness and color.
 *
 * Most vector formats allow only one fill and line style per path. The SWF
 * file format extends this concept by allowing each edge to have its own line
 * and fill style.  This can have unpredictable results when fill styles
 * change in the middle of a path.
 *
 * A shape is composed of the following elements:
 *
 *  CharacterId - A 16-bit value that uniquely identifies this shape as a
 *  `character' in the dictionary. The CharacterId can be referred to in
 *  control tags such as PlaceObject. Characters can be reused and combined
 *  with other characters to make more complex shapes.
 *
 *  Bounding box - The rectangle that completely encloses the shape.
 *
 *  Fill style array - A list of all the fill styles used in a shape.
 *
 *  Line style array - A list of all the line styles used in a shape.  126
 *  Shapes
 *
 *  Shape record array - A list of shape records. Shape records can define
 *  straight or curved edges, style changes, or move the drawing position.
 *
 *  NOTE: Line and fill styles are defined only once and may be used (and
 *  reused) by any of the edges in the shape.
 */

struct DefineShape: public DefineTag {
    Rect br;	ShapeWithStyle sws;

    DefineShape(Tag& t): DefineTag(t) { }
    DefineShape(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    void load(BitStream& bs) {
	if (code == TAG::CODE::DefineShape2)
	    ShapeWithStyle::lots = true,
	    ShapeWithStyle::hasa = false; else
	    ShapeWithStyle::lots =
	    ShapeWithStyle::hasa = (code == TAG::CODE::DefineShape3);
	bs >> br >> sws;
    }

    void show(AttrInfo* attr) {
	fr->DrawShape(&sws, reinterpret_cast<PlaceObject*>(attr));
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(shape) #" << id << " in " << br << std::endl << sws;
    }
};
typedef	DefineShape DefineShape2;
typedef	DefineShape DefineShape3;

struct DefineShape4: public DefineShape {
    Rect ebr;
    union {	 uint8_t _;
	struct { uint8_t UsesScalingStrokes:1, UsesNonScalingStrokes:1,
			 UsesFillWindingRule:1, Reserved:5;
	};
    };

    DefineShape4(Tag& t, BitStream& bs): DefineShape(t) { load(bs); }

    void load(BitStream& bs) {
	ShapeWithStyle::lots = ShapeWithStyle::ls2 =
	ShapeWithStyle::hasa = true;
	bs >> br >> ebr >> _ >> sws;
    }

    void show(AttrInfo* attr) {
	fr->DrawShape(&sws, reinterpret_cast<PlaceObject*>(attr));
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(shape) #" << id << " in " << br << std::endl << sws;
    }
};

struct DefineMorphShape: public DefineTag {
    Rect br0, br1;
    //uint32_t off2;			// XXX: useless
    MorphShapeWithStyle sws;

    DefineMorphShape(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    void load(BitStream& bs) {		uint32_t off2;
	bs >> br0 >> br1 >> off2 >> sws;
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(morph) #" << id << " from " << br0
	    << " to " << br1 << std::endl << sws;
    }
};

struct ImageBitmap: public DefineTag {
    uint8_t fmtn;
    uint16_t pitch;
    uint16_t width, height;
    uint8_t *bmpd, *paf;

     ImageBitmap(Tag& t): DefineTag(t) { bmpd = paf = NULL; }
     ImageBitmap(Tag& t, BitStream& bs): DefineTag(t, bs) { bmpd = paf = NULL; }
    ~ImageBitmap() { delete[] bmpd; delete[] paf; }
};

struct DefineBitsLossless: public ImageBitmap {
    enum { FMT__8BIT = 0x03, FMT_15BIT, FMT_32BIT, FMT_16BIT, FMT_24BIT, };

     DefineBitsLossless(Tag& t, BitStream& bs):
	 ImageBitmap(t, bs) { load(bs); }

    void load(BitStream& bs) {
	uint8_t* data = new uint8_t[(size -= 7u)];
	bs >> fmtn >> width >> height;
	bs.read((char*)data, size);
	decode(data);	size += 7u;
	delete[] data;
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(image) #" << id << ": " << width << "x" << height
	    << "@0x" << std::setfill('0') << std::hex << std::setw(2)
	    << fmtn  << std::setfill(' ') << std::dec << std::endl;
    }

    void decode(uint8_t* data) {
	uint32_t nlen;

	switch (fmtn) {
	case FMT__8BIT: {
	    nlen = (pitch = (width + 3u) & ~0x03) * height;
	    uint16_t cmsz = data[0];
	    uint8_t n = pitch - width;
	    if (code == TAG::CODE::DefineBitsLossless)
		 cmsz = (cmsz + 1u) * 3u, pitch = width * 3u;
	    else cmsz = (cmsz + 1u) << 2, pitch = width << 2;

	    uint16_t i, j;
	    uint32_t* ptr = (uint32_t*)(bmpd =
		    (uint8_t*)(new uint32_t[width * height]));
	    uint8_t *cmap = new uint8_t[nlen += cmsz], *idx = cmap + cmsz;

	    inflate_wrapper((char*)&data[1], size - 1u, cmap, nlen);
	    if (code == TAG::CODE::DefineBitsLossless) {
		for (i=0u; i < height; ++i) {
		    for (j=0u; j < width; ++j) {
			*ptr++ = ((cmap[*idx + 0u]/*R*/ <<  8)
				| (cmap[*idx + 1u]/*G*/ << 16)
				| (cmap[*idx + 2u]/*B*/ << 24)
				| ((uint32_t)0xff	 <<  0));
			++idx;
		    }	idx += n;
		}
	    } else {
		for (i=0u; i < height; ++i) {
		    for (j=0u; j < width; ++j) {
			*ptr++ = ((cmap[*idx + 0u]/*R*/ <<  8)
				| (cmap[*idx + 1u]/*G*/ << 16)
				| (cmap[*idx + 2u]/*B*/ << 24)
				| (cmap[*idx + 3u]/*A*/ <<  0) | 0x00);
			++idx;
		    }	idx += n;
		}
	    }	fmtn = FMT_32BIT;	delete[] cmap;
	}   break;

	case FMT_15BIT: {
	    uint16_t *ptr = (uint16_t*)(bmpd =
			    (uint8_t*)(new uint16_t[nlen = width * height]));

#define	RGB555_TO_ARGB(pixel) \
	((uint32_t)(0xff) | ((pixel & (0x1f << 10)/*B*/) << 17) \
			  | ((pixel & (0x1f <<  5)/*G*/) << 14) \
			  | ((pixel & (0x1f <<  0)/*R*/) << 11) )
#define	RGB555_TO_RGB565(pixel) \
			  ( ((pixel & (0x1f << 10)/*B*/) <<  0) \
			  | ((pixel & (0x1f <<  5)/*G*/) <<  1) \
			  | ((pixel & (0x1f <<  0)/*R*/) << 11) )

	    inflate_wrapper((char*)data, size, ptr , (nlen << 1));
	    for (ptr += nlen; (uint16_t*)bmpd < ptr--; )
		*ptr = RGB555_TO_RGB565(*ptr);
	    pitch = width << 1;		fmtn = FMT_16BIT;
	}   break;

	case FMT_32BIT: {
	    uint8_t* ptr = bmpd = (uint8_t*)(new uint32_t[width * height]);

	    inflate_wrapper((char*)data, size, bmpd,
		    nlen = (pitch = width << 2) * height);
	    if (code == TAG::CODE::DefineBitsLossless) {
		for (ptr += nlen; bmpd < ptr; ) {
		     ptr -= 4;		*ptr = 0xff;
		}
	    }
	}   break;
	}
    }
};
typedef	DefineBitsLossless DefineBitsLossless2;

struct DefineBitsJPEG: public ImageBitmap {

    ~DefineBitsJPEG() {
	if (!jtbl.src) return;
	jpeg_destroy_decompress(&jtbl);
	jtbl.src = NULL;
    }

     DefineBitsJPEG(Tag& t, BitStream& bs): ImageBitmap(t) {
	jsmgr.init_source 	= js_init_ource;
        jsmgr.skip_input_data	= js_skip_input_data;
        jsmgr.fill_input_buffer = js_fill_input_buffer;
        jsmgr.resync_to_restart = jpeg_resync_to_restart;
        jsmgr.term_source	= js_term_source;

	jtbl.err = jpeg_std_error(&jerr);
	jerr.error_exit = j_error_exit;

	load(bs);
     }

    void load(BitStream& bs) {
	uint32_t nlen = size;
	uint8_t* data;

	if (code != TAG::CODE::JPEGTables) {
	    DefineTag::load(bs); add2dict();	nlen -= 2u;
	}   bs.read((char*)(data = new uint8_t[nlen]), nlen);
	//dump_data(std::clog, data, nlen);

	if (code == TAG::CODE::JPEGTables) {
	    if (setjmp(jclt.jbuf)) {
		jpeg_destroy_decompress(&jtbl);
		jtbl.src = NULL;	return;
	    }   jpeg_create_decompress (&jtbl);

	    jtbl.client_data = &jclt;	jtbl.src = &jsmgr;
	    jclt.data = (char*)data;	jclt.size = nlen;

	    nlen = jpeg_read_header(&jtbl, FALSE);

	    assert(nlen == (uint32_t)JPEG_HEADER_TABLES_ONLY);

	    // XXX: maybe we can free JPEGTables structure after here
	}   else decode(data, nlen);	delete[] data;
    }

    void show(AttrInfo* attr) { }

    void dump(std::ostream& os) {
	uint32_t nlen = size;		Tag::dump(os);
	if (code == TAG::CODE::JPEGTables) os  << "  JPEG tables";
	else {	nlen -= 2u;
	    os  << "  Object(JPEG) #" << id;
	}   os  << " with " << nlen << " bytes\n";
    }

    void decode(uint8_t* data, uint32_t nlen) {
	struct jpeg_decompress_struct jpds;

	/* Parameter to be fed into the deblocking filter. The parameter
	 * describes a relative strength of the deblocking filter from 0-100%
	 * expressed in a normalized 8.8 fixed point format.
	 */

	FIXED8 DeblockParam;
	uint32_t offa = 0u;

	const uint8_t PNG_hdr[] = {
	    0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A
	}, GIF89a_hdr[] = { 'G', 'I', 'F', '8', '9', 'a' };

	if (!memcmp(data, PNG_hdr, sizeof(PNG_hdr))) {	// TODO:
	    return;
	} else
	if (!memcmp(data, GIF89a_hdr, sizeof(GIF89a_hdr))) {	// TODO:
	    return;
	} //else if (memcmp(data, 0xD8FF, 2)) ;	// XXX: JPEG/SOI

	jclt.data = (char*)data;
	jclt.size = nlen;

	// FIXME: urgly code here
	if (code == TAG::CODE::DefineBits) {
	    assert(jtbl.src);
	    memcpy(&jpds, &jtbl, sizeof(jtbl));
	    if (setjmp(jclt.jbuf)) fmtn = 0x00;
	    else djpeg(&jpds);	return;
	}   jpds.err = &jerr;

	if (setjmp(jclt.jbuf)) {
	    jpeg_destroy_decompress(&jpds);
	    fmtn = 0x00;	return;
	}   jpeg_create_decompress (&jpds);
	jpds.client_data = &jclt;
	jpds.src = &jsmgr;

	if (TAG::CODE::DefineBitsJPEG2 < code) {
#if	__BYTE_ORDER == __BIG_ENDIAN
	    offa = ((data[0] << 24) | (data[1] << 16) |
		    (data[2] <<  8) |  data[3] <<  0);
#else// XXX: le2ne_32()
	    offa = *(uint32_t*)data;
#endif

	    jclt.size = offa;		jclt.data += 4u;

	    if (code == TAG::CODE::DefineBitsJPEG4) {
#if	__BYTE_ORDER == __BIG_ENDIAN
		DeblockParam =  (((uint8_t*)jclt.data)[0] << 8) |
				(((uint8_t*)jclt.data)[1] << 0);
#else// XXX: le2ne_32()
		DeblockParam = *(FIXED8*)jclt.data;
#endif
		jclt.data += 2u;
	    }
	}

	(void)DeblockParam;	// FIXME:

#if 	__BYTE_ORDER == __BIG_ENDIAN
	if (*(uint32_t*)jclt.data == 0xFFD9FFD8)
	    *(uint32_t*)jclt.data =  0xFFD8FFD9;
#else// Kludge to correct some corrupted files
	if (*(uint32_t*)jclt.data == 0xD8FFD9FF)
	    *(uint32_t*)jclt.data =  0xD9FFD8FF;
#endif

	djpeg(&jpds);

	if (code == TAG::CODE::DefineBitsJPEG3) {
	    uint32_t npix = width * height;
	    inflate_wrapper((char*)(data + 4 + offa), nlen - 4 - offa,
		    (paf = new uint8_t[npix]),  npix);
	}

	jpeg_finish_decompress (&jpds);
	jpeg_destroy_decompress(&jpds);
    }

    void djpeg(struct jpeg_decompress_struct* jpds) {
	uint8_t* ptr;	JSAMPROW buffer[1];	assert(jpds);

	jpeg_read_header(jpds, TRUE);
	jpeg_start_decompress(jpds);

	width   = jpds->output_width;
	height  = jpds->output_height;
	pitch   = jpds->output_components * width;
	bmpd    = (ptr = new uint8_t[(pitch * height)]);

	while (jpds->output_scanline < jpds->output_height) {
	    buffer[0] = ptr;	ptr += pitch;
	    jpeg_read_scanlines(jpds, buffer, 1);
	}   fmtn = DefineBitsLossless::FMT_24BIT;

	switch (jpds->out_color_space) {
	case JCS_GRAYSCALE: {
	    uint8_t* pds = bmpd + pitch * height;
	    for (ptr -= pitch; bmpd < ptr--; )
		*(--pds) = *ptr, *(--pds) = *ptr, *(--pds) = *ptr;
	}   break;
	case JCS_RGB:; default:;
	}
    }

    static struct JPEGClient jclt;
    static struct jpeg_error_mgr jerr;
    static struct jpeg_source_mgr jsmgr;
    static struct jpeg_decompress_struct jtbl;
};
typedef	DefineBitsJPEG JPEGTables;
typedef	DefineBitsJPEG DefineBits;
typedef	DefineBitsJPEG DefineBitsJPEG2;
typedef	DefineBitsJPEG DefineBitsJPEG3;
typedef	DefineBitsJPEG DefineBitsJPEG4;

struct DefineButton: public DefineTag {
    std::vector<Button*> btVec;
    std::vector<Action*> axVec;

    ~DefineButton() {
	for (std::vector<Button*>::iterator it = btVec.begin();
		it != btVec.end(); ++it) delete *it;
	for (std::vector<Action*>::iterator it = axVec.begin();
		it != axVec.end(); ++it) delete *it;
    }
     DefineButton(Tag& t, BitStream& bs): DefineTag(t, bs) {
	load(bs);	cxf = NULL;	siVec = NULL;
     }

    void load(BitStream& bs) {
	for (uint16_t i=0u; ; ++i) {	uint8_t state;
	    bs >> state;		if (!state) break;
	    Button* bt = new Button;	bs >> *bt;
	    bt->_ = state;		btVec.push_back(bt);
	}				Action* ax;
	do { ax = new Action;		bs >> *ax;
	     axVec.push_back(ax);
	} while (ax->id != ACTION::CODE::End);
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  <<   "  Object(button) #" << id << std::endl;
	for (std::vector<Button*>::iterator it = btVec.begin();
		it != btVec.end(); ++it) os << **it;
	for (std::vector<Action*>::iterator it = axVec.begin();
		it != axVec.end(); ++it) os << **it;
    }

    std::vector<SoundInfo>* siVec;
    CXForm* cxf;
};

struct DefineButton2: public DefineTag {
    union {	 uint8_t _;
	struct { uint8_t menu:1, xxxxxxx:7; };
    };	//uint16_t btns;		// used only when loading/parsing
    std::vector<Button2  *> btVec;
    std::vector<Condition*> cdVec;

     DefineButton2(Tag& t, BitStream& bs): DefineTag(t, bs) {
	load(bs);			//cxf = NULL;	siVec = NULL;
    }

    ~DefineButton2() {
	for (std::vector<Button2  *>::iterator it = btVec.begin();
		it != btVec.end(); ++it) delete *it;
	for (std::vector<Condition*>::iterator it = cdVec.begin();
		it != cdVec.end(); ++it) delete *it;
    }

    void load(BitStream& bs) {		uint16_t btns;
	bs >> _ >> btns;
	for (uint16_t i=0u; ; ++i) {	uint8_t state;
	    bs >> state;		if (!state) break;
	    Button2* bt = new Button2;	bs >> *bt;
	    bt->_ = state;		btVec.push_back(bt);
	}
	for (uint8_t i=0u; btns; ++i) { Condition* cd;
	    bs >> btns;			cd = new Condition;
	    bs >> *cd;			cdVec.push_back(cd);
	}
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  <<  "  Object(button) #" << id
	    << (menu ? " (Track menu)" : " (Push button)") << std::endl;
	for (std::vector<Button2*>::iterator it = btVec.begin();
		it != btVec.end(); ++it) os << **it;
	for (std::vector<Condition*>::iterator it = cdVec.begin();
		it != cdVec.end(); ++it) os << **it;
    }

    //std::vector<SoundInfo>* siVec;
    //CXForm* cxf;			// XXX: does DefineButton2 need these?
};

struct DefineFontInfo;
struct DefineFont: public DefineTag {
#if 0
    std::vector<uint16_t>  ofVec;	// XXX: useless
#endif
    std::vector<FontShape> ftVec;

    DefineFont(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    void load(BitStream& bs) {	uint16_t i, glyf;
	bs >> glyf;		glyf >>= 1;	// at least 1;
	ftVec.resize(glyf);	bs.ignore((glyf << 1) - 2);
#if 0	// XXX:
	ofVec.resize(glyf);	ofVec[0] = (glyf << 1);
	for (uint16_t i=1; i < glyf; ++i) bs >> ofVec[i];
#endif
	for (i=0; i < glyf; ++i) bs >> ftVec[i];
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	uint16_t i = 0, cnt = ftVec.size();
	os  << "  Object(font) #" << id << std::endl << std::setfill('0');
	for (; i < cnt; ++i)
	    os  << "    Glyph #" << std::setw(4) << i << ": " << ftVec[i];
	os  << std::setfill(' ');
    }

    DefineFontInfo* fi;
};

struct DefineFontInfo: public Tag {
    //uint16_t ftid;	//uint8_t nlen;	// used only when loading/parsing
    std::string ftnm;
    union {
	struct {			// after V6
	    uint8_t wide:1, bold:1, italic:1, xxxx:1,
		    small_text:1, xx:2;
	}   V7;
	struct {			// before V6
	    uint8_t wide:1, bold:1, italic:1, ansii:1, shiftjis:1, unicode:1,
		    xx:2;		// wide is always 1 since V6
	};  uint8_t _;
    };	    //uint8_t lang;		// since V6
    std::vector<uint16_t> ftmap;	// wide

    DefineFontInfo(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) {
	union { uint8_t nlen, lang, code; };
	uint16_t i, ftid, glyf;		DefineFont* pdf;
	bs >> ftid >> nlen;		ftnm.resize(nlen);
	Dictionary::iterator it = DefineTag::di->find(ftid);
	assert(it != DefineTag::di->end() &&
	       it->second->code == TAG::CODE::DefineFont);
	pdf  = reinterpret_cast<DefineFont*>(it->second);
	glyf = pdf->ftVec.size();	pdf->fi = this;
	for (uint8_t c, n=0; n < nlen; ++n) {
	    bs >> c;		ftnm[n] = c;
	}   ftmap.resize(glyf);
	if (5u < swfv) bs >> _ >> lang;
	if (wide) for (i=0; i < glyf; ++i)   bs >> ftmap[i];
	else	  for (i=0; i < glyf; ++i) { bs >> code;    ftmap[i] = code; }
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	uint16_t i, glyf = ftmap.size();
	os  << "    Font name: " << ftnm.data() << std::endl
	    << "    Font maps: \n" << std::setfill('0');
	for (i=0; i < glyf; ++i)
	    os  << "      Glyph #" << std::setw(4) << i
		<< ": `" << (char)ftmap[i] << "'(0x" << std::hex
		<< ftmap[i] << ")" << std::dec << std::endl;
	os  << std::setfill(' ');
    }
};
typedef DefineFontInfo DefineFontInfo2;

struct DefineFont2: public DefineTag {
    enum LangCode {
	LOCAL, LATIN, JAPANESE, KOREAN, SIMPLIFIED_CHINESE,
	TRADITIONAL_CHINESE, UNKNOWN
    };

    union {
	struct {
	    uint8_t bold:1, italic:1, wide:1, woff:1,
		    ansii:1, unicode:1, shiftjis:1, has_layout:1;
	};  uint8_t _;			// wide is always 1 since V6
    };	uint8_t lang;			// reserved before V6
    int16_t asc, des, ldh;		// out of order regarding alignment
    //uint8_t nlen;			// used only when loading/parsing
    std::string ftnm;
    //uint16_t glyf;			// used only when loading/parsing
#if 0
    std::vector<uint32_t>  ofVec;	// XXX: useless. wide, glyf+1
#endif
    std::vector<FontShape> ftVec;
    std::vector<uint16_t>  ftmap;	// wide
    std::vector< int16_t>  adVec;
    std::vector<Rect>      brVec;
    //int16_t kernings;			// used only when loading/parsing
    std::map<uint32_t, int16_t> knMap;	// wide

    DefineFont2(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    void load(BitStream& bs) {
	union { uint8_t nlen, code; };	uint16_t i, glyf;
	bs >> _ >> lang >> nlen;	ftnm.resize(nlen);
	for (uint8_t c, n=0; n < nlen; ++n) {
	    bs >> c;		ftnm[n] = c;
	}   bs >> glyf;		++glyf;

#if 0
	ofVec.resize(glyf);		// XXX: useless
#endif
	// The last offset entry indicates the offset of
	// the data coming after the list of shapes.
	if (woff) {
#if 0
	    for (i=0; i < glyf; ++i)	bs >> ofVec[i];
#else
	    bs.ignore((glyf << 2));
#endif
	} else {
#if 0	    // uint16_t off;
	    for (i=0; i < glyf; ++i) {	bs >> off;	ofVec[i] = off; }
#else
	    bs.ignore((glyf << 1));
#endif
	}   ftVec.resize(--glyf);	ftmap.resize(glyf);

	for (i=0; i < glyf; ++i)	bs >> ftVec[i];

	if (wide) for (i=0; i < glyf; ++i)	  bs >> ftmap[i];
	else for (i=0; i < glyf; ++i) {	bs >> code;	ftmap[i] = code; }

	if (has_layout) {		bs >> asc >> des >> ldh;
	    adVec.resize(glyf);		brVec.resize(glyf);
	    for (i=0; i < glyf; ++i)	bs >> adVec[i];
	    for (i=0; i < glyf; ++i)	bs >> brVec[i];

					bs >> glyf;
	    if (wide) {			KerningWide kn;
		while (glyf--) {	bs >> kn;
		    knMap.insert(std::map<uint32_t, int16_t>::
			    value_type(kn.code, kn.adjs));
		}
	    } else {			Kerning kn;
		while (glyf--) {	bs >> kn;
		    knMap.insert(std::map<uint32_t, int16_t>::
			    value_type(((kn.c2 << 16) | kn.c1), kn.adjs));
		}
	    }
	}
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	uint16_t i, glyf = ftVec.size();
	os  << "  Object(font) #" << id		<< "\n    Font name: "
	    << ftnm.data() << std::setfill('0') << "\n    Font shapes: \n";
	for (i=0; i < glyf; ++i)
	    os  << "      Glyph #" << std::setw(4) << i << ": " << ftVec[i];

	os  << "    Font maps: \n";
	for (i=0; i < glyf; ++i)
	    os  << "      Glyph #" << std::setw(4) << i << ": `"
		<< (char)ftmap[i] << "'(0x" << std::hex << ftmap[i]
		<< ")\n" << std::dec;

	if (has_layout) {
	    os  << "    Font ascent: " << asc << ", descent: " << des
		<< ", leading height: " << ldh << std::endl
		<< "    Font advances and boundings: \n";
	    for (i=0; i < glyf; ++i)
		os  << "      Glyph #" << std::setw(4) << i
		    << ": " << adVec[i] << ", " << brVec[i] << std::endl;

	    os  << "    Font kernings: " << (glyf = knMap.size()) << std::endl;
	    for (std::map<uint32_t, int16_t>::iterator it = knMap.begin();
		    it != knMap.end(); ++it) //os  << kn;
		os  << "\t(#" << std::setw(4) << (it->first & 0xffff)
		    <<  ", #" << std::setw(4) << (it->first >> 16)
		    << ") adjuct " << it->second << " twips\n";
	    os  << std::setfill(' ');
	}
    }
};

struct DefineText: public DefineTag {
    Rect br;
    Matrix mtx;
    //uint8_t gnb, anb;			// used only when loading/parsing
    std::vector<TextRecordString*> teVec;

     DefineText(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    ~DefineText() {
	for (std::vector<TextRecordString*>::iterator it = teVec.begin();
		it != teVec.end(); ++it) delete *it;
    }

    void load(BitStream& bs) {	uint8_t gnb, anb;
	bool alpha = (code == TAG::CODE::DefineText2);
	bs >> br >> mtx >> gnb >> anb;
	TextEntry::gnb = gnb, TextEntry::anb = anb;
	for (;;) {			TextRecord tr;
	    bs >> tr;			if (!tr.endr) break;
	    TextRecordString* trs = new TextRecordString(tr, alpha);
	    bs >> *trs;			teVec.push_back(trs);
	}
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(text) #" << id
	    << " in " << br << " by " << mtx << std::endl;
	for (std::vector<TextRecordString*>::iterator it = teVec.begin();
		it != teVec.end(); ++it) os << **it;
    }
};
typedef	DefineText DefineText2;

struct DefineEditText: public DefineTag {
    Rect br;
    union {
	struct {
	    uint16_t has_font:1, has_mxlen:1, has_color:1, readonly:1,
		     password:1, multiline:1, word_wrap:1, has_text:1,
		     use_outlines:1, html:1, WasStatic:1, border:1,
		     no_select:1, has_layout:1, auto_size:1, HasFontClass:1;
	};  uint16_t _;
    }	flag;
    uint16_t mxlen;			// out of order regarding alignment
    struct { uint16_t ref, height; } font;
    RGBA rgba;
    struct {
	uint8_t align;
	uint16_t lmgn, rmgn;
	int16_t indent, leading;
    }	layout;
    std::string tnam, text;

    DefineEditText(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    void load(BitStream& bs) {
	bs >> br >> flag._;
	if (flag.has_font)  bs >> font.ref >> font.height;
	if (flag.has_color) bs >> rgba;
	if (flag.has_mxlen) bs >> mxlen;
	if (flag.has_layout)bs >> layout.align >> layout.lmgn >>
		layout.rmgn >> layout.indent >> layout.leading;
	bs >> tnam;	    if (flag.has_text) bs >> text;
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(edit) #" << id << " in " << br << std::endl
	    << "    \"" << tnam.data() << "\": \'" << text.data()
	    << '\'' << std::endl;
    }
};
typedef DefineEditText DefineEditText2;

struct VideoFrame;
struct DefineVideoStream: public DefineTag {
    enum { DEBLOCK_ORIG, DEBLOCK_OFF, DEBLOCK_ON, };
    enum { CODEC_SORENSON_H263 = 2u, CODEC_SCREEN_VIDEO, };

    //uint16_t fc;			// XXX: useless in vector container
    uint16_t width, height;		// XXX: in pixel
    union {	 uint8_t _;
	struct { uint8_t smoothing:1, deblocking:2, xxxxx:5; };
    };	uint8_t codec;

    DefineVideoStream(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    void load(BitStream& bs) {		uint16_t fc;
	bs >> fc >> width >> height >> _ >> codec;
	vfVec.resize(fc);
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(video) #" << id << " with " << vfVec.size()
	    << " frames of " << width << "x" << height << "@0x"
	    << std::setfill('0') << std::hex << std::setw(2) << codec
	    << std::setfill(' ') << std::dec << std::endl;
#if 0
	if (!vfVec.empty())	// FIXME:
	    for (std::vector<VideoFrame*>::iterator it = vfVec.begin();
		    it != vfVec.end(); ++it) os  << (*it)->dump(os);
#endif
    }

    std::vector<VideoFrame*> vfVec;
};


struct FrameList: public std::vector<std::vector<ControlTag*> > {
    uint16_t pf, lf;
};


struct DefineSprite: public DefineTag {
    //uint16_t fc;			// useless in vector container

    DefineSprite(Tag& t, BitStream& bs):
	    DefineTag(t, bs) { fl.pf = fl.lf = 0u; load(bs); }

    void load(BitStream& bs) {	uint16_t fc;
	bs >> fc;		fl.resize(fc);
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(sprite) #" << id
	    << " with " << fl.size() << " frames\n";
    }

    FrameList fl;
};


struct VideoFrame: public Tag {
    //uint16_t rfid;			// used only when loading/parsing
    uint16_t frame;
    uint8_t* data;

    ~VideoFrame() { delete[] data; }
     VideoFrame(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) {	    uint16_t rfid;
	uint32_t len = size - 4;    bs >> rfid >> frame;
	bs.read((char*)(data = new uint8_t[len]), len);

	Dictionary::iterator it = DefineTag::di->find(rfid);
	assert(it != DefineTag::di->end() &&
	       it->second->code == TAG::CODE::DefineVideoStream);
	reinterpret_cast<DefineVideoStream*>(it->second)->vfVec.push_back(this);
    }

    void dump(std::ostream& os) {   Tag::dump(os);
	os  << "  Video frame #" << frame << std::endl;	// XXX:
    }
};


struct DefineButtonCXForm: public Tag {
    //uint16_t btid;			// used only when loading/parsing
    CXForm cxf;

    DefineButtonCXForm(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) {	uint16_t btid;
	bs >> btid >> cxf;
	Dictionary::iterator it = DefineTag::di->find(btid);
	assert(it != DefineTag::di->end());
	if (it->second->code == TAG::CODE::DefineButton)
	    reinterpret_cast<DefineButton *>(it->second)->cxf = &cxf;
#if 0	// XXX:
	else if (it->second->code == TAG::CODE::DefineButton2)
	    reinterpret_cast<DefineButton2*>(it->second)->cxf = &cxf;
#endif
	else assert(false);
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Button CXF: " << cxf << std::endl;	// XXX:
    }
};

struct DefineButtonSound: public Tag {
    enum {
	POINTER_LEAVE = 0u, POINTER_ENTER, POINTER_OVER_DOWN,
	POINTER_OVER_UP, POINTER_MAX,
    };

    //uint16_t btid;			// used only when loading/parsing
    std::vector<SoundInfo> siVec;

    DefineButtonSound(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) {	uint16_t btid;
	siVec.resize(POINTER_MAX);	bs >> btid;
	for (int i=0; i < POINTER_MAX; ++i) bs >> siVec[i];
	Dictionary::iterator it = DefineTag::di->find(btid);
	assert(it != DefineTag::di->end());
	if (it->second->code == TAG::CODE::DefineButton)
	    reinterpret_cast<DefineButton *>(it->second)->siVec = &siVec;
#if 0	// XXX:
	else if (it->second->code == TAG::CODE::DefineButton2)
	    reinterpret_cast<DefineButton2*>(it->second)->siVec = &siVec;
#endif
	else assert(false);
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Button sound: \n";
	for (std::vector<SoundInfo>::iterator it = siVec.begin();
		it != siVec.end(); ++it) os << *it;
    }
};

typedef DefineTag Object;
typedef std::list<std::pair<Object*, AttrInfo*> > DispList;

/* Using the display list:
 *
 * The following procedure creates and displays an animation:
 *
 *  1. Define each character with a definition tag.
 *
 *  Each character is given a unique character ID, and added to the dictionary.
 *
 *  2. Add each character to the display list with a PlaceObject2 tag.
 *
 *  Each PlaceObject2 tag specifies the character to be displayed, plus the
 *  following attributes:
 *
 *	* A depth value, which controls the stacking order of the character
 *	being placed. Characters with lower depth values appear to be
 *	underneath characters with higher depth values. A depth value of 1
 *	means the character is displayed at the bottom of the stack. Only one
 *	character can be at any given depth.
 *
 *	* A transformation matrix, which determines the position, scale,
 *	factor, and angle of rotation of the character being placed. The same
 *	character can be placed more than once (at different depths) with a
 *	different transformation matrix.
 *
 *	* An optional color transform, which specifies the color effect
 *	applied to the character being placed. Color effects include
 *	transparency and color shifts.
 *
 *	* An optional name string, which identifies the character being placed
 *	for SetTarget actions. SetTarget is used to perform actions inside
 *	sprite objects.
 *
 *	* An optional ClipDepth value, which specifies the top-most depth that
 *	will be masked by the character being placed.
 *
 *	* An optional ratio value, which controls how a morph character is
 *	displayed when placed. A ratio of zero displays the character at the
 *	start of the morph. A ratio of 65535 displays the character at the end
 *	of the morph.
 *
 *  3. Use a ShowFrame tag to render the contents of the display list to the
 *  screen.
 *
 *  4. Use a PlaceObject2 tag to modify each character on the display List.
 *
 *  Each PlaceObject2 assigns a new transformation matrix to the character at
 *  a given depth.  The character ID is not specified because each depth can
 *  have only one character.
 *
 *  5. Use a ShowFrame tag to display the characters in their new positions.
 *
 *  Repeat steps 4 and 5 for each frame of the animation.
 *
 *  NOTE: If a character does not change from frame to frame, you do not need
 *  to replace the unchanged character after each frame.
 *
 *  6. Use a RemoveObject2 tag to Remove each character from the display list.
 *  Only the depth value is required to identify the character being removed.
 *
 */

struct ControlTag: public Tag {
    ControlTag(Tag& t): Tag(t) { add2ctrl(); }

    void add2ctrl() { (*fl)[fl->lf].push_back(this); }

    //virtual int operator() () = 0;
    virtual void exec() { }

    static FrameList* fl;
    static DispList* dl;
};


struct PlaceObject: public ControlTag, public AttrInfo {
    //uint16_t rfid, depth;
    Matrix mtx;
    CXForm cxf;

    PlaceObject(Tag& t): ControlTag(t) { }
    PlaceObject(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void load(BitStream& bs) {
	uint32_t pos = bs.pos + size;
	bs >> rfid >> depth >> mtx;
	if (bs.pos < pos) bs >> cxf; else
	    cxf.mr = cxf.mg = cxf.mb = cxf.ma = CXForm::DEFAULT_MUL,
	    cxf.ar = cxf.ag = cxf.ab = cxf.aa = CXForm::DEFAULT_ADD;
    }

    void exec() {
	Dictionary::iterator ic = DefineTag::di->find(rfid);
	DispList  ::iterator it = dl->begin();
	assert(ic != DefineTag::di->end());

	while (++it != dl->end()) if (depth < it->second->depth) break;
	dl->insert(it, DispList::value_type(ic->second, this));
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Place object #" << rfid << " in depth #" << depth
	    << ": \n\t" << mtx << ", " << cxf << std::endl;
    }
};

struct PlaceObject3: public PlaceObject {
    union {
	struct {
	    union {
		struct {
		     uint8_t has_mv:1, has_id:1, has_mtx:1, has_cxf:1,
			     has_mpos:1, has_name:1, has_clip:1, has_axn:1;
		};   uint8_t _;
	    };
	    struct { uint8_t has_fltr:1, has_blm:1, has_bmc:1,
			     has_img:1, has_cls:1, reserved:3; };
	};  uint16_t __;
    };					// inheritant from PlaceObject
    //uint16_t depth, rfid;		//Matrix mtx;	CXForm cxf;
    uint8_t blm, bmc;			// out of order regarding alignment
    uint16_t mpos, clip;		// out of order regarding alignment
    std::string cls, onam;
    std::vector<Filter*> flVec;		//uint16_t reserved;
    uint32_t flags;
    //std::vector<Event> enVec;		// better to use map container
    std::map<uint32_t, std::vector<Action*> > evMap;
    //uint32_t end;			// used only when loading/parsing

    /* mpos is used in two cases:
     *
     * (1) When the place object references a DefineMorphShape object. In this
     * case, it defines a linear position between the first and second shapes
     * (0 - draws shape 1 and 65535 - draws shape 2, any intermediate value
     * draws a morphed shape, smaller the value the more it looks like shape 1
     * larger the value, the more it looks like shape 2).
     *
     * (2) When the place object references a VideoFrame object. In this case,
     * the morph position represents the frame number. This means any movie is
     * limited to 65536 frames (the first frame is frame #0). At a regular,
     * NTSC frame rate, it represents about 18 minutes of video. Long videos
     * can be created using a new video stream every 18 minutes.
     */

    enum BlendMode {
	/* The equations use R as Result, C as the object color component, B
	 * and the background color. All components are viewed as values from
	 * 0 to 255. The result is a temporary value which is later saved in
	 * the new background before processing the next object.
	 */

	//Normal = 0,
	Normal = 1,	// R = C,
	// Copy the object as is.

	Layer,		//
	// Uses multiple objects to render (?)

	Multiply,	// R = B * C / 255,
	// Multiply the background with the object colors.

	Screen,		// R = (255 - B) * (255 - C) / 255,
	// Multiply the inverse colors of the background and the object.

	Lighten,	// R = max(B, C),
	// Take the largest of each component of the background and object.

	Darken,		// R = min(B, C),
	// Take the smallest of each component of the background and object.

	Difference,	// R = | B - C |,
	// Defines the absolute value of the difference.

	Add,		// R = min(B + C, 255),
	// Add the components and clamp at 255.

	Substract,	// R = max(B - C, 0),
	// Subtract the components and clamp at 0.

	Invert,		// R = 255 - B,
	// Inverse the background components.

	Alpha,		// Ra = Ca,
	/* Copy the alpha channel of the object in the background. This mode
	 * requires that the parent (background) be set to mode Layer.
	 */

	Erase,		// Ra = 255 - Ca,
	/* Copy the inverse of the alpha channel of the object in the
	 * background alpha. This mode requires that the parent (background)
	 * be set to mode Layer.
	 */

	Overlay,	// R = (B < 128 ? B * C : (255 - B) * (255 - C)) / 255,
	/* Apply the same effect as multiply or screen depending on the
	 * background color before the operation. (Note: the comparison with
	 * 128 could be <= and the results would be same for C but not B. I
	 * current do not know which one is picked)
	 */

	Hardlight,	// R = (C < 128 ? B * C : (255 - B) * (255 - C)) / 255,
	/* Apply the same effect as multiply or screen depending on the object
	 * color. (Note: the comparison with 128 could be <= and the results
	 * would be same for C but not B. I currently do not know which one is
	 * picked)
	 */

	// Values 15 to 255 are reserved.
    };

     PlaceObject3(Tag& t, BitStream& bs): PlaceObject(t), __(0x0) { load(bs); }

    ~PlaceObject3() {
	for (std::vector<Filter*>::iterator it = flVec.begin();
		it != flVec.end(); ++it) delete *it;
	for (std::map<uint32_t, std::vector<Action*> >::iterator
		it0 = evMap.begin(); it0 != evMap.end(); ++it0) {
	    std::vector<Action*>& axVec = it0->second;
	    for (std::vector<Action*>::iterator it = axVec.begin();
		    it != axVec.end(); ++it) delete *it;
	}
    }

    void load(BitStream& bs) {
	if (code == TAG::CODE::PlaceObject3)
	    bs >> __; else bs >> _;	bs >> depth;
	if (has_cls)  bs >> cls;
	if (has_id)   bs >> rfid; else rfid = 0u;
	if (has_mtx)  bs >> mtx;
	if (has_cxf) {
	    cxf.ma = SHRT_MAX;	cxf.aa = CHAR_MAX;
		      bs >> cxf;
	}
	if (has_mpos) bs >> mpos;
	if (has_name) bs >> onam;
	if (has_clip) bs >> clip;
	if (has_fltr) {
	    Filter* pf;			uint8_t fcnt, type;
	    bs >> fcnt;			flVec.resize(fcnt);
	    for (uint8_t i = 0; i < fcnt; ++i) {	bs >> type;
		switch (type) {
		case Filter::DropShadow:
		    pf = new FilterDropShadow(type, bs);	break;
		case Filter::Blur:
		    pf = new FilterBlur(type, bs);		break;
		case Filter::Grow:
		    pf = new FilterGrow(type, bs);		break;
		case Filter::Bevel:
		    pf = new FilterBevel(type, bs);		break;
		case Filter::GradientGrow:
		    pf = new FilterGradientGrow(type, bs);	break;
		case Filter::Convolution:
		    pf = new FilterConvolution(type, bs);	break;
		case Filter::ColorMatrix:
		//case Filter::AdjustColor:
		    pf = new FilterColorMatrix(type, bs);	break;
		case Filter::GradientBevel:
		    pf = new FilterGradientBevel(type, bs);	break;
		default:		pf = NULL;
		}			flVec[i] = pf;
	    }
	}
	if (has_blm) bs >> blm;
	if (has_bmc) bs >> bmc;
	if (has_axn) {			uint32_t end;
	    uint16_t xxxx;		bs >> xxxx;
	    if (5u < swfv) {		bs >> flags;	// FIXME:
		for (;;) {		bs >> end;
		    if (!end) break;	Action* ax;
		    std::vector<Action*>& axVec = evMap[end];
		    bs >> end;		// discard length, XXX:
		    do {		ax = new Action;
			bs >> *ax;	axVec.push_back(ax);
		    } while (ax->id != ACTION::CODE::End);
		}
	    } else {	bs >> xxxx;	flags = xxxx;
		for (;;) {
		    bs >> xxxx;		if (!xxxx) break;
		    end = xxxx;		Action* ax;
		    std::vector<Action*>& axVec = evMap[end];
		    bs >> end;		// discard length, XXX:
		    do {		ax = new Action;
			bs >> *ax;	axVec.push_back(ax);
		    } while (ax->id != ACTION::CODE::End);
		}
	    }
	} else flags = 0u;
    }

    void exec() {
	if (has_id) {
	    Dictionary::iterator ic = DefineTag::di->find(rfid);
	    assert(ic != DefineTag::di->end());
	    if (has_mv) {
		DispList::reverse_iterator it = dl->rbegin();
		for (; it != dl->rend(); ++it)
		    if (depth == it->second->depth) break;
		assert(it != dl->rend());
		it->first  = ic->second;
		it->second = this;
	    } else {
		DispList::iterator it = dl->begin();
		while (++it != dl->end()) if (depth < it->second->depth) break;
		dl->insert(it, DispList::value_type(ic->second, this));
	    }
	} else {
	    DispList::reverse_iterator it = dl->rbegin();
	    for (; it != dl->rend(); ++it)
		if (depth == it->second->depth) break;
	    assert(it != dl->rend());
	    if (has_mv) it->second = this; else dl->erase((++it).base());
	}
    }

    void dump(std::ostream& os) {
	Tag::dump(os);			os  << "  ";
	if (has_id) os  << (has_mv ? "Rep" : "P") << "lace object #"
		<< rfid << " to"; else
	    os  << (has_mv ? "M" : "Rem") << "ove object on";
	    os  << " depth #" << depth << ": 0x"
		<< std::setfill('0') << std::hex  << std::setw(4) << __
		<< std::setfill(' ') << std::dec  << std::endl;
	if (has_cls)  os  << "    CLS:  " << cls  << std::endl;
	if (has_mtx)  os  << "    MTX:  " << mtx  << std::endl;
	if (has_cxf)  os  << "    CXF:  " << cxf  << std::endl;
	if (has_mpos) os  << "    MPos: " << mpos << std::endl;
	if (has_name) os  << "    Name: " << onam.data() << std::endl;
	if (has_clip) os  << "    Clip #" << clip << std::endl;
	if (has_fltr) ;
	if (has_axn) ;
    }
};
typedef	PlaceObject3 PlaceObject2;

struct RemoveObject: public ControlTag {
    uint16_t rfid, depth;

    RemoveObject(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void load(BitStream& bs) {
	if (code == TAG::CODE::RemoveObject) bs >> rfid;
	else rfid = 0;			     bs >> depth;
    }

    void exec() {
	DispList::reverse_iterator it = dl->rbegin();
#if 0
	if (rfid) for (; it != ie; ++it)
	    if (depth == it->second->depth &&
			 it->first->id == rfid) break; else
#endif
	for (; it != dl->rend(); ++it) if (depth == it->second->depth) break;
	assert(it != dl->rend());	dl->erase((++it).base());
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Remove object #" << rfid
	    << " in depth #" << depth << std::endl;
    }
};
typedef	RemoveObject RemoveObject2;

struct DoAction: public ControlTag {
    uint16_t spid;	// XXX:
    std::vector<Action*> axVec;

    ~DoAction() {
	for (std::vector<Action*>::iterator it = axVec.begin();
		it != axVec.end(); ++it) delete *it;
    }

     DoAction(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void load(BitStream& bs) {		Action* ax;
	if (code == TAG::CODE::DoInitAction) bs >> spid; else spid = 0;
	do { ax = new Action;		bs >> *ax;
	     axVec.push_back(ax);
	} while (ax->id != ACTION::CODE::End);
    }

    void exec() {   // TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	for (std::vector<Action*>::iterator it = axVec.begin();
		it != axVec.end(); ++it) os << **it;
    }
};
typedef DoAction DoInitAction;

struct DefineSound: public DefineTag {
    enum {  FMT_RAW, FMT_ADPCM, FMT_MP3, FMT_RAW_LE,
	    FMT_NELLYMOSER_16K, FMT_NELLYMOSER_8K,
	    FMT_NELLYMOSER,	FMT_SPEEX = 11u, };

    //uint16_t id;
    union {	 uint8_t _;		// stereo: LRLRLR...
	struct { uint8_t stereo:1, s16:1, rate:2, fmtn:4; };
    };
    uint32_t smpl;
    uint8_t* data;

    ~DefineSound() { delete[] data; }
     DefineSound(Tag& t, BitStream& bs): DefineTag(t, bs) { load(bs); }

    void decode(bool m2s) {		// TODO: streamized decoding
	switch (fmtn) {
	case FMT_RAW:
	case FMT_ADPCM:
	case FMT_MP3:
	case FMT_RAW_LE:
	case FMT_NELLYMOSER_16K:
	case FMT_NELLYMOSER_8K:
	case FMT_NELLYMOSER:
	case FMT_SPEEX:
	default:;
	}
    }

    void load(BitStream& bs) {
	uint32_t len = size - 7u;	bs >> _ >> smpl;
	bs.read((char*)(data = new uint8_t[len]), len);		si = NULL;
    }

    void show(AttrInfo* attr) {		// TODO: decode-output-free
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Object(sound): #" << id << " of " << std::setw(5)
	    << DefineSound::get_rate(rate) << " Hz, "
	    << std::setw(2) << (0x08 << s16) << " bits, "
	    << (stereo ? "stereo" : "mono") << ", "
	    << DefineSound::get_fmts(fmtn) << "("
	    << (int)fmtn << "), " << smpl << " samples\n";
    }

    static uint16_t get_rate(uint8_t ir)
	    { return ((11025u * (0x01 << ir)) >> 1); }
    static const char* get_fmts(uint8_t fmtn) {
	static const char* fmts[16] = {
	    "raw", "adpcm", "mp3", "rawle", "nellymoser(16kHz)",
	    "nellymoser(8kHz)", "nellymoser", "speex", NULL,
	};	    return fmts[fmtn];
    }

    SoundInfo* si;
};

struct SoundStreamHead2;

struct SoundStreamBlock: public ControlTag {	// XXX:
    uint8_t* data;

    ~SoundStreamBlock() { delete[] data; }
     SoundStreamBlock(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void decode(bool m2s) {
    }

    void load(BitStream& bs) {
	bs.read((char*)(data = new uint8_t[size]), size);
	//if (last) last->next = this;	next = NULL;
    }

    void exec() {   // TODO: decode-output-free
    }

    static SoundStreamHead2* head;  // XXX:
    //static SoundStreamBlock* last;
    //SoundStreamBlock* next;
};

struct SoundStreamHead2: public ControlTag {
    union {
	struct {
	    uint16_t pbstereo:1, pbsize:1, pbrate:2, xxxx:4;	// playback
	    uint16_t sdstereo:1, sdsize:1, sdrate:2, fmtn:4;	// sound-orig
	};  uint16_t _;
    };	    uint16_t smpl;		int16_t  seek;		// MP3 only

    SoundStreamHead2(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void load(BitStream& bs) {		 bs >> _ >> smpl;
	if (fmtn == DefineSound::FMT_MP3)bs >> seek; else seek = 0;
	SoundStreamBlock::head = this;	//SoundStreamBlock::last = NULL;
    }

    void exec() { }	// TODO:

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Playback:     " << std::setw(5)
	    << DefineSound::get_rate(pbrate) << " Hz, "
	    << std::setw(2) << (0x08 << pbsize) << " bits, "
	    << (pbstereo ? "stereo" : "mono") << std::endl
	    << "  Sound stream: " << std::setw(5)
	    << DefineSound::get_rate(sdrate) << " Hz, "
	    << std::setw(2) << (0x08 << sdsize) << " bits, "
	    << (sdstereo ? "stereo" : "mono")
	    << ", " << DefineSound::get_fmts(fmtn)
	    << ", " << smpl << " samples";
	if (seek) os << ", seek to " << seek;	os << std::endl;
    }
};
typedef	SoundStreamHead2 SoundStreamHead;

#if 1
struct StopSound: public ControlTag {
    StopSound(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void load(BitStream& bs) { bs.ignore(size); }

    void exec() {	// TODO:
	DispList::iterator it = dl->begin();
	while (++it != dl->end())
	    if (it->first->code == TAG::CODE::DefineSound) break;
	dl->erase(it);	// XXX:
    }
};
#else
typedef TagUnknown StopSound;
#endif

struct StartSound: public ControlTag, public AttrInfo {
    SoundInfo si;

    StartSound(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void load(BitStream& bs) { bs >> si; }

    void exec() {
	DispList  ::iterator it = dl->begin();
	Dictionary::iterator ic = DefineTag::di->find(si.rfid);
	assert(ic != DefineTag::di->end() &&
	       ic->second->code == TAG::CODE::DefineSound);

	rfid = si.rfid, depth = 0u;
	while (++it != dl->end()) if (depth < it->second->depth) break;
	dl->insert(it, DispList::value_type(ic->second, this));
    }

    void dump(std::ostream& os) {	Tag::dump(os);	    os << si; }
};

struct FrameLabel: public ControlTag {
    std::string label;
    uint8_t flags;

    FrameLabel(Tag& t, BitStream& bs): ControlTag(t) { load(bs); }

    void load(BitStream& bs) {			  bs >> label;
	if (5u < swfv && label.size() < size - 1) bs >> flags;
	else flags = 0u;
    }

    void exec() { }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Frame label: " << label.data() << "(0x"
	    << std::setfill('0') << std::hex << std::setw(4) << flags << ")\n"
	    << std::setfill(' ') << std::dec;
    }
};

struct SetBackgroundColor: public DefineTag { // It _is_ a control tag.
    RGB rgb;

    SetBackgroundColor(Tag& t, BitStream& bs): DefineTag(t) { load(bs); }

    void load(BitStream& bs) {
	bs >> rgb;	di->clear();
	attr.rfid = attr.depth = 0u;
	ControlTag::dl->insert(ControlTag::dl->begin(),
		DispList::value_type(this, NULL));
    }

    void show(AttrInfo*) { fr->SetBackgroundColor(rgb); }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  " << rgb << std::endl;
    }

    AttrInfo attr;	// XXX:
};

#if 0
struct DefineBinaryData: public DefineTag {
    /* The DefineBinaryData tag permits arbitrary binary data to be embedded
     * in a SWF file.  DefineBinaryData is a definition tag, like DefineShape
     * and DefineSprite. It associates a blob of binary data with a standard
     * SWF 16-bit character ID. The character ID is entered into the SWF
     * file's character dictionary.
     *
     * DefineBinaryData is intended to be used in conjunction with the
     * SymbolClass tag. The SymbolClass tag can be used to associate a
     * DefineBinaryData tag with an AS3 class definition.
     *
     * The AS3 class must be a subclass of ByteArray. When the class is
     * instantiated, it will be populated automatically with the contents of
     * the binary data resource.
     */

    uint8_t* data;

     DefineBinaryData(Tag& t, BitStream& bs): DefineTag(t) { load(bs); }
    ~DefineBinaryData() { delete[] data; }

    void load(BitStream& bs) {
	union { uint32_t reserved, len; };
	bs >> reserved;
	len = size - sizeof(reserved);
	data = new uint8_t[len];
	bs.read((char*)data, len);
    }

    void show(AttrInfo* attr) {		// TODO:
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	dump_data(std::clog, data, size - 4);
    }
};
#endif

struct ShowFrame: public ControlTag {
    ShowFrame(Tag& t, BitStream& bs): ControlTag(t) { ++fl->lf; }

    void curs(uint16_t x, uint16_t y) {
	// TODO: draw a cursor on the frame
    }

    void exec() { }   // TODO:
};

struct DefineBitsPtr: public Tag {
    uint32_t rfid;

    DefineBitsPtr(Tag&t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) { bs >> rfid; }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Reference to bitmap #" << rfid << std::endl;
    }
};

struct ScriptLimits: public Tag {
    uint16_t recursion, timeout;

    ScriptLimits(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) { bs >> recursion >> timeout; }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Set max recursion depth: " << recursion
	    << ", default timeout: " << timeout << 's' << std::endl;
    }
};

struct SetTabIndex: public Tag {
    uint16_t depth, tidx;

    SetTabIndex(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) { bs >> depth >> tidx; }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  Set object in depth #" << depth
	    << " to in tab-index: " << tidx << std::endl;
    }
};

#if 1
struct ProtectDebug: public Tag {
    //uint16_t reserved;		// useless
    std::string md5pass;

    ProtectDebug(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) {
	if (4u < swfv && size) {	// XXX:
	    if (code != TAG::CODE::ProtectDebug2) {
		uint16_t xxxx;	bs >> xxxx;
	    }			bs >> md5pass;
	}
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	os  << "  MD5 password: `" << md5pass.data() << "'\n";
    }
};
#else
typedef TagDefault ProtectDebug;
#endif
typedef	ProtectDebug Protect;
typedef	ProtectDebug ProtectDebug2;

struct ExportAssets: public Tag {
    //uint16_t count;			// useless in vector container
    std::vector<External> exVec;	// XXX:

    ExportAssets(Tag& t): Tag(t) { }
    ExportAssets(Tag& t, BitStream& bs): Tag(t) { load(bs); }

    void load(BitStream& bs) {	uint16_t count;
	bs >> count;		exVec.resize(count);
	for (uint16_t i=0; i < count; ++i) bs >> exVec[i];
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	uint16_t i, count = exVec.size();
	for (i=0; i < count; ++i)
	    os  << "  ID #" << exVec[i].rfid
		<< ": " << exVec[i].smbl << std::endl;
    }
};

struct ImportAssets: public ExportAssets {
    std::string url;

    ImportAssets(Tag& t, BitStream& bs): ExportAssets(t) { load(bs); }

    void load(BitStream& bs) {
	bs >> url;		ExportAssets::load(bs);
    }

    void dump(std::ostream& os) {	Tag::dump(os);
	uint16_t i, count = exVec.size();
	os  << "  URL: " << url.data() << std::endl;
	for (i=0; i < count; ++i)
	    os  << "  ID #" << exVec[i].rfid
		<< ": " << exVec[i].smbl << std::endl;
    }
};

};

#endif//TAG_HPP
// vim:sts=4:ts=8:
