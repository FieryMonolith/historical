#include "ec_settings.h"

using namespace std;

const string ECSettings::n_genomicRate("GENOMIC_RATE");
const string ECSettings::n_nonGenomicRate("NON_GENOMIC_RATE");
const string ECSettings::n_point("POINT");
const string ECSettings::n_deletion("DELETION");
const string ECSettings::n_insertionRandom("INSERTION_RANDOM");
const string ECSettings::n_insertionDuplication("INSERTION_DUPLICATION");
const string ECSettings::n_chanceOfData("CHANCE_OF_DATA");

ECSettings::ECSettings(void)
  : d_genomicRate(0.0),
    d_nonGenomicRate(0.0),
    d_point(0.0),
    d_deletion(0.0),
    d_insertionRandom(0.0),
    d_insertionDuplication(0.0),
    d_chanceOfData(0.0)
{
  return;
}

ECSettings::~ECSettings(void) {
  return;
}

bool ECSettings::Initialize(void) {
  Destroy();

  SettingParser::InputBase *ib;

  // ECSettings::GenomicRate
  ib = new SettingParser::Input<double &, ECSettings>(n_genomicRate, this,
						&ECSettings::GenomicRate);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_genomicRate, ib));

  // ECSettings::NonGenomicRate
  ib = new SettingParser::Input<double &, ECSettings>(
					     n_nonGenomicRate, this,
					     &ECSettings::NonGenomicRate);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_nonGenomicRate, ib));

  // ECSettings::Point
  ib = new SettingParser::Input<double &, ECSettings>(n_point, this,
						&ECSettings::Point);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_point, ib));

  // ECSettings::Deletion
  ib = new SettingParser::Input<double &, ECSettings>(n_deletion, this,
						&ECSettings::Deletion);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_deletion, ib));

  // ECSettings::InsertionRandom
  ib = new SettingParser::Input<double &, ECSettings>(
					    n_insertionRandom, this,
					    &ECSettings::InsertionRandom);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_insertionRandom, ib));

  // ECSettings::InsertionDuplication
  ib = new SettingParser::Input<double &, ECSettings>(
				      n_insertionDuplication, this,
				      &ECSettings::InsertionDuplication);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_insertionDuplication, ib));

  // ECSettings::InsertionDuplication
  ib = new SettingParser::Input<double &, ECSettings>(
				      n_chanceOfData, this,
				      &ECSettings::ChanceOfData);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_chanceOfData, ib));

  return true;
}
