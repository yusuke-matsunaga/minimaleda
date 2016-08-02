#ifndef LIBYM_VERILOG_ELABORATOR_ELABORATOR_H
#define LIBYM_VERILOG_ELABORATOR_ELABORATOR_H

/// @file libym_verilog/elaborator/include/Elaborator.h
/// @brief Elaborator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Elaborator.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ym_verilog/vl/VlFwd.h"

#include "ym_utils/Alloc.h"
#include "ym_utils/MsgHandler.h"

#include "CfDict.h"
#include "AttrDict.h"
#include "ElbStub.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtMgr;

class ElbMgr;
class ElbFactory;

class UdpGen;
class ModuleGen;
class DeclGen;
class ItemGen;
class StmtGen;
class ExprGen;
class AttrGen;

//////////////////////////////////////////////////////////////////////
/// @class Elaborator Elaborator.h "Elaborator.h"
/// @brief elaboration を行うクラス
/// 基本的には 内部に状態を持たないファンクタークラス
/// 実際には elaboration 途中でオブジェクトを保持しておくハッシュ表
/// などを持つ．最終結果は引数の ElbMgr に格納される．
//////////////////////////////////////////////////////////////////////
class Elaborator
{
  friend class ElbProxy;
  
public:

  /// @brief コンストラクタ
  /// @param[in] msg_mgr メッセージマネージャ
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  Elaborator(MsgMgr& msg_mgr,
	     ElbMgr& elb_mgr,
	     ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~Elaborator();
  

public:
  
  /// @brief エラボレーションを行う．
  /// @param[in] pt_mgr パース木を管理するクラス
  /// @return エラー数を返す．
  size_t
  operator()(const PtMgr& pt_mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // elaboration で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief メッセージマネージャを取り出す．
  MsgMgr&
  msg_mgr();
  
  /// @brief 後で処理する defparam 文を登録する．
  /// @param[in] header アイテムテンプレートのヘッダ (defparam を持つ)
  /// @param[in] defparam defparam 文のテンプレート
  void
  add_defparamstub(const VlModule* module,
		   const PtItem* header);

  /// @brief phase1 で行う処理を登録する．
  /// @param[in] stub phase1 で行う処理を表すスタブ
  void
  add_phase1stub(ElbStub* stub);

  /// @brief phase2 で行う処理を登録する．
  /// @param[in] stub phase2 で行う処理を表すスタブ
  void
  add_phase2stub(ElbStub* stub);

  /// @brief phase3 で行う処理を登録する．
  /// @param[in] stub phase3 で行う処理を表すスタブ
  void
  add_phase3stub(ElbStub* stub);


private:
  //////////////////////////////////////////////////////////////////////
  // 要素を検索する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 名前からモジュール定義を取り出す．
  /// @param[in] name 名前
  /// @return name という名のモジュール定義
  /// @return なければ NULL を返す．
  const PtModule*
  find_moduledef(const char* name) const;
  
  /// @brief constant function を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という関数を返す．
  /// @return なければ NULL を返す．
  const ElbTaskFunc*
  find_constant_function(const VlNamedObj* parent,
			 const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief constant function を登録する．
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  /// @param[in] func 関数
  void
  reg_constant_function(const VlNamedObj* parent,
			const char* name,
			ElbTaskFunc* func);

  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる型定義
  //////////////////////////////////////////////////////////////////////

  struct DefParamStub
  {
    /// @brief コンストラクタ
    DefParamStub(const VlModule* module,
		 const PtItem* pt_head,
		 const PtDefParam* pt_defparam) :
      mModule(module),
      mPtHeader(pt_head),
      mPtDefparam(pt_defparam)
    {
    }
    
    /// @brief 対象のモジュール
    const VlModule* mModule;

    /// @brief パース木の DefParam ヘッダ
    const PtItem* mPtHeader;
    
    /// @brief パース木の DefParam 文
    const PtDefParam* mPtDefparam;
  };
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマネージャ
  MsgMgr& mMsgMgr;

  // 生成したオブジェクトを管理するクラス
  ElbMgr& mMgr;
  
  // オブジェクト生成用のファクトリクラス
  ElbFactory& mFactory;
  
  // ElbStub 用のメモリアロケータ
  SimpleAlloc mAlloc;
  
  // UDP 生成用のオブジェクト
  UdpGen* mUdpGen;

  // モジュール生成用のオブジェクト
  ModuleGen* mModuleGen;

  // 宣言要素生成用のオブジェクト
  DeclGen* mDeclGen;

  // 構成要素生成用のオブジェクト
  ItemGen* mItemGen;

  // ステートメント生成用のオブジェクト
  StmtGen* mStmtGen;

  // 式生成用のオブジェクト
  ExprGen* mExprGen;
  
  // attribute instance 生成用のオブジェクト
  AttrGen* mAttrGen;
  
  // constant function の辞書
  CfDict mCfDict;

  // モジュールテンプレートの辞書
  hash_map<string, const PtModule*> mModuleDict;
  
  // attribute instance の辞書
  AttrDict mAttrDict;
  
  // defparam 文の元のリスト
  list<DefParamStub> mDefParamStubList;
  
  // phase1 で生成するオブジェクトを追加するリスト
  ElbStubList mPhase1StubList1;

  // phase1 で生成するオブジェクトを読み出すリスト
  ElbStubList mPhase1StubList2;

  // phase2 生成するオブジェクトを入れたリスト
  ElbStubList mPhase2StubList;

  // phase3 で link するオブジェクトを入れたリスト
  ElbStubList mPhase3StubList;
  

private:
  //////////////////////////////////////////////////////////////////////
  // コンパイルオプション
  //////////////////////////////////////////////////////////////////////

  // IOに範囲がなく宣言のみに範囲を持つ場合を許すとき true
  bool mAllowEmptyIORange;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセージマネージャを取り出す．
inline
MsgMgr&
Elaborator::msg_mgr()
{
  return mMsgMgr;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ELABORATOR_H
