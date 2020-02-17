#ifndef __Keyboard_h__
#define __Keyboard_h__

// Just to get something up and running, I am just going to assume the
//   use of ASCII.  I want to fix later such as the inclusion of a 
//   translation class.

#include "iKeyboard.h"

#include "BuiltinTypes.h"

class Keyboard : public iKeyboard
{
public:
   Keyboard(void);
  ~Keyboard(void) { return; }

private:
  // Prevent copy semantics
  Keyboard(const Keyboard &);
  Keyboard &operator=(const Keyboard &);

public:
  /*** Button Information ****************************************************/
  uint ButtonState(const uint8 button) const
  {
    const uint a = button / 8;
    const uint b = button % 8;
    return d_keys[a] & (1 << b);
  }

  uint ChangedState(const uint8 button) const
  {
    const uint a = button / 8;
    const uint b = button % 8;
    return d_changed[a] & (1 << b);
  }

  uint IsPressed(const uint8 button) const
  {
    const uint a = button / 8;
    const uint b = button % 8;
    return d_keys[a] & (1 << b);
  }

  uint IsTriggered(const uint8 button) const
  {
    const uint a = button / 8;
    const uint b = button % 8;
    return (d_changed[a] & (1 << b)) && (d_keys[a] & (1 << b));
  }

  uint IsNotPressed(const uint8 button) const
  {
    const uint a = button / 8;
    const uint b = button % 8;
    return (d_keys[a] & (1 << b)) == 0;
  }

  uint IsNotTriggered(const uint8 button) const
  {
    const uint a = button / 8;
    const uint b = button % 8;
    return (d_changed[a] & (1 << b)) && (d_keys[a] & (1 << b)) == 0;
  }

  void SetButton(const uint8 button)
  {
    const uint a = button / 8;
    const uint b = button % 8;
    if((d_keys[a] & (1 << b)) == 0) d_changed[a] |= (1 << b);
    d_keys[a] |= (1 << b);
    return;
  }

  void ClearButton(const uint8 button)
  {
    const uint a = button / 8;
    const uint b = button % 8;
    if((d_keys[a] & (1 << b)) == 1) d_changed[a] |= (1 << b);
    d_keys[a] &= ~(1 << b);
    return;
  }

  void ClearChanged(void); // DEFINED IN SOURCE FILE

private:
  uint32 d_keyCount;
  uint8  d_keys[32];
  uint8  d_changed[32];
};

#endif // __Keyboard_h__
