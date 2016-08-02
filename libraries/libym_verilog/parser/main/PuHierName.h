#ifndef LIBYM_VERILOG_PARSER_PUHIERNAME_H
#define LIBYM_VERILOG_PARSER_PUHIERNAME_H

/// @file libym_verilog/parser/PuHierName.h
/// @brief PuHierName のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PuHierName.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "PtiFwd.h"
#include "PtrList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PuHeirName Putils.h "Putils.h"
/// @ingroup VlParser
/// @brief 階層つき名を表すクラス
///
/// 中身は階層部分を表す PtNameBranch のリストと末尾の名前を表す
/// 文字列から成る．
///
/// コンストラクタでは最下層の名前の設定のみを行う．
/// 階層の追加は add(const char*), add(int index, const char*)
/// で行う．これは内部で PtNameBranch を生成していることに因る．
//////////////////////////////////////////////////////////////////////
class PuHierName
{
  friend class Parser;
  
private:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] nb 階層ブランチ
  /// @param[in] name 名前
  PuHierName(UnitAlloc& alloc,
	     PtNameBranch* nb,
	     const char* name);

  /// @brief デストラクタ
  ~PuHierName();


public:

  /// @brief 階層を追加する．
  /// @param[in] nb 追加する階層ブランチ
  /// @param[in] tail_name 追加する最下層の名前
  void
  add(PtNameBranch* nb,
      const char* tail_name);


public:
  
  /// @brief 最下層の名前を取り出す．
  /// @return 最下層の名前
  const char*
  tail_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtrList<PtNameBranch> mNbList;

  // 最下層の名前
  const char* mTailName;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 単一の文字列からのコンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] nb 階層ブランチ
// @param[in] name 名前
inline
PuHierName::PuHierName(UnitAlloc& alloc,
		       PtNameBranch* nb,
		       const char* name) :
  mNbList(alloc),
  mTailName(name)
{
  mNbList.push_back(nb);
}

// @brief デストラクタ
inline
PuHierName::~PuHierName()
{
}

// 今の tail_name を階層名のブランチとしてその下に tail_name を
// 追加する．
inline
void
PuHierName::add(PtNameBranch* nb,
		const char* tail_name)
{
  mNbList.push_back(nb);
  mTailName = tail_name;
}

// @brief 最下層の名前を取り出す．
// @return 最下層の名前
inline
const char*
PuHierName::tail_name() const
{
  return mTailName;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_PUHIERNAME_H
