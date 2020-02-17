#ifndef __QT_MAIN_WINDOW_H__
#define __QT_MAIN_WINDOW_H__

#include <qwidget.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qlabel.h>
#include <qaccel.h>

#include "qt_ijgl.h"
#include "qt_editor.h"
#include "qt_key_binding.h"
#include "interactive_environment.h"

class FileHandler;

class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow(InteractiveEnvironment &, FileHandler &,
	     QWidget * = 0, const char * = 0);
  ~MainWindow(void);

private:
  InteractiveEnvironment &d_ie;

  QVBoxLayout d_layout;

  QMenuBar d_menu;
  QPopupMenu d_fileMenu;
  QPopupMenu d_viewMenu;
  QPopupMenu d_helpMenu;
  QAccel d_accel;
  iJGL d_ijgl;
  Editor d_editor;
  KeyBinding d_bindings;

  QGridLayout d_dataLayout;

  QLabel d_generationLabel,  d_generationData;
  QLabel d_currentTimeLabel, d_currentTimeData;
  QLabel d_simTimeLabel,     d_simTimeData;
  QLabel d_avgFitnessLabel,  d_avgFitnessData;
  QLabel d_avgPointsLabel,   d_avgPointsData;
  QLabel d_avgSpringsLabel,  d_avgSpringsData;
  QLabel d_maxFitnessLabel,  d_maxFitnessData;
  QLabel d_maxHeightLabel,   d_maxHeightData;
  QLabel d_avgHeightLabel,   d_avgHeightData;

  QTimer d_timer;

  int d_graphics;

  FileHandler &d_fh;
  int d_done;

public slots:
  // File
  void Pause(void);
  void ViewGenome(void);

  // View
  void NormalView(void);
  void DownView(void);

  // Help
  void About(void);

  void UpdateSystem(void);
};

#endif
