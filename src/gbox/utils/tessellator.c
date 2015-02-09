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
 * @file        tessellator.c
 * @ingroup     utils
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "tessellator"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "tessellator.h"
#include "impl/tessellator/tessellator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the output points grow
#ifdef __gb_small__
#   define GB_TESSELLATOR_OUTPUTS_GROW                          (32)
#else
#   define GB_TESSELLATOR_OUTPUTS_GROW                          (64)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t gb_tessellator_done_output(gb_tessellator_impl_t* impl)
{
    // check
    tb_assert_abort(impl && impl->mesh && impl->func);

    // init outputs first
    if (!impl->outputs) impl->outputs = tb_vector_init(GB_TESSELLATOR_OUTPUTS_GROW, tb_item_func_mem(sizeof(gb_point_t), tb_null, tb_null));

    // check outputs
    tb_vector_ref_t outputs = impl->outputs;
    tb_assert_abort(outputs);

    // done
    tb_for_all_if (gb_mesh_face_ref_t, face, gb_mesh_face_itor(impl->mesh), face)
    {
        // the face is inside?
        if (gb_tessellator_face_inside(face)) 
        {
            // clear outputs
            tb_vector_clear(outputs);

            // make contour
            gb_mesh_edge_ref_t  head        = gb_mesh_face_edge(face);
            gb_mesh_edge_ref_t  edge        = head;
            gb_point_ref_t      point       = tb_null;
            gb_point_ref_t      point_first = tb_null;
            do
            {
                // the point
                point = gb_tessellator_vertex_point(gb_mesh_edge_org(edge));
                tb_assert_abort(point);

                // append point
                tb_vector_insert_tail(outputs, point);

                // save the first point
                if (!point_first) point_first = point;

                // the next edge
                edge = gb_mesh_edge_lnext(edge);

            } while (edge != head);

            // exists valid contour?
            if (tb_vector_size(outputs) > 2)
            {
                // check
                tb_assert_abort(tb_vector_data(outputs));

                // append the first point for closing the contour
                tb_vector_insert_tail(outputs, point_first);

                // done it
                impl->func((gb_point_ref_t)tb_vector_data(outputs), tb_vector_size(outputs), impl->priv);
            }
        }
    }
}
static tb_void_t gb_tessellator_done_convex(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    tb_assert_abort(impl && impl->func && polygon && bounds);

    // only one convex contour
    tb_assert_abort(polygon->convex && polygon->counts && !polygon->counts[1]);

    // make convex or monotone? done it directly
    if (impl->mode == GB_TESSELLATOR_MODE_CONVEX || impl->mode == GB_TESSELLATOR_MODE_MONOTONE)
    {
        // done it
        impl->func(polygon->points, polygon->counts[0], impl->priv);

        // ok
        return ;
    }

    // must be triangulation mode now
    tb_assert_abort(impl->mode == GB_TESSELLATOR_MODE_TRIANGULATION);

    // make mesh
    if (!gb_tessellator_make_mesh(impl, polygon)) return ;

    // only two faces
    gb_mesh_ref_t mesh = impl->mesh;
    tb_assert_abort(mesh && tb_iterator_size(gb_mesh_face_itor(mesh)) == 2);

    // the arbitrary face is marked "inside" and the triangulation will be not effected
    gb_tessellator_face_inside_set(gb_mesh_face_head(mesh), 1);

    // make triangulation region
    if (!gb_tessellator_make_triangulation(impl)) return ;

    // done output
    gb_tessellator_done_output(impl);
}
static tb_void_t gb_tessellator_done_concave(gb_tessellator_impl_t* impl, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{ 
    // check
    tb_assert_abort(impl && polygon && !polygon->convex && bounds);

    // make mesh
    if (!gb_tessellator_make_mesh(impl, polygon)) return ;

    // make horizontal monotone region
    if (!gb_tessellator_make_monotone(impl, bounds)) return ;

    // need make convex or triangulation polygon?
    if (impl->mode == GB_TESSELLATOR_MODE_CONVEX || impl->mode == GB_TESSELLATOR_MODE_TRIANGULATION)
    {
        // make triangulation region for each horizontal monotone region
        if (!gb_tessellator_make_triangulation(impl)) return ;

        // make convex? 
        if (impl->mode == GB_TESSELLATOR_MODE_CONVEX)
        {
            // TODO merge triangles to the convex polygon
            tb_trace_noimpl();
        }
    }

    // done output
    gb_tessellator_done_output(impl);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
gb_tessellator_ref_t gb_tessellator_init()
{
    // init it
    return (gb_tessellator_ref_t)tb_malloc0_type(gb_tessellator_impl_t);
}
tb_void_t gb_tessellator_exit(gb_tessellator_ref_t tessellator)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // exit mesh
    if (impl->mesh) gb_mesh_exit(impl->mesh);
    impl->mesh = tb_null;

    // exit outputs
    if (impl->outputs) tb_vector_exit(impl->outputs);
    impl->outputs = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t gb_tessellator_mode_set(gb_tessellator_ref_t tessellator, tb_size_t mode)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // set mode
    impl->mode = mode;
}
tb_void_t gb_tessellator_rule_set(gb_tessellator_ref_t tessellator, tb_size_t rule)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // set rule
    impl->rule = rule;
}
tb_void_t gb_tessellator_func_set(gb_tessellator_ref_t tessellator, gb_tessellator_func_t func, tb_cpointer_t priv)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_and_check_return(impl);

    // set func
    impl->func = func;
    impl->priv = priv;
}
tb_void_t gb_tessellator_done(gb_tessellator_ref_t tessellator, gb_polygon_ref_t polygon, gb_rect_ref_t bounds)
{
    // check
    gb_tessellator_impl_t* impl = (gb_tessellator_impl_t*)tessellator;
    tb_assert_abort_and_check_return(impl && impl->func && polygon && polygon->points && polygon->counts && bounds);

    // is convex polygon for each contour?
    if (polygon->convex)
    {
        // done
        tb_size_t       index               = 0;
        gb_point_ref_t  points              = polygon->points;
        tb_uint16_t*    counts              = polygon->counts;
        tb_uint16_t     contour_counts[2]   = {0, 0};
        gb_polygon_t    contour             = {tb_null, contour_counts, tb_true};
        while ((contour_counts[0] = *counts++))
        {
            // init the polygon for this contour
            contour.points = points + index;

            // done tessellator for the convex contour, will be faster
            gb_tessellator_done_convex(impl, &contour, bounds);

            // update the contour index
            index += contour_counts[0];
        }
    }
    else
    {
        // done tessellator for the concave polygon
        gb_tessellator_done_concave(impl, polygon, bounds);
    }
}
