#ifndef LIBYM_CEC_FRAIGHASH_H
#define LIBYM_CEC_FRAIGHASH_H

/// @file libym_cec/FraigHash.h
/// @brief FraigHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: FraigHash.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cec/cec_nsdef.h"


BEGIN_NAMESPACE_YM_CEC

class FraigNode;

//////////////////////////////////////////////////////////////////////
/// @class FraigHash FraigHash.h "FraigHash.h"
/// @brief FraigNode のハッシュ表を実装するためのクラス
//////////////////////////////////////////////////////////////////////
class FraigHash
{
public:

  /// @brief コンストラクタ
  FraigHash();

  /// @brief デストラクタ
  ~FraigHash();
  

public:
  
  /// @brief 要素数を得る．
  ymuint
  num() const;

  /// @brief テーブルサイズを得る．
  ymuint
  table_size() const;

  /// @brief 先頭の要素を得る．
  FraigNode*
  elem(ymuint hash_val) const;
  
  /// @brief 拡張が必要か調べる．
  bool
  need_expand();
  
  /// @brief 内容を空にする．
  /// @note テーブルサイズはそのまま
  void
  clear();
  
  /// @brief 要素を追加する．
  void
  add_elem(ymuint hash_val,
	   FraigNode* node,
	   FraigNode*& link);

  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 要素数
  ymuint32 mNum;
  
  // mHashTable のサイズ
  ymuint32 mHashSize;
  
  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;
  
  // ハッシュ表
  FraigNode** mHashTable;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief 要素数を得る．
inline
ymuint
FraigHash::num() const
{
  return mNum;
}

// @brief テーブルサイズを得る．
inline
ymuint
FraigHash::table_size() const
{
  return mHashSize;
}

// @brief 先頭の要素を得る．
inline
FraigNode*
FraigHash::elem(ymuint hash_val) const
{
  return mHashTable[hash_val % mHashSize];
}
  
// @brief 拡張が必要か調べる．
inline
bool
FraigHash::need_expand()
{
  if ( mNum >= mNextLimit ) {
    alloc_table(mHashSize << 1);
    return true;
  }
  return false;
}
  
// @brief 内容を空にする．
// @note テーブルサイズはそのまま
inline
void
FraigHash::clear()
{
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 要素を追加する．
inline
void
FraigHash::add_elem(ymuint hash_val,
		    FraigNode* node,
		    FraigNode*& link)
{
  ymuint idx = hash_val % mHashSize;
  link = mHashTable[idx];
  mHashTable[idx] = node;
  ++ mNum;
}

END_NAMESPACE_YM_CEC

#endif // LIBYM_CEC_FRAIGHASH_H
