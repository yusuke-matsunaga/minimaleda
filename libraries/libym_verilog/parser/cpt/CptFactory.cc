
/// @file libym_verilog/pt_compact/CptFactory.cc
/// @brief CptFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptFactory.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// Pt クラス(の継承クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFactory::CptFactory(AllocBase& alloc) :
  PtiFactory(alloc)
{
}

// デストラクタ
CptFactory::~CptFactory()
{
}

// @brief 文字列領域の確保
const char*
CptFactory::new_string(const char* str)
{
  ymuint32 len = strlen(str) + 1;
  char* dst = static_cast<char*>(alloc().get_memory(len));
  strcpy(dst, str);
  return dst;
}

// 確保したオブジェクトに関する情報を出力する．
void
CptFactory::dump_profile(ostream& s) const
{
  s << "mNumDefStub " << mNumDefStub<< endl;
  s << "mNumUdp " << mNumUdp<< endl;
  s << "mNumUdpEntry " << mNumUdpEntry<< endl;
  s << "mNumUdpEntryS " << mNumUdpEntryS<< endl;
  s << "mNumUdpValue " << mNumUdpValue<< endl;
  s << "mNumModule " << mNumModule<< endl;
  s << "mNumPort " << mNumPort<< endl;
  s << "mNumPortRef " << mNumPortRef<< endl;
  s << "mNumPortRefI " << mNumPortRefI<< endl;
  s << "mNumPortRefR " << mNumPortRefR<< endl;
  s << "mNumIOH " << mNumIOH<< endl;
  s << "mNumIOHV " << mNumIOHV<< endl;
  s << "mNumIOItem " << mNumIOItem<< endl;
  s << "mNumIOItemI " << mNumIOItemI<< endl;
  s << "mNumParamH " << mNumParamH<< endl;
  s << "mNumParamHV " << mNumParamHV<< endl;
  s << "mNumParamHT " << mNumParamHT<< endl;
  s << "mNumLocalParamH " << mNumLocalParamH<< endl;
  s << "mNumLocalParamHV " << mNumLocalParamHV<< endl;
  s << "mNumLocalParamHT " << mNumLocalParamHT<< endl;
  s << "mNumSpecParamH " << mNumSpecParamH<< endl;
  s << "mNumSpecParamHV " << mNumSpecParamHV<< endl;
  s << "mNumEventH " << mNumEventH<< endl;
  s << "mNumGenvarH " << mNumGenvarH<< endl;
  s << "mNumVarH " << mNumVarH<< endl;
  s << "mNumRegH " << mNumRegH<< endl;
  s << "mNumRegHV " << mNumRegHV<< endl;
  s << "mNumNetH " << mNumNetH<< endl;
  s << "mNumNetHS " << mNumNetHS<< endl;
  s << "mNumNetHD " << mNumNetHD<< endl;
  s << "mNumNetHSD " << mNumNetHSD<< endl;
  s << "mNumNetHV " << mNumNetHV<< endl;
  s << "mNumNetHVS " << mNumNetHVS<< endl;
  s << "mNumNetHVD " << mNumNetHVD<< endl;
  s << "mNumNetHVSD " << mNumNetHVSD<< endl;
  s << "mNumDeclItem " << mNumDeclItem<< endl;
  s << "mNumDeclItemI " << mNumDeclItemI<< endl;
  s << "mNumDeclItemR " << mNumDeclItemR<< endl;
  s << "mNumDefParamH " << mNumDefParamH<< endl;
  s << "mNumDefParam " << mNumDefParam<< endl;
  s << "mNumContAssignH " << mNumContAssignH<< endl;
  s << "mNumContAssignHS " << mNumContAssignHS<< endl;
  s << "mNumContAssignHD " << mNumContAssignHD<< endl;
  s << "mNumContAssignHSD " << mNumContAssignHSD<< endl;
  s << "mNumContAssign " << mNumContAssign<< endl;
  s << "mNumInitial " << mNumInitial<< endl;
  s << "mNumAlways " << mNumAlways<< endl;
  s << "mNumTask " << mNumTask<< endl;
  s << "mNumFunction " << mNumFunction<< endl;
  s << "mNumSizedFunc " << mNumSizedFunc<< endl;
  s << "mNumTypedFunc " << mNumTypedFunc<< endl;
  s << "mNumGateH " << mNumGateH<< endl;
  s << "mNumGateHS " << mNumGateHS<< endl;
  s << "mNumGateHD " << mNumGateHD<< endl;
  s << "mNumGateHSD " << mNumGateHSD<< endl;
  s << "mNumMuH " << mNumMuH<< endl;
  s << "mNumMuHS " << mNumMuHS<< endl;
  s << "mNumMuHD " << mNumMuHD<< endl;
  s << "mNumMuHSD " << mNumMuHSD<< endl;
  s << "mNumMuHP " << mNumMuHP<< endl;
  s << "mNumInst " << mNumInst<< endl;
  s << "mNumInstN " << mNumInstN<< endl;
  s << "mNumInstR " << mNumInstR<< endl;
  s << "mNumGenerate " << mNumGenerate<< endl;
  s << "mNumGenBlock " << mNumGenBlock<< endl;
  s << "mNumGenBlockN " << mNumGenBlockN<< endl;
  s << "mNumGenIf " << mNumGenIf<< endl;
  s << "mNumGenCase " << mNumGenCase<< endl;
  s << "mNumGenCaseItem " << mNumGenCaseItem<< endl;
  s << "mNumGenFor " << mNumGenFor<< endl;
  s << "mNumSpecItem " << mNumSpecItem<< endl;
  s << "mNumSpecPath " << mNumSpecPath<< endl;
  s << "mNumPathDecl " << mNumPathDecl<< endl;
  s << "mNumPathDelay " << mNumPathDelay<< endl;
  s << "mNumDisable " << mNumDisable<< endl;
  s << "mNumDisableH " << mNumDisableH<< endl;
  s << "mNumEnable " << mNumEnable<< endl;
  s << "mNumEnableH " << mNumEnableH<< endl;
  s << "mNumSysEnable " << mNumSysEnable<< endl;
  s << "mNumDcStmt " << mNumDcStmt<< endl;
  s << "mNumEcStmt " << mNumEcStmt<< endl;
  s << "mNumWait " << mNumWait<< endl;
  s << "mNumAssign " << mNumAssign<< endl;
  s << "mNumAssignC " << mNumAssignC<< endl;
  s << "mNumNbAssign " << mNumNbAssign<< endl;
  s << "mNumNbAssignC " << mNumNbAssignC<< endl;
  s << "mNumEventStmt " << mNumEventStmt<< endl;
  s << "mNumNullStmt " << mNumNullStmt<< endl;
  s << "mNumIf " << mNumIf<< endl;
  s << "mNumCase " << mNumCase<< endl;
  s << "mNumCaseX " << mNumCaseX<< endl;
  s << "mNumCaseZ " << mNumCaseZ<< endl;
  s << "mNumCaseItem " << mNumCaseItem<< endl;
  s << "mNumForever " << mNumForever<< endl;
  s << "mNumRepeat " << mNumRepeat<< endl;
  s << "mNumWhile " << mNumWhile<< endl;
  s << "mNumFor " << mNumFor<< endl;
  s << "mNumPcAssign " << mNumPcAssign<< endl;
  s << "mNumDeassign " << mNumDeassign<< endl;
  s << "mNumForce " << mNumForce<< endl;
  s << "mNumRelease " << mNumRelease<< endl;
  s << "mNumParBlock " << mNumParBlock<< endl;
  s << "mNumParBlockN " << mNumParBlockN<< endl;
  s << "mNumSeqBlock " << mNumSeqBlock<< endl;
  s << "mNumSeqBlockN " << mNumSeqBlockN<< endl;
  s << "mNumOpr1 " << mNumOpr1<< endl;
  s << "mNumOpr2 " << mNumOpr2<< endl;
  s << "mNumOpr3 " << mNumOpr3<< endl;
  s << "mNumConcat " << mNumConcat<< endl;
  s << "mNumMultiConcat " << mNumMultiConcat<< endl;
  s << "mNumMinTypMax3 " << mNumMinTypMax3<< endl;
  s << "mNumPrimary " << mNumPrimary<< endl;
  s << "mNumPrimaryI " << mNumPrimaryI<< endl;
  s << "mNumPrimaryR " << mNumPrimaryR<< endl;
  s << "mNumPrimaryIR " << mNumPrimaryIR<< endl;
  s << "mNumPrimaryH " << mNumPrimaryH<< endl;
  s << "mNumPrimaryHI " << mNumPrimaryHI<< endl;
  s << "mNumPrimaryHR " << mNumPrimaryHR<< endl;
  s << "mNumPrimaryHIR " << mNumPrimaryHIR<< endl;
  s << "mNumPrimaryCI " << mNumPrimaryCI<< endl;
  s << "mNumPrimaryCR " << mNumPrimaryCR<< endl;
  s << "mNumPrimaryHCI " << mNumPrimaryHCI<< endl;
  s << "mNumFuncCall " << mNumFuncCall<< endl;
  s << "mNumFuncCallH " << mNumFuncCallH<< endl;
  s << "mNumSysFuncCall " << mNumSysFuncCall<< endl;
  s << "mNumIntConstant1 " << mNumIntConstant1<< endl;
  s << "mNumIntConstant2 " << mNumIntConstant2<< endl;
  s << "mNumIntConstant3 " << mNumIntConstant3<< endl;
  s << "mNumRealConstant " << mNumRealConstant<< endl;
  s << "mNumStringConstant " << mNumStringConstant<< endl;
  s << "mNumDelayControl " << mNumDelayControl<< endl;
  s << "mNumEventControl " << mNumEventControl<< endl;
  s << "mNumRepeatControl " << mNumRepeatControl<< endl;
  s << "mNumOrderedCon " << mNumOrderedCon<< endl;
  s << "mNumAiOrderedCon " << mNumAiOrderedCon<< endl;
  s << "mNumNamedCon " << mNumNamedCon<< endl;
  s << "mNumAiNamedCon " << mNumAiNamedCon<< endl;
  s << "mNumStrength " << mNumStrength<< endl;
  s << "mNumDelay " << mNumDelay<< endl;
  s << "mNumNameBranch " << mNumNameBranch<< endl;
  s << "mNumNameBranchI " << mNumNameBranchI<< endl;
  s << "mNumAttrInst " << mNumAttrInst<< endl;
  s << "mNumAttrSpec " << mNumAttrSpec<< endl;
}

END_NAMESPACE_YM_VERILOG
