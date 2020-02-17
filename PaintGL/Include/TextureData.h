#ifndef __TextureData_h__
#define __TextureData_h__

// TEMPORARY ******************
#include <windows.h>
#include <GL/gl.h>

class RGBA;

class TextureData
{
public:
  /*** Begin Constructor/Destructor ******************************************/
  TextureData(void) throw()
  : d_glTextureName(0),
    d_width(0),
    d_height(0),
    d_widthRatio(0),
    d_heightRatio(0),
    d_buffer(0)
  {
    return;
  }

  ~TextureData(void) throw()
  {
    if(d_buffer != 0) { delete [] d_buffer; d_buffer = 0; }
    if(d_glTextureName != 0) { glDeleteTextures(1, &d_glTextureName); }
    return;
  }
  /*** End Constructor/Destructor ********************************************/


  /*** Begin Public Data *****************************************************/
  // id that identifies texture within opengl
  unsigned int d_glTextureName;

  // Actual width and height of the texture in pixels
  unsigned int d_width, d_height;

  // used to sample from a power of 2 texture
  double d_widthRatio, d_heightRatio;

  // Array that stores the pixel color information
  RGBA* d_buffer;
  /*** End Public Data *******************************************************/

}; // class TextureData

#endif // __TextureData_h__
