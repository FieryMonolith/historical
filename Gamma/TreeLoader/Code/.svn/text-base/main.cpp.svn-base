#include <iostream>
#include <cstring>
#include <cstdlib>

#include "ProgramInterface.h"

using namespace std;

void HowTo(void);

int main(int argc, char **argv)
{
  if(argc == 1) { HowTo(); return 0; }

  bool verboseOn = false;
  bool outputToFile = false;
  bool generateReport = false;
  unsigned int samples = 0;
  unsigned int leavesToSample = 0;
  char *historicFilename = 0;
  char *detailFilename = 0;
  for(int i=1; i<argc; i++)
  {
    if(strcmp(argv[i], "-h") == 0)
    {
      if(argc <= i+1) { HowTo(); return 0; }
      historicFilename = argv[i+1];
      ++i;
    }
    else if(strcmp(argv[i], "-d") == 0)
    {
      if(argc <= i+1) { HowTo(); return 0; }
      detailFilename = argv[i+1]; ++i;
    }
    else if(strcmp(argv[i], "-v") == 0)
    {
      verboseOn = true;
    }
    else if(strcmp(argv[i], "-f") == 0)
    {
      outputToFile = true;
    }
    else if(strcmp(argv[i], "-r") == 0)
    {
      generateReport = true;
    }
    else if(strcmp(argv[i], "-s") == 0)
    {
      if(argc <= i+1) { HowTo(); return 0; }
      samples = atoi(argv[i+1]);
      ++i;
    }
    else if(strcmp(argv[i], "-l") == 0)
    {
      if(argc <= i+1) { HowTo(); return 0; }
      leavesToSample = atoi(argv[i+1]);
      ++i;
    }
    else { HowTo(); return 0; }
  }

  Run(historicFilename, detailFilename, verboseOn, outputToFile, 
      generateReport, samples, leavesToSample);

  return 0;
}

void HowTo(void)
{
  cout << "Incorrect inputs to this program:" << endl;
  cout << "TreeLoader [options]" << endl;
  cout << "  -h [historic_file]" << endl;
  cout << "  -d [detail_file]" << endl;
  cout << "  -v                               (verbose output on)" << endl;
  cout << "  -f                               (generator output files)" << endl;
  cout << "  -r                               (generate report files)" << endl;
  cout << "  -s [how_many_samples]            optional" << endl;
  cout << "  -l [quantity_leafs_to_sample]    optional" << endl;

  return;
}
