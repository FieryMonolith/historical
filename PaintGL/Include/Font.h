#ifndef __Font_h__
#define __Font_h__

#include <windows.h>
#include <GL/gl.h>
#include <string>
#include <vector>
#include <stdexcept>

#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/ftglyph.h"

class RGBA;

class Font
{
public:
  Font(const std::string &filename, const unsigned int pixelHeight);
  ~Font(void);

  // The Flagship Function Of The Library - This Thing Will Print
  // Out Text At Window Coordinates X, Y, Using The Font ft_font.
  // The Current Modelview Matrix Will Also Be Applied To The Text. 
  void print(float x, float y, const char *fmt, ...);
  void make_dlist(FT_Face &face, unsigned char ch, GLuint list_base, GLuint * tex_base);
  inline int next_p2 (int a )
  {
	  int rval=2;
	  // rval<<=1 Is A Prettier Way Of Writing rval*=2; 
	  while(rval<a) rval<<=1;
	  return rval;
  }
  inline void pushScreenCoordinateMatrix() { return; }
  inline void pop_projection_matrix() { return; }

private:
 	float h;           // Holds The Height Of The Font.
	GLuint * textures; // Holds The Texture Id's 
	GLuint list_base;  // Holds The First Display List Id

private:
  Font(const Font &);
  Font &operator=(const Font &);
}; // class Font

#endif // __Font_h__
