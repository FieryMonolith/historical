#ifndef __PRIMATIVE_H__
#define __PRIMATIVE_H__

class Primative {
public:
  Primative(void);
  virtual Primative(void);

  virtual Draw(void) const = 0;
};

#endif
