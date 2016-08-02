#ifndef YM_TCLPP_TCLCMD_H
#define YM_TCLPP_TCLCMD_H

/// @file ym_tclpp/TclCmd.h
/// @brief TclCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclCmd.h 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"
#include "ym_tclpp/TclBase.h"
#include "ym_tclpp/TclObj.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @class TclCmd TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief Tclコマンドと結びつけられているオブジェクトの基底クラス
///
/// ClientData が C++ 的に進化したものと考えれば良い．
//////////////////////////////////////////////////////////////////////
class TclCmd :
  public TclBase
{
protected:

  /// @brief コンストラクタ
  /// @note この時点では特定のインタプリタとは結び付いていない．
  TclCmd();

  /// @brief デストラクタ
  /// コマンドとのバインディングも解除する．
  /// 通常はコマンドの削除に伴ってこのデストラクタが起動されるので，
  /// bind()したらこのオブジェクトは知らないうちに解放されている
  /// 可能性があることに注意．
  virtual
  ~TclCmd();


public:

  /// @brief 特定のインタプリタ上のコマンドと結び付ける．
  /// @param[in] interp インタープリタ
  /// @param[in] cmd_name コマンド名
  /// @retval TCL_OK 登録が成功した
  /// @retval TCL_ERROR 登録が失敗した
  /// 以降，このオブジェクトはTclインタプリタの支配下に置かれる
  /// ので勝手に解放される可能性があることに注意．
  /// また，コマンドを削除せずにこのオブジェクトを破壊しても
  /// いけない．
  virtual
  int
  bind(Tcl_Interp* interp,
       const string& cmd_name);

  /// @brief コマンド名を得る．
  string
  command_name() const;

  /// @brief コマンド情報を取得する．
  /// @param[out] info コマンド情報を格納する変数
  /// @retval TCL_OK 成功した．
  /// @retval TCL_ERROR (あるとは思えないけど) 失敗した．
  int
  command_info(Tcl_CmdInfo* info) const;

  /// @brief コマンドトークン(Tcl_CreateCommand()の返り値)を得る．
  Tcl_Command
  cmd_token() const;

  /// @brief 最初の引数 (argv[0]) を得る．
  /// @note この値は cmd_proc() 中でしか適正ではない．
  const string&
  arg0() const;

  /// @brief TclPopt を登録する．
  /// @param[in] popt 登録するオプション解析用オブジェクト
  /// @note TclPopt オブジェクトはこのコマンドが削除されるときに一緒に削除される．
  void
  bind_popt(TclPopt* popt);

  /// @brief Tcl関数に渡す本当のコマンド処理関数
  friend
  int
  cmd_command_callback(ClientData clientdata,
		       Tcl_Interp* interp,
		       int objc,
		       Tcl_Obj *const objv[]);

  /// @brief コマンド削除関数
  friend
  void
  cmd_delete_callback(ClientData clientdata);


protected:

  /// @brief help/usage オプションを制御する．
  /// @param[in] enable true の時，help/usage オプションを有効にする．
  void
  autohelp(bool enable);

  /// @brief オプションの解析を行う．
  /// @param[inout] objv 引数の配列
  /// @param[out] help help/usage オプションがあったら true をセットする．
  /// @return エラーが起きたら TCL_ERROR を返す．
  /// @note パーズの結果，objv が書き換わる場合がある．
  /// @note というか書き換えないと2重にパーズされることになる．
  virtual
  int
  parse_opt(TclObjVector& objv,
	    bool& help);

  /// @brief cmd_proc() を呼び出す前に呼ばれる関数
  /// @param [inout] objv 引数の配列
  /// @note デフォルトではなにもしないで TCL_OK を返す．
  virtual
  int
  before_cmd_proc(TclObjVector& objv);

  /// @brief コマンドを実行する仮想関数
  /// @param [inout] objv 引数の配列
  /// @note 純粋仮想関数なので継承クラスで実装する必要がある．
  virtual
  int
  cmd_proc(TclObjVector& objv) = 0;

  /// @brief cmd_proc() の終了後呼ばれる仮想関数
  /// @note デフォルトでは何もしない．
  virtual
  void
  after_cmd_proc();

  /// @brief コマンドの開始を記録する．
  /// @param [inout] objv 引数の配列
  /// @note デフォルトでは何もしない．
  virtual
  void
  rec_cmd_start(const TclObjVector& objv);

  /// @brief コマンドの終了を記録する．
  /// @param[in] rcode このコマンドの終了値
  /// @note デフォルトでは何もしない．
  virtual
  void
  rec_cmd_end(int rcode);

  /// @brief help/usage で用いる説明文を設定する関数
  void
  set_usage_string(const string& str);

  /// @brief help/usage で用いる説明文を返す関数
  string
  usage_string();

  /// @brief help オプションがあったときに呼ばれる仮想関数
  virtual
  void
  print_help();

  /// @brief usage オプションがあったときに呼ばれる仮想関数
  virtual
  void
  print_usage();

  /// @brief Popt グループを生成する．
  /// @param[in] popt1, popt2, popt3, popt4, popt5, popt6 グループの要素
  /// @return 生成した TclPoptGroup を返す．
  /// @note ここで生成したオブジェクトは TclCmd が削除されるときに
  /// 同時に削除される．
  TclPoptGroup*
  new_popt_group(TclPopt* popt1 = NULL,
		 TclPopt* popt2 = NULL,
		 TclPopt* popt3 = NULL,
		 TclPopt* popt4 = NULL,
		 TclPopt* popt5 = NULL,
		 TclPopt* popt6 = NULL);

  /// @brief Popt グループに要素を追加する．
  /// @param[in] group new_popt_group() で作成した TclPoptGroup
  /// @param[in] popt 追加する TclPopt
  void
  add_popt(TclPoptGroup* group,
	   TclPopt* popt);


private:

  /// @brief コマンド処理関係の一連の仮想関数を呼んでいる真のコマンド処理関数
  /// @note これを上書きすることも可能だけど普通は必要ないはず
  virtual
  int
  _cmd_proc(TclObjVector& objv);


protected:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // コマンド名
  string mCmdName;

  // コマンドトークン(Tcl_CreateCommand()の返り値)
  Tcl_Command mCmdToken;

  // CmdProc() が呼ばれたときの最初の引数の内容
  // 通常は mCmdName と同じはずだけど，例外があるかどうかは知らない．
  // この値は CmdProc() 中でしか使えない．
  // Tcl-8.0 より導入された namespace のために，起動されたコマンド名が
  // namespace::command の場合があるため，この値は mCmdName と同一である
  // とは限らなくなった．
  string mArg0;

  // help/usage オプションを制御するフラグ
  bool mAutoHelp;

  // help/usage で用いる説明文
  string mUsageString;

  // オプション解析用オブジェクトのリスト
  list<TclPopt*> mPoptList;

  // オプション文字列をキーとして TclPopt を要素とするハッシュ表
  hash_map<string, TclPopt*> mPoptTable;

  // TclPoptGroup のリスト
  list<TclPoptGroup*> mPoptGroupList;

};


//////////////////////////////////////////////////////////////////////
/// @class TclCmdCls TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief TclCmd を生成しコマンドに結び付けるコマンドのための
/// TclCmd の派生クラス．
///
/// 要するにクラスコマンド．
///
/// 具体的には cmd_proc() 関数の中で create_obj() を呼び出して
/// コマンドオブジェクトを生成し，オブジェクト名とバインディング
/// する．
///
/// アプリケーションユーザはこのクラスを継承してオブジェクトを生成
/// する仮想関数のみを書けば良い．
//////////////////////////////////////////////////////////////////////
class TclCmdCls :
  public TclCmd
{
private:

  /// @brief コマンドを実行する仮想関数
  ///
  /// ここでコマンドオブジェクト (TclCmd)の生成を行う．
  virtual
  int
  cmd_proc(TclObjVector& objv);


protected:

  /// @brief このコマンドが起動された時にオブジェクトの生成を行う仮想関数
  ///
  /// これは継承クラスが必ず実装しなければならない．
  virtual
  TclCmd*
  create_obj() const = 0;

public:
  // コンストラクタ，デストラクタは不要

};



//////////////////////////////////////////////////////////////////////
/// @class TclCmdBinder TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief コマンドオブジェクトを生成して登録するための
/// テンプレートクラス
///
/// 実はこのクラス自体には意味はなく, ただ単にクラステンプレート
/// の仕組みを利用して関数を1つ定義するためのギミック
///
/// @verbatim
/// class cmd_cls;      // TclCmd の派生クラス
/// Tcl_Interp* interp; // Tcl インタプリタ
/// string cmd_name;    // コマンド名
/// TclCmdBinder<cmd_class>::reg(interp, cmd_name);
/// @endverbatim
/// の様に用いる．
//////////////////////////////////////////////////////////////////////
template <typename Cmd>
class TclCmdBinder
{
public:

  /// @brief コマンドオブジェクトを登録する．
  /// @param[in] interp TCLインタープリタクラスへのポインタ
  /// @param[in] cmd_name コマンド名
  static
  int
  reg(Tcl_Interp* interp,
      const string& cmd_name);
};



//////////////////////////////////////////////////////////////////////
/// @class TclCmdBinder1 TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief コマンドオブジェクトを生成して登録するための
/// テンプレートクラス
///
/// 実はこのクラス自体には意味はなく, ただ単にクラステンプレート
/// の仕組みを利用して関数を1つ定義するためのギミック
///
/// @verbatim
/// class cmd_cls;      // TclCmd の派生クラス
/// typename arg1_type  // 第一引数の型
/// Tcl_Interp* interp; // Tcl インタプリタ
/// arg1_type arg1      // cmd_cls のコンストラクタの第一引数
/// string cmd_name;    // コマンド名
/// TclCmdBinder1<cmd_class, arg1_type>::reg(interp, arg1, cmd_name);
/// @endverbatim
/// の様に用いる．
//////////////////////////////////////////////////////////////////////
template <typename Cmd,
	  typename T1>
class TclCmdBinder1
{
public:

  /// @brief コマンドオブジェクトを登録する．
  /// @param[in] interp TCLインタープリタクラスへのポインタ
  /// @param[in] arg1  コマンドオブジェクトを生成するときの引数
  /// @param[in] cmd_name コマンド名
  static
  int
  reg(Tcl_Interp* interp,
      T1 arg1,
      const string& cmd_name);
};



//////////////////////////////////////////////////////////////////////
/// @class TclCmdBinder2 TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief コマンドオブジェクトを生成して登録するための
/// テンプレートクラス
///
/// 実はこのクラス自体には意味はなく, ただ単にクラステンプレート
/// の仕組みを利用して関数を1つ定義するためのギミック
///
/// @verbatim
/// class cmd_cls;      // TclCmd の派生クラス
/// typename arg1_type  // 第1引数の型
/// typename arg2_type  // 第2引数の型
/// Tcl_Interp* interp; // Tcl インタプリタ
/// arg1_type arg1      // cmd_cls のコンストラクタの第1引数
/// arg1_type arg2      // cmd_cls のコンストラクタの第2引数
/// string cmd_name;    // コマンド名
/// TclCmdBinder2<cmd_class,
///               arg1_type,
///               arg2_type>::reg(interp, arg1, arg2, cmd_name);
/// @endverbatim
/// の様に用いる．
//////////////////////////////////////////////////////////////////////
template <typename Cmd,
	  typename T1,
	  typename T2>
class TclCmdBinder2
{
public:

  /// @brief コマンドオブジェクトを登録する．
  /// @param[in] interp TCLインタープリタクラスへのポインタ
  /// @param[in] arg1 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg2 コマンドオブジェクトを生成するときの引数
  /// @param[in] cmd_name コマンド名
  static
  int
  reg(Tcl_Interp* interp,
      T1 arg1,
      T2 arg2,
      const string& cmd_name);
};



//////////////////////////////////////////////////////////////////////
/// @class TclCmdBinder3 TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief コマンドオブジェクトを生成して登録するための
/// テンプレートクラス
///
/// 実はこのクラス自体には意味はなく, ただ単にクラステンプレート
/// の仕組みを利用して関数を1つ定義するためのギミック
///
/// @verbatim
/// class cmd_cls;      // TclCmd の派生クラス
/// typename arg1_type  // 第1引数の型
/// typename arg2_type  // 第2引数の型
/// typename arg3_type  // 第3引数の型
/// Tcl_Interp* interp; // Tcl インタプリタ
/// arg1_type arg1      // cmd_cls のコンストラクタの第1引数
/// arg1_type arg2      // cmd_cls のコンストラクタの第2引数
/// arg1_type arg3      // cmd_cls のコンストラクタの第3引数
/// string cmd_name;    // コマンド名
/// TclCmdBinder3<cmd_class,
///               arg1_type,
///               arg2_type,
///               arg3_type>::reg(interp, arg1, arg2, arg3, cmd_name);
/// @endverbatim
/// の様に用いる．
//////////////////////////////////////////////////////////////////////
template <typename Cmd,
	  typename T1,
	  typename T2,
	  typename T3>
class TclCmdBinder3
{
public:

  /// @brief コマンドオブジェクトを登録する．
  /// @param[in] interp TCLインタープリタクラスへのポインタ
  /// @param[in] arg1 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg2 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg3 コマンドオブジェクトを生成するときの引数
  /// @param[in] cmd_name コマンド名
  static
  int
  reg(Tcl_Interp* interp,
      T1 arg1,
      T2 arg2,
      T3 arg3,
      const string& cmd_name);
};



//////////////////////////////////////////////////////////////////////
/// @class TclCmdBinder4 TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief コマンドオブジェクトを生成して登録するための
/// テンプレートクラス
///
/// 実はこのクラス自体には意味はなく, ただ単にクラステンプレート
/// の仕組みを利用して関数を1つ定義するためのギミック
///
/// @verbatim
/// class cmd_cls;      // TclCmd の派生クラス
/// typename arg1_type  // 第1引数の型
/// typename arg2_type  // 第2引数の型
/// typename arg3_type  // 第3引数の型
/// typename arg4_type  // 第4引数の型
/// Tcl_Interp* interp; // Tcl インタプリタ
/// arg1_type arg1      // cmd_cls のコンストラクタの第1引数
/// arg1_type arg2      // cmd_cls のコンストラクタの第2引数
/// arg1_type arg3      // cmd_cls のコンストラクタの第3引数
/// arg1_type arg4      // cmd_cls のコンストラクタの第4引数
/// string cmd_name;    // コマンド名
/// TclCmdBinder4<cmd_class,
///               arg1_type,
///               arg2_type,
///               arg3_type,
///               arg4_type>::reg(interp, arg1, arg2, arg3,
///                               arg4, cmd_name);
/// @endverbatim
/// の様に用いる．
//////////////////////////////////////////////////////////////////////
template <typename Cmd,
	  typename T1,
	  typename T2,
	  typename T3,
	  typename T4>
class TclCmdBinder4
{
public:

  /// @brief コマンドオブジェクトを登録する．
  /// @param[in] interp TCLインタープリタクラスへのポインタ
  /// @param[in] arg1 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg2 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg3 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg4 コマンドオブジェクトを生成するときの引数
  /// @param[in] cmd_name コマンド名
  static
  int
  reg(Tcl_Interp* interp,
      T1 arg1,
      T2 arg2,
      T3 arg3,
      T4 arg4,
      const string& cmd_name);
};



//////////////////////////////////////////////////////////////////////
/// @class TclCmdBinder5 TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief コマンドオブジェクトを生成して登録するための
/// テンプレートクラス
///
/// 実はこのクラス自体には意味はなく, ただ単にクラステンプレート
/// の仕組みを利用して関数を1つ定義するためのギミック
///
/// @verbatim
/// class cmd_cls;      // TclCmd の派生クラス
/// typename arg1_type  // 第1引数の型
/// typename arg2_type  // 第2引数の型
/// typename arg3_type  // 第3引数の型
/// typename arg4_type  // 第4引数の型
/// typename arg5_type  // 第5引数の型
/// Tcl_Interp* interp; // Tcl インタプリタ
/// arg1_type arg1      // cmd_cls のコンストラクタの第1引数
/// arg1_type arg2      // cmd_cls のコンストラクタの第2引数
/// arg1_type arg3      // cmd_cls のコンストラクタの第3引数
/// arg1_type arg4      // cmd_cls のコンストラクタの第4引数
/// arg1_type arg5      // cmd_cls のコンストラクタの第5引数
/// string cmd_name;    // コマンド名
/// TclCmdBinder5<cmd_class,
///               arg1_type,
///               arg2_type,
///               arg3_type,
///               arg4_type,
///               arg5_type>::reg(interp, arg1, arg2, arg3,
///                               arg4, arg5, cmd_name);
/// @endverbatim
/// の様に用いる．
//////////////////////////////////////////////////////////////////////
template <typename Cmd,
	  typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5>
class TclCmdBinder5
{
public:

  /// @brief コマンドオブジェクトを登録する．
  /// @param[in] interp TCLインタープリタクラスへのポインタ
  /// @param[in] arg1 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg2 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg3 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg4 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg5 コマンドオブジェクトを生成するときの引数
  /// @param[in] cmd_name コマンド名
  static
  int
  reg(Tcl_Interp* interp,
      T1 arg1,
      T2 arg2,
      T3 arg3,
      T4 arg4,
      T5 arg5,
      const string& cmd_name);
};



//////////////////////////////////////////////////////////////////////
/// @class TclCmdBinder6 TclCmd.h <ym_tclpp/TclCmd.h>
/// @ingroup Tclpp
/// @brief コマンドオブジェクトを生成して登録するための
/// テンプレートクラス
///
/// 実はこのクラス自体には意味はなく, ただ単にクラステンプレート
/// の仕組みを利用して関数を1つ定義するためのギミック
///
/// @verbatim
/// class cmd_cls;      // TclCmd の派生クラス
/// typename arg1_type  // 第1引数の型
/// typename arg2_type  // 第2引数の型
/// typename arg3_type  // 第3引数の型
/// typename arg4_type  // 第4引数の型
/// typename arg5_type  // 第5引数の型
/// typename arg6_type  // 第6引数の型
/// Tcl_Interp* interp; // Tcl インタプリタ
/// arg1_type arg1      // cmd_cls のコンストラクタの第1引数
/// arg1_type arg2      // cmd_cls のコンストラクタの第2引数
/// arg1_type arg3      // cmd_cls のコンストラクタの第3引数
/// arg1_type arg4      // cmd_cls のコンストラクタの第4引数
/// arg1_type arg5      // cmd_cls のコンストラクタの第5引数
/// arg1_type arg6      // cmd_cls のコンストラクタの第6引数
/// string cmd_name;    // コマンド名
/// TclCmdBinder6<cmd_class,
///               arg1_type,
///               arg2_type,
///               arg3_type,
///               arg4_type,
///               arg5_type,
///               arg6_type>::reg(interp, arg1, arg2, arg3,
///                               arg4, arg5, arg6, cmd_name);
/// @endverbatim
/// の様に用いる．
//////////////////////////////////////////////////////////////////////
template <typename Cmd,
	  typename T1,
	  typename T2,
	  typename T3,
	  typename T4,
	  typename T5,
	  typename T6>
class TclCmdBinder6
{
public:

  /// @brief コマンドオブジェクトを登録する．
  /// @param[in] interp TCLインタープリタクラスへのポインタ
  /// @param[in] arg1 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg2 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg3 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg4 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg5 コマンドオブジェクトを生成するときの引数
  /// @param[in] arg6 コマンドオブジェクトを生成するときの引数
  /// @param[in] cmd_name コマンド名
  static
  int
  reg(Tcl_Interp* interp,
      T1 arg1,
      T2 arg2,
      T3 arg3,
      T4 arg4,
      T5 arg5,
      T6 arg6,
      const string& cmd_name);
};


//////////////////////////////////////////////////////////////////////
// TclCmd のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// コマンド名を得る．
inline
string
TclCmd::command_name() const
{
  return mCmdName;
}

// コマンド情報を取得する．
inline
int
TclCmd::command_info(Tcl_CmdInfo* info) const
{
  return TclBase::command_info(command_name(), info);
}

// コマンドトークン(Tcl_CreateCommand()の返り値)を得る．
inline
Tcl_Command
TclCmd::cmd_token() const
{
  return mCmdToken;
}

// 最初の引数 (argv[0]) を得る．
inline
const string&
TclCmd::arg0() const
{
  return mArg0;
}

// @brief help/usage で用いる説明文を設定する関数
inline
void
TclCmd::set_usage_string(const string& str)
{
  mUsageString = str;
}

// @brief help/usage で用いる説明文を返す関数
inline
string
TclCmd::usage_string()
{
  return mUsageString;
}


//////////////////////////////////////////////////////////////////////
// TclCmdBinder のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コマンドオブジェクトを登録する．
// @param[in] interp TCLインタープリタクラスへのポインタ
// @param[in] cmd_name コマンド名
template<typename Cmd>
inline
int
TclCmdBinder<Cmd>::reg(Tcl_Interp* interp,
		       const string& cmd_name)
{
  Cmd* cmd = new Cmd();
  if ( cmd == NULL ) {
    return TCL_ERROR;
  }
  else {
    return cmd->bind(interp, cmd_name);
  }
}


//////////////////////////////////////////////////////////////////////
// TclCmdBinder1 のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コマンドオブジェクトを登録する．
// @param[in] interp TCLインタープリタクラスへのポインタ
// @param[in] arg  コマンドオブジェクトを生成するときの引数
// @param[in] cmd_name コマンド名
template<typename Cmd,
	 typename T1>
inline
int
TclCmdBinder1<Cmd, T1>::reg(Tcl_Interp* interp,
			    T1 arg1,
			    const string& cmd_name)
{
  Cmd* cmd = new Cmd(arg1);
  if ( cmd == NULL ) {
    return TCL_ERROR;
  }
  else {
    return cmd->bind(interp, cmd_name);
  }
}


//////////////////////////////////////////////////////////////////////
// TclCmdBinder2 のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コマンドオブジェクトを登録する．
// @param[in] interp TCLインタープリタクラスへのポインタ
// @param[in] arg1 コマンドオブジェクトを生成するときの引数
// @param[in] arg2 コマンドオブジェクトを生成するときの引数
// @param[in] cmd_name コマンド名
template<typename Cmd,
	 typename T1,
	 typename T2>
inline
int
TclCmdBinder2<Cmd, T1, T2>::reg(Tcl_Interp* interp,
				T1 arg1,
				T2 arg2,
				const string& cmd_name)
{
  Cmd* cmd = new Cmd(arg1, arg2);
  if ( cmd == NULL ) {
    return TCL_ERROR;
  }
  else {
    return cmd->bind(interp, cmd_name);
  }
}


//////////////////////////////////////////////////////////////////////
// TclCmdBinder3 のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コマンドオブジェクトを登録する．
// @param[in] interp TCLインタープリタクラスへのポインタ
// @param[in] arg1 コマンドオブジェクトを生成するときの引数
// @param[in] arg2 コマンドオブジェクトを生成するときの引数
// @param[in] arg3 コマンドオブジェクトを生成するときの引数
// @param[in] cmd_name コマンド名
template<typename Cmd,
	 typename T1,
	 typename T2,
	 typename T3>
inline
int
TclCmdBinder3<Cmd, T1, T2, T3>::reg(Tcl_Interp* interp,
				    T1 arg1,
				    T2 arg2,
				    T3 arg3,
				    const string& cmd_name)
{
  Cmd* cmd = new Cmd(arg1, arg2, arg3);
  if ( cmd == NULL ) {
    return TCL_ERROR;
  }
  else {
    return cmd->bind(interp, cmd_name);
  }
}


//////////////////////////////////////////////////////////////////////
// TclCmdBinder4 のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コマンドオブジェクトを登録する．
// @param[in] interp TCLインタープリタクラスへのポインタ
// @param[in] arg1 コマンドオブジェクトを生成するときの引数
// @param[in] arg2 コマンドオブジェクトを生成するときの引数
// @param[in] arg3 コマンドオブジェクトを生成するときの引数
// @param[in] arg4 コマンドオブジェクトを生成するときの引数
// @param[in] cmd_name コマンド名
template<typename Cmd,
	 typename T1,
	 typename T2,
	 typename T3,
	 typename T4>
inline
int
TclCmdBinder4<Cmd, T1, T2, T3, T4>::reg(Tcl_Interp* interp,
					T1 arg1,
					T2 arg2,
					T3 arg3,
					T4 arg4,
					const string& cmd_name)
{
  Cmd* cmd = new Cmd(arg1, arg2, arg3, arg4);
  if ( cmd == NULL ) {
    return TCL_ERROR;
  }
  else {
    return cmd->bind(interp, cmd_name);
  }
}


//////////////////////////////////////////////////////////////////////
// TclCmdBinder5 のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コマンドオブジェクトを登録する．
// @param[in] interp TCLインタープリタクラスへのポインタ
// @param[in] arg1 コマンドオブジェクトを生成するときの引数
// @param[in] arg2 コマンドオブジェクトを生成するときの引数
// @param[in] arg3 コマンドオブジェクトを生成するときの引数
// @param[in] arg4 コマンドオブジェクトを生成するときの引数
// @param[in] arg5 コマンドオブジェクトを生成するときの引数
// @param[in] cmd_name コマンド名
template<typename Cmd,
	 typename T1,
	 typename T2,
	 typename T3,
	 typename T4,
	 typename T5>
inline
int
TclCmdBinder5<Cmd, T1, T2, T3, T4, T5>::reg(Tcl_Interp* interp,
					    T1 arg1,
					    T2 arg2,
					    T3 arg3,
					    T4 arg4,
					    T5 arg5,
					    const string& cmd_name)
{
  Cmd* cmd = new Cmd(arg1, arg2, arg3, arg4, arg5);
  if ( cmd == NULL ) {
    return TCL_ERROR;
  }
  else {
    return cmd->bind(interp, cmd_name);
  }
}


//////////////////////////////////////////////////////////////////////
// TclCmdBinder6 のインライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コマンドオブジェクトを登録する．
// @param[in] interp TCLインタープリタクラスへのポインタ
// @param[in] arg1 コマンドオブジェクトを生成するときの引数
// @param[in] arg2 コマンドオブジェクトを生成するときの引数
// @param[in] arg3 コマンドオブジェクトを生成するときの引数
// @param[in] arg4 コマンドオブジェクトを生成するときの引数
// @param[in] arg5 コマンドオブジェクトを生成するときの引数
// @param[in] arg6 コマンドオブジェクトを生成するときの引数
// @param[in] cmd_name コマンド名
template<typename Cmd,
	 typename T1,
	 typename T2,
	 typename T3,
	 typename T4,
	 typename T5,
	 typename T6>
inline
int
TclCmdBinder6<Cmd, T1, T2, T3, T4, T5, T6>::reg(Tcl_Interp* interp,
						T1 arg1,
						T2 arg2,
						T3 arg3,
						T4 arg4,
						T5 arg5,
						T6 arg6,
						const string& cmd_name)
{
  Cmd* cmd = new Cmd(arg1, arg2, arg3, arg4, arg5, arg6);
  if ( cmd == NULL ) {
    return TCL_ERROR;
  }
  else {
    return cmd->bind(interp, cmd_name);
  }
}

END_NAMESPACE_YM_TCLPP

#endif // YM_TCLPP_TCLCMD_H
