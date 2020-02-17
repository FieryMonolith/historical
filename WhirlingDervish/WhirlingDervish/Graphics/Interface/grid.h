#ifndef __GRID_H__
#define __GRID_H__

class Grid {
public:
  Grid(void) { return; }
  virtual ~Grid(void) { return; }

  virtual void ChangeGrid(
       double originX, double originY, double originZ,
       double relativeNegativeBoundX, double relativePositiveBoundX,
       double relativeNegativeBoundY, double relativePositiveBoundY,
       double relativeNegativeBoundZ, double relativePositiveBoundZ,
       double xyTick, double xzTick, double yzTick)=0;

  virtual void Show(void)=0;
  virtual void Hide(void)=0;
};

#endif
