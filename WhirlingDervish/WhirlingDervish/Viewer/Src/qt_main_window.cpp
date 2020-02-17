#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

#include "qt_main_window.h"

#include "file_handler.h"
#include "language_engine.h"
#include "parser.h"

using namespace std;

MainWindow::MainWindow(InteractiveEnvironment &ie, FileHandler &fh,
		       QWidget *, const char *)
  : d_ie(ie),
    d_layout(this),
    d_menu(this, "MenuBar"),
    d_fileMenu(&d_menu),
    d_viewMenu(&d_menu),
    d_helpMenu(&d_menu),
    d_accel(this),
    d_ijgl(d_ie.GetPS(), this, "iJGL"),
    d_editor(this, "Editor", true),
    d_bindings(&d_accel, &d_ijgl, this, this, "Key Bindings"),
    d_dataLayout(3, 6, 3, "Data Layout"),
    d_generationLabel("Generation:", this),
    d_generationData("0", this),
    d_currentTimeLabel("Current Time:", this),
    d_currentTimeData("0.0", this),
    d_simTimeLabel("Sim. Time:", this),
    d_simTimeData("0.0", this),
    d_avgFitnessLabel("Avg. Fit.:", this),
    d_avgFitnessData("0.0", this),
    d_avgPointsLabel("Avg. Points:", this),
    d_avgPointsData("0.0", this),
    d_avgSpringsLabel("Avg. Springs:", this),
    d_avgSpringsData("0.0", this),
    d_maxFitnessLabel("Max. Fit.:", this),
    d_maxFitnessData("0.0", this),
    d_maxHeightLabel("Max. Height:", this),
    d_maxHeightData("0.0", this),
    d_avgHeightLabel("Avg. Height", this),
    d_avgHeightData("0.0", this),
    d_timer(this),
    d_graphics(0),
    d_fh(fh),
    d_done(0) {
  d_layout.addWidget(&d_menu);
  d_layout.addWidget(&d_ijgl);
  d_layout.addLayout(&d_dataLayout);

  d_dataLayout.addWidget(&d_generationLabel,  0, 0, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_generationData,   0, 1, Qt::AlignRight);
  d_dataLayout.addWidget(&d_currentTimeLabel, 0, 2, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_currentTimeData,  0, 3, Qt::AlignRight);
  d_dataLayout.addWidget(&d_simTimeLabel,     0, 4, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_simTimeData,      0, 5, Qt::AlignRight);

  d_dataLayout.addWidget(&d_avgFitnessLabel, 1, 0, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_avgFitnessData,  1, 1, Qt::AlignRight);
  d_dataLayout.addWidget(&d_avgPointsLabel,  1, 2, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_avgPointsData,   1, 3, Qt::AlignRight);
  d_dataLayout.addWidget(&d_avgSpringsLabel, 1, 4, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_avgSpringsData,  1, 5, Qt::AlignRight);

  d_dataLayout.addWidget(&d_maxFitnessLabel, 2, 0, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_maxFitnessData,  2, 1, Qt::AlignRight);
  d_dataLayout.addWidget(&d_maxHeightLabel,  2, 2, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_maxHeightData,   2, 3, Qt::AlignRight);
  d_dataLayout.addWidget(&d_avgHeightLabel,  2, 4, Qt::AlignLeft);
  d_dataLayout.addWidget(&d_avgHeightData,   2, 5, Qt::AlignRight);

  d_fileMenu.insertItem("Pause", this, SLOT(Pause()));
  d_fileMenu.insertItem("View Genome", this, SLOT(ViewGenome()));
  d_fileMenu.insertItem("Key bindings", &d_bindings, SLOT(exec()));
  d_fileMenu.insertItem("Exit", qApp, SLOT(quit()));

  d_viewMenu.insertItem("Normal", this, SLOT(NormalView()));
  d_viewMenu.insertItem("Down", this, SLOT(DownView()));

  d_helpMenu.insertItem("About", this, SLOT(About()));

  d_menu.insertItem("File", &d_fileMenu);
  d_menu.insertItem("View", &d_viewMenu);
  d_menu.insertItem("Help", &d_helpMenu);

  connect(&d_timer, SIGNAL(timeout()), this, SLOT(UpdateSystem()));
  d_timer.start(10, FALSE);

  return;
}

MainWindow::~MainWindow(void) {
  d_timer.stop();
  d_ie.Shutdown(d_fh);

  return;
}

// File
void MainWindow::Pause(void) {
  if(d_ie.Paused()) {
    d_ie.Unpause();
  } else {
    d_ie.Pause();
  }

  return;
}

void MainWindow::ViewGenome(void) {
#if 0
  stringstream ss;
  if(!d_ie.GenomeSelected(ss)) {
    QMessageBox::information(0, "Whirling Dervish Play",
			     "ERROR: Couldn't get genome." );
  }

  d_editor.SetText(ss.str());
#endif
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
  QMessageBox::information(0, "Whirling Dervish View", "Have Fun!" );

  return;
}

void MainWindow::UpdateSystem(void) {
  if(d_graphics == 6) {
    d_graphics = 0;
    d_ijgl.updateGL();
  } else {
    d_graphics++;
  }

  if(d_ie.CurrentGeneration() < d_ie.MaxGeneration()) {
    d_ie.ExperimentProcess(0.01, d_fh);
  } else if(d_done == 0) {
    d_done = 1;
    QMessageBox::information(0, "Whirling Dervish View",
			     "Experiment Complete." );
  }

  d_generationData.setNum((int)d_ie.Generation());
  d_currentTimeData.setNum(d_ie.CurrentTime());
  d_simTimeData.setNum(d_ie.SimulationTime());
  d_avgFitnessData.setNum(d_ie.AvgFitness());
  d_avgPointsData.setNum(d_ie.AvgPoints());
  d_avgSpringsData.setNum(d_ie.AvgSprings());
  d_maxFitnessData.setNum(d_ie.MaxFitness());
  d_maxHeightData.setNum(d_ie.MaxHeight());
  d_avgHeightData.setNum(d_ie.AvgHeight());

  return;
}
