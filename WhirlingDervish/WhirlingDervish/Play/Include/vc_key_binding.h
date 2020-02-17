#ifndef __VC_KEY_BINDING_H__
#define __VC_KEY_BINDING_H__

#include <fstream>
#include <vector>

#include "bindingform.h"

// file modified by Sherri Goings 7/1/03
// file modified by Jason Stredwick 7/20/2004

class iJGL;
class MainWindow;

class KeyBinding {
public:
  KeyBinding(QAccel *, iJGL *, MainWindow *);
  ~KeyBinding(void);

public slots:
  void SaveBindings(void);
  void LoadBindings();

  // Change or update settings
  void Change(void);
  void Apply(void);
  void Cancel(void);
  void Done(void);

  // Get change in key binding
  void SystemGroup(int which);
  void MouseGroup(int which);
  void EyepointGroup(int which);

protected:
  void keyPressEvent(QKeyEvent *event);
  //void keyReleaseEvent(QKeyEvent *event);

private:
  enum { // System Group
         QUIT, ABOUT, GRID,

	 // Mouse Group
	 MOUSE_TOGGLE, FREE_LOOK,

	 // Eyepoint Group
	 FORWARD, BACKWARD, LEFT, RIGHT, TURN_LEFT, TURN_RIGHT, 
	 UP, DOWN, PITCH_UP, PITCH_DOWN,

	 BINDING_COUNT
  };

  MainWindow *d_mw;
  iJGL *d_jgl;

  int bindingCount;

  QKeySequence *keyBindings;
  QKeySequence *tempBindings;
  std::vector<QLabel*> bindingLabels;
  char **comments;
  bool d_acceptKey;
  int d_currentIndex;
  QAccel *d_accel;

  void Bind(void);
  void unBind(void);
  void SetDefaults(void);
  QKeySequence *GetKeySequence(void);
};


#endif
