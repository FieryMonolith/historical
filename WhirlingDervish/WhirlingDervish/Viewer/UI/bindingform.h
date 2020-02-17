/****************************************************************************
** Form interface generated from reading ui file 'UI/bindingform.ui'
**
** Created: Tue Jul 20 14:11:28 2004
**      by: The User Interface Compiler ($Id: bindingform.h,v 1.1.1.1 2004/10/14 22:49:33 jason Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef BINDINGFORM_H
#define BINDINGFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QCheckBox;
class QLabel;
class QPushButton;
class QSlider;
class QTabWidget;
class QWidget;

class BindingForm : public QDialog
{
    Q_OBJECT

public:
    BindingForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~BindingForm();

    QPushButton* CancelButton;
    QPushButton* DoneButton;
    QPushButton* ApplyButton;
    QPushButton* LoadDefaultsButton;
    QPushButton* SaveDefaultsButton;
    QTabWidget* KeyBinder;
    QWidget* system;
    QButtonGroup* SystemButtonGroup;
    QPushButton* QuitButton;
    QPushButton* HelpButton;
    QPushButton* PauseButton;
    QPushButton* GridButton;
    QButtonGroup* SystemButtonGroup2;
    QLabel* GridKey;
    QLabel* PauseKey;
    QLabel* HelpKey;
    QLabel* QuitKey;
    QWidget* mouse;
    QLabel* QuitKey_2;
    QButtonGroup* MouseButtonGroup;
    QPushButton* MouseToggleButton;
    QPushButton* FreeLookButton;
    QLabel* TextLabel1;
    QSlider* MouseSpeedMultiplier;
    QCheckBox* InvertMouse;
    QLabel* MouseToggleKey;
    QLabel* FreeLookKey;
    QWidget* eyepoint;
    QButtonGroup* EyepointButtonGroup2;
    QLabel* MoveRightKey;
    QLabel* MoveUpKey;
    QLabel* MoveLeftKey;
    QLabel* TurnLeftKey;
    QLabel* TurnRightKey;
    QLabel* MoveDownKey;
    QLabel* PitchUpKey;
    QLabel* PitchDownKey;
    QLabel* MoveBackwardKey;
    QLabel* MoveForwardKey;
    QButtonGroup* EyepointButtonGroup;
    QPushButton* ForwardButton;
    QPushButton* BackwardButton;
    QPushButton* LeftButton;
    QPushButton* RightButton;
    QPushButton* TurnRightButton;
    QPushButton* UpButton;
    QPushButton* DownButton;
    QPushButton* PitchUpButton;
    QPushButton* PitchDownButton;
    QPushButton* TurnLeftButton;

protected:

protected slots:
    virtual void languageChange();
};

#endif // BINDINGFORM_H
