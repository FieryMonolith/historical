/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#include <cassert>
#include <fstream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "Texture.h"
#include "Vector3d.h"
#include "Matrix.h"

#include "Model.h"

using namespace std;

const unsigned int MODEL_MESH = 26;
const unsigned int MODEL_COMPOSITE = 27;

VertexInfo::VertexInfo(void)
: d_id(0),
  d_location(0),
  d_normal(0),
  d_textureCoordinate(0)
{
	return;
}

VertexInfo::VertexInfo(const VertexInfo &vi) {
	Copy(vi);

	return;
}

VertexInfo::~VertexInfo(void) {
	d_location=0;
	d_normal=0;
  d_textureCoordinate=0;

	return;
}

VertexInfo &VertexInfo::operator=(const VertexInfo &vi) {
	Copy(vi);

	return *this;
}

void VertexInfo::Copy(const VertexInfo &vi) {
	d_location = vi.d_location;
	d_normal = vi.d_normal;
  d_textureCoordinate = vi.d_textureCoordinate;
	d_id = vi.d_id;

	return;
}

Model::Model(const string &name, unsigned int type)
: d_type(type),
  d_polymeshType(GL_TRIANGLE_STRIP),
  d_hasTexture(false),
  d_textureName(0)
{
	d_name = name;

	return;
}

Model::Model(const string &name, unsigned int type, unsigned int polymeshType)
: d_type(type),
  d_polymeshType(polymeshType),
  d_hasTexture(false),
  d_textureName(0)
{
	d_name = name;

	return;
}

// Deletes its own data and its children
Model::~Model(void) {
	map<unsigned int, Vector3d *>::iterator i;
	for(i = d_vertices.begin(); i != d_vertices.end(); ++i) {
		if(i->second != 0) {
			delete i->second;
			i->second = 0;
		}
	}

	for(i = d_normals.begin(); i != d_normals.end(); ++i) {
		if(i->second != 0) {
			delete i->second;
			i->second = 0;
		}
	}

	vector<VertexInfo>::iterator ii;
	for(ii = d_points.begin(); ii != d_points.end(); ++ii) {
		ii->d_location = 0;
		ii->d_normal = 0;
	}

	vector<Model *>::iterator iii;
	for(iii = d_models.begin(); iii != d_models.end(); ++iii) {
	  if(*iii != 0) {
	    delete *iii;
	    *iii = 0;
	  }
	}

	return;
}

// Adds texture information at the current level
void Model::AddTexture(unsigned int name) {
  if(name == 0) { return; }

  d_textureName = name;
  d_hasTexture = true;

  return;
}

// Adds a vertex to the current model level
void Model::AddVertex(unsigned int id, Vector3d *vertex) {
	if(vertex == 0) { return; }

	d_vertices.insert(make_pair(id, vertex));

	return;
}

// Adds a point(index to a vertex) to the current model level
void Model::AddPoint(unsigned int id) {
	VertexInfo vi;
	vi.d_id = id;

  map<unsigned int, Vector3d *>::iterator i = d_vertices.find(id);
	if(i == d_vertices.end()) { return; }
	vi.d_location = i->second;

  i = d_normals.find(id);
	if(i != d_normals.end()) { vi.d_normal = i->second; }

  i = d_textureCoordinates.find(id);
  if(i != d_textureCoordinates.end()) {
    if(i->second != 0) {
      vi.d_textureCoordinate = i->second;
    }
  }

	d_points.push_back(vi);

	return;
}

// Adds a vertex normal to the specified vertex for the current model level
void Model::AddNormal(unsigned int id, Vector3d *normal) {
	if(normal == 0) { return; }

	d_normals.insert(make_pair(id, normal));

	return;
}

// Adds a texture coordinate to the specified vertex for the current model
//   level
void Model::AddTextureCoordinate(unsigned int id, Vector3d *tc) {
	if(tc == 0) { return; }

	d_textureCoordinates.insert(make_pair(id, tc));

	return;
}

// Add a child model at the current level
void Model::AddModel(Model *m) {
  if(m == 0) { return; }

  d_models.push_back(m);

  return;
}

// Draw the current model by calling the appropriate draw method
void Model::Draw(unsigned int textureName) const {
  if(d_type == MODEL_MESH) {
    DrawMesh(textureName);
  } else if(d_type == MODEL_COMPOSITE) {
    DrawModels(textureName);
  }

  return;
}

// Draw the mesh for the current model level
void Model::DrawMesh(unsigned int textureName) const {
  bool hasTexture = false;
  unsigned int drawTexture = 0;

  if(d_hasTexture) {
    hasTexture = true;
    drawTexture = d_textureName;
  } else if(textureName > 0) {
    hasTexture = true;
    drawTexture = textureName;
  }

  if(hasTexture) {
    glBindTexture(GL_TEXTURE_2D, drawTexture);
  }

	vector<VertexInfo>::const_iterator i;
	glBegin(d_polymeshType);
	for(i = d_points.begin(); i != d_points.end(); ++i) {
    const VertexInfo &vi = *i;

    // NORMAL
    if(i->d_normal != 0) {
			glNormal3dv(vi.d_normal->Array());
		}

#if 0
    if(d_name == "plane-72-144") {
      Vector3d *t = vi.d_textureCoordinate;
    }
#endif
    // TEXTURE
    if(hasTexture && i->d_textureCoordinate != 0) {
      glTexCoord2dv(vi.d_textureCoordinate->Array());
    }

    // VERTEX
    if(i->d_location != 0) {
		  glVertex3dv(vi.d_location->Array());
		}
	}
	glEnd();

  return;
}

// The current model is a composite so draw children
void Model::DrawModels(unsigned int textureName) const {
  unsigned int drawTexture = textureName;
  if(d_hasTexture) { drawTexture = d_textureName; }

  vector<Model *>::const_iterator i;
  for(i = d_models.begin(); i != d_models.end(); ++i) {
    if(*i != 0) {
      (*i)->Draw(drawTexture);
    }
  }

  return;
}

// Output model to file
void Model::Output(const string &filename) const {
  ofstream output;
  output.open(filename.c_str());
  if(!output) {
    return;
  }

  output << "TYPE MESH\n";
  output << "POLYGON_MESH_TYPE " << d_polymeshType << "\n";
  output << "NAME " << d_name << "\n";

  map<unsigned int, Vector3d *>::const_iterator iv;
  for(iv = d_vertices.begin(); iv != d_vertices.end(); ++iv) {
    if(iv->second == 0) {
      output.close();
      return;
    }

    output << "VERTEX " << iv->first << " " << (*(iv->second))[0] << " ";
    output << (*(iv->second))[1] << " " << (*(iv->second))[2] << "\n";
  }

  for(iv = d_normals.begin(); iv != d_normals.end(); ++iv) {
    if(iv->second == 0) {
      output.close();
      return;
    }

    output << "NORMAL " << iv->first << " " << (*(iv->second))[0] << " ";
    output << (*(iv->second))[1] << " " << (*(iv->second))[2] << "\n";
  }

  for(iv = d_textureCoordinates.begin(); iv != d_textureCoordinates.end(); ++iv) {
    if(iv->second == 0) {
      output.close();
      return;
    }

    output << "TEX_COORD " << iv->first << " " << (*(iv->second))[0] << " ";
    output << (*(iv->second))[1] << "\n";
  }

  vector<VertexInfo>::const_iterator ip;
  for(ip = d_points.begin(); ip != d_points.end(); ++ip) {
    output << "POINT " << ip->d_id << "\n";
  }

  output.close();

  return;
}

Model *Model::Copy(void) const {
  Model *m = new Model(d_name, d_type, d_polymeshType);
  if(m == 0) { return 0; }
  m->d_hasTexture = d_hasTexture;
  m->d_textureName = d_textureName;

  if(d_type == MODEL_COMPOSITE) {
    if(!CopyModels(*m)) {
      delete m;
      return 0;
    }
  } else if(d_type == MODEL_MESH) {
    if(!CopyMesh(*m)) {
      delete m;
      return 0;
    }
  } else {
    return 0;
  }

  return m;
}

bool Model::CopyMesh(Model &m) const {
  map<unsigned int, Vector3d *>::const_iterator iv;
  for(iv = d_vertices.begin(); iv != d_vertices.end(); ++iv) {
    if(iv->second == 0) { return false; }

    Vector3d *v = new Vector3d(*(iv->second));
    if(v == 0) { return false; }

    m.AddVertex(iv->first, v);
  }

  map<unsigned int, Vector3d *>::const_iterator in;
  for(in = d_normals.begin(); in != d_normals.end(); ++in) {
    if(in->second == 0) { return false; }

    Vector3d *v = new Vector3d(*(in->second));
    if(v == 0) { return false; }

    m.AddNormal(in->first, v);
  }

  map<unsigned int, Vector3d *>::const_iterator it;
  for(it = d_textureCoordinates.begin(); it != d_textureCoordinates.end(); ++it) {
    if(it->second == 0) { return false; }

    Vector3d *v = new Vector3d(*(it->second));
    if(v == 0) { return false; }

    m.AddTextureCoordinate(it->first, v);
  }

  vector<VertexInfo>::const_iterator ip;
  for(ip = d_points.begin(); ip != d_points.end(); ++ip) {
    m.AddPoint(ip->d_id);
  }

  return true;
}

bool Model::CopyModels(Model &m) const {
  vector<Model *>::const_iterator i;
  for(i = d_models.begin(); i != d_models.end(); ++i) {
    if(*i == 0) { return false; }

    Model *tempm = (*i)->Copy();
    if(tempm == 0) { return false; }

    m.AddModel(tempm);
  }

  return true;
}

// Transform the model by the given matrix
void Model::Transform(const Matrix4x4d &matrix) {
  if(d_type == MODEL_COMPOSITE) {
    TransformModel(matrix);
  } else if(d_type == MODEL_MESH) {
    TransformMesh(matrix);
  }

  return;
}

void Model::TransformModel(const Matrix4x4d &matrix) {
  vector<Model *>::iterator i;
  for(i = d_models.begin(); i != d_models.end(); ++i) {
    if(*i == 0) { return; }

    (*i)->Transform(matrix);
  }

  return;
}

void Model::TransformMesh(const Matrix4x4d &matrix) {
  map<unsigned int, Vector3d *>::iterator i;
  for(i = d_vertices.begin(); i != d_vertices.end(); ++i) {
    if(i->second == 0) { continue; }
    Vector3d &t = *(i->second);

    Matrix4x4d current(1, 0, 0, t[0],
	  				           0, 1, 0, t[1],
		  			           0, 0, 1, t[2],
			  		           0, 0, 0, 1);
    Matrix4x4d newMatrix = matrix * current;
    t[0] = newMatrix.At(0,3);
    t[1] = newMatrix.At(1,3);
    t[2] = newMatrix.At(2,3);
  }

  Matrix4x4d normalMatrix(matrix);
  normalMatrix.At(0,3) = 0.0;
  normalMatrix.At(1,3) = 0.0;
  normalMatrix.At(2,3) = 0.0;

  for(i = d_normals.begin(); i != d_normals.end(); ++i) {
    if(i->second == 0) { continue; }
    Vector3d &t = *(i->second);

    Matrix4x4d current(1, 0, 0, t[0],
                       0, 1, 0, t[1],
			  		           0, 0, 1, t[2],
				  	           0, 0, 0, 1);

    Matrix4x4d newMatrix = normalMatrix * current;
    t[0] = newMatrix.At(0,3);
    t[1] = newMatrix.At(1,3);
    t[2] = newMatrix.At(2,3);
  }

  return;
}
