#include <windows.h>
#include <GL/gl.h>
#include <memory>
#include <sstream>

#include "Pixel.h"
#include "TextureData.h"
#include "Texture.h"

#include "TextureLibrary.h"

#include "EasyBMP.h"

using namespace EasyBMP;
using namespace std;

TextureLibrary gTextureLibrary;

TextureLibrary::TextureLibrary(void)
: d_bufferSize(0), d_buffer(0), d_nonameId(1)
{
  return;
}

TextureLibrary::~TextureLibrary(void)
{
  set<TextureData*>::iterator i = d_textureData.end();
  for(i=d_textureData.begin(); i!=d_textureData.end(); ++i)
  {
    if(*i != 0)
    {
      delete *i;
      *i = 0;
    }
  }

  d_textureData.clear();
  d_filenameMap.clear();
  d_glTextureNameMap.clear();

  return;
}

void TextureLibrary::ClearTexture(TextureData &data,
                                  const RGBA &fillColor) const
{
  unsigned int total = data.d_width * data.d_height;
  for(unsigned int i=0; i<total; ++i)
  {
    data.d_buffer[i] = fillColor;
  }

  // Update the opengl texture
  glBindTexture(GL_TEXTURE_2D, data.d_glTextureName);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                  data.d_width, data.d_height,
                  GL_RGBA, GL_UNSIGNED_BYTE, data.d_buffer);

  if(glGetError() != GL_NO_ERROR) { throw 1; }

  return;
}

TextureData *TextureLibrary::CopyTexture(TextureData &data)
{
  // Increase how many textures are using this data.
  ++data.d_count;
  return &data;
}

void TextureLibrary::DeleteTexture(TextureData &data)
{
  // Look up texture data pointer to make sure it is still valid
  set<TextureData*>::iterator it = d_textureData.find(&data);

  // No texture data was found, ignoring the rest of the function.
  if(it == d_textureData.end()) { return; }

  // Reduce how many textures are using this data.
  --data.d_count;
  if(data.d_count > 0) { return; }

  // Remove texture from opengl
  glDeleteTextures(1, &data.d_glTextureName);

  // Cleanup library bookkeeping
  d_textureData.erase(&data);
  d_filenameMap.erase(data.d_name);
  d_glTextureNameMap.erase(data.d_glTextureName);

  delete &data;

  return;
}

TextureData *TextureLibrary::DifferentiateTexture(TextureData &data)
{
  // Create a brand new texture by using CreateTexture for the template
  auto_ptr<TextureData> newData;
  try
  {
    newData.reset(CreateTexture(RGBA(), data.d_width, data.d_height));
    if(newData.get() == 0) { throw 0; }
  }
  catch(...) { throw 1; }

  // Copy the image into the new texture data.
  memcpy(newData.get()->d_buffer,
         data.d_buffer,
         data.d_width*data.d_height*sizeof(RGBA));

  // Update the opengl texture
  glBindTexture(GL_TEXTURE_2D, newData.get()->d_glTextureName);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                  newData.get()->d_width, newData.get()->d_height,
                  GL_RGBA, GL_UNSIGNED_BYTE, newData.get()->d_buffer);

  if(glGetError() != GL_NO_ERROR) { throw 2; }

  return newData.release();
}

void TextureLibrary::UpdateTexture(TextureData &data)
{
  unsigned int startX = data.d_minX;
  unsigned int startY = data.d_minY;
  unsigned int width  = data.d_maxX - data.d_minX + 1;
  unsigned int height = data.d_maxY - data.d_minY + 1;

  // Get a temporary buffer that will hold the changed data.  It needs to
  //   be the exact size of data written to the opengl texture.
  RGBA *textureBuffer = 0; // Do not delete, it is managed by library
  try { textureBuffer = GetBuffer(width, height); }
  catch(...) { throw 1; }

  // Fill in temporary buffer with data area that has changed
  RGBA *pBuffer = textureBuffer;
  for(unsigned int y=startY; y<startY+height; ++y)
    for(unsigned int x=startX; x<startX+width; ++x)
    {
      *pBuffer = data.d_buffer[y * data.d_width + x];
      ++pBuffer;
    }

  // Update the opengl texture
  glBindTexture(GL_TEXTURE_2D, data.d_glTextureName);
  glTexSubImage2D(GL_TEXTURE_2D, 0, startX, startY,
                  width, height,
                  GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

  if(glGetError() != GL_NO_ERROR) { throw 2; }

  return;
}

RGBA *TextureLibrary::GetBuffer(const unsigned int width,
                                const unsigned int height)
{
  unsigned int total = width * height;
  if(total > d_bufferSize)
  {
    if(d_buffer != 0) { delete [] d_buffer; }
    try
    {
      d_buffer = new RGBA[total];
      if(d_buffer == 0) { throw 0; }
    }
    catch(...) { throw 1; }
    d_bufferSize = total;
  }
  return d_buffer;
}

unsigned int TextureLibrary::NextPowerOf2(unsigned int value) const
{
  value = value - 1;
  value = value | (value >> 1);
  value = value | (value >> 2);
  value = value | (value >> 4);
  value = value | (value >> 8);
  value = value | (value >>16);
  return value + 1;
}
