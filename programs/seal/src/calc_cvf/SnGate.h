#ifndef CALC_CVF_SNGATE_H
#define CALC_CVF_SNGATE_H

/// @file calc_cvf/SnGate.h
/// @brief SnGate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnGate.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "SimNode.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
/// @class SnGate SimNode.h
/// @brief 多入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class SnGate :
  public SimNode
{
protected:

  /// @brief コンストラクタ
  SnGate(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnGate();


public:

  /// @brief ファンイン数を得る．
  virtual
  size_t
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(size_t pos) const;
  

protected:

  /// @brief ランダムな順序を作る．
  /// @param[in] randgen 乱数発生器
  /// @note 結果は mOrder に入る．
  void
  random_order(RandGen& randgen);
  
  
protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  size_t mNfi;
  
  // ファンインの配列
  SimNode** mFanins;
  
  // ランダムな順序をいれておく配列
  size_t* mOrder;
  
  // calc_iobs で用いる作業領域
  tPackedVal* mTmp;

  // calc_iobsfunc で用いる作業領域
  Bdd* mTmpFunc;
  
};


//////////////////////////////////////////////////////////////////////
/// @class SnGate1 SimNode.h
/// @brief 1入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class SnGate1 :
  public SimNode
{
protected:

  /// @brief コンストラクタ
  SnGate1(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnGate1();


public:

  /// @brief ファンイン数を得る．
  virtual
  size_t
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(size_t pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ファンイン
  SimNode* mFanin;

};


//////////////////////////////////////////////////////////////////////
/// @class SnGate2 SimNode.h
/// @brief 2入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class SnGate2 :
  public SimNode
{
protected:

  /// @brief コンストラクタ
  SnGate2(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnGate2();


public:

  /// @brief ファンイン数を得る．
  virtual
  size_t
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(size_t pos) const;
  
  
protected:
  
  /// @brief ランダムな順序を作る．
  /// @param[in] randgen 乱数発生器
  /// @note 結果は mOrder に入る．
  void
  random_order(RandGen& randgen);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ファンインの配列
  SimNode* mFanins[2];
  
  // ランダムな順序をいれておく配列
  size_t mOrder[2];
  
};


//////////////////////////////////////////////////////////////////////
/// @class SnGate3 SimNode.h
/// @brief 3入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class SnGate3 :
  public SimNode
{
protected:

  /// @brief コンストラクタ
  SnGate3(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnGate3();


public:

  /// @brief ファンイン数を得る．
  virtual
  size_t
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(size_t pos) const;
  
  
protected:
  
  /// @brief ランダムな順序を作る．
  /// @param[in] randgen 乱数発生器
  /// @note 結果は mOrder に入る．
  void
  random_order(RandGen& randgen);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ファンインの配列
  SimNode* mFanins[3];
  
  // ランダムな順序をいれておく配列
  size_t mOrder[3];
  
};


//////////////////////////////////////////////////////////////////////
/// @class SnGate4 SimNode.h
/// @brief 4入力ゲートの基底クラス
//////////////////////////////////////////////////////////////////////
class SnGate4 :
  public SimNode
{
protected:

  /// @brief コンストラクタ
  SnGate4(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnGate4();


public:

  /// @brief ファンイン数を得る．
  virtual
  size_t
  nfi() const;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(size_t pos) const;
  
  
protected:
  
  /// @brief ランダムな順序を作る．
  /// @param[in] randgen 乱数発生器
  /// @note 結果は mOrder に入る．
  void
  random_order(RandGen& randgen);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ファンインの配列
  SimNode* mFanins[4];
  
  // ランダムな順序をいれておく配列
  size_t mOrder[4];
  
};

END_NAMESPACE_YM_SEAL_CVF

#endif // CALC_CVF_SNGATE_H
