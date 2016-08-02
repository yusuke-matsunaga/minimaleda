#ifndef LIBYM_VERILOG_ELB_MODULEHASH_H
#define LIBYM_VERILOG_ELB_MODULEHASH_H

/// @file libym_verilog/elaborator/include/ModuleHash.h
/// @brief ModuleHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ModuleHash.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlModule;
class VlNamedObj;

//////////////////////////////////////////////////////////////////////
/// @class ModuleHash ModuleHash.h "ModuleHash.h"
/// @brief モジュール定義名をキーにして VlModule を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class ModuleHash
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のオブジェクト
  ModuleHash(AllocBase& alloc);

  /// @brief デストラクタ
  ~ModuleHash();


public:
  //////////////////////////////////////////////////////////////////////
  // ModuleHash の関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 内容を空にする．
  void
  clear();
  
  /// @brief 要素を追加する．
  /// @note 同名のモジュールが登録されていたらそのエントリを無効化する．
  void
  add(const VlModule* obj);
      
  /// @brief 名前から該当する要素を検索する．
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  const VlModule*
  find(const VlNamedObj* parent,
       const char* name) const;

  /// @brief このオブジェクトが使用しているメモリ量を返す．
  size_t
  allocated_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルの領域を確保する．
  void
  alloc_table(ymuint32 size);

  /// @brief ハッシュ値を計算する．
  ymuint32
  hash_func(const VlNamedObj* parent,
	    const char* name) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用されるデータ構造
  //////////////////////////////////////////////////////////////////////
  
  struct Cell
  {
    // 親のスコープ
    const VlNamedObj* mParent;

    // モジュールの定義名
    const char* mName;

    // モジュール
    const VlModule* mModule;

    // 次の要素を指すリンク
    Cell* mLink;
    
  };

  
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

#endif // LIBYM_VERILOG_ELB_MODULEHASH_H
