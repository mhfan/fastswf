//#!/usr/bin/tcc -run 
/****************************************************************
 * $ID: swf.cpp        Thu, 06 Apr 2006 00:20:21 +0800  mhfan $ *
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

extern "C" {

#ifdef	RENDER_TWIN
#include <twin.h>

#include "twinint.h"
#include "twin_x11.h"

#ifdef	Button2
#undef	Button2
#endif
#else
#include <cairo.h>
#endif

};

#include <cstring>
#include <cerrno>

#include "swf.hpp"

namespace SWF {

uint8_t  Tag::swfv;
int SoundMixer::fd = -1;
uint32_t Tag::size, Tag::tend;
uint8_t FillStyleGradient::alpha;
uint8_t TextEntry::gnb, TextEntry::anb;
bool ShapeWithStyle::lots, ShapeWithStyle::hasa;//, ShapeWithStyle::news;
uint16_t ShapeRecordSetup::fcnt, ShapeRecordSetup::lcnt;
int16_t Audio::obuf[Audio::OUTPUT_BUFFER_SIZE];
#ifdef	ABSOLUTE_COORDINATE
Twip ShapeRecord::x0, ShapeRecord::y0;
#endif
StyleBits ShapeRecordSetup::snbit;

struct JPEGClient DefineBitsJPEG::jclt;
struct jpeg_error_mgr DefineBitsJPEG::jerr;
struct jpeg_source_mgr DefineBitsJPEG::jsmgr;
struct jpeg_decompress_struct DefineBitsJPEG::jtbl;

SoundStreamHead2* SoundStreamBlock::head;
//SoundStreamBlock* SoundStreamBlock::last;
#ifdef	NDEBUG
uint16_t    DefineTag::id;	// XXX
#endif
Dictionary* DefineTag::di;
FrameRender*DefineTag::fr;
SoundMixer* DefineTag::sm;
FrameList* ControlTag::fl;
DispList*  ControlTag::dl;

#ifdef	RENDER_TWIN
struct FrameRenderTwin: public FrameRender {
    FrameBuffer fb;
    f32_16_t sx, sy;
    twin_pixmap_t* pixmap;

    ~FrameRenderTwin() { twin_pixmap_destroy(pixmap); };
     FrameRenderTwin(uint16_t w = DEFAULT_SCREEN_WIDTH,
		     uint16_t h = DEFAULT_SCREEN_HEIGHT) {
	fb.viewport(0, 0, w, h, FrameBuffer::CENTER_MASK);	// XXX
	// TODO: show boot splash?
    };

    void SetViewport(Rect& br) {
	twin_pointer_t pixels;
	uint16_t w = br.width() / TWIPS, h = br.height() / TWIPS;

	sx = (1 << 16) / TWIPS, sy = (1 << 16) / TWIPS;

	if (w < fb.width && h < fb.height)
	     fb.viewport(0, 0, w, h, FrameBuffer::CENTER_MASK);
	else sx = sx * fb.width / w, sy = sy * fb.height / h;

	pixels.rgb16 = fb.pixl;
	pixmap = twin_pixmap_create_const(TWIN_RGB16,   // XXX
		fb.width, fb.height, fb.bpl, pixels);
	pixmap->clip.left   = pixmap->clip.top = 0;
	pixmap->clip.right  = pixmap->width;
	pixmap->clip.bottom = pixmap->height;
    };

    void SetBackgroundColor(RGB& rgb) {
	twin_fill(pixmap, rgb, TWIN_OVER,
		0, 0, pixmap->width, pixmap->height);
    };

    void ShowShape(ShapeWithStyle* sws) {
	ShapeRecordSetup* psr = NULL;
	twin_path_t* path = twin_path_create();
	twin_path_move(path, 0, 0);
	for (std::vector<ShapeRecord*>::iterator it = sws->srVec.begin();
		it != sws->srVec.end(); ++it) {
	    if ((*it)->type) {
		if ((*it)->edgt) {
		    ShapeRecordEdgeLine * psr =
			    reinterpret_cast<ShapeRecordEdgeLine *>(*it);
		    twin_path_rdraw (path, psr->dx, psr->dy);
		} else {
		    ShapeRecordEdgeCurve* psr =
			    reinterpret_cast<ShapeRecordEdgeCurve*>(*it);
		    twin_path_rcurve(path, psr->ctrl.dx, psr->ctrl.dy,
			    psr->ctrl.dx, psr->ctrl.dy, psr->anch.dx,
			    psr->anch.dy);
		}
	    } else {
		if (psr && (psr->f0s || psr->f1s)) {
		    FillStyle* pfs = fsVec[psr->f0s];
		    switch (pfs->type) {
		    case FillStyle::SOLID:
		    case FillStyle::GRADIENT_LINEAR:
		    case FillStyle::GRADIENT_RADIAL:
		    case FillStyle::BITMAP_TITLED_SMOOTH: 
		    case FillStyle::BITMAP_TILLED_HARD:
		    case FillStyle::BITMAP_CLIPPED_HARD:
		    case FillStyle::BITMAP_CLIPPED_SMOOTH: 
		    default: dtrace;
		    }
		    twin_fill_path(fr->pixmap, path, 0, 0);
		}
		psr = reinterpret_cast<ShapeRecordSetup    *>(*it);
		if (psr->has_mv2) {
		    twin_path_empty (path);
		    twin_path_rmove (path, psr->mv2.dx, psr->mv2.dy);
		}
		if (psr->lns) {
dtrace;
		}
	    }
	}
    };
};

Flash::Flash() { fr = new FrameRenderTwin (); };

#else// RENDER_CAIRO:

struct FrameRenderCairo: public FrameRender {
    cairo_t* cr;
    FrameBuffer fb;
    cairo_surface_t* sf;

    ~FrameRenderCairo() {
	cairo_surface_destroy(sf);	cairo_destroy(cr);
    };
     FrameRenderCairo(uint16_t w = DEFAULT_SCREEN_WIDTH,
		      uint16_t h = DEFAULT_SCREEN_HEIGHT) {
	fb.viewport(0, 0, w, h, FrameBuffer::CENTER_MASK);	// XXX
	// TODO: show boot splash?
    };

    void SetViewport(Rect& br) {
	cairo_format_t fmt;
	uint16_t w = br.width() / TWIPS, h = br.height() / TWIPS;

	switch (fb.depth) {
	case 32: fmt = CAIRO_FORMAT_ARGB32;	break;
	case 24: fmt = CAIRO_FORMAT_RGB24;	break;
	case 16:
	default: fmt = CAIRO_FORMAT_RGB16_565;  break;
	}

	if (w < fb.width && h < fb.height)
	    fb.viewport(0, 0, w, h, FrameBuffer::CENTER_MASK);

	cr = cairo_create(sf = cairo_image_surface_create_for_data(
                (unsigned char*)fb.pixl, fmt, fb.width, fb.height,
                fb.ppl * fb.bpp));

	cairo_scale(cr, 1.f * fb.width  / w / TWIPS
		      , 1.f * fb.height / h / TWIPS);
    };

    void SetBackgroundColor(RGB& rgb) {
#ifndef	NORMALIZED_COLOR
	cairo_set_source_rgb(cr, rgb.r/255.f, rgb.g/255.f, rgb.b/255.f);
#else
	cairo_set_source_rgb(cr, rgb.f.r, rgb.f.g, rgb.f.b);
#endif
	cairo_paint(cr);
    };

    void DrawShape(ShapeWithStyle* sws, PlaceObject* pl) {
	ShapeRecordSetup* psr = NULL;
#ifndef	ABSOLUTE_COORDINATE
	cairo_move_to(cr, 0, 0);	// for relative coordnate
#endif
	for (std::vector<ShapeRecord*>::iterator it = sws->srVec.begin();
		it != sws->srVec.end(); ++it) {
	    if ((*it)->type) {
		if ((*it)->edgt) {
		    ShapeRecordEdgeLine * psr =
			    reinterpret_cast<ShapeRecordEdgeLine *>(*it);
#ifndef	ABSOLUTE_COORDINATE
		    cairo_rel_line_to(cr, psr->dx, psr->dy);
#else
		    cairo_line_to(cr, psr->dx, psr->dy);
#endif
		} else {
		    ShapeRecordEdgeCurve* psr =
			    reinterpret_cast<ShapeRecordEdgeCurve*>(*it);
#ifndef	ABSOLUTE_COORDINATE
#ifndef	CUBIC_BEZIER
		    cairo_rel_curve_to(cr, (psr->ctrl.dx << 1)/3,
					   (psr->ctrl.dy << 1)/3,
			    psr->ctrl.dx  + psr->anch.dx/3,
			    psr->ctrl.dy  + psr->anch.dy/3,
			    psr->ctrl.dx  + psr->anch.dx,
			    psr->ctrl.dy  + psr->anch.dy);
#else
		    cairo_rel_curve_to(cr, psr->ctrl.dx, psr->ctrl.dy,
					   psr->cubi.dx, psr->cubi.dy,
					   psr->anch.dx, psr->anch.dy);
#endif
#else// RELATIVE_COORDINATE:
#ifndef	CUBIC_BEZIER
#warning "Stupid, please define the CUBIC_BEZIER macro instead!"
		    double x0, y0;
		    cairo_get_current_point(cr, &x0, &y0);
		    cairo_curve_to(cr, ((psr->ctrl.dx << 1) + x0)/3,
				       ((psr->ctrl.dy << 1) + y0)/3,
			     ((psr->ctrl.dx << 1) + psr->anch.dx)/3,
			     ((psr->ctrl.dy << 1) + psr->anch.dy)/3,
			       psr->anch.dx, psr->anch.dy);
#else
		    cairo_curve_to(cr, psr->ctrl.dx, psr->ctrl.dy,
				       psr->cubi.dx, psr->cubi.dy,
				       psr->anch.dx, psr->anch.dy);
#endif
#endif//ABSOLUTE_COORDINATE
		}
	    } else {
		if (psr) {
		    if (psr->lns < (uint16_t)-1)
			StrokePath(sws->lsVec[psr->lns]);
		    if (psr->f1s < (uint16_t)-1)	// XXX
			FillRegion(sws->fsVec[psr->f1s]); else
		    if (psr->f0s < (uint16_t)-1)
			FillRegion(sws->fsVec[psr->f0s]);
		}   psr =   reinterpret_cast<ShapeRecordSetup    *>(*it);
		if (psr->has_mv2)
#ifndef	ABSOLUTE_COORDINATE
		    cairo_rel_move_to(cr, psr->mv2.dx, psr->mv2.dy);
#else
		    cairo_move_to    (cr, psr->mv2.dx, psr->mv2.dy);
#endif
	    }
	}

#if 0
	// FIXME: handle clipping here.
	if (clip) {
	    if (clip < depth) cairo_restore(cr);
	    else {
		cairo_save(cr);
		cairo_clip(cr);
	    }
	}
#endif/* comment by mhfan */
		
		if (psr) {
		    if (psr->lns < (uint16_t)-1)
			StrokePath(sws->lsVec[psr->lns]);
		    if (psr->f1s < (uint16_t)-1)	// XXX
			FillRegion(sws->fsVec[psr->f1s]); else
		    if (psr->f0s < (uint16_t)-1)
			FillRegion(sws->fsVec[psr->f0s]);
		}
    };

private:
    void StrokePath(LineStyle& ls) {
	cairo_set_line_width (cr, ls.width);
#ifndef NORMALIZED_COLOR
	cairo_set_source_rgba(cr, ls.rgba.r/255.f, ls.rgba.g/255.f,
				  ls.rgba.b/255.f, ls.rgba.a/255.f);
#else
	cairo_set_source_rgba(cr, ls.rgba.f.r, ls.rgba.f.g,
				  ls.rgba.f.b, ls.rgba.f.a);
#endif
	cairo_stroke(cr);
    };

    void FillRegion(FillStyle* pfs) {
	switch (pfs->type) {
	case FillStyle::SOLID: {
	     RGBA& rgba = reinterpret_cast<FillStyleSolid*>(pfs)->rgba;
#ifndef NORMALIZED_COLOR
	     cairo_set_source_rgba(cr, rgba.r/255.f, rgba.g/255.f
				     , rgba.b/255.f, rgba.a/255.f);
#else
	     cairo_set_source_rgba(cr, rgba.f.r, rgba.f.g, rgba.f.b, rgba.f.a);
#endif
//cairo_clip(cr);
	     cairo_fill(cr);	// XXX
	}    return;

	case FillStyle::GRADIENT_LINEAR:
	case FillStyle::GRADIENT_RADIAL: {
	     cairo_pattern_t* pt;
	     if (pfs->type == FillStyle::GRADIENT_LINEAR)
		  pt = cairo_pattern_create_linear(-16384.f, 0.f, 16383.f, 0.f);
	     else pt = cairo_pattern_create_radial(0.f, 0.f, 0.f, 0.f, 0.f
								, 16383.f);
	     FillStyleGradient* pg = reinterpret_cast<FillStyleGradient*>(pfs);

	     for (std::vector<GradientRecord>::iterator it = pg->grVec.begin();
		     it != pg->grVec.end(); ++it) {
#ifndef	NORMALIZED_COLOR
		 cairo_pattern_add_color_stop_rgba(pt, it->pos/255.f
			 , it->rgba.r/255.f, it->rgba.g/255.f
			 , it->rgba.b/255.f, it->rgba.a/255.f);
#else
		 cairo_pattern_add_color_stop_rgba(pt, it->fpos
			 , it->rgba.f.r, it->rgba.f.g
			 , it->rgba.f.b, it->rgba.f.a);
#endif
	     }

	     cairo_matrix_t mx;
#ifndef	CTM_FLOAT_POINT
	     cairo_matrix_init(&mx, pg->mat.sx/65535.f, pg->mat.r1/65535.f
				  , pg->mat.r1/65535.f, pg->mat.sy/65535.f
				  , pg->mat.tx	      , pg->mat.ty);
#else
	     cairo_matrix_init(&mx, pg->mat.f.sx, pg->mat.f.r1
				  , pg->mat.f.r1, pg->mat.f.sy
				  , pg->mat.tx	, pg->mat.ty);
#endif

	     cairo_matrix_scale(&mx, TWIPS, TWIPS);

	     cairo_matrix_invert(&mx);
	     cairo_pattern_set_matrix(pt, &mx);

	     cairo_set_source(cr, pt);
	     cairo_fill(cr);

	     cairo_pattern_destroy(pt);
	}    break;

	case FillStyle::BITMAP_TILLED_HARD:
	case FillStyle::BITMAP_CLIPPED_HARD:

	case FillStyle::BITMAP_TITLED_SMOOTH: 
	case FillStyle::BITMAP_CLIPPED_SMOOTH: {    // FIXME: how to smooth?
	     FillStyleBitmap* pb = reinterpret_cast<FillStyleBitmap*>(pfs);
	     Dictionary::iterator it = DefineTag::di->find(pb->rfid);
	     ImageBitmap* im = dynamic_cast<ImageBitmap*>(it->second);
	     assert(im && im->bmpd);
#if 1
	     cairo_format_t fmt;
	     switch (im->fmtn) {
	     case DefineBitsLossless::FMT_32BIT:
		    fmt = CAIRO_FORMAT_ARGB32;	    break;
	     case DefineBitsLossless::FMT_24BIT:
		    fmt = CAIRO_FORMAT_RGB24;	    break;
	     case DefineBitsLossless::FMT_16BIT:
	     default: 
		    fmt = CAIRO_FORMAT_RGB16_565;   break;
	     }
#endif
	     cairo_surface_t* sf = cairo_image_surface_create_for_data(
		     im->bmpd, fmt, im->width, im->height, im->pitch);
	     cairo_pattern_t* pt = cairo_pattern_create_for_surface(sf);

	     if ((pfs->type & 0x02))
		  cairo_pattern_set_extend(pt, CAIRO_EXTEND_NONE);
	     else cairo_pattern_set_extend(pt, CAIRO_EXTEND_REPEAT);

	     cairo_matrix_t mx;
#ifndef	CTM_FLOAT_POINT
	     cairo_matrix_init(&mx, pb->mat.sx/65535.f, pb->mat.r1/65535.f
				  , pb->mat.r1/65535.f, pb->mat.sy/65535.f
				  , pb->mat.tx	      , pb->mat.ty);
#else
	     cairo_matrix_init(&mx, pb->mat.f.sx, pb->mat.f.r1
				  , pb->mat.f.r1, pb->mat.f.sy
				  , pb->mat.tx	, pb->mat.ty);
#endif

	     cairo_matrix_invert(&mx);
	     cairo_pattern_set_matrix(pt, &mx);

	     cairo_set_source(cr, pt);
	     cairo_fill(cr);

	     cairo_pattern_destroy(pt);
	     cairo_surface_destroy(sf);
	}    break;
	default: dtrace;	return;
	}
    };
};

Flash::Flash() {
    fr = new FrameRenderCairo();
    sm = new SoundMixer();
};
#endif//RENDER_TWIN

bool Flash::open(const char* fn)
{
    enum {  MAX_SWF_VERSION		= 8u,
	    EMBEDDED_SWF_ALIGNMENT	= 0x1000u,
	    MAGIC_SIGNATURE_COMPRESSED  = 'SWC',
	    MAGIC_SIGNATURE		= 'SWF',
    };

    std::ios::sync_with_stdio(false);	// XXX

    // TODO: read file by memory mapped io stream.
    fs.open(fn, std::ios::in | std::ios::binary);
    while (fs.read((char*)fh._, sizeof(fh.__)) && 
		//MAX_SWF_VERSION < fh.ver &&
	    !(fh.mgc == MAGIC_SIGNATURE ||
	      fh.mgc == MAGIC_SIGNATURE_COMPRESSED))
	fs.ignore(EMBEDDED_SWF_ALIGNMENT - sizeof(fh.__));

    if (!fs.read((char*)&fh.len, sizeof(fh.len))) {
	std::cerr << "\nInvalid SWF: " << fn << std::endl;
					return false;
    }	bs.init(fs, (fh._[0] == 'C'));	bs.pos += sizeof(fh);

    std::cout.put('\n').put(fh._[2]).put(fh._[1]).put(fh._[0]).put('v')
	    << fh.ver << " with" << std::setw(8)
	    << fh.len << " bytes: " << fn << std::endl;

    mh.load(bs);    mh.dump(std::cout); fr->SetViewport(mh.fs);

#if 1
    Tag::swfv = fh.ver;			DefineTag::sm = sm;
    ControlTag::dl = &dl;		DefineTag::di = &di;
    ControlTag::fl = &fl;		DefineTag::fr = fr;
    fl.resize(mh.fc);			tl.start();
    fl.pf = fl.lf = 0u;
#endif

    rl = fn;				return true;
}

void Flash::load(uint8_t ht)
{
    if (!(bs.pos < fh.len) || bs.eof()) return;  // XXX

    do {

    Tag t(bs), *pt;
    Tag::tend = bs.pos + t.size;
    switch (t.code) {
#define	NEW_TAG_CASE(tag) \
    case TAG::CODE::tag:	pt = new tag(t, bs);	break

    case TAG::CODE::End:		//NEW_TAG_CASE(End);
	ControlTag::fl = &fl;		// XXX
	pt = new End(t, bs);		ht = LOAD_BY_TAG; 	break;
    case TAG::CODE::ShowFrame:		//NEW_TAG_CASE(ShowFrame);
	pt = new ShowFrame(t, bs);	ht&=~LOAD_BY_FRAME; 	break;
    NEW_TAG_CASE(DefineShape);
    NEW_TAG_CASE(FreeCharacter);
    NEW_TAG_CASE(PlaceObject);
    NEW_TAG_CASE(RemoveObject);
    NEW_TAG_CASE(DefineBits);
    NEW_TAG_CASE(DefineButton);
    NEW_TAG_CASE(JPEGTables);
    NEW_TAG_CASE(SetBackgroundColor);
    NEW_TAG_CASE(DefineFont);
    NEW_TAG_CASE(DefineText);
    NEW_TAG_CASE(DoAction);
    NEW_TAG_CASE(DefineFontInfo);
    NEW_TAG_CASE(DefineSound);
    NEW_TAG_CASE(StartSound);
    NEW_TAG_CASE(StopSound);
    NEW_TAG_CASE(DefineButtonSound);
    NEW_TAG_CASE(SoundStreamHead);
    NEW_TAG_CASE(SoundStreamBlock);
    NEW_TAG_CASE(DefineBitsLossless);
    NEW_TAG_CASE(DefineBitsJPEG2);
    NEW_TAG_CASE(DefineShape2);
    NEW_TAG_CASE(DefineButtonCXForm);
    NEW_TAG_CASE(Protect);
    NEW_TAG_CASE(PathsArePostscript);
    NEW_TAG_CASE(PlaceObject2);

    NEW_TAG_CASE(RemoveObject2);
    NEW_TAG_CASE(SyncFrame);

    NEW_TAG_CASE(FreeAll);
    NEW_TAG_CASE(DefineShape3);
    NEW_TAG_CASE(DefineText2);
    NEW_TAG_CASE(DefineButton2);
    NEW_TAG_CASE(DefineBitsJPEG3);
    NEW_TAG_CASE(DefineBitsLossless2);
    //NEW_TAG_CASE(DefineSprite);
    case TAG::CODE::DefineSprite: {
	DefineSprite* sp = new DefineSprite(t, bs);
	ControlTag::fl = &sp->fl;
	load(ht);   pt =  sp;
	Tag::tend = bs.pos;
    }	break;
    NEW_TAG_CASE(NameCharacter);
    NEW_TAG_CASE(ProductInfo);
    NEW_TAG_CASE(DefineTextFormat);
    NEW_TAG_CASE(FrameLabel);

    NEW_TAG_CASE(SoundStreamHead2);
    NEW_TAG_CASE(DefineMorphShape);
    NEW_TAG_CASE(GenerateFrame);
    NEW_TAG_CASE(DefineFont2);
    NEW_TAG_CASE(GeneratorCommand);
    NEW_TAG_CASE(DefineEditText);
    NEW_TAG_CASE(DefineVideo);
    NEW_TAG_CASE(DefineCommandObject);
    NEW_TAG_CASE(CharacterSet);
    NEW_TAG_CASE(ExternalFont);



    NEW_TAG_CASE(ExportAssets);
    NEW_TAG_CASE(ImportAssets);
    NEW_TAG_CASE(ProtectDebug);
    NEW_TAG_CASE(DoInitAction);
    NEW_TAG_CASE(DefineVideoStream);
    NEW_TAG_CASE(VideoFrame);
    NEW_TAG_CASE(DefineFontInfo2);

    NEW_TAG_CASE(ProtectDebug2);
    NEW_TAG_CASE(ScriptLimits);
    NEW_TAG_CASE(SetTabIndex);



    NEW_TAG_CASE(PlaceObject3);
    // ...
    NEW_TAG_CASE(DefineBitsPtr);
    default:		pt = new TagUnknown(t, bs);		break;
    }			ts.push_back(pt);

    pt->dump(std::clog);	// XXX
    if (Tag::tend != bs.pos) {
#ifndef	NDEBUG
	pt->dump(std::clog);
#endif//NDEBUG
	std::clog << "Unexpected tag end: "
		  << bs.pos << "(" << Tag::tend << ")\n";
	bs.ignore(Tag::tend - bs.pos);	// FIXME: bs.seekg(Tag::tend);
    }

    } while (ht && !bs.eof() && bs.pos < fh.len);
}

void Flash::play(uint8_t flag, int8_t sp)
{
    uint32_t ct = 0, ft = 1000000u * 256u / mh.fr;

    if (sp < 0) ft /= -sp; else ft *= sp;

    for (; !(flag & PLAY_ONCE) || fl.pf < mh.fc; ct += ft) {
	while (!(fl.pf < fl.lf)) load(LOAD_BY_FRAME);

	advf();			// TODO: handle events here

	while (tl.fresh() < ct && fl.lf < mh.fc) load(LOAD_BY_TAG);
//	tl.delay(ct);		// XXX

//	dump(std::clog);	// XXX
	show();			// TODO: skip frame regarding A-V sync
    }
}

};

// vim:sts=4:ts=8:
