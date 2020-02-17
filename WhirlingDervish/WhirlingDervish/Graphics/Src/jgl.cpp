#include <cassert>
#include <cstdlib>

#include "jgl_private.h"
#include "eyepoint.h"
#include "grid.h"
#include "ipopulation_storage.h"

#include "jgl.h"

JGL::JGL(iPopulationStorage &ps) : d_data(NULL) {
  d_data = new JGLPrivate(ps);
  assert(d_data);

  return;
}

JGL::~JGL(void) {
  if(d_data != NULL) {
    delete d_data;
    d_data = NULL;
  }

  return;
}

Eyepoint &JGL::GetEyepoint(void) {
  return d_data->GetEyepoint();
}

Grid &JGL::GetGrid(void) {
  return d_data->GetGrid();
}

void JGL::InitializeGL(void) {
  d_data->InitializeGL();

  return;
}

void JGL::ResizeGL(int width, int height) {
  d_data->ResizeGL(width, height);

  return;
}

void JGL::PaintGL(void) {
  d_data->PaintGL();

  return;
}

void JGL::NormalView(void) {
  d_data->NormalView();

  return;
}

void JGL::DownView(void) {
  d_data->DownView();

  return;
}
