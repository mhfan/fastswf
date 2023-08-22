//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: displist.h     Wed, 05 Apr 2006 02:22:44 +0800  mhfan $ *
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
#ifndef DISPLIST_H
#define DISPLIST_H

/*
 * Displaying a frame of a Macromedia Flash movie is a three-stage process:
 *
 * 1. Objects are defined with definition tags such as DefineShape ,
 * DefineSprite etc. Each object is given a unique ID called a character—,
 * and stored in a repository called the dictionary.
 *
 * 2. Selected characters are copied from the dictionary and placed on the
 * display list. This is the list of the characters that will be displayed in
 * the next frame.
 *
 * 3. Once complete, the contents of the display list are rendered to the
 * screen with ShowFrame
 *
 * Each character on the display list is assigned a depth value. The depth
 * determines the stacking order of the character. Characters with lower depth
 * values are displayed underneath characters with higher depth values. A
 * character with a depth value of 1 is displayed at the bottom of the stack.
 * A character may appear more than once in the display list, but at different
 * depths. There can be only one character at any given depth.
 *
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
 *       1). A depth value. This controls the stacking order of the character
 *       being placed. Characters with lower depth values appear to be
 *       underneath characters with higher depth values. A depth value of 1
 *       means the character is displayed at the bottom of the stack. There
 *       can be only one character at any given depth.
 *
 *       2). A transformation matrix. This determines the position, scale,
 *       factor, and angle of rotation of the character being placed. The same
 *       character may be placed more than once (at different depths) with a
 *       different transformation matrix.
 *
 *       3). An optional color transform. This specifies the color effect
 *       applied to the character being placed. Color effects include
 *       transparency and color shifts.
 *
 *       4). An optional name string. This identifies the character being
 *       placed for SetTarget actions. SetTarget is used to perform actions
 *       inside sprite objects.
 *
 *       5). An optional ClipDepth value. This specifies the top-most depth
 *       that will be masked by the character being placed.
 *
 *       6). An optional ratio value. This controls how a morph character is
 *       displayed when placed. A ratio of zero displays the character at the
 *       start of the morph. A ratio of 65535 displays the character at the
 *       end of the morph.
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
 *
 */

#define swf_character                   swf_definition
struct swf_definition {
};

struct swf_dictionary {
    struct swf_definition* next;
};

#define swf_dispitem                    swf_displist
struct swf_displist {
    struct swf_definition* next;
};

#endif//DISPLIST_H
// vim:sts=4:ts=8:
