#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "environment_data.h"

class Body;
class CreatureInfo;
class FileHandler;

class Environment : public EnvironmentData {
public:
  Environment(void);
  ~Environment(void);

  void Shutdown(FileHandler &);

  // Execute these in this order
  bool Initialize(void);
  bool LoadConfiguration(FileHandler &,
			 const std::string &runSettingsFilename,
			 const std::string &physicsSettingsFilename,
			 const std::string &ecSettingsFilename,
			 const std::string &instructionSetFilename);
  bool InitializeExperiment(FileHandler &);

  // Use Run for just running an experiment, use the Process functions
  // for gui use or to allow interaction during and experiment.
  bool Run(FileHandler &);
  bool LoadCreature(FileHandler &, const std::string &fileName);
  bool LoadCreatureFromGeneration(FileHandler &, int fileId);

protected:
  // Need these function in order to be able to update the system from 
  // different locations.  Does not check for initialization!!!
  bool ProcessGenerationStep(double timestep, double &currentTime,
			     double simulationTime,
			     unsigned int &generation,
			     FileHandler &fh, int fileId);
  bool ProcessSimulationStep(double timestep);

private:
  // initialize the experiment
  bool Validate(void) const;
  bool InitializeOutputFiles(FileHandler &, const std::string &filename);
  bool Load(FileHandler &, const std::string &filename);

  bool Evaluate(void);

  // Used to give some randomness to the initial state of the organism so
  // it doesn't evolve to rely on something in particular.
  void Jitter(void);

  bool InitializeCreatureInfo(void);
  bool YBound(const Body &, double &minY, double &maxY) const;
  bool CenterOfMass(Vector3d &out, const Body &) const;

  bool UpdateAllDistance(void);

  // Update xzDistance and yDistance
  bool UpdateDistance(const Body &, CreatureInfo &ci);

  void CalculateInterdistance(void);
  void Interdistance(CreatureInfo &, const Body &) const;
  void UpdateInterdistance(const Body &, CreatureInfo &);

  double CalculateMaxBound(const Body &) const;

  unsigned xyDistanceFit(const double, const bool);
  unsigned interDistanceFit(const unsigned);
  unsigned heightFit(const double);
  unsigned comHeightFit(const double);
  unsigned springFit(const unsigned);
  unsigned pointFit(const unsigned);

  void OutputData(FileHandler &, int fileId, double t);
};

#endif
