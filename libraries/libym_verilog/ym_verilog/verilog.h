#ifndef YM_VERILOG_VERILOG_H
#define YM_VERILOG_VERILOG_H

/// @file ym_verilog/verilog.h
/// @brief libymverilog に共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: verilog.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @namespace nsYm::nsVerilog
/// @brief Verilog パーサ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

/// @defgroup VlGroup Verilog パッケージ
///
/// Verilog-HDL(IEEE1364, IEEE1364-2001) フォーマットの
/// フロントエンド


#include "ymtools.h"
#include "vpi_user.h"

/// @brief ym_verilog 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_VERILOG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsVerilog)

/// @brief ym_verilog 用の名前空間の終了
#define END_NAMESPACE_YM_VERILOG \
END_NAMESPACE(nsVerilog) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class VlLineWatcher;
class VlMgr;
class VlTime;

/// @defgroup VlCommon Verilog 用基本ユーティリティ
/// @ingroup VlGroup
/// @{

//////////////////////////////////////////////////////////////////////
/// @brief 1ビットの値
//////////////////////////////////////////////////////////////////////
enum tVpiScalarVal {
  /// @brief 00: high-impedance
  kVpiScalarZ = 0,
  /// @brief 01: 普通の 0
  kVpiScalar0 = 1,
  /// @brief 10: 普通の 1
  kVpiScalar1 = 2,
  /// @brief 11: 0 か 1 か不明
  kVpiScalarX = 3
};

/// @brief tVpiScalarVal から bool への変換
bool
conv_to_bool(tVpiScalarVal src);

/// @brief tVpiScalarVal から論理値への変換
/// @note kVpiScalarZ -> kVpiScalarX をやっている
tVpiScalarVal
conv_to_logic(tVpiScalarVal src);

/// @brief tVpiScalarVal から int への変換
/// @note kVpiScalar1 以外はすべての 0 にマッピングする．
int
conv_to_int(tVpiScalarVal src);

/// @brief tVpiScalarVal から実数値への変換
/// @note kVpiScalar1 を 1.0 に，それい以外はすべての 0.0 にマッピングする．
double
conv_to_real(tVpiScalarVal src);

/// @brief int から tVpiScalarVal への変換
/// @note 0 なら kVpiScalar0 に，それ以外は kVpiScalar1 にマッピングする．
tVpiScalarVal
conv_to_scalar(int src);

/// @brief ymuint から tVpiScalarVal への変換
/// @note 0 なら kVpiScalar0 に，それ以外は kVpiScalar1 にマッピングする．
tVpiScalarVal
conv_to_scalar(ymuint src);

/// @brief 実数値から tVpiScalarVal への変換
/// @note 0.0 なら kVpiScalar0 に，それ以外は kVpiScalar1 にマッピングする．
tVpiScalarVal
conv_to_scalar(double src);

/// @brief tVpiScalarVal の否定
/// @note kVpiScalarZ は kVpiScalarX と同様に扱われる．
tVpiScalarVal
operator!(tVpiScalarVal src);

/// @brief tVpiScalarVal の AND
tVpiScalarVal
operator&&(tVpiScalarVal src1,
	   tVpiScalarVal src2);

/// @brief tVpiScalarVal の OR
tVpiScalarVal
operator||(tVpiScalarVal src1,
	   tVpiScalarVal src2);

/// @brief tVpiScalarVal の等価比較
tVpiScalarVal
eq(tVpiScalarVal src1,
   tVpiScalarVal src2);

/// @brief tVpiScalarVal の等価比較
tVpiScalarVal
neq(tVpiScalarVal src1,
    tVpiScalarVal src2);


//////////////////////////////////////////////////////////////////////
// 列挙型の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @brief オブジェクトの型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiObjType {
  kVpiAlways          = vpiAlways,
  kVpiAssignStmt      = vpiAssignStmt,
  kVpiAssignment      = vpiAssignment,
  kVpiBegin           = vpiBegin,
  kVpiCase            = vpiCase,
  kVpiCaseItem        = vpiCaseItem,
  kVpiConstant        = vpiConstant,
  kVpiContAssign      = vpiContAssign,
  kVpiDeassign        = vpiDeassign,
  kVpiDefParam        = vpiDefParam,
  kVpiDelayControl    = vpiDelayControl,
  kVpiDisable         = vpiDisable,
  kVpiEventControl    = vpiEventControl,
  kVpiEventStmt       = vpiEventStmt,
  kVpiFor             = vpiFor,
  kVpiForce           = vpiForce,
  kVpiForever         = vpiForever,
  kVpiFork            = vpiFork,
  kVpiFuncCall        = vpiFuncCall,
  kVpiFunction        = vpiFunction,
  kVpiGate            = vpiGate,
  kVpiIf              = vpiIf,
  kVpiIfElse          = vpiIfElse,
  kVpiInitial         = vpiInitial,
  kVpiIntegerVar      = vpiIntegerVar,
  kVpiInterModPath    = vpiInterModPath,
  kVpiIterator        = vpiIterator,
  kVpiIODecl          = vpiIODecl,
  kVpiMemory          = vpiMemory,
  kVpiMemoryWord      = vpiMemoryWord,
  kVpiModPath         = vpiModPath,
  kVpiModule          = vpiModule,
  kVpiNamedBegin      = vpiNamedBegin,
  kVpiNamedEvent      = vpiNamedEvent,
  kVpiNamedFork       = vpiNamedFork,
  kVpiNet             = vpiNet,
  kVpiNetBit          = vpiNetBit,
  kVpiNullStmt        = vpiNullStmt,
  kVpiOperation       = vpiOperation,
  kVpiParamAssign     = vpiParamAssign,
  kVpiParameter       = vpiParameter,
  kVpiPartSelect      = vpiPartSelect,
  kVpiPathTerm        = vpiPathTerm,
  kVpiPort            = vpiPort,
  kVpiPortBit         = vpiPortBit,
  kVpiPrimTerm        = vpiPrimTerm,
  kVpiRealVar         = vpiRealVar,
  kVpiReg             = vpiReg,
  kVpiRegBit          = vpiRegBit,
  kVpiRelease         = vpiRelease,
  kVpiRepeat          = vpiRepeat,
  kVpiRepeatControl   = vpiRepeatControl,
  kVpiSchedEvent      = vpiSchedEvent,
  kVpiSpecParam       = vpiSpecParam,
  kVpiSwitch          = vpiSwitch,
  kVpiSysFuncCall     = vpiSysFuncCall,
  kVpiSysTaskCall     = vpiSysTaskCall,
  kVpiTableEntry      = vpiTableEntry,
  kVpiTask            = vpiTask,
  kVpiTaskCall        = vpiTaskCall,
  kVpiTchk            = vpiTchk,
  kVpiTchkTerm        = vpiTchkTerm,
  kVpiTimeVar         = vpiTimeVar,
  kVpiTimeQueue       = vpiTimeQueue,
  kVpiUdp             = vpiUdp,
  kVpiUdpDefn         = vpiUdpDefn,
  kVpiUserSystf       = vpiUserSystf,
  kVpiVarSelect       = vpiVarSelect,
  kVpiWait            = vpiWait,
  kVpiWhile           = vpiWhile,
  kVpiAttribute       = vpiAttribute,
  kVpiBitSelect       = vpiBitSelect,
  kVpiCallback        = vpiCallback,
  kVpiDelayTerm       = vpiDelayTerm,
  kVpiDelayDevice     = vpiDelayDevice,
  kVpiFrame           = vpiFrame,
  kVpiGateArray       = vpiGateArray,
  kVpiModuleArray     = vpiModuleArray,
  kVpiPrimitiveArray  = vpiPrimitiveArray,
  kVpiNetArray        = vpiNetArray,
  kVpiRange           = vpiRange,
  kVpiRegArray        = vpiRegArray,
  kVpiSwitchArray     = vpiSwitchArray,
  kVpiUdpArray        = vpiUdpArray,
  kVpiContAssignBit   = vpiContAssignBit,
  kVpiNamedEventArray = vpiNamedEventArray,
  kVpiScope           = vpiScope
};


//////////////////////////////////////////////////////////////////////
/// @brief unconnected port drive の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiUnconnDrive {
  kVpiHighZ = vpiHighZ,
  kVpiPull1 = vpiPull1,
  kVpiPull0 = vpiPull0
};


//////////////////////////////////////////////////////////////////////
/// @brief default delay Mode の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiDefDelayMode {
  kVpiDelayModeNone    = vpiDelayModeNone,
  kVpiDelayModePath    = vpiDelayModePath,
  kVpiDelayModeDistrib = vpiDelayModeDistrib,
  kVpiDelayModeUnit    = vpiDelayModeUnit,
  kVpiDelayModeZero    = vpiDelayModeZero,
  kVpiDelayModeMTM     = vpiDelayModeMTM
};


//////////////////////////////////////////////////////////////////////
/// @brief direction の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiDirection {
  kVpiInput       = vpiInput,
  kVpiOutput      = vpiOutput,
  kVpiInout       = vpiInout,
  kVpiMixedIO     = vpiMixedIO,
  kVpiNoDirection = vpiNoDirection
};


//////////////////////////////////////////////////////////////////////
/// @brief net の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiNetType {
  kVpiWire     = vpiWire,
  kVpiWand     = vpiWand,
  kVpiWor      = vpiWor,
  kVpiTri      = vpiTri,
  kVpiTri0     = vpiTri0,
  kVpiTri1     = vpiTri1,
  kVpiTriReg   = vpiTriReg,
  kVpiTriAnd   = vpiTriAnd,
  kVpiTriOr    = vpiTriOr,
  kVpiSupply1  = vpiSupply1,
  kVpiSupply0  = vpiSupply0,
  kVpiNone     = vpiNone
};


//////////////////////////////////////////////////////////////////////
/// @brief primitive の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiPrimType {
  kVpiAndPrim       = vpiAndPrim,
  kVpiNandPrim      = vpiNandPrim,
  kVpiNorPrim       = vpiNorPrim,
  kVpiOrPrim        = vpiOrPrim,
  kVpiXorPrim       = vpiXorPrim,
  kVpiXnorPrim      = vpiXnorPrim,
  kVpiBufPrim       = vpiBufPrim,
  kVpiNotPrim       = vpiNotPrim,
  kVpiBufif0Prim    = vpiBufif0Prim,
  kVpiBufif1Prim    = vpiBufif1Prim,
  kVpiNotif0Prim    = vpiNotif0Prim,
  kVpiNotif1Prim    = vpiNotif1Prim,
  kVpiNmosPrim      = vpiNmosPrim,
  kVpiPmosPrim      = vpiPmosPrim,
  kVpiCmosPrim      = vpiCmosPrim,
  kVpiRnmosPrim     = vpiRnmosPrim,
  kVpiRpmosPrim     = vpiRpmosPrim,
  kVpiRcmosPrim     = vpiRcmosPrim,
  kVpiRtranPrim     = vpiRtranPrim,
  kVpiRtranif0Prim  = vpiRtranif0Prim,
  kVpiRtranif1Prim  = vpiRtranif1Prim,
  kVpiTranPrim      = vpiTranPrim,
  kVpiTranif0Prim   = vpiTranif0Prim,
  kVpiTranif1Prim   = vpiTranif1Prim,
  kVpiPullupPrim    = vpiPullupPrim,
  kVpiPulldownPrim  = vpiPulldownPrim,
  kVpiSeqPrim       = vpiSeqPrim,
  kVpiCombPrim      = vpiCombPrim
};


//////////////////////////////////////////////////////////////////////
/// @brief module path/data path の極性
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiPolarity {
  kVpiPositive = vpiPositive,
  kVpiNegative = vpiNegative,
  kVpiUnknown  = vpiUnknown
};


//////////////////////////////////////////////////////////////////////
/// @brief module path の edge type
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiEdge {
  kVpiNoEdge  = vpiNoEdge,
  kVpiEdge01  = vpiEdge01,
  kVpiEdge10  = vpiEdge10,
  kVpiEdge0x  = vpiEdge0x,
  kVpiEdgex1  = vpiEdgex1,
  kVpiEdge1x  = vpiEdge1x,
  kVpiEdgex0  = vpiEdgex0,
  kVpiPosedge = vpiPosedge, // = (kVpiEdgex1 | kVpiEdge01 | kVpiEdge0x)
  kVpiNegedge = vpiNegedge, // = (kVpiEdgex0 | kVpiEdge10 | kVpiEdge1x)
  kVpiAnyEdge = vpiAnyEdge  // = (kVpiPosedge | kVpiNegedge)
};


//////////////////////////////////////////////////////////////////////
/// @brief path delay connection の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiPathType {
  kVpiPathFull     = vpiPathFull,
  kVpiPathParallel = vpiPathParallel
};


//////////////////////////////////////////////////////////////////////
/// @brief timing check の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiTchkType {
  kVpiSetup      = vpiSetup,
  kVpiHold       = vpiHold,
  kVpiPeriod     = vpiPeriod,
  kVpiWidth      = vpiWidth,
  kVpiSkew       = vpiSkew,
  kVpiRecovery   = vpiRecovery,
  kVpiNoChange   = vpiNoChange,
  kVpiSetupHold  = vpiSetupHold,
  kVpiFullskew   = vpiFullskew,
  kVpiRecrem     = vpiRecrem,
  kVpiRemoval    = vpiRemoval,
  kVpiTimeskew   = vpiTimeskew
};


//////////////////////////////////////////////////////////////////////
/// @brief operation の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiOpType {
  kVpiMinusOp        = vpiMinusOp,
  kVpiPlusOp         = vpiPlusOp,
  kVpiNotOp          = vpiNotOp,
  kVpiBitNegOp       = vpiBitNegOp,
  kVpiUnaryAndOp     = vpiUnaryAndOp,
  kVpiUnaryNandOp    = vpiUnaryNandOp,
  kVpiUnaryOrOp      = vpiUnaryOrOp,
  kVpiUnaryNorOp     = vpiUnaryNorOp,
  kVpiUnaryXorOp     = vpiUnaryXorOp,
  kVpiUnaryXNorOp    = vpiUnaryXNorOp,
  kVpiSubOp          = vpiSubOp,
  kVpiDivOp          = vpiDivOp,
  kVpiModOp          = vpiModOp,
  kVpiEqOp           = vpiEqOp,
  kVpiNeqOp          = vpiNeqOp,
  kVpiCaseEqOp       = vpiCaseEqOp,
  kVpiCaseNeqOp      = vpiCaseNeqOp,
  kVpiGtOp           = vpiGtOp,
  kVpiGeOp           = vpiGeOp,
  kVpiLtOp           = vpiLtOp,
  kVpiLeOp           = vpiLeOp,
  kVpiLShiftOp       = vpiLShiftOp,
  kVpiRShiftOp       = vpiRShiftOp,
  kVpiAddOp          = vpiAddOp,
  kVpiMultOp         = vpiMultOp,
  kVpiLogAndOp       = vpiLogAndOp,
  kVpiLogOrOp        = vpiLogOrOp,
  kVpiBitAndOp       = vpiBitAndOp,
  kVpiBitOrOp        = vpiBitOrOp,
  kVpiBitXorOp       = vpiBitXorOp,
  kVpiBitXNorOp      = vpiBitXNorOp,
  kVpiConditionOp    = vpiConditionOp,
  kVpiConcatOp       = vpiConcatOp,
  kVpiMultiConcatOp  = vpiMultiConcatOp,
  kVpiEventOrOp      = vpiEventOrOp,
  kVpiNullOp         = vpiNullOp,
  kVpiListOp         = vpiListOp,
  kVpiMinTypMaxOp    = vpiMinTypMaxOp,
  kVpiPosedgeOp      = vpiPosedgeOp,
  kVpiNegedgeOp      = vpiNegedgeOp,
  kVpiArithLShiftOp  = vpiArithLShiftOp,
  kVpiArithRShiftOp  = vpiArithRShiftOp,
  kVpiPowerOp        = vpiPowerOp
};


//////////////////////////////////////////////////////////////////////
/// @brief constant の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし kVpiSignedXXXXX はない．
//////////////////////////////////////////////////////////////////////
enum tVpiConstType {
  kVpiDecConst          = vpiDecConst,
  kVpiRealConst         = vpiRealConst,
  kVpiBinaryConst       = vpiBinaryConst,
  kVpiOctConst          = vpiOctConst,
  kVpiHexConst          = vpiHexConst,
  kVpiStringConst       = vpiStringConst,
  kVpiIntConst          = vpiIntConst,
  kVpiSignedDecConst    = kVpiDecConst    | 8,
  kVpiSignedBinaryConst = kVpiBinaryConst | 8,
  kVpiSignedOctConst    = kVpiOctConst    | 8,
  kVpiSignedHexConst    = kVpiHexConst    | 8
};


//////////////////////////////////////////////////////////////////////
/// @brief case の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiCaseType {
  kVpiCaseExact = vpiCaseExact,
  kVpiCaseX     = vpiCaseX,
  kVpiCaseZ     = vpiCaseZ
};


//////////////////////////////////////////////////////////////////////
/// @brief function の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし kVpiRealtimeFunc はない．
//////////////////////////////////////////////////////////////////////
enum tVpiFuncType {
  kVpiIntFunc         = vpiIntFunc,
  kVpiRealFunc        = vpiRealFunc,
  kVpiTimeFunc        = vpiTimeFunc,
  kVpiSizedFunc       = vpiSizedFunc,
  kVpiSizedSignedFunc = vpiSizedSignedFunc,
  kVpiRealtimeFunc    = 6,
  kVpiSysFuncInt      = vpiSysFuncInt,
  kVpiSysFuncReal     = vpiSysFuncReal,
  kVpiSysFuncTime     = vpiSysFuncTime,
  kVpiSysSizedFunc    = vpiSysFuncSized
};


//////////////////////////////////////////////////////////////////////
/// @brief delay の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum tVpiDelayType {
  kVpiModPathDelay      = vpiModPathDelay,
  kVpiInterModPathDelay = vpiInterModPathDelay,
  kVpiMIPDelay          = vpiMIPDelay
};


//////////////////////////////////////////////////////////////////////
/// @brief strength の値
//////////////////////////////////////////////////////////////////////
enum tVpiStrength {
  kVpiNoStrength   = 0,
  kVpiSupplyDrive  = 1,
  kVpiStrongDrive  = 2,
  kVpiPullDrive    = 3,
  kVpiWeakDrive    = 4,
  kVpiLargeCharge  = 5,
  kVpiMediumCharge = 6,
  kVpiSmallCharge  = 7,
  kVpiHiZ          = 8
};


//////////////////////////////////////////////////////////////////////
/// @brief 変数のデータ型
//////////////////////////////////////////////////////////////////////
enum tVpiVarType {
  kVpiVarNone     = 0,
  kVpiVarInteger  = 1,
  kVpiVarReal     = 2,
  kVpiVarTime     = 3,
  kVpiVarRealtime = 4
};


//////////////////////////////////////////////////////////////////////
/// @brief 式の型
/// @note ちょっと紛らわしいのはサイズなしの情報とサイズ込みの情報
/// をともに tVpiValueType と言っている．
//////////////////////////////////////////////////////////////////////
enum tVpiValueType {
  /// @brief 型無し(エラー値)
  kVpiValueNone      = 0,

  /// @brief 符号つき型のビット
  kVpiValueSigned    = 1,
  /// @brief サイズつき型のビット
  kVpiValueSized     = 2,
  /// @brief ビットベクタ型のビット
  kVpiValueBitVector = 4,
  /// @brief 実数型のビット
  kVpiValueRealBit   = 8,

  /// @brief 符号無し，サイズ無しのビットベクタ
  kVpiValueUU        = kVpiValueBitVector,
  /// @brief 符号無し，サイズありのビットベクタ
  kVpiValueUS        = kVpiValueBitVector | kVpiValueSized,
  /// @brief 符号つき，サイズ無しのビットベクタ
  kVpiValueSU        = kVpiValueBitVector | kVpiValueSigned,
  /// @brief 符号つき，サイズありのビットベクタ
  kVpiValueSS        = kVpiValueBitVector | kVpiValueSigned | kVpiValueSized,

  /// @brief 実数型
  kVpiValueRealType  = kVpiValueRealBit | kVpiValueSigned | kVpiValueSized
};

/// @brief ビットベクタ型を作る．
/// @param[in] type 元となるタイプ
/// @param[in] size サイズ
tVpiValueType
pack(tVpiValueType type,
     ymuint32 size);

/// @brief 2つの型をマージする．
/// @param[in] vtype1, vtype2 型
tVpiValueType
merge(tVpiValueType vtype1,
      tVpiValueType vtype2);

/// @brief 型を取り出す．
/// @param[in] packed_type 型とサイズをパックしたもの．
tVpiValueType
unpack_type(tVpiValueType packed_type);

/// @brief サイズを取り出す．
/// @param[in] packed_type 型とサイズをパックしたもの．
ymuint32
unpack_size(tVpiValueType type);

/// @brief 符号つきの型かどうかのチェック
/// @param[in] type 型
/// @return 符号つきの型の時 true を返す．
bool
is_signed_type(tVpiValueType type);

/// @brief サイズつきの型かどうかのチェック
/// @param[in] type 型
/// @return サイズつきの型の時 true を返す．
bool
is_sized_type(tVpiValueType type);

/// @brief ビットベクタ型かどうかのチェック
/// @param[in] type 型
/// @return ビットベクタ型の時 true を返す．
bool
is_bitvector_type(tVpiValueType type);

/// @brief integer 型のサイズ
const ymuint32 kVpiSizeInteger = 32U;

/// @brief real 型のサイズ
const ymuint32 kVpiSizeReal = 64U;

/// @brief time 型のサイズ
const ymuint32 kVpiSizeTime = 64U;

/// @brief integer 型
const tVpiValueType kVpiValueInteger = pack(kVpiValueSS, kVpiSizeInteger);

/// @brief real 型
const tVpiValueType kVpiValueReal = pack(kVpiValueRealType, kVpiSizeReal);

/// @brief time 型
const tVpiValueType kVpiValueTime = pack(kVpiValueUS, kVpiSizeTime);


//////////////////////////////////////////////////////////////////////
/// @brief 範囲指定のモード
//////////////////////////////////////////////////////////////////////
enum tVpiRangeMode {
  /// @brief 範囲指定なし
  kVpiNoRange    = 0,
  /// @brief 固定
  kVpiConstRange = 1, // [ a : b ]
  /// @brief 可変(プラス)
  kVpiPlusRange  = 2, // [ a +: b ]
  /// @brief 可変(マイナス)
  kVpiMinusRange = 3  // [ a -: b ]
};


//////////////////////////////////////////////////////////////////////
/// @brief IO宣言で用いられる補助的な型
//////////////////////////////////////////////////////////////////////
enum tVpiAuxType {
  /// @brief 型無し
  kVpiAuxNone = 0,
  /// @brief net 型
  kVpiAuxNet  = 1,
  /// @brief reg 型
  kVpiAuxReg  = 2,
  /// @brief 変数型
  kVpiAuxVar  = 3
};


//////////////////////////////////////////////////////////////////////
/// @brief vectored/scalared 指定の値
//////////////////////////////////////////////////////////////////////
enum tVpiVsType {
  /// @brief 指定なし
  kVpiVsNone   = 0,
  /// @brief ベクター型
  kVpiVectored = 1,
  /// @brief スカラー型
  kVpiScalared = 2
};


//////////////////////////////////////////////////////////////////////
/// @brief specify block item の種類
//////////////////////////////////////////////////////////////////////
enum tVpiSpecItemType {
  kVpiPulsestyleOnEvent  = 0,
  kVpiPulsestyleOnDetect = 1,
  kVpiShowcancelled      = 2,
  kVpiNoshowcancelled    = 3
};


//////////////////////////////////////////////////////////////////////
/// @brief specify block path の種類
//////////////////////////////////////////////////////////////////////
enum tVpiSpecPathType {
  kVpiSpecPathNull   = 0,
  kVpiSpecPathIf     = 1,
  kVpiSpecPathIfnone = 2
};


//////////////////////////////////////////////////////////////////////
/// @brief UDP のテーブルで使われる値
//////////////////////////////////////////////////////////////////////
enum tVpiUdpVal {
  kVpiUdpVal0  = 1 << 0,
  kVpiUdpVal1  = 1 << 1,
  kVpiUdpValX  = 1 << 2,
  kVpiUdpValB  = kVpiUdpVal0 | kVpiUdpVal1,
  kVpiUdpValQ  = kVpiUdpValB | kVpiUdpValX,

  kVpiUdpVal00 = kVpiUdpVal0 << 3,
  kVpiUdpVal01 = kVpiUdpVal1 << 3,
  kVpiUdpVal0X = kVpiUdpValX << 3,
  kVpiUdpVal0B = kVpiUdpVal00 | kVpiUdpVal01,
  kVpiUdpVal0Q = kVpiUdpVal0B | kVpiUdpVal0X,

  kVpiUdpVal10 = kVpiUdpVal0 << 6,
  kVpiUdpVal11 = kVpiUdpVal1 << 6,
  kVpiUdpVal1X = kVpiUdpValX << 6,
  kVpiUdpVal1B = kVpiUdpVal10 | kVpiUdpVal11,
  kVpiUdpVal1Q = kVpiUdpVal1B | kVpiUdpVal1X,

  kVpiUdpValX0 = kVpiUdpVal0 << 9,
  kVpiUdpValX1 = kVpiUdpVal1 << 9,
  kVpiUdpValXX = kVpiUdpValX << 9,
  kVpiUdpValXB = kVpiUdpValX0 | kVpiUdpValX1,
  kVpiUdpValXQ = kVpiUdpValXB | kVpiUdpValXX,

  kVpiUdpValB0 = kVpiUdpVal00 | kVpiUdpVal10,
  kVpiUdpValB1 = kVpiUdpVal01 | kVpiUdpVal11,
  kVpiUdpValBX = kVpiUdpVal0X | kVpiUdpVal1X,
  kVpiUdpValBB = kVpiUdpValB0 | kVpiUdpValB1,
  kVpiUdpValBQ = kVpiUdpValBB | kVpiUdpValBX,

  kVpiUdpValQ0 = kVpiUdpValB0 | kVpiUdpValX0,
  kVpiUdpValQ1 = kVpiUdpValB1 | kVpiUdpValX1,
  kVpiUdpValQX = kVpiUdpValBX | kVpiUdpValXX,
  kVpiUdpValQB = kVpiUdpValQ0 | kVpiUdpValQ1,
  kVpiUdpValQQ = kVpiUdpValQB | kVpiUdpValQX,

  kVpiUdpValP  = kVpiUdpVal01 | kVpiUdpVal0X | kVpiUdpValX1,
  kVpiUdpValN  = kVpiUdpVal10 | kVpiUdpVal1X | kVpiUdpValX0,

  kVpiUdpValR  = kVpiUdpVal01,
  kVpiUdpValF  = kVpiUdpVal10,

  kVpiUdpValNC = 0
};

/// @brief 遷移シンボル (エッジシンボル) のチェック
/// @param[in] val UDP テーブル中のシンボル値
/// @return val が遷移シンボル(エッジシンボル)なら true を返す．
bool
is_edge_symbol(tVpiUdpVal val);

/// @brief UDP テーブルのシンボルを表す文字列を得る．
/// @param[in] val UDP テーブル中のシンボル値
/// @return val を表す文字列
string
symbol2string(tVpiUdpVal val);

// @brief UDP テーブルのシンボルを 2バイトの文字にする．
// @param[in] val UDP テーブルのシンボル値
// @return val を表す2バイトの文字
int
symbol2dbyte(tVpiUdpVal val);

/// @brief シンボルのマージ
/// @param[in] symbol1 シンボル1
/// @param[in] symbol2 シンボル2
/// @return symbol1 と symbol2 をあわせたシンボルを返す．
/// @note symbol1, symbol2 ともにレベルシンボルでなければならない．
tVpiUdpVal
merge_udp_value(tVpiUdpVal symbol1,
		tVpiUdpVal symbol2);

/// @}


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief tVpiScalarVal から bool への変換
inline
bool
conv_to_bool(tVpiScalarVal src)
{
  return src == kVpiScalar1;
}

// @brief tVpiScalarVal から論理値への変換
// @note kVpiScalarZ -> kVpiScalarX をやっている
inline
tVpiScalarVal
conv_to_logic(tVpiScalarVal src)
{
  switch ( src ) {
  case kVpiScalar0: return kVpiScalar0;
  case kVpiScalar1: return kVpiScalar1;
  case kVpiScalarX:
  case kVpiScalarZ: return kVpiScalarX;
  }
  assert_not_reached(__FILE__, __LINE__);
  // ダミー
  return kVpiScalarX;
}

// @brief tVpiScalarVal から int への変換
// @note kVpiScalar1 以外はすべての 0 にマッピングする．
inline
int
conv_to_int(tVpiScalarVal src)
{
  if ( src == kVpiScalar1 ) {
    return 1;
  }
  else {
    return 0;
  }
}

// @brief tVpiScalarVal から実数値への変換
// @note kVpiScalar1 を 1.0 に，それい以外はすべての 0.0 にマッピングする．
inline
double
conv_to_real(tVpiScalarVal src)
{
  if ( src == kVpiScalar1 ) {
    return 1.0;
  }
  else {
    return 0.0;
  }
}

// @brief int から tVpiScalarVal への変換
// @note 0 なら kVpiScalar0 に，それ以外は kVpiScalar1 にマッピングする．
inline
tVpiScalarVal
conv_to_scalar(int src)
{
  if ( src == 0 ) {
    return kVpiScalar0;
  }
  else {
    return kVpiScalar1;
  }
}

// @brief ymuint から tVpiScalarVal への変換
// @note 0 なら kVpiScalar0 に，それ以外は kVpiScalar1 にマッピングする．
inline
tVpiScalarVal
conv_to_scalar(ymuint src)
{
  return conv_to_scalar(static_cast<int>(src));
}

// @brief 実数値から tVpiScalarVal への変換
// @note 0.0 なら kVpiScalar0 に，それ以外は kVpiScalar1 にマッピングする．
inline
tVpiScalarVal
conv_to_scalar(double src)
{
  if ( src == 0.0 ) {
    return kVpiScalar0;
  }
  else {
    return kVpiScalar1;
  }
}

// @brief tVpiScalarVal の否定
// @note kVpiScalarZ は kVpiScalarX と同様に扱われる．
inline
tVpiScalarVal
operator!(tVpiScalarVal src)
{
  switch ( src ) {
  case kVpiScalar0: return kVpiScalar1;
  case kVpiScalar1: return kVpiScalar0;
  case kVpiScalarX:
  case kVpiScalarZ: return kVpiScalarX;
  }
  assert_not_reached(__FILE__, __LINE__);
  // ダミー
  return kVpiScalarX;
}

// @brief tVpiScalarVal の AND
inline
tVpiScalarVal
operator&&(tVpiScalarVal src1,
	   tVpiScalarVal src2)
{
  if ( src1 == kVpiScalar0 || src2 == kVpiScalar0 ) {
    return kVpiScalar0;
  }
  if ( src1 == kVpiScalar1 && src2 == kVpiScalar1 ) {
    return kVpiScalar1;
  }
  return kVpiScalarX;
}

// @brief tVpiScalarVal の OR
inline
tVpiScalarVal
operator||(tVpiScalarVal src1,
	   tVpiScalarVal src2)
{
  if ( src1 == kVpiScalar1 || src2 == kVpiScalar1 ) {
    return kVpiScalar1;
  }
  if ( src1 == kVpiScalar0 && src2 == kVpiScalar0 ) {
    return kVpiScalar0;
  }
  return kVpiScalarX;
}

// @brief tVpiScalarVal の等価比較
inline
tVpiScalarVal
eq(tVpiScalarVal src1,
   tVpiScalarVal src2)
{
  if ( src1 == kVpiScalarX || src1 == kVpiScalarZ ||
       src2 == kVpiScalarX || src2 == kVpiScalarZ ) {
    return kVpiScalarX;
  }
  if ( src1 == src2 ) {
    return kVpiScalar1;
  }
  return kVpiScalar0;
}

// @brief tVpiScalarVal の等価比較
inline
tVpiScalarVal
neq(tVpiScalarVal src1,
    tVpiScalarVal src2)
{
  if ( src1 == kVpiScalarX || src1 == kVpiScalarZ ||
       src2 == kVpiScalarX || src2 == kVpiScalarZ ) {
    return kVpiScalarX;
  }
  if ( src1 != src2 ) {
    return kVpiScalar1;
  }
  return kVpiScalar0;
}

// @brief ビットベクタ型を作る．
inline
tVpiValueType
pack(tVpiValueType type,
     ymuint32 size)
{
  ymuint32 tmp = type | (size << 4);
  return static_cast<tVpiValueType>(tmp);
}

// @brief 2つの型をマージする．
inline
tVpiValueType
merge(tVpiValueType vtype1,
      tVpiValueType vtype2)
{
  ymuint32 tmp
    = static_cast<ymuint32>(vtype1) | static_cast<ymuint32>(vtype2);
  return static_cast<tVpiValueType>(tmp);
}

// @brief 型を取り出す．
inline
tVpiValueType
unpack_type(tVpiValueType packed_type)
{
  ymuint32 tmp = static_cast<ymuint32>(packed_type) & 0xF;
  return static_cast<tVpiValueType>(tmp);
}

// @brief サイズを取り出す．
inline
ymuint32
unpack_size(tVpiValueType type)
{
  return (static_cast<ymuint32>(type) >> 4);
}

// @brief 符号つきの型かどうかのチェック
// @return 符号つきの型の時 true を返す．
inline
bool
is_signed_type(tVpiValueType type)
{
  return (static_cast<ymuint32>(type) & kVpiValueSigned) == kVpiValueSigned;
}

// @brief サイズつきの型かどうかのチェック
// @return サイズつきの型の時 true を返す．
inline
bool
is_sized_type(tVpiValueType type)
{
  return (static_cast<ymuint32>(type) & kVpiValueSized) == kVpiValueSized;
}

// @brief ビットベクタ型かどうかのチェック
// @return ビットベクタ型の時 true を返す．
inline
bool
is_bitvector_type(tVpiValueType type)
{
  return (static_cast<ymuint>(type) & kVpiValueBitVector) == kVpiValueBitVector;
}

/// @brief 遷移シンボル (エッジシンボル) のチェック
/// @param[in] val
/// @return val が遷移シンボル(エッジシンボル)なら true を返す．
inline
bool
is_edge_symbol(tVpiUdpVal val)
{
  return (val >= 8);
}

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_YM

using nsVerilog::BitVector;
using nsVerilog::VlLineWatcher;
using nsVerilog::VlMgr;
using nsVerilog::VlTime;

END_NAMESPACE_YM

#endif // YM_VERILOG_VERILOG_H
