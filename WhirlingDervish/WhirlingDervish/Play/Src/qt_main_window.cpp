#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

#include "qt_main_window.h"

#include "jvector.h"
#include "parser.h"
#include "creature_info.h"
#include "creature.h"
#include "body.h"
#include "physical_point_info.h"
#include "physical_point.h"

using namespace std;

MainWindow::MainWindow(InteractiveEnvironment &ie, FileHandler &fh,
		       QWidget *, const char *)
  : d_ie(ie),
    d_fh(fh),
    d_layout(this),
    d_menu(this, "MenuBar"),
    d_fileMenu(&d_menu),
    d_creatureMenu(&d_menu),
    d_viewMenu(&d_menu),
    d_helpMenu(&d_menu),
    d_accel(this),
    d_ijgl(d_ie.GetPS(), this, "iJGL"),
    d_editor(this, "Editor", true),
    d_bindings(&d_accel, &d_ijgl, this, this, "Key Bindings"),
    d_timer(this),
    d_dataLayout(1, 8, 5, "Data Layout"),
    d_gravityLabel("Gravity: ", this),
    d_dragLabel("Coef. of Drag: ", this),
    d_timeLabel("Time step: ", this),
    d_gravityXEdit("0", this, "Gravity X Edit"),
    d_gravityYEdit("0", this, "Gravity Y Edit"),
    d_gravityZEdit("0", this, "Gravity Z Edit"),
    d_dragEdit("0", this, "Drag Edit"),
    d_timeEdit("0", this, "Time Edit"),
    d_gravityX(0.0),
    d_gravityY(0.0),
    d_gravityZ(0.0),
    d_dragCoefficient(0.0),
    d_timestep(0.0),
    d_heightLabel("Max. Height:", this),
    d_heightData("0.0", this),
    d_creatureId(-1),
    d_freezeItemLoc(-1),
    d_frozen(false),
    d_graphics(0) {
  d_fileName = "";

  d_layout.addWidget(&d_menu);
#if 1
  d_layout.addWidget(&d_ijgl);
  d_layout.addLayout(&d_dataLayout);
  d_layout.addLayout(&d_infoLayout);
#endif

  d_fileMenu.insertItem("Open", this, SLOT(Open()));
  d_fileMenu.insertItem("Save", this, SLOT(Save()));
  d_fileMenu.insertItem("Save As", this, SLOT(SaveAs()));
  d_fileMenu.insertSeparator();
  d_fileMenu.insertItem("Key bindings", &d_bindings, SLOT(exec()));
  d_fileMenu.insertItem("Exit", qApp, SLOT(quit()));

  d_freezeItemLoc = d_creatureMenu.insertItem("Freeze", this, SLOT(Freeze()));
  d_creatureMenu.insertItem("Reset", this, SLOT(Reset()));
  d_creatureMenu.insertItem("Edit", this, SLOT(Edit()));
  d_creatureMenu.insertItem("Clear", this, SLOT(Clear()));

  d_viewMenu.insertItem("Normal", this, SLOT(NormalView()));
  d_viewMenu.insertItem("Down", this, SLOT(DownView()));

  d_helpMenu.insertItem("About", this, SLOT(About()));

  d_menu.insertItem("File", &d_fileMenu);
  d_menu.insertItem("Creature", &d_creatureMenu);
  d_menu.insertItem("View", &d_viewMenu);
  d_menu.insertItem("Help", &d_helpMenu);

#if 1
  d_dataLayout.addWidget(&d_gravityLabel, 0, 0, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_gravityXEdit, 0, 1);
  d_dataLayout.addWidget(&d_gravityYEdit, 0, 2);
  d_dataLayout.addWidget(&d_gravityZEdit, 0, 3);
  d_dataLayout.addWidget(&d_dragLabel, 0, 4, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_dragEdit, 0, 5);
  d_dataLayout.addWidget(&d_timeLabel, 0, 6, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_timeEdit, 0, 7);

  QString data;
  data.setNum(d_ie.GravityX());
  d_gravityXEdit.setText(data);
  data.setNum(d_ie.GravityY());
  d_gravityYEdit.setText(data);
  data.setNum(d_ie.GravityZ());
  d_gravityZEdit.setText(data);
  data.setNum(d_ie.Drag());
  d_dragEdit.setText(data);
  data.setNum(d_ie.Timestep());
  d_timeEdit.setText(data);

  connect(&d_gravityXEdit, SIGNAL(returnPressed()),
	  this, SLOT(ChangeGravityX()));
  connect(&d_gravityYEdit, SIGNAL(returnPressed()),
	  this, SLOT(ChangeGravityY()));
  connect(&d_gravityZEdit, SIGNAL(returnPressed()),
	  this, SLOT(ChangeGravityZ()));
  connect(&d_dragEdit, SIGNAL(returnPressed()),
	  this, SLOT(ChangeDrag()));
  connect(&d_timeEdit, SIGNAL(returnPressed()),
	  this, SLOT(ChangeTimestep()));

  d_infoLayout.addWidget(&d_heightLabel);
  d_infoLayout.addWidget(&d_heightData);

  // Create an empty creature
  if(!d_ie.InsertCreature(d_creatureId, "")) {
    cerr << "ERROR: Failed to creature an empty creature." << endl;
    qApp->quit();
    return;
  }
  d_ie.SelectCreature(d_creatureId);
#endif

  connect(&d_editor, SIGNAL(Text(QString)), this, SLOT(Update(QString)));

  connect(&d_timer, SIGNAL(timeout()), this, SLOT(UpdateSystem()));
  d_timer.start(10, FALSE);

  return;
}

void MainWindow::MouseMoveEvent(QMouseEvent *e) {
#if 0
  cout << "HI:" << endl;
  cout << "X: " << e->globalX() << "  Y: "
       << e->globalY() << endl;
#endif

  return;
}

MainWindow::~MainWindow(void) {
  d_timer.stop();
  d_ie.Shutdown(d_fh);

  return;
}

// File
void MainWindow::Open(void) {
  QString filename =
    QFileDialog::getOpenFileName(QString::null, "Creatures (*.cre)", this);

  if(filename.isEmpty() || filename.isNull()) {
    return;
  }

  d_fileName = filename.latin1();

  int fileId = d_fh.Create(d_fileName, ios::in);
  if(fileId < 0) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: File doesn't exist, or failed to open." );
  }

  string str("");
  if(!d_fh.InputCreature(fileId, d_tempCreature, str)) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Failed to load creature." );
  }

  d_fh.Close(fileId);

  if(!d_ie.AlterSelected(str, d_tempCreature.Fitness())) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Failed to alter creature." );
  } else {
    QString t("WhirlingDervishPlay   ");
    t.append(filename);
    setCaption(t);
  }

  return;
}

void MainWindow::Save(void) {
  if(d_fileName == "") {
    SaveAs();
    return;
  }

  int fileId = d_fh.Create(d_fileName, ios::out);
  if(fileId < 0) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: File doesn't exist, or failed to open." );
  }

  if(!d_ie.FillSelectedInfo(d_tempCreature)) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Get a copy of the selected creature." );
  }

  if(!d_fh.OutputCreature(fileId, d_tempCreature)) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Failed to save creature." );
  }

  d_fh.Close(fileId);

  return;
}

void MainWindow::SaveAs(void) {
  QString filename =
    QFileDialog::getSaveFileName(QString::null, "Creatures (*.cre)", this);

  if(filename.isEmpty() || filename.isNull()) {
    return;
  }

  d_fileName = filename.latin1();

  Save();

  return;
}

// Creature
void MainWindow::Freeze(void) {
  if(d_frozen) {
    d_frozen = false;
    d_creatureMenu.changeItem(d_freezeItemLoc, "Freeze");
    d_ie.FreezeSelected();
  } else {
    d_frozen = true;
    d_creatureMenu.changeItem(d_freezeItemLoc, "UnFreeze");
    d_ie.UnfreezeSelected();
  }

  return;
}

void MainWindow::Reset(void) {
  d_ie.ResetSelected();

  return;
}

void MainWindow::Clear(void) {
  d_ie.AlterSelected("");

  return;
}

void MainWindow::Edit(void) {
  stringstream ss;
  if(!d_ie.GenomeSelected(ss)) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Couldn't get genome." );
  }

  d_editor.SetText(ss.str());
  d_editor.show();

  return;
}

//View
void MainWindow::NormalView(void) {
  d_ijgl.NormalView();

  return;
}

void MainWindow::DownView(void) {
  d_ijgl.DownView();

  return;
}

// Help
void MainWindow::About(void) {
  QMessageBox::information(0, "Whirling Dervish Play", "Have Fun!" );

  return;
}

void MainWindow::Update(QString s) {
  string str(s.latin1());

  Parser &p = d_ie.GetParser();

  vector<int> errors;
  if(!p.Validate(errors, str)) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Invalid code!" );
    return;
  }

  if(!d_ie.AlterSelected(str)) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Failed to alter creature!" );
  }

  return;
}

void MainWindow::UpdateSystem(void) {
  if(d_graphics == 6) {
    d_graphics = 0;
    d_ijgl.updateGL();
  } else {
    d_graphics++;
  }

  d_ie.Process(d_ie.Timestep());

  DisplayHeight();

  return;
}

void MainWindow::ChangeGravityX(void) {
  QString s = d_gravityXEdit.text();
  bool ok = false;
  double d = s.toDouble(&ok);
  if(!ok) { return; }

  d_ie.GravityX() = d;

  return;
}

void MainWindow::ChangeGravityY(void) {
  QString s = d_gravityYEdit.text();
  bool ok = false;
  double d = s.toDouble(&ok);
  if(!ok) { return; }

  d_ie.GravityY() = d;

  return;
}

void MainWindow::ChangeGravityZ(void) {
  QString s = d_gravityZEdit.text();
  bool ok = false;
  double d = s.toDouble(&ok);
  if(!ok) { return; }

  d_ie.GravityZ() = d;

  return;
}

void MainWindow::ChangeDrag(void) {
  QString s = d_dragEdit.text();
  bool ok = false;
  double d = s.toDouble(&ok);
  if(!ok) { return; }

  d_ie.Drag() = d;

  return;
}

void MainWindow::ChangeTimestep(void) {
  QString s = d_timeEdit.text();
  bool ok = false;
  double d = s.toDouble(&ok);
  if(!ok) { return; }

  d_ie.Timestep() = d;

  return;
}

void MainWindow::DisplayHeight(void) {
  double height = 0.0;

  iPopulationStorage &ps = d_ie.GetPS();
  const vector<CreatureInfo *> &vci = ps.Population();
  if(vci.size() > 0 && vci[0] != NULL) {
    Body &b = vci[0]->GetCreature().GetBody();
    const map<int64, PhysicalPointInfo *> &points = b.GetPoints();
    map<int64, PhysicalPointInfo *>::const_iterator ipoints;
    for(ipoints=points.begin(); ipoints!=points.end(); ++ipoints) {
      if(ipoints->second != NULL) {
	const PhysicalPoint &p = ipoints->second->GetPhysicalPoint();
	if(p.Position()[1] > height) {
	  height = p.Position()[1];
	}
      }
    }
  }

  d_heightData.setNum(height);

  return;
}
