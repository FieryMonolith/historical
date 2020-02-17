#ifndef __iSurface_System_h__
#define __iSurface_System_h__

/*

 * The iSurface should only be deletable by the sSystem and not by a global 
 *   user.  To that end I protected iSurface's destructor.  However, sSystem 
 *   still needs to manage generic surfaces, so it needs to store access to 
 *   generic surfaces.  The management also needs a way to delete a generic 
 *   surface so the iSurface_System intermediary class was created.  This 
 *   class is accessible only within sSystem and provides a uniform layer 
 *   below specific types of surfaces such as OpenglSurface.

 */

#include "iSurface.h"

#include "BuiltinTypes.h"

class iMouse;
class iKeyboard;

class iSurface_System : public iSurface
{
protected:
  iSurface_System(void) throw () { return; }

public:
  virtual ~iSurface_System(void) throw () { return; }

private:
  iSurface_System(const iSurface_System &);
  iSurface_System &operator=(const iSurface_System &);

public:
  // Device interfaces
  virtual iKeyboard &GetKeyboard(void)           throw ()    = 0;
  virtual iMouse    &GetMouse(void)              throw ()    = 0;

  virtual void ProcessMessages_Blocking   (void) throw (int) = 0;
  virtual void ProcessMessages_NonBlocking(void) throw (int) = 0;

  // Surface screen position
  virtual uint32 GetX(void) const                          throw ()   = 0;
  virtual uint32 GetY(void) const                          throw ()   = 0;
  virtual void NewPosition(const uint32 x, const uint32 y) throw(int) = 0;

  // Surface visibility
  virtual void Show(void)     throw () = 0;
  virtual void Hide(void)     throw () = 0;
  virtual void Minimize(void) throw () = 0;
  virtual void Restore(void)  throw () = 0;
};

#endif // __iSurface_System_h__
