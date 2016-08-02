#ifndef YM_MISLIB_MISLIBHANDLER_H
#define YM_MISLIB_MISLIBHANDLER_H

/// @file ym_mislib/MislibHandler.h
/// @brief MislibHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibHandler.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_mislib/mislib_nsdef.h>

#include <ym_utils/ShString.h>
#include <ym_utils/FileRegion.h>
#include <ym_utils/MsgHandler.h>
#include <ym_lexp/LogExpr.h>


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibPin MislibHandler.h <ym_mislib/MislibHandler.h>
/// @ingreoup MislibGroup
/// @brief MislibHandler::gate() 中で使われるピンの情報を保持するクラス
//////////////////////////////////////////////////////////////////////
class MislibPin
{
  friend class MislibParserImpl;
  
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief 極性情報を表す列挙型
  //////////////////////////////////////////////////////////////////////
  enum tPhase {
    /// @brief 反転なし (立ち上がり -> 立ち上がり)
    kNoninv,
    /// @brief 反転あり (立ち上がり -> 立ち下がり)
    kInv,
    /// @brief 両方あり
    kUnknown
  };
  

public:

  /// @brief コンストラクタ
  MislibPin();

  /// @brief デストラクタ
  ~MislibPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名を得る．
  ShString
  name() const;

  /// @brief 極性情報を得る．
  tPhase
  phase() const;

  /// @brief 入力負荷を得る．
  double
  input_load() const;

  /// @brief 最大駆動負荷を得る．
  double
  max_load() const;

  /// @brief 立ち上がり固定遅延を得る．
  double
  rise_block_delay() const;

  /// @brief 立ち上がり負荷依存遅延を得る．
  double
  rise_fanout_delay() const;

  /// @brief 立ち下がり固定遅延を得る．
  double
  fall_block_delay() const;

  ///  @brief 立ち下がり負荷依存遅延を得る．
  double
  fall_fanout_delay() const;
  
  /// @brief 全体の位置情報を得る．
  FileRegion
  loc() const;

  /// @brief ピン名の位置情報を得る．
  FileRegion
  name_loc() const;

  /// @brief 極性の位置情報を得る．
  FileRegion
  phase_loc() const;

  /// @brief 入力負荷の位置情報を得る．
  FileRegion
  input_load_loc() const;

  /// @brief 最大負荷の位置情報を得る．
  FileRegion
  max_load_loc() const;

  /// @brief 立ち上がり固定遅延の位置情報を得る．
  FileRegion
  rise_block_delay_loc() const;

  /// @brief 立ち上がり負荷依存遅延の位置情報を得る．
  FileRegion
  rise_fanout_delay_loc() const;

  /// @brief 立ち下がり固定遅延の位置情報を得る．
  FileRegion
  fall_block_delay_loc() const;

  /// @brief 立ち下がり負荷依存遅延の位置情報を得る．
  FileRegion
  fall_fanout_delay_loc() const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ピン名
  ShString mName;
  
  // 極性情報
  tPhase mPhase;

  // 入力負荷
  double mInputLoad;

  // 最大駆動負荷
  double mMaxLoad;

  // 立ち上がり固定遅延
  double mRiseBlockDelay;

  // 立ち上がり負荷依存遅延
  double mRiseFanoutDelay;

  // 立ち下がり固定遅延
  double mFallBlockDelay;

  // 立ち下がり負荷依存遅延
  double mFallFanoutDelay;
  
  // 位置情報の配列
  FileRegion mLocArray[9];
  
};


//////////////////////////////////////////////////////////////////////
/// @class MislibHandler MislibHandler.h <ym_mislib/MislibHandler.h>
/// @ingroup MislibGroup
/// @brief mislib パーサーのイベントハンドラの基底クラス
/// 純粋仮想関数を持っているので，このクラスを継承し，仮想関数を上書き
/// する必要がある．
/// @sa MislibParser
//////////////////////////////////////////////////////////////////////
class MislibHandler
{
  friend class MislibParserImpl;
  
protected:

  /// @brief コンストラクタ
  MislibHandler();
  
  /// @brief デストラクタ
  virtual
  ~MislibHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは必要に応じて下記の仮想関数を上書きすること
  // デフォルトの実装はなにもしない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  virtual
  void
  init();

  /// @brief GATE 文の処理
  /// @param[in] loc GATE 文の位置情報
  /// @param[in] name 名前
  /// @param[in] name_loc 名前の位置情報
  /// @param[in] area 面積
  /// @param[in] area_loc 面積の位置情報
  /// @param[in] oname 出力ピン名
  /// @param[in] oname_loc 出力ピン名の位置情報
  /// @param[in] expr 論理式
  /// @param[in] expr_loc 論理式の位置情報
  /// @param[in] ipin_list 入力ピンの情報の配列
  virtual
  void
  gate(const FileRegion& loc,
       ShString name,
       const FileRegion& name_loc,
       double area,
       const FileRegion& area_loc,
       ShString oname,
       const FileRegion& oname_loc,
       const LogExpr& expr,
       const FileRegion& expr_loc,
       const vector<MislibPin>& ipin_list);

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが利用することのできる関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief メッセージマネージャの取得
  MsgMgr&
  msg_mgr();

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサー本体
  MislibParserImpl* mParser;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ピン名を得る．
inline
ShString
MislibPin::name() const
{
  return mName;
}

// @brief 極性情報を得る．
inline
MislibPin::tPhase
MislibPin::phase() const
{
  return mPhase;
}

// @brief 入力負荷を得る．
inline
double
MislibPin::input_load() const
{
  return mInputLoad;
}

// @brief 最大駆動負荷を得る．
inline
double
MislibPin::max_load() const
{
  return mMaxLoad;
}

// @brief 立ち上がり固定遅延を得る．
inline
double
MislibPin::rise_block_delay() const
{
  return mRiseBlockDelay;
}

// @brief 立ち上がり負荷依存遅延を得る．
inline
double
MislibPin::rise_fanout_delay() const
{
  return mRiseFanoutDelay;
}

// @brief 立ち下がり固定遅延を得る．
inline
double
MislibPin::fall_block_delay() const
{
  return mFallBlockDelay;
}

//  @brief 立ち下がり負荷依存遅延を得る．
inline
double
MislibPin::fall_fanout_delay() const
{
  return mFallFanoutDelay;
}
  
// @brief 全体の位置情報を得る．
inline
FileRegion
MislibPin::loc() const
{
  return mLocArray[0];
}

// @brief ピン名の位置情報を得る．
inline
FileRegion
MislibPin::name_loc() const
{
  return mLocArray[1];
}

// @brief 極性の位置情報を得る．
inline
FileRegion
MislibPin::phase_loc() const
{
  return mLocArray[2];
}

// @brief 入力負荷の位置情報を得る．
inline
FileRegion
MislibPin::input_load_loc() const
{
  return mLocArray[3];
}

// @brief 最大負荷の位置情報を得る．
inline
FileRegion
MislibPin::max_load_loc() const
{
  return mLocArray[4];
}

// @brief 立ち上がり固定遅延の位置情報を得る．
inline
FileRegion
MislibPin::rise_block_delay_loc() const
{
  return mLocArray[5];
}

// @brief 立ち上がり負荷依存遅延の位置情報を得る．
inline
FileRegion
MislibPin::rise_fanout_delay_loc() const
{
  return mLocArray[6];
}

// @brief 立ち下がり固定遅延の位置情報を得る．
inline
FileRegion
MislibPin::fall_block_delay_loc() const
{
  return mLocArray[7];
}

// @brief 立ち下がり負荷依存遅延の位置情報を得る．
inline
FileRegion
MislibPin::fall_fanout_delay_loc() const
{
  return mLocArray[8];
}

END_NAMESPACE_YM_MISLIB

#endif // YM_MISLIB_MISLIBHANDLER_H
