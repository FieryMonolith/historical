#ifndef __GRID_IMPLEMENTATION_H__
#define __GRID_IMPLEMENTATION_H__

#include "grid.h"

class GridImplementation : public Grid {
public:
  GridImplementation(
       double originX, double originY, double originZ,
       double relativeNegativeBoundX, double relativePositiveBoundX,
       double relativeNegativeBoundY, double relativePositiveBoundY,
       double relativeNegativeBoundZ, double relativePositiveBoundZ,
       double xyTick, double xzTick, double yzTick);
  ~GridImplementation(void);

  void ChangeGrid(
       double originX, double originY, double originZ,
       double relativeNegativeBoundX, double relativePositiveBoundX,
       double relativeNegativeBoundY, double relativePositiveBoundY,
       double relativeNegativeBoundZ, double relativePositiveBoundZ,
       double xyTick, double xzTick, double yzTick);
  void Draw(void) const;

  void Show(void);
  void Hide(void);

private:
  void DrawPlane(int, int, int, int) const;

  // x=0, y=1, z=2
  double d_origin[3];
  double d_relativeNegativeBound[3];
  double d_relativePositiveBound[3];
  double d_tick[3];
  int    d_ticks[3];
  int    d_drawPlane[3];
  bool   d_show;
};

#endif
