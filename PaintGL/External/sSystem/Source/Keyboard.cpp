#include "Keyboard.h"

#include <cstring>

Keyboard::Keyboard(void)
{
  d_keyCount = 0;
  ::memset(d_keys,    0x00, sizeof(d_keys));
  ::memset(d_changed, 0x00, sizeof(d_changed));
  return;
}

void Keyboard::ClearChanged(void)
{
  ::memset(d_changed, 0x00, sizeof(d_changed));
  return;
}
