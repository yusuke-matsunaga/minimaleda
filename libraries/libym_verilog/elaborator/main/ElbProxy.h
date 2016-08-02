#ifndef LIBYM_VERILOG_ELABORATOR_ELBPROXY_H
#define LIBYM_VERILOG_ELABORATOR_ELBPROXY_H

/// @file libym_verilog/elaborator/ElbProxy.h
/// @brief ElbProxy のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbProxy.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ym_verilog/pt/PtArray.h"
#include "ym_utils/MsgHandler.h"
#include "Elaborator.h"
#include "ElbMgr.h"
#include "ElbFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class ElbEnv;
class ElbParamCon;

//////////////////////////////////////////////////////////////////////
/// @class ElbProxy ElbProxy.h "ElbProxy.h"
/// @brief Elaborator の代理クラス
/// @note このクラスを継承して用いる．
//////////////////////////////////////////////////////////////////////
class ElbProxy
{
protected:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  ElbProxy(Elaborator& elab,
	   ElbMgr& elb_mgr,
	   ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~ElbProxy();


public:
  //////////////////////////////////////////////////////////////////////
  // 初期化
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] module_gen モジュール生成用のオブジェクト
  /// @param[in] decl_gen 宣言要素生成用のオブジェクト
  /// @param[in] item_gen 構成要素生成用のオブジェクト
  /// @param[in] stmt_gen ステートメント生成用のオブジェクト
  /// @param[in] expr_gen 式生成用のオブジェクト
  /// @param[in] attr_gen 属性生成用のオブジェクト
  void
  init(ModuleGen* module_gen,
       DeclGen* decl_gen,
       ItemGen* item_gen,
       StmtGen* stmt_gen,
       ExprGen* expr_gen,
       AttrGen* attr_gen);


protected:
  //////////////////////////////////////////////////////////////////////
  // コンパイルオプション
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する宣言に範囲があり，IO宣言に範囲がない記述を認めるか
  bool
  allow_empty_io_range();


protected:
  //////////////////////////////////////////////////////////////////////
  // 検索用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前から UDP 定義を取出す．
  /// @param[in] name 名前
  /// @return name という名の UDP を返す．
  /// @return なければ NULL を返す．
  const ElbUdpDefn*
  find_udp(const char* name) const;

  /// @brief 名前から UserSystf を取出す．
  /// @param[in] name 名前
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ NULL を返す．
  const ElbUserSystf*
  find_user_systf(const char* name) const;

  /// @brief スコープと名前から名前付き要素を取り出す．
  /// @param[in] parent 検索対象のスコープ
  /// @param[in] name 名前
  /// @return parent というスコープ内の name という要素を返す．
  /// @return なければ NULL を返す．
  ElbObjHandle*
  find_obj(const VlNamedObj* parent,
	   const char* name) const;

  /// @brief 名前によるオブジェクトの探索
  /// @param[in] base_scope 起点となるスコープ
  /// @param[in] nb_array 階層名の上部 (NULL の場合も有りうる)
  /// @param[in] name 末尾の名前
  /// @param[in] ulimit 探索する名前空間の上限
  /// @return 見付かったオブジェクトを返す．
  /// 見付からなかったら NULL を返す．
  ElbObjHandle*
  find_obj_up(const VlNamedObj* base_scope,
	      PtNameBranchArray nb_array,
	      const char* name,
	      const VlNamedObj* ulimit);

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

  /// @brief パース木の属性定義から属性リストを取り出す．
  /// @param[in] pt_attr パース木の属性定義
  ElbAttrList*
  find_attr_list(const PtAttrInst* pt_attr) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief UDP を登録する．
  /// @param[in] def_name 定義名
  /// @param[in] udp 登録する UDP
  void
  reg_udp(const char* def_name,
	  const ElbUdpDefn* udp);

  /// @brief generate block を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_genblock(ElbScope* obj);

  /// @brief block scope を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_blockscope(ElbScope* obj);

  /// @brief タスクを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_task(ElbTaskFunc* obj);

  /// @brief 関数を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_function(ElbTaskFunc* obj);

  /// @brief 宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_decl(int tag,
	   ElbDecl* obj);

  /// @brief 配列型の宣言要素を登録する．
  /// @param[in] tag タグ
  /// @param[in] obj 登録するオブジェクト
  void
  reg_declarray(int tag,
		ElbDeclArray* obj);

  /// @brief モジュール配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_modulearray(ElbModuleArray* obj);

  /// @brief ElbModule を登録する．
  /// @param[in] module 登録するモジュール
  void
  reg_module(ElbModule* module);

  /// @brief プリミティブ配列を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primarray(ElbPrimArray* obj);

  /// @brief プリミティブを登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_primitive(ElbPrimitive* obj);

  /// @brief defparam を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_defparam(ElbDefParam* obj);

  /// @brief paramassign を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_paramassign(ElbParamAssign* obj);

  /// @brief continuous assignment を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_contassign(ElbContAssign* obj);

  /// @brief process を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_process(ElbProcess* obj);

  /// @brief genvar を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_genvar(ElbGenvar* obj);

  /// @brief gfroot を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_gfroot(ElbGfRoot* obj);

  /// @brief constant function を登録する．
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  /// @param[in] func 関数
  void
  reg_constant_function(const VlNamedObj* parent,
			const char* name,
			ElbTaskFunc* func);

  /// @brief 属性リストを登録する．
  /// @param[in] パース木の属性定義
  /// @param[in] attr_list 属性リスト
  void
  reg_attr_list(const PtAttrInst* pt_attr,
		ElbAttrList* attr_list);

  /// @brief 属性リストを登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] attr_list 属性リスト
  void
  reg_attr(const VlObj* obj,
	   bool def,
	   ElbAttrList* attr_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // elaboration で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後で処理する defparam 文を登録する．
  /// @param[in] header アイテムテンプレートのヘッダ (defparam を持つ)
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

  /// @brief 1引数版の ElbStub を作る．
  template<typename T,
	   typename A>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A),
	    A a);

  /// @brief 2引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A, B),
	    A a,
	    B b);

  /// @brief 3引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B,
	   typename C>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A, B, C),
	    A a,
	    B b,
	    C c);

  /// @brief 4引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B,
	   typename C,
	   typename D>
  ElbStub*
  make_stub(T* obj,
	    void (T::*memfunc)(A, B, C, D),
	    A a,
	    B b,
	    C c,
	    D d);


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の生成関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module の中身のうちスコープに関係するインスタンス化を行う．
  /// @param[in] modle モジュール
  /// @param[in] pt_module モジュール定義
  /// @param[in] param_con パラメータ割り当ての情報
  void
  phase1_module_item(ElbModule* module,
		     const PtModule* pt_module,
		     const ElbParamCon* param_con);


protected:
  //////////////////////////////////////////////////////////////////////
  // 宣言要素のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief IO宣言要素を実体化する．
  /// @param[in] module 親のモジュール
  /// @param[in] taskfunc 親のタスク/関数
  /// @param[in] pt_head_array IO宣言ヘッダの配列
  /// @note module, task, function は1つのみが値を持つ．残りは NULL．
  void
  instantiate_iodecl(ElbModule* module,
		     ElbTaskFunc* taskfunc,
		     PtIOHeadArray pt_head_array);

  /// @brief 宣言要素のリストをインスタンス化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head_array 宣言ヘッダの配列
  void
  instantiate_decl(const VlNamedObj* parent,
		   PtDeclHeadArray pt_head_array);

  /// @brief パラメータ用の instantiate 関数
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head_array 宣言ヘッダの配列
  /// @param[in] is_local local_param の時 true
  void
  instantiate_param(const VlNamedObj* parent,
		    PtDeclHeadArray pt_head_array,
		    bool is_local);


protected:
  //////////////////////////////////////////////////////////////////////
  // 構成要素 (module item) のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_item_array 要素定義の配列
  void
  phase1_item(const VlNamedObj* parent,
	      PtItemArray pt_item_array);

  /// @brief constant function の生成を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_function 関数定義
  ElbTaskFunc*
  instantiate_constant_function(const VlNamedObj* parent,
				const PtItem* pt_function);


protected:
  //////////////////////////////////////////////////////////////////////
  // ステートメントのインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係するステートメントの実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_stmt 対象のステートメント
  /// @param[in] cf constant function 中のステートメントの時 true
  void
  phase1_stmt(const VlNamedObj* parent,
	      const PtStmt* pt_stmt,
	      bool cf = false);

  /// @brief ステートメントの実体化を行う．
  /// @param[in] parent 親のスコープ
  /// @param[in] process 親のプロセス
  /// @param[in] env 生成時の環境
  /// @param[in] pt_stmt 対象のステートメント
  ElbStmt*
  instantiate_stmt(const VlNamedObj* parent,
		   ElbProcess* process,
		   const ElbEnv& env,
		   const PtStmt* pt_stmt);


protected:
  //////////////////////////////////////////////////////////////////////
  // 式のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr から ElbExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_expr(const VlNamedObj* parent,
		   const ElbEnv& env,
		   const PtExpr* pt_expr);

  /// @brief PtExpr から定数式の ElbExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_constant_expr(const VlNamedObj* parent,
			    const PtExpr* pt_expr);

  /// @brief PtExpr からイベント式の ElbExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_event_expr(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr);

  /// @brief PtExpr からシステム関数の引数を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_arg(const VlNamedObj* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtExpr から左辺式を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_lhs(const VlNamedObj* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtExpr から右辺式を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] lhs 左辺式
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_rhs(const VlNamedObj* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr,
		  ElbExpr* lhs);

  /// @brief 範囲を表す式を生成
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_left 範囲のMSBを表すパース木
  /// @param[in] pt_right 範囲のLSBを表すパース木
  /// @param[out] left 範囲の MSB の式
  /// @param[out] right 範囲の LSB の式
  /// @param[out] left_val 範囲の MSB の値
  /// @param[out] right_val 範囲の LSB の値
  bool
  instantiate_range(const VlNamedObj* parent,
		    const PtExpr* pt_left,
		    const PtExpr* pt_right,
		    ElbExpr*& left,
		    ElbExpr*& right,
		    int& left_val,
		    int& right_val);

  /// @brief PtExpr(primary) から named_event を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  ElbDecl*
  instantiate_namedevent(const VlNamedObj* parent,
			 const PtExpr* pt_expr);

  /// @brief PtDelay から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_delay 遅延を表すパース木
  ElbDelay*
  instantiate_delay(const VlNamedObj* parent,
		    const PtDelay* pt_delay);

  /// @brief PtOrderedCon から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 順序付きわりあて式
  /// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
  /// 遅延なのかわからないので PtOrderedCon で表していることによる．
  ElbDelay*
  instantiate_delay(const VlNamedObj* parent,
		    const PtItem* pt_head);

  /// @brief PtExpr を評価し int 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_expr_int(const VlNamedObj* parent,
		    const PtExpr* pt_expr,
		    int& value);

  /// @brief PtExpr を評価し bool 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_expr_bool(const VlNamedObj* parent,
		     const PtExpr* pt_expr,
		     bool& value);

  /// @brief PtExpr を評価しビットベクタ値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_expr_bitvector(const VlNamedObj* parent,
			  const PtExpr* pt_expr,
			  BitVector& value);

  /// @brief 式を int 値に変換する．
  /// @return 変換に成功したら true を返す．
  /// @note 変換に失敗したらエラーメッセージを出力する．
  bool
  expr_to_int(ElbExpr* expr,
	      int& val);


protected:
  //////////////////////////////////////////////////////////////////////
  // attribute instance の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtAttrInst から属性リストを生成し，オブジェクトに付加する．
  /// @param[in] pt_attr 属性を表すパース木
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] obj 付加する対象のオブジェクト
  void
  instantiate_attribute(PtAttrInstArray pt_attr_array,
			bool def,
			const VlObj* obj);


protected:
  //////////////////////////////////////////////////////////////////////
  // メッセージの出力
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const string& msg);


protected:
  //////////////////////////////////////////////////////////////////////
  // メンバへのアクセス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファクトリオブジェクトを得る．
  ElbFactory&
  factory();

  /// @brief メッセージマネージャを得る．
  MsgMgr&
  msg_mgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の生成器
  Elaborator& mElaborator;

  // 生成したオブジェクトを管理するマネージャクラス
  ElbMgr& mMgr;

  // オブジェクトを生成するファクトリクラス
  ElbFactory& mFactory;

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

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前から UDP 定義を取出す．
// @param[in] name 名前
// @return name という名の UDP を返す．
// @return なければ NULL を返す．
inline
const ElbUdpDefn*
ElbProxy::find_udp(const char* name) const
{
  return mMgr.find_udp(name);
}

// @brief 名前から UserSystf を取出す．
// @param[in] name 名前
// @return name という名のユーザー定義関数を返す．
// @return なければ NULL を返す．
inline
const ElbUserSystf*
ElbProxy::find_user_systf(const char* name) const
{
  return mMgr.find_user_systf(name);
}

// @brief スコープと名前から名前付き要素を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という要素を返す．
// @return なければ NULL を返す．
inline
ElbObjHandle*
ElbProxy::find_obj(const VlNamedObj* parent,
		   const char* name) const
{
  return mMgr.find_obj(parent, name);
}

// @brief 名前によるオブジェクトの探索
// @param[in] base_scope 起点となるスコープ
// @param[in] nb_array 階層名の上部 (NULL の場合も有りうる)
// @param[in] name 末尾の名前
// @param[in] ulimit 探索する名前空間の上限
// @return 見付かったオブジェクトを返す．
// 見付からなかったら NULL を返す．
inline
ElbObjHandle*
ElbProxy::find_obj_up(const VlNamedObj* base_scope,
		      PtNameBranchArray nb_array,
		      const char* name,
		      const VlNamedObj* ulimit)
{
  return mMgr.find_obj_up(base_scope, nb_array, name, ulimit);
}

// @brief UDP を登録する．
// @param[in] def_name 定義名
// @param[in] udp 登録する UDP
inline
void
ElbProxy::reg_udp(const char* def_name,
		  const ElbUdpDefn* udp)
{
  mMgr.reg_udp(def_name, udp);
}

// @brief generate block を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_genblock(ElbScope* obj)
{
  mMgr.reg_genblock(obj);
}

// @brief block scope を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_blockscope(ElbScope* obj)
{
  mMgr.reg_blockscope(obj);
}

// @brief タスクを登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_task(ElbTaskFunc* obj)
{
  mMgr.reg_task(obj);
}

// @brief 関数を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_function(ElbTaskFunc* obj)
{
  mMgr.reg_function(obj);
}

// @brief 宣言要素を登録する．
/// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_decl(int tag,
		   ElbDecl* obj)
{
  mMgr.reg_decl(tag, obj);
}

// @brief 配列型の宣言要素を登録する．
/// @param[in] tag タグ
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_declarray(int tag,
			ElbDeclArray* obj)
{
  mMgr.reg_declarray(tag, obj);
}

// @brief モジュール配列を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_modulearray(ElbModuleArray* obj)
{
  mMgr.reg_modulearray(obj);
}

// @brief モジュールを登録する．
// @param[in] module 登録するモジュール
inline
void
ElbProxy::reg_module(ElbModule* module)
{
  mMgr.reg_module(module);
}

// @brief プリミティブ配列を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_primarray(ElbPrimArray* obj)
{
  mMgr.reg_primarray(obj);
}

// @brief プリミティブを登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_primitive(ElbPrimitive* obj)
{
  mMgr.reg_primitive(obj);
}

// @brief defparam を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_defparam(ElbDefParam* obj)
{
  mMgr.reg_defparam(obj);
}

// @brief paramassign を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_paramassign(ElbParamAssign* obj)
{
  mMgr.reg_paramassign(obj);
}

// @brief continuous assignment を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_contassign(ElbContAssign* obj)
{
  mMgr.reg_contassign(obj);
}

// @brief process を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_process(ElbProcess* obj)
{
  mMgr.reg_process(obj);
}

// @brief genvar を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_genvar(ElbGenvar* obj)
{
  mMgr.reg_genvar(obj);
}

// @brief gfroot を登録する．
// @param[in] obj 登録するオブジェクト
inline
void
ElbProxy::reg_gfroot(ElbGfRoot* obj)
{
  mMgr.reg_gfroot(obj);
}

// @brief 名前からモジュール定義を取り出す．
// @param[in] name 名前
// @return name という名のモジュール定義
// @return なければ NULL を返す．
inline
const PtModule*
ElbProxy::find_moduledef(const char* name) const
{
  return mElaborator.find_moduledef(name);
}

// @brief constant function を取り出す．
// @param[in] parent 検索対象のスコープ
// @param[in] name 名前
// @return parent というスコープ内の name という関数を返す．
// @return なければ NULL を返す．
inline
const ElbTaskFunc*
ElbProxy::find_constant_function(const VlNamedObj* parent,
				 const char* name) const
{
  return mElaborator.find_constant_function(parent, name);
}

// @brief constant function を登録する．
// @param[in] parent 親のスコープ
// @param[in] name 名前
// @param[in] func 関数
inline
void
ElbProxy::reg_constant_function(const VlNamedObj* parent,
				const char* name,
				ElbTaskFunc* func)
{
  mElaborator.reg_constant_function(parent, name, func);
}

// @brief パース木の属性定義から属性リストを取り出す．
// @param[in] pt_attr パース木の属性定義
inline
ElbAttrList*
ElbProxy::find_attr_list(const PtAttrInst* pt_attr) const
{
  return mElaborator.mAttrDict.find(pt_attr);
}

// @brief 属性リストを登録する．
// @param[in] パース木の属性定義
// @param[in] attr_list 属性リスト
inline
void
ElbProxy::reg_attr_list(const PtAttrInst* pt_attr,
			ElbAttrList* attr_list)
{
  return mElaborator.mAttrDict.add(pt_attr, attr_list);
}

// @brief 属性リストを登録する．
// @param[in] obj 対象のオブジェクト
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] attr_list 属性リスト
inline
void
ElbProxy::reg_attr(const VlObj* obj,
		   bool def,
		   ElbAttrList* attr_list)
{
  mMgr.reg_attr(obj, def, attr_list);
}

// @brief 後で処理する defparam 文を登録する．
// @param[in] module 親のモジュール
// @param[in] pt_head アイテムテンプレートのヘッダ (defparam を持つ)
inline
void
ElbProxy::add_defparamstub(const VlModule* module,
			   const PtItem* pt_head)
{
  mElaborator.add_defparamstub(module, pt_head);
}

// @brief phase1 で行う処理を登録する．
// @param[in] stub phase1 で行う処理を表すスタブ
inline
void
ElbProxy::add_phase1stub(ElbStub* stub)
{
  mElaborator.add_phase1stub(stub);
}

// @brief phase2 で行う処理を登録する．
// @param[in] stub phase2 で行う処理を表すスタブ
inline
void
ElbProxy::add_phase2stub(ElbStub* stub)
{
  mElaborator.add_phase2stub(stub);
}

// @brief phase3 で行う処理を登録する．
// @param[in] stub phase3 で行う処理を表すスタブ
inline
void
ElbProxy::add_phase3stub(ElbStub* stub)
{
  mElaborator.add_phase3stub(stub);
}

// @brief ファクトリオブジェクトを得る．
inline
ElbFactory&
ElbProxy::factory()
{
  return mFactory;
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
ElbProxy::put_msg(const char* src_file,
		  int src_line,
		  const FileRegion& file_loc,
		  tMsgType type,
		  const char* label,
		  const char* msg)
{
  return msg_mgr().put_msg(src_file, src_line, file_loc, type, label, msg);
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
ElbProxy::put_msg(const char* src_file,
		  int src_line,
		  const FileRegion& file_loc,
		  tMsgType type,
		  const char* label,
		  const string& msg)
{
  return msg_mgr().put_msg(src_file, src_line, file_loc, type, label, msg);
}

// @brief メッセージマネージャを得る．
inline
MsgMgr&
ElbProxy::msg_mgr()
{
  return mElaborator.msg_mgr();
}


//////////////////////////////////////////////////////////////////////
// テンプレート関数の定義
//////////////////////////////////////////////////////////////////////

/// @brief 1引数版の ElbStub を作る．
template<typename T,
	 typename A>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A),
		    A a)
{
  void* p = mElaborator.mAlloc.get_memory(sizeof(ElbStubT1<T, A>));
  return new (p) ElbStubT1<T, A>(obj, memfunc, a);
}

// @brief 2引数版の ElbStub を作る．
template<typename T,
	 typename A,
	 typename B>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A, B),
		    A a,
		    B b)
{
  void* p = mElaborator.mAlloc.get_memory(sizeof(ElbStubT2<T, A, B>));
  return new (p) ElbStubT2<T, A, B>(obj, memfunc, a, b);
}

// @brief 3引数版の ElbStub を作る．
template<typename T,
	 typename A,
	 typename B,
	 typename C>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A, B, C),
		    A a,
		    B b,
		    C c)
{
  void* p = mElaborator.mAlloc.get_memory(sizeof(ElbStubT3<T, A, B, C>));
  return new (p) ElbStubT3<T, A, B, C>(obj, memfunc, a, b, c);
}

// @brief 4引数版の ElbStub を作る．
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
inline
ElbStub*
ElbProxy::make_stub(T* obj,
		    void (T::*memfunc)(A, B, C, D),
		    A a,
		    B b,
		    C c,
		    D d)
{
  void* p = mElaborator.mAlloc.get_memory(sizeof(ElbStubT4<T, A, B, C, D>));
  return new (p) ElbStubT4<T, A, B, C, D>(obj, memfunc, a, b, c, d);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ELBPROXY_H
