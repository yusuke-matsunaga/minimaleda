#ifndef LIBYM_CEC_FRAIGHANDLE_H
#define LIBYM_CEC_FRAIGHANDLE_H

/// @file libym_cec/FraigHandle.h
/// @brief FraigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: FraigHandle.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cec/cec_nsdef.h"


BEGIN_NAMESPACE_YM_CEC

class FraigNode;

//////////////////////////////////////////////////////////////////////
/// @class FraigHandle FraigHandle.h <ym_aig/FraigHandle.h>
/// @brief 枝を表すクラス
//////////////////////////////////////////////////////////////////////
class FraigHandle
{
  friend class FraigMgrImpl;
  friend class FraigNode;
  
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  FraigHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  FraigHandle(FraigNode* node,
	      bool inv);

  /// @brief デストラクタ
  ~FraigHandle();

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(FraigNode* node,
      bool inv);

  
public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定の枝を返す．
  FraigHandle
  operator~() const;

  /// @brief ノードを得る．
  FraigNode*
  node() const;
  
  /// @brief ノードの通し番号を得る．
  ymuint
  var_id() const;

  /// @brief 極性を得る．
  /// @return 反転しているとき true を返す．
  bool
  inv() const;

  /// @brief 定数0を指しているとき true を返す．
  bool
  is_zero() const;

  /// @brief 定数1を指しているとき true を返す．
  bool
  is_one() const;

  /// @brief 定数を指しているとき true を返す．
  bool
  is_const() const;
  
  /// @brief 外部入力ノードへのハンドルのとき true を返す．
  bool
  is_input() const;
  
  /// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
  /// @note is_input() の時のみ意味を持つ．
  ymuint
  input_id() const;
  
  /// @brief ANDノードへのハンドルのとき true を返す．
  bool
  is_and() const;

  /// @brief pos で指示されたファンインのハンドルを得る．
  /// @note pos は 0 か 1 でなければならない．
  /// @note is_and() の時のみ意味を持つ．
  FraigHandle
  fanin_handle(ymuint pos) const;

  /// @brief fanin0 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  FraigHandle
  fanin0_handle() const;

  /// @brief fanin1 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  FraigHandle
  fanin1_handle() const;
  
  /// @brief 代表ハンドルを得る．
  FraigHandle
  rep_handle() const;
  
  /// @brief ハッシュ値を返す．
  ymuint32
  hash_func() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(FraigHandle src1,
	     FraigHandle src2);

  
private:

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  FraigHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべてのデータをまとめたもの
  ympuint mPackedData;

};

/// @relates FraigHandle
/// @brief 等価比較演算
bool
operator==(FraigHandle src1,
	   FraigHandle src2);

/// @relates FraigHandle
/// @brief 非等価比較演算
bool
operator!=(FraigHandle src1,
	   FraigHandle src2);

/// @relates FraigHandle
/// @brief 内容をダンプする関数
/// @param[in] src 根の枝
/// @param[in] mark すでに処理したノードの番号を保持するマーク
/// @param[in] s 出力先のストリーム
void
dump_handle(FraigHandle src,
	    hash_set<ymuint>& mark,
	    ostream& s);

/// @relates FraigHandle
/// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   FraigHandle src);


//////////////////////////////////////////////////////////////////////
// FraigHandle のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
FraigHandle::FraigHandle() :
  mPackedData(0UL)
{
}

// @brief 内容を直接指定したコンストラクタ
inline
FraigHandle::FraigHandle(ympuint data) :
  mPackedData(data)
{
}

// @brief 内容を設定したコンストラクタ
inline
FraigHandle::FraigHandle(FraigNode* node,
			 bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief デストラクタ
inline
FraigHandle::~FraigHandle()
{
}

// @brief 内容を設定する．
inline
void
FraigHandle::set(FraigNode* node,
		 bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief 否定の枝を返す．
inline
FraigHandle
FraigHandle::operator~() const
{
  return FraigHandle(mPackedData ^ 1UL);
}

// @brief ノードを得る．
inline
FraigNode*
FraigHandle::node() const
{
  return reinterpret_cast<FraigNode*>(mPackedData & ~3UL);
}

// @brief 極性を得る．
// @return 反転しているとき true を返す．
inline
bool
FraigHandle::inv() const
{
  return static_cast<bool>(mPackedData & 1UL);
}

// @brief 定数0を指しているとき true を返す．
inline
bool
FraigHandle::is_zero() const
{
  return mPackedData == 0UL;
}

// @brief 定数1を指しているとき true を返す．
inline
bool
FraigHandle::is_one() const
{
  return mPackedData == 1UL;
}

// @brief 定数を指しているとき true を返す．
inline
bool
FraigHandle::is_const() const
{
  return (mPackedData & ~1UL) == 0UL;
}
  
// @brief ハッシュ値を返す．
inline
ymuint32
FraigHandle::hash_func() const
{
  return static_cast<ymuint32>(mPackedData);
}

// @relates FraigHandle
// @brief 等価比較演算
inline
bool
operator==(FraigHandle src1,
	   FraigHandle src2)
{
  return src1.mPackedData == src2.mPackedData;
}

// @relates FraigHandle
// @brief 非等価比較演算
inline
bool
operator!=(FraigHandle src1,
	   FraigHandle src2)
{
  return !operator==(src1, src2);
}

END_NAMESPACE_YM_CEC

BEGIN_NAMESPACE_HASH
// FraigHandleをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsCec::FraigHandle>
{
  ymuint
  operator()(nsYm::nsCec::FraigHandle aig) const
  {
    return aig.hash_func();
  }
};
END_NAMESPACE_HASH

#endif // LIBYM_CEC_FRAIGHANDLE_H
