#include "genome.h"
#include "body.h"
#include "physical_point.h"
#include "massless_spring_dampner.h"
#include "instruction_set_simple.h"
#include "environment_data.h"

#include "spider_simple.h"

#include "jvector.h"
#include "binding.h"

using namespace std;

SpiderSimple::SpiderSimple(void) {
  return;
}

SpiderSimple::~SpiderSimple(void) {
  return;
}

State *SpiderSimple::NewState(void) const {
  return new SimpleState;
}

bool SpiderSimple::Initialize(State &state) const {
  SimpleState &s = (SimpleState &)state;
  s.d_currentPoint = 0;
  s.d_currentId = 0;

  return true;
}

int Spider::Execute(State &state, const Instruction &instruction, Body &body,
		    EnvironmentData &ed) const {
  SimpleState &s = (SimpleState &)state;

  int fail = 0;
  unsigned int id = i->InstructionId();
  if(id == SimpleInstruction::NOP) {
    fail = nop();
  } else if(id == SimpleInstruction::SPLIT) {
    fail = split(s, instruction, body, ed);
  } else if(id == SimpleInstruction::SPLIT_DUP) {
    fail = split_dup(s, instruction, body, ed);
  } else if(id == SimpleInstruction::ADD_LINE) {
    fail = add_line(s, instruction, body, ed);
  } else if(id == SimpleInstruction::REMOVE_LINE) {
    fail = remove_line(s, instruction, body, ed);
  } else if(id == SimpleInstruction::REMOVE_POINT) {
    fail = remove_point(s, instruction, body, ed);
  } else if(id == SimpleInstruction::MERGE) {
    fail = merge(s, instruction, body, ed);
  } else if(id == SimpleInstruction::MOVE_TO) {
    fail = move_to(s, instruction, body);
  } else if(id == SimpleInstruction::CHANGE_MASS) {
    fail = change_mass(s, instruction);
  } else if(id == SimpleInstruction::CHANGE_KS
    fail = change_ks(s, instruction);
  } else if(id == SimpleInstruction::CHANGE_KD) {
    fail = change_kd(s, instruction);
  } else if(id == SimpleInstruction::CHANGE_FLUCTUATION_DELTA) {
    fail = change_fluctuation_delta(s, instruction);
  } else if(id == SimpleInstruction::CHANGE_FLUCTUATION_START) {
    fail = change_fluctuation_start(s, instruction);
  } else {
    return -2;
  }

  return 0;
}

int SpiderCharles::nop(void) const {
  return 0;
}

int SpiderSimple::split(SimpleState &s,
			const Instruction &instruction,
			Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  int gid1 = s.d_currentId;
  PhysicalPoint *current = NULL;

  Vector3d newLocation;
  double mass = PhysicalPointConstraints::DEFAULT_MASS;

  if(s.d_currentPoint != 0) {
    current = body.GetPoint(id1);
    if(currrent == NULL) { return -3; }
    newLocation = current->Position();
    mass = current->Mass();
  }

  const Split &s = (const Split &)instruction;
  Vector3d t(s.d_x, s.d_y, s.d_z);
  newLocation += t;
  mass += s.d_mass;

  PhysicalPoint *p = new_point(s, newLocation, mass, instruction, body, ed);

  int64 msdId;
  if(new_spring(s, newLocation, s.d_mass, body, msdId,
		*current, *p, ed) == NULL) {
    return -3;
  }
  body.AddConnection(msdId, id1, s.d_currentPoint);

  return 0;
}

int SpiderSimple::split_dup(SimpleState &s,
			     const Instruction &instruction,
			     Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  PhysicalPoint *current = body.GetPoint(id1);
  PhysicalPoint *p = new_point(s, instruction, body, ed);

  if(s.d_currentPoint == 0 && p != NULL) { return 0; }
  if(p == NULL || current == NULL) { return -3; }

  vector<pair<int64, int64> >temp;
  const map<int64, pair<int64, int64> > &lines = body.Connections();
  map<int64, pair<int64, int64> >::const_iterator line;
  for(line = lines.begin(); line != lines.end(); ++line) {
    int64 t1 = line->second.first;
    int64 t2 = line->second.second;
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(i->first, t));
  }

  vector<pair<int64, int64> >::iterator ti;
  for(ti=temp.begin(); ti!=temp.end(); ++ti) {
    int64 springId = ti->first;
    int64 t = ti->second;

    PhysicalPoint *p2 = body.GetPoint(t);
    if(p2 == NULL) { return -3; }

    int64 tId;
    if(copy_spring(s, body, tId, springId, *p, *p2, ed) == NULL) { return -3; }
    body.AddConnection(tId, s.d_currentPoint, t);
  }

  int64 msdId;
  if(new_spring(s, body, msdId, *current, *p, ed) == NULL) { return -3; }
  body.AddConnection(msdId, id1, s.d_currentPoint);

  return 0;
}

int SpiderSimple::add_line(SimpleState &s,
			    const Instruction &instruction,
			    Body &body, EnvironmentData &ed) const {
  if(s.d_currentPoint == 0) { return 0; }
  int64 otherId = FindPoint(s, GetParameter(s, instruction), body);
  if(otherId == 0) { return 0; }

  PhysicalPoint *p1 = body.GetPoint(s.d_currentPoint);
  PhysicalPoint *p2 = body.GetPoint(otherId);
  if(p1 == NULL || p2 == NULL) { return -3; }

  int msdId;
  if(new_spring(s, body, msdId, *p1, *p2, ed) == NULL) { return -3; }
  body.AddConnection(msdId, otherId, s.d_currentPoint);

  s.d_currentPoint = otherId;

  return 0;
}

int SpiderSimple::remove_line(SimpleState &s,
			       const Instruction &instruction,
			       Body &body, EnvironmentData &ed) const {
  if(s.d_currentPoint == 0) { return 0; }
  int64 otherId = FindPoint(s, GetParameter(s, instruction), body);
  if(otherId == 0) { return 0; }

  int64 springId = find_spring(body, s.d_currentPoint, otherId);
  if(springId == 0) { return 0; }

  if(!body.RemoveLine(ed, springId)) { return -3; }

  s.d_currentPoint = otherId;

  return 0;
}

int SpiderSimple::remove_point(SimpleState &s,
				const Instruction &instruction,
				Body &body, EnvironmentData &ed) const {
  if(s.d_currentPoint == 0) { return 0; }
  if(!body.RemovePoint(ed, s.d_currentPoint)) { return -3; }
  return move_to(s, instruction, body);
}

int SpiderSimple::merge(SimpleState &s,
			 const Instruction &instruction,
			 Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  int64 id2 = FindPoint(s, GetParameter(s, instruction), body);
  if(id1 == 0 || id2 == 0) { return 0; }

  PhysicalPoint *current1 = body.GetPoint(id1);
  PhysicalPoint *current2 = body.GetPoint(id2);
  if(current1 == NULL || current2 == NULL) { return -3; }

  PhysicalPoint *p = merge_point(s, body, ed, *current1, *current2);
  if(p == NULL) { return -3; }

  vector<pair<int64, int64> > temp;
  const map<int64, pair<int64, int64> > &lines = body.Connections();
  map<int64, pair<int64, int64> >::const_iterator line;
  for(line = lines.begin(); line != lines.end(); ++line) {
    int64 t1 = line->second.first;
    int64 t2 = line->second.second;
    int64 t;
    if(t1 == id1 && t2 != id2) {
      t = t2;
    } else if(t2 == id1 && t1 != id2) {
      t = t1;
    } else if(t1 == id2) {
      t = t2;
    } else if(t2 == id2) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(i->first, t));
  }

  vector<pair<int64, int64> >::iterator ti;
  for(ti=temp.begin(); ti!=temp.end(); ++ti) {
    int64 springId = ti->first;
    int64 t = ti->second;

    PhysicalPoint *p2 = body.GetPoint(t);
    if(p2 == NULL) { return -3; }

    int64 tId;
    if(copy_spring(s, body, tId, springId, *p, *p2, ed) == NULL) { return -3; }
    body.AddConnection(tId, s.d_currentPoint, t);
  }

  body.RemovePoint(id1);
  body.RemovePoint(id2);

  return 0;
}

int SpiderSimple::move_to(SimpleState &s,
			  const Instruction &instruction,
			  Body &body) const {
  s.d_currentPoint = FindPoint(s, GetParameter(s, instruction), body);

  return 0;
}

void SpiderSimple::change_mass(double delta) {
  if(d_currentPoint < 0) { return; }
  if(!d_body->HasPoint(d_currentPoint)) {
    assert(0);
  }

  PhysicalPoint &point = d_body->GetPoint(d_currentPoint);

  point.ChangeMass(delta);

  return;
}

void SpiderSimple::change_innate_length(int pointId, double delta) {
  if(pointId < 0) { assert(0); }
  if(d_currentPoint < 0 || !d_body->HasPoint(pointId)) { return; }
  if(!d_body->HasPoint(d_currentPoint)) { assert(0); }

  if(!d_body->HasLine(d_currentPoint, pointId)) { return; }
  MasslessSpringDampner &line = d_body->GetLine(d_currentPoint, pointId);

  line.ChangeInnateLength(delta);

  return;
}

void SpiderSimple::change_ks(int pointId, double delta) {
  if(pointId < 0) { assert(0); }
  if(d_currentPoint < 0 || !d_body->HasPoint(pointId)) { return; }
  if(!d_body->HasPoint(d_currentPoint)) { assert(0); }

  if(!d_body->HasLine(d_currentPoint, pointId)) { return; }
  MasslessSpringDampner &line = d_body->GetLine(d_currentPoint, pointId);

  line.ChangeKS(delta);

  return;
}

void SpiderSimple::change_kd(int pointId, double delta) {
  if(pointId < 0) { assert(0); }
  if(d_currentPoint < 0 || !d_body->HasPoint(pointId)) { return; }
  if(!d_body->HasPoint(d_currentPoint)) { assert(0); }

  if(!d_body->HasLine(d_currentPoint, pointId)) { return; }
  MasslessSpringDampner &line = d_body->GetLine(d_currentPoint, pointId);

  line.ChangeKD(delta);

  return;
}

void SpiderSimple::move_to(int pointId) {
  if(pointId < 0) { assert(0); }

  if(d_body->HasPoint(pointId)) {
    d_currentPoint = pointId;
  }

  return;
}

void SpiderSimple::nop(void) {
  return;
}

PhysicalPoint *SpiderSimple::new_point(SimpleState &s,
				       const Vector3d &newLocation,
				       double mass,
				       Body &body,
				       EnvironmentData &ed) const {
  return body.AddPoint(ed, s.d_currentPoint, newLocation, mass,
		       PhysicalPointConstraints::DEFAULT_STATIC_MU,
		       PhysicalPointConstraints::DEFAULT_KINETIC_MU,
		       PhysicalPointConstraints::DEFAULT_DRAG);
}

PhysicalPoint *SpiderSimple::merge_point(SimpleState &s, Body &body,
					 EnvironmentData &ed,
					 const PhysicalPoint &p1,
					 const PhysicalPoint &p2) const {
  Vector3d newLocation((p1.Position() + p2.Position()) / 2.0);
  double mass = p1.Mass() + p2.Mass();

  return body.AddPoint(ed, s.d_currentPoint, newLocation, mass,
		       PhysicalPointConstraints::DEFAULT_STATIC_MU,
		       PhysicalPointConstraints::DEFAULT_KINETIC_MU,
		       PhysicalPointConstraints::DEFAULT_DRAG);
}

MasslessSpringDampner *SpiderSimple::new_spring(SimpleState &s, Body &body,
						int64 &id,
						const PhysicalPoint &p1,
						const PhysicalPoint &p2,
						EnvironmentData &ed) const {
  return body.AddLine(ed, id, p1, p2,
		      Distance(p1.Position(), p2.Position()),
		      s.d_springConstants[0], s.d_springConstants[1],
		      s.d_springFluctuations[0], s.d_springFluctuations[1]);
}

MasslessSpringDampner *SpiderSimple::copy_spring(SimpleState &s, Body &body,
						  int64 &id, int64 msdId,
						  const PhysicalPoint &p1,
						  const PhysicalPoint &p2,
						  EnvironmentData &ed) const {
  MasslessSpringDampner *msd = body.GetLine(msdId);
  if(msd == NULL) { id = 0; return NULL; }

  return body.AddLine(ed, id, p1, p2,
		      Distance(p1.Position(), p2.Position()),
		      msd->ks(), msd->kd(), msd->FluctuationDelta(),
		      msd->FluctuationStart());
}

int64 SpiderSimple::find_spring(Body &body, int64 id1, int64 id2) const {
  const map<int64, pair<int64, int64> > &msds = body.Connections();
  map<int64, pair<int64, int64> >::const_iterator i;
  for(i=msds.begin(); i!=msds.end(); ++i) {
    int64 p1 = i->second.first;
    int64 p2 = i->second.second;

    if((p1 == id1 && p2 == id2) ||
       (p1 == id2 && p2 == id1)) {
      return i->first;
    }
  }

  return 0;
}

SpiderSimple::SimpleState::SimpleState(const vector<Instruction *> &i)
  : State(i) {
  return;
}

SpiderSimple::SimpleState::~SimpleState(void) {
  d_conversion.clear();
  return;
}
