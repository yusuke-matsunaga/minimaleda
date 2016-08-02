#ifndef YM_GDS_GDS_NSDEF_H
#define YM_GDS_GDS_NSDEF_H

/// @file ym_gds/gds_nsdef.h
/// @brief gds パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gds_nsdef.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

/// @brief gds 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_GDS \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsGds)

/// @brief gds 用の名前空間の終了
#define END_NAMESPACE_YM_GDS \
END_NAMESPACE(nsGds) \
END_NAMESPACE_YM

/// @brief GdsParser 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_GDS_PARSER	\
BEGIN_NAMESPACE_YM_GDS			\
BEGIN_NAMESPACE(nsParser)

/// @brief GdsParser 用の名前空間の終了
#define END_NAMESPACE_YM_GDS_PARSER		\
END_NAMESPACE(nsParser)			\
END_NAMESPACE_YM_GDS

/// @namespace nsYm::nsGds
/// @brief GDS-II 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

/// @namespace nsYm::nsGds::nsParser
/// @brief GDS-II パーサー関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// Record Types
//////////////////////////////////////////////////////////////////////
enum tGdsRtype {
  kGdsHEADER = 0,
  kGdsBGNLIB = 1,
  kGdsLIBNAME = 2,
  kGdsUNITS = 3,
  kGdsENDLIB = 4,
  kGdsBGNSTR = 5,
  kGdsSTRNAME = 6,
  kGdsENDSTR = 7,
  kGdsBOUNDARY = 8,
  kGdsPATH = 9,
  kGdsSREF = 10,
  kGdsAREF = 11,
  kGdsTEXT = 12,
  kGdsLAYER = 13,
  kGdsDATATYPE = 14,
  kGdsWIDTH = 15,
  kGdsXY = 16,
  kGdsENDEL = 17,
  kGdsSNAME = 18,
  kGdsCOLROW = 19,
  kGdsTEXTNODE = 20,
  kGdsNODE = 21,
  kGdsTEXTTYPE = 22,
  kGdsPRESENTATION = 23,
  kGdsSPACING = 24,
  kGdsSTRING = 25,
  kGdsSTRANS = 26,
  kGdsMAG = 27,
  kGdsANGLE = 28,
  kGdsUINTEGER = 29,
  kGdsUSTRING = 30,
  kGdsREFLIBS = 31,
  kGdsFONTS = 32,
  kGdsPATHTYPE = 33,
  kGdsGENERATIONS = 34,
  kGdsATTRTABLE = 35,
  kGdsSTYPTABLE = 36,
  kGdsSTRTYPE = 37,
  kGdsELFLAGS = 38,
  kGdsELKEY = 39,
  kGdsLINKTYPE = 40,
  kGdsLINKKEYS = 41,
  kGdsNODETYPE = 42,
  kGdsPROPATTR = 43,
  kGdsPROPVALUE = 44,
  kGdsBOX = 45,
  kGdsBOXTYPE = 46,
  kGdsPLEX = 47,
  kGdsBGNEXTN = 48,
  kGdsENDEXTN = 49,
  kGdsTAPENUM = 50,
  kGdsTAPECODE = 51,
  kGdsSTRCLASS = 52,
  kGdsRESERVED = 53,
  kGdsFORMAT = 54,
  kGdsMASK = 55,
  kGdsENDMASKS = 56,
  kGdsLIBDIRSIZE = 57,
  kGdsSRFNAME = 58,
  kGdsLIBSECUR = 59,
  kGdsBORDER = 60,
  kGdsSOFTFENCE = 61,
  kGdsHARDFENCE = 62,
  kGdsSOFTWIRE = 63,
  kGdsHARDWIRE = 64,
  kGdsPATHPORT = 65,
  kGdsNODEPORT = 66,
  kGdsUSERCONSTRAINT = 67,
  kGdsSPACER_ERROR = 68,
  kGdsCONTACT = 69,

  kGdsLast = kGdsCONTACT
};


//////////////////////////////////////////////////////////////////////
// Data Types
//////////////////////////////////////////////////////////////////////
enum tGdsDtype {
  kGdsNodata = 0,
  kGdsBitArray = 1,
  kGds2Int = 2,
  kGds4Int = 3,
  kGds4Real = 4,
  kGds8Real = 5,
  kGdsString = 6
};


//////////////////////////////////////////////////////////////////////
// 1バイトの単位
//////////////////////////////////////////////////////////////////////

typedef ymuint8 tGdsByte;


//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class GdsRecord;
class GdsRecMgr;
class GdsScanner;
class GdsDumper;

END_NAMESPACE_YM_GDS

#endif
