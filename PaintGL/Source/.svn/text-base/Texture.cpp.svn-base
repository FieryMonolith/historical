#include <memory>

#include "Pixel.h"
#include "TextureData.h"

#include "Texture.h"

#include "EasyBMP.h"

using namespace std;
using namespace EasyBMP;


/*** Begin Constructor/Destructor/Assignment *********************************/
Texture::Texture(const string &imageFilename)
// LoadFromFile never returns 0, it would throw an integer exception first.
: d_data(LoadFromFile(imageFilename)),
  d_minX(0), d_minY(0), d_maxX(0), d_maxY(0),
  d_drawX(0), d_drawY(0),
  d_changed(false)
{
  return;
}

Texture::Texture(const RGBA &fillColor,
                 const unsigned int width, const unsigned int height)
// CreateTexture never returns 0, it would throw an integer exception first.
: d_data(CreateTexture(fillColor, width, height)),
  d_minX(0), d_minY(0), d_maxX(0), d_maxY(0),
  d_drawX(0), d_drawY(0),
  d_changed(false)
{
  return;
}

Texture::Texture(const Texture &texture)
: d_data(texture.d_data),
  d_minX(0), d_minY(0), d_maxX(0), d_maxY(0),
  d_drawX(0), d_drawY(0),
  d_changed(false)
{
  return;
}

Texture &Texture::operator=(const Texture &texture)
{
  if(&texture == this) { return *this; }

  d_data = texture.d_data;
  d_minX = d_minY = d_maxX = d_maxY = 0;
  d_drawX = d_drawY = 0;
  d_changed = false;

  return *this;
}

Texture::~Texture(void)
{
  return;
}
/*** End Constructor/Destructor/Assignment ***********************************/


/*** Begin Get/Set Texture Information ***************************************/
unsigned int Texture::GetGLTextureName(void) const
{
  return d_data->d_glTextureName;
}

unsigned int Texture::GetWidth(void) const
{
  return d_data->d_width;
}

unsigned int Texture::GetHeight(void) const
{
  return d_data->d_height;
}

double Texture::GetWidthRatio(void) const
{
  return d_data->d_widthRatio;
}

double Texture::GetHeightRatio(void) const
{
  return d_data->d_heightRatio;
}

const RGBA &Texture::GetPixel(const unsigned int x, const unsigned int y) const
{
  if(x >= d_data->d_width || y >= d_data->d_height) { throw 1; }
  return d_data->d_buffer[y * d_data->d_width + x];
}

const RGBA &Texture::GetPixel(void) const
{
  return d_data->d_buffer[d_drawY * d_data->d_width + d_drawX];
}

void Texture::SetDrawLocation(const unsigned int x, const unsigned int y)
{
  if(x >= d_data->d_width || y >= d_data->d_height) { throw 1; }

  d_drawX = x;
  d_drawY = y;
  return;
}
/*** End Get/Set Texture Information *****************************************/


/*** Begin Texture Modifiers *************************************************/
void Texture::Differentiate(void)
{
  if(d_data.GetCount() <= 1) { return; }

  try
  {
    // Create a copy of the TextureData and set data to the new data.
    // Copy can throw an integer
    // operator= can throw a bad_alloc
    d_data = Copy();
  }
  catch(...) { throw 1; }

  return;
}

void Texture::Load(const string &filename)
{
  // Create a new TextureData by loading data from file.
  // Throws an integer if it fails to create a new data.
  TextureData *newTexture = LoadFromFile(filename);

  // Copy the current TextureData's data into a temporary TextureData
  TextureData temp = *d_data; // shallow copy - default operator=

  // Copy the new TextureData's data into the current TextureData
  *d_data = *newTexture; // shallow copy - default operator=

  // Do not delete newTexture because temp will delete the old data.
  newTexture = 0;

  return;
}

void Texture::Resize(const unsigned int width, const unsigned int height,
                     const RGBA &fillColor)
{
  // Create a new TextureData with the given dimensions and fill color.
  // Throws an integer if it fails to create a new data.
  TextureData *newTexture = CreateTexture(fillColor, width, height);

  // Copy the current TextureData's data into a temporary TextureData
  TextureData temp = *d_data; // shallow copy - default operator=

  // Copy the new TextureData's data into the current TextureData
  *d_data = *newTexture; // shallow copy - default operator=

  // Copy the temporary TextureData's data into the new TextureData
  *newTexture = temp; // shallow copy - default operator=

  // Deleting the newTexture which now contains the old data.
  delete newTexture;

  return;
}

void Texture::Clear(const RGBA &fillColor)
{
  unsigned int total = d_data->d_width * d_data->d_height;
  for(unsigned int i=0; i<total; ++i) { d_data->d_buffer[i] = fillColor; }

  glBindTexture(GL_TEXTURE_2D, d_data->d_glTextureName);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                  d_data->d_width, d_data->d_height,
                  GL_RGBA, GL_UNSIGNED_BYTE, d_data->d_buffer);

  if(glGetError() != GL_NO_ERROR) { throw 1; }

  d_changed = false;

  return;
}

void Texture::SetPixel(const RGBA &color)
{
  if(d_changed == false)
  {
    d_minX = d_maxX = d_drawX;
    d_minY = d_maxY = d_drawY;
  }
  else
  {
    if(d_drawX < d_minX) { d_minX = d_drawX; }
    if(d_drawX > d_maxX) { d_maxX = d_drawX; }
    if(d_drawY < d_minY) { d_minY = d_drawY; }
    if(d_drawY > d_maxY) { d_maxY = d_drawY; }
  }

  d_changed = true;

  unsigned int index = d_drawY * d_data->d_width + d_drawX;
  d_data->d_buffer[index] = color;

  return;
}

void Texture::Flush(void)
{
  unsigned int startX = d_minX;
  unsigned int startY = d_minY;
  unsigned int width  = d_maxX - d_minX + 1;
  unsigned int height = d_maxY - d_minY + 1;

  // Get a temporary buffer that will hold the changed data.  It needs to
  //   be the exact size of data written to the opengl texture.
  RGBA *textureBuffer = 0;
  try
  {
    textureBuffer = new RGBA[width * height];
    if(textureBuffer == 0) { throw 0; }
  }
  catch(...) { throw 1; }

  // Fill in temporary buffer with data area that has changed
  RGBA *pBuffer = textureBuffer;
  for(unsigned int y=startY; y<startY+height; ++y)
    for(unsigned int x=startX; x<startX+width; ++x)
    {
      *pBuffer = d_data->d_buffer[y * d_data->d_width + x];
      ++pBuffer;
    }

  // Update the opengl texture
  glBindTexture(GL_TEXTURE_2D, d_data->d_glTextureName);
  glTexSubImage2D(GL_TEXTURE_2D, 0, startX, startY,
                  width, height,
                  GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

  delete [] textureBuffer;

  if(glGetError() != GL_NO_ERROR) { throw 2; }

  d_changed = false;

  return;
}
/*** End Texture Modifiers ***************************************************/


/*** Begin Texture Output ****************************************************/
void Texture::SaveAs(const string &filename) const
{
  BMP bmp;
  if(bmp.SetSize(d_data->d_width, d_data->d_height) == false) { throw 1; }

  // This program has the texture origin at the bottom left and the bitmap's
  //   origin is the upper left, so the image needs to be inverted vertically.
  RGBA *pBuffer = d_data->d_buffer;
  for(unsigned int y=1; y<=d_data->d_height; y++) // =1; <= height
    for(unsigned int x=0; x<d_data->d_width; x++) // =0; <  width
    {
      RGBApixel pixel;
      pixel.Red   = pBuffer->red;
      pixel.Green = pBuffer->green;
      pixel.Blue  = pBuffer->blue;
      pixel.Alpha = pBuffer->alpha;
      bmp.SetPixel(x, d_data->d_height - y, pixel);
      ++pBuffer;
    }

  if(bmp.WriteToFile(filename.c_str()) == false) { throw 2; }

  return;
}
/*** End Texture Output ******************************************************/


/*** Begin Helper Methods ****************************************************/
TextureData *Texture::CreateTexture(const RGBA &fillColor,
                                    const unsigned int width,
                                    const unsigned int height)
{
  // Create the data structure to hold the new texture
  auto_ptr<TextureData> data;
  try
  {
    data.reset(new TextureData);
    if(data.get() == 0) { throw 0; }
  }
  catch(...) { throw 1; }

  // Set texture dimensions
  data->d_width  = width;
  data->d_height = height;

  // Determine the width/height ratio relative to the actual texture size,
  //   which must be a power of 2.
  unsigned int textureWidth  = NextPowerOf2(data->d_width);
  unsigned int textureHeight = NextPowerOf2(data->d_height);
  data->d_widthRatio  = static_cast<double>(data->d_width)  / textureWidth;
  data->d_heightRatio = static_cast<double>(data->d_height) / textureHeight;

  // Determine total size of the RGBA buffer
  unsigned int total = data->d_width * data->d_height;

  // Create pixel buffer
  try
  {
    // Destructor will handle cleanup of buffer
    data->d_buffer = new RGBA[total];
    if(data->d_buffer == 0) { throw 0; }
  }
  catch(...) { throw 2; }

  // Fill in all the pixel data with the fill color.
  for(unsigned int i=0; i<total; ++i)
  {
    data->d_buffer[i] = fillColor;
  }

  RGBA *textureBuffer = 0;
  try
  {
    textureBuffer = new RGBA[textureWidth * textureHeight];
    if(textureBuffer == 0) { throw 0; }
  }
  catch(...) { throw 3; }

  for(unsigned int y=0; y<height; ++y)
    for(unsigned int x=0; x<width; ++x)
    {
      //memcpy(&textureBuffer[y*textureWidth], &buffer[y*data->d_width], width);
      textureBuffer[y*textureWidth + x] = data->d_buffer[y*data->d_width + x];
    }

  // Tell opengl to create a new texture.
  glGenTextures(1, &(data->d_glTextureName));
  if(glGetError() != GL_NO_ERROR) { delete [] textureBuffer; throw 4; }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, data->d_glTextureName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight,
               0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

  delete [] textureBuffer;

  if(glGetError() != GL_NO_ERROR) {  throw 5; }

  return data.release();
}

TextureData *Texture::Copy(void) const
{
  // Create the data structure to hold the new texture
  auto_ptr<TextureData> copyData;
  try
  {
    copyData.reset(new TextureData);
    if(copyData.get() == 0) { throw 0; }
  }
  catch(...) { throw 1; }

  // Set texture dimensions
  copyData->d_width  = d_data->d_width;
  copyData->d_height = d_data->d_height;

  // Determine the width/height ratio relative to the actual texture size,
  //   which must be a power of 2.
  unsigned int textureWidth  = NextPowerOf2(copyData->d_width);
  unsigned int textureHeight = NextPowerOf2(copyData->d_height);
  copyData->d_widthRatio  =
    static_cast<double>(copyData->d_width)  / textureWidth;
  copyData->d_heightRatio =
    static_cast<double>(copyData->d_height) / textureHeight;

  // Determine total size of the RGBA buffer
  unsigned int total = copyData->d_width * copyData->d_height;

  // Create pixel buffer
  try
  {
    // Destructor will handle cleanup of buffer
    copyData->d_buffer = new RGBA[total];
    if(copyData->d_buffer == 0) { throw 0; }
  }
  catch(...) { throw 2; }

  // Fill in all the pixel data with the fill color.
  for(unsigned int i=0; i<total; ++i)
  {
    copyData->d_buffer[i] = d_data->d_buffer[i];
  }

  RGBA *textureBuffer = 0;
  try
  {
    textureBuffer = new RGBA[textureWidth * textureHeight];
    if(textureBuffer == 0) { throw 0; }
  }
  catch(...) { throw 3; }

  for(unsigned int y=0; y<copyData->d_height; ++y)
    for(unsigned int x=0; x<copyData->d_width; ++x)
    {
      //memcpy(&textureBuffer[y*textureWidth], &buffer[y*data->d_width], width);
      textureBuffer[y*textureWidth + x] =
        copyData->d_buffer[y * copyData->d_width + x];
    }

  // Tell opengl to create a new texture.
  glGenTextures(1, &(copyData->d_glTextureName));
  if(glGetError() != GL_NO_ERROR) { delete [] textureBuffer; throw 4; }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, copyData->d_glTextureName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight,
               0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

  delete [] textureBuffer;

  if(glGetError() != GL_NO_ERROR) {  throw 5; }

  return copyData.release();
}

TextureData *Texture::LoadFromFile(const string &filename)
{
  // Load image
  BMP bmp;
  if(bmp.ReadFromFile(filename.c_str()) == false) { throw 1; }

  // Determine the image width and height and throw exception if the image
  //   has a zero or negative dimension.
  if(bmp.TellWidth() <= 0 || bmp.TellHeight() <= 0) { throw 2; }

  // Create the data structure to hold the new texture
  auto_ptr<TextureData> data;
  try
  {
    data.reset(new TextureData);
    if(data.get() == 0) { throw 0; }
  }
  catch(...) { throw 3; }

  // Set texture dimensions
  data->d_width  = static_cast<unsigned int>(bmp.TellWidth());
  data->d_height = static_cast<unsigned int>(bmp.TellHeight());

  // Determine the width/height ratio relative to the actual texture size,
  //   which must be a power of 2.
  unsigned int textureWidth  = NextPowerOf2(data->d_width);
  unsigned int textureHeight = NextPowerOf2(data->d_height);
  data->d_widthRatio  = static_cast<double>(data->d_width)  / textureWidth;
  data->d_heightRatio = static_cast<double>(data->d_height) / textureHeight;

  // Create pixel buffer
  try
  {
    // Destructor will handle cleanup of buffer
    data->d_buffer = new RGBA[data->d_width * data->d_height];
    if(data->d_buffer == 0) { throw 0; }
  }
  catch(...) { throw 4; }

  // Fill in all the pixel data from the image into the buffer.  Because this
  //   program has its original at the bottom left and the bitmap has theirs
  //   at the top left, the image needs to be inverted vertically.
  RGBA *pBuffer = data->d_buffer;
  for(unsigned int y=1; y<=data->d_height; ++y) // =1 ; <=height  for inversion
    for(unsigned int x=0; x<data->d_width; ++x) // =0 ; < width
    {
      RGBApixel pixel = bmp.GetPixel(x, data->d_height - y);
      pBuffer->red   = pixel.Red;
      pBuffer->green = pixel.Green;
      pBuffer->blue  = pixel.Blue;
      pBuffer->alpha = static_cast<unsigned char>(0xff);
      ++pBuffer;
    }

  RGBA *textureBuffer = 0;
  try
  {
    textureBuffer = new RGBA[textureWidth * textureHeight];
    if(textureBuffer == 0) { throw 0; }
  }
  catch(...) { throw 5; }

  for(unsigned int y=0; y<data->d_height; ++y)
    for(unsigned int x=0; x<data->d_width; ++x)
    {
      //memcpy(&textureBuffer[y*textureWidth], &buffer[y*data->d_width], width);
      textureBuffer[y*textureWidth + x] = data->d_buffer[y*data->d_width + x];
    }

  // Tell opengl to create a new texture.
  glGenTextures(1, &(data->d_glTextureName));
  if(glGetError() != GL_NO_ERROR) { delete [] textureBuffer; throw 6; }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, data->d_glTextureName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight,
               0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

  delete [] textureBuffer;

  if(glGetError() != GL_NO_ERROR) { throw 7; }

  return data.release();
}

unsigned int Texture::NextPowerOf2(unsigned int value) const
{
  value = value - 1;
  value = value | (value >> 1);
  value = value | (value >> 2);
  value = value | (value >> 4);
  value = value | (value >> 8);
  value = value | (value >>16);
  return value + 1;
}
/*** End Helper Methods ******************************************************/
