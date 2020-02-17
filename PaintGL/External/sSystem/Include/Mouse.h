#ifndef __Mouse_h__
#define __Mouse_h__

#include "iMouse.h"

#include "BuiltinTypes.h"

class Mouse : public iMouse
{
public:
   Mouse(void) { return; }
  ~Mouse(void) { return; }

private:
  // Prevent copy semantics
  Mouse(const Mouse &);
  Mouse &operator=(Mouse &);

public:
  /*** State Information *****************************************************/
  uint32 Present(void)  const
  {
    return d_mouseConfig & MousePresentBit;
  }

  uint32 HasWheel(void) const
  {
    return d_mouseConfig & WheelPresentBit;
  }

  uint32 ButtonCount(void) const
  {
    return d_mouseConfig >> Buttons & 0x1f;
  }

  /*** Button Information ****************************************************/
  uint32 ButtonState(const ButtonId button) const
  {
    return d_buttonState & button;
  }

  uint32 ChangedState(const ButtonId button) const
  {
    return d_changed & button;
  }

  uint32 IsPressed(const ButtonId button) const
  {
    return d_buttonState & button;
  }

  uint32 IsNotPressed(const ButtonId button) const
  {
    return (d_buttonState & button) == 0;
  }

  uint32 IsTriggered(const ButtonId button) const
  {
    return d_changed & button && d_buttonState & button;
  }

  uint32 IsNotTriggered(const ButtonId button) const
  {
    return d_changed & button && (d_buttonState & button) == 0;
  }

  void SetButton(const ButtonId button)
  {
    if((d_buttonState & button) == 0) d_changed |= button;
    d_buttonState |= button;
    return;
  }

  void ClearButton(const ButtonId button)
  {
    if((d_buttonState & button) == 1) d_changed |= button;
    d_buttonState &= ~button;
    return;
  }

  void ClearChanged(void);

  /*** Position Information **************************************************/
  Position GetPosition(void) const { return d_current;    }
  Position GetPrevious(void) const { return d_previous;   }
  uint32 GetX(void)          const { return d_current.x;  }
  uint32 GetY(void)          const { return d_current.y;  }
  uint32 GetPrevX(void)      const { return d_previous.x; }
  uint32 GetPrevY(void)      const { return d_previous.y; }

  void SetPosition(const Position p) { d_current  = p; return; }
  void SetPrevious(const Position p) { d_previous = p; return; }
  void SetPosition(const uint32 x, const uint32 y)
  {
    d_current.x = x;
    d_current.y = y;
    return;
  }
  void SetPrevious(const uint32 x, const uint32 y)
  {
    d_previous.x = x;
    d_previous.y = y;
    return;
  }

protected:
  /*** Data ******************************************************************/
  // present(1), hasWheel(2), buttonCount(3-7)
  uint32 d_mouseConfig;
  // Each bit represents the on/off state of a specific button
  uint32 d_buttonState;
  // Each bit represents whether or not the button state bit has changed
  uint32 d_changed;
  // Position information of the mouse
  Position d_current, d_previous;

protected:
  /*** Support structures ****************************************************/
  // These two enums are used to compute the bit locations of configuration
  //   data.
  enum Config_StartBit
  {
    MousePresent = (1 << 0),
    WheelPresent = (1 << 1),
    Buttons      = (1 << 2)
  };
  enum Config_Bits
  {
    MousePresentBit  = (1 << 0),
    WheelPresentBit  = (1 << 1),
    ButtonsBits      = (0x1f << 2)
  };
}; // class Mouse

#endif // __Mouse_h__
