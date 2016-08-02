
/// @file libym_verilog/tests/vlview/VmMiscNode.cc
/// @brief VmMiscNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmMiscNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmMiscNode.h"
#include "ym_verilog/vl/VlDelay.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief strength 型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_strength(const QString& label,
		     tVpiStrength value) const
{
  add_child( new VmStrengthValNode(label, value) );
}

// @brief 遅延値型のノードを追加する．
// @param[in] value 値
void
VmNode::add_delay(const VlDelay* value) const
{
  if ( value != NULL ) {
    add_child( new VmDelayNode(value) );
  }
}

// @brief dir 型のノードを追加する．
// @param[in] value 値
void
VmNode::add_dir(tVpiDirection dir) const
{
  add_child( new VmDirNode(dir) );
}

// @brief 文字列型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_str(const QString& label,
		const QString& value) const
{
  add_child( new VmStrNode(label, value) );
}

// @brief 文字列型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_str(const QString& label,
		const string& value) const
{
  add_child( new VmStrNode(label, value.c_str()) );
}

// @brief 文字列型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_str(const QString& label,
		const char* value) const
{
  add_child( new VmStrNode(label, value) );
}

// @brief ブール型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_bool(const QString& label,
		 bool value) const
{
  add_child( new VmBoolNode(label, value) );
}

// @brief 整数型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_int(const QString& label,
		  int value) const
{
  add_child( new VmIntNode(label, value) );
}

// @brief 整数型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_int(const QString& label,
		  ymuint value) const
{
  add_child( new VmIntNode(label, value) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmDelayNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] delay 遅延情報
VmDelayNode::VmDelayNode(const VlDelay* delay) :
  mDelay(delay)
{
}

// @brief デストラクタ
VmDelayNode::~VmDelayNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDelayNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiDelay";
    }
    else if ( column == 1 ) {
      // 手抜き
      return mDelay->decompile().c_str();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmDelayNode::loc() const
{
  return mDelay->file_region();
}

// @brief 子供の配列を作る．
void
VmDelayNode::expand() const
{
}


//////////////////////////////////////////////////////////////////////
// クラス VmScalarNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VmScalarNode::VmScalarNode()
{
}

// @brief デストラクタ
VmScalarNode::~VmScalarNode()
{
}

// @brief 対象のファイル上での位置を返す．
// @note このクラスでは空の FileRegion を返す．
FileRegion
VmScalarNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
// @note このクラスではなにもしない．
void
VmScalarNode::expand() const
{
}


//////////////////////////////////////////////////////////////////////
// クラス VmStrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] str 文字列
VmStrNode::VmStrNode(const QString& label,
		     const QString& str) :
  mLabel(label),
  mStr(str)
{
}

// @brief デストラクタ
VmStrNode::~VmStrNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmStrNode::data(int column,
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
// クラス VmAuxTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] aux_type 補助的なデータ型
// @param[in] net_type ネットの型
// @param[in] var_type 変数の型
VmAuxTypeNode::VmAuxTypeNode(tVpiAuxType aux_type,
			     tVpiNetType net_type,
			     tVpiVarType var_type) :
  mAuxType(aux_type),
  mNetType(net_type),
  mVarType(var_type)
{
}

// @brief デストラクタ
VmAuxTypeNode::~VmAuxTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmAuxTypeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiAuxType";
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
// クラス VmNetTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] net_type ネットの型
VmNetTypeNode::VmNetTypeNode(const QString& label,
			     tVpiNetType net_type) :
  mLabel(label),
  mNetType(net_type)
{
}

// @brief デストラクタ
VmNetTypeNode::~VmNetTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmNetTypeNode::data(int column,
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
// クラス VmVarTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_type 変数の型
VmVarTypeNode::VmVarTypeNode(tVpiVarType var_type) :
  mVarType(var_type)
{
}

// @brief デストラクタ
VmVarTypeNode::~VmVarTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmVarTypeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiVarType";
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
// クラス VmDirNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dir 入出力の方向
VmDirNode::VmDirNode(tVpiDirection dir) :
  mDir(dir)
{
}

// @brief デストラクタ
VmDirNode::~VmDirNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDirNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiDirection";
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
// クラス VmUdNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ud unconnected drive の型
VmUdNode::VmUdNode(tVpiUnconnDrive ud) :
  mUd(ud)
{
}

// @brief デストラクタ
VmUdNode::~VmUdNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiUnconnectedDrive";
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
// クラス VmDelayModeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] delay mode の型
VmDelayModeNode::VmDelayModeNode(tVpiDefDelayMode delay_mode) :
  mDelayMode(delay_mode)
{
}

// @brief デストラクタ
VmDelayModeNode::~VmDelayModeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDelayModeNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiDefaultDelayMode";
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
// クラス VmPrimTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] prim_type primitive タイプ
VmPrimTypeNode::VmPrimTypeNode(tVpiPrimType prim_type) :
  mPrimType(prim_type)
{
}

// @brief デストラクタ
VmPrimTypeNode::~VmPrimTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimTypeNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPrimType";
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
// クラス VmRangeModeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] range_mode 範囲指定の型
VmRangeModeNode::VmRangeModeNode(tVpiRangeMode range_mode) :
  mRangeMode(range_mode)
{
}

// @brief デストラクタ
VmRangeModeNode::~VmRangeModeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmRangeModeNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiRangeMode";
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
// クラス VmStrengthValNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] strength 値を表すノード
VmStrengthValNode::VmStrengthValNode(const QString& label,
				     tVpiStrength strength) :
  mLabel(label),
  mStrength(strength)
{
}

// @brief デストラクタ
VmStrengthValNode::~VmStrengthValNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmStrengthValNode::data(int column,
			int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
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
// クラス VmVsNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vs vectored/scalared 情報
VmVsNode::VmVsNode(tVpiVsType vs) :
  mVs(vs)
{
}

// @brief デストラクタ
VmVsNode::~VmVsNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmVsNode::data(int column,
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
// クラス VmBoolNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] val 真理値
VmBoolNode::VmBoolNode(const QString& label,
		       bool val) :
  mLabel(label),
  mVal(val)
{
}

// @brief デストラクタ
VmBoolNode::~VmBoolNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmBoolNode::data(int column,
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


//////////////////////////////////////////////////////////////////////
// クラス VmIntNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] val 値
VmIntNode::VmIntNode(const QString& label,
		     int val) :
  mLabel(label),
  mVal(val)
{
}

// @brief デストラクタ
VmIntNode::~VmIntNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmIntNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return mVal;
    }
  }
  return QVariant();
}

END_NAMESPACE_YM_VERILOG
