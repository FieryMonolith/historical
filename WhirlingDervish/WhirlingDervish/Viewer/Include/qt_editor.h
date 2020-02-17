#ifndef __QT_EDITOR_H__
#define __QT_EDITOR_H__

#include <qdialog.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qstring.h>

class Editor : public QDialog {
  Q_OBJECT

public:
  Editor(QWidget * = 0, const char * = 0, bool modal = false, WFlags f = 0);
  ~Editor(void);

  void SetText(const QString &);

private:
  QVBoxLayout d_vlayout;
  QHBoxLayout d_hlayout;
  QMultiLineEdit d_edit;
  QPushButton d_close;

private slots:
  void Close(void);

signals:
  void Text(QString);
};

#endif
