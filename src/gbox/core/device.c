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
 * @author      ruki
 * @file        device.c
 * @ingroup     core
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "device"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "device.h"
#include "device/prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
#ifdef GB_CONFIG_THIRD_HAVE_GL
__tb_extern_c__ gb_device_ref_t gb_device_init_gl(gb_window_ref_t window);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_device_ref_t gb_device_init(gb_window_ref_t window)
{
    // check
    tb_assert_and_check_return_val(window, tb_null);

    // the window mode
    tb_size_t mode = gb_window_mode(window);
    tb_assert_and_check_return_val(mode, tb_null);

    // done
    gb_device_ref_t device = tb_null;
    switch (mode)
    {
#ifdef GB_CONFIG_THIRD_HAVE_GL
    case GB_WINDOW_MODE_GL:
        device = gb_device_init_gl(window);
        break;
#endif
    case GB_WINDOW_MODE_BITMAP:
        device = gb_device_init_bitmap(gb_window_bitmap(window));
        break;
    default:
        // trace
        tb_trace_e("unknown window mode: %lu", mode);
        break;
    }

    // ok?
    return device;
}
tb_void_t gb_device_exit(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl);

    // exit it
    if (impl->exit) impl->exit(device);
}
tb_size_t gb_device_type(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, GB_DEVICE_TYPE_NONE);

    // the type
    return impl->type;
}
tb_size_t gb_device_pixfmt(gb_device_ref_t device)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return_val(impl, GB_PIXFMT_NONE);

    // the pixfmt
    return impl->pixfmt;
}
tb_void_t gb_device_draw_clear(gb_device_ref_t device, gb_color_t color)
{
    // check
    gb_device_impl_t* impl = (gb_device_impl_t*)device;
    tb_assert_and_check_return(impl && impl->draw_clear);

    // clear it
    impl->draw_clear(device, color);
}
