
/// @file libym_verilog/elb_impl/EiExpr.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"
#include "EiOperation.h"

#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlTime.h"
#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiExprBase1
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiExprBase1::EiExprBase1(const PtBase* pt_obj) :
  mPtObj(pt_obj)
{
}
  
// @brief デストラクタ
EiExprBase1::~EiExprBase1()
{
}

// @brief ファイル位置を返す．
FileRegion
EiExprBase1::file_region() const
{
  return mPtObj->file_region();
}
  
// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiExprBase1::set_scalar(tVpiScalarVal v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 実数値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiExprBase1::set_real(double v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiExprBase1::set_bitvector(const BitVector& v)
{
  assert_not_reached(__FILE__, __LINE__);
}  


//////////////////////////////////////////////////////////////////////
// クラス EiOperation
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiOperation::EiOperation(const PtBase* pt_expr) :
  EiExprBase1(pt_expr)
{
}
  
// @brief デストラクタ
EiOperation::~EiOperation()
{
}

// @brief 型の取得
tVpiObjType
EiOperation::type() const
{
  return kVpiOperation;
}

// @brief 演算子の時に true を返す．
bool
EiOperation::is_operation() const
{
  return true;
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiOperation::decompile_impl(int ppri) const
{
  // 各演算子を表す文字列のテーブル
  static const char* sym_table[] = {
    "",               // dummy
    "-",              // vpiMinusOp
    "+",              // vpiPlusOp
    "!",              // vpiNotOp
    "~",              // vpiBitNeg
    "&",              // vpiUnaryAndOp
    "~&",             // vpiUnaryNandOp
    "|",              // vpiUnaryOrOp
    "~|",             // vpiUnaryNorOp
    "^",              // vpiUnaryXorOp
    "~^",             // vpiUnaryXNorOp
    "-",              // vpiSubOp
    "/",              // vpiDivOp
    "%",              // vpiModOp
    "==",             // vpiEqOp
    "!=",             // vpiNeqOp
    "===",            // vpiCaseEqOp
    "!==",            // vpiCaseNeqOp
    ">",              // vpiGtOp
    ">=",             // vpiGeOp
    "<",              // vpiLtOp
    "<=",             // vpiLeOp
    "<<",             // vpiLShiftOp
    ">>",             // vpiRShiftOp
    "+",              // vpiAddOp
    "*",              // vpiMultOp
    "&&",             // vpiLogAndOp
    "||",             // vpiLogOrOp
    "&",              // vpiBitAndOp
    "|",              // vpiBitOrOp
    "^",              // vpiBitXorOp
    "~^",             // vpiBitXNorOp
    "?:",             // vpiConditionOp(dummy)
    "{}",             // vpiConcatOp(dummy)
    "{{}}",           // vpiMultiConcatOp(dummy)
    "or",             // vpiEventOrOp(dummy)
    ";",              // vpiNullOp(dummy)
    ",",              // vpiListOp(dummy)
    ":",              // vpiMinTypMaxOp(dummy)
    "posedge ",       // vpiPosedgeOp
    "negedge ",       // vpiNegedgeOp
    "<<<",            // vpiArithLShiftOp
    ">>>",            // vpiArithRShiftOp
    "**"              // vpiPowerOp
  };
  
  // 優先順位のテーブル
  static int pri_table[] = {
    0,                // dummy
    13,               // vpiMinusOp
    13,               // vpiPlusOp
    13,               // vpiNotOp
    13,               // vpiBitNegOp
    13,               // vpiUnaryAndOp
    13,               // vpiUnaryNandOp
    13,               // vpiUnaryOrOp
    13,               // vpiUnaryNorOp
    13,               // vpiUnaryXorOp
    13,               // vpiUnaryXNorOp
    10,               // vpiSubOp
    11,               // vpiDivOp
    11,               // vpiModOp
    7,                // vpiEqOp
    7,                // vpiNeqOp
    7,                // vpiCaseEqOp
    7,                // vpiCaseNeqOp
    8,                // vpiGtOp
    8,                // vpiGeOp
    8,                // vpiLtOp
    8,                // vpiLeOp
    9,                // vpiLShiftOp
    9,                // vpiRShiftOp
    10,               // vpiAddOp
    11,               // vpiMultOp
    3,                // vpiLogAndOp
    2,                // vpiLogOrOp
    6,                // vpiBitAndOp
    4,                // vpiBitOrOp
    5,                // vpiBitXorOp
    5,                // vpiBitXNorOp
    1,                // vpiConditionOp
    0,                // vpiConcatOp
    0,                // vpiMultiConcatOp
    0,                // vpiEventOrOp
    0,                // vpiNullOp
    0,                // vpiListOp
    0,                // vpiMinTypMaxOp
    0,                // vpiPosedgeOp
    0,                // vpiNegedgeOp
    9,                // vpiArithLShiftOp
    9,                // vpiArithRShiftOp
    12                // vpiPowerOp
  };
  
  string ans;

  tVpiOpType optype = op_type();
  // parent_optype の優先順位が自分の優先順位よりも高ければ括弧が必要
  bool need_par = false;
  int pri = pri_table[optype];
  if ( ppri > pri ) {
    need_par = true;
    ans += "(";
  }
  
  switch ( optype ) {
    
    // 空
  case kVpiNullOp:
    break;
    
    // 単項演算子
  case kVpiMinusOp:
  case kVpiNotOp:
  case kVpiBitNegOp:
  case kVpiPlusOp:
  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryXNorOp:
  case kVpiUnaryXorOp:
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    ans += sym_table[optype] + _operand(0)->decompile_impl(pri);
    break;
    
    // 二項演算子
  case kVpiAddOp:
  case kVpiArithLShiftOp:
  case kVpiArithRShiftOp:
  case kVpiBitAndOp:
  case kVpiBitOrOp:
  case kVpiBitXNorOp:
  case kVpiBitXorOp:
  case kVpiCaseEqOp:
  case kVpiCaseNeqOp:
  case kVpiDivOp:
  case kVpiEqOp:
  case kVpiGeOp:
  case kVpiGtOp:
  case kVpiLShiftOp:
  case kVpiLeOp:
  case kVpiLogAndOp:
  case kVpiLogOrOp:
  case kVpiLtOp:
  case kVpiModOp:
  case kVpiMultOp:
  case kVpiNeqOp:
  case kVpiPowerOp:
  case kVpiRShiftOp:
  case kVpiSubOp:
    ans += _operand(0)->decompile_impl(pri) + sym_table[optype] +
      _operand(1)->decompile_impl(pri);
    break;
    
    // 三項演算子
  case kVpiConditionOp:
    ans += _operand(0)->decompile_impl(pri) + "?" +
      _operand(1)->decompile_impl(pri) + ":" +
      _operand(2)->decompile_impl(pri);
    break;

  case kVpiMinTypMaxOp:
    ans += _operand(0)->decompile_impl(pri) + ":" +
      _operand(1)->decompile_impl(pri) + ":" +
      _operand(2)->decompile_impl(pri);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( need_par ) {
    ans += ")";
  }
  return ans;
}

END_NAMESPACE_YM_VERILOG
