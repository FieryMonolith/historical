#ifndef __System_Spec_h__
#define __System_Spec_h__

#include "System.h"

#include <windows.h>

#include "BuiltinTypes.h"

class iSurface;

class sSystem_MSC : public System
{
public:
   sSystem_MSC(void) { return; }
  ~sSystem_MSC(void) { return; }

private:
  // Prevent copy semantics
  sSystem_MSC(const sSystem_MSC &);
  sSystem_MSC &operator=(const sSystem_MSC &);

public:
  // A value of 0 means use a default size for width and height
  iSurface *GenerateOpenglSurface(const uint32 width,
                                  const uint32 height,
                                  const uint32 x,
                                  const uint32 y);

public:
  void SetProgramInstance(HINSTANCE i) { d_programInstance  = i; return; }
  void SetPrevInstance   (HINSTANCE i) { d_previousInstance = i; return; }
  void SetCmdShow(int x) { d_nCmdShow = x; return; }

private:
  HINSTANCE d_programInstance;
  HINSTANCE d_previousInstance;
  int d_nCmdShow;
}; // class sSystem_MSC

#endif // __System_Spec_h__
