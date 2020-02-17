#include <cmath>

#include "spring.h"

Spring::Spring(double innateLength, double ks, double kd,
	       double fluctuationDelta, double fluctuationStart,
	       double fluctuationRate)
  : d_innateLength(innateLength),
    d_originalInnateLength(innateLength),
    d_ks(ks),
    d_kd(kd),
    d_fluctuationDelta(fluctuationDelta),
    d_fluctuationStart(fluctuationStart),
    d_fluctuationRate(fluctuationRate),
    d_increment(0.0) {
  return;
}

Spring::Spring(const Spring &s) {
  *this = s;

  return;
}

Spring::~Spring(void) {
  return;
}

Spring &Spring::operator=(const Spring &s) {
  d_innateLength = s.d_innateLength;
  d_originalInnateLength = s.d_originalInnateLength;
  d_ks = s.d_ks;
  d_kd = s.d_kd;
  d_fluctuationDelta = s.d_fluctuationDelta;
  d_fluctuationStart = s.d_fluctuationStart;
  d_fluctuationRate  = s.d_fluctuationRate;
  d_increment = s.d_increment;

  return *this;
}

void Spring::Fluctuate(double timestep, double maxFluctuationPercent) {
  const double phase = sin((d_fluctuationRate * d_increment) +
			   (M_PI * d_fluctuationStart));
  const double fluxRange = fabs(d_fluctuationDelta) * maxFluctuationPercent;
  d_innateLength = d_originalInnateLength * (1 + phase * fluxRange);

  if(d_fluctuationRate > 0) {
    d_increment = fmod(d_increment + timestep,
	  	     2.0 * M_PI / fabs(d_fluctuationRate));
  }

  return;
}
