#ifndef LIBYM_TECHMAP_PATGEN_PGFUNCREP_H
#define LIBYM_TECHMAP_PATGEN_PGFUNCREP_H

/// @file libym_techmap/patgen/PgFuncRep.h
/// @brief PgFuncRep のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "ym_npn/npn_nsdef.h"
#include "ym_npn/TvFunc.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PgFunc;

//////////////////////////////////////////////////////////////////////
/// @class PgFuncRep PgFuncRep.h "PgFuncRep.h"
/// @brief NPN同値類の代表関数を表すクラス
//////////////////////////////////////////////////////////////////////
class PgFuncRep
{
  friend class PgFuncMgr;
private:

  /// @brief コンストラクタ
  PgFuncRep();

  /// @brief デストラクタ
  ~PgFuncRep();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 代表関数を返す．
  const TvFunc&
  rep_func() const;

  /// @brief 自分の同値類に属する関数の数を返す．
  ymuint
  func_num() const;

  /// @brief 関数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
  const PgFunc*
  func(ymuint pos) const;

  /// @brief パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタン番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < pat_num() )
  ymuint
  pat_id(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 代表関数
  TvFunc mFunc;

  // 関数のリスト
  vector<PgFunc*> mFuncList;

  // パタン番号のリスト
  vector<ymuint32> mPatList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
PgFuncRep::PgFuncRep()
{
}

// @brief デストラクタ
inline
PgFuncRep::~PgFuncRep()
{
}

// @brief ID番号を返す．
inline
ymuint
PgFuncRep::id() const
{
  return mId;
}

// @brief 代表関数を返す．
inline
const TvFunc&
PgFuncRep::rep_func() const
{
  return mFunc;
}

// @brief 自分の同値類に属する関数の数を返す．
inline
ymuint
PgFuncRep::func_num() const
{
  return mFuncList.size();
}

// @brief 関数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
inline
const PgFunc*
PgFuncRep::func(ymuint pos) const
{
  return mFuncList[pos];
}

// @brief パタン数を返す．
inline
ymuint
PgFuncRep::pat_num() const
{
  return mPatList.size();
}

// @brief パタン番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < pat_num() )
inline
ymuint
PgFuncRep::pat_id(ymuint pos) const
{
  return mPatList[pos];
}

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGFUNCREP_H
