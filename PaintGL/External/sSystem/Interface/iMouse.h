#ifndef __iMouse_h__
#define __iMouse_h__

#include "BuiltinTypes.h"

class iMouse
{
public:
  struct Position;
  enum ButtonId;

protected:
   iMouse(void) { return; }
  ~iMouse(void) { return; }

private:
  iMouse(const iMouse &);
  iMouse &operator=(const iMouse &);

public:
  /*** State Information *****************************************************/
  // Returns a boolean valued integer for each state, 0 == false/not present.
  virtual uint32 Present(void)     const = 0;
  virtual uint32 HasWheel(void)    const = 0;
  virtual uint32 ButtonCount(void) const = 0;

  /*** Button Information ****************************************************/
  // Returns a boolean valued integer for a button's state, 0 == false.
  virtual uint32 ButtonState (const ButtonId button) const = 0;
  // Returns a boolean valued integer 0 == false.  The value is if the given
  //   button has changed state during the last round of system message
  //   handling.
  virtual uint32 ChangedState(const ButtonId button) const = 0;

  // Returns a boolean valued integer 0 == false.  IsPressed is true if
  //   the button's state is currently on.
  virtual uint32 IsPressed  (const ButtonId button) const = 0;
  // Returns a boolean valued integer 0 == false.  IsTriggered is true if
  //   the button's state just changed to on, so
  //   button == on && changed == true
  virtual uint32 IsTriggered(const ButtonId button) const = 0;

  // Returns a boolean valued integer 0 == false.  IsPressed is true if
  //   the button's state is currently NOT on.
  virtual uint32 IsNotPressed  (const ButtonId button) const = 0;
  // Returns a boolean valued integer 0 == false.  IsTriggered is true if
  //   the button's state just changed to off, so
  //   button == off && changed == true
  virtual uint32 IsNotTriggered(const ButtonId button) const = 0;

  // Turns bits on for valid buttons.
  virtual void SetButton  (const ButtonId button) = 0;
  // Turns bits off for valid buttons.
  virtual void ClearButton(const ButtonId button) = 0;
  // Turns off all changed bits
  virtual void ClearChanged(void) = 0;

  /*** Position Information **************************************************/
  // Get Functions (current and previous)
  virtual Position GetPosition(void) const = 0;
  virtual Position GetPrevious(void) const = 0;
  virtual uint32 GetX(void)          const = 0;
  virtual uint32 GetY(void)          const = 0;
  virtual uint32 GetPrevX(void)      const = 0;
  virtual uint32 GetPrevY(void)      const = 0;

  // Set Functions (current and previous)
  virtual void SetPosition(const Position p) = 0;
  virtual void SetPrevious(const Position p) = 0;
  virtual void SetPosition(const uint32 x, const uint32 y) = 0;
  virtual void SetPrevious(const uint32 x, const uint32 y) = 0;

public:
  /*** Support structures ****************************************************/
  struct Position { uint32 x, y; };

  // Valid buttons utilized by the mouse.  How many buttons the 
  //   mouse physically has is determined by button count.
  enum ButtonId
  {
    Button1 = (1 << 0),
    Button2 = (1 << 1),
    Button3 = (1 << 2),
    Button4 = (1 << 3),
    Button5 = (1 << 4)
  };
}; // class iMouse

#endif // __iMouse_h__
