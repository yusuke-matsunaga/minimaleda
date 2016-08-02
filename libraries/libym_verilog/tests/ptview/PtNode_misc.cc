
/// @file libym_verilog/tests/vlview/PtNode_misc.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode_misc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_misc.h"
#include "PtNode_expr.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ControlNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] control コントロール
ControlNode::ControlNode(const PtControl* control) :
  mControl(control)
{
}

// @brief デストラクタ
ControlNode::~ControlNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ControlNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      switch ( mControl->type() ) {
      case kPtDelayControl:  return "DelayControl";
      case kPtEventControl:  return "EventControl";
      case kPtRepeatControl: return "RepeatControl";
      }
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
ControlNode::loc() const
{
  return mControl->file_region();
}

// @brief 子供の配列を作る．
void
ControlNode::expand() const
{
  switch ( mControl->type() ) {
  case kPtDelayControl:
    mChildren.reserve(1);
    mChildren.push_back( new ExprNode("Delay", mControl->delay()) );
    break;
    
  case kPtEventControl:
    {
      ymuint32 n = mControl->event_num();
      mChildren.reserve(n);
      for (ymuint32 i = 0; i < n; ++ i) {
	mChildren.push_back( new ExprNode("Event", mControl->event(i)) );
      }
    }
    break;
    
  case kPtRepeatControl:
    {
      ymuint32 n = mControl->event_num();
      mChildren.reserve(n + 1);
      mChildren.push_back( new ExprNode("Repeat", mControl->rep_expr()) );
      for (ymuint32 i = 0; i < n; ++ i) {
	mChildren.push_back( new ExprNode("Event", mControl->event(i)) );
      }
    }
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ConnectionListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] con_array 接続のリスト
ConnectionListNode::ConnectionListNode(const PtConnectionArray& con_array) :
  mConArray(con_array)
{
}

// @brief デストラクタ
ConnectionListNode::~ConnectionListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ConnectionListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Connection List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
ConnectionListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
ConnectionListNode::expand() const
{
  ymuint32 n = mConArray.size();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new ConnectionNode(mConArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ConnectionNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] con 接続
ConnectionNode::ConnectionNode(const PtConnection* con) :
  mCon(con)
{
}

// @brief デストラクタ
ConnectionNode::~ConnectionNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ConnectionNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Connection";
    }
    else if ( column == 1 ) {
      if ( mCon->name() ) {
	return mCon->name();
      }
      else {
	return "";
      }
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
ConnectionNode::loc() const
{
  return mCon->file_region();
}

// @brief 子供の配列を作る．
void
ConnectionNode::expand() const
{
  if ( mCon->expr() ) {
    mChildren.reserve(1);
    mChildren.push_back( new ExprNode("Expression", mCon->expr()) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス StrengthNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] con 接続
StrengthNode::StrengthNode(const PtStrength* strength) :
  mStrength(strength)
{
}

// @brief デストラクタ
StrengthNode::~StrengthNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
StrengthNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Strength";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
StrengthNode::loc() const
{
  return mStrength->file_region();
}

// @brief 子供の配列を作る．
void
StrengthNode::expand() const
{
  if ( mStrength->drive0() != kVpiNoStrength ) {
    assert_cond(mStrength->drive1() != kVpiNoStrength, __FILE__, __LINE__);
    assert_cond(mStrength->charge() == kVpiNoStrength, __FILE__, __LINE__);
    mChildren.reserve(2);
    mChildren.push_back( new StrengthValNode("Drive 0", mStrength->drive0()) );
    mChildren.push_back( new StrengthValNode("Drive 1", mStrength->drive1()) );
  }
  else {
    assert_cond(mStrength->drive1() == kVpiNoStrength, __FILE__, __LINE__);
    assert_cond(mStrength->charge() != kVpiNoStrength, __FILE__, __LINE__);
    mChildren.reserve(1);
    mChildren.push_back( new StrengthValNode("Charge", mStrength->charge()) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DelayNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] con 接続
DelayNode::DelayNode(const PtDelay* delay) :
  mDelay(delay)
{
}

// @brief デストラクタ
DelayNode::~DelayNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DelayNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Delay";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DelayNode::loc() const
{
  return mDelay->file_region();
}

// @brief 子供の配列を作る．
void
DelayNode::expand() const
{
  ymuint32 n = 0;
  for (ymuint32 i = 0; i < 3; ++ i) {
    if ( mDelay->value(i) ) {
      ++ n;
    }
  }
  mChildren.reserve(n);
  for (ymuint32 i = 0; i < 3; ++ i) {
    if ( mDelay->value(i) ) {
      QString label("Delay");
      label += static_cast<char>('0' + i);
      mChildren.push_back( new ExprNode(label, mDelay->value(i)) );
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス NameBranchListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_branch_list 階層名のリスト
NameBranchListNode::NameBranchListNode(const PtNameBranchArray& nb_list) :
  mNameBranchArray(nb_list)
{
}

// @brief デストラクタ
NameBranchListNode::~NameBranchListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
NameBranchListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Name Branch List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
NameBranchListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
NameBranchListNode::expand() const
{
  ymuint32 n = mNameBranchArray.size();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new NameBranchNode(mNameBranchArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス NameBranchNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name_branch 階層名
NameBranchNode::NameBranchNode(const PtNameBranch* name_branch) :
  mNameBranch(name_branch)
{
}

// @brief デストラクタ
NameBranchNode::~NameBranchNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
NameBranchNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Name Branch";
    }
    else if ( column == 1 ) {
      QString name = mNameBranch->name();
      if ( mNameBranch->index() ) {
	name += static_cast<char>('0' + mNameBranch->index());
      }
      return name;
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
NameBranchNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
NameBranchNode::expand() const
{
}


//////////////////////////////////////////////////////////////////////
// クラス StrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] str 文字列
StrNode::StrNode(const QString& label,
		 const QString& str) :
  mLabel(label),
  mStr(str)
{
}

// @brief デストラクタ
StrNode::~StrNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
StrNode::data(int column,
	      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return mStr;
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス AuxTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] aux_type 補助的なデータ型
// @param[in] net_type ネットの型
// @param[in] var_type 変数の型
AuxTypeNode::AuxTypeNode(tVpiAuxType aux_type,
			 tVpiNetType net_type,
			 tVpiVarType var_type) :
  mAuxType(aux_type),
  mNetType(net_type),
  mVarType(var_type)
{
}

// @brief デストラクタ
AuxTypeNode::~AuxTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
AuxTypeNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "AuxType";
    }
    else if ( column == 1 ) {
      switch ( mAuxType ) {
      case kVpiAuxNone: return "None";
      case kVpiAuxNet:
	switch ( mNetType ) {
	case kVpiSupply0: return "Supply0";
	case kVpiSupply1: return "Supply1";
	case kVpiTri:     return "Tri";
	case kVpiTriAnd:  return "Triand";
	case kVpiTriOr:   return "Trior";
	case kVpiTriReg:  return "Trireg";
	case kVpiTri0:    return "Tri0";
	case kVpiTri1:    return "Tri1";
	case kVpiWire:    return "Wire";
	case kVpiWand:    return "Wand";
	case kVpiWor:     return "Wor";
	case kVpiNone:    return "None";
	}
      case kVpiAuxReg:  return "Reg";
      case kVpiAuxVar:
	switch ( mVarType ) {
	case kVpiVarInteger:  return "Integer";
	case kVpiVarReal:     return "Real";
	case kVpiVarTime:     return "Time";
	case kVpiVarRealtime: return "Realtime";
	case kVpiVarNone:     return "None";
	}
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス NetTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] net_type ネットの型
NetTypeNode::NetTypeNode(const QString& label,
			 tVpiNetType net_type) :
  mLabel(label),
  mNetType(net_type)
{
}

// @brief デストラクタ
NetTypeNode::~NetTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
NetTypeNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      switch ( mNetType ) {
      case kVpiSupply0: return "Supply0";
      case kVpiSupply1: return "Supply1";
      case kVpiTri:     return "Tri";
      case kVpiTriAnd:  return "Triand";
      case kVpiTriOr:   return "Trior";
      case kVpiTriReg:  return "Trireg";
      case kVpiTri0:    return "Tri0";
      case kVpiTri1:    return "Tri1";
      case kVpiWire:    return "Wire";
      case kVpiWand:    return "Wand";
      case kVpiWor:     return "Wor";
      case kVpiNone:    return "None";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VarTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_type 変数の型
VarTypeNode::VarTypeNode(tVpiVarType var_type) :
  mVarType(var_type)
{
}

// @brief デストラクタ
VarTypeNode::~VarTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VarTypeNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "VarType";
    }
    else if ( column == 1 ) {
      switch ( mVarType ) {
      case kVpiVarInteger:  return "Integer";
      case kVpiVarReal:     return "Real";
      case kVpiVarTime:     return "Time";
      case kVpiVarRealtime: return "Realtime";
      case kVpiVarNone:     return "None";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス DirNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dir 入出力の方向
DirNode::DirNode(tVpiDirection dir) :
  mDir(dir)
{
}

// @brief デストラクタ
DirNode::~DirNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DirNode::data(int column,
	      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Direction";
    }
    else if ( column == 1 ) {
      switch ( mDir ) {
      case kVpiInput:       return "Input";
      case kVpiOutput:      return "Output";
      case kVpiInout:       return "Inout";
      case kVpiMixedIO:     return "Mixed IO";
      case kVpiNoDirection: return "No Direction";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス UdNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ud unconnected drive の型
UdNode::UdNode(tVpiUnconnDrive ud) :
  mUd(ud)
{
}

// @brief デストラクタ
UdNode::~UdNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
UdNode::data(int column,
	     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Unconnected Drive";
    }
    else if ( column == 1 ) {
      switch ( mUd ) {
      case kVpiHighZ: return "high-Z";
      case kVpiPull1: return "pull-1";
      case kVpiPull0: return "pull-0";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス DelayModeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] delay mode の型
DelayModeNode::DelayModeNode(tVpiDefDelayMode delay_mode) :
  mDelayMode(delay_mode)
{
}

// @brief デストラクタ
DelayModeNode::~DelayModeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DelayModeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Default Delay Mode";
    }
    else if ( column == 1 ) {
      switch ( mDelayMode ) {
      case kVpiDelayModeNone:    return "none";
      case kVpiDelayModePath:    return "path";
      case kVpiDelayModeDistrib: return "distrib";
      case kVpiDelayModeUnit:    return "unit";
      case kVpiDelayModeZero:    return "zero";
      case kVpiDelayModeMTM:     return "min-typ-max";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス PrimTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] prim_type primitive タイプ
PrimTypeNode::PrimTypeNode(tVpiPrimType prim_type) :
  mPrimType(prim_type)
{
}

// @brief デストラクタ
PrimTypeNode::~PrimTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
PrimTypeNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "PrimType";
    }
    else if ( column == 1 ) {
      switch ( mPrimType ) {
      case kVpiAndPrim:      return "and";
      case kVpiNandPrim:     return "nand";
      case kVpiNorPrim:      return "nor";
      case kVpiOrPrim:       return "or";
      case kVpiXorPrim:      return "xor";
      case kVpiXnorPrim:     return "xnor";
      case kVpiBufPrim:      return "buf";
      case kVpiNotPrim:      return "not";
      case kVpiBufif0Prim:   return "bufif0";
      case kVpiBufif1Prim:   return "bufif1";
      case kVpiNotif0Prim:   return "notif0";
      case kVpiNotif1Prim:   return "notif1";
      case kVpiNmosPrim:     return "nmos";
      case kVpiPmosPrim:     return "pmos";
      case kVpiCmosPrim:     return "cmos";
      case kVpiRnmosPrim:    return "rnmos";
      case kVpiRpmosPrim:    return "rpmos";
      case kVpiRcmosPrim:    return "rcmos";
      case kVpiRtranPrim:    return "rtran";
      case kVpiRtranif0Prim: return "rtranif0";
      case kVpiRtranif1Prim: return "rtranif1";
      case kVpiTranPrim:     return "tran";
      case kVpiTranif0Prim:  return "tranif0";
      case kVpiTranif1Prim:  return "tranif1";
      case kVpiPullupPrim:   return "pullup";
      case kVpiPulldownPrim: return "pulldown";
      case kVpiSeqPrim:      return "seq";
      case kVpiCombPrim:     return "comb";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス OpTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] op_type 演算子のタイプ
OpTypeNode::OpTypeNode(tVpiOpType op_type) :
  mOpType(op_type)
{
}

// @brief デストラクタ
OpTypeNode::~OpTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
OpTypeNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "OpType";
    }
    else if ( column == 1 ) {
      switch ( mOpType ) {
      case kVpiMinusOp:       return "minus";
      case kVpiPlusOp:        return "plus";
      case kVpiNotOp:         return "not";
      case kVpiBitNegOp:      return "bitneg";
      case kVpiUnaryAndOp:    return "unary and";
      case kVpiUnaryNandOp:   return "unary nand";
      case kVpiUnaryOrOp:     return "unary or";
      case kVpiUnaryNorOp:    return "unary nor";
      case kVpiUnaryXorOp:    return "unary xor";
      case kVpiUnaryXNorOp:   return "unary xnor";
      case kVpiSubOp:         return "sub";
      case kVpiDivOp:         return "div";
      case kVpiModOp:         return "mod";
      case kVpiEqOp:          return "eq";
      case kVpiNeqOp:         return "neq";
      case kVpiCaseEqOp:      return "caseeq";
      case kVpiCaseNeqOp:     return "caseneq";
      case kVpiGtOp:          return "gt";
      case kVpiGeOp:          return "ge";
      case kVpiLtOp:          return "lt";
      case kVpiLeOp:          return "le";
      case kVpiLShiftOp:      return "left shift";
      case kVpiRShiftOp:      return "right shift";
      case kVpiAddOp:         return "add";
      case kVpiMultOp:        return "mult";
      case kVpiLogAndOp:      return "logical and";
      case kVpiLogOrOp:       return "logical or";
      case kVpiBitAndOp:      return "bit and";
      case kVpiBitOrOp:       return "bit or";
      case kVpiBitXorOp:      return "bit xor";
      case kVpiBitXNorOp:     return "bit xnor";
      case kVpiConditionOp:   return "conditional";
      case kVpiConcatOp:      return "concat";
      case kVpiMultiConcatOp: return "multi concat";
      case kVpiEventOrOp:     return "event or";
      case kVpiNullOp:        return "null";
      case kVpiListOp:        return "list";
      case kVpiMinTypMaxOp:   return "min-typ-max";
      case kVpiPosedgeOp:     return "posedge";
      case kVpiNegedgeOp:     return "negedge";
      case kVpiArithLShiftOp: return "arithmetic left shift";
      case kVpiArithRShiftOp: return "arithmetic right shift";
      case kVpiPowerOp:       return "power";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス ConstTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] const_type 定数の型
ConstTypeNode::ConstTypeNode(tVpiConstType const_type) :
  mConstType(const_type)
{
}

// @brief デストラクタ
ConstTypeNode::~ConstTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ConstTypeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Const Type";
    }
    else if ( column == 1 ) {
      switch ( mConstType ) {
      case kVpiDecConst:          return "dec";
      case kVpiRealConst:         return "real";
      case kVpiBinaryConst:       return "binary";
      case kVpiOctConst:          return "oct";
      case kVpiHexConst:          return "hex";
      case kVpiStringConst:       return "string";
      case kVpiIntConst:          return "int";
      case kVpiSignedDecConst:    return "signed dec";
      case kVpiSignedBinaryConst: return "signed binary";
      case kVpiSignedOctConst:    return "signed oct";
      case kVpiSignedHexConst:    return "signed hex";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス RangeModeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] range_mode 範囲指定の型
RangeModeNode::RangeModeNode(tVpiRangeMode range_mode) :
  mRangeMode(range_mode)
{
}

// @brief デストラクタ
RangeModeNode::~RangeModeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
RangeModeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "RangeMode";
    }
    else if ( column == 1 ) {
      switch ( mRangeMode ) {
      case kVpiNoRange:    return "No Range";
      case kVpiConstRange: return "Constant Range";
      case kVpiPlusRange:  return "Plus Range";
      case kVpiMinusRange: return "Minus Range";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス StrengthValNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] strength 値を表すノード
StrengthValNode::StrengthValNode(const QString& label,
				 tVpiStrength strength) :
  mLabel(label),
  mStrength(strength)
{
}

// @brief デストラクタ
StrengthValNode::~StrengthValNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
StrengthValNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Strength";
    }
    else if ( column == 1 ) {
      switch ( mStrength ) {
      case kVpiNoStrength:   return "none";
      case kVpiSupplyDrive:  return "supply";
      case kVpiStrongDrive:  return "strong";
      case kVpiPullDrive:    return "pull";
      case kVpiWeakDrive:    return "weak";
      case kVpiLargeCharge:  return "large";
      case kVpiMediumCharge: return "medium";
      case kVpiSmallCharge:  return "small";
      case kVpiHiZ:          return "high-Z";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VsNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vs vectored/scalared 情報
VsNode::VsNode(tVpiVsType vs) :
  mVs(vs)
{
}

// @brief デストラクタ
VsNode::~VsNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VsNode::data(int column,
	     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vectored/scalared";
    }
    else if ( column == 1 ) {
      switch ( mVs ) {
      case kVpiVsNone:   return "None";
      case kVpiVectored: return "Vectored";
      case kVpiScalared: return "Scalared";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス BoolNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] val 真理値
BoolNode::BoolNode(const QString& label,
		   bool val) :
  mLabel(label),
  mVal(val)
{
}

// @brief デストラクタ
BoolNode::~BoolNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
BoolNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      if ( mVal) {
	return "True";
      }
      else {
	return "False";
      }
    }
  }
  return QVariant();
}

END_NAMESPACE_YM_VERILOG
