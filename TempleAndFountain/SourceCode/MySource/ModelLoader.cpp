/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * ModelLoader - The ModelLoader class is a state machine driven method
 *   of reading in and creating models from file.  The class simple takes
 *   the name of the model file to load and does all the work from there.
 *   The same goes for loading textures.  The class also acts like a
 *   factory and stores and deletes the models and textures that are
 *   loaded.  The class also relies on the lex to do the file parsing and
 *   passing of the appropriate data to this class.
 
 * See model_loader_scanner.lex for the lex information.

 */

#include "stdafx.h"
#include <fstream>
#include <sstream>

#include "Vector3d.h"
#include "ModelLoader.h"

#include "Matrix.h"
#include "Model.h"
#include "loader_def.h"
#include "FlexLexer.h"
#include "Texture.h"

using namespace std;

// Unique value of the different states used to load a model that relate
//   to the values returned by the lexer.
const unsigned int END_OF_INPUT           = 0;
const unsigned int LIMBO_STATE            = 5000;
const unsigned int TYPE_STATE             = 5001;
const unsigned int VERTEX_STATE           = 5002;
const unsigned int NORMAL_STATE           = 5003;
const unsigned int POINT_STATE            = 5004;
const unsigned int NAME_STATE             = 5007;
const unsigned int MODEL_FILE1_STATE      = 5008;
const unsigned int MODEL_FILE2_STATE      = 5009;
const unsigned int MODEL_FILE3_STATE      = 5010;
const unsigned int MODEL_FILE4_STATE      = 5011;
const unsigned int POLYGONMESH_TYPE_STATE = 5012;
const unsigned int TEX_COORD_STATE        = 5013;
const unsigned int LEX_ERROR              = 5100;

const unsigned int COMPOSITE_TYPE_GATHERED = 5050;
const unsigned int MESH_TYPE_GATHERED      = 5051;
const unsigned int INT_GATHERED            = 5052;
const unsigned int DOUBLE_GATHERED         = 5053;
const unsigned int NAME_GATHERED           = 5054;
const unsigned int FILE_NAME_GATHERED      = 5055;

ModelLoader::ModelLoader(void) {
  return;
}

// Clean up by deleting all the models and textures that were loaded
ModelLoader::~ModelLoader(void) {
  map<string, Model *>::iterator i;
  for(i = d_models.begin(); i != d_models.end(); ++i) {
    if(i->second != 0) {
      delete i->second;
      i->second = 0;
    }
  }

  map<string, CTexture *>::iterator ii;
  for(ii = d_textures.begin(); ii != d_textures.end(); ++ii) {
    if(ii->second != 0) {
      delete ii->second;
      ii->second = 0;
    }
  }

  return;
}

// Models can be transformed as part of the loading process so begin with
//   the identity matrix.
Model *ModelLoader::Load(const char *filename) {
	Matrix4x4d identity(1, 0, 0, 0,
						          0, 1, 0, 0,
						          0, 0, 1, 0,
						          0, 0, 0, 1);
	return Load(filename, identity);
}

// This allows for the user to specify the starting transformation matrix
//   for the given model.  As models components are loaded they are
//   transformed and will also modify this transformation matrix.
Model *ModelLoader::Load(const char *filename, const Matrix4x4d &matrix) {
  // Open input file
  ifstream in;
  in.open(filename);
  if(!in) { return 0; }

  // Pass input stream to lex
  yyFlexLexer yyfl(&in);

  Model *model = 0;
  unsigned int modelType = 0;
  int polygonmeshType = -1;
  string name;

  // Begin processing lexer output
  int currentState = LIMBO_STATE;
  int state = yyfl.yylex();
  while(state != END_OF_INPUT) {
    switch(state) {
    case END_OF_INPUT:
      // done with file
      break;
    case TYPE_STATE:
      if(currentState != LIMBO_STATE || !HandleTypeState(yyfl, modelType)) {
      	if(model != 0) { delete model; }
	      in.close();
	      return 0;
      }
      break;
    case VERTEX_STATE:
      if(modelType == COMPOSITE_TYPE_GATHERED || 
         (currentState != NAME_STATE && currentState != NORMAL_STATE && currentState != VERTEX_STATE &&
          currentState != TEX_COORD_STATE) || !HandleVertexState(yyfl, *model)) {
	      if(model != 0) { delete model; }
	      in.close();
	      return 0;
      }
      break;
    case NORMAL_STATE:
      if(modelType == COMPOSITE_TYPE_GATHERED || 
      	 (currentState != NAME_STATE && currentState != VERTEX_STATE && currentState != NORMAL_STATE &&
          currentState != TEX_COORD_STATE) || !HandleNormalState(yyfl, *model)) {
	      if(model != 0) { delete model; }
	      in.close();
	      return 0;
      }
      break;
    case TEX_COORD_STATE:
      if(modelType == COMPOSITE_TYPE_GATHERED || 
      	 (currentState != NAME_STATE && currentState != VERTEX_STATE && currentState != NORMAL_STATE &&
          currentState != TEX_COORD_STATE) || !HandleTexCoordState(yyfl, *model)) {
	      if(model != 0) { delete model; }
	      in.close();
	      return 0;
      }
      break;
    case POINT_STATE:
      if(modelType == COMPOSITE_TYPE_GATHERED || 
	       (currentState != VERTEX_STATE && currentState != NORMAL_STATE && currentState != TEX_COORD_STATE && 
          currentState != POINT_STATE) || !HandlePointState(yyfl, *model)) {
	      if(model != 0) { delete model; }
	      in.close();
	      return 0;
      }
      break;
    case POLYGONMESH_TYPE_STATE:
      if(currentState != TYPE_STATE || !HandlePolygonMeshTypeState(yyfl, polygonmeshType)) {
        if(model != 0) { delete model; }
        in.close();
        return 0;
      }
      break;
    case NAME_STATE:
      if((currentState != TYPE_STATE && currentState != POLYGONMESH_TYPE_STATE) || 
         !HandleNameState(yyfl, name)) {
	      if(model != 0) { delete model; }
	      in.close();
	      return 0;
      }

      if(polygonmeshType < 0) {
        model = new Model(name, modelType);
      } else {
        model = new Model(name, modelType, polygonmeshType);
      }
      if(model == 0) {
	      in.close();
	      return 0;
      }
      break;
    case MODEL_FILE1_STATE:
    case MODEL_FILE2_STATE:
    case MODEL_FILE3_STATE:
    case MODEL_FILE4_STATE:
      if(modelType == MESH_TYPE_GATHERED || 
      	 (currentState != NAME_STATE && currentState != MODEL_FILE1_STATE &&
	        currentState != MODEL_FILE2_STATE && currentState != MODEL_FILE3_STATE &&
	        currentState != MODEL_FILE4_STATE) ||
	       !HandleModelFileState(state, yyfl, *model, matrix)) {
	      if(model != 0) { delete model; }
	      in.close();
	      return 0;
      }
      break;
    default:
      // error
      if(model != 0) { delete model; }
    	in.close();
      return 0;
    };

    currentState = state;
    state = yyfl.yylex();
  }

  in.close();

  return model;
}

// Load a texture
unsigned int ModelLoader::LoadTexture(const char *filename) {
  if(filename == 0 || filename[0] == '\0') { return 0; }
  string textureFileName(filename);
  unsigned int textureName = 0;

  // Look to see if the texture is already loaded
  map<string, CTexture *>::iterator itexture = d_textures.find(textureFileName);
  if(itexture == d_textures.end()) {
    // Texture doesn't exist yet, so load it
    CTexture *texture = new CTexture;
    if(texture == 0) { return false; }

    if(!texture->LoadFile(filename)) { delete texture; return false; }
    textureName = texture->TexName();
  } else {
    // Texture exists, return texture id
    if(itexture->second == 0) { return false; }
    textureName = itexture->second->TexName();
  }

  return textureName;
}

/*** BEGIN Methods to handle the different lex states ***********************/
bool ModelLoader::HandleTypeState(yyFlexLexer &yyfl, unsigned int &modelType) {
  int value = yyfl.yylex();
  if(value != COMPOSITE_TYPE_GATHERED && value != MESH_TYPE_GATHERED) {
    return false;
  }

  if(value == COMPOSITE_TYPE_GATHERED) {
    modelType = MODEL_COMPOSITE;
  } else {
    modelType = MODEL_MESH;
  }

  return true;
}

bool ModelLoader::HandleNameState(yyFlexLexer &yyfl, std::string &name) {
  int value = yyfl.yylex();
  if(value != NAME_GATHERED) {
    return false;
  }

  name = yyfl.YYText();
  return true;
}

bool ModelLoader::HandleVertexState(yyFlexLexer &yyfl, Model &m) {
  double x=0.0, y=0.0, z=0.0;
  unsigned int id=0;

  int value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream sid;
  sid << yyfl.YYText();
  sid >> id;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s1;
  s1 << yyfl.YYText();
  s1 >> x;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s2;
  s2 << yyfl.YYText();
  s2 >> y;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s3;
  s3 << yyfl.YYText();
  s3 >> z;

  Vector3d *vertex = new Vector3d(x, y, z);
  if(vertex == 0) {
	  return false;
  }

  m.AddVertex(id, vertex);

  return true;
}

bool ModelLoader::HandleNormalState(yyFlexLexer &yyfl, Model &m) {
  double x=0.0, y=0.0, z=0.0;
  unsigned int id=0;

  int value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream sid;
  sid << yyfl.YYText();
  sid >> id;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s1;
  s1 << yyfl.YYText();
  s1 >> x;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s2;
  s2 << yyfl.YYText();
  s2 >> y;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s3;
  s3 << yyfl.YYText();
  s3 >> z;


  Vector3d *normal = new Vector3d(x, y, z);
  if(!normal) { return false; }

  m.AddNormal(id, normal);

  return true;
}

bool ModelLoader::HandleTexCoordState(yyFlexLexer &yyfl, Model &m) {
  double x=0.0, y=0.0;
  unsigned int id=0;

  int value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream sid;
  sid << yyfl.YYText();
  sid >> id;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s1;
  s1 << yyfl.YYText();
  s1 >> x;

  value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream s2;
  s2 << yyfl.YYText();
  s2 >> y;

  Vector3d *tc = new Vector3d(x, y, 0.0);
  if(!tc) { return false; }

  m.AddTextureCoordinate(id, tc);

  return true;
}

bool ModelLoader::HandlePointState(yyFlexLexer &yyfl, Model &m) {
  unsigned int id=0;

  int value = yyfl.yylex();
  if(value != DOUBLE_GATHERED && value != INT_GATHERED) {
    return false;
  }
  stringstream sid;
  sid << yyfl.YYText();
  sid >> id;

  m.AddPoint(id);

  return true;
}

bool ModelLoader::HandleModelFileState(int state, yyFlexLexer &yyfl, Model &m, 
                                       const Matrix4x4d &matrix){
  stringstream data;
  data << yyfl.YYText();

  string lexeme;
  string name;
  data >> ws;
  data >> lexeme;
  data >> ws;
  data >> name;
  data >> ws;

  // Check for texture name
  bool hasTexture = false;
  char c = data.peek();
  if((c >= 'a' && c <= 'z') ||
     (c >= 'A' && c <= 'Z') ||
     c == '/') {
    hasTexture = true;
  } else if(c == '.') {
    data >> c;
    c = data.peek();
    data.unget();
    data.clear();

    if(c < '0' || c > '9') {
      hasTexture = true;
    }
  }

  unsigned int textureName = 0;
  if(hasTexture) {
    string textureFileName;
    data >> textureFileName;
    map<string, CTexture *>::iterator itexture = d_textures.find(textureFileName);
    if(itexture == d_textures.end()) {
      CTexture *texture = new CTexture;
      if(texture == 0) { return false; }

      if(!texture->LoadFile(textureFileName.c_str())) { delete texture; return false; }
      textureName = texture->TexName();
    } else {
      if(itexture->second == 0) { return false; }
      textureName = itexture->second->TexName();
    }
  }

  double x=0.0, y=0.0, z=0.0;
  double outx=0.0, outy=0.0, outz=1.0;
  double upx=0.0, upy=1.0, upz=0.0;
  if(state == MODEL_FILE2_STATE || state == MODEL_FILE4_STATE) {
	  data >> ws;	data >> x;
	  data >> ws;	data >> y;
	  data >> ws; data >> z;
  }
  
  if(state == MODEL_FILE3_STATE || state == MODEL_FILE4_STATE) {
	  data >> ws;	data >> outx;
	  data >> ws;	data >> outy;
	  data >> ws; data >> outz;

	  data >> ws;	data >> upx;
	  data >> ws;	data >> upy;
	  data >> ws; data >> upz;
  }

  Vector3d location(x, y, z);
  Vector3d out(outx, outy, outz);
  Vector3d up(upx, upy, upz);

  Normalize(out);
  Normalize(up);
  Vector3d right = Cross(up, out);
  Normalize(right);

  Matrix4x4d rt(right[0], right[1], right[2], location[0],
				        up[0],    up[1],    up[2],    location[1],
				        out[0],   out[1],   out[2],   location[2],
				        0,        0,        0,        1);
  Matrix4x4d newMatrix = matrix * rt;

  Model *newModel;
  map<string, Model *>::iterator storedModel = d_models.find(name);
  if(storedModel == d_models.end()) {
    newModel = ModelLoader::Load(name.c_str(), newMatrix);
    if(newModel == 0) { return false; }

    if(newModel->Type() == MODEL_MESH) {
      d_models.insert(make_pair(name, newModel));
      newModel = newModel->Copy();
      if(newModel == 0) { return false; }
      newModel->Transform(newMatrix);
    }
  } else {
    newModel = storedModel->second->Copy();
    if(newModel == 0) { return false; }
    if(newModel->Type() == MODEL_MESH) {
      newModel->Transform(newMatrix);
    }
  }

  if(hasTexture) {
    newModel->AddTexture(textureName);
  }

  m.AddModel(newModel);

  return true;
}

bool ModelLoader::HandlePolygonMeshTypeState(yyFlexLexer &yyfl, int &polygonmeshType) {
  int value = yyfl.yylex();
  if(value != INT_GATHERED && value != DOUBLE_GATHERED) {
    return false;
  }

  stringstream in;
  in << yyfl.YYText();
  in >> value;
  if(value < 0) {
    return false;
  }

  polygonmeshType = value;

  return true;
}
/*** END Methods to handle the different lex states *************************/
