#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPARAMASSIGN_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPARAMASSIGN_H

/// @file libym_verilog/elaborator/ei/EiParamAssign.h
/// @brief EiParamAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParamAssign.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbParamAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParamAssign EiParamAssign.h "EiParamAssign.h"
/// @brief ElbParamAssign の実装クラス
//////////////////////////////////////////////////////////////////////
class EiParamAssign :
  public ElbParamAssign
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] param 対象の parameter
  /// @param[in] rhs 割り当て式の右辺
  EiParamAssign(const VlModule* module,
		const PtBase* pt_obj,
		ElbDecl* param,
		ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiParamAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlParamAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  parent() const;

  /// @brief 左辺値を返す．
  virtual
  const VlDecl*
  lhs() const;

  /// @brief 右辺値を返す．
  virtual
  const VlExpr*
  rhs() const;

  /// @brief 名前による接続の場合に true を返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_conn_by_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // パース木中の対応するオブジェクト
  const PtBase* mPtObj;

  // パラメータ
  ElbDecl* mLhs;

  // 右辺式
  ElbExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParamAssign2 EiParamAssign.h "EiParamAssign.h"
/// @brief ElbParamAssign の実装クラス (名前による割り当て)
//////////////////////////////////////////////////////////////////////
class EiParamAssign2 :
  public EiParamAssign
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の接続を表す要素
  /// @param[in] param 対象の parameter
  /// @param[in] rhs 割り当て式の右辺
  EiParamAssign2(const VlModule* module,
		 const PtBase* pt_obj,
		 ElbDecl* param,
		 ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiParamAssign2();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前による接続の場合に true を返す．
  /// @note このクラスでは true を返す．
  virtual
  bool
  is_conn_by_name() const;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDefParam EiParamAssign.h "EiParamAssign.h"
/// @brief ElbDefParam の実装クラス
//////////////////////////////////////////////////////////////////////
class EiDefParam :
  public ElbDefParam
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_header パース木の defparam ヘッダ
  /// @param[in] pt_item パース木の defparam 定義
  /// @param[in] param 対象の parameter
  /// @param[in] rhs 割り当て式の右辺
  EiDefParam(const VlModule* module,
	     const PtItem* pt_header,
	     const PtDefParam* pt_defparam,
	     ElbDecl* param,
	     ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiDefParam();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  parent() const;

  /// @brief 左辺値を返す．
  virtual
  const VlDecl*
  lhs() const;

  /// @brief 右辺値を返す．
  virtual
  const VlExpr*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // パース木の defparam ヘッダ
  const PtItem* mPtHead;

  // パース木の defparam 定義
  const PtDefParam* mPtDefParam;

  // パラメータ
  ElbDecl* mLhs;

  // 右辺式
  ElbExpr* mRhs;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPARAMASSIGN_H
