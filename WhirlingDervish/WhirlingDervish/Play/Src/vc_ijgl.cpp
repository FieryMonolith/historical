#include "ipopulation_storage.h"

#include "vc_ijgl.h"

#include "grid.h"
#include "eyepoint.h"

iJGL::iJGL(iPopulationStorage &ps) : d_jgl(ps) {
  //setFocusPolicy(QWidget::StrongFocus);

#if 0
  d_font = new QFont;
  assert(d_font);
  d_font->setStyleStrategy(QFont::PreferBitmap);

  d_selectionMap = new QPixmap;
  assert(d_selectionMap);

  d_jglSelectionFormat = new QGLFormat;
  assert(d_jglSelectionFormat);
  d_jglSelectionFormat->setDoubleBuffer(FALSE);

  d_jglSelectionContext = new QGLContext(*d_jglSelectionFormat, 
					 d_selectionMap);
  assert(d_jglSelectionContext);
#endif

  return;
}

iJGL::~iJGL(void) {
#if 0
  d_currentPrivateId = -1;
  d_currentPrivate = NULL;

  map<int, iJGLPrivate *>::iterator i;
  for(i = d_privates.begin(); i != d_privates.end(); ++i) {
    if(i->second != NULL) {
      delete i->second;
      i->second = NULL;
    }
  }
#endif

  return;
}

//QSizePolicy iJGL::sizePolicy(void) const {
//  return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//}

void iJGL::updateGL(int width, int height) {
  initializeGL();
  resizeGL(width, height);
  paintGL();

  return;
}

void iJGL::NormalView(void) {
  d_jgl.NormalView();

  return;
}

void iJGL::DownView(void) {
  d_jgl.DownView();

  return;
}

void iJGL::initializeGL(void) {
  d_jgl.InitializeGL();

  return;
}

void iJGL::resizeGL(int width, int height) {
  d_jgl.ResizeGL(width, height);

  return;
}

void iJGL::paintGL(void) {
  d_jgl.PaintGL();

  return;
}

#if 0
void iJGL::mousePressEvent(QMouseEvent *e) {
  //if(d_currentPrivate != NULL && !d_data.Type()) {
  if(d_currentPrivate != NULL) {
    int b;
    if(e->button() == Qt::RightButton) {
      b = 0;
    } else {
      b = 1;
    }

    int r = d_data.MousePressEvent(width(), height(), 
					      e->x(), e->y(), b);
    switch(r) {
    case 0:
      emit DeselectCreature();
      break;
    case 1:
      emit SelectCreature();
      break;
    case 2:
      emit SelectCreature();
      emit RightButton();
      break;
    };

    glDrawBuffer(GL_BACK);
    makeCurrent();
  }

  return;
}

void iJGL::mouseReleaseEvent(QMouseEvent *e) {
  //if(d_currentPrivate != NULL && !d_data.Type()) {
  if(d_currentPrivate != NULL) {
    d_data.MouseReleaseEvent();
  }

  return;
}

void iJGL::mouseMoveEvent(QMouseEvent *e) {
  //if(d_currentPrivate != NULL && !d_data.Type()) {
  if(d_currentPrivate != NULL) {
    d_data.MouseMoveEvent(e->x(), e->y());
  }

  return;
}

void iJGL::CreateView(int id, int type, const EnvironmentData &eData, 
		     InteractiveEnvironmentData &ieData) {
  iJGLPrivate *p = new iJGLPrivate(type, eData, ieData);
  if(p == NULL) { return; }
  if(!d_privates.insert(make_pair(id, p)).second) {
    delete p;
  }

  d_currentPrivateId = id;
  d_currentPrivate = p;

  d_data.InitializeGL();
  d_data.ResizeGL(width(), height());

  return;
}

bool iJGL::SwitchView(int id) {
  map<int, iJGLPrivate *>::iterator i = d_privates.find(id);
  if(i == d_privates.end() || i->second == NULL) { return false; }

  d_currentPrivate = i->second;
  d_currentPrivateId = id;
  d_data.ResizeGL(width(), height());

  return true;
}

void iJGL::RemoveView(int id, int newId) {
  map<int, iJGLPrivate *>::iterator i = d_privates.find(id);
  if(i == d_privates.end() || i->second == NULL) { return; }

  delete i->second;
  i->second = NULL;
  d_privates.erase(id);

  i = d_privates.find(newId);
  if(i == d_privates.end() || i->second == NULL) { return; }

  return;
}
#endif


void iJGL::IncreaseRotationSpeed(void) {
  d_jgl.GetEyepoint().IncreaseRotationSpeed();

  return;
}

void iJGL::DecreaseRotationSpeed(void) {
  d_jgl.GetEyepoint().DecreaseRotationSpeed();

  return;
}

void iJGL::IncreaseForwardSpeed(void) {
  d_jgl.GetEyepoint().IncreaseForwardSpeed();

  return;
}

void iJGL::DecreaseForwardSpeed(void) {
  d_jgl.GetEyepoint().DecreaseForwardSpeed();

  return;
}

void iJGL::MoveForward(void) {
  d_jgl.GetEyepoint().MoveForward();

  return;
}

void iJGL::MoveBackward(void) {
  d_jgl.GetEyepoint().MoveBackward();

  return;
}

void iJGL::MoveLeft(void) {
  d_jgl.GetEyepoint().MoveLeft();

  return;
}

void iJGL::MoveRight(void) {
  d_jgl.GetEyepoint().MoveRight();

  return;
}

void iJGL::MoveUp(void) {
  d_jgl.GetEyepoint().MoveUp();

  return;
}

void iJGL::MoveDown(void) {
  d_jgl.GetEyepoint().MoveDown();

  return;
}

void iJGL::YawLeft(void) {
  d_jgl.GetEyepoint().YawLeft();

  return;
}

void iJGL::YawRight(void) {
  d_jgl.GetEyepoint().YawRight();

  return;
}

void iJGL::PitchUp(void) {
  d_jgl.GetEyepoint().PitchUp();

  return;
}

void iJGL::PitchDown(void) {
  d_jgl.GetEyepoint().PitchDown();

  return;
}

void iJGL::ToggleGrid(void) {
  d_jgl.GetGrid().Show();

  return;
}

void iJGL::ToggleView(void) {
  d_jgl.GetEyepoint().ToggleView();

  return;
}

#if 0
int iJGLPrivate::MousePressEvent(int width, int height, int x, int y, int s) {
  d_pressed = false;

  d_ieData.d_pause.lock();

  bool alreadySelected = d_ieData.d_selected;
  unsigned int currentSelected = d_ieData.d_selectedId;

  // Invert y coordinate
  pair<int, int64> p = SelectCreature(width, height, x, height - y);

  if(alreadySelected && !d_ieData.d_selected) {
    d_ieData.d_pause.unlock();
    return 0;
  } else if(alreadySelected && d_ieData.d_selected &&
	    currentSelected != d_ieData.d_selectedId) {
    d_ieData.d_pause.unlock();
    return 0;
  }

  int whatToEmit = -1;
  if(p.first == 1 || p.first == 2) {
    whatToEmit = 1;
    d_frozen1 = d_frozen2 = false;
    IsFrozen(p.first, p.second, d_frozen1, d_frozen2);
    if(s == Qt::RightButton) {
      if(d_frozen1) {
	d_frozen1 = false;
      } else {
	d_frozen1 = true;
      }
      if(d_frozen2) {
	d_frozen2 = false;
      } else {
	d_frozen2 = true;
      }
      whatToEmit = 2;
    }

    d_linePercent = 0;
    if(p.first == 2) {
      d_linePercent = FindLinePercent(p.second, x, y);
    }
    if(d_linePercent >= 0) {
      d_pressed = true;
      d_typePressed = p.first;
      d_typeId = p.second;
      if(s == 0) {
	// Right button never releases, so do it here
	Freezer(d_typePressed, d_typeId, d_frozen1, d_frozen2);
      } else {
	Freezer(d_typePressed, d_typeId, true, true);
      }
    }
  }

  d_ieData.d_pause.unlock();

  return whatToEmit;
}

void iJGLPrivate::MouseReleaseEvent(void) {
  if(d_pressed) {
    Freezer(d_typePressed, d_typeId, d_frozen1, d_frozen2);
    d_linePercent = 0;
    d_pressed = false;
  }

  return;
}

void iJGLPrivate::MouseMoveEvent(int x, int y) {
  if(!d_pressed) { return; }

  d_ieData.d_pause.lock();

  if(x > d_width) { x = d_width; }
  else if(x < 0) { x = 0; }

  if(y > d_height) { y = d_height; }
  else if(y < 0) { y = 0; }

  if(d_typePressed == 1) {
    MovePoint(d_typeId, x, y);
  } else if(d_typePressed == 2) {
    MoveLine(d_typeId, d_linePercent, x, y);
  }

  d_ieData.d_pause.unlock();

  return;
}

pair<int, int64> iJGLPrivate::SelectCreature(int width, int height, 
					    int x, int y) {
  bool isNew = false;
  bool needsResize = false;
  unsigned int selectedId = 0;
  pair<int, int64> p;

  if(d_eData.d_creatures.empty()) {
    d_ieData.d_selected = false;
    d_ieData.d_selectedId = 0;
    p.first = 0;
    return p;
  }
  if(width == 0 && height == 0) {
    d_ieData.d_selected = false;
    d_ieData.d_selectedId = 0;
    p.first = -1;
    return p;
  }

  if(width != d_width || height != d_height) {
    d_selectionMap->resize(width, height);
    d_width = width;
    d_height = height;
    needsResize = true;
  }

  if(!d_jglSelectionContext->isValid()) {
    isNew = true;
    needsResize = true;
    if(!d_jglSelectionContext->create()) {
      d_ieData.d_selected = false;
      d_ieData.d_selectedId = 0;
      p.first = -2;
      return p;
    }
  }

  d_jglSelectionContext->makeCurrent();

  if(isNew) {
    InitializeGL();
  }
  if(needsResize) {
    glPointSize(4.0);
    glLineWidth(2.0);
    d_selectionMap->fill(QColor(Qt::black));
    ResizeGL(width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  glDrawBuffer(GL_FRONT);
  DrawEyepoint();

  // Render creatures here
  vector<unsigned int> creatureIds;
  // Black mean nothing selected
  float red=0.0, green=0.0, blue=0.1;
  map<unsigned int, Creature *>::const_iterator icreature;
  for(icreature = d_eData.d_creatures.begin(); 
      icreature != d_eData.d_creatures.end();
      ++icreature){
    if(icreature->second == NULL) { continue; }

    creatureIds.push_back(icreature->first);

    glColor3f(red, green, blue);

    glBegin(GL_POINTS);
    RenderPoints(CreatureCharacterization::Points(*(icreature->second)));
    glEnd();

    glBegin(GL_LINES);
    RenderLines(CreatureCharacterization::Lines(*(icreature->second)));
    glEnd();

    blue += 0.1;
    if(blue >= 1.0) {
      blue = 0.0;
      green += 0.1;
      if(green >= 1.0) {
	green = 0.0;
	red += 0.1;
	if(red >= 1.0) {
	  red = 0.0;
	  blue = 0.1;
	}
      }
    }
  }

  glReadBuffer(GL_FRONT);
  glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, (GLvoid *)&red);
  glReadPixels(x, y, 1, 1, GL_GREEN, GL_FLOAT, (GLvoid *)&green);
  glReadPixels(x, y, 1, 1, GL_BLUE, GL_FLOAT, (GLvoid *)&blue);

  int blueValue = (int)(blue*10.0);
  if(blue - ((float)blueValue/10.0) >= 0.05) {
    blueValue++;
  }
  int greenValue = (int)(green*10.0);
  if(green - ((float)greenValue/10.0) >= 0.05) {
    greenValue++;
  }
  int redValue = (int)(red*10.0);
  if(red - ((float)redValue/10.0) >= 0.05) {
    redValue++;
  }

  unsigned int which = blueValue + (greenValue * 10) + (redValue * 100);
  if(which == 0 || which-1 >= creatureIds.size()) {
    d_ieData.d_selected = false;
    d_ieData.d_selectedId = 0;
    p.first = -3;
    return p;
  }
  which--;
  selectedId = creatureIds.at(which);
  icreature = d_eData.d_creatures.find(selectedId);
  if(icreature == d_eData.d_creatures.end() || icreature->second == NULL) {
    d_ieData.d_selected = false;
    d_ieData.d_selectedId = 0;
    p.first = -4;
    return p;
  }

  d_ieData.d_selected = true;
  d_ieData.d_selectedId = selectedId;

  Creature *creature = icreature->second;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  DrawEyepoint();

  // Find Point
  vector<int64> ids;
  const map<int64, PhysicalPoint *> &points = 
    CreatureCharacterization::Points(*creature);
  map<int64, PhysicalPoint *>::const_iterator ipoint;
  red=0.0; green=0.0; blue=0.1;
  for(ipoint = points.begin(); ipoint != points.end(); ++ipoint) {
    if(ipoint->second == NULL) { continue; }

    ids.push_back(ipoint->first);

    glColor3f(red, green, blue);

    glBegin(GL_POINTS);
    glVertex3dv(ipoint->second->Location().Array());
    glEnd();

    blue += 0.1;
    if(blue >= 1.0) {
      blue = 0.0;
      green += 0.1;
      if(green >= 1.0) {
	green = 0.0;
	red += 0.1;
	if(red >= 1.0) {
	  red = 0.0;
	  blue = 0.1;
	}
      }
    }
  }

  glReadBuffer(GL_FRONT);
  glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, (GLvoid *)&red);
  glReadPixels(x, y, 1, 1, GL_GREEN, GL_FLOAT, (GLvoid *)&green);
  glReadPixels(x, y, 1, 1, GL_BLUE, GL_FLOAT, (GLvoid *)&blue);

  blueValue = (int)(blue*10.0);
  if(blue - ((float)blueValue/10.0) >= 0.05) {
    blueValue++;
  }
  greenValue = (int)(green*10.0);
  if(green - ((float)greenValue/10.0) >= 0.05) {
    greenValue++;
  }
  redValue = (int)(red*10.0);
  if(red - ((float)redValue/10.0) >= 0.05) {
    redValue++;
  }

  unsigned int which2 = blueValue + (greenValue * 10) + (redValue * 100);
  if(which2 != 0 && which2-1 < ids.size()) {
    which2--;

    p.first = 1;
    p.second = ids.at(which2);
    return p;
  }

  ids.clear();
  const map<int64, MasslessSpringDampner *> &msds =
    CreatureCharacterization::Lines(*creature);
  map<int64, MasslessSpringDampner *>::const_iterator imsd;
  red=0.0; green=0.0; blue=0.1;
  for(imsd = msds.begin(); imsd != msds.end(); ++imsd) {
    if(imsd->second == NULL) { continue; }

    Vector3d position1, position2;
    if(!imsd->second->Location(position1, position2)) { continue; }

    ids.push_back(imsd->first);

    glColor3d(red, green, blue);
    glBegin(GL_LINES);
    glVertex3dv(position1.Array());
    glVertex3dv(position2.Array());
    glEnd();

    blue += 0.1;
    if(blue >= 1.0) {
      blue = 0.0;
      green += 0.1;
      if(green >= 1.0) {
	green = 0.0;
	red += 0.1;
	if(red >= 1.0) {
	  red = 0.0;
	  blue = 0.1;
	}
      }
    }
  }

  glReadBuffer(GL_FRONT);
  glReadPixels(x, y, 1, 1, GL_RED, GL_FLOAT, (GLvoid *)&red);
  glReadPixels(x, y, 1, 1, GL_GREEN, GL_FLOAT, (GLvoid *)&green);
  glReadPixels(x, y, 1, 1, GL_BLUE, GL_FLOAT, (GLvoid *)&blue);

  blueValue = (int)(blue*10.0);
  if(blue - ((float)blueValue/10.0) >= 0.05) {
    blueValue++;
  }
  greenValue = (int)(green*10.0);
  if(green - ((float)greenValue/10.0) >= 0.05) {
    greenValue++;
  }
  redValue = (int)(red*10.0);
  if(red - ((float)redValue/10.0) >= 0.05) {
    redValue++;
  }

  which2 = blueValue + (greenValue * 10) + (redValue * 100);
  if(which2 != 0 && which2-1 < ids.size()) {
    which2--;
    p.first = 2;
    p.second = ids.at(which2);
    return p;
  }

  p.first = -5;
  return p;
}

void iJGLPrivate::IsFrozen(int type, int64 id, bool &true1, bool &true2) {
  if(type == 1) {
    PhysicalPoint *point = d_eData.GetPoint(id);
    true1 =  point->IsSuspended();
    true2 = false;
  } else if(type == 2) {
    MasslessSpringDampner *msd = d_eData.GetMasslessSpringDampner(id);
    int gid1, gid2;
    msd->BothPoints(gid1, gid2);
    int64 id1 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid1);
    int64 id2 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid2);
    if(id1 < 0 || id2 < 0) {
      return;
    }
    PhysicalPoint *point = d_eData.GetPoint(id1);
    true1 = point->IsSuspended();
    point = d_eData.GetPoint(id2);
    true2 = point->IsSuspended();
  }

  return;
}

void iJGLPrivate::Freezer(int type, int64 id, bool true1, bool true2) {
  if(type == 1) {
    PhysicalPoint *point = d_eData.GetPoint(id);
    point->Suspend(true1);
  } else if(type == 2) {
    MasslessSpringDampner *msd = d_eData.GetMasslessSpringDampner(id);
    int gid1, gid2;
    msd->BothPoints(gid1, gid2);
    int64 id1 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid1);
    int64 id2 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid2);
    if(id1 < 0 || id2 < 0) {
      return;
    }
    PhysicalPoint *point = d_eData.GetPoint(id1);
    point->Suspend(true1);
    point = d_eData.GetPoint(id2);
    point->Suspend(true2);
  }

  return;
}

void iJGLPrivate::MovePoint(int64 id, int x, int y) {
  // Get Selected point
  map<int64, PhysicalPoint *>::const_iterator ipoint;
  ipoint = d_eData.d_points.find(id);
  if(ipoint == d_eData.d_points.end() || ipoint->second == NULL) { return; }

  // Get eyepoint information
  Vector3d eyepointLocation = d_eyepoint->Location();
  Vector3d out = d_eyepoint->OutVector();
  Normalize(out);

  Vector3d windowLocation;
  ConvertWindowCoordinates(windowLocation, x, y, eyepointLocation, out);

  Vector3d intersectionPoint;
  if(!IntersectionVectorPlane(eyepointLocation, windowLocation, out, 
			      ipoint->second->Location(), intersectionPoint)) {
    return;
  }

  ipoint->second->SetLocation(intersectionPoint);

  return;
}

void iJGLPrivate::MoveLine(int64 id, double linePercent, int x, int y) {
  // Get endpoint locations (world)
  MasslessSpringDampner *msd = d_eData.GetMasslessSpringDampner(id);
  if(msd == NULL) {
    return;
  }

  int gid1, gid2;
  msd->BothPoints(gid1, gid2);
  int64 id1 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid1);
  int64 id2 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid2);
  if(id1 < 0 || id2 < 0) {
    return;
  }
  PhysicalPoint *point1 = d_eData.GetPoint(id1);
  if(point1 == NULL) { return; }
  const Vector3d &point1Location = point1->Location();
  PhysicalPoint *point2 = d_eData.GetPoint(id2);
  if(point2 == NULL) { return; }
  const Vector3d &point2Location = point2->Location();

  // Get eyepoint information
  Vector3d eyepointLocation = d_eyepoint->Location();
  Vector3d out = d_eyepoint->OutVector();
  Normalize(out);

  Vector3d windowLocation;
  ConvertWindowCoordinates(windowLocation, x, y, eyepointLocation, out);

  Vector3d line(point2Location - point1Location);
  double magnitude = Normalize(line);

  Vector3d pointOnLine(point1Location + (magnitude * linePercent * line));
  Vector3d intersectionPoint;
  if(!IntersectionVectorPlane(eyepointLocation, windowLocation, out, 
			      pointOnLine, intersectionPoint)) {
    return;
  }

  Vector3d delta(intersectionPoint - pointOnLine);

  point1->SetLocation(point1Location + delta);
  point2->SetLocation(point2Location + delta);

  return;
}

double iJGLPrivate::FindLinePercent(int64 lineId, int x, int y) {
  // Get endpoint locations (world)
  MasslessSpringDampner *msd = d_eData.GetMasslessSpringDampner(lineId);
  if(msd == NULL) {
    return -1.0;
  }

  int gid1, gid2;
  msd->BothPoints(gid1, gid2);
  int64 id1 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid1);
  int64 id2 = CreatureCharacterization::TranslatePointId(
			  *(d_eData.GetCreature(d_ieData.d_selectedId)), gid2);
  if(id1 < 0 || id2 < 0) {
    return -2.0;
  }
  PhysicalPoint *point = d_eData.GetPoint(id1);
  if(point == NULL) { return -3.0; }
  const Vector3d &point1Location = point->Location();
  point = d_eData.GetPoint(id2);
  if(point == NULL) { return -4.0; }
  const Vector3d &point2Location = point->Location();

  // Get eyepoint information
  Vector3d eyepointLocation = d_eyepoint->Location();
  Vector3d out = d_eyepoint->OutVector();
  Normalize(out);

  Vector3d windowLocation;
  ConvertWindowCoordinates(windowLocation, x, y, eyepointLocation, out);

  // Create a vector from point to eyepoint
  Vector3d point1Ray = (eyepointLocation - point1Location);
  Normalize(point1Ray);
  Vector3d point2Ray = (eyepointLocation - point2Location);
  Normalize(point2Ray);

  // Project the endpoints onto the window
  Vector3d intersectionPoint1, intersectionPoint2;
  if(!IntersectionVectorPlane(point1Location, eyepointLocation, out, 
			      windowLocation, intersectionPoint1)) {
    return -5.0;
  }
  if(!IntersectionVectorPlane(point2Location, eyepointLocation, out, 
			      windowLocation, intersectionPoint2)) {
    return -6.0;
  }

  Vector3d projectedVector(intersectionPoint2 - intersectionPoint1);
  double projectedVectorMagnitude = Normalize(projectedVector);
  Vector3d pointW1(windowLocation - intersectionPoint1);
  double pointW1Magnitude = Normalize(pointW1);

  double cosA = Inner(pointW1, projectedVector);
  double u = pointW1Magnitude * cosA;
  if(cosA < 0 || u > projectedVectorMagnitude) {
    // Beyond line
    return -7.0;
  }

  double lineFraction = 0;
  if(cosA == 0) {
    lineFraction = pointW1Magnitude / projectedVectorMagnitude;
  } else {
    lineFraction = u / projectedVectorMagnitude;
  }

  return lineFraction;
}
#endif
