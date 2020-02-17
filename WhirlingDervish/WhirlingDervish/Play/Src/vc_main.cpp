#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include "vc_main_window.h"
#include "interactive_environment.h"
#include "language_engine.h"
#include "file_handler.h"
#include "command_line_settings.h"

using namespace std;

MainWindow *g_mw;
HDC g_hdc;

LRESULT CALLBACK MainWindowProcedure(HWND hwnd, unsigned int message,
								                     WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, char *, int cmdShow)
{
  CommandLineSettings cls;
  if(!cls.Initialize()) {
    MessageBox(NULL,
               "Command line arguements.",
               "Could not Initialize CommandLineSettings, could not process", MB_OK);
    return 0;
  }

#if 0
  // This loads the location and file names of the configuration files.
  // If they are on the command line, they override the default name
  // and location.
  if(!cls.ProcessFileInfo(argc, argv)) {
    MessageBox(NULL,
               "Could not process command line.",
               "Command line arguements.", MB_OK);
    return 0;
  }
#endif

  // Prepare Experiment
  FileHandler fh;
  InteractiveEnvironment ie(0);
  if(!ie.Initialize()) {
    MessageBox(NULL,
               "ERROR: Failed to initialize the environment.",
               "Load Environment.", MB_OK);
    return 0;
  }

  // Load Configuration files
  if(!ie.LoadConfiguration(fh, cls.RunSettingsFile(),
			   cls.PhysicsSettingsFile(),
			   cls.ECSettingsFile(),
			   cls.InstructionSetSettingsFile())) {
    MessageBox(NULL,
               "ERROR: Failed to load configuration files.",
               "Initialization.", MB_OK);
    return 0;
  }

  // Process command line
  // Command line values override configuration values, so we process those
  // last.
  if(!cls.AddRules(ie.GetPhysicsSettings().Rules()) ||
     !cls.AddRules(ie.GetRunSettings().Rules()) ||
     !cls.AddRules(ie.GetECSettings().Rules())) {
    MessageBox(NULL,
               "Could not get rules to parse command line.",
               "Initialization.", MB_OK);
    return 0;
  }

#if 0
  if(!cls.ProcessSets(argc, argv)) {
    MessageBox(NULL,
               "Could not process command line.",
               "Initialization.", MB_OK);
    cerr <<  << endl;
    return 0;
  }
#endif

  ie.GetRunSettings().PopulationSize() = 1;
  ie.GetRunSettings().StartState() = 3;

  if(!ie.InitializeExperiment(fh)) {
    MessageBox(NULL,
               "ERROR: Unable to initialize the experiment.",
               "Initialization.", MB_OK);
    return 0;
  }

  // Create Main Window
  char className[] = "WhirlingDervishPlay";
  char caption[]   = "WhirlingDervishPlay";

  WNDCLASS _class;
  _class.style = 0;
  _class.lpfnWndProc = MainWindowProcedure; // window procedure: mandatory
  _class.cbClsExtra = 0;
  _class.cbWndExtra = 0;
  _class.hInstance = hInst;         // owner of the class: mandatory
  _class.hIcon = 0;
  _class.hCursor = LoadCursor (0, IDC_ARROW); // optional
  _class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // optional
  _class.lpszMenuName = 0;
  _class.lpszClassName = (LPCSTR)className; // mandatory
  RegisterClass(&_class);

  HWND _hwnd;
  _hwnd = CreateWindow(
    (LPCSTR)className,            // name of a registered window class
    (LPCSTR)caption,              // window caption
	  WS_OVERLAPPEDWINDOW,  // window style
    CW_USEDEFAULT,        // x position
    CW_USEDEFAULT,        // y position
    CW_USEDEFAULT,        // witdh
    CW_USEDEFAULT,        // height
    0,                    // handle to parent window
    0,                    // handle to menu
    hInst,                // application instance
    0);                   // window creation data

  MainWindow mw(ie, fh, _hwnd, hInst);
  g_mw = &mw;
  //mw.setGeometry(100, 100, 550, 600);
  //mw.show();
  EnableWindow(_hwnd, true);
  ShowWindow(_hwnd, cmdShow);
  UpdateWindow(_hwnd);

  // Create OpenGL Context
  PIXELFORMATDESCRIPTOR pfd;
  int iFormat;

  // get the device context (DC)
  g_hdc = GetDC(_hwnd);

  // set the pixel format for the DC
  ZeroMemory(&pfd, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;
  iFormat = ChoosePixelFormat(g_hdc, &pfd);
  SetPixelFormat(g_hdc, iFormat, &pfd);

  // create and enable the render context (RC)
  HGLRC hRC = wglCreateContext(g_hdc);
  wglMakeCurrent(g_hdc, hRC);

  // Create Timer
  SetTimer(_hwnd, 1, 10, NULL);

  MSG  msg;
  int status;

  while((status = GetMessage (& msg, 0, 0, 0)) != 0) {
    if(status == -1) {
        return 0;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  KillTimer(_hwnd, 1);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(_hwnd, g_hdc);

  return (int)msg.wParam;
}

LRESULT CALLBACK MainWindowProcedure(HWND hwnd, unsigned int message,
                                     WPARAM wParam, LPARAM lParam)
{
  switch(message) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_TIMER:
    g_mw->UpdateSystem();
    break;
  case WM_CHAR:
    switch((char)wParam) {
    case 'a':
    case 'A':
      g_mw->GetiJGL().MoveLeft();
      break;
    case 's':
    case 'S':
      g_mw->GetiJGL().MoveForward();
      break;
    case 'd':
    case 'D':
      g_mw->GetiJGL().MoveRight();
      break;
    case 'x':
    case 'X':
      g_mw->GetiJGL().MoveBackward();
      break;
    case 'z':
    case 'Z':
      g_mw->GetiJGL().YawLeft();
      break;
    case 'c':
    case 'C':
      g_mw->GetiJGL().YawRight();
      break;
    case 'f':
    case 'F':
      g_mw->GetiJGL().PitchUp();
      break;
    case 'v':
    case 'V':
      g_mw->GetiJGL().PitchDown();
      break;
    case 'o':
    case 'O':
      g_mw->NormalView();
      break;
    case 'p':
    case 'P':
      g_mw->DownView();
      break;
    case 'g':
    case 'G':
      g_mw->GetiJGL().ToggleGrid();
      break;
    }
    break;
  case WM_COMMAND:
    switch(LOWORD(wParam))
    {
    case 101: // Open
      g_mw->Open();
      break;
    case 102: // Save
      g_mw->Save();
      break;
    case 103: // Save As
      g_mw->SaveAs();
      break;
    case 105: // Quit
      PostQuitMessage (0);
      return 0;
    case 201: // Freeze
      g_mw->Freeze();
      break;
    case 202: // Reset
      g_mw->Reset();
      break;
    case 203: // Edit
      g_mw->Edit();
      break;
    case 204: // Clear
      g_mw->Clear();
      break;
    case 301: // Normal
      g_mw->NormalView();
      break;
    case 302: // Down
      g_mw->DownView();
      break;
    case 401: // Help
      g_mw->About();
      break;
    default:
      MessageBox(NULL, "Command not found.", "Command not found.", MB_OK);
      break;
    }
  }

  return DefWindowProc(hwnd, message, wParam, lParam );
}
