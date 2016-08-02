#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIGENBLOCK_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIGENBLOCK_H

/// @file libym_verilog/elaborator/ei/EiGenBlock.h
/// @brief EiGenBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiGenBlock.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbScope.h"
#include "ElbGfRoot.h"
#include <ym_verilog/pt/PtP.h>
#include <ym_utils/StrBuff.h>


BEGIN_NAMESPACE_YM_VERILOG

class EiGfBlock;

//////////////////////////////////////////////////////////////////////
/// @class EiGenBlock EiGenBlock.h "EiGenBlock.h"
/// @brief elaboration 中の generate block を表すクラス
//////////////////////////////////////////////////////////////////////
class EiGenBlock :
  public ElbScope
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  EiGenBlock(const VlNamedObj* parent,
	     const PtItem* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiGenBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // 対応するパース木の要素
  const PtItem* mPtItem;

};


//////////////////////////////////////////////////////////////////////
/// @class EiGfRoot EiGenBlock.h "EiGenBlock.h"
/// @brief GfBlock の親となるクラス
/// @note スコープとしての親ではなく，名前による検索のためのオブジェクト
//////////////////////////////////////////////////////////////////////
class EiGfRoot :
  public ElbGfRoot
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item 対応するパース木の要素
  EiGfRoot(const VlNamedObj* parent,
	   const PtItem* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiGfRoot();;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbGfRoot の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のスコープを追加する．
  virtual
  void
  add(int index,
      const VlNamedObj* block);

  /// @brief 子供のスコープを取り出す．
  virtual
  const VlNamedObj*
  elem_by_index(int index);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // 対応するパース木の要素
  const PtItem* mPtItem;

  // 子供のスコープのハッシュ表
  hash_map<int, const VlNamedObj*> mTable;

};


//////////////////////////////////////////////////////////////////////
/// @class EiGfBlock EiGenBlock.h "EiGenBlock.h"
/// @brief elaboration 中の generate for block を表すクラス
//////////////////////////////////////////////////////////////////////
class EiGfBlock :
  public EiGenBlock
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ環境
  /// @param[in] pt_item 対応するパース木の要素
  /// @param[in] index 対応する genvar の値
  EiGfBlock(const VlNamedObj* parent,
	    const PtItem* pt_item,
	    int index);

  /// @brief デストラクタ
  virtual
  ~EiGfBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  StrBuff mName;

};

END_NAMESPACE_YM_VERILOG

#endif // LiBYM_VERILOG_ELABORATOR_EI_EIGENBLOCK_H
