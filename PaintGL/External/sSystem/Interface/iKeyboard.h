#ifndef __iKeyboard_h__
#define __iKeyboard_h__

#include "BuiltinTypes.h"

class iKeyboard
{
protected:
   iKeyboard(void) { return; }
  ~iKeyboard(void) { return; }

private:
  iKeyboard(const iKeyboard &);
  iKeyboard &operator=(const iKeyboard &);

public:
  /*** Button Information ****************************************************/
  // Returns a boolean valued integer for a button's state, 0 == false.
  virtual uint ButtonState(const uint8 button)    const = 0;
  // Returns a boolean valued integer 0 == false.  The value is if the given
  //   button has changed state during the last round of system message
  //   handling.
  virtual uint ChangedState(const uint8 button)   const = 0;

  // Returns a boolean valued integer 0 == false.  IsPressed is true if
  //   the button's state is currently on.
  virtual uint IsPressed(const uint8 button)      const = 0;
  // Returns a boolean valued integer 0 == false.  IsTriggered is true if
  //   the button's state just changed to on, so
  //   button == on && changed == true
  virtual uint IsTriggered(const uint8 button)    const = 0;

  // Returns a boolean valued integer 0 == false.  IsPressed is true if
  //   the button's state is currently NOT on.
  virtual uint IsNotPressed(const uint8 button)   const = 0;
  // Returns a boolean valued integer 0 == false.  IsTriggered is true if
  //   the button's state just changed to off, so
  //   button == off && changed == true
  virtual uint IsNotTriggered(const uint8 button) const = 0;

  // Turns bits on for valid buttons.
  virtual void SetButton  (const uint8 button) = 0;
  // Turns bits off for valid buttons.
  virtual void ClearButton(const uint8 button) = 0;
  // Turns off all changed bits
  virtual void ClearChanged(void) = 0;
}; // class iKeyboard

#endif // __iKeyboard_h__
