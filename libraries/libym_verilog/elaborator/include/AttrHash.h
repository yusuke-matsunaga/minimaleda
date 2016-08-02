#ifndef LIBYM_VERILOG_ELB_ATTRHASH_H
#define LIBYM_VERILOG_ELB_ATTRHASH_H

/// @file libym_verilog/elaborator/include/AttrHash.h
/// @brief AttrHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AttrHash.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/Alloc.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AttrHash AttrHash.h "AttrHash.h"
/// @brief VlObj に付帯する VlAttribute を管理するクラス
//////////////////////////////////////////////////////////////////////
class AttrHash
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のアロケータ
  AttrHash(AllocBase& alloc);

  /// @brief デストラクタ
  ~AttrHash();


public:
  
  /// @brief 内容をクリアする．
  void
  clear();
  
  /// @brief 属性を追加する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] attr_list 付加する属性リスト
  void
  add(const VlObj* obj,
      bool def,
      ElbAttrList* attr_list);
  
  /// @brief 属性を取り出す．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  ElbAttrList*
  find(const VlObj* obj,
       bool def) const;

  /// @brief このオブジェクトが使用しているメモリ量を返す．
  size_t
  allocated_size() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // 対象のオブジェクト
    const VlObj* mObj;
    
    // 対応する属性リスト
    ElbAttrList* mAttrList[2];

    // ハッシュ上の次の要素を指すポインタ
    Cell* mLink;
    
  };


private:

  /// @brief 新しい Cell を生成する．
  /// @param[in] obj 対象のオブジェクト
  Cell*
  new_cell(const VlObj* obj);
  
  /// @brief オブジェクトから対応する Cell を取り出す．
  /// @param[in] obj 対象のオブジェクト
  Cell*
  find_cell(const VlObj* obj) const;

  /// @brief テーブルの領域を確保する．
  void
  alloc_table(ymuint32 size);
  
  /// @brief ハッシュ値を計算する．
  ymuint32
  hash_func(const VlObj* obj) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // Cell の確保用アロケータ
  AllocBase& mAlloc;
  
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

#endif // LIBYM_VERILOG_ELB_ATTRHASH_H
