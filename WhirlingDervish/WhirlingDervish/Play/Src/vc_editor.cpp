#include <windows.h>

#include "vc_main_window.h"
#include "vc_editor.h"

using namespace std;

LRESULT CALLBACK EditorFunction(HWND, UINT, WPARAM, LPARAM);

Editor *g_editor;
HWND g_alterHWND, g_closeHWND, g_textboxHWND;

Editor::Editor(HWND parentHWND, HINSTANCE parentInstance, MainWindow *mw) {
  d_mw = mw;

  // Create Editor Window
  char className[] = "Editor";
  char caption[]   = "Editor";

  WNDCLASS _class;
  _class.style = 0;
  _class.lpfnWndProc = EditorFunction; // window procedure: mandatory
  _class.cbClsExtra = 0;
  _class.cbWndExtra = 0;
  _class.hInstance = parentInstance;   // owner of the class: mandatory
  _class.hIcon = 0;
  _class.hCursor = LoadCursor (0, IDC_ARROW); // optional
  _class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // optional
  _class.lpszMenuName = 0;
  _class.lpszClassName = (LPCSTR)className; // mandatory
  RegisterClass(&_class);

  d_hwnd = CreateWindow(
    (LPCSTR)className,            // name of a registered window class
    (LPCSTR)caption,              // window caption
	  WS_OVERLAPPEDWINDOW & !WS_SYSMENU,  // window style
    CW_USEDEFAULT,        // x position
    CW_USEDEFAULT,        // y position
    400,                  // witdh
    600,                  // height
    parentHWND,           // handle to parent window
    0,                    // handle to menu
    parentInstance,       // application instance
    0);                   // window creation data

    EnableWindow(d_hwnd, true);

    g_editor = this;

    d_close = CreateWindow("BUTTON", "Close",
      WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_TEXT | BS_CENTER | BS_VCENTER,
      50, 500, 100, 50, d_hwnd, 0, parentInstance, 0);
    g_closeHWND = d_close;

    d_alter = CreateWindow("BUTTON", "Alter",
      WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_TEXT | BS_CENTER | BS_VCENTER,
      250, 500, 100, 50, d_hwnd, 0, parentInstance, 0);
    g_alterHWND = d_alter;

    //WNDCLASS wcl;
    //GetClassInfo(parentInstance, "EDIT", &wcl);
    //wcl.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    //wcl.lpszClassName = "WDEDIT";
    //RegisterClass(&wcl);
    d_textbox = CreateWindow("EDIT", "",
      WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL |
      ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL,
      25, 25, 340, 450, d_hwnd, 0, parentInstance, 0);
    //HDC tdc = GetDC(d_textbox);
    //SelectObject(tdc,GetStockObject(GRAY_BRUSH));

#if 0
  setPaletteBackgroundColor(QColor(0, 0, 0));
  setSizeGripEnabled(true);
  resize(400, 600);

  d_vlayout.addWidget(&d_edit);
  d_vlayout.addLayout(&d_hlayout);

  d_hlayout.addWidget(&d_alter);
  d_hlayout.addWidget(&d_close);

  d_edit.setPaletteBackgroundColor(QColor(0, 0, 0));
  d_edit.setPaletteForegroundColor(QColor(0, 255, 0));

  d_alter.setMinimumWidth(100);
  d_alter.setMaximumWidth(100);
  d_close.setMinimumWidth(100);
  d_close.setMaximumWidth(100);

  connect(&d_alter, SIGNAL(clicked()), this, SLOT(Alter()));
  connect(&d_close, SIGNAL(clicked()), this, SLOT(Close()));
#endif

  return;
}

Editor::~Editor(void) {
  DestroyWindow(d_hwnd);

  return;
}

void Editor::show(void) {
  ShowWindow(d_hwnd, SW_SHOW);

  return;
}

void Editor::hide(void) {
  ShowWindow(d_hwnd, SW_HIDE);

  return;
}

void Editor::Alter(void) {
  char txt[10000];
  SendMessage(d_textbox, (UINT)WM_GETTEXT,
              (WPARAM)(SendMessage(d_textbox, (UINT)WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0)+1),
              (LPARAM)txt);
  d_mw->Update(txt);

  return;
}

void Editor::Close(void) {
  hide();

  return;
}

void Editor::SetText(const std::string &s) {
  char sub[3];
  sub[0] = 0x0D;
  sub[1] = 0x0A;
  sub[2] = '\0';
  string tempstring = "";
  for(unsigned int i=0; i<s.length(); i++) {
    if(s[i] == '\n') {
      tempstring += sub;
    } else {
      tempstring += s[i];
    }
  }

  SendMessage(d_textbox, (UINT)EM_SETSEL, (WPARAM)0, (LPARAM)-1);
  SendMessage(d_textbox, (UINT)EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)tempstring.c_str());

  return;
}

LRESULT CALLBACK EditorFunction(HWND hwnd, UINT message, WPARAM wParam,
                                LPARAM lParam) {
  switch(message) {
  case WM_COMMAND:
    if(HIWORD(wParam) == BN_CLICKED) {
      if((HWND)lParam == g_alterHWND) {
        g_editor->Alter();
      } else if((HWND)lParam == g_closeHWND) {
        g_editor->Close();
      }
    }
    break;
  }

  return DefWindowProc(hwnd, message, wParam, lParam );
}
