/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <vector>
#include <map>

class Vector3d;
class Matrix4x4d;

// Global constants
extern const unsigned int MODEL_MESH;
extern const unsigned int MODEL_COMPOSITE;

/*

 * VertexInfo - VertexInfo holds the pertinent opengl data for each vertex.
 *   This information is an id, its location, texture coordinate, and
 *   the vertex normal.

 */
class VertexInfo {
public:
	VertexInfo(void);
	VertexInfo(const VertexInfo &);
	~VertexInfo(void);

	VertexInfo &operator=(const VertexInfo &);

	unsigned int d_id;
	Vector3d *d_location;
	Vector3d *d_normal;
  Vector3d *d_textureCoordinate;

private:
	void Copy(const VertexInfo &);
};

/*

 * Model - Model is a recursive/composite structure that can have model and
 *   texture information at each level but also contain children models.
 *   When drawn, the top level model proceeds to draw all of its children
 *   and itself.

 */
class Model {
public:
  // Create an empty model of a specified type and name
	Model(const std::string &name, unsigned int type);
  // Create an empty model of a specified type, name, and mesh type
	Model(const std::string &name, unsigned int type, unsigned int polymeshType);
	~Model(void);

	Model *Copy(void) const;
	void Draw(unsigned int textureName=0) const;
  void Output(const std::string &) const; // Output to file
  void Transform(const Matrix4x4d &); // Will also transform children

  // The following functions apply to this level of the model only
  void AddVertex(unsigned int id, Vector3d *vertex);
	void AddPoint(unsigned int id);
	void AddNormal(unsigned int id, Vector3d *normal);
	void AddTextureCoordinate(unsigned int id, Vector3d *tc);
	void AddModel(Model *);
  void AddTexture(unsigned int);

  // Accessors
  std::map<unsigned int, Vector3d *> &VertexList(void) { return d_vertices; }
	const std::vector<VertexInfo> &Vertices(void) const {return d_points;}
	const std::string &Name(void) const { return d_name; }
	unsigned int Type(void) const { return d_type; }
  unsigned int PolygonMeshType(void) const { return d_polymeshType; }

private:
	std::string d_name;
	unsigned int d_type; // Mesh or composite
  unsigned int d_polymeshType; // Type of mesh
	std::map<unsigned int, Vector3d *> d_vertices;
	std::map<unsigned int, Vector3d *> d_normals;
  std::map<unsigned int, Vector3d *> d_textureCoordinates;
	std::vector<VertexInfo> d_points;
	std::vector<Model *> d_models; // Children
  bool d_hasTexture;
  unsigned int d_textureName;

	Model(const Model &);
	Model &operator=(const Model &);

  // Helper functions for mesh versus composite model types
	bool CopyMesh(Model &) const;
	bool CopyModels(Model &) const;
	void DrawMesh(unsigned int) const;
	void DrawModels(unsigned int) const;
  void TransformMesh(const Matrix4x4d &);
  void TransformModel(const Matrix4x4d &);
};

#endif
