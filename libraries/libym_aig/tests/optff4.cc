
/// @file libym_aig/tests/optff.cc
/// @brief 最適ファクタードフォームを求めるプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: optff4.cc 1594 2008-07-18 10:26:12Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/LogExpr.h"
#include "ym_npn/TvFunc.h"
#include "ym_npn/NpnMgr.h"
#include "ym_npn/NpnMap.h"

#define VERIFY_FF3TABLE 0


BEGIN_NAMESPACE_YM

// 関数ベクタとそれに対応するファクタードフォームを記録するためのクラス
struct FuncRec
{
  // 関数の真理値表ベクタ
  ymuint32 mFuncVect;

  // ファクタードフォーム
  LogExpr mExpr;
  
  // リテラル数
  int mLitNum;
  
};

// 3入力関数に対する最適ファクタードフォームの表
LogExpr ff3table[256];

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
  0x0000U,
  0xC282U,
  0xE880U,
  0x62C8U,
  0xE881U,
  0xE882U,
  0xE883U,
  0xE886U,
  0x8EE0U,
  0x6A68U,
  0xE888U,
  0xE889U,
  0x6A6AU,
  0x56AAU,
  0xE88AU,
  0x8EE8U,
  0xC8A8U,
  0xE898U,
  0x2AA8U,
  0x782EU,
  0x4A98U,
  0x9818U,
  0xC2A4U,
  0x9696U,
  0xC2A8U,
  0xE8A8U,
  0xE41AU,
  0x9088U,
  0xE8AAU,
  0x4AA4U,
  0x4AA8U,
  0x2AC0U,
  0xAA64U,
  0x1888U,
  0xAA68U,
  0x96A8U,
  0x96AAU,
  0xBCA8U,
  0xE2ACU,
  0x6AA8U,
  0x6AA9U,
  0x6AAAU,
  0x0660U,
  0x6008U,
  0x4AC2U,
  0x6AACU,
  0xCA68U,
  0xB82CU,
  0x724EU,
  0x8180U,
  0x8181U,
  0x8000U,
  0x8001U,
  0x8002U,
  0x2968U,
  0x8AA8U,
  0xE2CAU,
  0x6AC2U,
  0xAA94U,
  0xEA68U,
  0x588AU,
  0x6AC8U,
  0xE8E8U,
  0x6ACAU,
  0x9868U,
  0xAAA8U,
  0xF08EU,
  0x96E8U,
  0x8AC0U,
  0xAAAAU,
  0x7886U,
  0x7888U,
  0x7889U,
  0x6C62U,
  0xCA98U,
  0x1EE8U,
  0x6C6AU,
  0x6AE8U,
  0x665AU,
  0xCAA4U,
  0xA4A8U,
  0x6968U,
  0xCAA8U,
  0xAAC0U,
  0x696AU,
  0xCAAAU,
  0xAAC1U,
  0xAAC2U,
  0x9886U,
  0xCAACU,
  0xC00AU,
  0x9888U,
  0x988AU,
  0x6666U,
  0xAAC8U,
  0x6668U,
  0xC4A2U,
  0x8960U,
  0x9898U,
  0x2828U,
  0xEAA8U,
  0xE006U,
  0x8660U,
  0xCAC2U,
  0xE008U,
  0xE009U,
  0xAC62U,
  0x8668U,
  0xCACAU,
  0xE4A8U,
  0xE4AAU,
  0x4828U,
  0xA968U,
  0xD88AU,
  0x6818U,
  0xEAC8U,
  0x43A8U,
  0x6088U,
  0x69A8U,
  0xCAE8U,
  0x4228U,
  0x6828U,
  0xC96AU,
  0x8998U,
  0x682AU,
  0xF888U,
  0xEC62U,
  0x8080U,
  0xC66AU,
  0x0880U,
  0xE968U,
  0x1AC8U,
  0xE668U,
  0xD2ACU,
  0xA088U,
  0xA9A8U,
  0x2880U,
  0xA824U,
  0x8228U,
  0x6CE2U,
  0xA828U,
  0x5AACU,
  0x2888U,
  0x6248U,
  0x624AU,
  0x69E8U,
  0x83C2U,
  0x6868U,
  0x4882U,
  0xA9C2U,
  0x1968U,
  0x66E8U,
  0xC828U,
  0xC82AU,
  0xA842U,
  0x17E8U,
  0x89E0U,
  0xE816U,
  0x8E70U,
  0xE818U,
  0xE819U,
  0x700EU,
  0x1668U,
  0x5ACAU,
  0x3C6AU,
  0x86E0U,
  0xC3A8U,
  0xE088U,
  0xE08AU,
  0x4282U,
  0x6880U,
  0x6881U,
  0xC228U,
  0xE826U,
  0x6882U,
  0xC229U,
  0xC22AU,
  0xE828U,
  0x9006U,
  0xE829U,
  0xE82AU,
  0x9008U,
  0x0EE0U,
  0x6888U,
  0x688AU,
  0xA248U,
  0x4EB8U,
  0x8880U,
  0xA6E8U,
  0x8888U,
  0x9AC8U,
  0x2EE8U,
  0xE84AU,
  0x68A8U,
  0x8280U,
  0xC6E2U,
  0xA880U,
  0xA881U,
  0x2BE8U,
  0xA882U,
  0xA888U,
  0xE246U,
  0x16A8U,
  0x4EE2U,
  0xE248U,
  0xE249U,
  0x3CA8U,
  0xE24AU,
  0x3CAAU,
  0x9C6AU,
  0xE868U,
  0xC882U,
  0x9968U,
  0x4A68U,
  0x6EE0U,
  0xC88AU,
  0x9666U,
  0xF00EU,
  0x9668U,
  0x9669U,
  0x966AU,
  0xA8A8U,
  0xFFFFU // end-marker
};


// ff3table を作る．
void
init_table()
{
  LogExpr V0 = LogExpr::make_posiliteral(0);
  LogExpr V1 = LogExpr::make_posiliteral(1);
  LogExpr V2 = LogExpr::make_posiliteral(2);

  ff3table[  0] = LogExpr::make_zero();
  ff3table[  1] = ( ~V0 & ~V1 & ~V2 );
  ff3table[  2] = ( V0 & ~V1 & ~V2 );
  ff3table[  3] = ( ~V1 & ~V2 );
  ff3table[  4] = ( ~V0 & V1 & ~V2 );
  ff3table[  5] = ( ~V0 & ~V2 );
  ff3table[  6] = ( ~V2 & ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) );
  ff3table[  7] = ( ~V2 & ( ~V0 | ~V1 ) );
  ff3table[  8] = ( V0 & V1 & ~V2 );
  ff3table[  9] = ( ~V2 & ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) );
  ff3table[ 10] = ( V0 & ~V2 );
  ff3table[ 11] = ( ~V2 & ( V0 | ~V1 ) );
  ff3table[ 12] = ( V1 & ~V2 );
  ff3table[ 13] = ( ~V2 & ( ~V0 | V1 ) );
  ff3table[ 14] = ( ~V2 & ( V0 | V1 ) );
  ff3table[ 15] = ~V2;
  ff3table[ 16] = ( ~V0 & ~V1 & V2 );
  ff3table[ 17] = ( ~V0 & ~V1 );
  ff3table[ 18] = ( ~V1 & ( ( V0 & ~V2 ) | ( ~V0 & V2 ) ) );
  ff3table[ 19] = ( ~V1 & ( ~V0 | ~V2 ) );
  ff3table[ 20] = ( ~V0 & ( ( V1 & ~V2 ) | ( ~V1 & V2 ) ) );
  ff3table[ 21] = ( ~V0 & ( ~V1 | ~V2 ) );
  ff3table[ 22] = ( ( ~V0 & ~V1 & V2 ) | ( ( ( ~V0 & V1 ) | ( V0 & ~V1 ) ) & ~V2 ) );
  ff3table[ 23] = ( ( ~V0 & ~V1 ) | ( ( ~V0 | ~V1 ) & ~V2 ) );
  ff3table[ 24] = ( ( ~V0 & ~V1 & V2 ) | ( V0 & V1 & ~V2 ) );
  ff3table[ 25] = ( ( ~V0 & ~V1 ) | ( V0 & V1 & ~V2 ) );
  ff3table[ 26] = ( ( V0 & ~V2 ) | ( ~V0 & V2 & ~V1 ) );
  ff3table[ 27] = ( ( ~V1 & ~V0 ) | ( ~V2 & V0 ) );
  ff3table[ 28] = ( ( V1 & ~V2 ) | ( ~V1 & V2 & ~V0 ) );
  ff3table[ 29] = ( ( ~V0 & ~V1 ) | ( ~V2 & V1 ) );
  ff3table[ 30] = ( ( ~V2 & V1 ) | ( ( ( ~V2 & V0 ) | ( V2 & ~V0 ) ) & ~V1 ) );
  ff3table[ 31] = ( ~V2 | ( ~V0 & ~V1 ) );
  ff3table[ 32] = ( V0 & ~V1 & V2 );
  ff3table[ 33] = ( ~V1 & ( ( ~V0 & ~V2 ) | ( V0 & V2 ) ) );
  ff3table[ 34] = ( V0 & ~V1 );
  ff3table[ 35] = ( ~V1 & ( V0 | ~V2 ) );
  ff3table[ 36] = ( ( ~V0 & V1 & ~V2 ) | ( V0 & ~V1 & V2 ) );
  ff3table[ 37] = ( ( ~V0 & ~V2 ) | ( V0 & V2 & ~V1 ) );
  ff3table[ 38] = ( ( V0 & ~V1 ) | ( ~V0 & V1 & ~V2 ) );
  ff3table[ 39] = ( ( ~V1 & V0 ) | ( ~V2 & ~V0 ) );
  ff3table[ 40] = ( V0 & ( ( V1 & ~V2 ) | ( ~V1 & V2 ) ) );
  ff3table[ 41] = ( ( V0 & ~V1 & V2 ) | ( ( ( V0 & V1 ) | ( ~V0 & ~V1 ) ) & ~V2 ) );
  ff3table[ 42] = ( V0 & ( ~V1 | ~V2 ) );
  ff3table[ 43] = ( ( V0 & ~V1 ) | ( ( V0 | ~V1 ) & ~V2 ) );
  ff3table[ 44] = ( ( V1 & ~V2 ) | ( ~V1 & V2 & V0 ) );
  ff3table[ 45] = ( ( ~V2 & V1 ) | ( ( ( ~V2 & ~V0 ) | ( V2 & V0 ) ) & ~V1 ) );
  ff3table[ 46] = ( ( V0 & ~V1 ) | ( ~V2 & V1 ) );
  ff3table[ 47] = ( ~V2 | ( V0 & ~V1 ) );
  ff3table[ 48] = ( ~V1 & V2 );
  ff3table[ 49] = ( ~V1 & ( ~V0 | V2 ) );
  ff3table[ 50] = ( ~V1 & ( V0 | V2 ) );
  ff3table[ 51] = ~V1;
  ff3table[ 52] = ( ( ~V1 & V2 ) | ( V1 & ~V2 & ~V0 ) );
  ff3table[ 53] = ( ( ~V0 & ~V2 ) | ( ~V1 & V2 ) );
  ff3table[ 54] = ( ( ~V1 & V2 ) | ( ( ( ~V1 & V0 ) | ( V1 & ~V0 ) ) & ~V2 ) );
  ff3table[ 55] = ( ~V1 | ( ~V0 & ~V2 ) );
  ff3table[ 56] = ( ( ~V1 & V2 ) | ( V1 & ~V2 & V0 ) );
  ff3table[ 57] = ( ( ~V1 & V2 ) | ( ( ( ~V1 & ~V0 ) | ( V1 & V0 ) ) & ~V2 ) );
  ff3table[ 58] = ( ( V0 & ~V2 ) | ( ~V1 & V2 ) );
  ff3table[ 59] = ( ~V1 | ( V0 & ~V2 ) );
  ff3table[ 60] = ( ( V1 & ~V2 ) | ( ~V1 & V2 ) );
  ff3table[ 61] = ( ( ~V1 | ~V2 ) & ( V1 | V2 | ~V0 ) );
  ff3table[ 62] = ( ( ~V1 | ~V2 ) & ( V1 | V2 | V0 ) );
  ff3table[ 63] = ( ~V1 | ~V2 );
  ff3table[ 64] = ( ~V0 & V1 & V2 );
  ff3table[ 65] = ( ~V0 & ( ( ~V1 & ~V2 ) | ( V1 & V2 ) ) );
  ff3table[ 66] = ( ( ~V0 & V1 & V2 ) | ( V0 & ~V1 & ~V2 ) );
  ff3table[ 67] = ( ( ~V1 & ~V2 ) | ( V1 & V2 & ~V0 ) );
  ff3table[ 68] = ( ~V0 & V1 );
  ff3table[ 69] = ( ~V0 & ( V1 | ~V2 ) );
  ff3table[ 70] = ( ( ~V0 & V1 ) | ( V0 & ~V1 & ~V2 ) );
  ff3table[ 71] = ( ( ~V0 & V1 ) | ( ~V2 & ~V1 ) );
  ff3table[ 72] = ( V1 & ( ( V0 & ~V2 ) | ( ~V0 & V2 ) ) );
  ff3table[ 73] = ( ( ~V0 & V1 & V2 ) | ( ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) & ~V2 ) );
  ff3table[ 74] = ( ( V0 & ~V2 ) | ( ~V0 & V2 & V1 ) );
  ff3table[ 75] = ( ( ~V2 & ~V1 ) | ( ( ( ~V2 & V0 ) | ( V2 & ~V0 ) ) & V1 ) );
  ff3table[ 76] = ( V1 & ( ~V0 | ~V2 ) );
  ff3table[ 77] = ( ( ~V0 & V1 ) | ( ( ~V0 | V1 ) & ~V2 ) );
  ff3table[ 78] = ( ( V1 & ~V0 ) | ( ~V2 & V0 ) );
  ff3table[ 79] = ( ~V2 | ( ~V0 & V1 ) );
  ff3table[ 80] = ( ~V0 & V2 );
  ff3table[ 81] = ( ~V0 & ( ~V1 | V2 ) );
  ff3table[ 82] = ( ( ~V0 & V2 ) | ( V0 & ~V2 & ~V1 ) );
  ff3table[ 83] = ( ( ~V0 & V2 ) | ( ~V1 & ~V2 ) );
  ff3table[ 84] = ( ~V0 & ( V1 | V2 ) );
  ff3table[ 85] = ~V0;
  ff3table[ 86] = ( ( ~V0 & V2 ) | ( ( ( ~V0 & V1 ) | ( V0 & ~V1 ) ) & ~V2 ) );
  ff3table[ 87] = ( ~V0 | ( ~V1 & ~V2 ) );
  ff3table[ 88] = ( ( ~V0 & V2 ) | ( V0 & ~V2 & V1 ) );
  ff3table[ 89] = ( ( ~V0 & V2 ) | ( ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) & ~V2 ) );
  ff3table[ 90] = ( ( V0 & ~V2 ) | ( ~V0 & V2 ) );
  ff3table[ 91] = ( ( ~V0 | ~V2 ) & ( V0 | V2 | ~V1 ) );
  ff3table[ 92] = ( ( ~V0 & V2 ) | ( V1 & ~V2 ) );
  ff3table[ 93] = ( ~V0 | ( V1 & ~V2 ) );
  ff3table[ 94] = ( ( ~V0 | ~V2 ) & ( V0 | V2 | V1 ) );
  ff3table[ 95] = ( ~V0 | ~V2 );
  ff3table[ 96] = ( V2 & ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) );
  ff3table[ 97] = ( ( ~V0 & ~V1 & ~V2 ) | ( ( ( ~V0 & V1 ) | ( V0 & ~V1 ) ) & V2 ) );
  ff3table[ 98] = ( ( V0 & ~V1 ) | ( ~V0 & V1 & V2 ) );
  ff3table[ 99] = ( ( ~V1 & ~V2 ) | ( ( ( ~V1 & V0 ) | ( V1 & ~V0 ) ) & V2 ) );
  ff3table[100] = ( ( ~V0 & V1 ) | ( V0 & ~V1 & V2 ) );
  ff3table[101] = ( ( ~V0 & ~V2 ) | ( ( ( ~V0 & V1 ) | ( V0 & ~V1 ) ) & V2 ) );
  ff3table[102] = ( ( V0 & ~V1 ) | ( ~V0 & V1 ) );
  ff3table[103] = ( ( ~V0 | ~V1 ) & ( V0 | V1 | ~V2 ) );
  ff3table[104] = ( ( V0 & V1 & ~V2 ) | ( ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) & V2 ) );
  ff3table[105] = ( ( ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) & ~V2 ) | ( ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) & V2 ) );
  ff3table[106] = ( ( V0 & ~V2 ) | ( ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) & V2 ) );
  ff3table[107] = ( ( V0 | ~V1 | V2 ) & ( ( ( V0 | V1 ) & ( ~V0 | ~V1 ) ) | ~V2 ) );
  ff3table[108] = ( ( V1 & ~V2 ) | ( ( ( V1 & ~V0 ) | ( ~V1 & V0 ) ) & V2 ) );
  ff3table[109] = ( ( ~V0 | V1 | V2 ) & ( ( ( ~V0 | ~V1 ) & ( V0 | V1 ) ) | ~V2 ) );
  ff3table[110] = ( ( V0 | V1 ) & ( ~V0 | ~V1 | ~V2 ) );
  ff3table[111] = ( ~V2 | ( ( V0 | V1 ) & ( ~V0 | ~V1 ) ) );
  ff3table[112] = ( V2 & ( ~V0 | ~V1 ) );
  ff3table[113] = ( ( ~V0 & ~V1 ) | ( ( ~V0 | ~V1 ) & V2 ) );
  ff3table[114] = ( ( ~V1 & V0 ) | ( V2 & ~V0 ) );
  ff3table[115] = ( ~V1 | ( ~V0 & V2 ) );
  ff3table[116] = ( ( ~V0 & V1 ) | ( V2 & ~V1 ) );
  ff3table[117] = ( ~V0 | ( ~V1 & V2 ) );
  ff3table[118] = ( ( ~V0 | ~V1 ) & ( V0 | V1 | V2 ) );
  ff3table[119] = ( ~V0 | ~V1 );
  ff3table[120] = ( ( V2 & ~V1 ) | ( ( ( V2 & ~V0 ) | ( ~V2 & V0 ) ) & V1 ) );
  ff3table[121] = ( ( ~V0 | ~V1 | ~V2 ) & ( ( ( ~V0 | V1 ) & ( V0 | ~V1 ) ) | V2 ) );
  ff3table[122] = ( ( V0 | V2 ) & ( ~V0 | ~V2 | ~V1 ) );
  ff3table[123] = ( ~V1 | ( ( V0 | V2 ) & ( ~V0 | ~V2 ) ) );
  ff3table[124] = ( ( V1 | V2 ) & ( ~V1 | ~V2 | ~V0 ) );
  ff3table[125] = ( ~V0 | ( ( V1 | V2 ) & ( ~V1 | ~V2 ) ) );
  ff3table[126] = ( ( V0 | V1 | V2 ) & ( ~V0 | ~V1 | ~V2 ) );
  ff3table[127] = ( ~V0 | ~V1 | ~V2 );
  ff3table[128] = ( V0 & V1 & V2 );
  ff3table[129] = ( ( ~V0 & ~V1 & ~V2 ) | ( V0 & V1 & V2 ) );
  ff3table[130] = ( V0 & ( ( ~V1 & ~V2 ) | ( V1 & V2 ) ) );
  ff3table[131] = ( ( ~V1 & ~V2 ) | ( V1 & V2 & V0 ) );
  ff3table[132] = ( V1 & ( ( ~V0 & ~V2 ) | ( V0 & V2 ) ) );
  ff3table[133] = ( ( ~V0 & ~V2 ) | ( V0 & V2 & V1 ) );
  ff3table[134] = ( ( V0 & V1 & V2 ) | ( ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) & ~V2 ) );
  ff3table[135] = ( ( ~V2 & ~V1 ) | ( ( ( ~V2 & ~V0 ) | ( V2 & V0 ) ) & V1 ) );
  ff3table[136] = ( V0 & V1 );
  ff3table[137] = ( ( V0 & V1 ) | ( ~V0 & ~V1 & ~V2 ) );
  ff3table[138] = ( V0 & ( V1 | ~V2 ) );
  ff3table[139] = ( ( V0 & V1 ) | ( ~V2 & ~V1 ) );
  ff3table[140] = ( V1 & ( V0 | ~V2 ) );
  ff3table[141] = ( ( V1 & V0 ) | ( ~V2 & ~V0 ) );
  ff3table[142] = ( ( V0 & V1 ) | ( ( V0 | V1 ) & ~V2 ) );
  ff3table[143] = ( ~V2 | ( V0 & V1 ) );
  ff3table[144] = ( V2 & ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) );
  ff3table[145] = ( ( ~V0 & ~V1 ) | ( V0 & V1 & V2 ) );
  ff3table[146] = ( ( V0 & ~V1 & ~V2 ) | ( ( ( V0 & V1 ) | ( ~V0 & ~V1 ) ) & V2 ) );
  ff3table[147] = ( ( ~V1 & ~V2 ) | ( ( ( ~V1 & ~V0 ) | ( V1 & V0 ) ) & V2 ) );
  ff3table[148] = ( ( ~V0 & V1 & ~V2 ) | ( ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) & V2 ) );
  ff3table[149] = ( ( ~V0 & ~V2 ) | ( ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) & V2 ) );
  ff3table[150] = ( ( ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) & ~V2 ) | ( ( ( ~V0 & ~V1 ) | ( V0 & V1 ) ) & V2 ) );
  ff3table[151] = ( ( ~V0 | ~V1 | V2 ) & ( ( ( ~V0 | V1 ) & ( V0 | ~V1 ) ) | ~V2 ) );
  ff3table[152] = ( ( V0 & V1 ) | ( ~V0 & ~V1 & V2 ) );
  ff3table[153] = ( ( ~V0 & ~V1 ) | ( V0 & V1 ) );
  ff3table[154] = ( ( V0 & ~V2 ) | ( ( ( V0 & V1 ) | ( ~V0 & ~V1 ) ) & V2 ) );
  ff3table[155] = ( ( V0 | ~V1 ) & ( ~V0 | V1 | ~V2 ) );
  ff3table[156] = ( ( V1 & ~V2 ) | ( ( ( V1 & V0 ) | ( ~V1 & ~V0 ) ) & V2 ) );
  ff3table[157] = ( ( ~V0 | V1 ) & ( V0 | ~V1 | ~V2 ) );
  ff3table[158] = ( ( V0 | V1 | V2 ) & ( ( ( V0 | ~V1 ) & ( ~V0 | V1 ) ) | ~V2 ) );
  ff3table[159] = ( ~V2 | ( ( ~V0 | V1 ) & ( V0 | ~V1 ) ) );
  ff3table[160] = ( V0 & V2 );
  ff3table[161] = ( ( V0 & V2 ) | ( ~V0 & ~V2 & ~V1 ) );
  ff3table[162] = ( V0 & ( ~V1 | V2 ) );
  ff3table[163] = ( ( V0 & V2 ) | ( ~V1 & ~V2 ) );
  ff3table[164] = ( ( V0 & V2 ) | ( ~V0 & ~V2 & V1 ) );
  ff3table[165] = ( ( ~V0 & ~V2 ) | ( V0 & V2 ) );
  ff3table[166] = ( ( V0 & V2 ) | ( ( ( V0 & ~V1 ) | ( ~V0 & V1 ) ) & ~V2 ) );
  ff3table[167] = ( ( V0 | ~V2 ) & ( ~V0 | V2 | ~V1 ) );
  ff3table[168] = ( V0 & ( V1 | V2 ) );
  ff3table[169] = ( ( V0 & V2 ) | ( ( ( V0 & V1 ) | ( ~V0 & ~V1 ) ) & ~V2 ) );
  ff3table[170] = V0;
  ff3table[171] = ( V0 | ( ~V1 & ~V2 ) );
  ff3table[172] = ( ( V0 & V2 ) | ( V1 & ~V2 ) );
  ff3table[173] = ( ( V0 | ~V2 ) & ( ~V0 | V2 | V1 ) );
  ff3table[174] = ( V0 | ( V1 & ~V2 ) );
  ff3table[175] = ( V0 | ~V2 );
  ff3table[176] = ( V2 & ( V0 | ~V1 ) );
  ff3table[177] = ( ( ~V1 & ~V0 ) | ( V2 & V0 ) );
  ff3table[178] = ( ( V0 & ~V1 ) | ( ( V0 | ~V1 ) & V2 ) );
  ff3table[179] = ( ~V1 | ( V0 & V2 ) );
  ff3table[180] = ( ( V2 & ~V1 ) | ( ( ( V2 & V0 ) | ( ~V2 & ~V0 ) ) & V1 ) );
  ff3table[181] = ( ( ~V0 | V2 ) & ( V0 | ~V2 | ~V1 ) );
  ff3table[182] = ( ( V0 | ~V1 | ~V2 ) & ( ( ( V0 | V1 ) & ( ~V0 | ~V1 ) ) | V2 ) );
  ff3table[183] = ( ~V1 | ( ( ~V0 | V2 ) & ( V0 | ~V2 ) ) );
  ff3table[184] = ( ( V0 & V1 ) | ( V2 & ~V1 ) );
  ff3table[185] = ( ( V0 | ~V1 ) & ( ~V0 | V1 | V2 ) );
  ff3table[186] = ( V0 | ( ~V1 & V2 ) );
  ff3table[187] = ( V0 | ~V1 );
  ff3table[188] = ( ( V1 | V2 ) & ( ~V1 | ~V2 | V0 ) );
  ff3table[189] = ( ( V0 | ~V1 | ~V2 ) & ( ~V0 | V1 | V2 ) );
  ff3table[190] = ( V0 | ( ( V1 | V2 ) & ( ~V1 | ~V2 ) ) );
  ff3table[191] = ( V0 | ~V1 | ~V2 );
  ff3table[192] = ( V1 & V2 );
  ff3table[193] = ( ( V1 & V2 ) | ( ~V1 & ~V2 & ~V0 ) );
  ff3table[194] = ( ( V1 & V2 ) | ( ~V1 & ~V2 & V0 ) );
  ff3table[195] = ( ( ~V1 & ~V2 ) | ( V1 & V2 ) );
  ff3table[196] = ( V1 & ( ~V0 | V2 ) );
  ff3table[197] = ( ( ~V0 & ~V2 ) | ( V1 & V2 ) );
  ff3table[198] = ( ( V1 & V2 ) | ( ( ( V1 & ~V0 ) | ( ~V1 & V0 ) ) & ~V2 ) );
  ff3table[199] = ( ( V1 | ~V2 ) & ( ~V1 | V2 | ~V0 ) );
  ff3table[200] = ( V1 & ( V0 | V2 ) );
  ff3table[201] = ( ( V1 & V2 ) | ( ( ( V1 & V0 ) | ( ~V1 & ~V0 ) ) & ~V2 ) );
  ff3table[202] = ( ( V0 & ~V2 ) | ( V1 & V2 ) );
  ff3table[203] = ( ( V1 | ~V2 ) & ( ~V1 | V2 | V0 ) );
  ff3table[204] = V1;
  ff3table[205] = ( V1 | ( ~V0 & ~V2 ) );
  ff3table[206] = ( V1 | ( V0 & ~V2 ) );
  ff3table[207] = ( V1 | ~V2 );
  ff3table[208] = ( V2 & ( ~V0 | V1 ) );
  ff3table[209] = ( ( ~V0 & ~V1 ) | ( V2 & V1 ) );
  ff3table[210] = ( ( V2 & V1 ) | ( ( ( V2 & ~V0 ) | ( ~V2 & V0 ) ) & ~V1 ) );
  ff3table[211] = ( ( ~V1 | V2 ) & ( V1 | ~V2 | ~V0 ) );
  ff3table[212] = ( ( ~V0 & V1 ) | ( ( ~V0 | V1 ) & V2 ) );
  ff3table[213] = ( ~V0 | ( V1 & V2 ) );
  ff3table[214] = ( ( ~V0 | V1 | ~V2 ) & ( ( ( ~V0 | ~V1 ) & ( V0 | V1 ) ) | V2 ) );
  ff3table[215] = ( ~V0 | ( ( ~V1 | V2 ) & ( V1 | ~V2 ) ) );
  ff3table[216] = ( ( V1 & V0 ) | ( V2 & ~V0 ) );
  ff3table[217] = ( ( ~V0 | V1 ) & ( V0 | ~V1 | V2 ) );
  ff3table[218] = ( ( V0 | V2 ) & ( ~V0 | ~V2 | V1 ) );
  ff3table[219] = ( ( V0 | ~V1 | V2 ) & ( ~V0 | V1 | ~V2 ) );
  ff3table[220] = ( V1 | ( ~V0 & V2 ) );
  ff3table[221] = ( ~V0 | V1 );
  ff3table[222] = ( V1 | ( ( V0 | V2 ) & ( ~V0 | ~V2 ) ) );
  ff3table[223] = ( ~V0 | V1 | ~V2 );
  ff3table[224] = ( V2 & ( V0 | V1 ) );
  ff3table[225] = ( ( V2 & V1 ) | ( ( ( V2 & V0 ) | ( ~V2 & ~V0 ) ) & ~V1 ) );
  ff3table[226] = ( ( V0 & ~V1 ) | ( V2 & V1 ) );
  ff3table[227] = ( ( ~V1 | V2 ) & ( V1 | ~V2 | V0 ) );
  ff3table[228] = ( ( V1 & ~V0 ) | ( V2 & V0 ) );
  ff3table[229] = ( ( ~V0 | V2 ) & ( V0 | ~V2 | V1 ) );
  ff3table[230] = ( ( V0 | V1 ) & ( ~V0 | ~V1 | V2 ) );
  ff3table[231] = ( ( V0 | V1 | ~V2 ) & ( ~V0 | ~V1 | V2 ) );
  ff3table[232] = ( ( V0 & V1 ) | ( ( V0 | V1 ) & V2 ) );
  ff3table[233] = ( ( V0 | V1 | ~V2 ) & ( ( ( V0 | ~V1 ) & ( ~V0 | V1 ) ) | V2 ) );
  ff3table[234] = ( V0 | ( V1 & V2 ) );
  ff3table[235] = ( V0 | ( ( ~V1 | V2 ) & ( V1 | ~V2 ) ) );
  ff3table[236] = ( V1 | ( V0 & V2 ) );
  ff3table[237] = ( V1 | ( ( ~V0 | V2 ) & ( V0 | ~V2 ) ) );
  ff3table[238] = ( V0 | V1 );
  ff3table[239] = ( V0 | V1 | ~V2 );
  ff3table[240] = V2;
  ff3table[241] = ( V2 | ( ~V0 & ~V1 ) );
  ff3table[242] = ( V2 | ( V0 & ~V1 ) );
  ff3table[243] = ( ~V1 | V2 );
  ff3table[244] = ( V2 | ( ~V0 & V1 ) );
  ff3table[245] = ( ~V0 | V2 );
  ff3table[246] = ( V2 | ( ( V0 | V1 ) & ( ~V0 | ~V1 ) ) );
  ff3table[247] = ( ~V0 | ~V1 | V2 );
  ff3table[248] = ( V2 | ( V0 & V1 ) );
  ff3table[249] = ( V2 | ( ( ~V0 | V1 ) & ( V0 | ~V1 ) ) );
  ff3table[250] = ( V0 | V2 );
  ff3table[251] = ( V0 | ~V1 | V2 );
  ff3table[252] = ( V1 | V2 );
  ff3table[253] = ( ~V0 | V1 | V2 );
  ff3table[254] = ( V0 | V1 | V2 );
  ff3table[255] = LogExpr::make_one();
}

// 3入力関数に対する最適ファクタードフォームを求める．
LogExpr
optff3(const TvFunc& func)
{
  assert_cond(func.ni() == 3, __FILE__, __LINE__);

  ymuint32 pat = 0UL;
  for (size_t pos = 0; pos < 8; ++ pos) {
    if ( func.value(pos) ) {
      pat |= (1UL << pos);
    }
  }
  LogExpr expr = ff3table[pat];

  return expr;
}

#if VERIFY_FF3Table

void
test3()
{
  
  vector<int> values(8);
  for (ymulong v = 0; v < 256; ++ v) {
    for (size_t p = 0; p < 8; ++ p) {
      values[p] = (v & (1UL << p)) ? 1 : 0;
    }
    TvFunc f(3, values);
    LogExpr expr = optff3(f);

    ymuint32 pat = 0UL;
    for (size_t pos = 0; pos < 8; ++ pos) {
      if ( f.value(pos) ) {
	pat |= (1UL << pos);
      }
    }
    vector<ymulong> vals(3);
    for (size_t i = 0; i < 3; ++ i) {
      ymulong pat = 0UL;
      for (size_t pos = 0; pos < 8; ++ pos) {
	if ( pos & (1UL << i) ) {
	  pat |= (1UL << pos);
	}
      }
      vals[i] = pat;
    }
    ymulong pat2 = expr.eval(vals, 0xFFUL);
    if ( pat != pat2 ) {
      cout << "error" << endl
	   << "pat = ";
      for (size_t pos = 0; pos < 8; ++ pos) {
	if ( pat & (1UL << pos) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
      cout << "pat2 = ";
      for (size_t pos = 0; pos < 8; ++ pos) {
	if ( pat2 & (1UL << pos) ) {
	  cout << "1";
	}
	else {
	  cout << "0";
	}
      }
      cout << endl;
    }
  }
}

#endif

// 分解などで3入力以下に落とせる関数の処理を行う．
LogExpr
step1(ymuint32 pat)
{
  // 各変数に対するコファクターを計算する．
  // ちょっとトリッキー
  ymuint32 pat0[4];
  ymuint32 pat1[4];
  
  ymuint32 tmp;

  tmp = pat & 0x5555U;        // 0x2x4x6x8xAxCxEx
  tmp |= (tmp >> 1);          // 022446688AACCEEx
  tmp &= 0x3333U;             // 02xx46xx8AxxCExx
  tmp |= (tmp >> 2);          // 0246468A8ACECExx
  tmp &= 0x0F0FU;             // 0246xxxx8ACExxxx
  tmp |= (tmp >> 4);          // 02468ACE8ACExxxx
  tmp &= 0x00FFU;             // 02468ACExxxxxxxx
  pat0[0] = tmp;
  
  tmp = (pat >> 1) & 0x5555U; // 1x3x5x7x9xBxDxFx
  tmp |= (tmp >> 1);          // 133557799BBDDFFx
  tmp &= 0x3333U;             // 13xx57xx9BxxDFxx
  tmp |= (tmp >> 2);          // 1357579B9BDFDFxx
  tmp &= 0x0F0FU;             // 1357xxxx9BDFxxxx
  tmp |= (tmp >> 4);          // 13579BDF9BDFxxxx
  tmp &= 0x00FFU;             // 13579BDFxxxxxxxx
  pat1[0] = tmp;

  tmp = pat & 0x3333U;        // 01xx45xx89xxCDxx
  tmp |= (tmp >> 2);          // 0145458989CDCDxx
  tmp &= 0x0F0FU;             // 0145xxxx89CDxxxx
  tmp |= (tmp >> 4);          // 014589CD89CDxxxx
  tmp &= 0x00FFU;             // 014589CDxxxxxxxx
  pat0[1] = tmp;

  tmp = (pat >> 2) & 0x3333U; // 23xx67xxABxxEFxx
  tmp |= (tmp >> 2);          // 236767ABABEFEFxx
  tmp &= 0x0F0FU;             // 2367xxxxABEFxxxx
  tmp |= (tmp >> 4);          // 2367ABEFABEFxxxx
  tmp &= 0x00FFU;             // 2367ABEFxxxxxxxx
  pat1[1] = tmp;

  tmp = pat & 0x0F0FU;        // 0123xxxx89ABxxxx
  tmp |= (tmp >> 4);          // 012389AB89ABxxxx
  tmp &= 0x00FFU;             // 012389ABxxxxxxxx
  pat0[2] = tmp;

  tmp = (pat >> 4) & 0x0F0FU; // 4567xxxxCDEFxxxx
  tmp |= (tmp >> 4);          // 4567CDEFCDEFxxxx
  tmp &= 0x00FFU;             // 4567CDEFxxxxxxxx
  pat1[2] = tmp;
  
  pat0[3] = pat & 0x00FFU;
  pat1[3] = (pat >> 8) & 0x00FFU;
  
  for (size_t i = 0; i < 4; ++ i) {
    if ( pat0[i] == pat1[i] ) {
      // i 番めの変数と無関係
      LogExpr expr1 = ff3table[pat0[i]];
      VarVarMap lmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  lmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      return expr1.remap_var(lmap);
    }
    
    if ( pat0[i] == 0x00UL ) {
      // xi & fxi
      LogExpr expr1 = ff3table[pat1[i]];
      VarVarMap lmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  lmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      LogExpr expr2 = expr1.remap_var(lmap);
      return expr2 & LogExpr::make_posiliteral(i);
    }
    
    if ( pat0[i] == 0xFFUL ) {
      // xi' | fxi
      LogExpr expr1 = ff3table[pat1[i]];
      VarVarMap lmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  lmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      LogExpr expr2 = expr1.remap_var(lmap);
      return expr2 | LogExpr::make_negaliteral(i);
    }
    
    if ( pat1[i] == 0x00UL ) {
      // xi' & fxi'
      LogExpr expr1 = ff3table[pat0[i]];
      VarVarMap lmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  lmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      LogExpr expr2 = expr1.remap_var(lmap);
      return expr2 & LogExpr::make_negaliteral(i);
    }
    
    if ( pat1[i] == 0xFFUL ) {
      // xi | fxi'
      LogExpr expr1 = ff3table[pat0[i]];
      VarVarMap lmap;
      size_t k = 0;
      for (size_t j = 0; j < 4; ++ j) {
	if ( j != i ) {
	  lmap.insert(make_pair(k, j));
	  ++ k;
	}
      }
      LogExpr expr2 = expr1.remap_var(lmap);
      return expr2 | LogExpr::make_posiliteral(i);
    }
  }

#if 0
  ymuint32 pat0n1n =
    (((pat >> 0) & 1U) << 0) |
    (((pat >> 4) & 1U) << 1) |
    (((pat >> 8) & 1U) << 2) |
    (((pat >> 12) & 1U) << 3);
  ymuint32 pat0p1n =
    (((pat >> 1) & 1U) << 0) |
    (((pat >> 5) & 1U) << 1) |
    (((pat >> 9) & 1U) << 2) |
    (((pat >> 13) & 1U) << 3);
  ymuint32 pat0n1p =
    (((pat >> 2) & 1U) << 0) |
    (((pat >> 6) & 1U) << 1) |
    (((pat >> 10) & 1U) << 2) |
    (((pat >> 14) & 1U) << 3);
  ymuint32 pat0p1p =
    (((pat >> 3) & 1U) << 0) |
    (((pat >> 7) & 1U) << 1) |
    (((pat >> 11) & 1U) << 2) |
    (((pat >> 15) & 1U) << 3);
  
  ymuint32 pata = pat0n1n;
  size_t groupa = 1U;
  ymuint32 patb = 0x0U;
  size_t groupb = 0U;
  size_t groupc = 0U;
  if ( pat0p1n == pata ) {
    groupa |= 2U;
  }
  else {
    patb = pat0p1n;
    groupb |= 2U;
  }
  if ( pat0n1p == pata ) {
    groupa |= 4U;
  }
  else if ( pat0n1p == patb ) {
    groupb |= 4U;
  }
  else {
    groupc |= 4U;
  }
  if ( pat0p1p == pata ) {
    groupa |= 8U;
  }
  else if ( pat0p1p == patb ) {
    groupb |= 8U;
  }
  else {
    groupc |= 8U;
  }
  if ( groupc == 0U ) {
    assert_cond( groupa + groupb == 15U, __FILE__, __LINE__);
    // コファクターが2つのパタンからなる．
    switch ( groupa ) {
    case 1U:  // 0n1n             : 0p1n, 0n1p, 0p1p
      break;

    case 7U:  // 0n1n, 0p1n, 0n1p : 0p1p
      break;

    case 9U:  // 0n1n, 0p1p       : 0p1n, 0n1p
      break;

    case 11U: // 0n1n, 0p1n, 0p1p : 0n1p
      break;

    case 13U: // 0n1n, 0n1p, 0p1p : 0p1n
      break;
      
    case 2U:  // 0p1n             : 0n1n, 0n1p, 0p1p
    case 4U:  // 0n1p             : 0n1n, 0p1n, 0p1p
    case 6U:  // 0p1n, 0n1p       : 0n1n, 0p1p
    case 8U:  // 0p1p             : 0n1n, 0p1n, 0n1p
    case 10U: // 0p1n, 0p1p       : 0n1n, 0n1p
    case 12U: // 0n1p, 0p1p       : 0n1n, 0p1n
    case 14U: // 0p1n, 0n1p, 0p1p : 0n1n
      // 0n1n はかならず groupa に含まれる．
      assert_cond(__FILE__, __LINE__);
      break;
      
    case 3U:  // 0n1n, 0p1n       : 0n1p, 0p1p
    case 5U:  // 0n1n, 0n1p       : 0p1n, 0p1p
    case 15U: // 0n1n, 0p1n, 0n1p, 0p1p
      // これはありえない
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
#endif
  return LogExpr::make_one();
}

void
test4()
{
#if 0
  size_t n_count = 0;
  ymuint32* ptr = npn4rep;
  for ( ; ; ++ ptr) {
    ymuint32 pat = *ptr;
    if ( pat == 0xFFFFU ) {
      break;
    }
    cout << "pat  = " << hex << pat << dec << endl;
    LogExpr expr = optff4(pat);
    if ( expr.is_one() ) {
      // 見つからなかった．
      continue;
    }
    cout << "expr = " << expr << endl;
    vector<ymulong> vals(4);
    for (size_t i = 0; i < 4; ++ i) {
      ymulong pat = 0UL;
      for (size_t pos = 0; pos < 16; ++ pos) {
	if ( pos & (1UL << i) ) {
	  pat |= (1UL << pos);
	}
      }
      vals[i] = pat;
    }
    ymulong pat2 = expr.eval(vals, 0xFFFFUL);
    if ( pat != pat2 ) {
      cout << "Error" << endl
	   << " pat2 = " << hex << pat2 << dec << endl;
    }
    cout << endl;
    ++ n_count;
  }
  cout << n_count << endl;
#endif
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  init_table();

  test4();
  
  return 0;
}
