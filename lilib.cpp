// ライブラリ全体で共用する定数・変数・関数

#include <cstdlib>
#include "lilib.h"

using namespace std;
using namespace lilib;

const int lilib::RADIUS_BITS = 64;	// Radius のビット数
const int lilib::BUFFER_SIZE = 100;	// 文字列表示用のバッファサイズ
int lilib::longFloatBits;		// LongFloat のビット数、未設定状態は 0

gmp_randstate_t lilib::randomState;	// 乱数生成器

// 多倍長数の精度の設定
void lilib::setPrecision(int precision){

  // この関数は一度しか呼べない
  if(longFloatBits){
    cerr << "[ERROR] lilib::setPrecision(int precision) : This function must be called only once." << endl;
    exit(1);
  }

  if(precision < RADIUS_BITS){
    cerr << "[ERROR] lilib::setPrecision(int precision) : precision must be greater than or equal to " << RADIUS_BITS << "." << endl;
    exit(1);
  }

  longFloatBits = precision;
}

// 多倍長数の精度の取得
int lilib::getPrecision(){
  return longFloatBits;
}

// 乱数の初期化
void lilib::initRandom(unsigned long int seed){
  gmp_randinit_default(randomState);
  gmp_randseed_ui(randomState, seed);
}
