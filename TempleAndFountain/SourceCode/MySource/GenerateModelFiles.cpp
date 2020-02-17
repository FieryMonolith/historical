/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Model.h"
#include "ModelGenerator.h"

using namespace std;

/*

 * These are global functions that are localized to this file.  They provide
 *   a mechanism to generate the model files in case they are lost or missing.

 */

/*** BEGIN Forward declarations of local functions **************************/
void GenerateWorld(void);
void GenerateColumn(void);
void GenerateCappedColumn(void);
void GenerateCap(void);
void GenerateFountain(void);
void GenerateChair(void);
void GenerateTempleFloor(void);
void GenerateGround(void);
void GenerateSky(void);
void GenerateBackground(void);
void GenerateSun(void);
void GenerateMoon(void);
void GenerateWater(void);
/*** END Forward declarations of local functions ****************************/

/*** BEGIN Model generation functions ***************************************/
void CreateModels(void) {
  GenerateWorld();
  GenerateColumn();
  GenerateCappedColumn();
  GenerateFountain();
  //GenerateChair();
  GenerateTempleFloor();
  GenerateGround();
  GenerateSky();
  GenerateBackground();
  GenerateSun();
  GenerateMoon();
  GenerateWater();

  return;
}

void GenerateWorld(void) {
  ofstream out("Models/world.jml");
  if(!out) { return; }

  out << "TYPE COMPOSITE\n";
  out << "NAME TheWorld\n";
  out << "MODEL_FILE Models/fountain.jml 0.0 48.0 0.0\n";
  out << "MODEL_FILE Models/column_with_cap.jml Textures/statuary_white_512_512.bmp 174.0 0.0 -390.0\n";
  out << "MODEL_FILE Models/column_with_cap.jml Textures/statuary_white_512_512.bmp -174.0 0.0 -390.0\n";
  out << "MODEL_FILE Models/column.jml Textures/statuary_white_512_512.bmp -174.0 48.0 246.0\n";
  out << "MODEL_FILE Models/column.jml Textures/statuary_white_512_512.bmp 0.0 48.0 246.0\n";
  out << "MODEL_FILE Models/column.jml Textures/statuary_white_512_512.bmp 174.0 48.0 246.0\n";
  out << "MODEL_FILE Models/temple_floor.jml Textures/grey_marble_512_512.bmp\n";
  out << "MODEL_FILE Models/ground.jml Textures/grass_512_512.bmp 0.0 0.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "#MODEL_FILE Models/night_sky.jml Textures/winter_milkyway_512_512.bmp 0.0 -1400.0 0.0 -1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/day_sky.jml Textures/daytime_sky_512_512.bmp 0.0 500.0 0.0 -1.0 0.0 0.0 0.0 1.0 0.0\n";
  //out << "MODEL_FILE Models/background.jml Textures/mountain_512_512.bmp 0.0 0.0 0.0 0.0 -1.0 0.0 0.0 0.0 1.0\n";

  out.close();

  return;
}

void GenerateColumn(void) {
  ofstream out("Models/column.jml");
  if(!out) { return; }

  out << "TYPE COMPOSITE\n";
  out << "NAME Column\n";

  out << "\n#Base\n";
  out << "MODEL_FILE Models/column_base_top.jml Textures/grey_marble_512_512.bmp 0.0 12.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp 0.0 6.0 -36.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp -36.0 6.0 0.0 -1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp 0.0 6.0 36.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp 36.0 6.0 0.0 1.0 0.0 0.0 0.0 1.0 0.0\n";

  out << "\n#Disks\n";
  out << "MODEL_FILE Models/column_largering_outside.jml 0.0 15.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_largering_top.jml 0.0 18.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_cylinder_between_wheels.jml 0.0 21.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_flute_ring.jml 0.0 21.0 0.0 0.0 -1.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/column_smallring_outside.jml 0.0 22.5 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";

  out << "\n#Fluted columns\n";
  out << "MODEL_FILE Models/column_flute_ring.jml 0.0 24.0 0.0 0.0 -1.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/column_fluted3.jml 0.0 60.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_fluted1.jml 0.0 96.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_fluted3.jml 0.0 132.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_fluted1.jml 0.0 168.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_fluted2.jml 0.0 204.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_flute_ring.jml 0.0 204.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";

  out << "\n#Disks Again\n";
  out << "MODEL_FILE Models/column_smallring_outside.jml 0.0 205.5 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_flute_ring.jml 0.0 207.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_cylinder_between_wheels.jml 0.0 210.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/column_largering_top.jml 0.0 210.0 0.0 0.0 -1.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/column_largering_outside.jml 0.0 213.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";

  out << "\n#Base Again\n";
  out << "MODEL_FILE Models/column_base_top.jml Textures/grey_marble_512_512.bmp 0.0 216.0 0.0 0.0 -1.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp 0.0 222.0 -36.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp -36.0 222.0 0.0 -1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp 0.0 222.0 36.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_base_side.jml Textures/grey_marble_512_512.bmp 36.0 222.0 0.0 1.0 0.0 0.0 0.0 1.0 0.0\n";

  out << "MODEL_FILE Models/column_cap.jml Textures/grey_marble_512_512.bmp 0.0 228.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";

  out.close();

  Model *m = 0;
  m = ModelGenerator::GenPlane(72.0, 12.0, 48.0, 48.0);
  if(m == 0) { return; }
	m->Output("Models/column_base_side.jml");
  delete m;

  m = ModelGenerator::GenPlaneWithHole(72.0, 72.0, 27.6, 16, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/column_base_top.jml");
  delete m;

  m = ModelGenerator::GenCurvedCylinder(3.0, 27.6, 8, 32, 0.5, 1.0, -1.0 * M_PI / 2.0, 0.0, false, 18.0, 18.0);
  if(m == 0) { return; }
  m->Output("Models/column_largering_outside.jml");
  delete m;

  m = ModelGenerator::GenRecord(24.0, 27.6, 32, 1.0, 0.0635, false, 18.0, 18.0);
  if(m == 0) { return; }
  m->Output("Models/column_largering_top.jml");
  delete m;

  m = ModelGenerator::GenCylinder(32, 24.0, 3.0, 1.0, 0.0635, false, true, 24.0, 24.0);
  if(m == 0) { return; }
  m->Output("Models/column_cylinder_between_wheels.jml");
  delete m;

  m = ModelGenerator::GenCurvedCylinder(1.5, 25.5, 8, 32, 0.5, 1.0, (-1.0 * M_PI / 2.0), 0.0, false, 18.0, 18.0);
  if(m == 0) { return; }
  m->Output("Models/column_smallring_outside.jml");
  delete m;

  m = ModelGenerator::GenRecord(25.5, 12.0, 32, 1.0, -0.0535, false, 18.0, 18.0);
  if(m == 0) { return; }
  m->Output("Models/column_flute_ring.jml");
  delete m;

  m = ModelGenerator::GenFlutedColumn(24.0, 36.0, 16, 3.5, 8, 36.0, 36.0, 7.5);
  if(m == 0) { return; }
  m->Output("Models/column_fluted1.jml");
  delete m;

  m = ModelGenerator::GenFlutedColumn(24.0, 36.0, 16, 3.5, 8, 48.0, 48.0, 1.3, 27.5);
  if(m == 0) { return; }
  m->Output("Models/column_fluted2.jml");
  delete m;

  m = ModelGenerator::GenFlutedColumn(24.0, 36.0, 16, 3.5, 8, 72.0, 72.0, 0.5, 27.1);
  if(m == 0) { return; }
  m->Output("Models/column_fluted3.jml");
  delete m;

  m = ModelGenerator::GenPlane(72.0, 72.0, 72.0, 72.0);
  if(m == 0) { return; }
	m->Output("Models/column_cap.jml");
  delete m;

  return;
}

void GenerateCappedColumn(void) {
  ofstream out("Models/column_with_cap.jml");
  if(!out) { return; }

  out << "TYPE COMPOSITE\n";
  out << "NAME ColumnWithCap\n";
  out << "MODEL_FILE Models/column.jml\n";
  out << "MODEL_FILE Models/cap.jml 0.0 228.0 0.0\n";

  out.close();

  GenerateColumn();
  GenerateCap();

  return;
}

void GenerateCap(void) {
  ofstream out("Models/cap.jml");
  if(!out) { return; }

  out << "TYPE COMPOSITE\n";
  out << "NAME Cap\n";

  out << "\n#Generate Fancy Cap\n";
  out << "MODEL_FILE Models/column_with_cap_front.jml Textures/grey_marble_512_512.bmp 0.0 6.0 -36.0\n";
  out << "MODEL_FILE Models/column_with_cap_side.jml Textures/grey_marble_512_512.bmp 36.0 4.5 0.0 1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_with_cap_front.jml Textures/grey_marble_512_512.bmp 0.0 6.0 36.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_with_cap_side.jml Textures/grey_marble_512_512.bmp -36.0 4.5 0.0 -1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_with_cap_top.jml Textures/grey_marble_512_512.bmp 0.0 12.0 0.0 0.0 1.0 0.0 0.0 0.0 -1.0\n";

  out << "MODEL_FILE Models/column_with_cap_cylinder.jml Textures/grey_marble_512_512.bmp -48.0 9.0 -36.0\n";
  out << "MODEL_FILE Models/column_with_cap_cylinder.jml Textures/grey_marble_512_512.bmp 48.0 9.0 36.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_with_cap_cap.jml Textures/grey_marble_512_512.bmp -48.0 9.0 -36.0\n";
  out << "MODEL_FILE Models/column_with_cap_cap_reverse.jml Textures/grey_marble_512_512.bmp 48.0 9.0 -36.0\n";

  out << "MODEL_FILE Models/column_with_cap_cap.jml Textures/grey_marble_512_512.bmp 48.0 9.0 36.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/column_with_cap_cap_reverse.jml Textures/grey_marble_512_512.bmp -48.0 9.0 36.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";

  out.close();

  Model *m = 0;
  m = ModelGenerator::GenSpecificQuad(48.0, 6.0, 0.0, -48.0, 6.0, 0.0, -36.0, -6.0, 0.0, 36.0, -6.0, 0.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/column_with_cap_front.jml");
  delete m;

  m = ModelGenerator::GenSpecificQuad(36.0, 4.5, -9.0, -36.0, 4.5, -9.0, -36.0, -4.5, 0.0, 36.0, -4.5, 0.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/column_with_cap_side.jml");
  delete m;

  m = ModelGenerator::GenSpecificQuad(48.0, 36.0, 0.0, -48.0, 36.0, 0.0, -48.0, -36.0, 0.0, 48.0, -36.0, 0.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/column_with_cap_top.jml");
  delete m;

  m = ModelGenerator::GenCylinder(16, 3.0, 72.0, 0.75, M_PI / 2.0, false, false, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/column_with_cap_cylinder.jml");
  delete m;

  m = ModelGenerator::GenDisc(3.0, 36, 0.75, M_PI / 2.0, 48.0, 48.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/column_with_cap_cap.jml");
  delete m;

  m = ModelGenerator::GenDisc(3.0, 36, 0.75, M_PI, 48.0, 48.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/column_with_cap_cap_reverse.jml");
  delete m;

  return;
}

void GenerateFountain(void) {
  ofstream out("Models/fountain.jml");
  if(!out) { return; }
  
  out << "TYPE COMPOSITE\n";
  out << "NAME Fountain\n";
  out << "MODEL_FILE Models/fountain_interior_wall.jml Textures/tile.bmp 0.0 -24.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/fountain_exterior_wall.jml Textures/grey_marble_512_512.bmp 0.0 0.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/fountain_floor.jml Textures/marble_512_512.bmp 0.0 -24.0 0.0 -1.0 0.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/fountain_seat_top.jml Textures/red_blotch_marble_512_512.bmp 0.0 30.0 0.0 -1.0 0.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/fountain_seat_bottom.jml Textures/grey_marble_512_512.bmp 0.0 24.0 0.0 1.0 0.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/fountain_seat_side.jml Textures/red_blotch_marble_512_512.bmp 0.0 24.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/fountain_tube.jml Textures/grey_marble_512_512.bmp 0.0 -24.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0\n";
  out << "MODEL_FILE Models/fountain_tube_cap.jml Textures/water_128_128.bmp 0.0 30.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0\n";
  
  out.close();

  Model *m = 0;
  m = ModelGenerator::GenRecord(72.0, 90.0, 64, 1.0, 0.0, false, 48.0, 48.0);
  if(m == 0) { return; }
	m->Output("Models/fountain_seat_top.jml");
  delete m;

  m = ModelGenerator::GenRecord(78.0, 90.0, 64, 1.0, 0.0, true, 48.0, 48.0);
  if(m == 0) { return; }
	m->Output("Models/fountain_seat_bottom.jml");
  delete m;

  m = ModelGenerator::GenCylinder(64, 90.0, 6.0, 1.0, 0.0, false, true, 48.0, 48.0);
  if(m == 0) { return; }
	m->Output("Models/fountain_seat_side.jml");
  delete m;

  m = ModelGenerator::GenCylinder(64, 72.0, 54.0, 1.0, 0.0, true, true, 56.5, 54.0, 0.0, 0.0);
  if(m == 0) { return; }
	m->Output("Models/fountain_interior_wall.jml");
  delete m;

  m = ModelGenerator::GenDisc(72.0, 64, 1.0, 0.0, 72.0, 72.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/fountain_floor.jml");
  delete m;

  m = ModelGenerator::GenCylinder(64, 78.0, 24.0, 1.0, 0.0, false, true, 48.0, 48.0);
  if(m == 0) { return; }
	m->Output("Models/fountain_exterior_wall.jml");
  delete m;

  m = ModelGenerator::GenCylinder(8, 2.0, 54.0, 1.0, 0.0, false, true, 48.0, 48.0);
  if(m == 0) { return; }
	m->Output("Models/fountain_tube.jml");
  delete m;

  m = ModelGenerator::GenSphere(2.0, 8, 10.0, 10.0);
  if(m == 0) { return; }
  m->Output("Models/fountain_tube_cap.jml");
  delete m;

  return;
}

void GenerateChair(void) {
  //Model *m = 0;
  return;
}

void GenerateTempleFloor(void) {
  ofstream out("Models/temple_floor.jml");
  if(!out) {
    return;
  }

  out << "TYPE COMPOSITE\n";
  out << "NAME TempleFloor\n";
  out << "MODEL_FILE Models/temple_floor_center.jml Textures/marble_512_512.bmp 0.0 48.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_floor_curve.jml -210.0 48.0 0.0 0.0 1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_floor_curve.jml 210.0 48.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";

  out << "\n#RightWall\n";
  out << "MODEL_FILE Models/temple_floor_curve_side.jml 210.0 48.0 0.0 -1.0 0.0 0.0 0.0 0.0 -1.0\n";

  out << "\n#LeftWall\n";
  out << "MODEL_FILE Models/temple_floor_curve_side.jml -210.0 48.0 0.0 1.0 0.0 0.0 0.0 0.0 -1.0\n";

  out << "\n#FrontBoxes\n";
  out << "MODEL_FILE Models/temple_front_square_top.jml -174.0 48.0 -282.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_front_square_side.jml -210.0 24.0 -282.0 -1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_front_square_side.jml -138.0 24.0 -282.0 1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_front_square_front.jml -174.0 24.0 -354.0\n";

  out << "MODEL_FILE Models/temple_front_square_top.jml 174.0 48.0 -282.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_front_square_side.jml 138.0 24.0 -282.0 -1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_front_square_side.jml 210.0 24.0 -282.0 1.0 0.0 0.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_front_square_front.jml 174.0 24.0 -354.0\n";

  out << "\n#Back wall and floor\n";
  out << "MODEL_FILE Models/temple_back_top.jml 87.0 48.0 246.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_back_top.jml -87.0 48.0 246.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_back_side.jml -210.0 24.0 246.0 -1.0 0.0 0.0 0.0 1.0 0.0 \n";
  out << "MODEL_FILE Models/temple_back_side.jml 210.0 24.0 246.0 1.0 0.0 0.0 0.0 1.0 0.0 \n";
  out << "MODEL_FILE Models/temple_back_back1.jml -174.0 24.0 282.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_back_back1.jml 0.0 24.0 282.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_back_back1.jml 174.0 24.0 282.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_back_back2.jml -87.0 24.0 282.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";
  out << "MODEL_FILE Models/temple_back_back2.jml 87.0 24.0 282.0 0.0 0.0 -1.0 0.0 1.0 0.0\n";

  out << "\n#Steps\n";
  out << "MODEL_FILE Models/temple_step_top3.jml Textures/marble_512_512.bmp 0.0 48.0 -219.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front1.jml Textures/marble_512_512.bmp 0.0 45.0 -228.0\n";
  out << "MODEL_FILE Models/temple_step_top1.jml Textures/marble_512_512.bmp 0.0 42.0 -237.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front3.jml Textures/marble_512_512.bmp 0.0 39.0 -246.0\n";
  out << "MODEL_FILE Models/temple_step_top3.jml Textures/marble_512_512.bmp 0.0 36.0 -255.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front1.jml Textures/marble_512_512.bmp 0.0 33.0 -264.0\n";
  out << "MODEL_FILE Models/temple_step_top2.jml Textures/marble_512_512.bmp 0.0 30.0 -273.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front2.jml Textures/marble_512_512.bmp 0.0 27.0 -282.0\n";
  out << "MODEL_FILE Models/temple_step_top3.jml Textures/marble_512_512.bmp 0.0 24.0 -291.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front3.jml Textures/marble_512_512.bmp 0.0 21.0 -300.0\n";
  out << "MODEL_FILE Models/temple_step_top1.jml Textures/marble_512_512.bmp 0.0 18.0 -309.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front2.jml Textures/marble_512_512.bmp 0.0 15.0 -318.0\n";
  out << "MODEL_FILE Models/temple_step_top1.jml Textures/marble_512_512.bmp 0.0 12.0 -327.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front1.jml Textures/marble_512_512.bmp 0.0 9.0 -336.0\n";
  out << "MODEL_FILE Models/temple_step_top2.jml Textures/marble_512_512.bmp 0.0 6.0 -345.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";
  out << "MODEL_FILE Models/temple_step_front3.jml Textures/marble_512_512.bmp 0.0 3.0 -354.0\n";

  out.close();

  Model *m = 0;

  m = ModelGenerator::GenPlaneWithHole(420.0, 420.0, 72.0, 32, 130.0, 130.0);
  if(m == 0) { return; }
  m->Output("Models/temple_floor_center.jml");
  delete m;

  m = ModelGenerator::GenDisc(210.0, 64, 0.5, M_PI / 2.0, 72.0, 72.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/temple_floor_curve.jml");
  delete m;

  m = ModelGenerator::GenCylinder(64, 210.0, 48.0, 0.5, 0.0, false, false, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_floor_curve_side.jml");
  delete m;

  m = ModelGenerator::GenPlane(72.0, 144.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_front_square_top.jml");
  delete m;

  m = ModelGenerator::GenPlane(144.0, 48.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_front_square_side.jml");
  delete m;

  m = ModelGenerator::GenPlane(72.0, 48.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_front_square_front.jml");
  delete m;

  m = ModelGenerator::GenPlane(102.0, 72.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_back_top.jml");
  delete m;

  m = ModelGenerator::GenPlane(72.0, 48.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_back_side.jml");
  delete m;

  m = ModelGenerator::GenPlane(72.0, 48.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_back_back1.jml");
  delete m;

  m = ModelGenerator::GenPlane(102.0, 48.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_back_back2.jml");
  delete m;

  m = ModelGenerator::GenPlane(276.0, 18.0, 72.0, 36.0, 0.5, 13.5);
  if(m == 0) { return; }
  m->Output("Models/temple_step_top1.jml");
  delete m;

  m = ModelGenerator::GenPlane(276.0, 18.0, 48.0, 48.0, 21.3);
  if(m == 0) { return; }
  m->Output("Models/temple_step_top2.jml");
  delete m;

  m = ModelGenerator::GenPlane(276.0, 18.0, 48.0, 48.0, 10.2, 23.1);
  if(m == 0) { return; }
  m->Output("Models/temple_step_top3.jml");
  delete m;

  m = ModelGenerator::GenPlane(276.0, 6.0, 48.0, 48.0, 12.7, 27.7);
  if(m == 0) { return; }
  m->Output("Models/temple_step_front1.jml");
  delete m;

  m = ModelGenerator::GenPlane(276.0, 6.0, 75.0, 48.0, 0.5, 18.3);
  if(m == 0) { return; }
  m->Output("Models/temple_step_front2.jml");
  delete m;

  m = ModelGenerator::GenPlane(276.0, 6.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/temple_step_front3.jml");
  delete m;

  return;
}

void GenerateGround(void) {
  Model *m = 0;
  m = ModelGenerator::GenPlane(4000.0, 4000.0, 48.0, 48.0);
  if(m == 0) { return; }
  m->Output("Models/ground.jml");
  delete m;

  return;
}

void GenerateSky(void) {
  Model *m = 0;
  m = ModelGenerator::GenSphere(3000.0, 64, M_PI * 2.0 * 3000.0, M_PI * 500.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/night_sky.jml");
  delete m;

  m = ModelGenerator::GenSphere(2900.0, 64, M_PI * 2.0 * 2900.0, M_PI * 500.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/day_sky.jml");
  delete m;

  return;
}

void GenerateBackground(void) {
  ofstream out("Models/mountains.jml");
  if(!out) { return; }
  out << "MODEL_FILE Models/background.jml Textures/mountain_512_512.bmp 0.0 0.0 0.0 0.0 -1.0 0.0 0.0 0.0 1.0\n";
  out.close();

  Model *m = 0;
  m = ModelGenerator::GenCylinder(128, 1800.0, 1000.0, 1.0, 0.0, true, true, 900.0, 1000.0);
  if(m == 0) { return; }
  m->Output("Models/background.jml");
  delete m;

  return;
}

void GenerateSun(void) {
  ofstream out("Models/sun.jml");
  if(!out) {
    return;
  }

  out << "TYPE COMPOSITE\n";
  out << "NAME Sun\n";
  out << "MODEL_FILE Models/sun_disc.jml Textures/sun_512_512.bmp\n";

  out.close();

  Model *m = 0;
  //m = ModelGenerator::GenDisc(250.0, 64, 1.0, 0.0, 250.0, 250.0, 0.0, 0.0, true);
  m = ModelGenerator::GenSphere(250.0, 64, 250.0, 250.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/sun_disc.jml");
  delete m;

  return;
}

void GenerateMoon(void) {
  ofstream out("Models/moon.jml");
  if(!out) {
    return;
  }

  out << "TYPE COMPOSITE\n";
  out << "NAME Moon\n";
  out << "MODEL_FILE Models/moon_disc.jml Textures/moon_512_512.bmp\n";

  out.close();

  Model *m = 0;
  //m = ModelGenerator::GenDisc(225.0, 64, 1.0, 0.0, 225.0, 225.0, 0.0, 0.0, true);
  m = ModelGenerator::GenSphere(225.0, 64, 225.0, 225.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/moon_disc.jml");
  delete m;

  return;
}

void GenerateWater(void) {
  Model *m = 0;
  m = ModelGenerator::GenSphere(2.0, 8, 10.0, 10.0);
  if(m == 0) { return; }
  m->Output("Models/water_sphere.jml");
  delete m;

  ofstream out("Models/water_top.jml");
  if(!out) {
    return;
  }

  out << "TYPE COMPOSITE\n";
  out << "NAME WaterTop\n";
  out << "MODEL_FILE Models/water_for_fountain_disc.jml Textures/water_128_128.bmp 0.0 48.0 0.0 0.0 -1.0 0.0 0.0 0.0 -1.0\n";

  out.close();

  m = ModelGenerator::GenDisc(72.0, 64, 1.0, 0.0, 72.0, 72.0, 0.0, 0.0, true);
  if(m == 0) { return; }
  m->Output("Models/water_for_fountain_disc.jml");
  delete m;

  return;
}
/*** END Model generation functions *****************************************/
