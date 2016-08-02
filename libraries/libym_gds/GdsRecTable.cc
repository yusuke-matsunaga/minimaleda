
/// @file libym_gds/GdsRecTable.cc
/// @brief GDS-II の予約語の変換テーブル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsRecTable.cc 997 2007-09-07 09:58:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GdsRecTable.h"


BEGIN_NAMESPACE_YM_GDS_PARSER

// yacc/bison が生成したヘッダファイル
#include "GdsParser.h"

END_NAMESPACE_YM_GDS_PARSER


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// Record type とトークンの変換テーブル
//////////////////////////////////////////////////////////////////////

// 唯一のオブジェクトを取り出す．
const GdsRecTable&
GdsRecTable::obj()
{
  static GdsRecTable theObj;
  return theObj;
}

// コンストラクタ
GdsRecTable::GdsRecTable()
{
  mTable[kGdsHEADER].mToken = HEADER;
  mTable[kGdsHEADER].mString = "HEADER";
  mTable[kGdsHEADER].mDtype = kGds2Int;
  mTable[kGdsHEADER].mDataNum = 1;

  mTable[kGdsBGNLIB].mToken = BGNLIB;
  mTable[kGdsBGNLIB].mString = "BGNLIB";
  mTable[kGdsBGNLIB].mDtype = kGds2Int;
  mTable[kGdsBGNLIB].mDataNum = 12;

  mTable[kGdsLIBNAME].mToken = LIBNAME;
  mTable[kGdsLIBNAME].mString = "LIBNAME";
  mTable[kGdsLIBNAME].mDtype = kGdsString;
  mTable[kGdsLIBNAME].mDataNum = 1;

  mTable[kGdsUNITS].mToken = UNITS;
  mTable[kGdsUNITS].mString = "UNITS";
  mTable[kGdsUNITS].mDtype = kGds8Real;
  mTable[kGdsUNITS].mDataNum = 2;

  mTable[kGdsENDLIB].mToken = ENDLIB;
  mTable[kGdsENDLIB].mString = "ENDLIB";
  mTable[kGdsENDLIB].mDtype = kGdsNodata;
  mTable[kGdsENDLIB].mDataNum = 0;

  mTable[kGdsBGNSTR].mToken = BGNSTR;
  mTable[kGdsBGNSTR].mString = "BGNSTR";
  mTable[kGdsBGNSTR].mDtype = kGds2Int;
  mTable[kGdsBGNSTR].mDataNum = 12;

  mTable[kGdsSTRNAME].mToken = STRNAME;
  mTable[kGdsSTRNAME].mString = "STRNAME";
  mTable[kGdsSTRNAME].mDtype = kGdsString;
  mTable[kGdsSTRNAME].mDataNum = 1;

  mTable[kGdsENDSTR].mToken = ENDSTR;
  mTable[kGdsENDSTR].mString = "ENDSTR";
  mTable[kGdsENDSTR].mDtype = kGdsNodata;
  mTable[kGdsENDSTR].mDataNum = 0;

  mTable[kGdsBOUNDARY].mToken = BOUNDARY;
  mTable[kGdsBOUNDARY].mString = "BOUNDARY";
  mTable[kGdsBOUNDARY].mDtype = kGdsNodata;
  mTable[kGdsBOUNDARY].mDataNum = 0;

  mTable[kGdsPATH].mToken = PATH;
  mTable[kGdsPATH].mString = "PATH";
  mTable[kGdsPATH].mDtype = kGdsNodata;
  mTable[kGdsPATH].mDataNum = 0;

  mTable[kGdsSREF].mToken = SREF;
  mTable[kGdsSREF].mString = "SREF";
  mTable[kGdsSREF].mDtype = kGdsNodata;
  mTable[kGdsSREF].mDataNum = 0;

  mTable[kGdsAREF].mToken = AREF;
  mTable[kGdsAREF].mString = "AREF";
  mTable[kGdsAREF].mDtype = kGdsNodata;
  mTable[kGdsAREF].mDataNum = 0;

  mTable[kGdsTEXT].mToken = TEXT;
  mTable[kGdsTEXT].mString = "TEXT";
  mTable[kGdsTEXT].mDtype = kGdsNodata;
  mTable[kGdsTEXT].mDataNum = 0;

  mTable[kGdsLAYER].mToken = LAYER;
  mTable[kGdsLAYER].mString = "LAYER";
  mTable[kGdsLAYER].mDtype = kGds2Int;
  mTable[kGdsLAYER].mDataNum = 1;

  mTable[kGdsDATATYPE].mToken = DATATYPE;
  mTable[kGdsDATATYPE].mString = "DATATYPE";
  mTable[kGdsDATATYPE].mDtype = kGds2Int;
  mTable[kGdsDATATYPE].mDataNum = 1;

  mTable[kGdsWIDTH].mToken = WIDTH;
  mTable[kGdsWIDTH].mString = "WIDTH";
  mTable[kGdsWIDTH].mDtype = kGds4Int;
  mTable[kGdsWIDTH].mDataNum = 1;

  mTable[kGdsXY].mToken = XY;
  mTable[kGdsXY].mString = "XY";
  mTable[kGdsXY].mDtype = kGds4Int;
  mTable[kGdsXY].mDataNum = -2;

  mTable[kGdsENDEL].mToken = ENDEL;
  mTable[kGdsENDEL].mString = "ENDEL";
  mTable[kGdsENDEL].mDtype = kGdsNodata;
  mTable[kGdsENDEL].mDataNum = 0;

  mTable[kGdsSNAME].mToken = SNAME;
  mTable[kGdsSNAME].mString = "SNAME";
  mTable[kGdsSNAME].mDtype = kGdsString;
  mTable[kGdsSNAME].mDataNum = 1;

  mTable[kGdsCOLROW].mToken = COLROW;
  mTable[kGdsCOLROW].mString = "COLROW";
  mTable[kGdsCOLROW].mDtype = kGds2Int;
  mTable[kGdsCOLROW].mDataNum = 2;

  mTable[kGdsTEXTNODE].mToken = TEXTNODE;
  mTable[kGdsTEXTNODE].mString = "TEXTNODE";
  mTable[kGdsTEXTNODE].mDtype = kGdsNodata;
  mTable[kGdsTEXTNODE].mDataNum = 0;

  mTable[kGdsNODE].mToken = NODE;
  mTable[kGdsNODE].mString = "NODE";
  mTable[kGdsNODE].mDtype = kGdsNodata;
  mTable[kGdsNODE].mDataNum = 0;

  mTable[kGdsTEXTTYPE].mToken = TEXTTYPE;
  mTable[kGdsTEXTTYPE].mString = "TEXTTYPE";
  mTable[kGdsTEXTTYPE].mDtype = kGds2Int;
  mTable[kGdsTEXTTYPE].mDataNum = 1;

  mTable[kGdsPRESENTATION].mToken = PRESENTATION;
  mTable[kGdsPRESENTATION].mString = "PRESENTATION";
  mTable[kGdsPRESENTATION].mDtype = kGdsBitArray;
  mTable[kGdsPRESENTATION].mDataNum = 1;

  mTable[kGdsSPACING].mToken = SPACING;
  mTable[kGdsSPACING].mString = "SPACING";
  mTable[kGdsSPACING].mDtype = kGdsNodata;
  mTable[kGdsSPACING].mDataNum = 0;

  mTable[kGdsSTRING].mToken = STRING;
  mTable[kGdsSTRING].mString = "STRING";
  mTable[kGdsSTRING].mDtype = kGdsString;
  mTable[kGdsSTRING].mDataNum = 1;

  mTable[kGdsSTRANS].mToken = STRANS;
  mTable[kGdsSTRANS].mString = "STRANS";
  mTable[kGdsSTRANS].mDtype = kGdsBitArray;
  mTable[kGdsSTRANS].mDataNum = 1;

  mTable[kGdsMAG].mToken = MAG;
  mTable[kGdsMAG].mString = "MAG";
  mTable[kGdsMAG].mDtype = kGds8Real;
  mTable[kGdsMAG].mDataNum = 1;

  mTable[kGdsANGLE].mToken = ANGLE;
  mTable[kGdsANGLE].mString = "ANGLE";
  mTable[kGdsANGLE].mDtype = kGds8Real;
  mTable[kGdsANGLE].mDataNum = 1;

  mTable[kGdsUINTEGER].mToken = UINTEGER;
  mTable[kGdsUINTEGER].mString = "UINTEGER";
  mTable[kGdsUINTEGER].mDtype = kGdsNodata;
  mTable[kGdsUINTEGER].mDataNum = 0;

  mTable[kGdsUSTRING].mToken = USTRING;
  mTable[kGdsUSTRING].mString = "USTRING";
  mTable[kGdsUSTRING].mDtype = kGdsNodata;
  mTable[kGdsUSTRING].mDataNum = 0;

  mTable[kGdsREFLIBS].mToken = REFLIBS;
  mTable[kGdsREFLIBS].mString = "REFLIBS";
  mTable[kGdsREFLIBS].mDtype = kGdsString;
  mTable[kGdsREFLIBS].mDataNum = 22;

  mTable[kGdsFONTS].mToken = FONTS;
  mTable[kGdsFONTS].mString = "FONTS";
  mTable[kGdsFONTS].mDtype = kGdsString;
  mTable[kGdsFONTS].mDataNum = 22;

  mTable[kGdsPATHTYPE].mToken = PATHTYPE;
  mTable[kGdsPATHTYPE].mString = "PATHTYPE";
  mTable[kGdsPATHTYPE].mDtype = kGds2Int;
  mTable[kGdsPATHTYPE].mDataNum = 1;

  mTable[kGdsGENERATIONS].mToken = GENERATIONS;
  mTable[kGdsGENERATIONS].mString = "GENERATIONS";
  mTable[kGdsGENERATIONS].mDtype = kGds2Int;
  mTable[kGdsGENERATIONS].mDataNum = 1;

  mTable[kGdsATTRTABLE].mToken = ATTRTABLE;
  mTable[kGdsATTRTABLE].mString = "ATTRTABLE";
  mTable[kGdsATTRTABLE].mDtype = kGdsString;
  mTable[kGdsATTRTABLE].mDataNum = 1;

  mTable[kGdsSTYPTABLE].mToken = STYPTABLE;
  mTable[kGdsSTYPTABLE].mString = "STYPTABLE";
  mTable[kGdsSTYPTABLE].mDtype = kGdsString;
  mTable[kGdsSTYPTABLE].mDataNum = 1;

  mTable[kGdsSTRTYPE].mToken = STRTYPE;
  mTable[kGdsSTRTYPE].mString = "STRTYPE";
  mTable[kGdsSTRTYPE].mDtype = kGds2Int;
  mTable[kGdsSTRTYPE].mDataNum = 1;

  mTable[kGdsELFLAGS].mToken = ELFLAGS;
  mTable[kGdsELFLAGS].mString = "ELFLAGS";
  mTable[kGdsELFLAGS].mDtype = kGdsBitArray;
  mTable[kGdsELFLAGS].mDataNum = 1;

  mTable[kGdsELKEY].mToken = ELKEY;
  mTable[kGdsELKEY].mString = "ELKEY";
  mTable[kGdsELKEY].mDtype = kGds4Int;
  mTable[kGdsELKEY].mDataNum = 1;

  mTable[kGdsLINKTYPE].mToken = LINKTYPE;
  mTable[kGdsLINKTYPE].mString = "LINKTYPE";
  mTable[kGdsLINKTYPE].mDtype = kGds2Int;
  mTable[kGdsLINKTYPE].mDataNum = 1;

  mTable[kGdsLINKKEYS].mToken = LINKKEYS;
  mTable[kGdsLINKKEYS].mString = "LINKKEYS";
  mTable[kGdsLINKKEYS].mDtype = kGds4Int;
  mTable[kGdsLINKKEYS].mDataNum = 1;

  mTable[kGdsNODETYPE].mToken = NODETYPE;
  mTable[kGdsNODETYPE].mString = "NODETYPE";
  mTable[kGdsNODETYPE].mDtype = kGds2Int;
  mTable[kGdsNODETYPE].mDataNum = 1;

  mTable[kGdsPROPATTR].mToken = PROPATTR;
  mTable[kGdsPROPATTR].mString = "PROPATTR";
  mTable[kGdsPROPATTR].mDtype = kGds2Int;
  mTable[kGdsPROPATTR].mDataNum = 1;

  mTable[kGdsPROPVALUE].mToken = PROPVALUE;
  mTable[kGdsPROPVALUE].mString = "PROPVALUE";
  mTable[kGdsPROPVALUE].mDtype = kGdsString;
  mTable[kGdsPROPVALUE].mDataNum = 1;

  mTable[kGdsBOX].mToken = BOX;
  mTable[kGdsBOX].mString = "BOX";
  mTable[kGdsBOX].mDtype = kGdsNodata;
  mTable[kGdsBOX].mDataNum = 0;

  mTable[kGdsBOXTYPE].mToken = BOXTYPE;
  mTable[kGdsBOXTYPE].mString = "BOXTYPE";
  mTable[kGdsBOXTYPE].mDtype = kGds2Int;
  mTable[kGdsBOXTYPE].mDataNum = 1;

  mTable[kGdsPLEX].mToken = PLEX;
  mTable[kGdsPLEX].mString = "PLEX";
  mTable[kGdsPLEX].mDtype = kGds4Int;
  mTable[kGdsPLEX].mDataNum = 1;

  mTable[kGdsBGNEXTN].mToken = BGNEXTN;
  mTable[kGdsBGNEXTN].mString = "BGNEXTN";
  mTable[kGdsBGNEXTN].mDtype = kGds4Int;
  mTable[kGdsBGNEXTN].mDataNum = 1;

  mTable[kGdsENDEXTN].mToken = ENDEXTN;
  mTable[kGdsENDEXTN].mString = "ENDEXTN";
  mTable[kGdsENDEXTN].mDtype = kGds4Int;
  mTable[kGdsENDEXTN].mDataNum = 1;

  mTable[kGdsTAPENUM].mToken = TAPENUM;
  mTable[kGdsTAPENUM].mString = "TAPENUM";
  mTable[kGdsTAPENUM].mDtype = kGds2Int;
  mTable[kGdsTAPENUM].mDataNum = 1;

  mTable[kGdsTAPECODE].mToken = TAPECODE;
  mTable[kGdsTAPECODE].mString = "TAPECODE";
  mTable[kGdsTAPECODE].mDtype = kGds2Int;
  mTable[kGdsTAPECODE].mDataNum = 1;

  mTable[kGdsSTRCLASS].mToken = STRCLASS;
  mTable[kGdsSTRCLASS].mString = "STRCLASS";
  mTable[kGdsSTRCLASS].mDtype = kGdsBitArray;
  mTable[kGdsSTRCLASS].mDataNum = 1;

  mTable[kGdsRESERVED].mToken = RESERVED;
  mTable[kGdsRESERVED].mString = "RESERVED";
  mTable[kGdsRESERVED].mDtype = kGds4Int;
  mTable[kGdsRESERVED].mDataNum = 1;

  mTable[kGdsFORMAT].mToken = FORMAT;
  mTable[kGdsFORMAT].mString = "FORMAT";
  mTable[kGdsFORMAT].mDtype = kGds2Int;
  mTable[kGdsFORMAT].mDataNum = 1;

  mTable[kGdsMASK].mToken = MASK;
  mTable[kGdsMASK].mString = "MASK";
  mTable[kGdsMASK].mDtype = kGdsString;
  mTable[kGdsMASK].mDataNum = 1;

  mTable[kGdsENDMASKS].mToken = ENDMASKS;
  mTable[kGdsENDMASKS].mString = "ENDMASKS";
  mTable[kGdsENDMASKS].mDtype = kGdsNodata;
  mTable[kGdsENDMASKS].mDataNum = 0;

  mTable[kGdsLIBDIRSIZE].mToken = LIBDIRSIZE;
  mTable[kGdsLIBDIRSIZE].mString = "LIBDIRSIZE";
  mTable[kGdsLIBDIRSIZE].mDtype = kGds2Int;
  mTable[kGdsLIBDIRSIZE].mDataNum = 1;

  mTable[kGdsSRFNAME].mToken = SRFNAME;
  mTable[kGdsSRFNAME].mString = "SRFNAME";
  mTable[kGdsSRFNAME].mDtype = kGdsString;
  mTable[kGdsSRFNAME].mDataNum = 1;

  mTable[kGdsLIBSECUR].mToken = LIBSECUR;
  mTable[kGdsLIBSECUR].mString = "LIBSECUR";
  mTable[kGdsLIBSECUR].mDtype = kGds2Int;
  mTable[kGdsLIBSECUR].mDataNum = -3;

  mTable[kGdsBORDER].mToken = BORDER;
  mTable[kGdsBORDER].mString = "BORDER";
  mTable[kGdsBORDER].mDtype = kGdsNodata;
  mTable[kGdsBORDER].mDataNum = 0;

  mTable[kGdsSOFTFENCE].mToken = SOFTFENCE;
  mTable[kGdsSOFTFENCE].mString = "SOFTFENCE";
  mTable[kGdsSOFTFENCE].mDtype = kGdsNodata;
  mTable[kGdsSOFTFENCE].mDataNum = 0;

  mTable[kGdsHARDFENCE].mToken = HARDFENCE;
  mTable[kGdsHARDFENCE].mString = "HARDFENCE";
  mTable[kGdsHARDFENCE].mDtype = kGdsNodata;
  mTable[kGdsHARDFENCE].mDataNum = 0;

  mTable[kGdsSOFTWIRE].mToken = SOFTWIRE;
  mTable[kGdsSOFTWIRE].mString = "SOFTWIRE";
  mTable[kGdsSOFTWIRE].mDtype = kGdsNodata;
  mTable[kGdsSOFTWIRE].mDataNum = 0;

  mTable[kGdsHARDWIRE].mToken = HARDWIRE;
  mTable[kGdsHARDWIRE].mString = "HARDWIRE";
  mTable[kGdsHARDWIRE].mDtype = kGdsNodata;
  mTable[kGdsHARDWIRE].mDataNum = 0;

  mTable[kGdsPATHPORT].mToken = PATHPORT;
  mTable[kGdsPATHPORT].mString = "PATHPORT";
  mTable[kGdsPATHPORT].mDtype = kGdsNodata;
  mTable[kGdsPATHPORT].mDataNum = 0;

  mTable[kGdsNODEPORT].mToken = NODEPORT;
  mTable[kGdsNODEPORT].mString = "NODEPORT";
  mTable[kGdsNODEPORT].mDtype = kGdsNodata;
  mTable[kGdsNODEPORT].mDataNum = 0;

  mTable[kGdsUSERCONSTRAINT].mToken = USERCONSTRAINT;
  mTable[kGdsUSERCONSTRAINT].mString = "USERCONSTRAINT";
  mTable[kGdsUSERCONSTRAINT].mDtype = kGdsNodata;
  mTable[kGdsUSERCONSTRAINT].mDataNum = 0;

  mTable[kGdsSPACER_ERROR].mToken = SPACER_ERROR;
  mTable[kGdsSPACER_ERROR].mString = "SPACER_ERROR";
  mTable[kGdsSPACER_ERROR].mDtype = kGdsNodata;
  mTable[kGdsSPACER_ERROR].mDataNum = 0;

  mTable[kGdsCONTACT].mToken = CONTACT;
  mTable[kGdsCONTACT].mString = "CONTACT";
  mTable[kGdsCONTACT].mDtype = kGdsNodata;
  mTable[kGdsCONTACT].mDataNum = 0;
}

// デストラクタ
GdsRecTable::~GdsRecTable()
{
}

// レコード型からトークン値を得る．
int
GdsRecTable::rtype_token(tGdsRtype rtype) const
{
  return mTable[rtype].mToken;
}

// レコード型から文字列を得る．
const char*
GdsRecTable::rtype_string(tGdsRtype rtype) const
{
  return mTable[rtype].mString;
}

// このレコード型に対応したデータ型を得る．
tGdsDtype
GdsRecTable::dtype(tGdsRtype rtype) const
{
  return mTable[rtype].mDtype;
}

// データ型から文字列を得る．
const char*
GdsRecTable::dtype_string(tGdsDtype dtype) const
{
  switch ( dtype ) {
  case kGdsNodata:   return "No data present";
  case kGdsBitArray: return "Bit Array";
  case kGds2Int:     return "Two-Byte Signed Integer";
  case kGds4Int:     return "Four-Byte Signed Integer";
  case kGds4Real:    return "Four-Byte Real";
  case kGds8Real:    return "Eight-Byte Real";
  case kGdsString:   return "ASCII String";
  }
  return "illegal data type";
}

// レコード型から期待されるデータ数を得る．
// 負数の場合にはその絶対値の倍数の可変個数の意味
int
GdsRecTable::data_num(tGdsRtype rtype) const
{
  return mTable[rtype].mDataNum;
}

END_NAMESPACE_YM_GDS
