#ifndef LIBYM_MISLIB_MISLIBPTIMPL2_H
#define LIBYM_MISLIB_MISLIBPTIMPL2_H

/// @file libym_mislib/MislibPtImpl2.h
/// @brief MislibPt の派生クラスのヘッダファイル(その2)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl2.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPt.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @brief NOT論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtNot :
  public MislibPt
{
  friend class MislibParserImpl;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  MislibPtNot(const FileRegion& loc,
	      MislibPt* child1);

  /// @brief デストラクタ
  virtual
  ~MislibPtNot();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;
  
  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  MislibPt*
  child1() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子供
  MislibPt* mChild1;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief AND論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtAnd :
  public MislibPt
{
  friend class MislibParserImpl;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibPtAnd(const FileRegion& loc,
	      MislibPt* child1,
	      MislibPt* child2);

  /// @brief デストラクタ
  virtual
  ~MislibPtAnd();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;
  
  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  MislibPt*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  virtual
  MislibPt*
  child2() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1番目の子供
  MislibPt* mChild1;

  // 2番目の子供
  MislibPt* mChild2;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief OR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtOr :
  public MislibPt
{
  friend class MislibParserImpl;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibPtOr(const FileRegion& loc,
	     MislibPt* child1,
	     MislibPt* child2);

  /// @brief デストラクタ
  virtual
  ~MislibPtOr();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;
  
  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  MislibPt*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  virtual
  MislibPt*
  child2() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1番目の子供
  MislibPt* mChild1;

  // 2番目の子供
  MislibPt* mChild2;
  
};


//////////////////////////////////////////////////////////////////////
/// @brief ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtPin :
  public MislibPt
{
  friend class MislibParserImpl;
  friend class MislibPtPinList;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] name ピン名
  /// @param[in] phase 極性
  /// @param[in] input_load 入力負荷
  /// @param[in] max_load 最大駆動負荷
  /// @param[in] rise_block_delay 立ち上がり固定遅延
  /// @param[in] rise_fanout_delay 立ち上がり負荷依存遅延
  /// @param[in] fall_block_delay 立ち下がり固定遅延
  /// @param[in] fall_fanout_delay 立ち下がり負荷依存遅延
  MislibPtPin(const FileRegion& loc,
	      MislibPt* name,
	      MislibPt* phase,
	      MislibPt* input_load,
	      MislibPt* max_load,
	      MislibPt* rise_block_delay,
	      MislibPt* rise_fanout_delay,
	      MislibPt* fall_block_delay,
	      MislibPt* fall_fanout_delay);
  
  /// @brief デストラクタ
  virtual
  ~MislibPtPin();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief ピン名を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  name() const;

  /// @brief 極性情報を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  phase() const;

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  input_load() const;

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  max_load() const;

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  rise_block_delay() const;

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  rise_fanout_delay() const;

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  fall_block_delay() const;

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  virtual
  MislibPt*
  fall_fanout_delay() const;

  /// @brief 次のピンを設定する．
  virtual
  void
  set_next(MislibPt* pin);

  /// @brief 次のピンを取り出す．
  virtual
  MislibPt*
  next() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 入力ピン名
  MislibPt* mName;

  // 極性情報
  MislibPt* mPhase;

  // 入力負荷
  MislibPt* mInputLoad;

  // 最大駆動負荷
  MislibPt* mMaxLoad;

  // 立ち上がり固有遅延
  MislibPt* mRiseBlockDelay;

  // 立ち上がり負荷依存遅延係数
  MislibPt* mRiseFanoutDelay;

  // 立ち下がり固有遅延
  MislibPt* mFallBlockDelay;

  // 立ち下がり負荷依存遅延係数
  MislibPt* mFallFanoutDelay;

  // 次のピン
  MislibPt* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @brief ピンのリストを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtPinList :
  public MislibPt
{
  friend class MislibParserImpl;
private:

  /// @brief コンストラクタ
  MislibPtPinList();

  /// @brief デストラクタ
  virtual
  ~MislibPtPinList();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 末尾にピンを追加する．
  virtual
  void
  push_back(MislibPt* pin);

  /// @brief 先頭のピンを取り出す．
  virtual
  MislibPt*
  pin() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のピン
  MislibPt* mTop;

  // 末尾のピン
  MislibPt* mEnd;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBPTIMPL2_H
