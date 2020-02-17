#include "spring_info.h"

SpringInfo::SpringInfo(void) : d_id(0), d_pause(false) {
  return;
}

SpringInfo::SpringInfo(const SpringInfo &si) {
  *this = si;

  return;
}

SpringInfo::~SpringInfo(void) {
  d_pause = false;

  return;
}

SpringInfo &SpringInfo::operator=(const SpringInfo &si) {
  d_spring = si.d_spring;
  d_id = si.d_id;

  d_innateLength = si.d_innateLength;
  d_originalInnateLength = si.d_originalInnateLength;
  d_ks = si.d_ks;
  d_kd = si.d_kd;
  d_fluctuationDelta = si.d_fluctuationDelta;
  d_fluctuationStart = si.d_fluctuationStart;
  d_fluctuationRate = si.d_fluctuationRate;
  d_increment = si.d_increment;
  d_pause = si.d_pause;

  return *this;
}

void SpringInfo::Reset(void) {
  d_spring.InnateLength() = d_innateLength;
  d_spring.OriginalInnateLength() = d_originalInnateLength;
  d_spring.ks() = d_ks;
  d_spring.kd() = d_kd;
  d_spring.FluctuationDelta() = d_fluctuationDelta;
  d_spring.FluctuationStart() = d_fluctuationStart;
  d_spring.FluctuationRate() = d_fluctuationRate;
  d_spring.Increment() = d_increment;

  return;
}
