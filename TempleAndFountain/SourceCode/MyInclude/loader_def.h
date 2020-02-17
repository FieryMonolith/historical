/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __LOADER_DEF_H__
#define __LOADER_DEF_H__

// These are the states used by the ModelLoader

extern const unsigned int LIMBO_STATE;
extern const unsigned int TYPE_STATE;
extern const unsigned int VERTEX_STATE;
extern const unsigned int NORMAL_STATE;
extern const unsigned int POINT_STATE;
extern const unsigned int NAME_STATE;
extern const unsigned int MODEL_FILE1_STATE;
extern const unsigned int MODEL_FILE2_STATE;
extern const unsigned int MODEL_FILE3_STATE;
extern const unsigned int MODEL_FILE4_STATE;
extern const unsigned int POLYGONMESH_TYPE_STATE;
extern const unsigned int TEX_COORD_STATE;
extern const unsigned int LEX_ERROR;

extern const unsigned int COMPOSITE_TYPE_GATHERED;
extern const unsigned int MESH_TYPE_GATHERED;
extern const unsigned int INT_GATHERED;
extern const unsigned int DOUBLE_GATHERED;
extern const unsigned int NAME_GATHERED;
extern const unsigned int FILE_NAME_GATHERED;

#endif
