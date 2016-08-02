#ifndef LIBYM_MVN_MVARITHOP_H
#define LIBYM_MVN_MVARITHOP_H

/// @file libym_mvn/MvArithOp.h
/// @brief MvArithOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvAdd MvArithOp.h "MvArithOp.h"
/// @brief ADD 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvAdd :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvAdd(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvAdd();

};


//////////////////////////////////////////////////////////////////////
/// @class MvSub MvArithOp.h "MvArithOp.h"
/// @brief SUB 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvSub :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvSub(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvSub();

};


//////////////////////////////////////////////////////////////////////
/// @class MvMult MvArithOp.h "MvArithOp.h"
/// @brief MULT 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvMult :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvMult(MvModule* module,
	 ymuint bit_width1,
	 ymuint bit_width2,
	 ymuint bit_width3);

  /// @brief デストラクタ
  ~MvMult();

};


//////////////////////////////////////////////////////////////////////
/// @class MvDiv MvArithOp.h "MvArithOp.h"
/// @brief DIV 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvDiv :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvDiv(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvDiv();

};


//////////////////////////////////////////////////////////////////////
/// @class MvMod MvArithOp.h "MvArithOp.h"
/// @brief MOD 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvMod :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvMod(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvMod();

};


//////////////////////////////////////////////////////////////////////
/// @class MvPow MvArithOp.h "MvArithOp.h"
/// @brief POW 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvPow :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvPow(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvPow();

};


//////////////////////////////////////////////////////////////////////
/// @class MvSll MvArithOp.h "MvArithOp.h"
/// @brief SLL 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvSll :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvSll(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvSll();

};


//////////////////////////////////////////////////////////////////////
/// @class MvSrl MvArithOp.h "MvArithOp.h"
/// @brief SRL 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvSrl :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvSrl(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvSrl();

};


//////////////////////////////////////////////////////////////////////
/// @class MvSla MvArithOp.h "MvArithOp.h"
/// @brief Sla 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvSla :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvSla(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvSla();

};


//////////////////////////////////////////////////////////////////////
/// @class MvSra MvArithOp.h "MvArithOp.h"
/// @brief SRA 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvSra :
  public MvBinaryOp
{
  friend class MvMgr;
  
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvSra(MvModule* module,
	ymuint bit_width1,
	ymuint bit_width2,
	ymuint bit_width3);

  /// @brief デストラクタ
  ~MvSra();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVARITHOP_H
