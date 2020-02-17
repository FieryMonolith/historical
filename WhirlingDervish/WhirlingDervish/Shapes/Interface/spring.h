#ifndef __SPRING_H__
#define __SPRING_H__

class Spring {
public:
  Spring(double innateLength     = 0.0,
	 double ks               = 0.0,
	 double kd               = 0.0,
	 double fluctuationDelta = 0.0,
	 double fluctuationStart = 0.0,
	 double fluctuationRate  = 0.0);
  Spring(const Spring &);
  ~Spring(void);

  Spring &operator=(const Spring &);

  double  InnateLength(void)           const { return d_innateLength; }
  double &InnateLength(void)                 { return d_innateLength; }
  double  OriginalInnateLength(void)   const { return d_originalInnateLength; }
  double &OriginalInnateLength(void)         { return d_originalInnateLength; }
  double  ks(void)                     const { return d_ks; }
  double &ks(void)                           { return d_ks; }
  double  kd(void)                     const { return d_kd; }
  double &kd(void)                           { return d_kd; }
  double  FluctuationDelta(void)       const { return d_fluctuationDelta; }
  double &FluctuationDelta(void)             { return d_fluctuationDelta; }
  double  FluctuationStart(void)       const { return d_fluctuationStart; }
  double &FluctuationStart(void)             { return d_fluctuationStart; }
  double  FluctuationRate(void)        const { return d_fluctuationRate; }
  double &FluctuationRate(void)              { return d_fluctuationRate; }
  double  Increment(void)              const { return d_increment; }
  double &Increment(void)                    { return d_increment; }

  void Fluctuate(double timestep, double maxFluctuationPercent);

private:
  double d_innateLength;
  double d_originalInnateLength;
  double d_ks;
  double d_kd;
  double d_fluctuationDelta;
  double d_fluctuationStart;
  double d_fluctuationRate;
  double d_increment;
};

#endif
