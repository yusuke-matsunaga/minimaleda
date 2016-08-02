#ifndef BASE_SAFAULT_H
#define BASE_SAFAULT_H

/// @file src/base/SaFault.h
/// @brief SaFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SaFault.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "atpg_nsdef.h"
#include "Val3.h"
#include <ym_tgnet/tgnet_nsdef.h>


BEGIN_NAMESPACE_YM_ATPG

class FaultMgr;
class SaFault;
class TestVector;

//////////////////////////////////////////////////////////////////////
/// @brief 故障の状態
//////////////////////////////////////////////////////////////////////
enum FaultStatus {
  /// @brief 未検出
  kFsUndetected,
  /// @brief 検出
  kFsDetected,
  /// @brief テスト不能 (冗長)
  kFsUntestable,
  /// @brief アボート
  kFsAbort
};


//////////////////////////////////////////////////////////////////////
/// @class Finfo Finfo.h "Finfo.h"
/// @brief 等価故障グループに共通の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class Finfo
{
  friend class FaultMgr;
  
private:

  /// @brief コンストラクタ
  Finfo();

  /// @brief デストラクタ
  ~Finfo();


public:

  /// @brief 検出パタン数を返す．
  ymuint
  pat_num() const;
  
  /// @brief 検出パタンを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < pat_num() )
  TestVector*
  pat(ymuint pos = 0) const;

  /// @brief 代表故障を返す．
  SaFault*
  rep() const;
  
  /// @brief 等価な故障のリストを返す．
  const vector<SaFault*>&
  eq_faults() const;
  
  /// @brief 状態を返す．
  FaultStatus
  status() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内容を書き換えるメソッド
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 検出パタンをクリアする．
  void
  clear_testpat();
  
  /// @brief 検出パタンを追加する．
  void
  add_testpat(TestVector* testpat);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 検出したパタンのリスト
  vector<TestVector*> mPatList;

  // 等価故障のリスト
  vector<SaFault*> mEqFaults;
  
  // 状態
  FaultStatus mStatus;
  
};


//////////////////////////////////////////////////////////////////////
/// @class SaFault SaFault.h "SaFault.h"
/// @brief 単一縮退故障を表すクラス
//////////////////////////////////////////////////////////////////////
class SaFault
{
  friend class FaultMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] node 対象のゲート
  /// @param[in] output 入力の故障のとき false, 出力の故障のとき true を与える．
  /// @param[in] pos 故障位置
  /// @param[in] val 故障値 0 か非0 で 0/1 を区別する．
  SaFault(const TgNode* node,
	  bool output,
	  ymuint pos,
	  int val);

  /// @brief デストラクタ
  ~SaFault();


public:
  //////////////////////////////////////////////////////////////////////
  // read-only のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象のゲートを返す．
  const TgNode*
  node() const;
  
  /// @brief 故障の入力側のゲートを返す．
  const TgNode*
  source_node() const;
  
  /// @brief 入力の故障の時 true を返す．
  bool
  is_input_fault() const;

  /// @brief 出力の故障の時 true を返す．
  bool
  is_output_fault() const;

  /// @brief 故障位置を返す．
  ymuint
  pos() const;

  /// @brief 故障値を返す．
  /// @note 返す値は 0 か 1
  int
  val() const;
  
  /// @brief 故障値を3値型で返す．
  Val3
  val3() const;
  
  /// @brief 故障の内容を表す文字列を返す．
  string
  str() const;

  /// @brief 代表故障を返す．
  SaFault*
  rep() const;
  
  /// @brief 状態を返す．
  FaultStatus
  status() const;
  
  /// @brief 検出パタン数を返す．
  ymuint
  pat_num() const;
  
  /// @brief 検出パタンを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < pat_num() )
  TestVector*
  pat(ymuint pos = 0) const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のノード
  const TgNode* mNode;

  // 故障位置と故障値をパックしたもの
  ymuint32 mPosVal;

  // 等価故障グループに共通の情報
  Finfo* mFinfo;
  
};

/// @relates SaFault
/// @brief ストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] f 故障
ostream&
operator<<(ostream& s,
	   const SaFault* f);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
Finfo::Finfo() :
  mStatus(kFsUndetected)
{
}

// @brief デストラクタ
inline
Finfo::~Finfo()
{
}

// @brief 検出パタン数を返す．
inline
ymuint
Finfo::pat_num() const
{
  return mPatList.size();
}
  
// @brief 検出パタンを返す．
// @param[in] pos 位置番号 ( 0 <= pos < det_num() )
inline
TestVector*
Finfo::pat(ymuint pos) const
{
  return mPatList[pos];
}
    
// @brief 検出パタンをクリアする．
inline
void
Finfo::clear_testpat()
{
  mPatList.clear();
}

// @brief 検出パタンを追加する．
inline
void
Finfo::add_testpat(TestVector* testpat)
{
  mPatList.push_back(testpat);
}

// @brief 代表故障を返す．
inline
SaFault*
Finfo::rep() const
{
  return mEqFaults.front();
}

// @brief 等価な故障のリストを返す．
inline
const vector<SaFault*>&
Finfo::eq_faults() const
{
  return mEqFaults;
}
  
// @brief 状態を返す．
inline
FaultStatus
Finfo::status() const
{
  return mStatus;
}

// @brief 対象のゲートを返す．
inline
const TgNode*
SaFault::node() const
{
  return mNode;
}

// @brief 入力の故障の時 true を返す．
inline
bool
SaFault::is_input_fault() const
{
  return !is_output_fault();
}

// @brief 出力の故障の時 true を返す．
inline
bool
SaFault::is_output_fault() const
{
  return static_cast<bool>((mPosVal >> 1) & 1UL);
}

// @brief 故障位置を返す．
inline
ymuint
SaFault::pos() const
{
  return mPosVal >> 3;
}

// @brief 故障値を返す．
// @note 返す値は 0 か 1
inline
int
SaFault::val() const
{
  return static_cast<int>(mPosVal & 1UL);
}
  
// @brief 故障値を3値型で返す．
inline
Val3
SaFault::val3() const
{
  if ( val() ) {
    return kVal1;
  }
  else {
    return kVal0;
  }
}

// @brief 代表故障を返す．
inline
SaFault*
SaFault::rep() const
{
  return mFinfo->rep();
}

// @brief 状態を返す．
inline
FaultStatus
SaFault::status() const
{
  return mFinfo->status();
}

// @brief 検出パタン数を返す．
inline
ymuint
SaFault::pat_num() const
{
  return mFinfo->pat_num();
}

// @brief 検出パタンを返す．
// @param[in] pos 位置番号 ( 0 <= pos < pat_num() )
inline
TestVector*
SaFault::pat(ymuint pos) const
{
  return mFinfo->pat(pos);
}

END_NAMESPACE_YM_ATPG

#endif // ATPG_SAFAULT_H
