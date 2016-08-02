#ifndef YM_AIG_AIGHANDLE_H
#define YM_AIG_AIGHANDLE_H

/// @file ym_aig/AigHandle.h
/// @brief AigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: AigHandle.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/aig_nsdef.h"


BEGIN_NAMESPACE_YM_AIG

class AigNode;

//////////////////////////////////////////////////////////////////////
/// @class AigHandle AigHandle.h <ym_aig/AigHandle.h>
/// @brief 枝を表すクラス
//////////////////////////////////////////////////////////////////////
class AigHandle
{
  friend class AigMgrImpl;
  friend class AigNode;
  
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  AigHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  AigHandle(AigNode* node,
	    bool inv);

  /// @brief デストラクタ
  ~AigHandle();

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(AigNode* node,
      bool inv);

  
public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定の枝を返す．
  AigHandle
  operator~() const;

  /// @brief ノードを得る．
  AigNode*
  node() const;
  
  /// @brief ノードの通し番号を得る．
  ymuint
  node_id() const;

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
  AigHandle
  fanin_handle(ymuint pos) const;

  /// @brief fanin0 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  AigHandle
  fanin0_handle() const;

  /// @brief fanin1 のハンドルを得る．
  /// @note is_and() の時のみ意味を持つ．
  AigHandle
  fanin1_handle() const;
  
  /// @brief ハッシュ値を返す．
  ymuint32
  hash_func() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  bool
  operator==(AigHandle src1,
	     AigHandle src2);

  
private:

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  AigHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべてのデータをまとめたもの
  ympuint mPackedData;

};

/// @relates AigHandle
/// @brief 等価比較演算
bool
operator==(AigHandle src1,
	   AigHandle src2);

/// @relates AigHandle
/// @brief 非等価比較演算
bool
operator!=(AigHandle src1,
	   AigHandle src2);

/// @relates AigHandle
/// @brief 内容をダンプする関数
/// @param[in] src 根の枝
/// @param[in] mark すでに処理したノードの番号を保持するマーク
/// @param[in] s 出力先のストリーム
void
dump_handle(AigHandle src,
	    hash_set<ymuint>& mark,
	    ostream& s);

/// @relates AigHandle
/// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   AigHandle src);


//////////////////////////////////////////////////////////////////////
// AigHandle のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
AigHandle::AigHandle() :
  mPackedData(0UL)
{
}

// @brief 内容を直接指定したコンストラクタ
inline
AigHandle::AigHandle(ympuint data) :
  mPackedData(data)
{
}

// @brief 内容を設定したコンストラクタ
inline
AigHandle::AigHandle(AigNode* node,
		     bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief デストラクタ
inline
AigHandle::~AigHandle()
{
}

// @brief 内容を設定する．
inline
void
AigHandle::set(AigNode* node,
	       bool inv)
{
  mPackedData = reinterpret_cast<ympuint>(node) | inv;
}

// @brief 否定の枝を返す．
inline
AigHandle
AigHandle::operator~() const
{
  return AigHandle(mPackedData ^ 1UL);
}

// @brief ノードを得る．
inline
AigNode*
AigHandle::node() const
{
  return reinterpret_cast<AigNode*>(mPackedData & ~3UL);
}

// @brief 極性を得る．
// @return 反転しているとき true を返す．
inline
bool
AigHandle::inv() const
{
  return static_cast<bool>(mPackedData & 1UL);
}

// @brief 定数0を指しているとき true を返す．
inline
bool
AigHandle::is_zero() const
{
  return mPackedData == 0UL;
}

// @brief 定数1を指しているとき true を返す．
inline
bool
AigHandle::is_one() const
{
  return mPackedData == 1UL;
}

// @brief 定数を指しているとき true を返す．
inline
bool
AigHandle::is_const() const
{
  return (mPackedData & ~1UL) == 0UL;
}
  
// @brief ハッシュ値を返す．
inline
ymuint32
AigHandle::hash_func() const
{
  return static_cast<ymuint32>(mPackedData);
}

// @relates AigHandle
// @brief 等価比較演算
inline
bool
operator==(AigHandle src1,
	   AigHandle src2)
{
  return src1.mPackedData == src2.mPackedData;
}

// @relates AigHandle
// @brief 非等価比較演算
inline
bool
operator!=(AigHandle src1,
	   AigHandle src2)
{
  return !operator==(src1, src2);
}

END_NAMESPACE_YM_AIG

BEGIN_NAMESPACE_HASH
// AigHandleをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsAig::AigHandle>
{
  ymuint
  operator()(nsYm::nsAig::AigHandle aig) const
  {
    return aig.hash_func();
  }
};
END_NAMESPACE_HASH

#endif // YM_AIG_AIGHANDLE_H
