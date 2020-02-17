#ifndef __System_h__
#define __System_h__

#include "sSystem.h"

#include <vector>

#include "BuiltinTypes.h"

class iSurface;
class iSurface_System;
class iMouse;
class iKeyboard;

class System : public sSystem
{
protected:
   System(void) { return; }
  ~System(void);

private:
  System(const System &);
  System &operator=(const System &);

public:
  /*** BEGIN Surface Interface ***********************************************/
  void Destroy(iSurface *);

  // Device interfaces
  iMouse    &GetMouse   (iSurface *);
  iKeyboard &GetKeyboard(iSurface *);

  // Upon entering the blocking version the function will wait until there 
  //   is a message in the queue.  Once there is a message in the queue 
  //   the function will process all messages until the queue is empty 
  //   and then return.
  // This function also resets the changed status for all buttons for the 
  //   keyboard and mouse.
  // Could throw an exception if the system kills the surface internally 
  //   without prompting from the user.  Otherwise no exceptions.
  void ProcessMessages_Blocking(iSurface *) throw (int);
  // Upon entering the non-blocking version the function will process 
  //   all messages in the queue and then exit.  If there are no waiting 
  //   messages the function immediately returns.
  // This function also resets the changed status for all buttons for the 
  //   keyboard and mouse.
  // Could throw an exception if the system kills the surface internally 
  //   without prompting from the user.  Otherwise no exceptions.
  void ProcessMessages_NonBlocking(iSurface *) throw (int);

  // Surface screen position
  uint32 GetX(const iSurface *) const throw ();
  uint32 GetY(const iSurface *) const throw ();
  void NewPosition(iSurface *, const uint32 x, const uint32 y) throw(int);

  // Surface visibility
  void Show    (iSurface *) throw ();
  void Hide    (iSurface *) throw ();
  void Minimize(iSurface *) throw ();
  void Restore (iSurface *) throw ();
  /*** END Surface Interface *************************************************/

protected:
  std::vector<iSurface_System*> d_surfaces;

  std::vector<iSurface_System*>::iterator FindSurface(iSurface *) throw(int);
  std::vector<iSurface_System*>::const_iterator
    FindSurface(const iSurface *) const throw(int);
}; // class System

#endif // __System_h__
