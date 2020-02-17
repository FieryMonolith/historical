#ifndef __BOX_H__
#define __BOX_H__

#include "primative.h"
#include "jvector.h"
#include "spacial_description.h"

class Box : public Primative {
public:
  Box(void);
  ~Box(void);

  void Draw(void) const;

  const Vector3d &Position(void) const { return d_position; }
        Vector3d &Position(void)       { return d_position; }
  const Vector3d &Size(void) const { return d_size; }
        Vector3d &Size(void)       { return d_size; }

private:
  Vector3d d_size;
  Vector3d d_position;
};

#endif
