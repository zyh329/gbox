/*!The Graphic Box Library
 * 
 * GBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author		ruki
 * @file		rgb888.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PIXMAP_RGB888_H
#define GB_CORE_PIXMAP_RGB888_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "rgb24.h"
#include "rgb32.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */

static gb_pixel_t gb_pixmap_rgb888_pixel(gb_color_t color);
static gb_color_t gb_pixmap_rgb888_color(gb_pixel_t pixel);
static __tb_inline__ tb_uint32_t gb_pixmap_rgb888_blend(tb_uint32_t d, tb_uint32_t s, tb_byte_t a)
{
	gb_color_t c;
	gb_color_t cs = gb_pixmap_rgb888_color(s & 0x00ffffff);
	gb_color_t cd = gb_pixmap_rgb888_color(d & 0x00ffffff);
	c.r = ((a * (cs.r - cd.r)) >> 8) + cd.r;
	c.g = ((a * (cs.g - cd.g)) >> 8) + cd.g;
	c.b = ((a * (cs.b - cd.b)) >> 8) + cd.b;
	return gb_pixmap_rgb888_pixel(c) & 0x00ffffff;
}
static __tb_inline__ tb_uint32_t gb_pixmap_rgb888_blend2(tb_uint32_t d, gb_color_t s, tb_byte_t a)
{
	gb_color_t c;
	gb_color_t cd = gb_pixmap_rgb888_color(d & 0x00ffffff);
	c.r = ((a * (s.r - cd.r)) >> 8) + cd.r;
	c.g = ((a * (s.g - cd.g)) >> 8) + cd.g;
	c.b = ((a * (s.b - cd.b)) >> 8) + cd.b;
	return gb_pixmap_rgb888_pixel(c) & 0x00ffffff;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
static __tb_inline__ gb_pixel_t gb_pixmap_rgb888_pixel(gb_color_t color)
{
	return gb_pixmap_rgb32_pixel(color) & 0x00ffffff;
}
static __tb_inline__ gb_color_t gb_pixmap_rgb888_color(gb_pixel_t pixel)
{
	return gb_pixmap_rgb32_color(pixel & 0x00ffffff);
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_pixel_set_la(tb_pointer_t data, tb_uint32_t pixel, tb_byte_t alpha)
{
	tb_bits_set_u24_le(data, gb_pixmap_rgb888_blend(tb_bits_get_u24_le(data), pixel, alpha));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_pixel_set_ba(tb_pointer_t data, tb_uint32_t pixel, tb_byte_t alpha)
{
	tb_bits_set_u24_be(data, gb_pixmap_rgb888_blend(tb_bits_get_u24_be(data), pixel, alpha));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_pixel_cpy_la(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
	tb_bits_set_u24_le(data, gb_pixmap_rgb888_blend(tb_bits_get_u24_le(data), tb_bits_get_u24_le(source), alpha));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_pixel_cpy_ba(tb_pointer_t data, tb_cpointer_t source, tb_byte_t alpha)
{
	tb_bits_set_u24_be(data, gb_pixmap_rgb888_blend(tb_bits_get_u24_be(data), tb_bits_get_u24_be(source), alpha));
}
static __tb_inline__ gb_color_t gb_pixmap_rgb888_color_get_l(tb_cpointer_t data)
{
	return gb_pixmap_rgb888_color(tb_bits_get_u24_le(data));
}
static __tb_inline__ gb_color_t gb_pixmap_rgb888_color_get_b(tb_cpointer_t data)
{
	return gb_pixmap_rgb888_color(tb_bits_get_u24_be(data));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_color_set_lo(tb_pointer_t data, gb_color_t color)
{
	tb_bits_set_u24_le(data, gb_pixmap_rgb888_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_color_set_bo(tb_pointer_t data, gb_color_t color)
{
	tb_bits_set_u24_be(data, gb_pixmap_rgb888_pixel(color));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_color_set_la(tb_pointer_t data, gb_color_t color)
{
	tb_bits_set_u24_le(data, gb_pixmap_rgb888_blend(tb_bits_get_u24_le(data), gb_pixmap_rgb888_pixel(color), color.a));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_color_set_ba(tb_pointer_t data, gb_color_t color)
{
	tb_bits_set_u24_be(data, gb_pixmap_rgb888_blend(tb_bits_get_u24_be(data), gb_pixmap_rgb888_pixel(color), color.a));
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_pixels_fill_la(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
	tb_byte_t* 		p = (tb_byte_t*)data;
	tb_byte_t* 		e = p + count * 3;
	gb_color_t 		s = gb_pixmap_rgb888_color(pixel);
	while (p < e)
	{
		tb_bits_set_u24_le(p, gb_pixmap_rgb888_blend2(tb_bits_get_u24_le(p), s, alpha));
		p += 3;
	}
}
static __tb_inline__ tb_void_t gb_pixmap_rgb888_pixels_fill_ba(tb_pointer_t data, gb_pixel_t pixel, tb_size_t count, tb_byte_t alpha)
{
	tb_byte_t* 		p = (tb_byte_t*)data;
	tb_byte_t* 		e = p + count * 3;
	gb_color_t 		s = gb_pixmap_rgb888_color(pixel);
	while (p < e)
	{
		tb_bits_set_u24_be(p, gb_pixmap_rgb888_blend2(tb_bits_get_u24_be(p), s, alpha));
		p += 3;
	}
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static gb_pixmap_t const g_pixmap_lo_rgb888 =
{ 	
	"rgb888"
, 	24
, 	3
, 	GB_PIXFMT_RGB888 
, 	gb_pixmap_rgb888_pixel
, 	gb_pixmap_rgb888_color
,	gb_pixmap_rgb24_pixel_get_l
,	gb_pixmap_rgb24_pixel_set_lo
, 	gb_pixmap_rgb24_pixel_cpy_o
,	gb_pixmap_rgb888_color_get_l
,	gb_pixmap_rgb888_color_set_lo
, 	gb_pixmap_rgb24_pixels_fill_lo
};

static gb_pixmap_t const g_pixmap_bo_rgb888 =
{ 	
	"rgb888"
, 	24
, 	3
, 	GB_PIXFMT_RGB888 | GB_PIXFMT_BENDIAN
, 	gb_pixmap_rgb888_pixel
, 	gb_pixmap_rgb888_color
,	gb_pixmap_rgb24_pixel_get_b
,	gb_pixmap_rgb24_pixel_set_bo
, 	gb_pixmap_rgb24_pixel_cpy_o
,	gb_pixmap_rgb888_color_get_b
,	gb_pixmap_rgb888_color_set_bo
, 	gb_pixmap_rgb24_pixels_fill_bo
};

static gb_pixmap_t const g_pixmap_la_rgb888 =
{ 	
	"rgb888"
, 	24
, 	3
, 	GB_PIXFMT_RGB888 
, 	gb_pixmap_rgb888_pixel
, 	gb_pixmap_rgb888_color
,	gb_pixmap_rgb24_pixel_get_l
,	gb_pixmap_rgb888_pixel_set_la
, 	gb_pixmap_rgb888_pixel_cpy_la
,	gb_pixmap_rgb888_color_get_l
,	gb_pixmap_rgb888_color_set_la
, 	gb_pixmap_rgb888_pixels_fill_la
};

static gb_pixmap_t const g_pixmap_ba_rgb888 =
{ 	
	"rgb888"
, 	24
, 	3
, 	GB_PIXFMT_RGB888 | GB_PIXFMT_BENDIAN
, 	gb_pixmap_rgb888_pixel
, 	gb_pixmap_rgb888_color
,	gb_pixmap_rgb24_pixel_get_b
,	gb_pixmap_rgb888_pixel_set_ba
, 	gb_pixmap_rgb888_pixel_cpy_ba
,	gb_pixmap_rgb888_color_get_b
,	gb_pixmap_rgb888_color_set_ba
, 	gb_pixmap_rgb888_pixels_fill_ba
};

#endif

