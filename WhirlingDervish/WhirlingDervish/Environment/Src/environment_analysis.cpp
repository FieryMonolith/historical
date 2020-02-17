#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <cassert>
#ifdef VISUAL_STUDIO
#include <cfloat>
#endif

#include "environment_analysis.h"

#include "physics.h"
#include "creature.h"
#include "physical_point.h"
#include "physical_point_info.h"
#include "spring.h"
#include "spring_info.h"
#include "random.h"
#include "population_storage.h"
#include "creature_info.h"
#include "language_engine.h"
#include "file_handler.h"

#include "parser.h"
#include "spider.h"

using namespace std;

Environment::Environment(void) {
  return;
}

Environment::~Environment(void) {
  return;
}

void Environment::Shutdown(FileHandler &fh) {
  return;

  if(d_outputFileId > 0) {
    fh.Close(d_outputFileId);
  }

  return;
}

/*
 * BEGIN Initialize the experiment.
 */

/*
 * Initialize the experiment.
 *   1. Create necessary data structures
 *   2. Load configuration files
 *   3. Validate some of the system settings
 *   4. Create population
 *   5. Load creatures if specified
 *   6. Create output files
 */

bool Environment::Initialize(void) {
  if(!InitializeData()) {
    cerr << "ERROR: Failed to initialize data." << endl;
    return false;
  }

  return true;
}

bool Environment::LoadConfiguration(FileHandler &fh,
				    const string &runSettingsFilename,
				    const string &physicsSettingsFilename,
				    const string &ecSettingsFilename,
				  const string &instructionSetSettingsFilename)
{
  return LoadConfigurationFiles(fh,
				runSettingsFilename,
				physicsSettingsFilename,
				ecSettingsFilename,
				instructionSetSettingsFilename);
}

bool Environment::InitializeExperiment(FileHandler &fh) {
  if(!Validate()) {
    return false;
  }

  if(!GetLanguageEngine().PrepareInstructionSet()) {
    cerr << "ERROR: Could not prepare the instruction set." << endl;
    return false;
  }

  if(!d_populationStorage.Initialize(d_runSettings.PopulationSize(),
				     (EnvironmentData &)(*this))) {
    cerr << "ERROR: Failed to initialize population." << endl;
    return false;
  }

  if(d_runSettings.RandomSeed() < -1) {
    GetRng().ResetSeed(-1);
  } else {
    GetRng().ResetSeed(d_runSettings.RandomSeed());
  }

  if(d_runSettings.StartState() == 1 || d_runSettings.StartState() == 2) {
    Load(fh, d_runSettings.LoadFile());
  }

  // Initialize output files
  if(d_runSettings.StartState() != 3 &&
     !InitializeOutputFiles(fh, d_runSettings.StoreFile())) {
    cerr << "ERROR: InitializeOutputFiles failed." << endl;
    return false;
  }

  // Save the rng state before jitter since it applies to the phenotype, the
  // jitter information is not saved.
  if(!d_randomTemp.LoadState(d_random.DumpState())) {
    cerr << "ERROR: Could not load the random number generator state." << endl;
    return false;
  }

  if(!InitializeCreatureInfo()) {
    cerr << "ERROR: Could not initialize creature info statistics." << endl;
    return false;
  }
  Jitter();

  return true;
}

bool Environment::Load(FileHandler &fh, const string &filename) {
  int t1, t2;
  double t4;

  int inId = fh.Create(filename, ios::in);
  if(inId <= 0) { return false; }

  // Don't use these anymore, but need to read them anyways
  if(!fh.Input(inId, t1, t2, t4)) {
    cerr << "ERROR: Failed to load experiment parameters." << endl;
    return false;
  }

  // Read in the creatures
  if(d_runSettings.StartState() == 1) {
    // Don't load random number generator state
    RandomNumberGenerator rng;
    if(!fh.Input(inId, rng, (EnvironmentData &)(*this),
		 GetPopulationStorage(), GetLanguageEngine())) {
      cerr << "ERROR: Failed to load proper generation." << endl;
      return false;
    }
  } else {
    if(!fh.Input(inId, GetRng(), (EnvironmentData &)(*this),
		 GetPopulationStorage(), GetLanguageEngine())) {
      cerr << "ERROR: Failed to load proper generation." << endl;
      return false;
    }
  }

  fh.Close(inId);

  return true;
}

// Validate parameters
bool Environment::Validate(void) const {
  if(d_runSettings.PopulationSize() <= 0) {
    cerr << "ERROR: Invalid population size." << endl;
    return false;
  }

  if(d_runSettings.MaxGenerations() <= 0) {
    cerr << "ERROR: Invalid number of generations." << endl;
    return false;
  }

  if(d_physicsSettings.Timestep() <= 0) {
    cerr << "ERROR: Invalid time step." << endl;
    return false;
  }

  if(d_runSettings.StartState() > 3) {
    cerr << "ERROR: Invalid start state." << endl;
    return false;
  }

  return true;
}

bool Environment::InitializeOutputFiles(FileHandler &fh,
					const string &filename) {
  return true;

  d_outputFileId = fh.Create(filename, ios::out);
  if(d_outputFileId <= 0) { return false; }

  if(!fh.Output(d_outputFileId,
		d_populationStorage.PopulationSize(),
		d_runSettings.MaxGenerations(),
		d_physicsSettings.Timestep())) {
    cerr << "ERROR: Output file initialization failed!" << endl;
    return false;
  }

  return true;
}

/*
 * END Initialize the experiment.
 */

/*
 * BEGIN Run the experiment.
 */

bool Environment::Run(FileHandler &fh) {
  unsigned int t = 0;

  OutputData(fh, d_outputFileId, d_currentTime);

  //while(d_generation < d_runSettings.MaxGenerations()) {
  while(d_generation < 2) {
    if(t != d_generation) {
      //cout << "Generation " << d_generation << endl;
      t = d_generation;
    }

    if(!ProcessGenerationStep(d_physicsSettings.Timestep(), d_currentTime,
			      d_physicsSettings.Simtime(),
			      d_generation, fh, d_outputFileId)) {
      cerr << "ERROR: ProcessGenerationStep failed." << endl;
      return false;
    }
  }

  d_currentTime = 0.0;
  d_generation = 1;

  return true;

  fh.Close(d_outputFileId);

  return true;
}

bool Environment::ProcessGenerationStep(double timestep, double &currentTime,
					double simulationTime,
					unsigned int &generation,
					FileHandler &fh, int fileId) {
  if(timestep <= 0) {
    cerr << "ERROR: Invalid timestep." << endl;
    return false;
  }

  if(!ProcessSimulationStep(timestep)) {
    cerr << "ERROR: Process failed." << endl;
    return false;
  }

  int beforeTime = (int)currentTime;

  currentTime += timestep;

  int afterTime = (int)currentTime;

  OutputData(fh, fileId, currentTime);

  if(afterTime > beforeTime) {
    vector<CreatureInfo *> &vci = d_populationStorage.Population();
    vector<CreatureInfo *>::iterator i;
    for(i=vci.begin(); i!=vci.end(); ++i) {
      if(*i == NULL) {
	cerr << "ERROR: A Creature pointer was NULL in Process." << endl;
	return false;
      }

      Body &body = (*i)->GetCreature().GetBody();
      Vector3d out;
      if(!CenterOfMass(out, body)) {
	cerr << "ERROR: Problem calculating center of mass." << endl;
	return false;
      }

      if(afterTime == 30) {
	(*i)->COMPosition30() = out;
      } else if(afterTime == 75) {
	(*i)->COMPosition75() = out;
      } else if(afterTime == 60) {
	(*i)->COMPosition60() = out;
      }
    }

    //UpdateAllDistance();
  }

  // Return if we are not yet done with this generation...
  if(currentTime < simulationTime) {
    return true;
  }

  // If we are finished with this generation, prepare for next generation...
  currentTime = 0.0;

  d_runSettings.Algorithm() = 14;
  if(!Evaluate()) {
    cerr << "ERROR: Evaluation failed." << endl;
    return false;
  }
  cout << BestFitness() << " ";
  d_runSettings.Algorithm() = 15;
  if(!Evaluate()) {
    cerr << "ERROR: Evaluation failed." << endl;
    return false;
  }
  cout << BestFitness() << " " << BestPoints() << " " << BestSprings() << " ";
  cout << BestHeight() << " " << BestMove() << endl;


#if 0
  if(fileId > 0 && !fh.Output(fileId, generation, d_randomTemp,
			      GetPopulationStorage())) {
    cerr << "ERROR: Recording current generation" << endl;
    return false;
  }

  if(!d_populationStorage.Reproduce((EnvironmentData &)(*this), GetRng(),
				    GetLanguageEngine())) {
    cerr << "ERROR: Failed to create the next generation." << endl;
    return false;
  }

  if(!d_randomTemp.LoadState(d_random.DumpState())) {
    cerr << "ERROR: Could not load the random number generator state." << endl;
    return false;
  }

  if(!InitializeCreatureInfo()) {
    cerr << "ERROR: Could not initialize creature info statistics." << endl;
    return false;
  }
  Jitter();
#endif

  generation++;

  return true;
}

bool Environment::Evaluate(void) {
  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  vector<CreatureInfo *>::iterator i;

  const unsigned int maxPoints = 10;
  //const unsigned int maxSprings = 20;
  const unsigned int maxSprings = 54;
  const double maxHeight = 9.0;
  // This can give a 15x15x15 meter^3  creature bound.
  const double maxBound = 26.0;
  int genotypes = 0;

  unsigned int d_totalPoints = 0;
  int d_totalSprings = 0;

  AvgPoints() = 0.0;
  MaxPoints() = 0;
  AvgSprings() = 0.0;
  MaxSprings() = 0;
  AvgFitness() = 0.0;
  MaxFitness() = 0.0;
  AvgHeight() = 0.0;
  MaxHeight() = 0.0;
  AvgMove() = 0.0;
  MaxMove() = 0.0;
  BestFitness() = 0.0;
  BestPoints() = 0;
  BestSprings() = 0;
  BestHeight() = 0.0;
  BestMove() = 0.0;

  bool firstFitness = true;

  if(!UpdateAllDistance()) { return false; }

  for(i=vci.begin(); i!=vci.end(); ++i) {
    if(*i == NULL) { return false; }

    const Creature &c = (*i)->GetCreature();
    const Body &body = c.GetBody();

    int pointCount = (int)body.PointCount();
    d_totalPoints += pointCount;
    if((unsigned int)pointCount > MaxPoints()) {
      MaxPoints() = (unsigned int)pointCount;
    }
    if(pointCount > (int)maxPoints) { pointCount = (int)maxPoints; }

    int springCount = (int)body.SpringCount();
    d_totalSprings += springCount;
    if((unsigned int)springCount > MaxSprings()) {
      MaxSprings() = (unsigned int)springCount;
    }
    // If there are too many springs, they start to detract from the fitness
    // by removing the benefit from the points.
    if(springCount > (int)maxSprings) {
      springCount = (int)maxSprings - springCount;
    }

    double minY, maxY;
    if(!YBound(body, minY, maxY)) { return false; }

    double originalHeight = (*i)->OriginalHeight();
    double height = maxY - minY;
    double trueHeight = height;
    if(height > MaxHeight()) { MaxHeight() = height; }
    if(height > maxHeight) { height = maxHeight; }

    double originalCOMHeight = (*i)->OriginalCenterOfMass()[1];
    originalCOMHeight -= (*i)->OriginalMinY();
    double comHeight = (*i)->CenterOfMass()[1];
    comHeight -= minY;
    if(comHeight > maxHeight) { comHeight = maxHeight; }

    double xzDistance = (*i)->DistanceMoved();
    if(xzDistance > MaxMove()) { MaxMove() = xzDistance; }

    //double yDistance = fabs((*i)->DistanceY());
    //UpdateInterdistance(body, **i);
    //double interdistance = (*i)->InterdistanceQuantity();

    bool moved = false;
    if(comHeight > 0.0254 * 1.0 && height  > 0.0254 * 3.0 &&
       comHeight > originalCOMHeight / 3.0 && height > originalHeight / 3.0) {
      moved = true;
    }

    Vector3d com60 = (*i)->COMPosition60();
    com60[1] = 0.0;
    Vector3d com30 = (*i)->COMPosition30();
    com30[1] = 0.0;
    Vector3d com75 = (*i)->COMPosition75();
    com75[1] = 0.0;
    Vector3d com120 = (*i)->CenterOfMass();
    com120[1] = 0.0;
    double distA = Distance(com30, com75);
    double distB = Distance(com75, com120);
    if(distB < distA) {
      double tempA = distA;
      distA = distB;
      distB = tempA;
    }

    double distC = Distance(com60, com120);

    // Calculate fitness
    // Values Available: xzDistance, yDistance, height, comHeight
    //                   interdistance, pointCount, springCount
    unsigned int algorithm = d_runSettings.Algorithm();
    double fitness = 0.0;
    
    unsigned xyFit = 0;
    unsigned hFit = 0;
    unsigned comHFit = 0;
    unsigned sFit = 0;
    unsigned pFit = 0;

    bool boundaries = true;

    switch(algorithm) {
    case 0:
      if(moved) {
	AvgMove() += xzDistance;
	fitness += 3.0 * xzDistance;
      }
      fitness += 0.0254 * pointCount;
      fitness += height;
      fitness += 3.0 * comHeight;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 1:
      moved = true;
      AvgMove() += xzDistance;
      fitness += 3.0 * xzDistance;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 2:
      moved = true;
      AvgMove() += xzDistance;
      fitness += 3.0 * xzDistance;
      fitness += 0.0254 * pointCount;
      fitness += height;
      fitness += 3.0 * comHeight;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 3:
      if(moved) {
	AvgMove() += xzDistance;
	fitness += 3.0 * xzDistance;
      }
      fitness += height;
      fitness += 3.0 * comHeight;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 4:
      if(moved) {
	AvgMove() += xzDistance;
	fitness += 3.0 * xzDistance;
      }
      fitness += 0.0254 * pointCount;
      fitness += 3.0 * comHeight;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 5:
      if(moved) {
	AvgMove() += xzDistance;
	fitness += 3.0 * xzDistance;
      }
      fitness += 0.0254 * pointCount;
      fitness += height;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 6:
      if(moved) {
	AvgMove() += xzDistance;
	fitness += 3.0 * xzDistance;
      }
      fitness += 0.0254 * pointCount;
      fitness += height;
      fitness += 3.0 * comHeight;
      break;
    case 7:
      moved = true;
      AvgMove() += xzDistance;
      fitness += 3.0 * xzDistance;
      fitness += 0.0254 * pointCount;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 8:
      moved = true;
      AvgMove() += xzDistance;
      fitness += 3.0 * xzDistance;
      fitness += height;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 9:
      moved = true;
      AvgMove() += xzDistance;
      fitness += 3.0 * xzDistance;
      fitness += 3.0 * comHeight;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 10:
      moved = true;
      AvgMove() += xzDistance;
      fitness += 3.0 * xzDistance;
      fitness += height;
      fitness += 3.0 * comHeight;
      if(springCount < 0) { fitness += springCount; }
      break;
    case 11:
      moved = true;
      AvgMove() += xzDistance;
      // Selective pressure is on the smallest distance moved
      fitness = (0.9 * distA) + (0.1 * distB);
      if(springCount < 0) { fitness += springCount; }
      break;
    case 12:
      moved = true;
      //charles & art's point based fitness
      fitness = 0;
      (*i)->vFitness().clear();
      xyFit = xyDistanceFit(xzDistance, moved);
      hFit = heightFit(height);
      comHFit = comHeightFit(comHeight);
      sFit = springFit(springCount);
      pFit = pointFit(pointCount);

      (*i)->vFitness().push_back(xyFit);
      (*i)->vFitness().push_back(hFit);
      (*i)->vFitness().push_back(comHFit);
      (*i)->vFitness().push_back(sFit);
      (*i)->vFitness().push_back(pFit);

      /*cerr << "{" << (*i)->vFitness().at(0) << ", "
	          << (*i)->vFitness().at(1) << ", "
	          << (*i)->vFitness().at(2) << ", "
	          << (*i)->vFitness().at(3) << ", "
	          << (*i)->vFitness().at(4) << "}" << endl;
      */
      fitness = xyFit + hFit + comHFit + sFit + pFit;
      break;
    case 13:
      if(moved) {
	AvgMove() += xzDistance;
	fitness += 3.0 * ((0.9 * distA) + (0.1 * distB));
      }
      fitness += 0.0254 * pointCount;
      fitness += height;
      fitness += 3.0 * comHeight;
      if(springCount < 0) { fitness += springCount; }
      break;
      AvgMove() += fitness;
      break;
    case 14:
      boundaries = false;
      moved = true;
      AvgMove() += xzDistance;
      fitness += xzDistance;
      //if(springCount < 0) { fitness += springCount; }
      break;
    case 15:
      boundaries = false;
      moved = true;
      AvgMove() += distC;
      fitness += distC;
      //if(springCount < 0) { fitness += springCount; }
      break;
    default:
      cout << "ERROR: Unknown fitness function type." << endl;
      return false;
    };

    if(boundaries) {
      // Boundaries for organisms, when exceeded, cause fitness to go to zero
      double bound = CalculateMaxBound(body);
#ifdef VISUAL_STUDIO
      if(_isnan(bound)) { fitness = 0.0; }
#else
      if(isnan(bound)) { fitness = 0.0; }
#endif
      //else if(bound > maxBound) { fitness += maxHeight - bound; }
      if(bound > maxBound) { fitness = 0.0; }
      if(height > 30.0) { fitness = 0.0; }
      if(maxY > 30.0) { fitness = 0.0; }
    }

    if(fitness < 0) { fitness = 0.0; }

    // Record information about best organism
    if(fitness > BestFitness() || firstFitness) {
      firstFitness = false;
      BestFitness() = fitness;
      BestPoints() = body.PointCount();
      BestSprings() = body.SpringCount();
      BestHeight() = trueHeight;
      BestMove() = 0.0;
      if(moved) {
	BestMove() = xzDistance;
      }
    }

    // Set calculated fitness
    (*i)->Fitness() = fitness;

    // Gather statistics
    genotypes++;
    AvgHeight() += height;
    AvgFitness() += fitness;
    if(fitness > MaxFitness()) { MaxFitness() = fitness; }
  }

  if(genotypes > 0) {
    AvgPoints()   = (double)d_totalPoints / (double)genotypes;
    AvgSprings()  = (double)d_totalSprings  / (double)genotypes;
    AvgHeight()  /= (double)genotypes;
    AvgFitness() /= (double)genotypes;
    AvgMove()    /= (double)genotypes;
  }

  return true;
}

/*
 * END Run the experiment.
 */

/*
 * BEGIN Simulation
 */

// Need this function in order to be able to update the system from 
// different locations
bool Environment::ProcessSimulationStep(double timestep) {
  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  if(vci.empty()) { return true; }

  // Reuse variables
  map<int64, PhysicalPointInfo *>::const_iterator ipoints;
  map<int64, Body::ConnectionData>::const_iterator ispring;
  Vector3d dragForce;
  Vector3d gravitationalForce;
  Vector3d resistiveForce;
  Vector3d force1, force2;

  // Process each creature
  // 1. Calculate Spring force
  // 2. Gravity
  // 3. Drag
  // 4. Temp Verlet to determine where the point-mass would be
  // 5. Friction
  // 6. Verlet, move point
  // 7. Adjust the point if it collides or goes below the ground
  // 8. Reset force
  // 9. Test for point, motion
  vector<CreatureInfo *>::iterator ci;
  for(ci=vci.begin(); ci!=vci.end(); ++ci) {
    if(*ci == NULL) {
      cerr << "ERROR: A Creature pointer was NULL in Process." << endl;
      return false;
    }

    if((*ci)->Still() || (*ci)->Pause()) { continue; }

    Body &body = (*ci)->GetCreature().GetBody();

    const map<int64, PhysicalPointInfo *> &points = body.GetPoints();
    if(points.empty()) { continue; }

    // 1. Calculate spring force
    const map<int64, Body::ConnectionData> &springs = body.Connections();
    for(ispring = springs.begin(); ispring != springs.end(); ++ispring) {
      const Body::ConnectionData &cd = ispring->second;

      if(cd.d_spring == NULL || cd.d_point1 == NULL || cd.d_point2 == NULL) {
	cerr << "ERROR: A Spring pointer was NULL in Process." << endl;
	return false;
      }

      if(cd.d_spring->Pause()) { continue; }

      Spring &spring    = cd.d_spring->GetSpring();
      PhysicalPoint &p1 = cd.d_point1->GetPhysicalPoint();
      PhysicalPoint &p2 = cd.d_point2->GetPhysicalPoint();

      Physics::SpringForce(force1, force2,
			   p1.Position(), p1.PreviousPosition(),
			   p2.Position(), p2.PreviousPosition(),
			   spring.ks(), spring.kd(), spring.InnateLength(),
			   timestep);
      p1.AddForce(force1);
      p2.AddForce(force2);

      spring.Fluctuate(timestep, d_physicsSettings.MaxPercentFluctuation());
    }

    //bool isStill = true;
    for(ipoints = points.begin(); ipoints != points.end(); ++ipoints) {
      if(ipoints->second == NULL) {
	cerr << "ERROR: A PhysicalPoint pointer was NULL in Process.";
	cerr << endl;
	return false;
      }
      PhysicalPoint &p = ipoints->second->GetPhysicalPoint();

      if(ipoints->second->Pause()) {
	p.Force() = Vector3d(0.0, 0.0, 0.0);
	continue;
      }

      // 2. Process Gravity
      Physics::CalculateGravitationalForce(p.Mass(),
					   d_physicsSettings.Gravity(),
					   gravitationalForce);
      p.AddForce(gravitationalForce);

      // 3. Process Drag
      //Physics::CalculateAirResistance(p.Drag(), p.Velocity(timestep),
      //dragForce);
      Physics::CalculateAirResistance(d_physicsSettings.Drag(),
				      p.Velocity(timestep),
				      dragForce);
      p.AddForce(dragForce);

      // 4. Temp Verlet to determine where the point-mass would be
      Vector3d tempP1(p.Position());
      Vector3d tempP2(p.PreviousPosition());
      Physics::Verlet(tempP1, tempP2, p.Force(), p.Mass(), timestep);

      // 5. Calculate Friction
      //if(tempP1[1] < 0.01) {
      if(tempP1[1] < 0) {
	//      if(p.Position()[1] < 0.01) {
	Physics::CalculateFrictionalForce(d_physicsSettings.StaticMu(),
					  d_physicsSettings.KineticMu(),
					  p.Mass(),
					  p.Force(),
					  p.Velocity(timestep),
					  resistiveForce);
	p.AddForce(resistiveForce);
      }

      // 6. Verlet, move point
      Physics::Verlet(p.Position(), p.PreviousPosition(), p.Force(),
		      p.Mass(), timestep);

      // 7. Adjust the point if it collides or goes below the ground
      if(p.Position()[1] < 0) {
	p.Position()[1] *= -1.0;
	p.PreviousPosition()[1] *= -1.0;
	Vector3d tv(p.PreviousPosition() - p.Position());
	tv *= 0.4;
	p.PreviousPosition() = p.Position() + tv;
	//double tvMag = Normalize(tv);
  
	//tvMag *= 0.4;
	//p.PreviousPosition() = p.Position() + (tv * tvMag);
      }
      // If the point is within 1 micrometer from the ground, shift the
      // point down so that the current point is on the ground.
      if(p.Position()[1] < 0.000001) {
	p.PreviousPosition()[1] -= p.Position()[1];
	p.Position()[1] = 0;
      }

      // 8. Reset force
      p.Force() = Vector3d(0.0, 0.0, 0.0);
    }
  }

  return true;
}
/*
 * END Simulation
 */

void Environment::Jitter(void) {
#if 0
  map<int64, PhysicalPointInfo *>::const_iterator i;
  const map<int64, PhysicalPointInfo *> &points = d_shapeEngine.Points();
  for(i=points.begin(); i!=points.end(); ++i) {
    if(i->second == NULL) { continue; }

    Vector3d &p = i->second->GetPhysicalPoint().Position();
    Vector3d &pp = i->second->GetPhysicalPoint().PreviousPosition();
    RandomNumberGenerator &rng = GetRng();

    Vector3d offset(rng.GetDouble(), 0.0, rng.GetDouble());
    offset *= 0.000001;

    p += offset;
    pp += offset;
  }
#endif

  return;
}

bool Environment::InitializeCreatureInfo(void) {
  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  if(vci.empty()) { return true; }

  vector<CreatureInfo *>::iterator ci;
  for(ci=vci.begin(); ci!=vci.end(); ++ci) {
    if(*ci == NULL) {
      cerr << "ERROR: A Creature pointer was NULL in Process." << endl;
      return false;
    }

    (*ci)->DistanceMoved() = 0.0;
    (*ci)->InterdistanceQuantity() = 0.0;

    Body &body = (*ci)->GetCreature().GetBody();

    double minY, maxY;
    if(!YBound(body, minY, maxY)) {
      return false;
    }
    (*ci)->OriginalHeight() = maxY - minY;
    (*ci)->OriginalMinY() = minY;

    if(!CenterOfMass((*ci)->CenterOfMass(), body)) {
      return false;
    }
    (*ci)->OriginalCenterOfMass() = (*ci)->CenterOfMass();
  }

  return true;
}

bool Environment::YBound(const Body &body, double &minY, double &maxY) const {
  bool first = true;

  minY = 0.0;
  maxY = 0.0;

  const map<int64, PhysicalPointInfo *> &p = body.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator ip;

  for(ip=p.begin(); ip!=p.end(); ++ip) {
    if(ip->second == NULL) { return false; }

    double y = ip->second->GetPhysicalPoint().Position()[1];

    if(first) {
      minY = y;
      maxY = y;
      first = false;
    } else if(y < minY) {
      minY = y;
    } else if(y > maxY) {
      maxY = y;
    }
  }

  return true;
}

bool Environment::CenterOfMass(Vector3d &out, const Body &b) const {
  double totalMass = 0.0;
  out = Vector3d(0.0, 0.0, 0.0);

  // Scale the y such that the minY is put on the y axis, for normalization,
  // since we only want height or the organism
  const map<int64, PhysicalPointInfo *> &p = b.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator ip;
  for(ip=p.begin(); ip!=p.end(); ++ip) {
    if(ip->second == NULL) { return false; }

    const PhysicalPoint &p = ip->second->GetPhysicalPoint();

    out += (p.Position() * p.Mass());
    totalMass += p.Mass();
  }

  if(totalMass <= 0) {
    out = Vector3d(0.0, 0.0, 0.0);

    return true;
  }

  out *= (1.0 / totalMass);

  return true;
}

bool Environment::UpdateAllDistance(void) {
  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  if(vci.empty()) { return true; }

  vector<CreatureInfo *>::iterator ci;
  for(ci=vci.begin(); ci!=vci.end(); ++ci) {
    if(*ci == NULL) {
      cerr << "ERROR: A Creature pointer was NULL in Process." << endl;
      return false;
    }

    Body &body = (*ci)->GetCreature().GetBody();
    if(!UpdateDistance(body, **ci)) {
      return false;
    }
  }

  return true;
}

bool Environment::UpdateDistance(const Body &b, CreatureInfo &ci) {
  Vector3d oldCOM(ci.CenterOfMass());
  double oldY = oldCOM[1];
  oldCOM[1] = 0.0;

  // Store current center of mass
  if(!CenterOfMass(ci.CenterOfMass(), b)) {
    return false;
  }

  Vector3d newCOM(ci.CenterOfMass());
  double newY = newCOM[1];
  newCOM[1] = 0.0;

  // Update the distance moved by incrementing by a delta,
  // the distance between the current center of mass and the previous one.
  ci.DistanceMoved() += Distance(newCOM, oldCOM);
  if(newY < oldY) {
    ci.DistanceY() += (oldY - newY);
  } else {
    ci.DistanceY() += (newY - oldY);
  }

  return true;
}

void Environment::CalculateInterdistance(void) {
  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  if(vci.empty()) { return; }

  vector<CreatureInfo *>::iterator ci;
  for(ci=vci.begin(); ci!=vci.end(); ++ci) {
    if(*ci == NULL) {
      cerr << "ERROR: A Creature pointer was NULL in Process." << endl;
      return;
    }

    Body &body = (*ci)->GetCreature().GetBody();
    Interdistance(**ci, body);
  }

  return;
}

void Environment::Interdistance(CreatureInfo &ci, const Body &b) const {
  const map<int64, Body::ConnectionData> &cd = b.Connections();
  map<int64, Body::ConnectionData>::const_iterator icd;

  map<int64, double> &d_id = ci.Interdistance();
  d_id.clear();

  for(icd = cd.begin(); icd != cd.end(); ++icd) {
    if(icd->second.d_point1 == NULL || icd->second.d_point2 == NULL) {
      assert(0);
    }

    double d = Distance(icd->second.d_point1->GetPhysicalPoint().Position(),
			icd->second.d_point2->GetPhysicalPoint().Position());
    d_id.insert(make_pair(icd->first, d));
  }

  return;
}

void Environment::UpdateInterdistance(const Body &b, CreatureInfo &ci) {
  map<int64, double> oldInterdistance;
  // a.swap(b) -> swap(a,b) -> swap from a to b
  ci.Interdistance().swap(oldInterdistance);

  // Store current center of mass
  Interdistance(ci, b);

  // Sum the deviation for each spring of the distance between its two points
  const map<int64, double> &cid = ci.Interdistance();
  map<int64, double>::const_iterator icid;
  map<int64, double>::const_iterator ioldcid;
  // I do this direction of go through current and look up old in case
  // some time in the future I allow springs to be removed, so we only
  // want to know about the ones that are there now.
  for(icid=cid.begin(); icid!=cid.end(); ++icid) {
    ioldcid = oldInterdistance.find(icid->first);
    if(ioldcid == oldInterdistance.end()) {
      continue;
    }

    ci.InterdistanceQuantity() += fabs(ioldcid->second - icid->second);
  }

  return;
}

double Environment::CalculateMaxBound(const Body &b) const {
  const map<int64, PhysicalPointInfo *> &p = b.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator ip;

  double minX=0.0, minY=0.0, minZ=0.0;
  double maxX=0.0, maxY=0.0, maxZ=0.0;
  bool first = true;

  for(ip=p.begin(); ip!=p.end(); ++ip) {
    if(ip->second == NULL) { return 0.0; }

    const PhysicalPoint &p = ip->second->GetPhysicalPoint();

    double x = p.Position()[0];
    double y = p.Position()[1];
    double z = p.Position()[2];

    if(first) {
      first = false;
      minX = maxX = x;
      minY = maxY = y;
      minZ = maxZ = z;
    } else {
      if(x < minX) { minX = x; }
      if(x > maxX) { maxX = x; }
      if(y < minY) { minY = y; }
      if(y > maxY) { maxY = y; }
      if(z < minZ) { minZ = z; }
      if(z > maxZ) { maxZ = z; }
    }
  }

  Vector3d p0(minX, minY, minZ);
  Vector3d p1(maxX, maxY, maxZ);

  return Distance(p0, p1);
}

unsigned Environment::xyDistanceFit(const double xyd, const bool moved){
    
     if(moved){
          if(xyd > 25.0) return 9;
          if(xyd > 20.0) return 8;
          if(xyd > 15.0) return 7;
          if(xyd > 10.0) return 6;
          if(xyd >  5.0) return 5;
          if(xyd >  2.0) return 4;
          if(xyd >  1.5) return 3;
          if(xyd >  1.0) return 2;
          if(xyd >  0.5) return 1;
          if(xyd >  0.0) return 0;
     }
     return 0;
}


unsigned Environment::heightFit(const double h){

  //return 0;
     if(h > 25.0) return 9;
     if(h > 20.0) return 8;
     if(h > 15.0) return 7; 
     if(h > 11.0) return 6;
     if(h > 8.0) return 5;
     if(h > 5.0) return 4;
     if(h > 2.0) return 3;
     if(h > 1.5) return 2;
     if(h > 1.0) return 1;
     if(h > 0.5) return 0;

     return 0;
}


unsigned Environment::comHeightFit(const double h){
     if( h > 10.0) return 9;
     if( h > 9.0) return 8;
     if( h > 8.0) return 7;
     if( h > 7.0) return 6;
     if( h > 6.0) return 5;
     if( h > 5.0) return 4;
     if( h > 4.0) return 3;
     if( h > 3.0) return 2;
     if( h > 2.0) return 1;
     if( h > 1.0) return 0;
     return 0;
}

unsigned Environment::springFit(const unsigned s){
     if(s >= 0 && s < 2) return 0;
     if(s >= 2 && s < 5) return 1;
     if(s >= 5 && s < 10) return 2;
     if(s >= 10 && s < 15) return 3;
     if(s >= 15 && s < 20) return 4;
     if(s >= 20 && s < 25) return 5;
     //if(s >= 10 && s < 15) return 6;
     //if(s >= 15 && s < 20) return 7;
     //if(s >= 20 && s < 25) return 8;
     if(s >= 25) return 5;
     return 0;
}

unsigned Environment::pointFit(const unsigned p){
     if(p >= 0 && p < 2) return 0;
     if(p >= 2 && p < 5) return 1;
     if(p >= 5 && p < 10) return 2;
     if(p >= 10 && p < 15) return 3;
     if(p >= 15 && p < 20) return 4;
     if(p >= 20 && p < 25) return 5;
     //if(p >= 10 && p < 15) return 6;
     //if(p >= 20 && p < 25) return 7;
     //if(p >= 30 && p < 35) return 8;
     if(p >= 35) return 6;
     return 0;
}

void Environment::OutputData(FileHandler &fh, int fileId, double t) {
  return;

  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  if(vci.empty()) {
    cerr << "ERROR: Empty population in output." << endl;
    return;
  }

  CreatureInfo *ci = vci[0];
  if(ci == NULL) {
    cerr << "ERROR: CreatureInfo pointer is NULL." << endl;
    return;
  }

  const Creature &c = ci->GetCreature();
  const Body &body = c.GetBody();

  bool success = false;
  ostream &out = fh.OutputStream(fileId, success);
  if(!success) { out << "ERROR: " << t << endl; return; }

  Vector3d com;
  if(!CenterOfMass(com, body)) {
    cerr << "ERROR: Calculating center of mass." << endl;
    return;
  }
  out << t << " " << com[0] << " " << com[1] << " " << com[2] << endl;

  //double minY, maxY;
  //if(!YBound(body, minY, maxY)) { return false; }
  //comHeight -= minY;

  //double xzDistance = ci->DistanceMoved();
  //if(xzDistance > MaxMove()) { MaxMove() = xzDistance; }

  return;
}

bool Environment::LoadCreature(FileHandler &fh, const string &fileName) {
  int fileId = fh.Create(fileName, ios::in);
  if(fileId < 0) {
    cerr << "ERROR: ( " << fileName; 
    cerr << " ) File doesn't exist, or failed to open. " << endl;
    return false;
  }

  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  if(vci.empty() || vci[0] == NULL) {
    cerr << "ERROR: Empty population." << endl;
    return false;
  }

  CreatureInfo *ci = vci[0];
  string str("");
  if(!fh.InputCreature(fileId, *ci, str)) {
    cerr << "ERROR: ( " << fileName; 
    cerr << " ) Failed to load creature." << endl;
    return false;
  }

  fh.Close(fileId);

  ci->Clean((EnvironmentData &)(*this));

  Spider &spider = GetLanguageEngine().GetSpider();
  Parser &p = GetLanguageEngine().GetParser();

  vector<int> errors;
  if(!p.Parse(ci->GetCreature().GetGenome(), errors, str) ||
     !spider.Run(ci->GetCreature().GetGenome(),
		 ci->GetCreature().GetBody(),
		 (EnvironmentData &)(*this))) {
    cerr << "ERROR: ( " << fileName; 
    cerr << " ) Failed to load creature." << endl;
    return false;
  }

#if 0
  string name = fileName;
  name += ".dat";
  d_outputFileId = fh.Create(name, ios::out);
  if(d_outputFileId <= 0) { return false; }
#endif

  return true;
}

bool Environment::LoadCreatureFromGeneration(FileHandler &fh, int fileId) {
  vector<CreatureInfo *> &vci = d_populationStorage.Population();
  if(vci.empty() || vci[0] == NULL) {
    cerr << "ERROR: Empty population." << endl;
    return false;
  }

  CreatureInfo *ci = vci[0];
  string str("");
  if(!fh.InputCreature(fileId, *ci, str)) {
    cerr << "ERROR: Failed to load creature." << endl;
    return false;
  }

  cout << ci->Fitness() << " ";

  ci->Clean((EnvironmentData &)(*this));

  Spider &spider = GetLanguageEngine().GetSpider();
  Parser &p = GetLanguageEngine().GetParser();

  vector<int> errors;
  if(!p.Parse(ci->GetCreature().GetGenome(), errors, str) ||
     !spider.Run(ci->GetCreature().GetGenome(),
		 ci->GetCreature().GetBody(),
		 (EnvironmentData &)(*this))) {
    cerr << "ERROR: Failed to load creature." << endl;
    return false;
  }

#if 0
  string name = fileName;
  name += ".dat";
  d_outputFileId = fh.Create(name, ios::out);
  if(d_outputFileId <= 0) { return false; }
#endif

  return true;
}
