#ifndef __iSurface_h__
#define __iSurface_h__

#include "BuiltinTypes.h"

class iSurface
{
protected:
   iSurface(void) throw () { return; }
  ~iSurface(void) throw () { return; }

private:
  iSurface(const iSurface &);
  iSurface &operator=(const iSurface &);

public:
  // Surface dimensions
  virtual uint32 GetWidth(void)  const throw () = 0;
  virtual uint32 GetHeight(void) const throw () = 0;
  virtual void Resize(const uint32 width, const uint32 height) throw (int) = 0;

  virtual void MakeCurrent(void) throw (int) = 0;
  virtual void SwapBuffers(void) throw (int) = 0;
}; // class iSurface

#endif // __iSurface_h__
