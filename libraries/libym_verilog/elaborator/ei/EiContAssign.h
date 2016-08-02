#ifndef LIBYM_VERILOG_ELABORATOR_EI_EICONTASSIGN_H
#define LIBYM_VERILOG_ELABORATOR_EI_EICONTASSIGN_H

/// @file libym_verilog/elaborator/ei/EiContAssign.h
/// @brief EiContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiContAssign.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbContAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCaHead EiContAssign.h "EiContAssign.h"
/// @brief 継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class EiCaHead :
  public ElbCaHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  EiCaHead(const VlModule* module,
	   const PtItem* pt_head);

  /// @brief デストラクタ
  virtual
  ~EiCaHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  module() const;

  /// @brief 0の強さを返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief 1の強さを返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief 遅延を表す式を返す．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // パース木のヘッダ定義
  const PtItem* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCaHeadD EiContAssign.h "EiContAssign.h"
/// @brief 遅延付きの継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class EiCaHeadD :
  public EiCaHead
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_head パース木のヘッダ定義
  /// @param[in] delay 遅延値
  EiCaHeadD(const VlModule* module,
	    const PtItem* pt_head,
	    ElbDelay* delay);

  /// @brief デストラクタ
  virtual
  ~EiCaHeadD();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延を表す式を返す．
  virtual
  ElbDelay*
  delay() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  ElbDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiContAssign EiContAssign.h "EiContAssign.h"
/// @brief 継続的代入文の実装クラス
//////////////////////////////////////////////////////////////////////
class EiContAssign :
  public ElbContAssign
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  EiContAssign(const PtBase* pt_obj,
	       ElbExpr* lhs,
	       ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiContAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置の取得
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief 左辺を返す．
  virtual
  const VlExpr*
  lhs() const;

  /// @brief 右辺を返す．
  virtual
  const VlExpr*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応するパース木の定義要素
  const PtBase* mPtObj;

  // 左辺式
  ElbExpr* mLhs;

  // 右辺式
  ElbExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiContAssign EiContAssign.h "EiContAssign.h"
/// @brief 継続的代入文の実装クラス
//////////////////////////////////////////////////////////////////////
class EiContAssign1 :
  public EiContAssign
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_obj 対応するパース木中の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  EiContAssign1(ElbCaHead* head,
		const PtBase* pt_obj,
		ElbExpr* lhs,
		ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiContAssign1();


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  module() const;

  /// @brief 0の強さを返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief 1の強さを返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief 遅延を表す式を返す．
  virtual
  const VlDelay*
  delay() const;

  /// @brief ネット宣言中の assignment の時に true を返す．
  virtual
  bool
  has_net_decl_assign() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbCaHead* mHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiContAssign2 EiContAssign.h "EiContAssign.h"
/// @brief net 宣言中の継続的代入文
//////////////////////////////////////////////////////////////////////
class EiContAssign2 :
  public EiContAssign
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pt_obj パース木の定義要素
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  EiContAssign2(const VlModule* module,
		const PtBase* pt_obj,
		ElbExpr* lhs,
		ElbExpr* rhs);

  /// @brief デストラクタ
  virtual
  ~EiContAssign2();


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  module() const;

  /// @brief 0の強さを返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief 1の強さを返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief 遅延を表す式を返す．
  virtual
  const VlDelay*
  delay() const;

  /// @brief ネット宣言中の assignment の時に true を返す．
  virtual
  bool
  has_net_decl_assign() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_ELBCONTASSIGN_H
