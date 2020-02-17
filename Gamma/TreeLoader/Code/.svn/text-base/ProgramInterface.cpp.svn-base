#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>

#include "ProgramInterface.h"

#include "PhylogeneticTree/Interface/Tree.h"
#include "PhylogeneticTree/Interface/iTreeNode.h"
#include "PhylogeneticTree/Interface/Utilities.h"
#include "PhylogeneticTree/Interface/GammaFunctions.h"
#include "Organisms/Interface/Avida.h"
#include "Support/Interface/OutputStream.h"
#include "Support/Interface/random.h"

using namespace std;
using namespace PhylogeneticTree;

/*** Helper Functions *******************************************************/
// Set detailFilename to zero if you want no file output for specific calc.
const double CalculateGamma(const Tree &, const char * const detailFilename);
void Cleanup(Tree **, vector<iOrganism*> &);
void CreateReport(const char *const detailFilename, ofstream &, ofstream &);
void LoadOrganisms(vector<iOrganism*> &organisms,
                   const char * const historicFilename,
                   const char * const detailFilename);
void PrepareTree(Tree &);
const double RunSamples(const Tree &, unsigned int samples,
                        unsigned int leavesToSample,
                        const double trueGamma,
                        const bool generateReport,
                        ostream &reportTxt,
                        ostream &reportCsv,
                        const char * const outFilename);

/*** Global Variables *******************************************************/
// Used to control whether console output is displayed.
OutputStream output(&cout, false);
RandomNumberGenerator rng;

/*** Main Function **********************************************************/
void Run(const char * const historicFilename,
         const char * const detailFilename,
         const bool verboseOn,
         const bool outputToFile,
         const bool generateReport,
         unsigned int samples,
         unsigned int leavesToSample)
{
  // Setup output
  output.SetShowState(verboseOn);

  // Load organisms
  vector<iOrganism*> organisms;
  try { LoadOrganisms(organisms, historicFilename, detailFilename); }
  catch(int) { return; }

  // Create and process the full tree
  Tree *fullTree = 0;
  output << "Creating full tree-" << endl;
  try { fullTree = new Tree(organisms); }
  catch(int x)
  {
    output << "Failed to create tree - " << x << endl;
    Cleanup(0, organisms);
    return;
  }

  try { PrepareTree(*fullTree); }
  catch(int) { Cleanup(&fullTree, organisms); return; }

  // Create Report
  ofstream reportFileTxt;
  ofstream reportFileCsv;
  if(generateReport == true)
  {
    try { CreateReport(detailFilename, reportFileTxt, reportFileCsv); }
    catch(int) { Cleanup(&fullTree, organisms); return; }

    reportFileTxt << "Type   Id       Gamma" << endl;
    reportFileCsv << "Type,Id,Gamma" << endl;
  }

  // Calculate gamma for the full tree
  const double gamma = CalculateGamma(*fullTree,
                                      (outputToFile) ? detailFilename : 0);
  if(generateReport == true)
  {
    reportFileTxt << "Full   0        " << gamma << endl;
    reportFileCsv << "Full,0," << gamma << endl;
  }

  // Calculate samples
  if(samples != 0 && leavesToSample != 0)
  {
    try
    {
      RunSamples(*fullTree, samples, leavesToSample, gamma,
                 generateReport, reportFileTxt, reportFileCsv,
                 (outputToFile) ? detailFilename : 0);
    }
    catch(...)
    {
      output << "Failed to examine samples." << endl;
      // Close reports
      if(generateReport == true)
      {
        reportFileTxt.close();
        reportFileCsv.close();
      }
      Cleanup(&fullTree, organisms);
    }
  }

  // Close reports
  if(generateReport == true)
  {
    reportFileTxt.close();
    reportFileCsv.close();
  }

  // Clean up data
  Cleanup(&fullTree, organisms);

  return;
}

/*** Global Function Definitions ********************************************/
const double CalculateGamma(const Tree &fullTree,
                            const char * const detailFilename)
{
  // Calculate gamma for full tree
  output << "Calculate gamma for full tree-" << endl;
  const double gamma = ComputeGamma(fullTree, 100000, detailFilename,
                                    output);
  output << endl;
  output << "Gamma = " << gamma << endl;
  output << endl;

  return gamma;
}

void Cleanup(Tree **fullTree, vector<iOrganism*> &organisms)
{
  // Clean up tree
  if(fullTree != 0 && *fullTree != 0)
  {
    delete *fullTree;
    *fullTree = 0;
  }

  // Clean up and delete organisms
  vector<iOrganism*>::iterator oi = organisms.begin();
  for(; oi != organisms.end(); ++oi)
  {
    if(*oi != 0)
    {
      delete *oi;
      *oi = 0;
    }
  }

  return;
}

void CreateReport(const char * const detailFilename,
                  ofstream &txt, ofstream &csv)
{
  if(detailFilename == 0)
  {
    output << "Failed to create report file." << endl;
    throw 1;
  }

  char reportFilename[256];
  stringstream ssOutFilename;
  ssOutFilename << detailFilename << ".report" << '\0';
  strcpy(reportFilename, ssOutFilename.str().c_str());

  char reportFileTxt[256];
  char reportFileCsv[256];
  reportFileTxt[0] = '\0';
  reportFileCsv[0] = '\0';
  strcpy(reportFileTxt, reportFilename);
  strcpy(reportFileCsv, reportFilename);
  strcat(reportFileTxt, ".txt");
  strcat(reportFileCsv, ".csv");

  try
  {
    txt.open(reportFileTxt);
    csv.open(reportFileCsv);
    if(!txt.good() || !csv.good()) { throw 0; }
  }
  catch(...)
  {
    output << "Failed to create report file." << endl;
    if(txt.good()) { txt.close(); }
    if(csv.good()) { csv.close(); }
    throw 2;
  }

  return;
}

void LoadOrganisms(vector<iOrganism*> &organisms,
                   const char * const historicFilename,
                   const char * const detailFilename)
{
  // open input files
  ifstream detailIn(detailFilename);
  ifstream historicIn(historicFilename);
  if(!detailIn || !historicIn)
  {
    if(!historicIn)   { output << "Could not open historic file." << endl; }
    else              { historicIn.close(); }

    if(!detailIn) { output << "Could not open detail file."   << endl; }
    else          { detailIn.close(); }

    throw 1;
  }
  output << "Historic and detail files opened." << endl << endl;

  // Load in organisms from file.
  output << "Loading input files- " << endl;

  try
  {
    // load files
    output << "Loading historic file        ... ";
    LoadAvidaOrganisms(organisms, historicIn, false);
    output << "Loaded." << endl;

    output << "Loading detail file          ... ";
    LoadAvidaOrganisms(organisms, detailIn, true);
    output << "Loaded." << endl;
  }
  catch(pair<int,int> errorData)
  {
    // output the error that occurred while processing a line
    output << "Failed." << endl;
    output << errorData.first << "\t error - " << errorData.second;
    output << endl;
    output << "Abandoning build." << endl;
    Cleanup(0, organisms);

    // Close input files
    historicIn.close();
    detailIn.close();

    throw 2;
  }

  // Close input files
  historicIn.close();
  detailIn.close();

  // Check for duplicate organism ids
  output << "Checking for duplicate ids   ... ";
  try
  {
    set<int> duplicateIds = CheckForDuplicateIds(organisms);
    if(!duplicateIds.empty()) { throw 0; }
  }
  catch(int)
  {
    output << "found.  Aborting ..." << endl;
    throw 3;
  }
  output << "none." << endl;
  output << endl;

  return;
}

void PrepareTree(Tree &fullTree)
{
  // Validate all nodes, no missing nodes in the list and valid ids/parentId
  //output << "Validating tree              ... ";
  //if(Validate(fullTree) == false)
  //{
  //  output << "failed." << endl;
  //  return false;
  //}
  //output << "Complete." << endl;

  // Checking for cycles
  //output << "Checking for cycles          ... ";
  //if(CheckForCycles(fullTree) == true)
  //{
  //  output << "found." << endl;
  //  return false;
  //}
  //output << "Complete." << endl;

  // Prune leaves from the tree that are not in the final population, if any
  output << "Prune non-living leaves      ... ";
  if(!PruneDeadLeaves(fullTree))
  {
    output << "Failed." << endl;
    throw 1;
  }
  output << "Complete." << endl;

  // Remove non-furcating nodes except the root and leaves
  output << "Removing non-furcating nodes ... ";
  RemoveNonfurcatingNodes(fullTree);
  output << "Complete." << endl;

  // Output tree information
  output << endl;
  if(output.GetShowState() == true && output.GetStream() != 0 &&
     PrintInformation(fullTree, *output.GetStream()) == false)
  {
    output << "Failed to retrieve tree information." << endl;
    throw 2;
  }

  return;
}

const double RunSamples(const Tree &fullTree,
                        unsigned int inputSamples,
                        unsigned int inputLeavesToSample,
                        const double trueGamma,
                        const bool generateReport,
                        ostream &reportTxt,
                        ostream &reportCsv,
                        const char * const outFilename)
{
  if(inputSamples == 0 || inputLeavesToSample == 0) { return 0; }

  double total = 0;
  unsigned int samples = inputSamples;
  unsigned int sampleSize = inputLeavesToSample;
  for(unsigned int i = 0; i < samples; ++i)
  {
    Tree *sampleTree = 0;
    try
    {
      output << "Working on sample " << i+1 << endl;

      sampleTree = Sample(fullTree, sampleSize, rng);
      if(sampleTree == 0)
      {
        output << "Failed to create a sample from the full tree." << endl;
        throw 1;
      }

      try { PrepareTree(*sampleTree); }
      catch(int)
      {
        output << "Failed to prepare sample tree." << endl;
        throw 2;
      }

      char *name = 0;
      char filename[256];
      if(outFilename)
      {
        stringstream ssOutFilename;
        ssOutFilename << outFilename << ".sample" << i+1 << '\0';
        strcpy(filename, ssOutFilename.str().c_str());
        name = filename;
      }

      const double gamma = CalculateGamma(*sampleTree, name);
      if(generateReport)
      {
        reportTxt << "Sample ";
        reportTxt.width(8);
        reportTxt << left << i+1;
        reportTxt << " " << gamma;
        reportTxt << endl;
        reportCsv << "Sample," << i+1 << "," << gamma << endl;
      }
      total += gamma;
    }
    catch(int)
    {
      output << "Failed to sample the full tree." << endl;
    }
    if(sampleTree != 0) { delete sampleTree; }
  }

  output << "True gamma = " << trueGamma << endl;
  output << "Average gamma = " << total << " / " << samples << " = ";
  double average = total / samples;
  output << average << endl;

  return average;
}
