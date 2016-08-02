#ifndef CALC_CVF_SNOR_H
#define CALC_CVF_SNOR_H

/// @file calc_cvf/SnOr.h
/// @brief SnOr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SnOr.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "SnGate.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

//////////////////////////////////////////////////////////////////////
/// @class SnOr SimNode.h
/// @brief ORノード
//////////////////////////////////////////////////////////////////////
class SnOr :
  public SnGate
{
public:

  /// @brief コンストラクタ
  SnOr(ymuint32 id,
       const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnOr();


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
  calc_max_iobs(RandGen& randgen);
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobsfunc();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobsfunc();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobsfunc(const Bdd& obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnOr2 SimNode.h
/// @brief 2入力ORノード
//////////////////////////////////////////////////////////////////////
class SnOr2 :
  public SnGate2
{
public:

  /// @brief コンストラクタ
  SnOr2(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnOr2();


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
  calc_max_iobs(RandGen& randgen);
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobsfunc();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobsfunc();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobsfunc(const Bdd& obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnOr3 SimNode.h
/// @brief 3入力ORノード
//////////////////////////////////////////////////////////////////////
class SnOr3 :
  public SnGate3
{
public:

  /// @brief コンストラクタ
  SnOr3(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnOr3();


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
  calc_max_iobs(RandGen& randgen);
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobsfunc();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobsfunc();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobsfunc(const Bdd& obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnOr4 SimNode.h
/// @brief 4入力ORノード
//////////////////////////////////////////////////////////////////////
class SnOr4 :
  public SnGate4
{
public:

  /// @brief コンストラクタ
  SnOr4(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnOr4();


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
  calc_max_iobs(RandGen& randgen);
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobs(tPackedVal obs);

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();

  /// @brief 入力の擬似最小 obs を計算する．
  virtual
  void
  calc_pseudo_min_iobsfunc();
  
  /// @brief 入力の最大 obs を計算する．
  virtual
  void
  calc_max_iobsfunc();
  
  /// @brief 入力の obs を計算する．
  virtual
  void
  _calc_iobsfunc(const Bdd& obs);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNor SimNode.h
/// @brief NORノード
//////////////////////////////////////////////////////////////////////
class SnNor :
  public SnOr
{
public:

  /// @brief コンストラクタ
  SnNor(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNor();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNor2 SimNode.h
/// @brief 2入力NORノード
//////////////////////////////////////////////////////////////////////
class SnNor2 :
  public SnOr2
{
public:

  /// @brief コンストラクタ
  SnNor2(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNor2();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNor3 SimNode.h
/// @brief 3入力NORノード
//////////////////////////////////////////////////////////////////////
class SnNor3 :
  public SnOr3
{
public:

  /// @brief コンストラクタ
  SnNor3(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNor3();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNor2 SimNode.h
/// @brief 2入力NORノード
//////////////////////////////////////////////////////////////////////
class SnNor4 :
  public SnOr4
{
public:

  /// @brief コンストラクタ
  SnNor4(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNor4();


public:

  /// @brief 正常値の計算を行う．
  virtual
  tPackedVal
  _calc_gval();

  /// @brief 故障値の計算を行う．
  virtual
  tPackedVal
  _calc_fval();

  /// @brief 正常値の計算を行う．
  virtual
  Bdd
  _calc_gfunc();

  /// @brief 故障値の計算を行う．
  virtual
  Bdd
  _calc_ffunc();
  
  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SEAL_CVF

#endif // CALC_CVF_SNOR_H
