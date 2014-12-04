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
 * @file        mesh.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_IMPL_MESH_H
#define GB_CORE_IMPL_MESH_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mesh_face.h"
#include "mesh_edge.h"
#include "mesh_vertex.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/* the mesh ref type 
 *
 * using the quad-edge data structure
 */
typedef struct{}*           gb_mesh_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* init the mesh 
 *
 * @param edge_func         the edge func
 * @param face_func         the face func
 * @param vertex_func       the vertex func
 *
 * @return                  the mesh
 */
gb_mesh_ref_t               gb_mesh_init(tb_item_func_t edge_func, tb_item_func_t face_func, tb_item_func_t vertex_func);

/* exit the mesh 
 *
 * @param list              the mesh
 */
tb_void_t                   gb_mesh_exit(gb_mesh_ref_t mesh);

/* clear the mesh 
 *
 * @param list              the mesh
 */
tb_void_t                   gb_mesh_clear(gb_mesh_ref_t mesh);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


