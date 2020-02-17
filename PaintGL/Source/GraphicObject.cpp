#include <windows.h>
#include <GL/gl.h>
#include <string>

#include "Pixel.h"
#include "Texture.h"

#include "GraphicObject.h"

using namespace std;

GraphicObject::Vertex::Vertex(const double i_x,
                              const double i_y,
                              const double i_z)
: x(i_x), y(i_y), z(i_z)
{
  return;
}

GraphicObject::GraphicObject(const char *imageFilename,
                             const double x,
                             const double y,
                             const double z)
: d_texture(new Texture(imageFilename))
{
  if(d_texture == 0) { throw 9; }

  unsigned int width  = d_texture->GetWidth();
  unsigned int height = d_texture->GetHeight();
  // Set up the d_quad vertices that the texture will be rendered on.
  d_quad[0].x = x;         d_quad[0].y = y;          d_quad[0].z = z; // lower left
  d_quad[1].x = x;         d_quad[1].y = y + height; d_quad[1].z = z; // upper left
  d_quad[2].x = x + width; d_quad[2].y = y + height; d_quad[2].z = z; // upper right
  d_quad[3].x = x + width; d_quad[3].y = y;          d_quad[3].z = z; // lower right

  return;
}

GraphicObject::GraphicObject(const RGBA &fillColor,
                             const unsigned int width,
                             const unsigned int height,
                             const double x,
                             const double y,
                             const double z)
: d_texture(new Texture(fillColor, width, height))
{
  if(d_texture == 0) { throw 9; }

  // Set up the d_quad vertices that the texture will be rendered on.
  d_quad[0].x = x;         d_quad[0].y = y;          d_quad[0].z = z; // lower left
  d_quad[1].x = x;         d_quad[1].y = y + height; d_quad[1].z = z; // upper left
  d_quad[2].x = x + width; d_quad[2].y = y + height; d_quad[2].z = z; // upper right
  d_quad[3].x = x + width; d_quad[3].y = y;          d_quad[3].z = z; // lower right

  return;
}

GraphicObject::GraphicObject(const GraphicObject &object)
: d_texture(new Texture(*object.d_texture))
{
  if(d_texture == 0) { throw 1; }

  Copy(object);
  return;
}

GraphicObject::~GraphicObject(void)
{
  if(d_texture != 0)
  {
    delete d_texture;
    d_texture = 0;
  }

  return;
}

GraphicObject &GraphicObject::operator=(const GraphicObject &object)
{
  // Do nothing for self assignment
  if(&object == this) { return *this; }

  try
  {
    d_texture = new Texture(*object.d_texture);
    if(d_texture == 0) { throw 0; }
  }
  catch(...) { throw 1; }

  Copy(object);

  return *this;
}

bool GraphicObject::Clear(const RGBA &fillColor)
{
  d_texture->Clear(fillColor);
  return true;
}

void GraphicObject::Copy(const GraphicObject &object)
{
  // Set up the d_quad vertices that the texture will be rendered on.
  // lower left
  d_quad[0].x = object.d_quad[0].x;
  d_quad[0].y = object.d_quad[0].y;
  d_quad[0].z = object.d_quad[0].z;

  // upper left
  d_quad[1].x = object.d_quad[1].x;
  d_quad[1].y = object.d_quad[1].y;
  d_quad[1].z = object.d_quad[1].z;

  // upper right
  d_quad[2].x = object.d_quad[2].x;
  d_quad[2].y = object.d_quad[2].y;
  d_quad[2].z = object.d_quad[2].z;
  
  // lower right
  d_quad[3].x = object.d_quad[3].x;
  d_quad[3].y = object.d_quad[3].y;
  d_quad[3].z = object.d_quad[3].z;

  return;
}

bool GraphicObject::Differentiate(void)
{
  try { d_texture->Differentiate(); }
  catch(...) { return false; }

  return true;
}

bool GraphicObject::Load(const string &filename)
{
  try { d_texture->Load(filename); }
  catch(...) { return false; }
  return true;
}

bool GraphicObject::SaveAs(const string &filename) const
{
  try { d_texture->SaveAs(filename); }
  catch(...) { return false; }
  return true;
}

void GraphicObject::Draw(void) const
{
  if(d_texture == 0) { return; }

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glBindTexture(GL_TEXTURE_2D, d_texture->GetGLTextureName());

  double w = d_texture->GetWidthRatio();
  double h = d_texture->GetHeightRatio();
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0); glVertex3d(d_quad[0].x, d_quad[0].y, d_quad[0].z);
  glTexCoord2d(0.0, h);   glVertex3d(d_quad[1].x, d_quad[1].y, d_quad[1].z);
  glTexCoord2d(w,   h);   glVertex3d(d_quad[2].x, d_quad[2].y, d_quad[2].z);
  glTexCoord2d(w,   0.0); glVertex3d(d_quad[3].x, d_quad[3].y, d_quad[3].z);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  if(glGetError() != GL_NO_ERROR) { return; }

  return;
}

void GraphicObject::Move(const double x, const double y)
{
  unsigned int width  = d_texture->GetWidth();
  unsigned int height = d_texture->GetHeight();

  // Set up the d_quad vertices that the texture will be rendered on.
  d_quad[0].x = x;         d_quad[0].y = y;          // lower left
  d_quad[1].x = x;         d_quad[1].y = y + height; // upper left
  d_quad[2].x = x + width; d_quad[2].y = y + height; // upper right
  d_quad[3].x = x + width; d_quad[3].y = y;          // lower right

  return;
}

void GraphicObject::Move(const double x, const double y, const double z)
{
  unsigned int width  = d_texture->GetWidth();
  unsigned int height = d_texture->GetHeight();

  // Set up the d_quad vertices that the texture will be rendered on.
  d_quad[0].x = x;         d_quad[0].y = y;          d_quad[0].z = z; // lower left
  d_quad[1].x = x;         d_quad[1].y = y + height; d_quad[1].z = z; // upper left
  d_quad[2].x = x + width; d_quad[2].y = y + height; d_quad[2].z = z; // upper right
  d_quad[3].x = x + width; d_quad[3].y = y;          d_quad[3].z = z; // lower right

  return;
}

void GraphicObject::Move(const double z)
{
  // Set up the d_quad vertices that the texture will be rendered on.
  d_quad[0].z = z; // lower left
  d_quad[1].z = z; // upper left
  d_quad[2].z = z; // upper right
  d_quad[3].z = z; // lower right

  return;
}

const bool GraphicObject::SetDrawLocation(const double x, const double y)
{
  if(x >= d_quad[0].x && x <= d_quad[2].x  &&
     y >= d_quad[0].y && y <= d_quad[2].y)
  {
    // Transform image back to (0,0) before computing the index
    unsigned int uiX = static_cast<unsigned int>(x - d_quad[0].x);
    unsigned int uiY = static_cast<unsigned int>(y - d_quad[0].y);

    try { d_texture->SetDrawLocation(uiX, uiY); }
    catch(...) { return false; }

    return true;
  }
  return false;
}

void GraphicObject::SetPixel(const RGBA &rgba)
{
  d_texture->SetPixel(rgba);
  return;
}
void GraphicObject::Flush(void)
{
  d_texture->Flush();
  return;
}

const RGBA &GraphicObject::GetPixel(void) const
{
  return d_texture->GetPixel();
}

const double GraphicObject::GetX(void) const
{
  return d_quad[0].x;
}

const double GraphicObject::GetY(void) const
{
  return d_quad[0].y;
}

const double GraphicObject::GetZ(void) const
{
  return d_quad[0].z;
}

const unsigned int GraphicObject::GetWidth(void) const
{
  return d_texture->GetWidth();
}

const unsigned int GraphicObject::GetHeight(void) const
{
  return d_texture->GetHeight();
}
