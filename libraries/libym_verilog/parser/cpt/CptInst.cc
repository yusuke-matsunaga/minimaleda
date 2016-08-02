
/// @file libym_verilog/pt_compact/CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptInst.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptInst.h"
#include "CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateH::CptGateH(const FileRegion& file_region,
		   tVpiPrimType prim_type,
		   PtInstArray inst_array) :
  mFileRegion(file_region),
  mPrimType(prim_type),
  mInstArray(inst_array)
{
}

// デストラクタ
CptGateH::~CptGateH()
{
}

// ファイル位置を返す．
FileRegion
CptGateH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptGateH::type() const
{
  return kPtItem_GateInst;
}

// プリミティブタイプ(vpiAndPrim など)を返す．
tVpiPrimType
CptGateH::prim_type() const
{
  return mPrimType;
}

// 要素数を返す．
ymuint32
CptGateH::size() const
{
  return mInstArray.size();
}

// @brief module/UDP/gate instance 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtInst*
CptGateH::inst(ymuint32 pos) const
{
  return mInstArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateHS::CptGateHS(const FileRegion& file_region,
		     tVpiPrimType prim_type,
		     PtStrength* strength,
		     PtInstArray inst_array) :
  CptGateH(file_region, prim_type, inst_array),
  mStrength(strength)
{
}

// デストラクタ
CptGateHS::~CptGateHS()
{
}

// strength を返す．
const PtStrength*
CptGateHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateHD::CptGateHD(const FileRegion& file_region,
		     tVpiPrimType prim_type,
		     PtDelay* delay,
		     PtInstArray inst_array) :
  CptGateH(file_region, prim_type, inst_array),
  mDelay(delay)
{
}

// デストラクタ
CptGateHD::~CptGateHD()
{
}

// delay を返す．
const PtDelay*
CptGateHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay をもつ gate instance のヘッダを表すクラス      //
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGateHSD::CptGateHSD(const FileRegion& file_region,
		       tVpiPrimType prim_type,
		       PtStrength* strength,
		       PtDelay* delay,
		       PtInstArray inst_array) :
  CptGateH(file_region, prim_type, inst_array),
  mStrength(strength),
  mDelay(delay)
{
}

// デストラクタ
CptGateHSD::~CptGateHSD()
{
}

// strength を返す．
const PtStrength*
CptGateHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const PtDelay*
CptGateHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuH::CptMuH(const FileRegion& file_region,
	       const char* def_name,
	       PtInstArray inst_array) :
  mFileRegion(file_region),
  mName(def_name),
  mInstArray(inst_array)
{
}

// デストラクタ
CptMuH::~CptMuH()
{
}

// ファイル位置を返す．
FileRegion
CptMuH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
tPtItemType
CptMuH::type() const
{
  return kPtItem_MuInst;
}

// 定義名を返す．
const char*
CptMuH::name() const
{
  return mName;
}

// 要素数を取り出す．
ymuint32
CptMuH::size() const
{
  return mInstArray.size();
}

// @brief module/UDP/gate instance 要素の取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtInst*
CptMuH::inst(ymuint32 pos) const
{
  return mInstArray[pos];
}


//////////////////////////////////////////////////////////////////////
// パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHP::CptMuHP(const FileRegion& file_region,
		 const char* def_name,
		 PtConnectionArray con_array,
		 PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mParamAssignArray(con_array)
{
}

// デストラクタ
CptMuHP::~CptMuHP()
{
}

// @brief パラメータ割り当てリストの取得
PtConnectionArray
CptMuHP::paramassign_array() const
{
  return mParamAssignArray;
}


//////////////////////////////////////////////////////////////////////
// strength を持つ MuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHS::CptMuHS(const FileRegion& file_region,
		 const char* def_name,
		 PtStrength* strength,
		 PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mStrength(strength)
{
}

// デストラクタ
CptMuHS::~CptMuHS()
{
}

// strength を返す．
const PtStrength*
CptMuHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay を持つ MuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHD::CptMuHD(const FileRegion& file_region,
		 const char* def_name,
		 PtDelay* delay,
		 PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mDelay(delay)
{
}

// デストラクタ
CptMuHD::~CptMuHD()
{
}

// delay を返す．
const PtDelay*
CptMuHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay を持つ MuH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptMuHSD::CptMuHSD(const FileRegion& file_region,
		   const char* def_name,
		   PtStrength* strength,
		   PtDelay* delay,
		   PtInstArray inst_array) :
  CptMuH(file_region, def_name, inst_array),
  mStrength(strength),
  mDelay(delay)
{
}

// デストラクタ
CptMuHSD::~CptMuHSD()
{
}

// strength を返す．
const PtStrength*
CptMuHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const PtDelay*
CptMuHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のベース実装クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInst::CptInst(const FileRegion& file_region,
		 PtConnectionArray con_array) :
  mFileRegion(file_region),
  mPortArray(con_array)
{
}

// デストラクタ
CptInst::~CptInst()
{
}

// ファイル位置を返す．
FileRegion
CptInst::file_region() const
{
  return mFileRegion;
}

// @brief 名前の取得
// @return 名前
// このクラスでは NULL を返す．
const char*
CptInst::name() const
{
  return NULL;
}

// @brief 範囲の左側の式の取得
// @return 範囲の左側の式
// このクラスでは NULL を返す．
const PtExpr*
CptInst::left_range() const
{
  return NULL;
}

// @brief 範囲の右側の式の取得
// @return 範囲の右側の式
// このクラスでは NULL を返す．
const PtExpr*
CptInst::right_range() const
{
  return NULL;
}

// @brief ポート数の取得
// @return ポート数
ymuint32
CptInst::port_num() const
{
  return mPortArray.size();
}

// @brief ポートの取得
// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
const PtConnection*
CptInst::port(ymuint32 pos) const
{
  return mPortArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 名前を持つ CptInst
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInstN::CptInstN(const FileRegion& file_region,
		       const char* name,
		       PtConnectionArray con_array) :
  CptInst(file_region, con_array),
  mName(name)
{
}

// デストラクタ
CptInstN::~CptInstN()
{
}

// 名前を返す．
const char*
CptInstN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// 名前と範囲指定を持つ CptInst
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptInstR::CptInstR(const FileRegion& file_region,
		       const char* name,
		       PtExpr* left,
		       PtExpr* right,
		       PtConnectionArray con_array) :
  CptInstN(file_region, name, con_array),
  mLeftRange(left),
  mRightRange(right)
{
}

// デストラクタ
CptInstR::~CptInstR()
{
}

// range の MSB を取出す．
const PtExpr*
CptInstR::left_range() const
{
  return mLeftRange;
}

// range の LSB を取出す．
const PtExpr*
CptInstR::right_range() const
{
  return mRightRange;
}


//////////////////////////////////////////////////////////////////////
// インスタンス文
//////////////////////////////////////////////////////////////////////

// gate instance 文のヘッダを生成する．
PtItem*
CptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtInstArray inst_array)
{
  ++ mNumGateH;
  void* p = alloc().get_memory(sizeof(CptGateH));
  return new (p) CptGateH(file_region, type, inst_array);
}

// gate instance 文のヘッダを生成する．
PtItem*
CptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtStrength* strength,
		      PtInstArray inst_array)
{
  ++ mNumGateHS;
  void* p = alloc().get_memory(sizeof(CptGateHS));
  return new (p) CptGateHS(file_region, type, strength, inst_array);
}

// gate instance 文のヘッダを生成する．
PtItem*
CptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtDelay* delay,
		      PtInstArray inst_array)
{
  ++ mNumGateHD;
  void* p = alloc().get_memory(sizeof(CptGateHD));
  return new (p) CptGateHD(file_region, type, delay, inst_array);
}

// gate instance 文のヘッダを生成する．
PtItem*
CptFactory::new_GateH(const FileRegion& file_region,
		      tVpiPrimType type,
		      PtStrength* strength,
		      PtDelay* delay,
		      PtInstArray inst_array)
{
  ++ mNumGateHSD;
  void* p = alloc().get_memory(sizeof(CptGateHSD));
  return new (p) CptGateHSD(file_region,
			    type, strength, delay, inst_array);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtInstArray inst_array)
{
  ++ mNumMuH;
  void* p = alloc().get_memory(sizeof(CptMuH));
  return new (p) CptMuH(file_region, def_name,
			inst_array);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtStrength* strength,
		    PtInstArray inst_array)
{
  ++ mNumMuHS;
  void* p = alloc().get_memory(sizeof(CptMuHS));
  return new (p) CptMuHS(file_region, def_name,
			 strength, inst_array);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtDelay* delay,
		    PtInstArray inst_array)
{
  ++ mNumMuHD;
  void* p = alloc().get_memory(sizeof(CptMuHD));
  return new (p) CptMuHD(file_region, def_name,
			 delay, inst_array);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtStrength* strength,
		    PtDelay* delay,
		    PtInstArray inst_array)
{
  ++ mNumMuHSD;
  void* p = alloc().get_memory(sizeof(CptMuHSD));
  return new (p) CptMuHSD(file_region, def_name,
			  strength, delay, inst_array);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
CptFactory::new_MuH(const FileRegion& file_region,
		    const char* def_name,
		    PtConnectionArray con_array,
		    PtInstArray inst_array)
{
  ++ mNumMuHP;
  void* p = alloc().get_memory(sizeof(CptMuHP));
  return new (p) CptMuHP(file_region, def_name,
			 con_array, inst_array);
}

// module instance/UDP instance の要素を生成する．
PtInst*
CptFactory::new_Inst(const FileRegion& file_region,
		     PtConnectionArray con_array)
{
  ++ mNumInst;
  void* p = alloc().get_memory(sizeof(CptInst));
  return new (p) CptInst(file_region, con_array);
}

// module instance/UDP instance の要素を生成する．
PtInst*
CptFactory::new_InstN(const FileRegion& file_region,
		      const char* name,
		      PtConnectionArray con_array)
{
  ++ mNumInstN;
  void* p = alloc().get_memory(sizeof(CptInstN));
  return new (p) CptInstN(file_region, name, con_array);
}

// module instance/UDP instance の要素を生成する．
PtInst*
CptFactory::new_InstV(const FileRegion& file_region,
		      const char* name,
		      PtExpr* left,
		      PtExpr* right,
		      PtConnectionArray con_array)
{
  ++ mNumInstR;
  void* p = alloc().get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, left, right, con_array);
}

END_NAMESPACE_YM_VERILOG
