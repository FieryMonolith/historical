#ifndef __EC_SETTINGS_H__
#define __EC_SETTINGS_H__

#include <string>

#include "setting.h"

class ECSettings : public Setting {
public:
  ECSettings(void);
  ~ECSettings(void);

  bool Initialize(void);

  double  GenomicRate(void) const { return d_genomicRate; }
  double &GenomicRate(void)       { return d_genomicRate; }

  double  NonGenomicRate(void) const { return d_nonGenomicRate; }
  double &NonGenomicRate(void)       { return d_nonGenomicRate; }

  double  Point(void) const { return d_point; }
  double &Point(void)       { return d_point; }

  double  Deletion(void) const { return d_deletion; }
  double &Deletion(void)       { return d_deletion; }

  double  InsertionRandom(void) const { return d_insertionRandom; }
  double &InsertionRandom(void)       { return d_insertionRandom; }

  double  InsertionDuplication(void) const { return d_insertionDuplication; }
  double &InsertionDuplication(void)       { return d_insertionDuplication; }

  double  ChanceOfData(void) const { return d_chanceOfData; }
  double &ChanceOfData(void)       { return d_chanceOfData; }

private:
  double d_genomicRate;
  double d_nonGenomicRate;
  double d_point;
  double d_deletion;
  double d_insertionRandom;
  double d_insertionDuplication;
  double d_chanceOfData;

  static const std::string n_genomicRate;
  static const std::string n_nonGenomicRate;
  static const std::string n_point;
  static const std::string n_deletion;
  static const std::string n_insertionRandom;
  static const std::string n_insertionDuplication;
  static const std::string n_chanceOfData;
};

#endif
