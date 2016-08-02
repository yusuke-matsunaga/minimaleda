#ifndef YM_LEXP_LEXP_H
#define YM_LEXP_LEXP_H

/// @file ym_lexp/lexp.h
/// @brief ym_lexp パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lexp.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup LexpGroup 論理式を扱うクラス
///
/// 論理式(ファクタードフォーム)を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．


#include "ymtools.h"


/// @brief ymlexp 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LEXP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLexp)

/// @brief ymlexp 用の名前空間の終了
#define END_NAMESPACE_YM_LEXP \
END_NAMESPACE(nsLexp) \
END_NAMESPACE_YM

/// @namespace nsYm::nsLexp
/// @brief LogExpr 関係のクラスの属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．

BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
/// @ingroup LexpGroup
/// @brief 極性を表す型
//////////////////////////////////////////////////////////////////////
enum tPol {
  /// @brief 肯定(正極性)
  kPolPosi = 0,
  /// @brief 否定(負極性)
  kPolNega = 1
};

/// @ingroup LexpGroup
/// @{

/// @brief 2つの極性の合成
/// @param[in] pol1, pol2 2つの極性
/// @return pol1 と pol2 を合成したもの
inline
tPol
operator*(tPol pol1,
	  tPol pol2)
{
  return tPol( int(pol1) ^ int(pol2) );
}

/// @brief 極性の否定
/// @param[in] pol 入力の極性
/// @return pol の否定
inline
tPol
operator~(tPol pol)
{
  return operator*(pol, kPolNega);
}

/// @brief ostream に対する書出し
/// @param[in] s 出力ストリーム
/// @param[in] pol 極性
/// @return s をそのまま返す．
inline
ostream&
operator<<(ostream& s,
	   tPol pol)
{
  return s << (( pol == kPolPosi ) ? "Posi" : "Nega");
}

/// @}


//////////////////////////////////////////////////////////////////////
/// @ingroup LexpGroup
/// @brief 変数番号を表す型
/// @note 範囲は 0 〜 kVarIdMax
//////////////////////////////////////////////////////////////////////
typedef ymuint32 tVarId;

//////////////////////////////////////////////////////////////////////
/// @ingroup LexpGroup
/// @relates tVarId
/// @brief 変数番号の最大値
/// @note 極性をパックすることを考えて 1 ビット減らしている．
//////////////////////////////////////////////////////////////////////
const tVarId kVarIdMax = (1UL << (sizeof(tVarId) * 8 - 1)) - 1;


//////////////////////////////////////////////////////////////////////
// tVarId を要素とするコンテナクラスの定義
//////////////////////////////////////////////////////////////////////

/// @ingroup LexpGroup
/// @brief 変数番号のベクタ
typedef vector<tVarId> VarVector;

/// @ingroup LexpGroup
/// @brief 変数番号のリスト
typedef list<tVarId> VarList;

/// @ingroup LexpGroup
/// @brief 変数番号から変数番号への写像 (連想配列)
typedef hash_map<tVarId, tVarId> VarVarMap;

/// @ingroup LexpGroup
/// @brief 文字列から変数番号への写像 (連想配列)
typedef hash_map<string, tVarId> StrVarMap;

/// @ingroup LexpGroup
/// @brief 変数番号から文字列への写像 (連想配列)
typedef hash_map<tVarId, string> VarStrMap;


//////////////////////////////////////////////////////////////////////
// LogExpr の先行宣言
//////////////////////////////////////////////////////////////////////

class LogExpr;

//////////////////////////////////////////////////////////////////////
// LogExpr を要素とするコンテナクラスの定義
//////////////////////////////////////////////////////////////////////

/// @ingroup LexpGroup
/// @brief LogExpr のベクタ
typedef vector<LogExpr> LogExprVector;

/// @ingroup LexpGroup
/// @brief LogExpr のリスト
typedef list<LogExpr> LogExprList;

/// @ingroup LexpGroup
/// @brief 変数番号から LogExpr への写像 (連想配列)
typedef hash_map<tVarId, LogExpr> VarLogExprMap;


END_NAMESPACE_YM_LEXP


BEGIN_NAMESPACE_YM

using nsLexp::LogExpr;

using nsLexp::LogExprVector;
using nsLexp::LogExprList;
using nsLexp::VarLogExprMap;

using nsLexp::tPol;
using nsLexp::kPolPosi;
using nsLexp::kPolNega;
using nsLexp::tVarId;
using nsLexp::kVarIdMax;
using nsLexp::VarVector;
using nsLexp::VarList;
using nsLexp::VarVarMap;
using nsLexp::StrVarMap;
using nsLexp::VarStrMap;

END_NAMESPACE_YM

#endif // YM_LEXP_LEXP_H
