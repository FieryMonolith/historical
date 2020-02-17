#include <iostream>

#include "jvector.h"

using namespace std;

int main(void) {
  Vector3d x(1.0, 1.0, 1.0);
  Vector3d y(-1.0, 0.0, -1.0);
  double magx = Normalize(x);
  double magy = Normalize(y);

  cout << "X(" << magx << "): " << x[0] << " " << x[1] << " " << x[2] << endl;
  cout << "Y(" << magy << "): " << y[0] << " " << y[1] << " " << y[2] << endl;

  return 0;
}
