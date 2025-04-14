// 多倍長実数クラス

#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include "lilib.h"

using namespace std;
using namespace lilib;

////////////////////////////////////////////////////////////////////////////////

// コンストラクタ
LongFloat::LongFloat(){

  // longFloatBits が未設定の場合
  if(!longFloatBits){
    cerr << "[ERROR] LongFloat() : Precision of LongFloat is not set." << endl;
    exit(1);
  }

  mpfr_init2(value, longFloatBits);
}

// コンストラクタ
LongFloat::LongFloat(int x){

  // longFloatBits が未設定の場合
  if(!longFloatBits){
    cerr << "[ERROR] LongFloat(int x) : Precision of LongFloat is not set." << endl;
    exit(1);
  }

  mpfr_init2(value, longFloatBits);

  mpfr_set_si(value, x, MPFR_RNDN);
}

// コンストラクタ
LongFloat::LongFloat(const Radius &x){
  mpfr_init2(value, longFloatBits);

  mpfr_set(value, x.value, MPFR_RNDN);
}

// コピーコンストラクタ
LongFloat::LongFloat(const LongFloat &x){
  mpfr_init2(value, longFloatBits);

  mpfr_set(value, x.value, MPFR_RNDN);
}

// デストラクタ
LongFloat::~LongFloat(){
  mpfr_clear(value);
}

////////////////////////////////////////////////////////////////////////////////

// double からの変換
void LongFloat::setDouble(double x){
  mpfr_set_d(value, x, MPFR_RNDU);
}

////////////////////////////////////////////////////////////////////////////////

// 文字列への変換
string LongFloat::getString() const{
  char s[BUFFER_SIZE];

  mpfr_sprintf(s, "%.9Re", value);

  return s;
}

// 文字列への変換(桁数・丸め方向指定)
string LongFloat::getString(int digits, int round) const{
  if(digits < 1){
    cerr << "[ERROR] LongFloat.getString(int digits) : digits must be larger than 1." << endl;
    exit(1);
  }

  char *s = new char[digits + BUFFER_SIZE];

  if(round < 0){
    sprintf(s, "%%.%dRDe", digits - 1);
  }
  else{
    if(round > 0){
      sprintf(s, "%%.%dRUe", digits - 1);
    }
    else{
      sprintf(s, "%%.%dRNe", digits - 1);
    }
  }

  mpfr_sprintf(s, s, value);

  string str(s);
  delete [] s;

  return str;
}

// 内部データを表す文字列の取得
string LongFloat::getInternalData() const{
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

////////////////////////////////////////////////////////////////////////////////

// 1 ulp を求める
Radius LongFloat::ulp() const{

  // 0 の場合
  if(mpfr_zero_p(value)){
    return 0;
  }

  Radius result;

  mpfr_set_ui(result.value, 1, MPFR_RNDN);
  result.value->_mpfr_exp = value->_mpfr_exp - value->_mpfr_prec + 1;

  return result;
}

// 中心値 *this 、半径 1 ulp の区間を取得
LongInterval LongFloat::addUlp() const{
  LongInterval result;

  result.center = *this;
  result.radius = ulp();
//  mpfr_set(result.radius.value, ulp().value, MPFR_RNDU);

  return result;
}

// 値を 0 以上 1 未満の乱数にする
void LongFloat::random(){
  mpfr_urandomb(value, randomState);
}

// 単項演算
////////////////////////////////////////////////////////////////////////////////

// +LongFloat
LongFloat LongFloat::operator +() const{
  return *this;
}

// -LongFloat
LongFloat LongFloat::operator -() const{
  LongFloat result(*this);
  result.value->_mpfr_sign *= -1;

  return result;
}

// 四則演算
////////////////////////////////////////////////////////////////////////////////

// LongFloat + int
LongFloat LongFloat::operator +(int x) const{
  LongFloat result;

  mpfr_add_si(result.value, value, x, MPFR_RNDN);

  return result;
}

// LongFloat - int
LongFloat LongFloat::operator -(int x) const{
  LongFloat result;

  mpfr_sub_si(result.value, value, x, MPFR_RNDN);

  return result;
}

// LongFloat * int
LongFloat LongFloat::operator *(int x) const{
  LongFloat result;

  mpfr_mul_si(result.value, value, x, MPFR_RNDN);

  return result;
}

// LongFloat / int
LongFloat LongFloat::operator /(int x) const{
  LongFloat result;

  mpfr_div_si(result.value, value, x, MPFR_RNDN);

  return result;
}

// LongFloat + LongFloat
LongFloat LongFloat::operator +(const LongFloat &x) const{
  LongFloat result;

  mpfr_add(result.value, value, x.value, MPFR_RNDN);

  return result;
}

// LongFloat - LongFloat
LongFloat LongFloat::operator -(const LongFloat &x) const{
  LongFloat result;

  mpfr_sub(result.value, value, x.value, MPFR_RNDN);

  return result;
}

// LongFloat * LongFloat
LongFloat LongFloat::operator *(const LongFloat &x) const{
  LongFloat result;

  mpfr_mul(result.value, value, x.value, MPFR_RNDN);

  return result;
}

// LongFloat / LongFloat
LongFloat LongFloat::operator /(const LongFloat &x) const{
  LongFloat result;

  mpfr_div(result.value, value, x.value, MPFR_RNDN);

  return result;
}

// LongFloat + LongInterval
LongInterval LongFloat::operator +(const LongInterval &x) const{
  LongInterval result;

  result.center = *this + x.center;
  result.radius = x.radius + result.center.ulp();

  return result;
}

// LongFloat - LongInterval
LongInterval LongFloat::operator -(const LongInterval &x) const{
  LongInterval result;

  result.center = *this - x.center;
  result.radius = x.radius + result.center.ulp();

  return result;
}

// LongFloat * LongInterval
LongInterval LongFloat::operator *(const LongInterval &x) const{
  LongInterval result;

  result.center = *this * x.center;
  result.radius = Radius(*this) * x.radius + result.center.ulp();

  return result;
}

// LongFloat / LongInterval
LongInterval LongFloat::operator /(const LongInterval &x) const{
  LongInterval result;

  result = *this * x.inverse();

  return result;
}

// 比較演算
////////////////////////////////////////////////////////////////////////////////

// LongFloat == int
bool LongFloat::operator ==(int x) const{
  return mpfr_cmp_si(value, x) == 0;
}

// LongFloat != int
bool LongFloat::operator !=(int x) const{
  return mpfr_cmp_si(value, x) != 0;
}

// LongFloat < int
bool LongFloat::operator <(int x) const{
  return mpfr_cmp_si(value, x) < 0;
}

// LongFloat > int
bool LongFloat::operator >(int x) const{
  return mpfr_cmp_si(value, x) > 0;
}

// LongFloat <= int
bool LongFloat::operator <=(int x) const{
  return mpfr_cmp_si(value, x) <= 0;
}

// LongFloat >= int
bool LongFloat::operator >=(int x) const{
  return mpfr_cmp_si(value, x) >= 0;
}

// LongFloat == LongFloat
bool LongFloat::operator ==(const LongFloat &x) const{
  return mpfr_equal_p(value, x.value);
}

// LongFloat != LongFloat
bool LongFloat::operator !=(const LongFloat &x) const{
  return !mpfr_equal_p(value, x.value);
}

// LongFloat < LongFloat
bool LongFloat::operator <(const LongFloat &x) const{
  return mpfr_less_p(value, x.value);
}

// LongFloat > LongFloat
bool LongFloat::operator >(const LongFloat &x) const{
  return mpfr_greater_p(value, x.value);
}

// LongFloat <= LongFloat
bool LongFloat::operator <=(const LongFloat &x) const{
  return mpfr_lessequal_p(value, x.value);
}

// LongFloat >= LongFloat
bool LongFloat::operator >=(const LongFloat &x) const{
  return mpfr_greaterequal_p(value, x.value);
}

// 代入演算
////////////////////////////////////////////////////////////////////////////////

// LongFloat = int
LongFloat &LongFloat::operator =(int x){
  mpfr_set_si(value, x, MPFR_RNDN);

  return *this;
}

// LongFloat *= int
LongFloat &LongFloat::operator *=(int x){
  mpfr_mul_si(value, value, x, MPFR_RNDN);

  return *this;
}

// LongFloat /= int
LongFloat &LongFloat::operator /=(int x){
  mpfr_div_si(value, value, x, MPFR_RNDN);

  return *this;
}

// LongFloat = LongFloat
LongFloat &LongFloat::operator =(const LongFloat &x){
  mpfr_set(value, x.value, MPFR_RNDN);

  return *this;
}

// LongFloat += LongFloat
LongFloat &LongFloat::operator +=(const LongFloat &x){
  mpfr_add(value, value, x.value, MPFR_RNDN);

  return *this;
}

// LongFloat -= LongFloat
LongFloat &LongFloat::operator -=(const LongFloat &x){
  mpfr_sub(value, value, x.value, MPFR_RNDN);

  return *this;
}

// LongFloat *= LongFloat
LongFloat &LongFloat::operator *=(const LongFloat &x){
  mpfr_mul(value, value, x.value, MPFR_RNDN);

  return *this;
}

// LongFloat /= LongFloat
LongFloat &LongFloat::operator /=(const LongFloat &x){
  mpfr_div(value, value, x.value, MPFR_RNDN);

  return *this;
}

// 四則演算（フレンド関数）
////////////////////////////////////////////////////////////////////////////////

// int + LongFloat
LongFloat operator +(int a, const LongFloat &b){
  LongFloat result;

  mpfr_add_si(result.value, b.value, a, MPFR_RNDN);

  return result;
}

// int - LongFloat
LongFloat operator -(int a, const LongFloat &b){
  LongFloat result;

  mpfr_sub_si(result.value, b.value, a, MPFR_RNDN);
  result.value->_mpfr_sign *= -1;

  return result;
}

// int * LongFloat
LongFloat operator *(int a, const LongFloat &b){
  LongFloat result;

  mpfr_mul_si(result.value, b.value, a, MPFR_RNDN);

  return result;
}

// int / LongFloat
LongFloat operator /(int a, const LongFloat &b){
  LongFloat result;

  mpfr_si_div(result.value, a, b.value, MPFR_RNDN);

  return result;
}

// 比較演算（フレンド関数）
////////////////////////////////////////////////////////////////////////////////

// int == LongFloat
bool operator ==(int a, const LongFloat &b){
  return mpfr_cmp_si(b.value, a) == 0;
}

// int != LongFloat
bool operator !=(int a, const LongFloat &b){
  return mpfr_cmp_si(b.value, a) != 0;
}

// int < LongFloat
bool operator <(int a, const LongFloat &b){
  return mpfr_cmp_si(b.value, a) > 0;
}

// int > LongFloat
bool operator >(int a, const LongFloat &b){
  return mpfr_cmp_si(b.value, a) < 0;
}

// int <= LongFloat
bool operator <=(int a, const LongFloat &b){
  return mpfr_cmp_si(b.value, a) >= 0;
}

// int >= LongFloat
bool operator >=(int a, const LongFloat &b){
  return mpfr_cmp_si(b.value, a) <= 0;
}

// フレンド関数
////////////////////////////////////////////////////////////////////////////////

// 絶対値
LongFloat abs(const LongFloat &x){
  if(x.value->_mpfr_sign > 0){
    return x;
  }
  else{
    return -x;
  }
}

// べき乗
LongFloat pow(const LongFloat &x, int n){
  LongFloat result;

  mpfr_pow_si(result.value, x.value, n, MPFR_RNDN);

  return result;
}

// べき乗
LongFloat pow(const LongFloat &a, const LongFloat &b){
  LongFloat result;

  mpfr_pow(result.value, a.value, b.value, MPFR_RNDN);

  return result;
}

// 平方根
LongFloat sqrt(const LongFloat &x){
  LongFloat result;

  mpfr_sqrt(result.value, x.value, MPFR_RNDN);

  return result;
}

////////////////////////////////////////////////////////////////////////////////

// std::ostream << LongFloat
ostream &operator <<(ostream &output, const LongFloat &x){
  output << x.getString();

  return output;
}
