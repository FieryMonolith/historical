#ifndef __VC_EDITOR_H__
#define __VC_EDITOR_H__

#include <windows.h>
#include <string>

class MainWindow;

class Editor {
public:
  Editor(HWND parentHWND, HINSTANCE parentInstance, MainWindow *);
  ~Editor(void);

  void SetText(const std::string &);
  void show(void);
  void hide(void);
  void Close(void);
  void Alter(void);

private:
//  QVBoxLayout d_vlayout;
//  QHBoxLayout d_hlayout;
//  QMultiLineEdit d_edit;
//  QPushButton d_alter;
//  QPushButton d_close;

  HWND d_close;
  HWND d_alter;
  HWND d_textbox;

  HWND d_hwnd;
  MainWindow *d_mw;
};

#endif
