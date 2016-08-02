#ifndef LIBYM_VERILOG_ELABORATOR_ELBENV_H
#define LIBYM_VERILOG_ELABORATOR_ELBENV_H

/// @file libym_verilog/elaborator/ElbEnv.h
/// @brief ElbEnv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbEnv.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlNamedObj;

//////////////////////////////////////////////////////////////////////
/// @class ElbEnv ElbEnv.h "ElbEnv.h"
/// @brief Elaboration 時の環境を表すクラス
///
/// 具体的には
/// - 定数式
/// - 関数内の式
/// - 定数関数内の式
/// - システム関数/システムタスクの引数
/// - イベント式
/// - net 型の左辺式
/// - reg/variables 型の左辺式
/// - procedural continuous assignment 文の左辺式
/// - force 文の左辺式
/// の種類がある．
/// 実際にはそれぞれの環境に対応した派生クラスを用いる．
//////////////////////////////////////////////////////////////////////
class ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbEnv();

  /// @brief コピーコンストラクタ
  ElbEnv(const ElbEnv& src);

  /// @brief 代入演算子
  const ElbEnv&
  operator=(const ElbEnv& src);

  /// @brief デストラクタ
  ~ElbEnv();


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbEnv の状態を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数式を要求する．
  void
  set_constant();

  /// @brief 親の function を設定する．
  /// @param[in] function 設定する function
  /// @param[in] cf constant function の時 true にするフラグ
  void
  set_constant_function(const VlNamedObj* function);

  /// @brief 親の function を設定する．
  void
  set_function();

  /// @brief 親の task を設定する．
  void
  set_task();

  /// @brief system task/system function の引数の印をつける．
  void
  set_system_tf_arg();

  /// @brief イベント式の印をつける．
  void
  set_event_expr();

  /// @brief net 型の左辺式の印をつける．
  void
  set_net_lhs();

  /// @brief reg/var 型の左辺式の印をつける．
  void
  set_var_lhs();

  /// @brief pca 代入文の左辺式の印をつける．
  void
  set_pca_lhs();

  /// @brief force 代入文の左辺式の印をつける．
  void
  set_force_lhs();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbEnv の状態を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数式が要求されている時に true を返す．
  bool
  is_constant() const;

  /// @brief constant function 内の生成の時に true を返す．
  bool
  inside_constant_function() const;

  /// @brief constant function 内の生成の時に親の function を返す．
  const VlNamedObj*
  constant_function() const;

  /// @brief function 内の生成時に true を返す．
  bool
  inside_function() const;

  /// @brief system task/system function の引数の時に true を返す．
  bool
  is_system_tf_arg() const;

  /// @brief イベント式の時に true を返す．
  bool
  is_event_expr() const;

  /// @brief 左辺式の時に true を返す．
  bool
  is_lhs() const;

  /// @brief net 型の左辺式の時に true を返す．
  bool
  is_net_lhs() const;

  /// @brief reg/var 型の左辺式の時に true を返す．
  bool
  is_var_lhs() const;

  /// @brief pca 代入文の左辺式の時に true を返す．
  bool
  is_pca_lhs() const;

  /// @brief force 代入文の左辺式の時に true を返す．
  bool
  is_force_lhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 種々のフラグ
  ymuint32 mFlags;

  // constant function
  const VlNamedObj* mCf;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbConstantEnv ElbEnv.h "ElbEnv.h"
/// @brief 定数式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbConstantEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  ElbConstantEnv();

};


//////////////////////////////////////////////////////////////////////
/// @class ElbConstantFunctionEnv ElbEnv.h "ElbEnv.h"
/// @brief 定数関数内を表す環境
//////////////////////////////////////////////////////////////////////
class ElbConstantFunctionEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 親の関数
  ElbConstantFunctionEnv(const VlNamedObj* func);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbTfEnv ElbEnv.h "ElbEnv.h"
/// @brief 関数内を表す環境
//////////////////////////////////////////////////////////////////////
class ElbTfEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] taskfunc タスクか関数のオブジェクト
  ElbTfEnv(const VlNamedObj* taskfunc);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbSystemTfArgEnv ElbEnv.h "ElbEnv.h"
/// @brief システムタスク/関数の引数を表す環境
//////////////////////////////////////////////////////////////////////
class ElbSystemTfArgEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] env 親の環境
  ElbSystemTfArgEnv(const ElbEnv& env);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbEventExprEnv ElbEnv.h "ElbEnv.h"
/// @brief イベント式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbEventExprEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] env 親の環境
  ElbEventExprEnv(const ElbEnv& env);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbNetLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief net 型の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbNetLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] env 親の環境
  ElbNetLhsEnv(const ElbEnv& env);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbVarLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief reg/var 型の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbVarLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] env 親の環境
  ElbVarLhsEnv(const ElbEnv& env);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbPcaLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief pca 代入文の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbPcaLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] env 親の環境
  ElbPcaLhsEnv(const ElbEnv& env);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbForceLhsEnv ElbEnv.h "ElbEnv.h"
/// @brief force 代入文の左辺式を表す環境
//////////////////////////////////////////////////////////////////////
class ElbForceLhsEnv :
  public ElbEnv
{
public:

  /// @brief コンストラクタ
  /// @param[in] env 親の環境
  ElbForceLhsEnv(const ElbEnv& env);

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ELBENV_H
