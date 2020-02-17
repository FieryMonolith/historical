#ifndef __OpenglSurface_Spec_h__
#define __OpenglSurface_Spec_h__

#include "OpenglSurface.h"

#include <windows.h>
#include <vector>

#include "BuiltinTypes.h"
#include "Mouse_Spec.h"
#include "Keyboard_Spec.h"

class OpenglSurface_MSC : public OpenglSurface
{
public:
  // A value of 0 means use a default size for width and height
  OpenglSurface_MSC(const uint32 width, const uint32 height,
                    const uint32 x, const uint32 y,
                    HINSTANCE hInstance) throw (int);
  ~OpenglSurface_MSC(void)               throw ();

private:
  OpenglSurface_MSC(const OpenglSurface_MSC &);
  OpenglSurface_MSC &operator=(const OpenglSurface_MSC &);

public:
  // Device interfaces
  iKeyboard &GetKeyboard(void) { return d_keyboard; }
  iMouse    &GetMouse(void)    { return d_mouse;    }

  void ProcessMessages_Blocking   (void) throw (int);
  void ProcessMessages_NonBlocking(void) throw (int);

  // Surface screen position
  uint32 GetX(void) const { return d_x; }
  uint32 GetY(void) const { return d_y; }
  void NewPosition(const uint32 x, const uint32 y)     throw (int);

  // Surface screen dimensions
  void Resize(const uint32 width, const uint32 height) throw (int);

  // Surface visibility
  void MakeCurrent(void) throw (int);
  void SwapBuffers(void) throw (int);

  void Show(void)        throw ();
  void Hide(void)        throw ();
  void Minimize(void)    throw ();
  void Restore(void)     throw ();

private:
  /*** Data ******************************************************************/
  HWND  d_hwnd;
  HDC   d_hdc;
  HGLRC d_hglrc;
  
  uint32 d_x, d_y;

  Mouse_MSC d_mouse;
  Keyboard_MSC d_keyboard;

  static WNDCLASS d_wndClass;
  static uint     d_registered;
  static std::vector< std::pair<HWND, OpenglSurface_MSC*> > d_hwndToSurface;

  /*** Support Functions *****************************************************/
  static LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
}; // class OpenglSurface_MSC

#endif // __OpenglSurface_Spec_h__
