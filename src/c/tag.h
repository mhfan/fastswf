//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: tag.h          Thu, 30 Mar 2006 20:24:14 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  范美辉(MeiHui FAN)  <mhfan@ustc.edu>            *
 *                                                              *
 * CopyRight (c)  2006  M.H.Fan                                 *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it              *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/
#ifndef TAG_H
#define TAG_H

#include <stdint.h>

struct __attribute__((packed)) swf_tag_hdr {
    union {
        struct {
            // 0x0000~0x01ff for future use, 0x0200~0x03ff for third party use
            uint16_t siz_ : 6, code : 10;
        };  uint16_t _;                 // XXX: lobs, hibs;
    };      uint32_t size;              // when siz_ is 0x3f
};

#if 1
enum swf_tag_code {
//    SWF_TAG_FileHeader        = None  //Control
// We can consider the file header as being a tag though it isn't a tag per say.

    SWF_TAG_End                 =  0,   //Control
// Mark the end of the file. It can't appear anywhere else but the end of file.

    SWF_TAG_ShowFrame           =  1,   //Definition
// Display the current display list and pauses for 1 frame time.

    SWF_TAG_DefineShape         =  2,   //Definition
// Define a simple geometric shape.

    SWF_TAG_FreeCharacter       =  3,   //Definition    // XXX: Unknown
// Release a character which won't be used anymore.

    SWF_TAG_PlaceObject         =  4,   //Display
// Place the specified object in the current display list.

    SWF_TAG_RemoveObject        =  5,   //Display
// Remove the specified object at the specified depth.

    SWF_TAG_DefineBits          =  6,   //Definition
// Define a JPEG bit stream.

    SWF_TAG_DefineButton        =  7,   //Definition
// Define an action button.

    SWF_TAG_JPEGTables          =  8,   //Definition
// Define the tables used to compress/decompress all the SWF 1.0 JPEG images.

    SWF_TAG_SetBackgroundColor  =  9,   //Display
// Change the background color.

    SWF_TAG_DefineFont          = 10,   //Definition
// List shapes corresponding to glyphs.

    SWF_TAG_DefineText          = 11,   //Definition
// Defines a text of characters displayed using a font without transparency.

    SWF_TAG_DoAction            = 12,   //Action
// Actions to perform at the time the next show frame is reached and
// before the result is being displayed. It can duplicate sprites,
// start/stop movie clips, etc.
// All the actions within a frame are executed sequencially in the order
// they are defined.

    SWF_TAG_DefineFontInfo      = 13,   //Definition
// Information about a previously defined font. Includes style, a map and name.

                            /* SWF Version 2.0 */

    SWF_TAG_DefineSound         = 14,   //Definition
// Declare a sound effect. This includes sound samples which can later be
// played back using either a StartSound or a DefineButtonSound.
// Note that the same DefineSound block can actually include multiple
// sound files and only part of the entire sound can be played back as required.

    SWF_TAG_StartSound          = 15,   //Display
// Start the referenced sound on the next DisplayFrame.

    SWF_TAG_StopSound           = 16,   //Display   // XXX: Unknown
// Stop the referenced sound on the next DisplayFrame.

    SWF_TAG_DefineButtonSound   = 17,   //Definition
// Defines how to play a sound effect for the given button when an event occurs.

    SWF_TAG_SoundStreamHead     = 18,   //Definition
// Declare a sound effect which will be interleaved with a movie data so as to
// be loaded over a network connection while being played.

    SWF_TAG_SoundStreamBlock    = 19,   //Definition
// A block of sound data. The size of this block of data will be defined in
// the previous SoundStreamHead tag. It can be used so sound is downloaded on
// a per frame basis instead of being loaded all at once.

    SWF_TAG_DefineBitsLossless  = 20,   //Definition
// A bitmap compressed using ZLIB (similar to the PNG format).

    SWF_TAG_DefineBitsJPEG2     = 21,   //Definition
// Declare a complete JPEG image (includes the bit stream and the tables all
// in one thus enabling multiple table to be used within the same SWF file).

    SWF_TAG_DefineShape2        = 22,   //Definition
// Declaration of complex 2D shapes.

    SWF_TAG_DefineButtonCxform  = 23,   //Definition
// Setup a color transformation for a button.

    SWF_TAG_Protect             = 24,   //Control
// Disable edition capabilities of the given SWF file.
// Though this doesn't need to be enforced by an SWF editor,
// it marks the file as being copyrighted in a way.

                            /* SWF Version 3.0 */

    SWF_TAG_PathsArePostscript  = 25,   //Display   // XXX: Unknown
// The shape paths are defined as in postscript?

    SWF_TAG_PlaceObject2        = 26,   //Display
// Place an object in the current display list.

// XXX: 27

    SWF_TAG_RemoveObject2       = 28,   //Display
// Remove the object at the specified level.

    SWF_TAG_SyncFrame           = 29,   //Display   // XXX: Unknown
// ?

// XXX: 30

    SWF_TAG_FreeAll             = 31,   //Display   // XXX: Unknown
// ?

    SWF_TAG_DefineShape3        = 32,   //Definition
// Declare a simple geometric shape.

    SWF_TAG_DefineText2         = 33,   //Definition
// Defines a text of characters displayed using a font with transparency.

    SWF_TAG_DefineButton2       = 34,   //Definition
// Define an action button. Includes a color transformation.

    SWF_TAG_DefineBitsJPEG3     = 35,   //Definition
// Defines a complete JPEG (includes the tables and data bit stream) twice.
// Once with the RGB data and once with the alpha channel. Note that the alpha
// channel uses the Z-lib compression instead of the JPEG one.

    SWF_TAG_DefineBitsLossless2 = 36,   //Definition
// Defines an RGBA bitmap compressed using ZLIB (similar to the PNG format).

    SWF_TAG_DefineSprite        = 39,   //Definition
// Declares an animated character. This is similar to a shape with a display
// list so the character can be changing on its own over time.

    SWF_TAG_NameCharacter       = 40,   //Definition    // XXX: Unknown
// Define the name of an object (for buttons, bitmaps, sprites and sounds)

    SWF_TAG_SerialNumber        = 41,   //Definition    // XXX: Unknown
// ?

    SWF_TAG_DefineTextFormat    = 42,   //Definition    // XXX: Unknown
// ?

    SWF_TAG_FrameLabel          = 43,   //Definition
// Names a frame or anchor. This frame can later be referenced using this name.

// XXX: 44

    SWF_TAG_SoundStreamHead2    = 45,   //Definition
// Declare a sound effect which will be interleaved with a movie data so as to
// be loaded over a network connection while being played.

    SWF_TAG_DefineMorphShape    = 46,   //Definition
// This is similar to a sprite with a simple morphing between two shapes.

    SWF_TAG_GenerateFrame       = 47,   //Definition    // XXX: Unknown
// ?

    SWF_TAG_DefineFont2         = 48,   //Definition
// Define a list of glyphs using shapes and other font metric like information.

    SWF_TAG_GeneratorCommand    = 49,   //Definition
// Gives some information about the tool which generated this SWF file and its
// version.

                            /* SWF Version 4.0 */

    SWF_TAG_DefineEditText      = 37,   //Definition
// An edit text enables the end users to enter text in a Flash window.

    SWF_TAG_DefineVideo         = 38,   //Definition    // XXX: Unknown
// ?

                            /* SWF Version 5.0 */

    SWF_TAG_DefineCommandObject = 50,   //Definition    // XXX: Unknown
// ?

    SWF_TAG_CharacterSet        = 51,   //Definition    // XXX: Unknown
// ?

    SWF_TAG_ExternalFont        = 52,   //Definition    // XXX: Unknown
// ?

// XXX: 53

// XXX: 54

// XXX: 55

    SWF_TAG_Export              = 56,   //Definition
// Exports a list of definitions to other movies. You can in this way create
// one or more movies to hold a colection of objects to be reused by other
// movies without having to insert these info in each movie. A single export
// is enough for an entire movie (and you should have just one).

    SWF_TAG_Import              = 57,   //Definition
// Imports a list of definitions from another movie. You can retrieve objects
// which were exported in the specified movie. You can have as many import as
// you like, though you should have only one per referenced movie.

    SWF_TAG_ProtectDebug        = 58,   //Control
// The data of this tag is an MD5 password like the Protect tag. When it
// exists and you know the password, you will be given the right to debug the
// movie with Flash V5.x.  WARNING: use ProtectDebug2 instead in V6.x movies.

                            /* SWF Version 6.0 */

    SWF_TAG_DoInitAction        = 59,   //Action
// Actions to perform the first time the following show frame is reached. All
// the initialization actions are executed before any other actions. You have
// to specify a sprite to which the actions are applied. Thus you don't need a
// set target action. When multiple initialization action blocks are within
// the same frame, they are executed one after another.

    SWF_TAG_DefineVideoStream   = 60,   //Definition
// Defines the necessary information for the player to display a video stream
// (i.e. size, codec, how to decode the data, etc.). Play the frames with
// VideoFrame tags.

    SWF_TAG_VideoFrame          = 61,   //Definition
// Show the specified video frame of a movie.

    SWF_TAG_DefineFontInfo2     = 62,   //Definition
// Defines information about a font, like the DefineFontInfo tag plus a
// language reference. To force the use of a given language, this tag should
// be used in v6.x+ movies instead of the DefineFontInfo.

// XXX: 63

    SWF_TAG_ProtectDebug2       = 64,   //Control
// The data of this tag is a 16 bits word followed by an MD5 password like the
// Protect tag. When it exists and you know the password, you will be given
// the right to debug the movie with Flash V6.x and over.

                            /* SWF Version 7.0 */

    SWF_TAG_ScriptLimits        = 65,   //Definition
// Change limits used to ensure scripts don't use more resources than you
// choose. In version 7, it supports a maximum recursive depth and a maximum
// amount of time scripts can be run for in seconds.

    SWF_TAG_SetTabIndex         = 66,   //Definition
// Define the order index so the player knows where to go next when the Tab
// key is pressed by the user.

                            /* SWF Version 8.0 */

// XXX: 67

// XXX: 68

// XXX: 69

    SWF_TAG_PlaceObject3        = 70,   //Definition
// Place an object in the display list. The object can include bitmap caching
// information, a blend mode and a set of filters.

    SWF_TAG_MaxTagNumber        //= 1024
};
#else

                            /* SWF Version 1.0 */

#define SWF_TAG_FileHeader              //None  //Control
// We can consider the file header as being a tag though it isn't a tag per say.

#define SWF_TAG_End                     0       //Control
// Mark the end of the file. It can't appear anywhere else but the end of file.

#define SWF_TAG_ShowFrame               1       //Definition
// Display the current display list and pauses for 1 frame time.

#define SWF_TAG_DefineShape             2       //Definition
// Define a simple geometric shape.

#define SWF_TAG_FreeCharacter           3       //Definition    // XXX: Unknown
// Release a character which won't be used anymore.

#define SWF_TAG_PlaceObject             4       //Display
// Place the specified object in the current display list.

#define SWF_TAG_RemoveObject            5       //Display
// Remove the specified object at the specified depth.

#define SWF_TAG_DefineBits              6       //Definition
// Define a JPEG bit stream.

#define SWF_TAG_DefineButton            7       //Definition
// Define an action button.

#define SWF_TAG_JPEGTables              8       //Definition
// Define the tables used to compress/decompress all the SWF 1.0 JPEG images.

#define SWF_TAG_SetBackgroundColor      9       //Display
// Change the background color.

#define SWF_TAG_DefineFont              10      //Definition
// List shapes corresponding to glyphs.

#define SWF_TAG_DefineText              11      //Definition
// Defines a text of characters displayed using a font without transparency.

#define SWF_TAG_DoAction                12      //Action
// Actions to perform at the time the next show frame is reached and
// before the result is being displayed. It can duplicate sprites,
// start/stop movie clips, etc.
// All the actions within a frame are executed sequencially in the order
// they are defined.

#define SWF_TAG_DefineFontInfo          13      //Definition
// Information about a previously defined font. Includes style, a map and name.

                            /* SWF Version 2.0 */

#define SWF_TAG_DefineSound             14      //Definition
// Declare a sound effect. This includes sound samples which can later be
// played back using either a StartSound or a DefineButtonSound.
// Note that the same DefineSound block can actually include multiple
// sound files and only part of the entire sound can be played back as required.

#define SWF_TAG_StartSound              15      //Display
// Start the referenced sound on the next DisplayFrame.

#define SWF_TAG_StopSound               16      //Display   // XXX: Unknown
// Stop the referenced sound on the next DisplayFrame.

#define SWF_TAG_DefineButtonSound       17      //Definition
// Defines how to play a sound effect for the given button when an event occurs.

#define SWF_TAG_SoundStreamHead         18      //Definition
// Declare a sound effect which will be interleaved with a movie data so as to
// be loaded over a network connection while being played.

#define SWF_TAG_SoundStreamBlock        19      //Definition
// A block of sound data. The size of this block of data will be defined in
// the previous SoundStreamHead tag. It can be used so sound is downloaded on
// a per frame basis instead of being loaded all at once.

#define SWF_TAG_DefineBitsLossless      20      //Definition
// A bitmap compressed using ZLIB (similar to the PNG format).

#define SWF_TAG_DefineBitsJPEG2         21      //Definition
// Declare a complete JPEG image (includes the bit stream and the tables all
// in one thus enabling multiple table to be used within the same SWF file).

#define SWF_TAG_DefineShape2            22      //Definition
// Declaration of complex 2D shapes.

#define SWF_TAG_DefineButtonCxform      23      //Definition
// Setup a color transformation for a button.

#define SWF_TAG_Protect                 24      //Control
// Disable edition capabilities of the given SWF file.
// Though this doesn't need to be enforced by an SWF editor,
// it marks the file as being copyrighted in a way.

                            /* SWF Version 3.0 */

#define SWF_TAG_PathsArePostscript      25      //Display   // XXX: Unknown
// The shape paths are defined as in postscript?

#define SWF_TAG_PlaceObject2            26      //Display
// Place an object in the current display list.

// XXX: 27

#define SWF_TAG_RemoveObject2           28      //Display
// Remove the object at the specified level.

#define SWF_TAG_SyncFrame               29      //Display   // XXX: Unknown
// ?

// XXX: 27

#define SWF_TAG_FreeAll                 31      //Display   // XXX: Unknown
// ?

#define SWF_TAG_DefineShape3            32      //Definition
// Declare a simple geometric shape.

#define SWF_TAG_DefineText2             33      //Definition
// Defines a text of characters displayed using a font with transparency.

#define SWF_TAG_DefineButton2           34      //Definition
// Define an action button. Includes a color transformation.

#define SWF_TAG_DefineBitsJPEG3         35      //Definition
// Defines a complete JPEG (includes the tables and data bit stream) twice.
// Once with the RGB data and once with the alpha channel. Note that the alpha
// channel uses the Z-lib compression instead of the JPEG one.

#define SWF_TAG_DefineBitsLossless2     36      //Definition
// Defines an RGBA bitmap compressed using ZLIB (similar to the PNG format).

#define SWF_TAG_DefineSprite            39      //Definition
// Declares an animated character. This is similar to a shape with a display
// list so the character can be changing on its own over time.

#define SWF_TAG_NameCharacter           40      //Definition    // XXX: Unknown
// Define the name of an object (for buttons, bitmaps, sprites and sounds)

#define SWF_TAG_SerialNumber            41      //Definition    // XXX: Unknown
// ?

#define SWF_TAG_DefineTextFormat        42      //Definition    // XXX: Unknown
// ?

#define SWF_TAG_FrameLabel              43      //Definition
// Names a frame or anchor. This frame can later be referenced using this name.

// XXX: 44

#define SWF_TAG_SoundStreamHead2        45      //Definition
// Declare a sound effect which will be interleaved with a movie data so as to
// be loaded over a network connection while being played.

#define SWF_TAG_DefineMorphShape        46      //Definition
// This is similar to a sprite with a simple morphing between two shapes.

#define SWF_TAG_GenerateFrame           47      //Definition    // XXX: Unknown
// ?

#define SWF_TAG_DefineFont2             48      //Definition
// Define a list of glyphs using shapes and other font metric like information.

#define SWF_TAG_GeneratorCommand        49      //Definition
// Gives some information about the tool which generated this SWF file and its
// version.

                            /* SWF Version 4.0 */

#define SWF_TAG_DefineEditText          37      //Definition
// An edit text enables the end users to enter text in a Flash window.

#define SWF_TAG_DefineVideo             38      //Definition    // XXX: Unknown
// ?

                            /* SWF Version 5.0 */

#define SWF_TAG_DefineCommandObject     50      //Definition    // XXX: Unknown
// ?

#define SWF_TAG_CharacterSet            51      //Definition    // XXX: Unknown
// ?

#define SWF_TAG_ExternalFont            52      //Definition    // XXX: Unknown
// ?

// XXX: 53

// XXX: 54

// XXX: 55

#define SWF_TAG_Export                  56      //Definition
// Exports a list of definitions to other movies. You can in this way create
// one or more movies to hold a colection of objects to be reused by other
// movies without having to insert these info in each movie. A single export
// is enough for an entire movie (and you should have just one).

#define SWF_TAG_Import                  57      //Definition
// Imports a list of definitions from another movie. You can retrieve objects
// which were exported in the specified movie. You can have as many import as
// you like, though you should have only one per referenced movie.

#define SWF_TAG_ProtectDebug            58      //Control
// The data of this tag is an MD5 password like the Protect tag. When it
// exists and you know the password, you will be given the right to debug the
// movie with Flash V5.x.  WARNING: use ProtectDebug2 instead in V6.x movies.

                            /* SWF Version 6.0 */

#define SWF_TAG_DoInitAction            59      //Action
// Actions to perform the first time the following show frame is reached. All
// the initialization actions are executed before any other actions. You have
// to specify a sprite to which the actions are applied. Thus you don't need a
// set target action. When multiple initialization action blocks are within
// the same frame, they are executed one after another.

#define SWF_TAG_DefineVideoStream       60      //Definition
// Defines the necessary information for the player to display a video stream
// (i.e. size, codec, how to decode the data, etc.). Play the frames with
// VideoFrame tags.

#define SWF_TAG_VideoFrame              61      //Definition
// Show the specified video frame of a movie.

#define SWF_TAG_DefineFontInfo2         62      //Definition
// Defines information about a font, like the DefineFontInfo tag plus a
// language reference. To force the use of a given language, this tag should
// be used in v6.x+ movies instead of the DefineFontInfo.

// XXX: 63

#define SWF_TAG_ProtectDebug2           64      //Control
// The data of this tag is a 16 bits word followed by an MD5 password like the
// Protect tag. When it exists and you know the password, you will be given
// the right to debug the movie with Flash V6.x and over.

                            /* SWF Version 7.0 */

#define SWF_TAG_ScriptLimits            65      //Definition
// Change limits used to ensure scripts don't use more resources than you
// choose. In version 7, it supports a maximum recursive depth and a maximum
// amount of time scripts can be run for in seconds.

#define SWF_TAG_SetTabIndex             66      //Definition
// Define the order index so the player knows where to go next when the Tab
// key is pressed by the user.

                            /* SWF Version 8.0 */


// XXX: 67

// XXX: 68

// XXX: 69

#define SWF_TAG_PlaceObject3            70      //Definition
// Place an object in the display list. The object can include bitmap caching
// information, a blend mode and a set of filters.

#define SWF_TAG_MaxTagNumber            72
//#define       SWF_TAG_MaxTagNumber            1024
#endif

struct swf;

struct swf_tagpsr {
    void (*prsr)(struct swf* swf);
    char *name;//, *desc;
};

void swf_read_taghdr(struct bstream* bs, struct swf_tag_hdr* th);

void swf_set_tagpsr(struct swf_tagpsr* tagpsr, uint8_t code,
        void (*prsr)(struct swf* swf));

void swf_tagpsr_init(struct swf_tagpsr* tagpsr);

#endif//TAG_H
// vim:sts=4:ts=8:
