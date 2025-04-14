#pragma once

#include <iostream>
#include <mpfr.h>

// クラスの前方宣言
////////////////////////////////////////////////////////////////////////////////

namespace lilib{
  class Radius;
  class LongFloat;
  class LongInterval;
}

// フレンド関数
////////////////////////////////////////////////////////////////////////////////

lilib::Radius operator *(int a, const lilib::Radius &b);		// int * Radius

lilib::LongFloat operator +(int a, const lilib::LongFloat &b);		// int + LongFloat
lilib::LongFloat operator -(int a, const lilib::LongFloat &b);		// int - LongFloat
lilib::LongFloat operator *(int a, const lilib::LongFloat &b);		// int * LongFloat
lilib::LongFloat operator /(int a, const lilib::LongFloat &b);		// int / LongFloat

bool operator ==(int a, const lilib::LongFloat &b);			// int == LongFloat
bool operator !=(int a, const lilib::LongFloat &b);			// int != LongFloat
bool operator <(int a, const lilib::LongFloat &b);			// int < LongFloat
bool operator >(int a, const lilib::LongFloat &b);			// int > LongFloat
bool operator <=(int a, const lilib::LongFloat &b);			// int <= LongFloat
bool operator >=(int a, const lilib::LongFloat &b);			// int >= LongFloat

lilib::LongFloat abs(const lilib::LongFloat &x);			// 絶対値
lilib::LongFloat pow(const lilib::LongFloat &x, int n);			// べき乗
lilib::LongFloat pow(const lilib::LongFloat &a, const lilib::LongFloat &b);	// べき乗
lilib::LongFloat sqrt(const lilib::LongFloat &x);			// 平方根

lilib::LongInterval operator +(int a, const lilib::LongInterval &b);	// int + LongInterval
lilib::LongInterval operator -(int a, const lilib::LongInterval &b);	// int - LongInterval
lilib::LongInterval operator *(int a, const lilib::LongInterval &b);	// int * LongInterval
lilib::LongInterval operator /(int a, const lilib::LongInterval &b);	// int / LongInterval

namespace lilib{

// ライブラリ全体で共用する定数・変数・関数
////////////////////////////////////////////////////////////////////////////////

  extern const int RADIUS_BITS;			// Radius のビット数
  extern const int BUFFER_SIZE;			// 文字列表示用のバッファサイズ
  extern int longFloatBits;			// LongFloat のビット数、未設定状態は 0

  extern gmp_randstate_t randomState;		// 乱数生成器

  void setPrecision(int precision);		// 多倍長数の精度の設定
  int getPrecision();				// 多倍長数の精度の取得

  void initRandom(unsigned long int seed);	// 乱数の初期化

////////////////////////////////////////////////////////////////////////////////

  // 区間半径クラス
  class Radius{
    private:
    public:
      mpfr_t value;

//    public:
      Radius();							// コンストラクタ
      Radius(double x);						// コンストラクタ
      Radius(const Radius &x);					// コピーコンストラクタ
      Radius(const LongFloat &x);				// コンストラクタ
      ~Radius();						// デストラクタ

      void setDouble(double x);					// double からの変換

      double getDouble() const;					// double への変換
      std::string getString() const;				// 文字列への変換
      std::string getInternalData() const;			// 内部データを表す文字列の取得

//      void random(int exponentMin, int exponentMax);		// 値を乱数にする(テスト用)

      Radius operator *(int x) const;				// Radius * int
      Radius operator /(int x) const;				// Radius / int

      Radius operator +(const Radius &x) const;			// Radius + Radius
      Radius operator *(const Radius &x) const;			// Radius * Radius
      Radius operator /(const Radius &x) const;			// Radius / Radius

      bool operator ==(const Radius &x) const;			// Radius == Radius
      bool operator !=(const Radius &x) const;			// Radius != Radius

      Radius &operator =(int x);				// Radius = int
      Radius &operator *=(int x);				// Radius *= int
      Radius &operator /=(int x);				// Radius /= int

      Radius &operator =(const Radius &x);			// Radius = Radius
      Radius &operator +=(const Radius &x);			// Radius += Radius
      Radius &operator *=(const Radius &x);			// Radius *= Radius
      Radius &operator /=(const Radius &x);			// Radius /= Radius

    friend Radius (::operator *)(int a, const Radius &b);	// int * Radius

    friend class LongFloat;					// 多倍長実数クラス
    friend class LongInterval;					// 多倍長区間クラス

//    friend LongInterval sqrt(const LongInterval &x);	// 平方根
  };

////////////////////////////////////////////////////////////////////////////////

  // 多倍長実数クラス
  class LongFloat{
    private:
    public:
      mpfr_t value;

//      void increase();					// 最下位ビットに 1 加える
//      void decrease();					// 最下位ビットから 1 減じる

//      void copy(const LongFloat &x, int size);		// limb 数の異なる LongFloat を代入する

//      void readString(std::string s);				// 文字列を読み込む

//      LongFloat sqrtInf() const;				// 平方根高速版(下への丸め)

//    public:
      LongFloat();						// コンストラクタ
      LongFloat(int x);						// コンストラクタ
      LongFloat(const Radius &x);				// コンストラクタ
//      LongFloat(std::string s);					// コンストラクタ
      LongFloat(const LongFloat &x);				// コピーコンストラクタ
      ~LongFloat();						// デストラクタ

      void setDouble(double x);					// double からの変換
//      void setString(std::string s);				// 文字列からの変換

//      double getDouble() const;					// double への変換
//      double getDouble(int round) const;			// double への変換(丸め方向指定)
      std::string getString() const;				// 文字列への変換
      std::string getString(int digits, int round) const;	// 文字列への変換(桁数・丸め方向指定)
      std::string getInternalData() const;			// 内部データを表す文字列の取得

      Radius ulp() const;					// 1 ulp を求める
      LongInterval addUlp() const;				// 中心値 *this 、半径 1 ulp の区間を取得

      void random();						// 値を 0 以上 1 未満の乱数にする
//      void random(int exponentMin, int exponentMax);		// 値を乱数にする(テスト用)

      LongFloat operator +() const;				// +LongFloat
      LongFloat operator -() const;				// -LongFloat

      LongFloat operator +(int x) const;			// LongFloat + int
      LongFloat operator -(int x) const;			// LongFloat - int
      LongFloat operator *(int x) const;			// LongFloat * int
      LongFloat operator /(int x) const;			// LongFloat / int

//      LongFloat operator *(const Radius &x) const;		// LongFloat * Radius

      LongFloat operator +(const LongFloat &x) const;		// LongFloat + LongFloat
      LongFloat operator -(const LongFloat &x) const;		// LongFloat - LongFloat
      LongFloat operator *(const LongFloat &x) const;		// LongFloat * LongFloat
      LongFloat operator /(const LongFloat &x) const;		// LongFloat / LongFloat

      LongInterval operator +(const LongInterval &x) const;	// LongFloat + LongInterval
      LongInterval operator -(const LongInterval &x) const;	// LongFloat - LongInterval
      LongInterval operator *(const LongInterval &x) const;	// LongFloat * LongInterval
      LongInterval operator /(const LongInterval &x) const;	// LongFloat / LongInterval

      bool operator ==(int x) const;				// LongFloat == int
      bool operator !=(int x) const;				// LongFloat != int
      bool operator <(int x) const;				// LongFloat < int
      bool operator >(int x) const;				// LongFloat > int
      bool operator <=(int x) const;				// LongFloat <= int
      bool operator >=(int x) const;				// LongFloat >= int

      bool operator ==(const LongFloat &x) const;		// LongFloat == LongFloat
      bool operator !=(const LongFloat &x) const;		// LongFloat != LongFloat
      bool operator <(const LongFloat &x) const;		// LongFloat < LongFloat
      bool operator >(const LongFloat &x) const;		// LongFloat > LongFloat
      bool operator <=(const LongFloat &x) const;		// LongFloat <= LongFloat
      bool operator >=(const LongFloat &x) const;		// LongFloat >= LongFloat

//      int operator <(const  LongInterval &x) const;		// LongFloat < LongInterval
//      int operator >(const  LongInterval &x) const;		// LongFloat > LongInterval
//      int operator <=(const LongInterval &x) const;		// LongFloat <= LongInterval
//      int operator >=(const LongInterval &x) const;		// LongFloat >= LongInterval

      LongFloat &operator =(int x);				// LongFloat = int
      LongFloat &operator *=(int x);				// LongFloat *= int
      LongFloat &operator /=(int x);				// LongFloat /= int

      LongFloat &operator =(const LongFloat &x);		// LongFloat = LongFloat
//      LongFloat &operator =(LongFloat &&x);			// LongFloat = LongFloat
      LongFloat &operator +=(const LongFloat &x);		// LongFloat += LongFloat
      LongFloat &operator -=(const LongFloat &x);		// LongFloat -= LongFloat
      LongFloat &operator *=(const LongFloat &x);		// LongFloat *= LongFloat
      LongFloat &operator /=(const LongFloat &x);		// LongFloat /= LongFloat

    friend LongFloat (::operator +)(int a, const LongFloat &b);	// int + LongFloat
    friend LongFloat (::operator -)(int a, const LongFloat &b);	// int - LongFloat
    friend LongFloat (::operator *)(int a, const LongFloat &b);	// int * LongFloat
    friend LongFloat (::operator /)(int a, const LongFloat &b);	// int / LongFloat

    friend bool ::operator ==(int a, const LongFloat &b);	// int == LongFloat
    friend bool ::operator !=(int a, const LongFloat &b);	// int != LongFloat
    friend bool ::operator <(int a, const LongFloat &b);	// int < LongFloat
    friend bool ::operator >(int a, const LongFloat &b);	// int > LongFloat
    friend bool ::operator <=(int a, const LongFloat &b);	// int <= LongFloat
    friend bool ::operator >=(int a, const LongFloat &b);	// int >= LongFloat

    friend LongFloat (::abs)(const LongFloat &x);			// 絶対値
    friend LongFloat (::pow)(const LongFloat &x, int n);		// べき乗
    friend LongFloat (::pow)(const LongFloat &a, const LongFloat &b);	// べき乗
    friend LongFloat (::sqrt)(const LongFloat &x);			// 平方根

    friend class Radius;					// 区間半径クラス
    friend class LongInterval;					// 多倍長区間クラス

//    friend LongInterval sqrt(const LongInterval &x);		// 平方根
  };

////////////////////////////////////////////////////////////////////////////////

  // 多倍長区間クラス
  class LongInterval{
    private:
    public:
      LongFloat center;						// 中心値
      Radius radius;						// 半径

//      void copy(const LongInterval &x, int size);		// limb 数の異なる LongInterval を代入する

//      void readString(std::string s);				// 文字列を読み込む

//      LongFloat infIn() const;					// 下端の取得(内側への丸め)
//      LongFloat supIn() const;					// 上端の取得(内側への丸め)

//    public:
      LongInterval();						// コンストラクタ
      LongInterval(int x);					// コンストラクタ
      LongInterval(const LongFloat &x);				// コンストラクタ
//      LongInterval(std::string s);				// コンストラクタ
      LongInterval(const LongInterval &x);			// コピーコンストラクタ
      LongInterval(int mid, int rad);				// コンストラクタ
//      LongInterval(const LongFloat &mid, int rad);		// コンストラクタ
//      LongInterval(int mid, const LongFloat &rad);		// コンストラクタ
      LongInterval(const LongFloat &mid, const Radius &rad);	// コンストラクタ
//      LongInterval(const LongFloat &mid, const LongFloat &rad);	// コンストラクタ

//      void setDouble(double x);					// double からの変換
//      void setString(std::string s);				// 文字列からの変換

//      void setMidRad(int mid, int rad);				// 中心値と半径の設定
//      void setMidRad(const LongFloat &mid, int rad);		// 中心値と半径の設定
//      void setMidRad(int mid, const LongFloat &rad);		// 中心値と半径の設定
//      void setMidRad(const LongFloat &mid, const LongFloat &rad);	// 中心値と半径の設定

//      void setInfSup(int inf, int sup);				// 下端と上端の設定
//      void setInfSup(const LongFloat &inf, int sup);		// 下端と上端の設定
//      void setInfSup(int inf, const LongFloat &sup);		// 下端と上端の設定
//      void setInfSup(const LongFloat &inf, const LongFloat &sup);	// 下端と上端の設定

//      double getDouble() const;					// double への変換
      std::string getMidRad() const;				// 中心値と半径を表す文字列の取得
      std::string getInfSup() const;				// 下端と上端を表す文字列の取得
      std::string getInternalData() const;			// 内部データを表す文字列の取得

//      void random(int exponentMin, int exponentMax, int error);	// 値を乱数にする(テスト用)

      LongFloat mid() const;					// 中心値の取得
      LongFloat rad() const;					// 半径の取得
      LongFloat diam() const;					// 直径の取得
      LongFloat inf() const;					// 下端の取得
      LongFloat sup() const;					// 上端の取得
      LongFloat mig() const;					// 最小絶対値の取得
      LongFloat mag() const;					// 最大絶対値の取得

//      int contains(int x) const;					// x を含むか
//      int containsEqual(int x) const;				// x を含むか(境界を含む)
//      int contains(const LongFloat &x) const;			// x を含むか
//      int containsEqual(const LongFloat &x) const;		// x を含むか(境界を含む)
//      int contains(const LongInterval &x) const;			// x を含むか
//      int containsEqual(const LongInterval &x) const;		// x を含むか(境界を含む)

      LongInterval inverse() const;				// 逆数区間の取得

      LongInterval operator +() const;				// +LongInterval
      LongInterval operator -() const;				// -LongInterval

      LongInterval operator +(int x) const;			// LongInterval + int
      LongInterval operator -(int x) const;			// LongInterval - int
      LongInterval operator *(int x) const;			// LongInterval * int
      LongInterval operator /(int x) const;			// LongInterval / int

      LongInterval operator +(const LongFloat &x) const;	// LongInterval + LongFloat
      LongInterval operator -(const LongFloat &x) const;	// LongInterval - LongFloat
      LongInterval operator *(const LongFloat &x) const;	// LongInterval * LongFloat
      LongInterval operator /(const LongFloat &x) const;	// LongInterval / LongFloat

      LongInterval operator +(const LongInterval &x) const;	// LongInterval + LongInterval
      LongInterval operator -(const LongInterval &x) const;	// LongInterval - LongInterval
      LongInterval operator *(const LongInterval &x) const;	// LongInterval * LongInterval
      LongInterval operator /(const LongInterval &x) const;	// LongInterval / LongInterval

//      int operator <(int x) const;				// LongInterval < int
//      int operator >(int x) const;				// LongInterval > int
//      int operator <=(int x) const;				// LongInterval <= int
//      int operator >=(int x) const;				// LongInterval >= int

//      int operator <(const LongFloat &x) const;			// LongInterval < LongFloat
//      int operator >(const LongFloat &x) const;			// LongInterval > LongFloat
//      int operator <=(const LongFloat &x) const;			// LongInterval <= LongFloat
//      int operator >=(const LongFloat &x) const;			// LongInterval >= LongFloat

//      bool operator ==(const LongInterval &x) const;		// LongInterval == LongInterval
//      bool operator !=(const LongInterval &x) const;		// LongInterval != LongInterval

//      int operator <(const LongInterval &x) const;		// LongInterval < LongInterval
//      int operator >(const LongInterval &x) const;		// LongInterval > LongInterval
//      int operator <=(const LongInterval &x) const;		// LongInterval <= LongInterval
//      int operator >=(const LongInterval &x) const;		// LongInterval >= LongInterval

      LongInterval &operator =(int x);				// LongInterval = int
      LongInterval &operator +=(int x);				// LongInterval += int
      LongInterval &operator -=(int x);				// LongInterval -= int
      LongInterval &operator *=(int x);				// LongInterval *= int
      LongInterval &operator /=(int x);				// LongInterval /= int

      LongInterval &operator =(const LongFloat &x);		// LongInterval = LongFloat
      LongInterval &operator +=(const LongFloat &x);		// LongInterval += LongFloat
      LongInterval &operator -=(const LongFloat &x);		// LongInterval -= LongFloat
      LongInterval &operator *=(const LongFloat &x);		// LongInterval *= LongFloat
      LongInterval &operator /=(const LongFloat &x);		// LongInterval /= LongFloat

      LongInterval &operator =(const LongInterval &x);		// LongInterval = LongInterval
      LongInterval &operator +=(const LongInterval &x);		// LongInterval += LongInterval
      LongInterval &operator -=(const LongInterval &x);		// LongInterval -= LongInterval
      LongInterval &operator *=(const LongInterval &x);		// LongInterval *= LongInterval
      LongInterval &operator /=(const LongInterval &x);		// LongInterval /= LongInterval

    friend LongInterval (::operator +)(int a, const LongInterval &b);	// int + LongInterval
    friend LongInterval (::operator -)(int a, const LongInterval &b);	// int - LongInterval
    friend LongInterval (::operator *)(int a, const LongInterval &b);	// int * LongInterval
    friend LongInterval (::operator /)(int a, const LongInterval &b);	// int / LongInterval

//    friend int operator <(int a, const LongInterval &b);		// int < LongInterval
//    friend int operator >(int a, const LongInterval &b);		// int > LongInterval
//    friend int operator <=(int a, const LongInterval &b);		// int <= LongInterval
//    friend int operator >=(int a, const LongInterval &b);		// int >= LongInterval

//    friend LongInterval pow(const LongInterval &x, int n);	// べき乗
//    friend LongInterval sqrt(const LongInterval &x);		// 平方根

    friend class LongFloat;					// 多倍長実数クラス
  };
}

////////////////////////////////////////////////////////////////////////////////

// std::ostream << Radius
std::ostream &operator <<(std::ostream &output, const lilib::Radius &x);

// std::ostream << LongFloat
std::ostream &operator <<(std::ostream &output, const lilib::LongFloat &x);

// std::ostream << LongInterval
std::ostream &operator <<(std::ostream &output, const lilib::LongInterval &x);
