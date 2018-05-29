/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Joshua Leung
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file DNA_view2d_types.h
 *  \ingroup DNA
 */

#ifndef __DNA_VIEW2D_TYPES_H__
#define __DNA_VIEW2D_TYPES_H__

#include "DNA_vec_types.h"

/* ---------------------------------- */

/* View 2D data - stored per region */
typedef struct View2D {
	rctf tot, cur;					/* tot - area that data can be drawn in; cur - region of tot that is visible in viewport */
	rcti vert, hor;					/* vert - vertical scrollbar region; hor - horizontal scrollbar region */
	rcti mask;						/* mask - region (in screenspace) within which 'cur' can be viewed */
	
	float min[2], max[2];			/* min/max sizes of 'cur' rect (only when keepzoom not set) */
	float minzoom, maxzoom;			/* allowable zoom factor range (only when (keepzoom & V2D_LIMITZOOM)) is set */
	
	short scroll;					/* scroll - scrollbars to display (bitflag) */
	short scroll_ui;				/* scroll_ui - temp settings used for UI drawing of scrollers */
	
	short keeptot;					/* keeptot - 'cur' rect cannot move outside the 'tot' rect? */
	short keepzoom;					/* keepzoom - axes that zooming cannot occur on, and also clamp within zoom-limits */
	short keepofs;					/* keepofs - axes that translation is not allowed to occur on */
	
	short flag;						/* settings */
	short align;					/* alignment of content in totrect */
	
	short winx, winy;				/* storage of current winx/winy values, set in UI_view2d_size_update */
	short oldwinx, oldwiny;			/* storage of previous winx/winy values encountered by UI_view2d_curRect_validate(), for keepaspect */
	
	short around;					/* pivot point for transforms (rotate and scale) */

	float *tab_offset;				/* different offset per tab, for buttons */
	int tab_num;					/* number of tabs stored */
	int tab_cur;					/* current tab */

	/* Usually set externally (as in, not in view2d files). */
	char alpha_vert, alpha_hor;		/* alpha of vertical and horizontal scrollbars (range is [0, 255]) */
	short size_vert, size_hor;		/* Dynamic size for scrollers without scale markers (no V2D_SCROLL_SCALE_FOO) */
	short pad;

	/* animated smooth view */
	struct SmoothView2DStore *sms;
	struct wmTimer *smooth_timer;
} View2D;

/* ---------------------------------- */

/* view zooming restrictions, per axis (v2d->keepzoom) */
enum {
	/* zoom is clamped to lie within limits set by minzoom and maxzoom */
	V2D_LIMITZOOM     = (1 << 0),
	/* aspect ratio is maintained on view resize */
	V2D_KEEPASPECT    = (1 << 1),
	/* zoom is kept when the window resizes */
	V2D_KEEPZOOM      = (1 << 2),
	/* zooming on x-axis is not allowed */
	V2D_LOCKZOOM_X    = (1 << 8),
	/* zooming on y-axis is not allowed */
	V2D_LOCKZOOM_Y    = (1 << 9),
};

/* view panning restrictions, per axis (v2d->keepofs) */
enum {
	/* panning on x-axis is not allowed */
	V2D_LOCKOFS_X  = (1 << 1),
	/* panning on y-axis is not allowed */
	V2D_LOCKOFS_Y  = (1 << 2),
	/* on resize, keep the x offset */
	V2D_KEEPOFS_X  = (1 << 3),
	/* on resize, keep the y offset */
	V2D_KEEPOFS_Y  = (1 << 4),
};

/* view extent restrictions (v2d->keeptot) */
enum {
	/* 'cur' view can be out of extents of 'tot' */
	V2D_KEEPTOT_FREE      = 0,
	/* 'cur' rect is adjusted so that it satisfies the extents of 'tot', with some compromises */
	V2D_KEEPTOT_BOUNDS    = 1,
	/* 'cur' rect is moved so that the 'minimum' bounds of the 'tot' rect are always respected (particularly in x-axis) */
	V2D_KEEPTOT_STRICT    = 2,
};

/* general refresh settings (v2d->flag) */
enum {
	/* global view2d horizontal locking (for showing same time interval) */
	/* TODO: this flag may be set in old files but is not accessible currently, should be exposed from RNA - Campbell */
	V2D_VIEWSYNC_SCREEN_TIME      = (1 << 0),
	/* within area (i.e. between regions) view2d vertical locking */
	V2D_VIEWSYNC_AREA_VERTICAL    = (1 << 1),
	/* apply pixel offsets on x-axis when setting view matrices */
	V2D_PIXELOFS_X                = (1 << 2),
	/* apply pixel offsets on y-axis when setting view matrices */
	V2D_PIXELOFS_Y                = (1 << 3),
	/* view settings need to be set still... */
	V2D_IS_INITIALISED            = (1 << 10),
};

/* scroller flags for View2D (v2d->scroll) */
enum {
	/* left scrollbar */
	V2D_SCROLL_LEFT              = (1 << 0),
	V2D_SCROLL_RIGHT             = (1 << 1),
	V2D_SCROLL_VERTICAL          = (V2D_SCROLL_LEFT | V2D_SCROLL_RIGHT),
	/* horizontal scrollbar */
	V2D_SCROLL_TOP               = (1 << 2),
	V2D_SCROLL_BOTTOM            = (1 << 3),
	/* UNUSED                    = (1 << 4), */
	V2D_SCROLL_HORIZONTAL        = (V2D_SCROLL_TOP | V2D_SCROLL_BOTTOM),
	/* scale markings - vertical */
	V2D_SCROLL_SCALE_VERTICAL    = (1 << 5),
	/* scale markings - horizontal */
	V2D_SCROLL_SCALE_HORIZONTAL  = (1 << 6),
	/* induce hiding of scrollbars - set by region drawing in response to size of region */
	V2D_SCROLL_VERTICAL_HIDE     = (1 << 7),
	V2D_SCROLL_HORIZONTAL_HIDE   = (1 << 8),
	/* scrollbar extends beyond its available window - set when calculating scrollbars for drawing */
	V2D_SCROLL_VERTICAL_FULLR    = (1 << 9),
	V2D_SCROLL_HORIZONTAL_FULLR  = (1 << 10),
};

/* scroll_ui, activate flag for drawing */
enum {
	V2D_SCROLL_H_ACTIVE       = (1 << 0),
	V2D_SCROLL_V_ACTIVE       = (1 << 1),
};

/* alignment flags for totrect, flags use 'shading-out' convention (v2d->align) */
enum {
	/* all quadrants free */
	V2D_ALIGN_FREE        = 0,
	/* horizontal restrictions */
	V2D_ALIGN_NO_POS_X    = (1 << 0),
	V2D_ALIGN_NO_NEG_X    = (1 << 1),
	/* vertical restrictions */
	V2D_ALIGN_NO_POS_Y    = (1 << 2),
	V2D_ALIGN_NO_NEG_Y    = (1 << 3),
};

#endif

