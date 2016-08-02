
/// @file ym_cell/DotLibParser.cc
/// @brief DotLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotLibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibParser.h"
#include "DotLibLex.h"


BEGIN_NAMESPACE_YM_CELL

// コンストラクタ
DotLibParser::DotLibParser() :
  mLex(new DotLibLex(mMsgMgr))
{

}

// デストラクタ
DotLibParser::~DotLibParser()
{
  delete mLex;
}

// @brief ファイルを読み込む．
bool
DotLibParser::read_file(const string& filename)
{
  if ( !mLex->open_file(filename) ) {
    return false;
  }


  mLex->close_file();

  return true;
}

// @brief 字句解析を行う．
int
DotLibParser::yylex(YYSTYPE& lval,
		    FileRegion& lloc)
{
  int token = mLex->read_token();
  switch ( token ) {
  case STR:
    case
  }
}

END_NAMESPACE_YM_CELL
