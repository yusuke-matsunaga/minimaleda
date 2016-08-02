#ifndef LIBYM_LEXP_LEXPNODE_H
#define LIBYM_LEXP_LEXPNODE_H

/// @file libym_lexp/LexpNode.h
/// @brief LexpNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LexpNode.h 1143 2008-01-04 07:27:17Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lexp/LogExpr.h"
#include "lexp_types.h"


BEGIN_NAMESPACE_YM_LEXP

class SopLit;

//////////////////////////////////////////////////////////////////////
/// @class LexpNode LexpNode.h "LexpNode.h"
/// @brief 論理式を形作るノードのクラス
/// 効率化のためにコピーはポインタのコピーを用いる．
/// そのために参照回数を持つ．
//////////////////////////////////////////////////////////////////////
class LexpNode
{
  friend class LogExpr;
  friend class LexpNodePtr;
  friend class LexpMgr;

public:
  
  //////////////////////////////////////////////////////////////////////
  /// @name 内部の情報を読み出す関数
  /// @{
  
  /// @brief 型を返す
  tType
  type() const;
  
  /// @brief 恒偽関数を表している時に真となる．
  bool
  is_zero() const;
  
  /// @brief 恒真関数を表している時に真となる．
  bool
  is_one() const;
  
  /// @brief 定数関数を表している時に真となる．
  bool
  is_constant() const;
  
  /// @brief 肯定のリテラルを表している時に真となる．
  bool
  is_posiliteral() const;
  
  /// @brief 否定のリテラルを表している時に真となる．
  bool
  is_negaliteral() const;
  
  /// @brief リテラルを表している時に真となる．
  bool
  is_literal() const;
  
  /// @brief type がリテラルで極性が pol の時に真となる．
  /// @param[in] pol 極性
  bool
  is_literal(tPol pol) const;
  
  /// @brief リテラルノードの場合に変数番号を取り出す．
  tVarId
  varid() const;
  
  /// @brief AND ノードの時に真となる．
  bool
  is_and() const;
  
  /// @brief OR ノードの時に真となる．
  bool
  is_or() const;
  
  /// @brief XOR ノードの時に真となる．
  bool
  is_xor() const;
  
  /// @brief 演算子ノードの時に真となる．
  ///
  /// enum の順番に依存するよくないコード
  bool
  is_op() const;
  
  /// @brief 演算子ノードの場合に項の数を返す．
  size_t
  child_num() const;
  
  /// @brief 演算子ノードの場合に子供のノードを返す．
  /// @param[in] pos 取り出す子供の位置
  /// @note 演算子ノード以外の場合と pos が範囲外の場合には定数0を返す．
  /// 通常は定数ノードが子供に含まれることはないのでエラーとわかる．
  const LexpNode*
  child(size_t pos) const;

  /// @brief vals の値にしたがった評価を行う．
  ymulong
  eval(const vector<ymulong>& vals,
       ymulong mask) const;
  
  /// @brief 定数,リテラルもしくは子供がリテラルのノードの時に true を返す．
  bool
  is_simple() const;
  
  /// @brief 子供がすべてリテラルの AND ノードの時に true を返す．
  bool
  is_simple_and() const;
  
  /// @brief 子供がすべてリテラルの OR ノードの時に true を返す．
  bool
  is_simple_or() const;
  
  /// @brief 子供がすべてリテラルの XOR ノードの時に true を返す．
  bool
  is_simple_xor() const;
  
  /// @brief SOP 形式の時 true を返す．
  bool
  is_sop() const;
  
  /// @brief リテラル数を返す．
  size_t
  litnum() const;
  
  /// @brief 特定の変数のリテラルの出現回数を返す．
  /// @param[in] varid 計測対象の変数番号
  size_t
  litnum(tVarId varid) const;
  
  /// @brief 特定の変数の特定の極性のリテラルの出現回数を返す．
  /// @param[in] varid 計測対象の変数番号
  /// @param[in] pol 計測対象の極性
  size_t
  litnum(tVarId varid,
	 tPol pol) const;
  
  /// @brief 使われている変数の最大の番号 + 1を得る．
  size_t
  input_size() const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  SopLit
  soplit(bool inverted) const;
  
  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  /// @param[in] varid 計測対象の変数番号
  SopLit
  soplit(bool inverted,
	 tVarId varid) const;
  
  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  /// @param[in] varid 計測対象の変数番号
  /// @param[in] pol 計測対象の極性
  SopLit
  soplit(bool inverted,
	 tVarId varid,
	 tPol pol) const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

private:
  //////////////////////////////////////////////////////////////////////
  /// @name 参照回数の制御
  /// @{
  
  // 参照回数を1つ増やす．
  void
  inc_ref() const;
  
  // 参照回数を1つ減らす．
  // 結果0になれば自分自身を削除する．
  void
  dec_ref() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // コンストラクタ/デストラクタはむやみに使ってはだめなので
  // private にしている．
public:
  
  // コンストラクタ
  LexpNode();
  
  // デストラクタ
  ~LexpNode();


private:
  
  // 参照回数を得る．
  size_t
  ref() const;

  // 参照回数をセットする．
  void
  ref(size_t ref) const;
  
  // 自殺する．
  void
  suicide();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数＋ノードタイプ(3ビット)
  ymuint32 mRefType;

  // 子供の数 もしくは 変数番号
  ymuint32 mNc;
  
  // 子を指すポインタの配列
  const LexpNode* mChildArray[1];

};

/// @relates LexpNode
/// @brief node0 と node1 が式として等価のときに true を返す．
bool
posi_equiv(const LexpNode* node0,
	   const LexpNode* node1);


/// @relates LexpNode
/// @brief node0 と node1 が式として否定の関係にあるときに true を返す．
bool
nega_equiv(const LexpNode* node0,
	   const LexpNode* node1);



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
LexpNode::LexpNode()
{
}

// デストラクタ
inline
LexpNode::~LexpNode()
{
}

inline
tType
LexpNode::type() const
{
  return tType(mRefType & 7);
}

inline
bool
LexpNode::is_zero() const
{
  return type() == kConst0;
}

inline
bool
LexpNode::is_one() const
{
  return type() == kConst1;
}

inline
bool
LexpNode::is_constant() const
{
  // ちょっとキタナイコード．
  return (int(type()) & ~1) == kConst0;
}

inline
bool
LexpNode::is_posiliteral() const
{
  return type() == kPosiLiteral;
}

inline
bool
LexpNode::is_negaliteral() const
{
  return type() == kNegaLiteral;
}

inline
bool
LexpNode::is_literal() const
{
  // ちょっとキタナイコード．
  return (static_cast<int>(type()) & ~1) == kPosiLiteral;
}

inline
bool
LexpNode::is_literal(tPol pol) const
{
  return pol == kPolPosi && type() == kPosiLiteral ||
    pol == kPolNega && type() == kNegaLiteral;
}

inline
tVarId
LexpNode::varid() const
{
  return is_literal() ? static_cast<tVarId>(mNc) : kVarIdMax;
}

inline
bool
LexpNode::is_and() const
{
  return type() == kAnd;
}

inline
bool
LexpNode::is_or() const
{
  return type() == kOr;
}

inline
bool
LexpNode::is_xor() const
{
  return type() == kXor;
}

inline
bool
LexpNode::is_op() const
{
  return type() >= kAnd;
}

inline
size_t
LexpNode::child_num() const
{
  return is_op() ? mNc : 0;
}

inline
const LexpNode*
LexpNode::child(size_t pos) const
{
  assert_cond(pos < child_num(), __FILE__, __LINE__);
  return mChildArray[pos];
}

inline
size_t
LexpNode::ref() const
{
  return static_cast<size_t>(mRefType >> 3);
}

inline
void
LexpNode::ref(size_t ref) const
{
  LexpNode* node = const_cast<LexpNode*>(this);
  // 昔の参照回数を落とす．
  node->mRefType &= 7;
  // ref をセットする．
  node->mRefType |= (ref << 3);
}

inline
void
LexpNode::inc_ref() const
{
  // MAX の時は増やさない．
  if ( ref() < kRefMax ) {
    LexpNode* node = const_cast<LexpNode*>(this);
    node->mRefType += 8;
  }
}

inline
void
LexpNode::dec_ref() const
{
  size_t r = ref();
  // MAX の時は減らさない．
  if ( r < kRefMax ) {
    LexpNode* node = const_cast<LexpNode*>(this);
    node->mRefType -= 8;
    if ( r == 1 ) {
      node->suicide();
    }
  }
}

END_NAMESPACE_YM_LEXP

#endif // LIBYM_LEXP_LEXPNODE_H
