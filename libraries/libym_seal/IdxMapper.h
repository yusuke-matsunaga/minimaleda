#ifndef LIBYM_SEAL_IDXMAPPER_H
#define LIBYM_SEAL_IDXMAPPER_H

/// @file libym_seal/IdxMapper.h
/// @brief fsm_analysis の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: fsm_analysis.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/seal_nsdef.h"

BEGIN_NAMESPACE_YM_SEAL

//////////////////////////////////////////////////////////////////////
/// @class IdxMapper
/// @brief BDD用の変数インデックスを計算するクラス
//////////////////////////////////////////////////////////////////////
class IdxMapper
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] input_num 外部入力数
  /// @param[in] output_num 外部出力数
  /// @param[in] ff_num FF数
  IdxMapper(ymuint input_num,
	    ymuint output_num,
	    ymuint ff_num);

  /// @brief デストラクタ
  ~IdxMapper();

  
public:

  /// @brief 外部入力の変数インデックスを得る．
  ymuint
  input_idx(ymuint pos) const;

  /// @brief 外部出力の変数インデックスを得る．
  ymuint
  output_idx(ymuint pos) const;
  
  /// @brief 正常回路の現状態の変数インデックスを得る．
  ymuint
  cur_normal_idx(ymuint pos) const;

  /// @brief 正常回路の次状態の変数インデックスを得る．
  ymuint
  next_normal_idx(ymuint pos) const;

  /// @brief 故障回路の現状態の変数インデックスを得る．
  ymuint
  cur_error_idx(ymuint pos) const;

  /// @brief 故障回路の次状態の変数インデックスを得る．
  ymuint
  next_error_idx(ymuint pos) const;
  
  /// @brief 現状態のエラービット
  ymuint
  cur_error_bit() const;

  /// @brief 次状態のエラービット
  ymuint
  next_error_bit() const;

  /// @brief 現状態の等価状態ビット
  ymuint
  cur_ident_bit() const;

  /// @brief 次状態の等価状態ビット
  ymuint
  next_ident_bit() const;
  
  /// @brief 状態からエラーを含んだ状態対を作る．
  State
  make_error_state(State normal_state,
		   ymuint error_pos) const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 外部入力数
  ymuint32 mInputNum;
  
  // 外部出力数
  ymuint32 mOutputNum;

  // FF数
  ymuint32 mFFNum;
  
};

  
// @brief コンストラクタ
// @param[in] input_num 外部入力数
// @param[in] output_num 外部出力数
// @param[in] ff_num FF数
inline
IdxMapper::IdxMapper(ymuint input_num,
		     ymuint output_num,
		     ymuint ff_num) :
  mInputNum(input_num),
  mOutputNum(output_num),
  mFFNum(ff_num)
{
}

// @brief デストラクタ
inline
IdxMapper::~IdxMapper()
{
}

// @brief 外部入力の変数インデックスを得る．
inline
ymuint
IdxMapper::input_idx(ymuint pos) const
{
  return mOutputNum + mFFNum * 4 + 4 + pos;
}

// @brief 外部出力の変数インデックスを得る．
inline
ymuint
IdxMapper::output_idx(ymuint pos) const
{
  return pos;
}
  
// @brief 正常回路の現状態の変数インデックスを得る．
inline
ymuint
IdxMapper::cur_normal_idx(ymuint pos) const
{
  return mOutputNum + pos * 2 + 0;
}

// @brief 正常回路の次状態の変数インデックスを得る．
inline
ymuint
IdxMapper::next_normal_idx(ymuint pos) const
{
  return mOutputNum + pos * 2 + 1;
}

// @brief 故障回路の現状態の変数インデックスを得る．
inline
ymuint
IdxMapper::cur_error_idx(ymuint pos) const
{
  return mOutputNum + mFFNum * 2 + pos * 2 + 0;
}

// @brief 故障回路の次状態の変数インデックスを得る．
inline
ymuint
IdxMapper::next_error_idx(ymuint pos) const
{
  return mOutputNum + mFFNum * 2 + pos * 2 + 1;
}
  
// @brief 現状態のエラービット
inline
ymuint
IdxMapper::cur_error_bit() const
{
  return mOutputNum + mFFNum * 4;
}

// @brief 次状態のエラービット
inline
ymuint
IdxMapper::next_error_bit() const
{
  return mOutputNum + mFFNum * 4 + 1;
}

// @brief 現状態の等価状態ビット
inline
ymuint
IdxMapper::cur_ident_bit() const
{
  return mOutputNum + mFFNum * 4 + 2;
}

// @brief 次状態の等価状態ビット
inline
ymuint
IdxMapper::next_ident_bit() const
{
  return mOutputNum + mFFNum * 4 + 3;
}
  
// @brief 状態からエラーを含んだ状態対を作る．
inline
State
IdxMapper::make_error_state(State normal_state,
			    ymuint error_pos) const
{
  State error_state = normal_state;
  assert_cond( error_pos < error_state.size(), __FILE__, __LINE__);
  if ( error_state[error_pos] == '1' ) {
    error_state[error_pos] = '0';
  }
  else {
    error_state[error_pos] = '1';
  }
  return normal_state + error_state + "00";
}

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_IDXMAPPER_H
