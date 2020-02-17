#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

#include <math.h>

int main(int argc, char **argv) {
  printf("Start\n");

  Matrix R;
  R.calcO(3.14159/3.0, 0, 0, 0, 0, 0);

  Matrix T;
  T.calcO(0,0,0,1,1,1);

  Matrix Tr=R*T;
  Matrix Ri=Transpose(R);
  Matrix To=Ri*Tr;

  printf("R:\n"); R.print();
  printf("T:\n"); T.print();
  printf("Tr:\n"); Tr.print();
  printf("Ri:\n"); Ri.print();
  printf("To:\n"); To.print();
  printf(" ... where T and To should be equal.\n");

  printf("Tr*Ri:\n"); (Tr*Ri).print();
  printf("R*Ri:\n"); (R*Ri).print();
  printf(" ... where R*Ri should be the identity.\n");

  Vector up(0,0,1), fw(0,1,1);
  printf("Angle(up,fw): %f\n", Angle(up,fw));
  Vector cross=Cross(fw,up);
  printf("Cross: (%f,%f,%f)\n", cross.x, cross.y, cross.z);

  printf("Length(up): %f, Length(Cross(fw, up)): %f, Inner(fw, up): %f\n",
    Length(up), Length(Cross(fw, up)), Inner(fw, up)
  );
  printf("atan2(1,1): %f\n", atan2(1,1));

  Vector foo(1,1,1);
  R.rotation(0.76, foo/Length(foo));
  R.fixupRotation();
  printf("R:\n"); R.print();

  //Vector tX(R.v00 - 1.0, R.v10, R.v20);
  //Vector tY(R.v01, R.v11 - 1.0, R.v21);
  //Vector tZ(R.v02, R.v12, R.v22 - 1.0);
  //printf("(%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n",
  //  tX.x, tX.y, tX.z,
  //  tY.x, tY.y, tY.z,
  //  tZ.x, tZ.y, tZ.z
  //);

  printf("Finish\n");

  return 1;
}
