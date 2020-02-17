#include <windows.h>

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

#include "vc_main_window.h"

#include "jvector.h"
#include "parser.h"
#include "creature_info.h"
#include "creature.h"
#include "body.h"
#include "physical_point_info.h"
#include "physical_point.h"

using namespace std;

MainWindow::MainWindow(InteractiveEnvironment &ie, FileHandler &fh, HWND hwnd,
                       HINSTANCE instance)
  : d_ie(ie),
    d_fh(fh),
    d_hwnd(hwnd),
    //d_layout(this),
    //d_accel(this),
    d_ijgl(d_ie.GetPS()),
    d_editor(hwnd, instance, this),
    //d_bindings(&d_accel, &d_ijgl, this, this, "Key Bindings"),
    //d_timer(this),
    //d_dataLayout(1, 8, 5, "Data Layout"),
    //d_gravityLabel("Gravity: ", this),
    //d_dragLabel("Coef. of Drag: ", this),
    //d_timeLabel("Time step: ", this),
    //d_gravityXEdit("0", this, "Gravity X Edit"),
    //d_gravityYEdit("0", this, "Gravity Y Edit"),
    //d_gravityZEdit("0", this, "Gravity Z Edit"),
    //d_dragEdit("0", this, "Drag Edit"),
    //d_timeEdit("0", this, "Time Edit"),
    //d_gravityX(0.0),
    //d_gravityY(0.0),
    //d_gravityZ(0.0),
    //d_dragCoefficient(0.0),
    d_timestep(0.0),
    //d_heightLabel("Max. Height:", this),
    //d_heightData("0.0", this),
    d_creatureId(-1),
    d_freezeItemLoc(-1),
    d_frozen(false),
    d_graphics(0)
{
  d_fileName = "";

  d_menu = CreateMenu();
  d_fileMenu = CreatePopupMenu();
  d_creatureMenu = CreatePopupMenu();
  d_viewMenu = CreatePopupMenu();
  d_helpMenu = CreatePopupMenu();

  InsertMenu(d_fileMenu, 0, MF_BYPOSITION | MF_STRING, 101, "Open");
  InsertMenu(d_fileMenu, 1, MF_BYPOSITION | MF_STRING, 102, "Save");
  InsertMenu(d_fileMenu, 2, MF_BYPOSITION | MF_STRING, 103, "Save As");
  InsertMenu(d_fileMenu, 3, MF_BYPOSITION | MF_SEPARATOR, 104, 0);
//  InsertMenu("Key bindings", &d_bindings, SLOT(exec()));
  InsertMenu(d_fileMenu, 4, MF_BYPOSITION | MF_STRING, 105, "Exit");

  InsertMenu(d_creatureMenu, 0, MF_BYPOSITION | MF_STRING, 201, "Freeze"); // SLOT(Freeze())
  InsertMenu(d_creatureMenu, 1, MF_BYPOSITION | MF_STRING, 202, "Reset"); // SLOT(Reset())
  InsertMenu(d_creatureMenu, 2, MF_BYPOSITION | MF_STRING, 203, "Edit"); // SLOT(Edit())
  InsertMenu(d_creatureMenu, 3, MF_BYPOSITION | MF_STRING, 204, "Clear"); // SLOT(Clear())

  InsertMenu(d_viewMenu, 0, MF_BYPOSITION | MF_STRING, 301, "Normal"); // SLOT(NormalView())
  InsertMenu(d_viewMenu, 1, MF_BYPOSITION | MF_STRING, 302, "Down"); // SLOT(DownView())

  InsertMenu(d_helpMenu, 0, MF_BYPOSITION | MF_STRING, 401, "About"); // SLOT(About())

  InsertMenu(d_menu, 0, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)d_fileMenu, "File");
  InsertMenu(d_menu, 1, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)d_creatureMenu, "Creature");
  InsertMenu(d_menu, 2, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)d_viewMenu, "View");
  InsertMenu(d_menu, 3, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)d_helpMenu, "Help");

  SetMenu(d_hwnd, d_menu);
  DrawMenuBar(d_hwnd);

#if 0
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

#endif

  // Create an empty creature
  if(!d_ie.InsertCreature(d_creatureId, "")) {
    MessageBox(NULL, "ERROR: Failed to creature an empty creature.",
               "Creating Initial Creature.", MB_OK);
    PostQuitMessage(0);
    return;
  }
  d_ie.SelectCreature(d_creatureId);

#if 0
  connect(&d_editor, SIGNAL(Text(QString)), this, SLOT(Update(QString)));

  connect(&d_timer, SIGNAL(timeout()), this, SLOT(UpdateSystem()));
  d_timer.start(10, FALSE);
#endif

  return;
}

MainWindow::~MainWindow(void) {
//  d_timer.stop();
  d_ie.Shutdown(d_fh);
  DestroyWindow(d_hwnd);

  return;
}

// File
// Fix Function
void MainWindow::Open(void) {
  OPENFILENAME ofn;
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.hInstance = 0;
  char filterStrings[] = "CRE Files\0*.cre\0All Files\0*.*";
  ofn.lpstrFilter = filterStrings;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter = 0;
  ofn.nFilterIndex = 1;
  char fileString[1024] = "";
  ofn.lpstrFile = fileString;
  ofn.nMaxFile = 1024;
  char filenameString[1024] = "";
  ofn.lpstrFileTitle = filenameString;
  ofn.nMaxFileTitle = 1024;
  ofn.lpstrInitialDir = NULL;
  ofn.lpstrTitle = NULL;
  ofn.Flags = OFN_CREATEPROMPT | OFN_LONGNAMES | OFN_PATHMUSTEXIST;
  ofn.nFileOffset = 0;
  ofn.nFileExtension = 0;
  ofn.lpstrDefExt = "cre";
  ofn.lCustData = 0;
  ofn.lpfnHook = NULL;
  ofn.lpTemplateName = NULL;
#if (_WIN32_WINNT >= 0x0500)
  ofn.pvReserved = NULL;
  ofn.dwReserved = 0;
  ofn.FlagsEx = 0;
#endif // (_WIN32_WINNT >= 0x0500)

  int filename = GetOpenFileName(&ofn);

  if(!filename) {
    return;
  }

  d_fileName = ofn.lpstrFile;

  int fileId = d_fh.Create(d_fileName, ios::in);
  if(fileId < 0) {
    MessageBox(NULL, "ERROR: File doesn't exist, or failed to open.",
			         "Whirling Dervish Play", MB_OK);
  }

  string str("");
  if(!d_fh.InputCreature(fileId, d_tempCreature, str)) {
    MessageBox(0, "ERROR: Failed to load creature.",
			         "Whirling Dervish Play", MB_OK);
  }

  d_fh.Close(fileId);

  if(!d_ie.AlterSelected(str, d_tempCreature.Fitness())) {
    MessageBox(NULL, "ERROR: Failed to alter creature.",
			         "Whirling Dervish Play", MB_OK);
  } else {
    string t("WhirlingDervishPlay   ");
    t += d_fileName;
    SetWindowText(d_hwnd, t.c_str());
  }

  return;
}

// Fix Function
void MainWindow::Save(void) {
  if(d_fileName == "") {
    SaveAs();
    return;
  }

  int fileId = d_fh.Create(d_fileName, ios::out);
  if(fileId < 0) {
    MessageBox(NULL, "ERROR: File doesn't exist, or failed to open.",
			         "Whirling Dervish Play", MB_OK);
  }

  if(!d_ie.FillSelectedInfo(d_tempCreature)) {
    MessageBox(NULL, "ERROR: Get a copy of the selected creature.",
			         "Whirling Dervish Play", MB_OK);
  }

  if(!d_fh.OutputCreature(fileId, d_tempCreature)) {
    MessageBox(NULL, "ERROR: Failed to save creature.",
			         "Whirling Dervish Play", MB_OK);
  }

  d_fh.Close(fileId);

  return;
}

// FIX Function
void MainWindow::SaveAs(void) {
  OPENFILENAME ofn;
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.hInstance = 0;
  char filterStrings[] = "CRE Files\0*.cre\0All Files\0*.*";
  ofn.lpstrFilter = filterStrings;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter = 0;
  ofn.nFilterIndex = 1;
  char fileString[1024] = "";
  ofn.lpstrFile = fileString;
  ofn.nMaxFile = 1024;
  char filenameString[1024] = "";
  ofn.lpstrFileTitle = filenameString;
  ofn.nMaxFileTitle = 1024;
  ofn.lpstrInitialDir = NULL;
  ofn.lpstrTitle = NULL;
  ofn.Flags = OFN_CREATEPROMPT | OFN_LONGNAMES | 
              OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
  ofn.nFileOffset = 0;
  ofn.nFileExtension = 0;
  ofn.lpstrDefExt = "cre";
  ofn.lCustData = 0;
  ofn.lpfnHook = NULL;
  ofn.lpTemplateName = NULL;
#if (_WIN32_WINNT >= 0x0500)
  ofn.pvReserved = NULL;
  ofn.dwReserved = 0;
  ofn.FlagsEx = 0;
#endif // (_WIN32_WINNT >= 0x0500)

  int filename = GetSaveFileName(&ofn);

  if(!filename) {
    return;
  }

  d_fileName = ofn.lpstrFile;

  Save();

  return;
}

// Creature
void MainWindow::Freeze(void) {
  MENUITEMINFO mii;
  HMENU hmenu = GetMenu(d_hwnd);
  GetMenuItemInfo(hmenu, 201, 0, &mii);

  if(d_frozen) {
    d_frozen = false;
    mii.dwTypeData = "Freeze";
    d_ie.FreezeSelected();
  } else {
    d_frozen = true;
    mii.dwTypeData = "UnFreeze";
    d_ie.UnfreezeSelected();
  }

  DrawMenuBar(d_hwnd);

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
    MessageBox(NULL, "ERROR: Couldn't get genome.", "Whirling Dervish Play",
               MB_OK);
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
  MessageBox(NULL, "Have Fun!", "Whirling Dervish Play", MB_OK);

  return;
}

void MainWindow::Update(const string &str) {
  Parser &p = d_ie.GetParser();

  vector<int> errors;
  if(!p.Validate(errors, str)) {
    MessageBox(NULL, "ERROR: Invalid code!",
			         "Whirling Dervish Play", MB_OK);
    return;
  }

  if(!d_ie.AlterSelected(str)) {
    MessageBox(NULL, "ERROR: Failed to alter creature!",
			         "Whirling Dervish Play", MB_OK);
  }

  return;
}

void MainWindow::UpdateSystem(void) {
  RECT rect;
  GetClientRect(d_hwnd, &rect);
  if(d_graphics == 6) {
    d_graphics = 0;
    d_ijgl.updateGL((int)rect.right, (int)rect.bottom);
    SwapBuffers(GetDC(d_hwnd));
  } else {
    d_graphics++;
  }

  d_ie.Process(d_ie.Timestep());

  DisplayHeight();

  return;
}

#if 0
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

#endif

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

  //d_heightData.setNum(height);

  return;
}
