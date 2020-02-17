/*
 * Example of a Win32 OpenGL program.
 * The OpenGL code is the based on the example
 * given at http://msdn.microsoft.com/en-us/library/ms537714(VS.85).aspx
 * which is in turn based on an X Window version
 */

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <fstream>
#include <cmath>
#include <vector>

#include "Pixel.h"
#include "GraphicObject.h"
#include "Loader.h"
#include "Font.h"
#include "iMouse.h"
#include "iKeyboard.h"
#include "iSurface.h"
#include "sSystem.h"

#include "EasyBMP.h"
using namespace EasyBMP;

using namespace std;

int whichType = 0;
vector<GraphicObject*> objects;
GraphicObject *drawArea=0;
Font *gFont=0;
RGBA drawColor(255, 0, 0, 255);

void DrawEllipse(const double x1, const double y1,
                 const double x2, const double y2,
                 GraphicObject &);
void DrawLine(const double x1, const double y1,
              const double x2, const double y2,
              GraphicObject &);
void DrawPoint(const double x, const double y, GraphicObject &);
void DrawRectangle(const double x1, const double y1,
                   const double x2, const double y2,
                   GraphicObject &);

void DrawScene(iSurface *, bool select=false);
bool Select(iSurface *);
void HandleKeyboard(iKeyboard &);
bool HandleMouse(iMouse &, iSurface *);

void UpdateViewport(iSurface *);
void InitializeGL(void);

iMouse *gMouse = 0;
iKeyboard *gKeyboard = 0;
iMouse::Position gMarkedPosition;

int main(int /*argc*/, char ** /*argv*/)
{
  try
  {
    // Initialize system resources
    sSystem   &system   = AccessSystem();
    iSurface  *surface  = system.GenerateOpenglSurface(640, 480, 100, 100);
    iMouse    &mouse    = system.GetMouse(surface);
    iKeyboard &keyboard = system.GetKeyboard(surface);

    // Initialize program parameters
    gMouse    = &mouse;
    gKeyboard = &keyboard;
    InitializeGL();
    UpdateViewport(surface);
    DrawScene(surface);

    // Run program in message handling based mode
    while(true)
    {
      system.ProcessMessages_Blocking(surface);
      if(keyboard.IsTriggered('Q')) break;
      if(keyboard.IsTriggered('T')) system.Minimize(surface);
      if(keyboard.IsTriggered('Y')) system.Restore(surface);
      if(keyboard.IsTriggered('U')) system.Show(surface);
      if(keyboard.IsTriggered('I')) system.NewPosition(surface, 300, 100);
      if(keyboard.IsTriggered('O')) surface->Resize(640, 640);
      if(keyboard.IsTriggered('P')) surface->Resize(640, 480);
      //HandleKeyboard(keyboard);
      if(!HandleMouse(mouse, surface)) break;
      DrawScene(surface);
    }

    // Clean up
    if(gFont != 0) { delete gFont; }
    system.Destroy(surface);
  }
  catch(...)
  {
    MessageBox(0, "Main caught exception.", "Error", MB_OK);
    return 1;
  }

  return 0;
}

void DrawScene(iSurface *surface, bool select)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  int value=0;
  vector<GraphicObject*>::iterator i = objects.end();
  for(value=1, i=objects.begin(); i!=objects.end(); ++i, ++value)
  {
    if(select) { glLoadName(value); }
    if(*i != 0) { (*i)->Draw(); }
  }

  gFont->print(450, 10, "%d", static_cast<int>(gMouse->ButtonCount()));

  gFont->print(490, 10, "(");
  gFont->print(500, 10, "%d", static_cast<int>(gMouse->GetX()));
  gFont->print(540, 10, ",");
  gFont->print(550, 10, "%d", static_cast<int>(gMouse->GetY()));
  gFont->print(590, 10, ")");

  glPopMatrix();

  glFlush();

  surface->SwapBuffers();

  return;
}

bool Select(iSurface *surface)
{
  GLuint selectBuffer[100];
  GLint hits;

  glSelectBuffer(100, selectBuffer);
  glRenderMode(GL_SELECT);

  glInitNames();
  glPushName(0);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix(gMouse->GetX(), gMouse->GetY(), 1, 1, viewport);
  glOrtho(0.0, surface->GetWidth(), 0.0, surface->GetHeight(), -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  DrawScene(surface, true);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  hits = glRenderMode(GL_RENDER);

  char s[256];

  // process hits
  GLuint *ptr = (GLuint*)selectBuffer;
  for(int i=0; i<hits; ++i)
  {
    GLuint names = *ptr; ++ptr;
    float min = *ptr/0x7fffffff; ++ptr;
    float max = *ptr/0x7fffffff; ++ptr;
    for(int j=0; j<names; ++j)
    {
      GLuint x = *ptr; ++ptr;
      switch(x-1)
      {
      case 0:
        whichType = 0;
        objects[1]->Move(1);
        objects[0]->Move(0);

        objects[3]->Move(1);
        objects[4]->Move(0);
        objects[6]->Move(1);
        objects[7]->Move(0);
        objects[9]->Move(1);
        objects[10]->Move(0);
        break;

      case 3:
        whichType = 1;
        objects[4]->Move(1);
        objects[3]->Move(0);

        objects[0]->Move(1);
        objects[1]->Move(0);
        objects[6]->Move(1);
        objects[7]->Move(0);
        objects[9]->Move(1);
        objects[10]->Move(0);
        break;

      case 6:
        whichType = 2;
        objects[7]->Move(1);
        objects[6]->Move(0);

        objects[0]->Move(1);
        objects[1]->Move(0);
        objects[3]->Move(1);
        objects[4]->Move(0);
        objects[9]->Move(1);
        objects[10]->Move(0);
        break;

      case 9:
        whichType = 3;
        objects[10]->Move(1);
        objects[9]->Move(0);

        objects[0]->Move(1);
        objects[1]->Move(0);
        objects[3]->Move(1);
        objects[4]->Move(0);
        objects[6]->Move(1);
        objects[7]->Move(0);
        break;

      case 13:
        drawColor = RGBA(255, 0, 0, 255);
        break;

      case 14:
        drawColor = RGBA(0, 255, 0, 255);
        break;

      case 15:
        drawColor = RGBA(0, 0, 255, 255);
        break;

      case 16:
        return false;
        break;
      }
    }
  }

  return true;
}

void DrawEllipse(const double x1, const double y1,
                 const double x2, const double y2,
                 GraphicObject &object)
{
  double width = fabs(x2 - x1);
  double height = fabs(y2 - y1);

  double a = width / 2.0;
  double b = height / 2.0;
  double cx = (x1 + x2) / 2.0;
  double cy = (y1 + y2) / 2.0;

  for(double theta=0.0; theta<=6.28; theta+=0.01)
  {
    double x = cx + a * cos(theta);
    double y = cy + b * sin(theta);
    if(object.SetDrawLocation(x, y) == true)
    {
      object.SetPixel(drawColor);
    }
  }

  return;
}

void DrawLine(const double x1, const double y1,
              const double x2, const double y2,
              GraphicObject &object)
{
  // For now, I am going to do the vector approach.
  double dx = x2 - x1;
  double dy = y2 - y1;
  double distance = sqrt(dx*dx + dy*dy) + 1.0; // inclusion of end point
  dx /= distance; // normalize vector
  dy /= distance; // normalize vector

  // Assuming the points are valid !!!!

  // Start (x,y) at the start point (x1,y1)
  double x = x1;
  double y = y1;
  while(distance > 0)
  {
    if(object.SetDrawLocation(x, y) == true)
    {
      object.SetPixel(drawColor);
    }

    x += dx;
    y += dy;
    distance -= 1.0;
  }

  return;
}

void DrawPoint(const double x, const double y, GraphicObject &object)
{
  if(object.SetDrawLocation(x, y) == true)
  {
    object.SetPixel(drawColor);
  }

  return;
}

void DrawRectangle(const double x1, const double y1,
                   const double x2, const double y2,
                   GraphicObject &object)
{
  DrawLine(x1, y1, x2, y1, object);
  DrawLine(x2, y1, x2, y2, object);
  DrawLine(x2, y2, x1, y2, object);
  DrawLine(x1, y2, x1, y1, object);

  return;
}

void HandleKeyboard(iKeyboard &keyboard)
{
  if(keyboard.IsTriggered('7'))
  {
    if(drawArea != 0) { drawArea->Differentiate(); }
  }
  else if(keyboard.IsTriggered('8'))
  {
    if(drawArea != 0) { drawArea->SaveAs("temp.bmp"); }
  }
  else if(keyboard.IsTriggered('9'))
  {
    if(drawArea != 0) { drawArea->Load("temp.bmp"); }
  }
  else if(keyboard.IsTriggered('0'))
  {
    if(drawArea != 0) { drawArea->Clear(RGBA(50,50,50,255)); }
  }

  return;
}

bool HandleMouse(iMouse &mouse, iSurface *surface)
{
  if(mouse.IsTriggered(iMouse::Button1))
  {
    gMarkedPosition = mouse.GetPosition();

    if(!Select(surface)) return false;

    if(whichType == 0 && drawArea != 0)
    {
      DrawPoint(mouse.GetX(), mouse.GetY(), *drawArea);
      drawArea->Flush();
    }
  }
  if(mouse.IsNotTriggered(iMouse::Button1))
  {
    if(whichType == 0 || drawArea == 0) { return true; }
    else if(whichType == 1)
    {
      DrawLine(gMarkedPosition.x, gMarkedPosition.y,
               mouse.GetX(), mouse.GetY(), *drawArea);
      drawArea->Flush();
    }
    else if(whichType == 2)
    {
      DrawRectangle(gMarkedPosition.x, gMarkedPosition.y,
                    mouse.GetX(), mouse.GetY(), *drawArea);
      drawArea->Flush();
    }
    else if(whichType == 3)
    {
      DrawEllipse(gMarkedPosition.x, gMarkedPosition.y,
                  mouse.GetX(), mouse.GetY(), *drawArea);
      drawArea->Flush();
    }
  }

  return true;
}

void UpdateViewport(iSurface *surface)
{
  glViewport(0, 0, surface->GetWidth(), surface->GetHeight());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, surface->GetWidth(), 0.0, surface->GetHeight(), -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  return;
}

void InitializeGL(void)
{
  glClearColor(0.5, 0.5, 0.5, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glRenderMode(GL_RENDER);

  try
  {
    LoadLayout("../Input/Layout/gui_layout.txt", objects, &drawArea);
  }
  catch(...)
  {
    MessageBox(0, "Loading layout", "Error", MB_OK);
    //PostQuitMessage(0);
  }

  try
  {
    gFont = new Font("../Input/Fonts/Arial.TTF", 16);
    //gFont = new Font("../Input/Fonts/cour.ttf", 16);
  }
  catch(...)
  {
    MessageBox(0, "Loading font", "Error", MB_OK);
    //PostQuitMessage(0);
  }

  switch(whichType)
  {
  case 1:
    objects[4]->Move(1);
    objects[3]->Move(0);
    break;
  case 2:
    objects[7]->Move(1);
    objects[6]->Move(0);
    break;
  case 3:
    objects[10]->Move(1);
    objects[9]->Move(0);
    break;
  case 0:
    objects[1]->Move(1);
    objects[0]->Move(0);
    break;
  }

  return;
}
