#include "System.h"

#include "iSurface.h"
#include "iSurface_System.h"
#include "iMouse.h"
#include "iKeyboard.h"

using namespace std;

/*** BEGIN Class Definitions *************************************************/
System::~System(void)
{
  vector<iSurface_System*>::iterator i = d_surfaces.begin();
  for(; i != d_surfaces.end(); ++i)
    try { Destroy(*i); } catch(...) { }

  return;
}

void System::Destroy(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  delete *i;
  *i = 0;
  d_surfaces.erase(i);
  return;
}

iMouse &System::GetMouse(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  return (*i)->GetMouse();
}

iKeyboard &System::GetKeyboard(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  return (*i)->GetKeyboard();
}

void System::ProcessMessages_Blocking(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  try { (*i)->ProcessMessages_Blocking(); } catch(...) { throw 2; }
  return;
}

void System::ProcessMessages_NonBlocking(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  try { (*i)->ProcessMessages_NonBlocking(); } catch(...) { throw 2; }
  return;
}

uint32 System::GetX(const iSurface *s) const
{
  vector<iSurface_System*>::const_iterator i = FindSurface(s);
  return (*i)->GetX();
}

uint32 System::GetY(const iSurface *s) const
{
  vector<iSurface_System*>::const_iterator i = FindSurface(s);
  return (*i)->GetY();
}

void System::NewPosition(iSurface *s, const uint32 x, const uint32 y)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  try { (*i)->NewPosition(x,y); } catch(...) { throw 2; }
  return;
}

void System::Show(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  (*i)->Show();
  return;
}

void System::Hide(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  (*i)->Hide();
  return;
}

void System::Minimize(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  (*i)->Minimize();
  return;
}

void System::Restore(iSurface *s)
{
  vector<iSurface_System*>::iterator i = FindSurface(s);
  (*i)->Restore();
  return;
}

vector<iSurface_System*>::const_iterator System::FindSurface(const iSurface *s) const
{
  vector<iSurface_System*>::const_iterator i = d_surfaces.begin();
  for(; i != d_surfaces.end(); ++i)
    if(*i == s) return i;

  throw 1;
}

vector<iSurface_System*>::iterator System::FindSurface(iSurface *s)
{
  vector<iSurface_System*>::iterator i = d_surfaces.begin();
  for(; i != d_surfaces.end(); ++i)
    if(*i == s) return i;

  throw 1;
}
/*** END   Class Definitions *************************************************/
