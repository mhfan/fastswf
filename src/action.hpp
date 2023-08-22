/****************************************************************
 * $ID: action.hpp     Thu, 06 Apr 2006 14:56:17 +0800  mhfan $ *
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
#ifndef ACTION_HPP
#define ACTION_HPP

#include <iomanip>
#include <fstream>

#include <string>
#include <vector>
#include <map>

#include "bs.hpp"
#include "type.hpp"

//#define USE_FIXED_POINT                       1
#include "math-sll.h"

namespace SWF {

void dump_data(std::ostream& os, uint8_t* data, uint16_t size);

namespace ACTION {

namespace CODE {

    /*
     * Different actions are supported in different version, so please, look
     * at the version when attempting to use that action.
     *
     * Some actions have been deprecated and should not be used in newer
     * version of Flash (mainly the untyped operators.)
     *
     * There are two schemes supported in Flash 9 and over: ActionScript 2 and
     * 3 (also referenced as AS2 and AS3.)
     *
     * AS2 was created in Flash animations version 1, enhanced in versions 3,
     * 4 and 5. In version 5, it because AS1. Version 6 greatly fixed many of
     * the problems in older versions. Version 7 and 8 only added a few more
     * features. Version 9 makes use of Tamarin which is a much more advanced
     * mechanism used to execute the compiled ActionScript code. The huge
     * improvement comes from the header that gives the system access to all
     * the functions and variables without having to search for them through
     * the action script.
     */

enum ActionCode {
#define UnusedAction(n) UnusedAction_##n = n

    End                 = 0x00,
    /* v1: <n.a.>
     * Category: Miscellaneous
     * Structure: <n.a.>
     * Operation: <n.a.>
     *
     * End a record of actions. There are no valid instances where this action
     * is optional.
     *
     * The End action itself as no meaning other than marking the end of the
     * list of actions. Yet, if reached, the execution of the script ends and
     * is considered complete.
     *
     * IMPORTANT NOTE: This action ends a complete list of actions. Declare
     * Function, With, try/catch/finally and other blocks of actions are never
     * ended with this action. Instead, internal blocks have a size in bytes1
     * that is used to determine the end of the block.
     */

    UnusedAction(0x01),
    UnusedAction(0x02),
    UnusedAction(0x03),

    NextFrame           = 0x04,
    /* v1: <n.a.>
     * Category: Movie
     * Structure: <n.a.>
     * Operation: goto(current_frame + 1);
     *
     * Go to the next frame. This means the next Show Frame tag will be hit.
     * This action does not change the play status.
     */

    PrevFrame           = 0x05,
    /* v1: <n.a.>
     * Category: Movie
     * Structure: <n.a.>
     * Operation: goto(current_frame - 1);
     *
     * Go to the previous frame. This is the opposite of the Next Frame action.
     */

    Play                = 0x06,
    /* v1: <n.a.>
     * Category: Movie
     * Structure: <n.a.>
     * Operation: target_movie.play();
     *
     * Enter the standard (default) auto-loop playback. The action only
     * affects the current target.
     */

    Stop                = 0x07,
    /* v1: <n.a.>
     * Category: Movie
     * Structure: <n.a.>
     * Operation: stop();
     *
     * Stop playing the current target (remain on the same ShowFrame.)
     *
     * Only a button, another script, or the plugin menu can be used to
     * restart the movie.
     */

    ToggleQuality       = 0x08,
    /* v1: <n.a.>
     * Category: Movie
     * Structure: <n.a.>
     * Operation:
     *  if (_root.quality == HIGH_QUALITY) {
     *      _root.set_quality(LOW_QUALITY);
     *  } else {
     *      _root.set_quality(HIGH_QUALITY);
     *  }
     *
     * Change the quality level from low to high and vice versa. At this time,
     * not sure what happens if you use medium!
     *
     * Note that the quality is defined on the root only and affects the
     * entire output.
     *
     * Newer SWF versions (since version 5) should use the movie quality
     * variable member instead of this direct action.
     */

    StopSound           = 0x09,
    /* v2: <n.a.>
     * Category: Sound
     * Structure: <n.a.>
     * Operation: stop_sound();
     *
     * This action is a global action that stops all sound effects at once.
     */

    Add                 = 0x0A,
    /* v4: pop 2 (n), push 1 (n)
     * Category: Arithmetic
     * Structure: <n.a.>
     * Operation:
     *  n1 := pop();
     *  n2 := pop();
     *  r := n2 + n1;
     *  push(r);
     *
     * This action pops two numbers from the stack, add them together and put
     * the result back on the stack.
     *
     * IMPORTANT NOTE: This instruction is not compliant to the ECMA Script
     * reference. It should only be used in SWF files using version 4. Since
     * version 5, the Add (typed) action should be used instead.
     */

    Subtract            = 0x0B,
    /* v4: pop 2 (n), push 1 (n)
     * Category: Arithmetic
     * Structure: <n.a.>
     * Operation:
     *  n1 := pop();
     *  n2 := pop();
     *  r := n2 - n1;
     *  push(r);
     *
     * This action pops two values, subtract the first one from the second and
     * put the result back on the stack.
     */

    Multiply            = 0x0C,
    /* v4: pop 2 (n), push 1 (n)
     * Category: Arithmetic
     * Structure: <n.a.>
     * Operation:
     *  n1 := pop();
     *  n2 := pop();
     *  r := n2 * n1;
     *  push(r);
     *
     * Pop two values, multiply them and put the result back on the stack.
     */

    Divide              = 0x0D,
     /* v4: pop 2 (n), push 1 (n)
     * Category: Arithmetic
     * Structure: <n.a.>
     * Operation:
     *  n1 := pop();
     *  n2 := pop();
     *  r := n2 / n1;
     *  push(r);
     *
     * Pop two values, devide them and put the result back on the stack.
     *
     * The numbers are always transformed to floating points. Use the Integral
     * Part action on the result to extract an integer.
     *
     * If n1 is zero, the result NaN, Infinity, or -Infinity is pushed to the
     * stack in SWF 5 and later. In SWF 4, the result is the string #ERROR#.
     */

    Equal               = 0x0E,
    Equals              = 0x0E,
    /* v4: pop 2 (a), push 1 (b)
     * Category: Comparisons
     * Structure: <n.a.>
     * Operation:
     *  a1 := pop();
     *  a2 := pop();
     *  r := a2 == a1;
     *  push(r);
     *
     * Pop two values, compare them for equality and put the Boolean result
     * back on the stack.
     *
     * If the numbers are equal, true is pushed to the stack for SWF 5 and
     * later. For SWF 4, 1 is pushed to the stack. Otherwise, false is pushed
     * to the stack for SWF 5 and later.
     *
     * The != is created by adding a Logical Not after the Equal action.
     *
     * The way the values are converted is not clearly documented. The fact is
     * that this operation generally transforms the strings into integers or
     * floating points which is not ECMA Script compliant.
     *
     * This action should only be used in SWF version 4.
     */

    Less                = 0x0F,
    LessThan            = 0x0F,
    /* v4: pop 2 (a), push 1 (b)
     * Category: Comparisons
     * Structure: <n.a.>
     * Operation:
     *  a1 := pop();
     *  a2 := pop();
     *  r := a2 < a1;
     *  push(r);
     *
     * Pop two values, compare them for inequality and put the Boolean result
     * back on the stack.
     *
     * Other comparison operators:
     *
     *  LessThan or Equal    (n2 <= n1)     Swap + LessThan + LogicalNot
     *  GreaterThan or Equal (n2 >= n1)     LessThan + LogicalNot
     *  GreaterThan          (n2 >  n1)     Swap + LessThan
     *
     * Note that this operator should only be used in version 4. Since version
     * 5, it is better to use LessThan (typed) or GreaterThan (typed).
     */

    LogicalAnd          = 0x10,
    /* v4: pop 2 (b), push 1 (b)
     * Category: Logical and Bitwise
     * Structure: <n.a.>
     * Operation:
     *  b1 := pop();
     *  b2 := pop();
     *  r := b2 && b1;
     *  push(r);
     */

    LogicalOr           = 0x11,
    /* v4: pop 2 (b), push 1 (b)
     * Category: Logical and Bitwise
     * Structure: <n.a.>
     * Operation:
     *  b1 := pop();
     *  b2 := pop();
     *  r := b2 || b1;
     *  push(r);
     */

    LogicalNot          = 0x12,
    /* v4: pop 1 (b), push 1 (b)
     * Category: Logical and Bitwise
     * Structure: <n.a.>
     * Operation:
     *  b1 := pop();
     *  r := ! b1;
     *  push(r);
     */

    StringEqual         = 0x13, //*v4: Pops two strings, compute the
    // equality and put the boolean result back on the stack.
    // 0 (b)    operation: s2 == s1

    StringLength        = 0x14, // v4: Pops two strings, compute the
    // equality and put the boolean result back on the stack.
    // 0 (n)    operation: strlen(s1)

    SubString           = 0x15, // v4: Pops two values and one string,
    // the first value is the new string size (at most that many characters)
    // and the second value is the index (1 based) of the first character to
    // start the copy from. The resulting string is pushed back on the stack.
    // 0 (s)    operation: s3[i2 .. i2 + i1 - 1]

                        //0x16

    Pop                 = 0x17, // v4: Pops a value from the stack and drop it.
    // 0 (i)    operation: a1

    IntegralPart        = 0x18, // v4: Pops one value, transform in an integer,
    // and push the result back on the stack.
    // 0 (i)    operation: floor(n1)

    // 0x19~0x1B

    GetVariable         = 0x1C, // v4: Pops one string, search for a variable
    // of that name, and push its value on the stack.
    // 0 (a)    operation: GetVariable(s1)

    /*
     * The variable name can include sprite names separated by slashes and
     * finished by a colon as in:
     *      /Sprite1/Sprite2:MyVar
     * This gets the variable named MyVar from the sprite named Sprite2 which
     * resides in Sprite1. Note that in a browser you can add variables at the
     * end of the movie URL (as defined in the W3C docs) and these will auto-
     * matically be accessible via the GetVariable instruction.
     *      my_movie?language=jp
     * Since Flash V5.x, there are internal variables available to you. These
     * can be read with the Get Variable instruction.
     */

    SetVariable         = 0x1D, // v4: Pops one value and one string, set the
    // variable of that name with that value. Nothing is pushed back on the
    // stack. The names can be full paths as defined in the GetVariable action.
    // 0        operation: s2 = v1

                        //0x1E~0x1F

    SetTarget2          = 0x20, // v3: Pops one string from the stack.
    // If the string is the empty string, then the next actions apply to the
    // main movie. Otherwise it is the name of a sprite and the followings
    // actions apply to that sprite only.
    // 0        operation: SetTarget2(s1)

    ConcatenateStrings  = 0x21, // v4: Pops two strings, concatenate them,
    // push the result on the stack.
    // 0 (s)    operation: s2 & s1

    GetProperty         = 0x22, // v4: Query the property 'n1' of the object
    // named 's2' (a field in a structure if you wish), and push the result
    // on the stack. See the table below for a list of possible properties
    // (or fields) values.
    // 0 (a)    operation: s2.n1

    SetProperty         = 0x23, // v4: Pop a value from the stack and use it
    // to set the specified named object at the specified field property.  See
    // the table below for a list of possible properties (or fields) values.
    // 0        operation: s3.n2 = a1

    DuplicateSprite     = 0x24, // v4: 's3' is the name of a sprite which is
    // copied. The new sprite has the name 's2' and is placed at depth 'i1'.
    // The depth isn't used the same way in Flash 4 and Flash 5. The depth
    // isn't clearly documented anywhere, but it looks like you need to have
    // a depth of 16384 or more for a duplicated sprite to work properly.
    // 0        operation: s3 s2 i1 Duplicate

    RemoveSprite        = 0x25, // v4: Pop the string 's1' with the name of
    // the sprite to be removed from view.
    // 0        operation: s1 Remove

    Trace               = 0x26, // v4: Print out string s1 in debugger
    // output window. Ignored otherwise.
    // 0        operation: s1 Trace

    StartDrag           = 0x27, // v4: Pop a target name (s1), a first boolean
    // (b2) -- (un)lock the center of the object to the mouse pointer,
    // a second boolean (b3) -- constrained the mouse in the following
    // rectangle (n4 to n7, representing y2, x2, y1, x1 -- NOTE: the rectangle
    // is not defined if (b3) is False). The object will follow the mouse
    // until a Stop Drag is applied or another object is defined as the object
    // to be dragged.
    // 0        operation: [n7 n6 n5 n4] b3 b2 s1 StartDrag

    StopDrag            = 0x28, // v4: Stop a drag operation.
    // 0        operation: StopDrag

    StringLessThan      = 0x29, //*v4: Pops two strings, compare them,
    // push the result back on the stack.
    // 0 (b)    operation: s2 < s1

    Throw               = 0x2A, // v7: This action pops one item and
    // returns it as an exceptional result. You can catch exceptions using
    // the Try action.
    // 0 (a)    operation: throw a1

    CastObject          = 0x2B, // v7: The Cast Object action makes sure that
    // the object o1 is an instance of the class s2. If it is the case, then
    // o1 is pushed back onto the stack. Otherwise Null is pushed back onto
    // the stack. The comparison is identical to the one applied by the
    // Instance Of action.
    // 0 (o)    operation: (s2) o1

    Implements          = 0x2C, // v7: This action declares an object as a
    // sub-class of one or more interfaces. The number of interfaces has to
    // be indicated by i2. An interface is referenced by its name
    // (which happens to be the same as the constructor function name.)
    // 0 (o)    operation: s1 implements s3, s4 ... sn (where n is i2 + 2)

    FSCommand2          = 0x2D, // v7: Execute the named command (s1)
    // passing on parameters (s3, s4 ... sn.)
    // 0 (a)    operation: fscommand2 s1, i2, s3 ... sn (where n is i2 + 2)
    // WARNING: the operation is assumed to be correct, so far I did not
    // find either proper documentation or a movie using this action to
    // make sure it was valid.

                        //0x2E~0x2F

    Random              = 0x30, // v4: Pops one number which is the maximum
    // value (not included) that the random() function will return,
    // push the generated value on the stack. 'n1' should not be zero.
    // 0 (i)    operation: random(n1)

    WStringLength       = 0x31, // v4: Pops one multi-byte string,
    // push it's length in actual character on the stack.
    // 0 (i)    operation: wclen(s1)

    Ord                 = 0x32, // v4: Pops one string, compute the ASCII
    // value of its first character and put it back on the stack.
    // 0 (i)    operation: s1[0]

    Chr                 = 0x33, // v4: Pops one integer, use it as a ASCII
    // character and push the newly created string on the stack.
    // 0 (s)    operation: i1

    GetTimer            = 0x34, // v4: Get the current movie timer in
    // milliseconds and push it on the stack. This is equivalent to
    // the number of frames which have been played so far.
    // 0 (i)    operation: movie_time()

    WSubString          = 0x35, // v4: Pops a multi-byte string,
    // get i1 characters from the position i2 (1 based) and
    // push the result back on the stack.
    // 0 (s)    operation: s3[i2 .. i2 + i1 - 1]

    WOrd                = 0x36, // v4: Pops one string, compute the multi-byte
    // value of its first character and put it back on the stack.
    // 0 (i)    operation: s1[0]

    WChr                = 0x37, // v4: Pops one integer, use it as a multi-byte
    // string character and push the newly created string on the stack.
    // 0 (s)    operation: i1

                        //0x38~0x39

    Delete              = 0x3A, // v5: Pops one string which is the name of the
    // property to be deleted. Then pop the object from which the property is
    // to be deleted.
    // 0 (undefined)    operation: delete(s1, o2)
    /*
     * It is necessary to Push Data type undefined (0x03) before the string
     * as in:
     *      96 04 00 03 00 'a' 00 3A
     *      delete("a");
     * to delete a global variable. According to some movies I have looked at,
     * the delete instruction returns some undefined value.
     */

    DeleteAll           = 0x3B, // v5: Pops one string which is the name of
    // the thread which will get all of its variables deleted.
    // 0        operation: delete(s1, o2)

    SetLocalVariable    = 0x3C, // v5: Pops a value and a variable name.
    // Create or set a local variable of that name with the (initial) value as
    // specified. The same local variable can safely be set in this way
    // multiple times. To only declare a local variable, use the
    // DeclareLocalVariable instead.
    // 0        operation: s2 = a1

    CallFunction        = 0x3D, // v5: Pops one string which is the name of
    // the function to call, pop one integer which indicates the number of
    // arguments following, pop each argument, call the named function,
    // push the result of the function on the stack. There is always a result.
    // The idea of the concept is that the function being called will use
    // its own stack, however, that's not really the case. In other words,
    // if a function stacks multiple results, they all are returned.
    // Flash V5.x offers a set of internal functions available to the end user
    // via this CallFunction instruction. Please, see the table below for a
    // complete list of these internal functions.
    // 0 (a)    operation: s1(i2, [a3, a4, a5...])

    Return              = 0x3E, // v5: Pops one object and return it to the
    // caller. The result is stacked on the caller stack, not the stack of
    // the function being executed.
    // 0 (a)    operation: return a1

    Modulo              = 0x3F, // v5: Pops two numbers, computes the modulo
    // and push the result back on the stack.   5
    // 0 (i)    operation: i2 % i1 or f2 % f1

    New                 = 0x40, // v5: Pop the class name for the new object
    // to create. Pop the number of arguments. Pop each argument. Create an
    // object of class s1. Call the constructor function (which has the same
    // name as the object class: s1). The result of the constructor is
    // discarded. Push the created object on the stack. The object should then
    // be saved in a variable or object method.
    // 0 (o)    operation: o = new s1; o.s1(i2, [a3, a4, a5 ...])

    DeclareLocalVariable= 0x41, // v5: Pops a variable name and marks it as a
    // local variable. If the variable doesn't exist yet, then its value is
    // undefined. To declare and set a local variable, use the
    // SetLocalVariable instead.
    // 0        operation: s1

    DeclareArray        = 0x42, // v5: Pops the number of elements in the
    // array. Pop one value per element and set the corresponding entry in the
    // array. The array is pushed on the stack. It can later be sent to a
    // function or set in a variable.
    // 0 (t)    operation: new array[i1]
    // array[0] = a2, array[1] = a3, ... array[i1 - 1] = an (where n = i1 + 1)

    DeclareObject       = 0x43, // v5: Pops the number of members in the
    // object. Pop one value and one name per member and set the corresponding
    // member in the object. The resulting object is pushed on the stack.
    // It can later be sent to a function or set in a variable.
    // Note: the member names are converted to strings; they certainly should
    // be strings thought anything is supported.
    // 0 (o)    operation: new Object
    // count = i1, object.a3 = a2, object.a5 = a4, ... object.an = am
    // (where n = i1 * 2 + 1 and m = i1 * 2)

    TypeOf              = 0x44, // v5: Pops some data, define its type and
    // push the name of its type back on the stack. The currently supported
    // types are as follow: number, boolean, string, object, movieclip,
    // null, undefined, function
    // 0 (s)    operation: typeof(a1)

    GetTarget           = 0x45, // v5: Pops an object, if it is a valid sprite,
    // push it's path on the stack. A sprite path can be used by different
    // other actions such as the Goto Expression.
    // 0 (s)    operation: target_of(o1)

    Enumerate           = 0x46, // v5: Pop the name of a thread (can it only
    // be the root?) and push all of its children (objects & variables) back
    // on the stack. The list is null terminated.
    // 0 (v)    operation: s1

    TAdd                = 0x47,
    /* v5: pop 2 (a), push 1 (a)
     * Details: (typed)
     * Category: Arithmetic
     * Structure: <n.a.>
     * Operation:
     *  a1 = pop();
     *  a2 = pop();
     *  if (is_int(a1) && is_int(a2)) {
     *      i1 := (int)a1;
     *      i2 := (int)a2;
     *      r  := i1 + i2;      // sum
     *  } else
     *  if (is_numeric(a1) && is_numeric(a2)) {
     *      f1 := (float)a1;
     *      f2 := (float)a2;
     *      r  := f1 + f2;      // sum
     *  } else {
     *      s1 := (string)a1;
     *      s2 := (string)a2;
     *      r  := s1 + s2;      // concatenation
     *  }
     *      push(r);
     *
     * Pops two numbers or two strings, computes the sum or concatenation and
     * push the result back on the stack.
     *
     * This action is typed meaning that it will automatically convert the
     * parameters as required and in a very well defined manner.
     */

    TLessThan           = 0x48, // v5: Pops two integers or two strings,
    // computes whether they are ordered from smaller to larger and push the
    // result back on the stack.
    // 0 (b)    operation: i2 < i1 or f2 < f1 or s2 < s1

    TEqual              = 0x49, // v5: Pops two integers, floats or strings,
    // computes whether they are equal and push the result back on the stack.
    // If a mix of types is found, then convertions occur. Strings may be
    // transformed in numbers and numbers in strings as with the untyped Equal
    // operator.
    // 0 (b)    operation: i2 == i1 or f2 == f1 or s2 == s1

    Number              = 0x4A, // v5: Pops one item and transform it into a
    // number. For strings it works as expected (see the strtof(3C) manual
    // pages). For a user defined object, the method named valueOf() is called.
    // 0 (n)    operation: a1.valueOf()

    String              = 0x4B, // v5: Pops one item and transform it into a
    // string. For numbers it works as expected (see the sprintf(3C) manual
    // pages). For a user defined object, the method named toString() is called.
    // 0 (n)    operation: a1.toString()

    Duplicate           = 0x4C, // v5: Pops one item and push it back twice.
    // 0 (a1 a1)        operation: a1

    Swap                = 0x4D, // v5: Pops two items and push them back the
    // other way around.
    // 0 (a1 a2)        operation: a2 <-> a1

    GetMember           = 0x4E, // v5: Pops one string or an integer (member
    // name), pop an object reference, defines the value of that object member
    // and push the result back on the stack.
    // 0 (a)    operation: o2[a1]

    SetMember           = 0x4F, // v5: Pops a value 'a1' which will be the new
    // member value, then one string or integer 'a2' (the name of the member to
    // modified or create), and finally pop an object reference 'o3'. If the
    // member doesn't exists yet, create it. Finally, sets the object member to
    // the value 'a1'.
    // 0        operation: o3[a2] = a1

    Increment           = 0x50, // v5: Pops one number, add 1 to it and push
    // it back on the stack.
    // 0 (n)    operation: n1 + 1

    Decrement           = 0x51, // v5: Pops one number, substract 1 to it and
    // push it back on the stack.
    // 0 (n)    operation: n1 - 1

    CallMethod          = 0x52, // v5: Pops the name of a method (can be the
    // empty string), pop an object, pop the number of arguments, pop each
    // argument, call the method (function) of the object, push the returned
    // value on the stack.
    // 0 (a)    operation: o2.s1(i3, [a4, a5, a6...]) or o2(i3, [a4, a5, a6...])

    NewMethod           = 0x53, // v5: Pops the name of a method (can be the
    // empty string), pop an object (created with the Declare Object,) pop the
    // number of arguments, pop each argument, create a new object, then call
    // the specified method (function) as the constructor function of the
    // object, push the returned value on the stack. This allows for overloaded
    // constructors as in C++.
    // 0 (o)    operation: o = new s2
    // o.s1(i3, [a4, a5, a6...]) or o.s2(i3, [a4, a5, a6...])

    InstanceOf          = 0x54, // v6: Pops the name of a constructor (s1 - ie.
    // "Color") then the name of an object (s2). Checks whether the named
    // object is part of the class defined by the constructor. If so, then true
    // is push on the stack, otherwise false.  Since SWF version 7, it is
    // possible to cast an object to another using the Cast Object action.
    // This action returns a copy of the object or Null, which in many cases
    // can be much more practical.
    // 0 (b)    operation: o2 is_instance_of s1

    EnumerateObject     = 0x55, // v6: Pops an object from the stack,
    // push a null, then push the name of each member on the stack.
    // 0 (v)    operation: o1

                        //0x56~0x5F

    And                 = 0x60, // v5: Pops two integers, computes the
    // bitwise And and push the result back on the stack.
    // 0 (i)    operation: i2 & i1

    Or                  = 0x61, // v5: Pops two integers, computes the
    // bitwise Or and push the result back on the stack.
    // 0 (i)    operation: i2 | i1

    Xor                 = 0x62, // v5: Pops two integers, computes the
    // bitwise Xor and push the result back
    // 0 (i)    operation: i2 ^ i1

    ShiftLeft           = 0x63, // v5: Pops two integers, shift the 2nd one
    // by the number of bits specified by the first integer and push the result
    // back on the stack.
    // 0 (i)    operation: i2 << i1

    ShiftRight          = 0x64, // v5: Pops two integers, shift the 2nd signed
    // integer by the number of bits specified by the first integer and push
    // the result back on the stack.
    // 0 (i)    operation: i2 >> i1

    ShiftRightUnsigned  = 0x65, // v5: Pops two integers, shift the 2nd
    // unsigned integer by the number of bits specified by the first integer
    // and push the result back on the stack.
    // 0 (i)    operation: i2 >>> i1

    StrictEqual         = 0x66, // v6: Pops two parameters and return whether
    // they are strictly equal. No cast is applied to either s1 or s2. Thus two
    // items of different type are not equal (0 == "0" is true, but 0 === "0"
    // is false.)
    // 0 (b)    operation: a1 === a2

    GreaterThan         = 0x67, // v6: Similar to Swap + Less Than. It checks
    // whether the second parameter is greater than the first and return the
    // boolean result on the stack.
    // 0 (b)    operation: a2 > a1

    StringGreaterThan   = 0x68, // v6: Similar to Swap + String Less Than.
    // It checks whether the second string is greater than the first and return
    // the boolean result on the stack.
    // 0 (b)    operation: s2 > s1

    Extends             = 0x69, // v7: The Extends action will be used to
    // define a new object which extends another object.
    // The declaration in ActionScript is: class A extends B;
    // In an SWF action script, you don't exactly declare objects, you actually
    // instantiate them and define their functions. This action creates a new
    // object named s2 which is an extension of the object s1.
    // Use this action whenever you need to inherit an object without calling
    // its constructor.
    // 0 (o)    operation: super_class = s1, sub_class = s2
    //          sub_class.prototype = new object
    //          sub_class.prototype.__proto__ = super_class.prototype;
    //          sub_class.prototype.__constructor__ = super_class;

                        //0x70~0x7F


    GotoFrame           = 0x81, // v1: The playback continues at the specified
    // frame (NOTE: The first frame in the movie has number 0)
    // 2        unsigned short  f_frame_no;

    GetURL              = 0x83, // v1: Load the specified URL in the specified
    // target window.
    // variable         string  f_url; string   f_target;
    /*
     * When the target is set as "_level0", the current SWF file is replaced
     * by the file specified in the f_url field. The name in the f_url field
     * should be a proper SWF file or the area will simply become black.
     *
     * When the target is set as "_level1", something special is supposed to
     * happen. I still don't know what it is...  Also the effect of _level1 +
     * an empty URL is ... (to remove level1?)
     *
     * The URL can be a javascript command when the protocol is set to
     * "javascript:". For instance, you can call your function as follow:
     * "javascript:MyFunction(5)". If you want to use dynamic values, you will
     * need to concatenate strings and use Get URL2 instead.
     *
     * The target can also be set to the regular HTML names such as "_top" or
     * a frame name.
     */

                        //0x84~0x86

    StoreRegister       = 0x87, // v5: Pop one value from the stack, push it
    // back on the stack and also store it in one of four or 256 registers
    // which number is specified in the tag (0, 1, 2 or 3 only if not in a
    // Declare Function (V7). I tried other numbers and they don't work in SWF
    // version 6 or older.) Until set a register has the value undefined. The
    // value of a register can be retrieved with a Push Data action and the
    // register type with the matching register number.
    // (To be tested) It is likely that trying to read a register which is not
    // legal in a Declare Function (V7) will generate an exception
    // (Yes! A Throw!) but I wouldn't be surprised if you just get undefined.
    // 1 (a)    unsigned char   f_register; operation: regs[f_register] = a1

    ConstantPool        = 0x88,
    DeclareDictionary   = 0x88, // v5: Declare an array of strings which can
    // later be retrieved using the Push Data action with a dictionary lookup.
    // There can be a maximum of 65534 strings. The visibility of a dictionary
    // is within its DoAction or other similar block of actions.
    // variable (s)     unsigned short  f_count; string f_dictionary[f_count];

    StrictMode          = 0x89, // v5: Set the strict mode (f_strict != 0) or
    // reset the strict mode (f_strict == 0).   5
    // 1        unsigned char   f_strict;

    WaitForFrame        = 0x8A, // v1: Wait until the frame specified in
    // f_frame is fully loaded to execute actions right after this one.
    // Otherwise skip the specified number of actions.
    // 3        unsigned short  f_frame; unsigned char  f_skip;

    /* This is usually used with a GotoFrame like in:
     *      Next Frame
     *      Wait for Frame #10
     *      (otherwise skip 1 action)
     *      Goto Frame #5
     *      Play
     *      End
     * This will usually be used to display some Loading... info before the
     * complete movie is loaded.
     */

    SetTarget           = 0x8B, // v1: If the string f_target is the empty
    // string, then the next actions apply to the main movie. Otherwise it is
    // the name of a sprite and the following actions apply to that sprite only.
    // variable         string  f_target;

    GotoLabel           = 0x8C, // v3: Go to a named frame. Frames are given
    // names with the use of the FrameLabel tag.
    // variable         string  f_label;

    WaitForFrame2       = 0x8D, // v4: Pop a value or a string used as the
    // frame number to wait for. The frame can be specified as with the Goto
    // Expression. If the frame was not reached yet, skip the following f_skip
    // actions
    // 1        unsigned char   f_skip; operation: a1 WaitForFrame

    DeclareFunction2    = 0x8E, // v7: Declare a function which can later be
    // called with the CallFunction action or Call Method action (when
    // defined as a function member.) The f_function_length defines the number
    // of bytes that the function declaration uses after the header (i.e. the
    // size of the actions defined in the function). All the actions included
    // in this block are part of the function body.
    /* variable
     *              struct {
     *                  string name;
     *                  uint16_t arg_count;
     *                  uint8_t  reg_count;
     *                  union {
     *                      struct {
     *                          uint16_t reserved:7, preload_global:1,
     *                                   preload_parent:1, preload_root:1,
     *                                   suppress_super:1, preload_super:1,
     *                                   suppress_args:1, preload_args:1,
     *                                   suppress_this:1, preload_this:1
     *                      };  uint16_t _;
     *                  };
     *                      Params paramVec;
     *                      uint32_t len;
     *              };
     * WARNING: the preload/suppress flags are defined on a short and thus the
     * bytes in a Flash file will look swapped.
     */
    /*
     * Do not terminate a function with an End action
     *
     * A function should terminate with a Return action. The value used by the
     * return statement will be the only value left on the caller stack.
     *
     * Functions declared with this action code byte also support the use of
     * up to 255 local registers (registers 0 to 254 since the f_reg_count
     * byte specifies the last register which can be used plus one). To access
     * the local registers, use the Push Data action with a load register and
     * to change a register value use the Store Register action.
     *
     * Also, it is possible to control the preloading or suppressing of the
     * different internal variables: this, arguments, super, _root, _parent
     * and _global. All the function arguments can also be ignored. If you
     * write a compiler, you should preload only the variables which are used
     * in the function body. And you should suppress all the variables which
     * are never being accessed in the function body [WARNING: note that it is
     * possible to concatenate two strings such as "th" and "is" to generate
     * the name "this" and then do a 'get that variable'. This means a
     * function can't really know whether any of the internal variables are
     * really never going to be used... but who writes code like that,
     * really?!]. If you are writing a smart player, then you may want to
     * avoid creating the variables until they are actually being used (thus
     * when an if() statement ends the function prematurly, you may end up not
     * creating many of these variables!).
     *
     * The preloading bits indicate in which register to load the given
     * internal variable. The suppressing bits indicate which internal
     * variable not to create at all. That is, if the preloading bit is not
     * set and the suppressing is not set, then the internal variable are
     * supposed to be created by name (i.e. you can access a variable named
     * "this" from within the function when bits 0 and 1 are zero).
     *
     * The f_reg_count parameter needs to be specified and it tells the player
     * the largest register number in use in this function. This way it can
     * allocate up to 255 registers on entry. By default, these registers are
     * initialized as undefined. The variables automatically loaded in
     * registers are loaded starting at register 1. The internal variables are
     * loaded in this order: this, arguments, super, _root, _parent and
     * _global. Thus, if you call a function which accepts three user
     * parameters and wants this and _parent, it will load this in register 1,
     * _parent in register 2 and the user parameters can be loaded in
     * registers 3, 4 and 5. User parameters are loaded in registers only if
     * their corresponding f_param_register field is not zero (see
     * swf_params). Also, they don't need to be defined in order.
     *
     * Note that system variables are loaded AFTER arguments. This means if
     * you put an argument in register 3 and this register is also used for
     * _root, then within the function the register 3 will be equal to the
     * content of _root. Compilers should never do that though.
     */

    Try                 = 0x8F, // v7: Declare a try/catch/finally block.
    /* variable union { struct { uint8_t reserved:5, catch_in_register:1,
     * finally:1, catch:1; };  uint8_t _; }; uint16_t try_size, catch_size,
     * finally_size; if(catch_in_register == 0) { string catch_name; } else {
     * uint8_t catch_register; }
     */
    /*
     * This has the behavior of the action script: try { ... } catch(name) {
     * ... } finally { ... } At this time, there is no definition of exception
     * in the action scripts.  But you can write a function which Throws.
     *
     * The sementic of the try/catch/finally block is very well defined in
     * ECMA 262 version 3 (see pages 87/88).
     *
     * The f_finally and f_catch may not both be null or the sementic of the
     * try block would be wrong. The f_try_size, f_catch_size and
     * f_finally_size are defined in bytes and give the size of each of the
     * block of instructions just like a function definition.
     *
     * Do not terminate these blocks with an End action
     *
     * If the f_catch_in_register is set to 1, then a register number is
     * specified instead of a variable name. This will usually be faster.
     */

                        //0x90~0x93

    With                = 0x94, // v5: The variable references within the
    // following f_size bytes are taken as names of members of the specified
    // object 'o1'. When no member is available in that object, the previous
    // With, or the corresponding global variable is queried. This is similar
    // to the Pascal language with instruction or a Set Target (dynamic).
    // Note that the number of With within other With is limited to 7 (a depth
    // of 8 is hard coded for some reasons...). Additional With blocks are
    // ignored. The size defines up to where the With has an effect (the f_size
    // is taken as a branchment offset except that it is always positive).
    // Note that it is to the creator of the action scripts to ensure the
    // proper nesting of each With.
    // 0        unsigned short  f_size; operation: with o1

                        //0x95

    Push                = 0x96,
    PushData            = 0x96,
    /* variable
     *              struct {
     *                  unsigned char   f_type;
     *                  ...             f_data;
     *              } f_push_data[<variable>];
     */

    /* This action was introduced in V4.0. The supported data types vary
     * depending on the version of player that you have. As many values as
     * necessary can be pushed at once. The f_push_data structure will be
     * repeated multiple times as required. For instance, to push two strings
     * on the stack at once, you would use the following code:
     *      96
     *      0C 00
     *      00 't' 'e' 's' 't' 00
     *      00 'm' 'o' 'r' 'e' 00
     * Please, see the table of data types below for more information.
     */

                        //0x97~0x98

    BranchAlways        = 0x99, // v4: Jump to the specified instruction.
    // 2        signed short    f_offset;
    // f_offset is a relative offset to the instruction to execute next; the
    // current program counter is taken as being AFTER the branch instruction.

    GetURL2             = 0x9A, // v4: Pop two strings, the URL (s2) and the
    // target name (s1).
    // 1        unsigned char   f_method; operation: s1 s2

    /*
     * All the usual HTML target names seem to be supported (_top, _blank,
     * <frame name>, etc.) You can also use the special internal names _level0
     * to _level10. _level0 is the current movie. Other levels, I'm still not
     * too sure how these can be used.
     *
     * Use f_method to tell Flash how to handle the variables (see table
     * below). The variables of the current SWF context can be forwarded to
     * the destination page using GET or POST (this means you can create
     * dynamic forms with full HTML conformance).
     *
     * It seems that in V4.x (or would it be in V6.x?!? — it doesn't seem to
     * work in V5.x) you could use URL2 to read a text file (with a .txt
     * extension) with a list of variables using something like this:
     *      Push URL "myvars.txt", Target "_level0"; Get URL2;
     * The syntax of the file myvars.txt is lines which are defined as a
     * variable name followed by an equal sign and the contents for that
     * variable. If contents of a single variable is more than one line, start
     * the following line(s) with an ampersand to continue that one variable.
     *
     * The URL can also start with "javascript:" in which case the given
     * browser javascript language receives a call.
     *
     *          f_method        Action
     *          0       <don't send variables>
     *          1       GET
     *          2       POST
     */

    DeclareFunction     = 0x9B, // v5: Declare a function which can later be
    // called with the CallFunction action. The f_function_length defines the
    // number of bytes that the function declaration takes. All the actions
    // included in this block are part of the function. A function should
    // terminate with a Return action. The value used by the return statement
    // should be the only value left on the caller stack.
    // Do not terminate a function with an End action
    /* variable
     *          string          f_name;
     *          unsigned short  f_arg_count;
     *          string          f_arg_name[f_arg_count];
     *          unsigned short  f_function_length;
     */

    /*
     * Prior version 6, the Macromedia player would keep all the data pushed
     * in a function as is when the function returned whether there is a
     * return statement or not.
     *
     * Since version 7, it is preferable to use the new type of functions:
     * Declare Function (V7).
     */

                        //0x9C

    BranchIfTrue        = 0x9D, // v4: Pop a boolean value; if true then
    // jump to the specified instruction; otherwise continue with the following
    // instructions.
    // 2        signed short    f_offset; operation: b1
    // NOTE: f_offset is a relative offset to the instruction to execute next;
    // the current program counter is taken as being AFTER the branch
    // instruction.

    CallFrame           = 0x9E, // v4: Pop a string or integer and call the
    // corresponding frame. This means:
    // (1) search the corresponding frame,
    // (2) execute its actions and
    // (3) continue with the action defined after this one.
    // The frame can be identified with a name or a number.
    // It is also possible to specify a target movie
    // ("<sprite name>.<frame name>? - to be tested...)
    // 0        nothing (this is a bug known by Macromedia); operation: s1

    GotoExpression      = 0x9F, // v4: Pop a value or a string and jump to the
    // specified frame. When a string is specified, it can include a path to a
    // sprite as in: /Test:55
    // When f_play is ON (1), the action is to play as soon as that frame is
    // reached. Otherwise, the frame is shown in stop mode.     4
    // 2        unsigned char   f_play; operation: a1

                        //0xA0~0xFF
};

};

struct ActionNameTable: public std::map<uint16_t, const char*> {
    ActionNameTable() {
#define ADD_ACTION_NAME(tag) \
        insert(value_type(ACTION::CODE::tag, #tag))

        ADD_ACTION_NAME(End);                   //  0



        ADD_ACTION_NAME(NextFrame);
        ADD_ACTION_NAME(PrevFrame);             //  5
        ADD_ACTION_NAME(Play);
        ADD_ACTION_NAME(Stop);
        ADD_ACTION_NAME(ToggleQuality);
        ADD_ACTION_NAME(StopSound);
        ADD_ACTION_NAME(Add);
        ADD_ACTION_NAME(Subtract);
        ADD_ACTION_NAME(Multiply);
        ADD_ACTION_NAME(Divide);
        ADD_ACTION_NAME(Equal);
        ADD_ACTION_NAME(LessThan);
        ADD_ACTION_NAME(LogicalAnd);
        ADD_ACTION_NAME(LogicalOr);
        ADD_ACTION_NAME(LogicalNot);
        ADD_ACTION_NAME(StringEqual);
        ADD_ACTION_NAME(StringLength);
        ADD_ACTION_NAME(SubString);

        ADD_ACTION_NAME(Pop);
        ADD_ACTION_NAME(IntegralPart);



        ADD_ACTION_NAME(GetVariable);
        ADD_ACTION_NAME(SetVariable);


        ADD_ACTION_NAME(SetTarget2);
        ADD_ACTION_NAME(ConcatenateStrings);
        ADD_ACTION_NAME(GetProperty);
        ADD_ACTION_NAME(SetProperty);
        ADD_ACTION_NAME(DuplicateSprite);
        ADD_ACTION_NAME(RemoveSprite);
        ADD_ACTION_NAME(Trace);
        ADD_ACTION_NAME(StartDrag);
        ADD_ACTION_NAME(StopDrag);
        ADD_ACTION_NAME(StringLessThan);
        ADD_ACTION_NAME(Throw);
        ADD_ACTION_NAME(CastObject);
        ADD_ACTION_NAME(Implements);
        ADD_ACTION_NAME(FSCommand2);


        ADD_ACTION_NAME(Random);
        ADD_ACTION_NAME(WStringLength);
        ADD_ACTION_NAME(Ord);
        ADD_ACTION_NAME(Chr);
        ADD_ACTION_NAME(GetTimer);
        ADD_ACTION_NAME(WSubString);
        ADD_ACTION_NAME(WOrd);
        ADD_ACTION_NAME(WChr);


        ADD_ACTION_NAME(Delete);
        ADD_ACTION_NAME(DeleteAll);
        ADD_ACTION_NAME(SetLocalVariable);
        ADD_ACTION_NAME(CallFunction);
        ADD_ACTION_NAME(Return);
        ADD_ACTION_NAME(Modulo);
        ADD_ACTION_NAME(New);
        ADD_ACTION_NAME(DeclareLocalVariable);
        ADD_ACTION_NAME(DeclareArray);
        ADD_ACTION_NAME(DeclareObject);
        ADD_ACTION_NAME(TypeOf);
        ADD_ACTION_NAME(GetTarget);
        ADD_ACTION_NAME(Enumerate);
        ADD_ACTION_NAME(TAdd);
        ADD_ACTION_NAME(TLessThan);
        ADD_ACTION_NAME(TEqual);
        ADD_ACTION_NAME(Number);
        ADD_ACTION_NAME(String);
        ADD_ACTION_NAME(Duplicate);
        ADD_ACTION_NAME(Swap);
        ADD_ACTION_NAME(GetMember);
        ADD_ACTION_NAME(SetMember);
        ADD_ACTION_NAME(Increment);
        ADD_ACTION_NAME(Decrement);
        ADD_ACTION_NAME(CallMethod);
        ADD_ACTION_NAME(NewMethod);
        ADD_ACTION_NAME(InstanceOf);
        ADD_ACTION_NAME(EnumerateObject);
        //0x56~0x5F
        ADD_ACTION_NAME(And);
        ADD_ACTION_NAME(Or);
        ADD_ACTION_NAME(Xor);
        ADD_ACTION_NAME(ShiftLeft);
        ADD_ACTION_NAME(ShiftRight);
        ADD_ACTION_NAME(ShiftRightUnsigned);
        ADD_ACTION_NAME(StrictEqual);
        ADD_ACTION_NAME(GreaterThan);
        ADD_ACTION_NAME(StringGreaterThan);
        ADD_ACTION_NAME(Extends);
        //0x70~0x7F
        ADD_ACTION_NAME(GotoFrame);
        ADD_ACTION_NAME(GetURL);



        ADD_ACTION_NAME(StoreRegister);
        ADD_ACTION_NAME(DeclareDictionary);
        ADD_ACTION_NAME(StrictMode);
        ADD_ACTION_NAME(WaitForFrame);
        ADD_ACTION_NAME(SetTarget);
        ADD_ACTION_NAME(GotoLabel);
        ADD_ACTION_NAME(WaitForFrame2);
        ADD_ACTION_NAME(DeclareFunction2);
        ADD_ACTION_NAME(Try);




        ADD_ACTION_NAME(With);

        ADD_ACTION_NAME(PushData);


        ADD_ACTION_NAME(BranchAlways);
        ADD_ACTION_NAME(GetURL2);
        ADD_ACTION_NAME(DeclareFunction);

        ADD_ACTION_NAME(BranchIfTrue);
        ADD_ACTION_NAME(CallFrame);
        ADD_ACTION_NAME(GotoExpression );

        // 0xA0~0xFF
    }
};


namespace DATATYPE {
    enum DataType {
    // The following is all types accepted in the PushData action: v5
    String                      = 0x00,
    // Push a string (literal) on the stack.

    Float                       = 0x01,
    /* Push a 32 bits IEEE floating point value on the stack. This type can be
     * used to specify a Property reference. Note that the property reference
     * will be saved as a floating point value though only the integral part
     * will be used. (literal)
     */

    Null                        = 0x02,
    /* Push NULL on the stack. This very looks like zero (0) except that it
     * can be used as an object pointer. (<none>)
     */

    Undefined                   = 0x03,
    /* Push an undefined object on the stack. This is not a string, integer,
     * float or Boolean. It simply is an undefined element. Any operation on
     * an undefined element yields undefined except an equal comparison (and
     * some operations such as a Pop). (<none>)
     */

    Register                    = 0x04,
    /* Push the content of the given register on the stack. In the main
     * thread, you have 4 registers in SWF (number 0, 1, 2 and 3). Since SWF
     * version 7, it is possible to use up to 256 registers in a
     * DeclareFunction (V7). However, outside such a function, the limit is
     * the same. To set a register, use the Store Register action.
     */

    Boolean                     = 0x05,
    // Push a Boolean value on the stack (needs to be 0 or 1).

    Double                      = 0x06,
    /* An IEEE double value saved in a strange way. The following gives you
     * the C code used to read these double values.
     *
     *  doubleresult;
     *  charvalue[8];
     *
     *  value[4] = ReadByte(input_stream);
     *  value[5] = ReadByte(input_stream);
     *  value[6] = ReadByte(input_stream);
     *  value[7] = ReadByte(input_stream);
     *  value[0] = ReadByte(input_stream);
     *  value[1] = ReadByte(input_stream);
     *  value[2] = ReadByte(input_stream);
     *  value[3] = ReadByte(input_stream);
     *
     *  result = *(double*)value;
     */

    Integer                     = 0x07,
    // Push an integer on the stack.

    Constant8                   = 0x08,
    DictionaryLookup            = 0x08,
    /* Push a string as declared with a DeclareDictionary action. The very
     * first string in a dictionary has reference 0. There can only be up to
     * 256 strings push with this instruction.
     *
     * Constant pool index
     */

    Constant16                  = 0x09,
    LargeDictionaryLookup       = 0x09,
    /* Push a string as declared with a DeclareDictionary action. The very
     * first string in a dictionary has reference 0. There can be up to 65534
     * strings pushed this way. Note that the strings 0 to 255 should use the
     * type 0x08 instead.
     */
};

};

namespace PROPERTIE {

enum Propertie {
    /* The following is the list of currently accepted properties or fields
     * for the GetProperty and the SetProperty actions. Note that the
     * properties can be specified with either an integer (type 7, requires
     * V5.0+) or a single precision floating point (type 1, V4.0 compatible).
     * And since strings are automatically transformed in a value when
     * required, one can use a string to represent the property number (type
     * 0). It works with a double value, I even tested a Boolean and null and
     * it works. Obviously it isn't a good idea to use these. The default
     * should be a single precision float. Please, see the Push Data action
     * for more information about data types.
     */

    PosX                =  0,   // v4: x position in pixels (not TWIPs!)
    PosY                =  1,   // v4: y position in pixels (not TWIPs!)
    ScaleX              =  2,   // v4: horizontal scaling factor in percent
    ScaleY              =  3,   // v4:   vertical scaling factor in percent
    CurrentFrame        =  4,   // v4: the very frame being played
    /* one can query the root current frame using an empty string ("") as the
     * name of the object; note that the first current frame is number 1 and
     * the last is equal to the total number of frames; on the other hand, the
     * Goto instruction expects a frame number from 0 to the number of
     * frames - 1
     */

    NumberOfFrames      =  5,
    // v4: total number of frames in movie/sprite/thread

    Alpha               =  6,   // v4: the alpha value in percent
    Visibility          =  7,   // v4: whether the object is visible
    Width               =  8,
    // v4: maximum  width of the object (scales the object to that width)

    Height              =  9,
    // v4: maximum height of the object (scales the object to that height)

    Rotation            = 10,   // v4: rotation angle in degrees
    Target              = 11,
    /* v4: return the name (full path) of an object; this can be viewed as a
     * reference to that object
     */

    FramesLoaded        = 12,   // v4: number of frames already loaded
    Name                = 13,   // v4: name of the object
    DropTarget          = 14,
    // v4: object over which this object was last dropped

    URL                 = 15,   // v4: URL linked to that object
    HighQuality         = 16,   // v4: whether we are in high quality mode
    ShowFocusRectangle  = 17,   // v4: whether the focus rectangle is visible
    SoundBufferTime     = 18,
    /* v4: position (or pointer) in the sound buffer; useful to synchronize
     * the graphics to the music
     */

    Quality             = 19,
    // v5: what the quality is (0 - Low, 1 - Medium or 2 - High)

    MouseX              = 20,
    // v5: current horizontal position of the mouse pointer within the window

    MouseY              = 21,
    // v5: current   vertical position of the mouse pointer within the window

    Clone               = 16384,
    // v4: this flag has to do with the depth of sprites being duplicated
};

};

};

struct ColorObject {    // v5
    union { struct { uint8_t b, g, r, x; }; uint32_t cv; };     // 0xRRGGBB
    //CXForm cxf;       // XXX:
    //void setCXForm(CXForm& cxf_) { return cxf = cxf_; }
    //CXForm getCXForm() { return cxf; }
    void setRGB(uint32_t c_) { cv = c_; }
    uint32_t getRGB() { return cv; }
    ColorObject(/*sprite target*/) { }
};

struct StringObject {
    std::string str;
    // int32_t len;
    uint32_t charCodeAt(int32_t idx) { return str[idx]; }
    StringObject charAt(int32_t idx) { return *this; }

    // TODO:
    StringObject concat(StringObject& so) { return *this; }
    StringObject fromCharCode(int32_t c1, int32_t c2) { return *this; }
    int32_t indexOf(StringObject& so, int32_t beg) { return 0; }
    int32_t lastIndexOf(StringObject& so, int32_t beg) { return 0; }
    StringObject slice(int32_t beg, int32_t end) { return *this; }
    //std::vector<StringObject> split(StringObject& dl) { ...; }
    StringObject substr(int32_t beg, int32_t len) { return *this; }
    StringObject substring(int32_t p1, int32_t p2) { return *this; }
    StringObject toLowerCase() { return *this; }
    StringObject toUpperCase() { return *this; }
};

struct MathObject {     // v5
    static constexpr ull e       = CONST_E;
    static constexpr ull pi      = CONST_PI;
    static constexpr ull ln2     = CONST_LN2;
    static constexpr ull ln10    = CONST_LN10;
    static constexpr ull sqrt2   = CONST_SQRT2;
    static constexpr ull log2e   = CONST_LOG2_E;
    static constexpr ull log10e  = CONST_LOG10_E;
    static constexpr ull sqrt1_2 = CONST_1_SQRT2;

    static ull abs(sll v) { return sll_abs(v); }
    static sll ceil (sll v) { sll i = sllint(v); return (i + (i < v)); }
    static sll floor(sll v) { sll i = sllint(v); return (i - (i < v)); }
    static sll round(sll v) { return sllint((v += CONST_1_2)); }
    static sll random() { return dbl2sll(random()); }
    static sll atan (sll v) { return sllatan (v); }
    static sll atan2(sll v) { return sllatan2(v); }

    static sll acos(sll v) { return v; }        // TODO:
    static sll asin(sll v) { return v; }        // TODO:

    static sll cos(sll v) { return sllcos(v); }
    static sll sin(sll v) { return sllsin(v); }
    static sll tan(sll v) { return slltan(v); }
    static sll exp(sll v) { return sllexp(v); } // FIXME: -.5 <= v <= .5
    static sll log(sll v) { return slllog(v); }
    static sll sqrt(sll v) { return sllsqrt(v); }
    static sll pow(sll x, sll y) { return sllpow(x, y); }
    static sll min(sll a, sll b) { return (a < b ? a : b); }
    static sll max(sll a, sll b) { return (a < b ? b : a); }
};

struct Action {
    /* Action:
     *
     * Actions are an essential part of an interactive SWF file. Actions allow
     * a file to react to events such as mouse movements or mouse clicks. The
     * SWF 3 action model and earlier supports a simple action model. The SWF
     * 4 action model supports a greatly enhanced action model that includes
     * an expression evaluator, variables, and conditional branching and
     * looping. The SWF 5 action model adds a JavaScript-style object model,
     * data types, and functions.
     */

    /* An action (or list of actions) can be triggered by a button state
     * transition, or by SWF 3 actions. The action is not executed
     * immediately, but is added to a list of actions to be processed. The
     * list is executed on a ShowFrame tag, or after the button state has
     * changed. An action can cause other actions to be triggered, in which
     * case, the action is added to the list of actions to be processed.
     * Actions are processed until the action list is empty.
     *
     * By default, Timeline actions such as Stop (see ActionStop), Play (see
     * ActionPlay), and GoToFrame (see ActionGotoFrame) apply to files that
     * contain them.  However, the SetTarget action (see ActionSetTarget),
     * which is called Tell Target in the Adobe Flash user interface, can be
     * used to send an action command to another file or sprite (see
     * DefineSprite).
     */

    union {      uint8_t id;
        struct { uint8_t i_:7, hasl:1; };
    };
    //std::vector<uint8_t> data;
    uint16_t size; uint8_t* data;

    ~Action() { delete[] data; }

    BitStream& load(BitStream& bs) {    size = 0;
        off = bs.pos;                   bs >> id;
        if (hasl) {                     //uint16_t size;
            bs >> size;                 data = new uint8_t[size];
            bs.read((char*)data, size); //data.resize(size);
            //for (uint16_t i=0; i < size; ++i) bs >> data[i];
        } else data = NULL;             return bs;
    }
    uint32_t off;

    friend BitStream& operator>>(BitStream& bs, Action& ax)
            { return ax.load(bs); }
    friend std::ostream& operator<<(std::ostream& os, Action& ax) {
        static ACTION::ActionNameTable anTbl;
        ACTION::ActionNameTable::iterator it = anTbl.find(ax.id);
        os  << "  " << (it == anTbl.end() ? "UnknownAction" : it->second)
            << "/0x" << std::setfill('0') << std::hex << std::setw(2)
            << (int)ax.id << std::setfill(' ') << std::dec
            << ": \t+" << ax.size;
        dump_data(os, ax.data, ax.size);        return os;
    }
};

struct StackMachine {
};

struct ActionScript {
};

};

#endif//ACTION_HPP
// vim:sts=4:ts=8:
