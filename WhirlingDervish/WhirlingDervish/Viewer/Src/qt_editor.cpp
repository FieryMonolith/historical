#include <qcolor.h>

#include "qt_editor.h"

Editor::Editor(QWidget *parent, const char *name, bool modal, WFlags f)
  : QDialog(parent, name, modal, f),
    d_vlayout(this),
    d_edit(this, "editor text edit"),
    d_close("Close", this, "editor close") {
  setPaletteBackgroundColor(QColor(0, 0, 0));
  setSizeGripEnabled(true);
  resize(400, 600);

  d_vlayout.addWidget(&d_edit);
  d_vlayout.addLayout(&d_hlayout);

  d_hlayout.addWidget(&d_close);

  d_edit.setPaletteBackgroundColor(QColor(0, 0, 0));
  d_edit.setPaletteForegroundColor(QColor(0, 255, 0));

  d_close.setMinimumWidth(100);
  d_close.setMaximumWidth(100);

  connect(&d_close, SIGNAL(clicked()), this, SLOT(Close()));

  return;
}

Editor::~Editor(void) {
  return;
}

void Editor::Close(void) {
  hide();

  return;
}

void Editor::SetText(const QString &s) {
  d_edit.setText(s);

  return;
}
