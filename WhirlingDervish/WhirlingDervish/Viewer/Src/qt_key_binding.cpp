// modified by Sherri Goings 7/1/03
// fixed keybinding so works, can use combinations of Ctrl, Shift, and 
// standard keys Alt does not bind

// modified by Jason Stredwick 7/20/2004
// changing available bindings

#include <qapplication.h>

#include <iostream>
#include <string.h>

#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qevent.h>
#include <qkeysequence.h>
#include <qlabel.h>
#include <qfile.h>
#include <qtextstream.h>

#include "qt_key_binding.h"

#include "qt_ijgl.h"
#include "qt_main_window.h"

using namespace std;

KeyBinding::KeyBinding(QAccel *accel, iJGL *jgl, MainWindow *mw,
		       QWidget *parent, const char *name)
  : BindingForm(parent, name), d_mw(mw), d_jgl(jgl), bindingCount(0),
    keyBindings(NULL), tempBindings(NULL), comments(NULL), d_acceptKey(false),
    d_currentIndex(-1), d_accel(NULL) {
  d_accel = accel;

  bindingCount = BINDING_COUNT;
  keyBindings = new QKeySequence[bindingCount];
  tempBindings = new QKeySequence[bindingCount];
  comments = new char*[bindingCount];
  for(int i=0; i<bindingCount; i++) {
    comments[i] = new char[256];
  }

  // push all labels into a vector for easy reference later
  bindingLabels.push_back(QuitKey);
  bindingLabels.push_back(HelpKey);
  bindingLabels.push_back(GridKey);
  //bindingLabels.push_back(PauseKey);
  bindingLabels.push_back(MouseToggleKey);
  bindingLabels.push_back(FreeLookKey);
  bindingLabels.push_back(MoveForwardKey);
  bindingLabels.push_back(MoveBackwardKey);
  bindingLabels.push_back(MoveLeftKey);
  bindingLabels.push_back(MoveRightKey);
  bindingLabels.push_back(TurnLeftKey);
  bindingLabels.push_back(TurnRightKey);
  bindingLabels.push_back(MoveUpKey);
  bindingLabels.push_back(MoveDownKey);
  bindingLabels.push_back(PitchUpKey);
  bindingLabels.push_back(PitchDownKey);

  fstream fp;
  fp.open("Storage/bindings.txt", ios::in);
  if(!fp) {
    fp.open("Storage/bindings.default.txt", ios::in);
    if(!fp) {
      SetDefaults();
    } else {
      LoadBindings();
      fp.close();
    }
  } else {
    LoadBindings();
    fp.close();
  }

  d_acceptKey = false;
  d_currentIndex = -1;

  Bind();
  Change();
  
  connect(SaveDefaultsButton, SIGNAL(clicked()), this, SLOT(SaveBindings()));
  connect(LoadDefaultsButton, SIGNAL(clicked()), this, SLOT(LoadBindings()));
  connect(ApplyButton, SIGNAL(clicked()), this, SLOT(Apply()));
  connect(DoneButton, SIGNAL(clicked()), this, SLOT(Done()));
  connect(CancelButton, SIGNAL(clicked()), this, SLOT(Cancel()));
  connect(SystemButtonGroup, SIGNAL(clicked(int)),this,SLOT(SystemGroup(int)));
  connect(MouseButtonGroup, SIGNAL(clicked(int)), this,SLOT(MouseGroup(int)));
  connect(EyepointButtonGroup, SIGNAL(clicked(int)), 
	  this, SLOT(EyepointGroup(int)));

  return;
}

KeyBinding::~KeyBinding(void) {
  if(keyBindings != NULL) {
    delete [] keyBindings;
  }
  if(tempBindings != NULL) {
    delete [] tempBindings;
  }
  if(comments != NULL) {
    for(int i=0; i<bindingCount; i++) {
      if(comments[i] != NULL) {
	delete comments[i];
      }
    }
    delete [] comments;
  }

  return;
}

void KeyBinding::Change(void) {
  for(int i=0; i<bindingCount; i++) {
    tempBindings[i] = keyBindings[i];
  }
  return;
}

void KeyBinding::Apply(void) {
  for(int i=0; i<bindingCount; i++) {
    keyBindings[i] = tempBindings[i];
  }

  Bind();

  return;
}

void KeyBinding::Cancel(void) {
   Change();
   for(int i=0; i<bindingCount; i++) {
     keyBindings[i] = tempBindings[i];
     bindingLabels.at(i)->setText(QString(keyBindings[i]));
   }
  return;
}

void KeyBinding::Done(void) {
  for(int i=0; i<bindingCount; i++) {
    keyBindings[i] = tempBindings[i];
  }

  Bind();

  return;
}

void KeyBinding::SystemGroup(int which) {
  d_currentIndex = QUIT + which;
  d_acceptKey = true;
  
  return;
}

void KeyBinding::MouseGroup(int which) {
  d_currentIndex = MOUSE_TOGGLE + which;
  d_acceptKey = true;

  return;
}

void KeyBinding::EyepointGroup(int which) {
  d_currentIndex = FORWARD + which;
  d_acceptKey = true;

  return;
}

void KeyBinding::Bind(void) {
  d_accel->clear();

  for(int i=0; i<bindingCount; i++) {
    d_accel->setItemEnabled(d_accel->insertItem(keyBindings[i]), TRUE);
  }
  d_accel->setItemEnabled(d_accel->insertItem(QKeySequence(tr("O", "Normal View"))), TRUE);
  d_accel->setItemEnabled(d_accel->insertItem(QKeySequence(tr("P", "Down View"))), TRUE);


  // System Group
  d_accel->connectItem(d_accel->findKey(keyBindings[QUIT]),
		       qApp, SLOT(quit()));
  d_accel->connectItem(d_accel->findKey(keyBindings[ABOUT]),
		       d_mw, SLOT(About()));
  d_accel->connectItem(d_accel->findKey(keyBindings[GRID]), 
		       d_jgl, SLOT(ToggleGrid()));
  //d_accel->connectItem(d_accel->findKey(keyBindings[PAUSE]), 
  //		       d_mw, SLOT(Pause()));

  // Mouse Group
#if 0
  d_accel->connectItem(d_accel->findKey(keyBindings[MOUSE_TOGGLE]), 
		       d_gui, SLOT(ToggleMouseMovement()));
  d_accel->connectItem(d_accel->findKey(keyBindings[FREE_LOOK]),
		       d_jgl, SLOT(ToggleView()));
#endif

  // Eyepoint Group
  d_accel->connectItem(d_accel->findKey(keyBindings[FORWARD]),
		       d_jgl, SLOT(MoveForward()));
  d_accel->connectItem(d_accel->findKey(keyBindings[BACKWARD]),
		       d_jgl, SLOT(MoveBackward()));
  d_accel->connectItem(d_accel->findKey(keyBindings[LEFT]),
		       d_jgl, SLOT(MoveLeft()));
  d_accel->connectItem(d_accel->findKey(keyBindings[RIGHT]),
		       d_jgl, SLOT(MoveRight()));
  d_accel->connectItem(d_accel->findKey(keyBindings[TURN_LEFT]),
		       d_jgl, SLOT(YawLeft()));
  d_accel->connectItem(d_accel->findKey(keyBindings[TURN_RIGHT]),
		       d_jgl, SLOT(YawRight()));
  d_accel->connectItem(d_accel->findKey(keyBindings[DOWN]),
		       d_jgl, SLOT(MoveDown()));
  d_accel->connectItem(d_accel->findKey(keyBindings[UP]),
		       d_jgl, SLOT(MoveUp()));
  d_accel->connectItem(d_accel->findKey(keyBindings[PITCH_DOWN]),
		       d_jgl, SLOT(PitchDown()));
  d_accel->connectItem(d_accel->findKey(keyBindings[PITCH_UP]),
		       d_jgl, SLOT(PitchUp()));

  d_accel->connectItem(d_accel->findKey(QKeySequence(tr("O", "Normal View"))),
		       d_jgl, SLOT(NormalView()));
  d_accel->connectItem(d_accel->findKey(QKeySequence(tr("P", "Down View"))),
		       d_jgl, SLOT(DownView()));

  return;
}

void KeyBinding::SaveBindings(void) {
 
  QFile bindingsFile("Storage/bindings.txt");
  bindingsFile.open(IO_WriteOnly);
  
  if(!bindingsFile.isOpen()) { return; } 
  
  QTextStream stream(&bindingsFile);

  for(int i=0; i<bindingCount; i++) {
    stream << (QString)keyBindings[i] << "\t" << comments[i] << endl;
  }

  bindingsFile.close();
  return;
}

void KeyBinding::LoadBindings() {
  QFile bindingsFile( "Storage/bindings.txt" );

  bindingsFile.open(IO_ReadOnly);
  
  if (!bindingsFile.isOpen()) { 
    bindingsFile.setName( "Storage/bindings.default.txt" );
    bindingsFile.open(IO_ReadOnly);
    if (!bindingsFile.isOpen()) { SetDefaults(); return; }
  }

  QTextStream stream(&bindingsFile);

  for (int i=0; i<bindingCount && !stream.atEnd(); i++) 
  {
    QString binding, comment;
    stream >> binding;
    comment = stream.readLine();
    
    if(strcmp(binding, "UNDEF") == 0) 
    {
      //tempBindings[i] = QKeySequence(Key_unknown);
      keyBindings[i] = QKeySequence(Key_unknown);
      bindingLabels.at(i)->setText("Key_unknown");
    } 
    else 
    {
      keyBindings[i] = QKeySequence(tr(binding, comment));
      //tempBindings[i] = QKeySequence(tr(binding, comment));
      bindingLabels.at(i)->setText(binding);
    }
    strcpy(comments[i], comment);
  }
  
  bindingsFile.close();
  d_currentIndex = -1;

  return;
}

void KeyBinding::SetDefaults(void) {
  keyBindings[QUIT] = QKeySequence(tr("Ctrl+Q", "File|Quit"));
  keyBindings[ABOUT] = QKeySequence(tr("Ctrl+H", "Help|About"));
  keyBindings[GRID] = QKeySequence(tr("Ctrl+G", "Grid"));
  //keyBindings[PAUSE] = QKeySequence(tr("P", "Pause"));
  keyBindings[MOUSE_TOGGLE] = QKeySequence(tr("M", "Mouse Toggle"));
  keyBindings[FREE_LOOK] = QKeySequence(tr("N", "Freelook"));
  keyBindings[FORWARD] = QKeySequence(tr("S", "Move Forward"));
  keyBindings[BACKWARD] = QKeySequence(tr("X", "Move Backward"));
  keyBindings[LEFT] = QKeySequence(tr("A", "Move Left"));
  keyBindings[RIGHT] = QKeySequence(tr("D", "Move Right"));
  keyBindings[TURN_LEFT] = QKeySequence(tr("Z", "Turn Left"));
  keyBindings[TURN_RIGHT] = QKeySequence(tr("C", "Turn Right"));
  keyBindings[UP] = QKeySequence(tr("Q", "Move Up"));
  keyBindings[DOWN] = QKeySequence(tr("E", "Move Down"));
  keyBindings[PITCH_UP] = QKeySequence(tr("F", "Pitch Up"));
  keyBindings[PITCH_DOWN] = QKeySequence(tr("V", "Pitch Down"));

  bindingLabels.at(QUIT)->setText("Ctrl+Q");
  bindingLabels.at(ABOUT)->setText("Ctrl+H");
  bindingLabels.at(GRID)->setText("Ctrl+G");
  //bindingLabels.at(PAUSE)->setText("P");
  bindingLabels.at(MOUSE_TOGGLE)->setText("M");
  bindingLabels.at(FREE_LOOK)->setText("N");
  bindingLabels.at(FORWARD)->setText("S");
  bindingLabels.at(BACKWARD)->setText("X");
  bindingLabels.at(LEFT)->setText("A");
  bindingLabels.at(RIGHT)->setText("D");
  bindingLabels.at(TURN_LEFT)->setText("Z");
  bindingLabels.at(TURN_RIGHT)->setText("C");
  bindingLabels.at(UP)->setText("Q");
  bindingLabels.at(DOWN)->setText("E");
  bindingLabels.at(PITCH_UP)->setText("F");
  bindingLabels.at(PITCH_DOWN)->setText("C");

  strcpy(comments[QUIT], "File|Quit");
  strcpy(comments[ABOUT], "Help|About");
  strcpy(comments[GRID], "Grid");
  //strcpy(comments[PAUSE], "Pause");
  strcpy(comments[MOUSE_TOGGLE], "Mouse Toggle");
  strcpy(comments[FREE_LOOK], "Freelook");
  strcpy(comments[FORWARD], "Move Forward");
  strcpy(comments[BACKWARD], "Move Backward");
  strcpy(comments[LEFT], "Move Left");
  strcpy(comments[RIGHT], "Move Right");
  strcpy(comments[TURN_LEFT], "Turn Left");
  strcpy(comments[TURN_RIGHT], "Turn Right");
  strcpy(comments[UP], "Move Up");
  strcpy(comments[DOWN], "Move Down");
  strcpy(comments[PITCH_UP], "Pitch Up");
  strcpy(comments[PITCH_DOWN], "Pitch Down");

  return;
}


void KeyBinding::keyPressEvent(QKeyEvent *event) {
  /**
   Check if a function has been selected to change its Key binding is checked. 
  */

  if(d_currentIndex < 0) {
    return;
  }
  if(event->key()==0) {
    return;
  }
  if(event->isAutoRepeat()) {
    return;
  }

  if(event->ascii()!=0) {
    // put together string using state and key of keyPressEvent
    QString keySeq = "";
    if (event->state()&256)
      keySeq += "Shift+";
    if (event->state()&512)
      keySeq += "Ctrl+";
    keySeq += char(event->key());
    tempBindings[d_currentIndex] = 
      QKeySequence(tr(keySeq,comments[d_currentIndex]));
    bindingLabels.at(d_currentIndex)->setText(keySeq);

    for(int i=0; i<bindingCount; i++) {
      if(tempBindings[d_currentIndex] == keyBindings[i]) {
	cout<<"ERROR, key code already in use!"<<endl;
	tempBindings[d_currentIndex]=keyBindings[d_currentIndex];
	bindingLabels.at(d_currentIndex)->setText(
					 QString(keyBindings[d_currentIndex]));
      } 	
    }

    d_currentIndex = -1;
    d_acceptKey = false;
  }

  return;
}

