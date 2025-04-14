// 区間半径クラス

#include <sstream>
#include <iomanip>
#include <cmath>
#include "lilib.h"

using namespace std;
using namespace lilib;

// コンストラクタ
Radius::Radius(){
  mpfr_init2(value, RADIUS_BITS);
}

// コンストラクタ
Radius::Radius(double x){
  if(x < 0){
    x = -x;
  }

  mpfr_init2(value, RADIUS_BITS);

  mpfr_set_d(value, x, MPFR_RNDU);
}

// コピーコンストラクタ
Radius::Radius(const Radius &x){
  mpfr_init2(value, RADIUS_BITS);

  mpfr_set(value, x.value, MPFR_RNDU);
}

// コンストラクタ
Radius::Radius(const LongFloat &x){
  LongFloat xPlus(x);

  xPlus.value->_mpfr_sign = 1;

  mpfr_init2(value, RADIUS_BITS);

  mpfr_set(value, xPlus.value, MPFR_RNDU);
}

// デストラクタ
Radius::~Radius(){
  mpfr_clear(value);
}

////////////////////////////////////////////////////////////////////////////////

// double からの変換
void Radius::setDouble(double x){
  if(x < 0){
    x = -x;
  }

  mpfr_set_d(value, x, MPFR_RNDU);
}

////////////////////////////////////////////////////////////////////////////////

// double への変換
double Radius::getDouble() const{
  return mpfr_get_d(value, MPFR_RNDU);
}

// 文字列への変換
string Radius::getString() const{
  char s[BUFFER_SIZE];

  mpfr_sprintf(s, "%.9RUe", value);

  return s;
}

// 内部データを表す文字列の取得
string Radius::getInternalData() const{
  ostringstream output;

  int n = ceil(value->_mpfr_prec / 64.0);
  int b = GMP_LIMB_BITS / 4;

  output << "prec = " << value->_mpfr_prec << ", ";
  output << "sign = " << value->_mpfr_sign << ", ";
  output << "exp = " << value->_mpfr_exp << endl;

  output << "d = ";
  for(int i = 0; i < n; i++){
    output << hex << setw(b) << setfill('0') << value->_mpfr_d[i] << " ";
  }

  return output.str();
}

// 四則演算
////////////////////////////////////////////////////////////////////////////////

// Radius * int
Radius Radius::operator *(int x) const{
  Radius result;

  if(x < 0){
    x = -x;
  }

  mpfr_mul_ui(result.value, value, x, MPFR_RNDU);

  return result;
}

// Radius / int
Radius Radius::operator /(int x) const{
  Radius result;

  if(x < 0){
    x = -x;
  }

  mpfr_div_ui(result.value, value, x, MPFR_RNDU);

  return result;
}

// Radius + Radius
Radius Radius::operator +(const Radius &x) const{
  Radius result;

  mpfr_add(result.value, value, x.value, MPFR_RNDU);

  return result;
}

// Radius * Radius
Radius Radius::operator *(const Radius &x) const{
  Radius result;

  mpfr_mul(result.value, value, x.value, MPFR_RNDU);

  return result;
}

// Radius / Radius
Radius Radius::operator /(const Radius &x) const{
  Radius result;

  mpfr_div(result.value, value, x.value, MPFR_RNDU);

  return result;
}

// 比較演算
////////////////////////////////////////////////////////////////////////////////

// Radius == Radius
bool Radius::operator ==(const Radius &x) const{
  if(mpfr_cmp(value, x.value)){
    return false;
  }
  else{
    return true;
  }
}

// Radius != Radius
bool Radius::operator !=(const Radius &x) const{
  if(mpfr_cmp(value, x.value)){
    return true;
  }
  else{
    return false;
  }
}

// 代入演算
////////////////////////////////////////////////////////////////////////////////

// Radius = int
Radius &Radius::operator =(int x){
  if(x < 0){
    x = -x;
  }

  mpfr_set_ui(value, x, MPFR_RNDU);

  return *this;
}

// Radius *= int
Radius &Radius::operator *=(int x){
  if(x < 0){
    x = -x;
  }

  mpfr_mul_ui(value, value, x, MPFR_RNDU);

  return *this;
}

// Radius /= int
Radius &Radius::operator /=(int x){
  if(x < 0){
    x = -x;
  }

  mpfr_div_ui(value, value, x, MPFR_RNDU);

  return *this;
}

// Radius = Radius
Radius &Radius::operator =(const Radius &x){
  mpfr_set(value, x.value, MPFR_RNDU);

  return *this;
}

// Radius += Radius
Radius &Radius::operator +=(const Radius &x){
  mpfr_add(value, value, x.value, MPFR_RNDU);

  return *this;
}

// Radius *= Radius
Radius &Radius::operator *=(const Radius &x){
  mpfr_mul(value, value, x.value, MPFR_RNDU);

  return *this;
}

// Radius /= Radius
Radius &Radius::operator /=(const Radius &x){
  mpfr_div(value, value, x.value, MPFR_RNDU);

  return *this;
}

// 四則演算（フレンド関数）
////////////////////////////////////////////////////////////////////////////////

// int * Radius
Radius operator *(int a, const Radius &b){
  Radius result;

  if(a < 0){
    a = -a;
  }

  mpfr_mul_ui(result.value, b.value, a, MPFR_RNDU);

  return result;
}

////////////////////////////////////////////////////////////////////////////////

// std::ostream << Radius
ostream &operator <<(ostream &output, const Radius &x){
  output << x.getString();

  return output;
}
