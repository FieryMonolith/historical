#ifndef __SPRING_INFO_H__
#define __SPRING_INFO_H__

#include "defs.h"
#include "spring.h"

class SpringInfo {
public:
  SpringInfo(void);
  SpringInfo(const SpringInfo &);
  ~SpringInfo(void);

  SpringInfo &operator=(const SpringInfo &);

  void Reset(void);

  const Spring &GetSpring(void) const { return d_spring; }
        Spring &GetSpring(void)       { return d_spring; }

  int64  Id(void) const { return d_id; }
  int64 &Id(void)       { return d_id; }

  double  InnateLength(void) const { return d_innateLength; }
  double &InnateLength(void)       { return d_innateLength; }

  double  OriginalInnateLength(void) const { return d_originalInnateLength; }
  double &OriginalInnateLength(void)       { return d_originalInnateLength; }

  double  ks(void) const { return d_ks; }
  double &ks(void)       { return d_ks; }

  double  kd(void) const { return d_kd; }
  double &kd(void)       { return d_kd; }

  double  FluctuationDelta(void) const { return d_fluctuationDelta; }
  double &FluctuationDelta(void)       { return d_fluctuationDelta; }

  double  FluctuationStart(void) const { return d_fluctuationStart; }
  double &FluctuationStart(void)       { return d_fluctuationStart; }

  double  FluctuationRate(void)  const { return d_fluctuationRate; }
  double &FluctuationRate(void)        { return d_fluctuationRate; }

  double  Increment(void) const { return d_increment; }
  double &Increment(void)       { return d_increment; }

  bool  Pause(void) const { return d_pause; }
  bool &Pause(void)       { return d_pause; }

private:
  Spring d_spring;
  int64 d_id;

  double d_innateLength;
  double d_originalInnateLength;
  double d_ks;
  double d_kd;
  double d_fluctuationDelta;
  double d_fluctuationStart;
  double d_fluctuationRate;
  double d_increment;

  bool d_pause;
};

#endif
