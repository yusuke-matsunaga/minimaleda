
/// @file libym_verilog/parser/PtiBase.cc
/// @brief PtiXXX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtiBase.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PtiDecl.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class PtiPortRef PtiModule.h "PtiModule.h"
// @brief port reference を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtiPortRef::PtiPortRef() :
  mDir(kVpiNoDirection)
{
}

// @brief デストラクタ
PtiPortRef::~PtiPortRef()
{
}

// @brief 方向をセットする．
void
PtiPortRef::set_dir(tVpiDirection dir)
{
  mDir = dir;
}

// @brief 方向を得る．
tVpiDirection
PtiPortRef::dir() const
{
  return mDir;
}


//////////////////////////////////////////////////////////////////////
// PtNameBranch 用の関数
// 適当な場所がないのでここに置いている．
//////////////////////////////////////////////////////////////////////

// @brief 階層名を作り出す関数
// @param[in] nb 階層ブランチリストの先頭
// @param[in] name 末尾の名前
// @return 階層名を展開したものを返す．
string
expand_full_name(const PtNameBranchArray& nb_array,
		 const char* name)
{
  ostringstream buf;
  const char* period = "";
  for (ymuint32 i = 0; i < nb_array.size(); ++ i) {
    const PtNameBranch* nb = nb_array[i];
    buf << period << nb->name();
    if ( nb->has_index() ) {
      buf << "[" << nb->index() << "]";
    }
    period = ".";
  }
  if ( name != NULL ) {
    buf << period << name;
  }
  buf.flush();
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
