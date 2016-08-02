#ifndef LIBYM_VERILOG_ELABORATOR_ATTRDICT_H
#define LIBYM_VERILOG_ELABORATOR_ATTRDICT_H

/// @file libym_verilog/elaborator/AttrDict.h
/// @brief AttrDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AttrDict.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/Alloc.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtAttrInst;

//////////////////////////////////////////////////////////////////////
/// @class AttrDict AttrDict.h "AttrDict.h"
/// @brief PtAttrInst をキーとして ElbAttrList を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class AttrDict
{
public:

  /// @brief コンストラクタ
  AttrDict();

  /// @brief デストラクタ
  ~AttrDict();


public:
  //////////////////////////////////////////////////////////////////////
  // AttrDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録する．
  /// @param[in] pt_attr パース木の属性定義
  /// @param[in] attr_list 登録する属性のリスト
  void
  add(const PtAttrInst* pt_attr,
      ElbAttrList* attr_list);
      
  /// @brief 属性リストを取り出す．
  /// @param[in] pt_attr パース木の属性定義
  ElbAttrList*
  find(const PtAttrInst* pt_attr) const;
  
  /// @brief 内容をクリアする．
  void
  clear();
  
  /// @brief このオブジェクトが使用しているメモリ量を返す．
  size_t
  allocated_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルの領域を確保する．
  void
  alloc_table(ymuint size);

  /// @brief ハッシュ値を計算する．
  ymuint
  hash_func(const PtAttrInst* pt_attr) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用するデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // パース木の属性定義
    const PtAttrInst* mPtAttr;
    
    // 対象の属性リスト
    ElbAttrList* mAttrList;
    
    // 次の要素を指すリンク
    Cell* mLink;
    
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell 用のアロケータ
  UnitAlloc mAlloc;
  
  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ATTRDICT_H
