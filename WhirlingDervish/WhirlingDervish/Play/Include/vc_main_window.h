#ifndef __VC_MAIN_WINDOW_H__
#define __VC_MAIN_WINDOW_H__

#include <windows.h>
#include <string>

#include "defs.h"
#include "vc_ijgl.h"
//#include "vc_key_binding.h"
#include "vc_editor.h"
#include "interactive_environment.h"
#include "file_handler.h"

class MainWindow {
public:
  MainWindow(InteractiveEnvironment &, FileHandler &, HWND, HINSTANCE);
  ~MainWindow(void);

  void setGeometry(int, int, int, int);
  iJGL &GetiJGL(void) { return d_ijgl; }
  //void show(void);
protected:
  //void MouseMoveEvent(QMouseEvent *e);
private:
  InteractiveEnvironment &d_ie;
  FileHandler &d_fh;

  HWND d_hwnd;

  HMENU d_menu;         // QMenuBar d_menu;
  HMENU d_fileMenu;     // QPopupMenu d_fileMenu;
  HMENU d_creatureMenu; // QPopupMenu d_creatureMenu;
  HMENU d_viewMenu;     // QPopupMenu d_viewMenu;
  HMENU d_helpMenu;     // QPopupMenu d_helpMenu;

  std::string d_fileName;

  double d_gravityX;
  double d_gravityY;
  double d_gravityZ;
  double d_dragCoefficient;
  double d_timestep;

  CreatureInfo d_tempCreature;

  int64 d_creatureId;
  int d_freezeItemLoc;
  bool d_frozen;

  int d_graphics;

  iJGL d_ijgl;
  Editor d_editor;

#if 0
  KeyBinding d_bindings;

  QTimer d_timer;

  QVBoxLayout d_layout;

  QAccel d_accel;
  QGridLayout d_dataLayout;
  QLabel d_gravityLabel;
  QLabel d_dragLabel;
  QLabel d_timeLabel;
  QLineEdit d_gravityXEdit;
  QLineEdit d_gravityYEdit;
  QLineEdit d_gravityZEdit;
  QLineEdit d_dragEdit;
  QLineEdit d_timeEdit;

  QHBoxLayout d_infoLayout;
  QLabel d_heightLabel;
#endif
  //QLabel d_heightData;

public:
  // File
  void Open(void);
  void Save(void);
  void SaveAs(void);

  // Creature
  void Freeze(void);
  void Reset(void);
  void Clear(void);
  void Edit(void);

  // View
  void NormalView(void);
  void DownView(void);

  // Help
  void About(void);

  void Update(const std::string &);
  void UpdateSystem(void);

#if 0
  void ChangeGravityX(void);
  void ChangeGravityY(void);
  void ChangeGravityZ(void);
  void ChangeDrag(void);
  void ChangeTimestep(void);
#endif

  void DisplayHeight(void);
};

#endif
