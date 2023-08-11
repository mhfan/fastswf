//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: tag.c          Mon, 03 Apr 2006 19:32:18 +0800  mhfan $ *
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

#include "bs.h"
#include "tag.h"
#include "type.h"
#include "shape.h"
#include "common.h"
#include "render.h"			// XXX
#include "swf.h"			// FIXME: don't export struct swf here

#define	SWF_TAGPSR(tag) \
	static void swf_tagpsr_ ## tag (struct swf* swf)

SWF_TAGPSR(Default)
{
    fprintf(stdout, "Ignore tag 0x%03x(%3d), skip %6d bytes: %s\n"
	    , swf->th.code, swf->th.code, swf->th.size
	    , swf->tagpsr[swf->th.code].name);
    while (swf->th.size--) bs_read_u8(swf->bs);
}

SWF_TAGPSR(ScriptLimits)
{
    uint16_t recursion, timeout;
    assert(swf->th.code == SWF_TAG_ScriptLimits);
    recursion = bs_read_u16(swf->bs);
    timeout   = bs_read_u16(swf->bs);
    fprintf(stdout, "ScriptLimits: recursion=%d, timeout=%d\n"
	    , recursion, timeout);
}

SWF_TAGPSR(SetBackgroundColor)
{
    assert(swf->th.code == SWF_TAG_SetBackgroundColor);

    swf_read_rgb(swf->bs, &swf->bgc);
    swf->render.setbgc(&swf->bgc);
    swf->frames = 0;

    fprintf(stdout, "#%d: %s to [%d %d %d]\n", swf->frames
	    , swf->tagpsr[swf->th.code].name
	    , swf->bgc.r, swf->bgc.g, swf->bgc.b);
}

void swf_read_shapefillstyle(struct bstream* bs, struct swf_fill_style* style,
	uint16_t tag_type)
{
    style->type = bs_read_u8(bs);
    switch (style->type) {
    case SWF_FILL_STYLE_SOLID:
	switch (tag_type) {
	case SWF_TAG_DefineMorphShape:
	    swf_read_rgba(bs, &style->solid.rgba);
	    swf_read_rgba(bs, &style->solid.rgba_morph);
	case SWF_TAG_DefineShape3:
	    swf_read_rgba(bs, &style->solid.rgba);
	default:
	    swf_read_rgb (bs, &style->solid.rgb);
	}   break;
    case SWF_FILL_STYLE_GRADIENT_LINEAR:
    case SWF_FILL_STYLE_GRADIENT_RADIAL:
	swf_read_matrix(bs, &style->gradient.matrix);
	if (tag_type == SWF_TAG_DefineMorphShape)
	    swf_read_matrix(bs, &style->gradient.matrix_morph);
	break;
    case SWF_FILL_STYLE_BITMAP_CLIPPED_S:
	break;
    case SWF_FILL_STYLE_BITMAP_TILLED_H:
	break;
    case SWF_FILL_STYLE_BITMAP_CLIPPED_H:
	break;
    }
}

void swf_read_shapewithstyle(struct bstream* bs,
	struct swf_shape_with_style* shape, uint16_t tag_type)
{
    int i;
    shape->styles.fill.coun_ = bs_read_u8(bs);
    if (shape->styles.fill.coun_ == 0xff)
	shape->styles.fill.count = bs_read_u16(bs);
    shape->styles.fill.styles = malloc(sizeof(*shape->styles.fill.styles)
	    * shape->styles.fill.count);
    for (i=0; i < shape->styles.fill.count; ++i)
	swf_read_shapefillstyle(bs, &shape->styles.fill.styles[i], tag_type);
dtrace;
//    swf_read_shaperecords(bs, shape->records);
}

SWF_TAGPSR(DefineShape)
{
    struct swf_defineshape d;
    d.id = bs_read_u16(swf->bs);
    swf_read_rect(swf->bs, &d.br);
    if (swf->th.code == SWF_TAG_DefineMorphShape) {
dtrace;
    } else swf_read_shapewithstyle(swf->bs, &d.shape, swf->th.code);
}

void swf_read_taghdr(struct bstream* bs, struct swf_tag_hdr* th)
{
    assert(bs && th);
    th->_ = bs_read_u16(bs);
    th->size = (th->siz_ == 0x3f) ? bs_read_u32(bs) : th->siz_;
}

void swf_set_tagpsr(struct swf_tagpsr* tagpsr, uint8_t code,
	void (*prsr)(struct swf* swf))
{
    assert(tagpsr && prsr);
    tagpsr[code].prsr = prsr;
}

void swf_tagpsr_init(struct swf_tagpsr* tagpsr)
{
#define	 SWF_TAGPSR_ENTRY(tag) \
	tagpsr[SWF_TAG_ ## tag].prsr = swf_tagpsr_ ## tag; \
	tagpsr[SWF_TAG_ ## tag].name = #tag;
#define	 SWF_TAGPSR_ENTRY2(tag, tag2) \
	tagpsr[SWF_TAG_ ## tag].prsr = swf_tagpsr_ ## tag2; \
	tagpsr[SWF_TAG_ ## tag].name = #tag;
    SWF_TAGPSR_ENTRY2(End, Default);
    SWF_TAGPSR_ENTRY2(ShowFrame, Default);
    SWF_TAGPSR_ENTRY2(DefineShape, Default);
    SWF_TAGPSR_ENTRY2(FreeCharacter, Default);
    SWF_TAGPSR_ENTRY2(PlaceObject, Default);
    SWF_TAGPSR_ENTRY2(RemoveObject, Default);
    SWF_TAGPSR_ENTRY2(DefineBits, Default);
    SWF_TAGPSR_ENTRY2(DefineButton, Default);
    SWF_TAGPSR_ENTRY2(JPEGTables, Default);
    SWF_TAGPSR_ENTRY(SetBackgroundColor);
    SWF_TAGPSR_ENTRY2(DefineFont, Default);
    SWF_TAGPSR_ENTRY2(DefineText, Default);
    SWF_TAGPSR_ENTRY2(DoAction, Default);
    SWF_TAGPSR_ENTRY2(DefineFontInfo, Default);
    SWF_TAGPSR_ENTRY2(DefineSound, Default);
    SWF_TAGPSR_ENTRY2(StartSound, Default);
    SWF_TAGPSR_ENTRY2(StopSound, Default);
    SWF_TAGPSR_ENTRY2(DefineButtonSound, Default);
    SWF_TAGPSR_ENTRY2(SoundStreamHead, Default);
    SWF_TAGPSR_ENTRY2(SoundStreamBlock, Default);
    SWF_TAGPSR_ENTRY2(DefineBitsLossless, Default);
    SWF_TAGPSR_ENTRY2(DefineBitsJPEG2, Default);
    SWF_TAGPSR_ENTRY2(DefineShape2, Default);
    SWF_TAGPSR_ENTRY2(DefineButtonCxform, Default);
    SWF_TAGPSR_ENTRY2(Protect, Default);
    SWF_TAGPSR_ENTRY2(PathsArePostscript, Default);
    SWF_TAGPSR_ENTRY2(PlaceObject2, Default);

    SWF_TAGPSR_ENTRY2(RemoveObject2, Default);
    SWF_TAGPSR_ENTRY2(SyncFrame, Default);

    SWF_TAGPSR_ENTRY2(FreeAll, Default);
    SWF_TAGPSR_ENTRY2(DefineShape3, Default);
    SWF_TAGPSR_ENTRY2(DefineText2, Default);
    SWF_TAGPSR_ENTRY2(DefineButton2, Default);
    SWF_TAGPSR_ENTRY2(DefineBitsJPEG3, Default);
    SWF_TAGPSR_ENTRY2(DefineBitsLossless2, Default);
    SWF_TAGPSR_ENTRY2(DefineSprite, Default);
    SWF_TAGPSR_ENTRY2(NameCharacter, Default);
    SWF_TAGPSR_ENTRY2(SerialNumber, Default);
    SWF_TAGPSR_ENTRY2(DefineTextFormat, Default);
    SWF_TAGPSR_ENTRY2(FrameLabel, Default);

    SWF_TAGPSR_ENTRY2(SoundStreamHead2, Default);
    SWF_TAGPSR_ENTRY2(DefineMorphShape, Default);
    SWF_TAGPSR_ENTRY2(GenerateFrame, Default);
    SWF_TAGPSR_ENTRY2(DefineFont2, Default);
    SWF_TAGPSR_ENTRY2(GeneratorCommand, Default);
    SWF_TAGPSR_ENTRY2(DefineEditText, Default);
    SWF_TAGPSR_ENTRY2(DefineVideo, Default);
    SWF_TAGPSR_ENTRY2(DefineCommandObject, Default);
    SWF_TAGPSR_ENTRY2(CharacterSet, Default);
    SWF_TAGPSR_ENTRY2(ExternalFont, Default);



    SWF_TAGPSR_ENTRY2(Export, Default);
    SWF_TAGPSR_ENTRY2(Import, Default);
    SWF_TAGPSR_ENTRY2(ProtectDebug, Default);
    SWF_TAGPSR_ENTRY2(DoInitAction, Default);
    SWF_TAGPSR_ENTRY2(DefineVideoStream, Default);
    SWF_TAGPSR_ENTRY2(VideoFrame, Default);
    SWF_TAGPSR_ENTRY2(DefineFontInfo2, Default);

    SWF_TAGPSR_ENTRY2(ProtectDebug2, Default);
    SWF_TAGPSR_ENTRY(ScriptLimits);
    SWF_TAGPSR_ENTRY2(SetTabIndex, Default);



    SWF_TAGPSR_ENTRY2(PlaceObject3, Default);
}

// vim:sts=4:ts=8:
