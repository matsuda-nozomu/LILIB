#include <iostream>
#include "lilib.h"

using namespace std;

int main(){
  lilib::setPrecision(100);
  cout << "Long precision is " << lilib::getPrecision() << "." << endl;
  cout << endl;

  LongInterval x;

  x = 1;
  cout << "x = " << x << endl;

  x /= 3;
  cout << "x = " << x << endl;

  x *= 3;
  cout << "x = " << x << endl;
  cout << endl;


  int m = 3, n = 2, i, j;
  LongIntervalMatrix a(m, n), b, c;

  for(i = 0; i < m; i++){
    for(j = 0; j < n; j++){
      a[i][j] = n * i + j;
    }
  }

  b = trans(a);
  c = a * b;

  cout << "a =" << endl << a << endl;
  cout << "b =" << endl << b << endl;
  cout << "c =" << endl << c << endl;

  return 0;
}
