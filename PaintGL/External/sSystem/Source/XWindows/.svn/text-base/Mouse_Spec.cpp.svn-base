#include "Mouse_Spec.h"

#include <windows.h>
#include <cstring>

Mouse_MSC::Mouse_MSC(void)
{
  d_mouseConfig = 0;
  d_buttonState = 0;
  d_changed     = 0;
  ::memset(&d_current,  0x00, sizeof(d_current));
  ::memset(&d_previous, 0x00, sizeof(d_previous));

  // Do nothing if mouse is not present
  if(::GetSystemMetrics(SM_MOUSEPRESENT)) d_mouseConfig |= MousePresentBit;
  else return;

  // Set mouse configurations
  if(::GetSystemMetrics(SM_MOUSEWHEELPRESENT))
    d_mouseConfig |= WheelPresentBit;

  d_mouseConfig |=
    ((::GetSystemMetrics(SM_CMOUSEBUTTONS) & 0x1f) << Buttons);

  //::ShowCursor(false);

  return;
}
