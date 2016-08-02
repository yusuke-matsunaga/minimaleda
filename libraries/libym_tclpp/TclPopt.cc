
/// @file libym_tclpp/TclPopt.cc
/// @brief TclPopt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TclPopt.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclPopt.h"
#include "ym_tclpp/TclCmd.h"
#include "TclPoptGroup.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// クラス TclPopt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPopt::TclPopt(TclCmd* cmd,
		 const string& opt_str,
		 const string& opt_desc,
		 const string& arg_desc,
		 bool allow_override) :
  mCmd(cmd),
  mOptStr(opt_str),
  mOptDesc(opt_desc),
  mArgDesc(arg_desc),
  mAllowOverride(allow_override),
  mCount(0)
{
  cmd->bind_popt(this);
}

// @brief デストラクタ
TclPopt::~TclPopt()
{
}

// @brief 関連付けられているコマンドを返す．
TclCmd*
TclPopt::cmd() const
{
  return mCmd;
}

// @brief オプション文字列を返す．
string
TclPopt::opt_str() const
{
  return mOptStr;
}

// @brief オプションの簡単な記述を返す．
string
TclPopt::opt_desc()
{
  return mOptDesc;
}

// @brief オプション引数の簡単な記述を返す．
string
TclPopt::arg_desc()
{
  return mArgDesc;
}

// @brief このオブジェクトが解析中に呼ばれていたら true を返す．
bool
TclPopt::is_specified() const
{
  return count() > 0;
}

// @brief このオブジェクトが解析中に呼ばれた回数を返す．
int
TclPopt::count() const
{
  return mCount;
}

// @brief 解析を始める前に呼ばれる初期化
// @note mCount を 0 に初期化してから init() を呼び出している．
void
TclPopt::_init()
{
  mCount = 0;
  init();
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPopt::_action(const string& opt_str,
		 TclObjVector::iterator& rpos,
		 const TclObjVector::iterator& end)
{
  if ( !mAllowOverride && mCount > 0 ) {
    string emsg = opt_str + ": specified more than once";
    set_result(emsg);
    return kTclPoptError;
  }
  ++ mCount;
  return action(opt_str, rpos, end);
}

// @brief 解析を始める前に呼ばれる初期化
// @note デフォルトの実装では何もしない．
void
TclPopt::init()
{
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPopt::action(const string& opt_str,
		TclObjVector::iterator& rpos,
		const TclObjVector::iterator& end)
{
  return kTclPoptOk;
}

// @brief 次の引数を取ってくる．
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @param[out] obj 読み出した引数を格納する変数
// @retval true 成功
// @retval false 失敗．たぶん，次の引数がなかった．
bool
TclPopt::get_next_arg(const string& opt_str,
		      TclObjVector::iterator& rpos,
		      const TclObjVector::iterator& end,
		      TclObj& obj)
{
  if ( rpos == end ) {
    string emsg = opt_str + ": no argument";
    set_result(emsg);
    return false;
  }
  obj = *rpos;
  ++ rpos;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptObjBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptObj::TclPoptObj(TclCmd* cmd,
		       const string& opt_str,
		       const string& opt_desc,
		       const string& arg_desc,
		       bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptObj::~TclPoptObj()
{
}

// @brief オプション引数の値を返す．
TclObj
TclPoptObj::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptObj::action(const string& opt_str,
		   TclObjVector::iterator& rpos,
		   const TclObjVector::iterator& end)

{
  if ( !get_next_arg(opt_str, rpos, end, mVal) ) {
    return kTclPoptError;
  }
  return action_obj(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptObj::action_obj(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptStr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptStr::TclPoptStr(TclCmd* cmd,
		       const string& opt_str,
		       const string& opt_desc,
		       const string& arg_desc,
		       bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptStr::~TclPoptStr()
{
}

// @brief オプション引数の値を返す．
string
TclPoptStr::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptStr::action(const string& opt_str,
		   TclObjVector::iterator& rpos,
		   const TclObjVector::iterator& end)
{
  TclObj arg_obj;
  if ( !get_next_arg(opt_str, rpos, end, arg_obj) ) {
    return kTclPoptError;
  }
  // この変換は失敗しないはず
  mVal = arg_obj;
  return action_str(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptStr::action_str(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptBool
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptBool::TclPoptBool(TclCmd* cmd,
			 const string& opt_str,
			 const string& opt_desc,
			 const string& arg_desc,
			 bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptBool::~TclPoptBool()
{
}

// @brief オプション引数の値を返す．
bool
TclPoptBool::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptBool::action(const string& opt_str,
		    TclObjVector::iterator& rpos,
		    const TclObjVector::iterator& end)
{
  TclObj arg_obj;
  if ( !get_next_arg(opt_str, rpos, end, arg_obj) ) {
    return kTclPoptError;
  }
  if ( bool_conv(arg_obj, mVal) == TCL_ERROR ) {
    string emsg = opt_str + ": Needs an boolean value";
    set_result(emsg);
    return kTclPoptError;
  }
  return action_bool(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptBool::action_bool(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptInt::TclPoptInt(TclCmd* cmd,
		       const string& opt_str,
		       const string& opt_desc,
		       const string& arg_desc,
		       bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptInt::~TclPoptInt()
{
}

// @brief オプション引数の値を返す．
ymint
TclPoptInt::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptInt::action(const string& opt_str,
		   TclObjVector::iterator& rpos,
		   const TclObjVector::iterator& end)
{
  TclObj arg_obj;
  if ( !get_next_arg(opt_str, rpos, end, arg_obj) ) {
    return kTclPoptError;
  }
  if ( int_conv(arg_obj, mVal) == TCL_ERROR ) {
    string emsg = opt_str + ": Needs an integer value";
    set_result(emsg);
    return kTclPoptError;
  }
  return action_int(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptInt::action_int(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptUint
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptUint::TclPoptUint(TclCmd* cmd,
			 const string& opt_str,
			 const string& opt_desc,
			 const string& arg_desc,
			 bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptUint::~TclPoptUint()
{
}

// @brief オプション引数の値を返す．
ymuint
TclPoptUint::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptUint::action(const string& opt_str,
		    TclObjVector::iterator& rpos,
		    const TclObjVector::iterator& end)
{
  TclObj arg_obj;
  if ( !get_next_arg(opt_str, rpos, end, arg_obj) ) {
    return kTclPoptError;
  }
  if ( uint_conv(arg_obj, mVal) == TCL_ERROR ) {
    string emsg = opt_str + ": Needs an unsigned integer value";
    set_result(emsg);
    return kTclPoptError;
  }
  return action_uint(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptUint::action_uint(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptLong
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptLong::TclPoptLong(TclCmd* cmd,
			 const string& opt_str,
			 const string& opt_desc,
			 const string& arg_desc,
			 bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptLong::~TclPoptLong()
{
}

// @brief オプション引数の値を返す．
ymlong
TclPoptLong::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptLong::action(const string& opt_str,
		    TclObjVector::iterator& rpos,
		    const TclObjVector::iterator& end)
{
  TclObj arg_obj;
  if ( !get_next_arg(opt_str, rpos, end, arg_obj) ) {
    return kTclPoptError;
  }
  if ( long_conv(arg_obj, mVal) == TCL_ERROR ) {
    string emsg = opt_str + ": Needs a long integer value";
    set_result(emsg);
    return kTclPoptError;
  }
  return action_long(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptLong::action_long(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptUlong
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptUlong::TclPoptUlong(TclCmd* cmd,
			   const string& opt_str,
			   const string& opt_desc,
			   const string& arg_desc,
			   bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptUlong::~TclPoptUlong()
{
}

// @brief オプション引数の値を返す．
ymulong
TclPoptUlong::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptUlong::action(const string& opt_str,
		     TclObjVector::iterator& rpos,
		     const TclObjVector::iterator& end)
{
  TclObj arg_obj;
  if ( !get_next_arg(opt_str, rpos, end, arg_obj) ) {
    return kTclPoptError;
  }
  if ( ulong_conv(arg_obj, mVal) == TCL_ERROR ) {
    string emsg = opt_str + ": Needs a long integer value";
    set_result(emsg);
    return kTclPoptError;
  }
  return action_ulong(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptUlong::action_ulong(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptDouble
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cmd 関連付けられるコマンド
// @param[in] opt_str 文字列オプション
// @param[in] opt_desc オプションの説明文
// @param[in] allow_override 二重定義を許すとき true にする．
TclPoptDouble::TclPoptDouble(TclCmd* cmd,
			     const string& opt_str,
			     const string& opt_desc,
			     const string& arg_desc,
			     bool allow_override) :
  TclPopt(cmd, opt_str, opt_desc, arg_desc, allow_override)
{
}

// @brief デストラクタ
TclPoptDouble::~TclPoptDouble()
{
}

// @brief オプション引数の値を返す．
double
TclPoptDouble::val() const
{
  return mVal;
}

// @brief オプション文字列がマッチしたときの処理
// @param[in] opt_str オプション文字列
// @param[inout] rpos コマンド行引数の次の位置
// @param[in] end コマンド行引数の終端位置
// @return 処理結果
tTclPoptStat
TclPoptDouble::action(const string& opt_str,
		      TclObjVector::iterator& rpos,
		      const TclObjVector::iterator& end)
{
  TclObj arg_obj;
  if ( !get_next_arg(opt_str, rpos, end, arg_obj) ) {
    return kTclPoptError;
  }
  if ( double_conv(arg_obj, mVal) == TCL_ERROR ) {
    string emsg = opt_str + ": Needs a double floating value";
    set_result(emsg);
    return kTclPoptError;
  }
  return action_double(opt_str);
}

// @brief マッチしたときのアクション
// @param[in] opt_str オプション文字列
// @return 処理結果
tTclPoptStat
TclPoptDouble::action_double(const string& opt_str)
{
  return kTclPoptOk;
}


//////////////////////////////////////////////////////////////////////
// クラス TclPoptGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TclPoptGroup::TclPoptGroup()
{
}

// @brief デストラクタ
TclPoptGroup::~TclPoptGroup()
{
}

// @brief 要素を追加する．
void
TclPoptGroup::add(TclPopt* popt)
{
  mPoptList.push_back(popt);
}

// @brief 複数のオプションが指定されていたら true を返す．
bool
TclPoptGroup::check() const
{
  int count = 0;
  for (list<TclPopt*>::const_iterator p = mPoptList.begin();
       p != mPoptList.end(); ++ p) {
    TclPopt* popt = *p;
    if ( popt->is_specified() ) {
      ++ count;
    }
  }
  return ( count > 1 );
}

// @brief 要素のリストを返す．
const list<TclPopt*>&
TclPoptGroup::popt_list() const
{
  return mPoptList;
}

END_NAMESPACE_YM_TCLPP
