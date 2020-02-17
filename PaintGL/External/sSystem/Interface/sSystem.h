#ifndef __sSystem_h__
#define __sSystem_h__

#include "BuiltinTypes.h"

class sSystem &AccessSystem(void);

class iSurface;
class iMouse;
class iKeyboard;

class sSystem
{
protected:
   sSystem(void) { return; }
  ~sSystem(void) { return; }

private:
  sSystem(const sSystem &);
  sSystem &operator=(const sSystem &);

public:
  /*** BEGIN Surface Interface ***********************************************/

  // A value of 0 means use a default size for width and height
  virtual iSurface *GenerateOpenglSurface(const uint32 width,
                                          const uint32 height,
                                          const uint32 x,
                                          const uint32 y) = 0;

  virtual void Destroy(iSurface *) = 0;

  // Device interfaces
  virtual iMouse    &GetMouse   (iSurface *) = 0;
  virtual iKeyboard &GetKeyboard(iSurface *) = 0;

  // Upon entering the blocking version the function will wait until there 
  //   is a message in the queue.  Once there is a message in the queue 
  //   the function will process all messages until the queue is empty 
  //   and then return.
  // This function also resets the changed status for all buttons for the 
  //   keyboard and mouse.
  // Could throw an exception if the system kills the surface internally 
  //   without prompting from the user.  Otherwise no exceptions.
  virtual void ProcessMessages_Blocking(iSurface *) throw (int) = 0;
  // Upon entering the non-blocking version the function will process 
  //   all messages in the queue and then exit.  If there are no waiting 
  //   messages the function immediately returns.
  // This function also resets the changed status for all buttons for the 
  //   keyboard and mouse.
  // Could throw an exception if the system kills the surface internally 
  //   without prompting from the user.  Otherwise no exceptions.
  virtual void ProcessMessages_NonBlocking(iSurface *) throw (int) = 0;

  // Surface screen position
  virtual uint32 GetX(const iSurface *) const throw () = 0;
  virtual uint32 GetY(const iSurface *) const throw () = 0;
  virtual void NewPosition(iSurface *, const uint32 x, const uint32 y)
    throw(int) = 0;

  // Surface visibility
  virtual void Show    (iSurface *) throw () = 0;
  virtual void Hide    (iSurface *) throw () = 0;
  virtual void Minimize(iSurface *) throw () = 0;
  virtual void Restore (iSurface *) throw () = 0;
  /*** END Surface Interface *************************************************/

}; // class sSystem

#endif // __sSystem_h__
