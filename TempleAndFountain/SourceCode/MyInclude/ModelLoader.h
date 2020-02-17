/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __MODELLOADER_H__
#define __MODELLOADER_H__

#include <string>
#include <map>

class Model;
class Matrix4x4d;
class yyFlexLexer;
class CTexture;

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
class ModelLoader {
public:
  ModelLoader(void);
  ~ModelLoader(void);

  // Models can be transformed as part of the loading process so begin with
  //   the identity matrix.
  Model *Load(const char *filename);

  // This allows for the user to specify the starting transformation matrix
  //   for the given model.  As models components are loaded they are
  //   transformed and will also modify this transformation matrix.
  Model *Load(const char *filename, const Matrix4x4d &matrix);

  // Load texture
  unsigned int LoadTexture(const char *filename);

private:
  std::map<std::string, Model *> d_models;
  std::map<std::string, CTexture *> d_textures;

  // Handle the different types of markers in the model file
  bool HandleTypeState(yyFlexLexer &yyfl, unsigned int &modelType);
  bool HandleNameState(yyFlexLexer &yyfl, std::string &name);
  bool HandleVertexState(yyFlexLexer &yyfl, Model &m);
  bool HandleNormalState(yyFlexLexer &yyfl, Model &m);
  bool HandleTexCoordState(yyFlexLexer &yyfl, Model &m);
  bool HandlePointState(yyFlexLexer &yyfl, Model &m);
  bool HandleModelFileState(int state, yyFlexLexer &yyfl, Model &m, const Matrix4x4d &matrix);
  bool HandlePolygonMeshTypeState(yyFlexLexer &yyfl, int &polygonmeshType);

  ModelLoader(const ModelLoader &);
  ModelLoader &operator=(const ModelLoader &);
};

#endif
