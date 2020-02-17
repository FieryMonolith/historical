#ifndef __QT_MAIN_WINDOW_H__
#define __QT_MAIN_WINDOW_H__

#include <string.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qaccel.h>

#include "defs.h"
#include "qt_ijgl.h"
#include "qt_key_binding.h"
#include "interactive_environment.h"
#include "file_handler.h"
#include "qt_editor.h"

class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow(InteractiveEnvironment &, FileHandler &,
	     QWidget * = 0, const char * = 0);
  ~MainWindow(void);

protected:
  void MouseMoveEvent(QMouseEvent *e);
private:
  InteractiveEnvironment &d_ie;
  FileHandler &d_fh;

  QVBoxLayout d_layout;

  QMenuBar d_menu;
  QPopupMenu d_fileMenu;
  QPopupMenu d_creatureMenu;
  QPopupMenu d_viewMenu;
  QPopupMenu d_helpMenu;
  QAccel d_accel;
  iJGL d_ijgl;
  Editor d_editor;
  KeyBinding d_bindings;

  QTimer d_timer;

  QGridLayout d_dataLayout;
  QLabel d_gravityLabel;
  QLabel d_dragLabel;
  QLabel d_timeLabel;
  QLineEdit d_gravityXEdit;
  QLineEdit d_gravityYEdit;
  QLineEdit d_gravityZEdit;
  QLineEdit d_dragEdit;
  QLineEdit d_timeEdit;
  double d_gravityX;
  double d_gravityY;
  double d_gravityZ;
  double d_dragCoefficient;
  double d_timestep;

  QHBoxLayout d_infoLayout;
  QLabel d_heightLabel;
  QLabel d_heightData;

  std::string d_fileName;

  CreatureInfo d_tempCreature;

  int64 d_creatureId;
  int d_freezeItemLoc;
  bool d_frozen;

  int d_graphics;

public slots:
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

  void Update(QString);
  void UpdateSystem(void);

  void ChangeGravityX(void);
  void ChangeGravityY(void);
  void ChangeGravityZ(void);
  void ChangeDrag(void);
  void ChangeTimestep(void);
  void DisplayHeight(void);
};

#endif
