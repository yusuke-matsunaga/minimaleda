#ifndef CALC_SVF_SNAND_H
#define CALC_SVF_SNAND_H

/// @file calc_svf/SnAnd.h
/// @brief SnAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnAnd.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "SnGate.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

//////////////////////////////////////////////////////////////////////
/// @class SnAnd SimNode.h
/// @brief ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd :
  public SnGate
{
public:

  /// @brief コンストラクタ
  SnAnd(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobs();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobs();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;
  
};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd2 SimNode.h
/// @brief 2入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd2 :
  public SnGate2
{
public:

  /// @brief コンストラクタ
  SnAnd2(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd2();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobs();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobs();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd3 SimNode.h
/// @brief 3入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd3 :
  public SnGate3
{
public:

  /// @brief コンストラクタ
  SnAnd3(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd3();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobs();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobs();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd4 SimNode.h
/// @brief 4入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd4 :
  public SnGate4
{
public:

  /// @brief コンストラクタ
  SnAnd4(ymuint32 id,
	 const vector<SimNode*>& inputs);
  
  /// @brief デストラクタ
  virtual
  ~SnAnd4();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobs();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobs();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand SimNode.h
/// @brief NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand :
  public SnAnd
{
public:

  /// @brief コンストラクタ
  SnNand(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand2 SimNode.h
/// @brief 2入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand2 :
  public SnAnd2
{
public:

  /// @brief コンストラクタ
  SnNand2(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand2();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand3 SimNode.h
/// @brief 3入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand3 :
  public SnAnd3
{
public:

  /// @brief コンストラクタ
  SnNand3(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand3();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand4 SimNode.h
/// @brief 4入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand4 :
  public SnAnd4
{
public:

  /// @brief コンストラクタ
  SnNand4(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand4();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SEAL_SVF

#endif // CALC_SVF_SNAND_H
