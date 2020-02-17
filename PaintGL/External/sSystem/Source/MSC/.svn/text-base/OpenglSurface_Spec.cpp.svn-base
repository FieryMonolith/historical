#include "OpenglSurface_Spec.h"

#include <cstring>

using namespace std;

/*** BEGIN Static Class Definitions ******************************************/
WNDCLASS OpenglSurface_MSC::d_wndClass;
uint     OpenglSurface_MSC::d_registered = 0;
vector< pair<HWND, OpenglSurface_MSC*> > OpenglSurface_MSC::d_hwndToSurface;
/*** END Static Class Definitions ********************************************/

/*** BEGIN Class Definitions *************************************************/
OpenglSurface_MSC::OpenglSurface_MSC(const uint32 width, const uint32 height,
                                     const uint32 x, const uint32 y,
                                     HINSTANCE hInstance)
: OpenglSurface(width, height),
  d_hwnd(0), d_hdc(0), d_hglrc(0),
  d_x(x), d_y(y),
  d_mouse(), d_keyboard()
{
  if(OpenglSurface_MSC::d_registered == 0)
  {
    d_wndClass.style         = CS_OWNDC; // Each window has unique DC
    d_wndClass.lpfnWndProc   = (WNDPROC)OpenglSurface_MSC::WndProc;
    d_wndClass.cbClsExtra    = 0;
    d_wndClass.cbWndExtra    = 0;
    d_wndClass.hInstance     = 0;
    d_wndClass.hIcon         = 0;
    d_wndClass.hCursor       = ::LoadCursor(0, IDC_ARROW);
    d_wndClass.hbrBackground = 0;
    d_wndClass.lpszMenuName  = 0;
    d_wndClass.lpszClassName = "OpenglSurface";

    if(!::RegisterClass(&d_wndClass)) throw 1;
    OpenglSurface_MSC::d_registered = 1;
  }

  d_hwnd = ::CreateWindow("OpenglSurface", 0,
                          WS_POPUP | WS_VISIBLE,
                          d_x, d_y,
                          d_width, d_height,
                          0, 0,
                          hInstance, 0);

  if(d_hwnd == 0) throw 2;

  try
  {
    OpenglSurface_MSC::d_hwndToSurface.push_back(make_pair(d_hwnd, this));

    d_hdc = ::GetDC(d_hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0x00, sizeof(pfd));

    pfd.nSize      = sizeof(pfd);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 8;

    int pixelformat = ::ChoosePixelFormat(d_hdc, &pfd);
    if(pixelformat == 0) throw 1;

    if(::SetPixelFormat(d_hdc, pixelformat, &pfd) == 0) throw 2;

    d_hglrc = ::wglCreateContext(d_hdc);
    if(d_hglrc == 0) throw 3;

    MakeCurrent();

    ::ShowWindow(d_hwnd, SW_SHOWNORMAL);
    ::UpdateWindow(d_hwnd);
  }
  catch(...)
  {
    ::DestroyWindow(d_hwnd);
    if(d_hglrc) ::wglDeleteContext(d_hglrc);
    throw 3;
  }

  return;
}

OpenglSurface_MSC::~OpenglSurface_MSC(void)
{
  try { MakeCurrent(); } catch(...) { }

  if(d_hglrc != 0) ::wglDeleteContext(d_hglrc);
  d_hglrc = 0;

  ::DestroyWindow(d_hwnd);
  d_hwnd = 0;

  return;
}

void OpenglSurface_MSC::ProcessMessages_Blocking(void)
{
  d_mouse.ClearChanged();
  d_keyboard.ClearChanged();

  MSG msg;
  do
  {
    if(::GetMessage(&msg, d_hwnd, 0, 0) == 0) throw 1;
    ::DispatchMessage(&msg);
  }
  while(::PeekMessage(&msg, d_hwnd, 0, 0, PM_NOREMOVE) != 0);

  return;
}

void OpenglSurface_MSC::ProcessMessages_NonBlocking(void)
{
  d_mouse.ClearChanged();
  d_keyboard.ClearChanged();

  MSG msg;
  while(::PeekMessage(&msg, d_hwnd, 0, 0, PM_NOREMOVE) != 0)
  {
    if(::GetMessage(&msg, d_hwnd, 0, 0) == 0) throw 1;
    ::DispatchMessage(&msg);
  }

  return;
}

void OpenglSurface_MSC::NewPosition(const uint32 x, const uint32 y)
{
  if(::MoveWindow(d_hwnd, x, y, d_width, d_height, FALSE) == 0)
    throw 1;

  d_x = x;
  d_y = y;

  if(::InvalidateRect(0, 0, true) == 0)
    throw 2;

  return;
}

void OpenglSurface_MSC::Resize(const uint32 width, const uint32 height)
{
  if(::MoveWindow(d_hwnd, d_x, d_y, width, height, FALSE) == 0)
    throw 1;

  OpenglSurface::Resize(width, height);

  if(::InvalidateRect(0, 0, true) == 0)
    throw 2;

  return;
}

void OpenglSurface_MSC::MakeCurrent(void)
{
  if(::wglMakeCurrent(d_hdc, d_hglrc) == FALSE)
    throw 1;

  return;
}

void OpenglSurface_MSC::SwapBuffers(void)
{
  if(::SwapBuffers(d_hdc) == FALSE)
    throw 1;

  return;
}

void OpenglSurface_MSC::Show(void)
{
  ::ShowWindow(d_hwnd, SW_SHOW);
  return;
}

void OpenglSurface_MSC::Hide(void)
{
  ::ShowWindow(d_hwnd, SW_HIDE);
  return;
}

void OpenglSurface_MSC::Minimize(void)
{
  ::ShowWindow(d_hwnd, SW_MINIMIZE);
  return;
}

void OpenglSurface_MSC::Restore(void)
{
  ::ShowWindow(d_hwnd, SW_RESTORE);
  return;
}
/*** END Class Definitions ***************************************************/

/*** BEGIN Class Static Definitions ******************************************/
LONG WINAPI OpenglSurface_MSC::WndProc(HWND hwnd, UINT msg,
                                       WPARAM wParam, LPARAM lParam)
{
  // If the message isn't one of these few I want, pass it on to the
  //   DefWindowProc immediately.
  switch(msg)
  {
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_MOUSEMOVE:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_MBUTTONDOWN:
  case WM_MBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
#if 0
  case WM_XBUTTONDOWN:
  case WM_XBUTTONUP:
#endif
    break;

  default:
    return static_cast<long>(::DefWindowProc(hwnd, msg, wParam, lParam));
  }

  // Find the appropriate OpenglSurface pointer or if one can't be 
  //   found pass it on to DefWindowProc.
  OpenglSurface_MSC *os = 0;

  vector< pair<HWND, OpenglSurface_MSC*> >::iterator i =
    OpenglSurface_MSC::d_hwndToSurface.begin();

  for(; i != OpenglSurface_MSC::d_hwndToSurface.end(); ++i)
    if(i->first == hwnd) os = i->second;

  if(os == 0)
    // Ignore --> Pass to default handler
    return static_cast<long>(::DefWindowProc(hwnd, msg, wParam, lParam));

  // Once the OpenglSurface has been found and we know the message is
  //   something desired ... process it.
  switch(msg)
  {
  /*** BEGIN Keyboard ********************************************************/
  case WM_KEYDOWN:
    os->d_keyboard.SetButton(static_cast<uint8>(wParam));
    break;

  case WM_KEYUP:
    os->d_keyboard.ClearButton(static_cast<uint8>(wParam));
    break;
  /*** END   Keyboard ********************************************************/

  /*** BEGIN Mouse ***********************************************************/
  case WM_MOUSEMOVE:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    break;

  case WM_LBUTTONDOWN:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    os->d_mouse.SetButton(Mouse::Button1);

    break;

  case WM_LBUTTONUP:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    os->d_mouse.ClearButton(Mouse::Button1);
    break;

  case WM_MBUTTONDOWN:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    os->d_mouse.SetButton(Mouse::Button2);

    break;

  case WM_MBUTTONUP:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    os->d_mouse.ClearButton(Mouse::Button2);
    break;

  case WM_RBUTTONDOWN:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    os->d_mouse.SetButton(Mouse::Button3);

    break;

  case WM_RBUTTONUP:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    os->d_mouse.ClearButton(Mouse::Button3);
    break;

#if 0
  case WM_XBUTTONDOWN:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    if     (HIWORD(wParam) == XBUTTON1) os->d_mouse.SetButton(Mouse::Button4);
    else if(HIWORD(wParam) == XBUTTON2) os->d_mouse.SetButton(Mouse::Button5);
    return TRUE;

  case WM_XBUTTONUP:
    os->d_mouse.SetPosition(static_cast<uint32>(LOWORD(lParam)),
                            static_cast<uint32>(os->d_height - HIWORD(lParam)));
    if     (HIWORD(wParam) == XBUTTON1) os->d_mouse.ClearButton(Mouse::Button4);
    else if(HIWORD(wParam) == XBUTTON2) os->d_mouse.ClearButton(Mouse::Button5);
    return TRUE;
#endif
  /*** END   Mouse ***********************************************************/
  }

  return 0;
}
/*** END Class Static Definitions ********************************************/
