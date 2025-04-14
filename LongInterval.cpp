// 多倍長区間クラス

#include <sstream>
#include <cstdlib>
#include "lilib.h"

using namespace std;
using namespace lilib;

// コンストラクタ
LongInterval::LongInterval(){

  // longFloatBits が未設定の場合
  if(!longFloatBits){
    cerr << "[ERROR] LongInterval() : Precision of LongFloat is not set." << endl;
    exit(1);
  }
}

// コンストラクタ
LongInterval::LongInterval(int x){

  // longFloatBits が未設定の場合
  if(!longFloatBits){
    cerr << "[ERROR] LongInterval(int x) : Precision of LongFloat is not set." << endl;
    exit(1);
  }

  center = x;
  radius = 0;
}

// コンストラクタ
LongInterval::LongInterval(const LongFloat &x){

  // longFloatBits が未設定の場合
  if(!longFloatBits){
    cerr << "[ERROR] LongInterval(int x) : Precision of LongFloat is not set." << endl;
    exit(1);
  }

  center = x;
  radius = 0;
}

// コンストラクタ
LongInterval::LongInterval(int mid, int rad){

  // longFloatBits が未設定の場合
  if(!longFloatBits){
    cerr << "[ERROR] LongInterval(int x) : Precision of LongFloat is not set." << endl;
    exit(1);
  }

  if(rad < 0){
    cerr << "[ERROR] LongInterval(int mid, int rad) : rad must be positive." << endl;
    exit(1);
  }

  center = mid;
  radius = rad;
}

// コンストラクタ
LongInterval::LongInterval(const LongFloat &mid, const Radius &rad){

  // longFloatBits が未設定の場合
  if(!longFloatBits){
    cerr << "[ERROR] LongInterval(int x) : Precision of LongFloat is not set." << endl;
    exit(1);
  }

  center = mid;
  radius = rad;
}

// コピーコンストラクタ
LongInterval::LongInterval(const LongInterval &x){
  center = x.center;
  radius = x.radius;
}

////////////////////////////////////////////////////////////////////////////////

// 中心値と半径を表す文字列の取得
string LongInterval::getMidRad() const{
  ostringstream output;

  output << "<" << center << ", " << radius << ">";

  return output.str();
}

// 下端と上端を表す文字列の取得
string LongInterval::getInfSup() const{
  ostringstream output;

  output << "[" << inf().getString(10, -1) << ", " << sup().getString(10, 1) << "]";

  return output.str();
}

// 内部データを表す文字列の取得
string LongInterval::getInternalData() const{
  ostringstream output;

  output << "center :" << endl;
  output << center.getInternalData() << endl;
  output << endl;
  output << "radius :" << endl;
  output << radius.getInternalData();

  return output.str();
}

////////////////////////////////////////////////////////////////////////////////

// 中心値の取得
LongFloat LongInterval::mid() const{
  return center;
}

// 半径の取得
LongFloat LongInterval::rad() const{
  return LongFloat(radius);
}

// 直径の取得
LongFloat LongInterval::diam() const{
  LongFloat result;

  mpfr_mul_ui(result.value, radius.value, 2, MPFR_RNDU);

  return result;
}

// 下端の取得
LongFloat LongInterval::inf() const{
  LongFloat result;

  mpfr_sub(result.value, center.value, radius.value, MPFR_RNDD);

  return result;
}

// 上端の取得
LongFloat LongInterval::sup() const{
  LongFloat result;

  mpfr_add(result.value, center.value, radius.value, MPFR_RNDU);

  return result;
}

// 最小絶対値の取得
LongFloat LongInterval::mig() const{

  // 0 を含む場合。
  if(mpfr_cmpabs(center.value, radius.value) <= 0){
    return 0;
  }

  if(center.value->_mpfr_sign > 0){
    return inf();
  }
  else{
    return -sup();
  }
}

// 最大絶対値の取得
LongFloat LongInterval::mag() const{
  if(center.value->_mpfr_sign > 0){
    return sup();
  }
  else{
    return -inf();
  }
}

////////////////////////////////////////////////////////////////////////////////

// 逆数区間の取得
LongInterval LongInterval::inverse() const{
  LongInterval z, invZ, rr;

  // z = center ^ 2
  z.center = center * center;
  z.radius = z.center.ulp();

  // rr = radius ^ 2
  mpfr_mul(rr.center.value, radius.value, radius.value, MPFR_RNDN);
  mpfr_set(rr.radius.value, rr.center.ulp().value, MPFR_RNDU);

  z -= rr;

  // invZ.center = 1 / z.center
  mpfr_ui_div(invZ.center.value, 1, z.center.value, MPFR_RNDN);

  LongInterval e = z * invZ.center - 1;

//  LongFloat a = (z * (e - 1) + z.center).mag();
//  LongFloat b = (z * z.center).inf();
  Radius a = Radius((z * (e - 1) + z.center).mag());
  Radius b = Radius((z * z.center).inf());

  // invZ.radius = a / b を上への丸めで計算する。
  mpfr_div(invZ.radius.value, a.value, b.value, MPFR_RNDU);

  return invZ * *this;
}

// 単項演算
////////////////////////////////////////////////////////////////////////////////

// +LongInterval
LongInterval LongInterval::operator +() const{
  return *this;
}

// -LongInterval
LongInterval LongInterval::operator -() const{
  LongInterval result(*this);
  result.center.value->_mpfr_sign *= -1;

  return result;
}

// 四則演算
////////////////////////////////////////////////////////////////////////////////

// LongInterval + int
LongInterval LongInterval::operator +(int x) const{
  LongInterval result;

  result.center = center + x;
  result.radius = radius + result.center.ulp();

  return result;
}

// LongInterval - int
LongInterval LongInterval::operator -(int x) const{
  LongInterval result;

  result.center = center - x;
  result.radius = radius + result.center.ulp();

  return result;
}

// LongInterval * int
LongInterval LongInterval::operator *(int x) const{
  LongInterval result;

  result.center = center * x;
  result.radius = radius * x + result.center.ulp();

  return result;
}

// LongInterval / int
LongInterval LongInterval::operator /(int x) const{
  LongInterval result;

  result.center = center / x;
  result.radius = radius / x + result.center.ulp();

  return result;
}

// LongInterval + LongFloat
LongInterval LongInterval::operator +(const LongFloat &x) const{
  LongInterval result;

  result.center = center + x;
  result.radius = radius + result.center.ulp();

  return result;
}

// LongInterval - LongFloat
LongInterval LongInterval::operator -(const LongFloat &x) const{
  LongInterval result;

  result.center = center - x;
  result.radius = radius + result.center.ulp();

  return result;
}

// LongInterval * LongFloat
LongInterval LongInterval::operator *(const LongFloat &x) const{
  LongInterval result;

  result.center = center * x;
  result.radius = radius * Radius(x) + result.center.ulp();

  return result;
}

// LongInterval / LongFloat
LongInterval LongInterval::operator /(const LongFloat &x) const{
  LongInterval result;

  result.center = center / x;
  result.radius = radius / Radius(x) + result.center.ulp();

  return result;
}

// LongInterval + LongInterval
LongInterval LongInterval::operator +(const LongInterval &x) const{
  LongInterval result;

  result.center = center + x.center;
  result.radius = radius + x.radius + result.center.ulp();

  return result;
}

// LongInterval - LongInterval
LongInterval LongInterval::operator -(const LongInterval &x) const{
  LongInterval result;

  result.center = center - x.center;
  result.radius = radius + x.radius + result.center.ulp();

  return result;
}

// LongInterval * LongInterval
LongInterval LongInterval::operator *(const LongInterval &x) const{
  LongInterval result;

  result.center = center * x.center;
  result.radius = (Radius(center) + radius) * x.radius + Radius(x.center) * radius + result.center.ulp();

  return result;
}

// LongInterval / LongInterval
LongInterval LongInterval::operator /(const LongInterval &x) const{
  return *this * x.inverse();
}

// 代入演算
////////////////////////////////////////////////////////////////////////////////

// LongInterval = int
LongInterval &LongInterval::operator =(int x){
  center = x;
  radius = 0;

  return *this;
}

// LongInterval += int
LongInterval &LongInterval::operator +=(int x){
  center += x;
  radius += center.ulp();

  return *this;
}

// LongInterval -= int
LongInterval &LongInterval::operator -=(int x){
  center -= x;
  radius += center.ulp();

  return *this;
}

// LongInterval *= int
LongInterval &LongInterval::operator *=(int x){
  center *= x;
  radius = radius * x + center.ulp();

  return *this;
}

// LongInterval /= int
LongInterval &LongInterval::operator /=(int x){
  center /= x;
  radius = radius / x + center.ulp();

  return *this;
}

// LongInterval = LongFloat
LongInterval &LongInterval::operator =(const LongFloat &x){
  center = x;
  radius = 0;

  return *this;
}

// LongInterval += LongFloat
LongInterval &LongInterval::operator +=(const LongFloat &x){
  center += x;
  radius += center.ulp();

  return *this;
}

// LongInterval -= LongFloat
LongInterval &LongInterval::operator -=(const LongFloat &x){
  center -= x;
  radius += center.ulp();

  return *this;
}

// LongInterval *= LongFloat
LongInterval &LongInterval::operator *=(const LongFloat &x){
  center *= x;
  radius = radius * x + center.ulp();

  return *this;
}

// LongInterval /= LongFloat
LongInterval &LongInterval::operator /=(const LongFloat &x){
  center /= x;
  radius = radius / x + center.ulp();

  return *this;
}

// LongInterval = LongInterval
LongInterval &LongInterval::operator =(const LongInterval &x){
  center = x.center;
  radius = x.radius;

  return *this;
}

// LongInterval += LongInterval
LongInterval &LongInterval::operator +=(const LongInterval &x){
  center += x.center;
  radius += x.radius + center.ulp();

  return *this;
}

// LongInterval -= LongInterval
LongInterval &LongInterval::operator -=(const LongInterval &x){
  center -= x.center;
  radius += x.radius + center.ulp();

  return *this;
}

// LongInterval *= LongInterval
LongInterval &LongInterval::operator *=(const LongInterval &x){
  radius = (Radius(center) + radius) * x.radius + Radius(x.center) * radius;
  center *= x.center;
  radius += center.ulp();

  return *this;
}

// LongInterval /= LongInterval
LongInterval &LongInterval::operator /=(const LongInterval &x){
  *this *= x.inverse();

  return *this;
}

// 四則演算（フレンド関数）
////////////////////////////////////////////////////////////////////////////////

// int + LongInterval
LongInterval operator +(int a, const LongInterval &b){
  LongInterval result;

  result.center = a + b.center;
  result.radius = b.radius + result.center.ulp();

  return result;
}

// int - LongInterval
LongInterval operator -(int a, const LongInterval &b){
  LongInterval result;

  result.center = a - b.center;
  result.radius = b.radius + result.center.ulp();

  return result;
}

// int * LongInterval
LongInterval operator *(int a, const LongInterval &b){
  LongInterval result;

  result.center = a * b.center;
  result.radius = a * b.radius + result.center.ulp();

  return result;
}

// int / LongInterval
LongInterval operator /(int a, const LongInterval &b){
  return a * b.inverse();
}

////////////////////////////////////////////////////////////////////////////////

// std::ostream << LongInterval
ostream &operator <<(ostream &output, const LongInterval &x){
  output << x.getMidRad();

  return output;
}
