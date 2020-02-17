#ifndef __OpenglSurface_h__
#define __OpenglSurface_h__

#include "iSurface_System.h"

#include "BuiltinTypes.h"

class OpenglSurface : public iSurface_System
{
public:
  // A value of 0 means use a default size for width and height
  OpenglSurface(const uint32 width, const uint32 height) throw ()
  : d_width(width), d_height(height)
  { return; }

  ~OpenglSurface(void) throw () { return; }

private:
  OpenglSurface(const OpenglSurface &);
  OpenglSurface &operator=(const OpenglSurface &);

public:
  uint32 GetWidth(void)  const { return d_width;  }
  uint32 GetHeight(void) const { return d_height; }
  void Resize(const uint32 w, const uint32 h) { d_width=w; d_height=h; return; }

protected:
  /*** Data ******************************************************************/
  uint32 d_width, d_height;
};

#endif // __OpenglSurface_h__
