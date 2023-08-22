//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: render.hpp     Thu, 06 Apr 2006 14:43:36 +0800  mhfan $ *
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
#ifndef RENDER_HPP
#define RENDER_HPP

#include "fb.hpp"
#include "type.hpp"
#include "common.h"

namespace SWF {

struct PlaceObject;
struct FrameRender {
    //void* priv;
    virtual ~FrameRender() { }
    virtual void SetViewport(Rect& br) = 0;
    virtual void SetBackgroundColor(RGB& rgb) = 0;
    virtual void DrawShape(ShapeWithStyle* sws, PlaceObject* pl) = 0;
};

#define DEFAULT_SCREEN_WIDTH            320
#define DEFAULT_SCREEN_HEIGHT           240

};

#endif//RENDER_HPP
// vim:sts=4:ts=8:
