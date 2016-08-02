#ifndef YM_TCLPP_TCLPOPT_H
#define YM_TCLPP_TCLPOPT_H

/// @file ym_tclpp/TclPopt.h
/// @brief TclPopt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclPopt.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"
#include "ym_tclpp/TclBase.h"
#include "ym_tclpp/TclObj.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @brief TclPopt のパーズ結果を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tTclPoptStat {
  /// @brief 正常終了
  kTclPoptOk,
  /// @brief 正常終了，以後の処理は行わない
  kTclPoptBreak,
  /// @brief エラー
  kTclPoptError
};


//////////////////////////////////////////////////////////////////////
/// @class TclPopt TclPopt.h <ym_tclpp/TclPopt.h>
/// @ingroup Tclpp
/// @brief Tcl コマンドのオプション解析時のアクション定義用基底クラス
//////////////////////////////////////////////////////////////////////
class TclPopt :
  public TclBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPopt(TclCmd* cmd,
	  const string& opt_str,
	  const string& opt_desc,
	  const string& arg_desc = string(),
	  bool allow_override = false);
  
  /// @brief デストラクタ
  virtual
  ~TclPopt();


public:

  /// @brief 関連付けられているコマンドを返す．
  TclCmd*
  cmd() const;

  /// @brief オプション文字列を返す．
  string
  opt_str() const;

  /// @brief オプションの簡単な記述を返す．
  string
  opt_desc();

  /// @brief オプション引数の簡単な記述を返す．
  string
  arg_desc();

  /// @brief このオブジェクトが解析中に呼ばれていたら true を返す．
  bool
  is_specified() const;
  
  /// @brief このオブジェクトが解析中に呼ばれた回数を返す．
  int
  count() const;


public:
  
  /// @brief 解析を始める前に呼ばれる初期化
  /// @note mCount を 0 に初期化してから init() を呼び出している．
  void
  _init();
  
  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  tTclPoptStat
  _action(const string& opt_str,
	  TclObjVector::iterator& rpos,
	  const TclObjVector::iterator& end);


private:
  //////////////////////////////////////////////////////////////////////
  // 以下の関数は継承クラスで実装する必要がある．
  //////////////////////////////////////////////////////////////////////

  /// @brief 解析を始める前に呼ばれる初期化
  /// @note デフォルトの実装では何もしない．
  virtual
  void
  init();
  
  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);


protected:

  /// @brief 次の引数を取ってくる．
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @param[out] obj 読み出した引数を格納する変数
  /// @retval true 成功
  /// @retval false 失敗．たぶん，次の引数がなかった．
  bool
  get_next_arg(const string& opt_str,
	       TclObjVector::iterator& rpos,
	       const TclObjVector::iterator& end,
	       TclObj& obj);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関連付けられているコマンド
  TclCmd* mCmd;

  // オプション文字列
  string mOptStr;

  // オプションの簡単な説明文
  string mOptDesc;

  // オプション引数の簡単な説明文
  string mArgDesc;

  // 2重定義を許すとき true にするフラグ
  bool mAllowOverride;

  // このオプションが現れた回数
  int mCount;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptObj TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <任意の引数>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptObj :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptObj(TclCmd* cmd,
	     const string& opt_str,
	     const string& opt_desc,
	     const string& arg_desc = string(),
	     bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptObj();


public:
  
  /// @brief オプション引数の値を返す．
  TclObj
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_obj() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);
  

private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_obj(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // パーズ結果を格納する変数
  TclObj mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptStr TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <引数文字列>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptStr :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptStr(TclCmd* cmd,
	     const string& opt_str,
	     const string& opt_desc,
	     const string& arg_desc = "STRING",
	     bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptStr();


public:

  /// @brief オプション引数の値を返す．
  string
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_str() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_str(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // パーズ結果を格納する変数
  string mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptBool TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <論理値>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptBool :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptBool(TclCmd* cmd,
	      const string& opt_str,
	      const string& opt_desc,
	      const string& arg_desc = "BOOL",
	      bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptBool();


public:
  
  /// @brief オプション引数の値を返す．
  bool
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_bool() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);
  

private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_bool(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // パーズ結果を格納する変数
  bool mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptInt TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <整数>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptInt :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptInt(TclCmd* cmd,
	     const string& opt_str,
	     const string& opt_desc,
	     const string& arg_desc = "INT",
	     bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptInt();


public:

  /// @brief オプション引数の値を返す．
  ymint
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_int() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_int(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーズ結果を格納する変数
  ymint mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptUint TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <符号なし整数>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptUint :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptUint(TclCmd* cmd,
	      const string& opt_str,
	      const string& opt_desc,
	      const string& arg_desc = "UINT",
	      bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptUint();


public:

  /// @brief オプション引数の値を返す．
  ymuint
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_uint() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_uint(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーズ結果を格納する変数
  ymuint mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptLong TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <long整数>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptLong :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptLong(TclCmd* cmd,
	      const string& opt_str,
	      const string& opt_desc,
	      const string& arg_desc = "LONG",
	      bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptLong();


public:

  /// @brief オプション引数の値を返す．
  ymlong
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_long() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_long(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーズ結果を格納する変数
  ymlong mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptUlong TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <ulong整数>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptUlong :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptUlong(TclCmd* cmd,
	       const string& opt_str,
	       const string& opt_desc,
	       const string& arg_desc = "ULONG",
	       bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptUlong();


public:

  /// @brief オプション引数の値を返す．
  ymulong
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_ulong() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_ulong(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーズ結果を格納する変数
  ymulong mVal;
  
};


//////////////////////////////////////////////////////////////////////
/// @class TclPoptDouble TclPopt.h <ym_tclpp/TclPopt.h>
/// @brief <-オプション文字列> <double>の形の解析を行うオブジェクト
//////////////////////////////////////////////////////////////////////
class TclPoptDouble :
  public TclPopt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cmd 関連付けられるコマンド
  /// @param[in] opt_str 文字列オプション
  /// @param[in] opt_desc オプションの説明文
  /// @param[in] arg_desc オプション引数の説明文
  /// @param[in] allow_override 二重定義を許すとき true にする．
  TclPoptDouble(TclCmd* cmd,
		const string& opt_str,
		const string& opt_desc,
		const string& arg_desc = "DOUBLE",
		bool allow_override = false);

  /// @brief デストラクタ
  virtual
  ~TclPoptDouble();


public:

  /// @brief オプション引数の値を返す．
  double
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // TclPopt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列がマッチしたときの処理
  /// @param[in] opt_str オプション文字列
  /// @param[inout] rpos コマンド行引数の次の位置
  /// @param[in] end コマンド行引数の終端位置
  /// @return 処理結果
  /// @note mVal に値をセットして action_double() を呼び出す．
  virtual
  tTclPoptStat
  action(const string& opt_str,
	 TclObjVector::iterator& rpos,
	 const TclObjVector::iterator& end);


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスの継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチしたときのアクション
  /// @param[in] opt_str オプション文字列
  /// @param[in] arg_val オプションの引数
  /// @return 処理結果
  /// @note デフォルトの実装ではなにもしないで kTclPoptOk を返す．
  virtual
  tTclPoptStat
  action_double(const string& opt_str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーズ結果を格納する変数
  double mVal;
  
};

END_NAMESPACE_YM_TCLPP

#endif // YM_TCLPP_TCLPOPT_H
