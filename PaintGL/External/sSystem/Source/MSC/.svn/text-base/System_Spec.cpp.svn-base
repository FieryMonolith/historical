#include "System_Spec.h"

#include "iSurface.h"
#include "iSurface_System.h"
#include "OpenglSurface_Spec.h"

using namespace std;

/*** System Access and Support *********************************************/
static sSystem_MSC g_sSystem_MSC;

sSystem &AccessSystem(void) { return g_sSystem_MSC; }

// Used to standardize application entry points
extern int main(int argc, char **argv);

// Win32 Graphical Application true entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR /*lpCmdLine*/, int nCmdShow)
{
  g_sSystem_MSC.SetProgramInstance(hInstance);
  g_sSystem_MSC.SetPrevInstance(hPrevInstance);
  g_sSystem_MSC.SetCmdShow(nCmdShow);
  return main(0, 0);
}

/*** sSystem_MSC Definitions ***********************************************/
iSurface *sSystem_MSC::GenerateOpenglSurface(const uint32 width,
                                             const uint32 height,
                                             const uint32 x,
                                             const uint32 y)
{
  try
  {
    iSurface_System *s = new OpenglSurface_MSC((width)  ? width : CW_USEDEFAULT,
                                               (height) ? height: CW_USEDEFAULT,
                                               (x)      ? x     : CW_USEDEFAULT,
                                               (y)      ? y     : CW_USEDEFAULT,
                                               d_programInstance);
    d_surfaces.push_back(s);
    return s;
  }
  catch(...) { throw 1; }
}
