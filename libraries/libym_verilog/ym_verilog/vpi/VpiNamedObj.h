#ifndef YM_VERILOG_VPI_VPINAMEDOBJ_H
#define YM_VERILOG_VPI_VPINAMEDOBJ_H

/// @file ym_verilog/vpi/VpiNamedObj.h
/// @brief VpiNamedObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiNamedObj.h 1052 2007-10-24 11:08:51Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>


BEGIN_NAMESPACE_YM_VERILOG

class VpiFactory;
class VpiScope;
class VpiModule;
class VpiNamedEvent;
class VpiExpr;
class VpiPorts;
class VpiPrimTerm;


//////////////////////////////////////////////////////////////////////
/// @class VpiNamedObj VpiNamedObj.h <ym_verilog/vpi/VpiNamedObj.h>
/// @ingroup VlElb
/// @brief スコープ要素で名前を持つクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class VpiNamedObj :
  public VpiBase
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiBase Parent;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiNamedObj に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  /// @note ここでは name_trunc() + name_suffix() を返す．
  virtual
  string
  name() const;

  /// @brief インデックス付きの名前の時の本体の名前を返す．
  virtual
  string
  name_trunk() const = 0;

  /// @brief インデックス付きの名前の時のインデックス部分の名前を返す．
  virtual
  string
  name_suffix() const = 0;

  /// @brief 名前にエスケープが必要なら付加したものを返す．
  /// @note 実際には name_trunk() 部分にエスケープが必要かどうか調べる．
  virtual
  string
  esc_name() const;

  /// @brief 階層つき名前を返す．
  /// @note 仕様書には書いていないがこれはエスケープされた文字列でないと
  /// @note 階層の区切りがわからなくなるのでエスケープしている．
  virtual
  string
  full_name() const;

  /// @brief 属するモジュールを返す．
  /// @note このクラスの実装では scope() を返す．
  virtual
  VpiModule*
  module() const;

  /// @brief 属するスコープを返す．
  virtual
  VpiScope*
  scope() const = 0;

  /// @brief IO と接続する primary 式に変換する．
  /// @param[in] factory VpiFactory
  /// @param[in] file_region もとの記述のファイル位置
  /// @param[in, out] left, right 範囲を表す式
  /// @param[in, out] sign 符号の有無の情報
  /// @param[out] stat チェック結果
  ///   - 0 問題なし
  ///   - 1 net/reg/integer/time 以外の要素だった
  ///   - 2 矛盾した符号/範囲定義を持つ．
  /// @return 生成された primary 式を返す．
  /// @note 場合によっては left, right, sign に情報をコピーする．
  /// @note このクラスのデフォルト実装は stat に 1 を入れ, NULL を返す．
  virtual
  VpiExpr*
  cast_to_ioprimary(VpiFactory* factory,
		    const FileRegion& file_region,
		    VpiExpr*& left,
		    VpiExpr*& right,
		    bool& sign,
		    int& stat);

  /// @brief オブジェクトを primary 式に変換する．
  /// @param[in] factory VpiFactory
  /// @param[in] file_region もとの記述のファイル位置
  /// @return primary 式を表す VpiExpr へのポインタを返す．
  /// @return net/reg/variable 以外のときは NULL を返す．
  /// @note このクラスのデフォルト実装は NULL を返す．
  virtual
  VpiExpr*
  cast_to_primary(VpiFactory* factory,
		  const FileRegion& file_region);
  
  /// @brief expression 中に現れた primary にインデックスを付加したものを返す．
  ///
  /// 範囲指定もビット指定もないときは index1 = index2 = NULL
  /// ビット指定があるときには index1 != NULL, index2 = NULL
  /// 範囲指定があるときには index1 != NULL, index2 != NULL
  /// error_on_index_for_scalar == true の時には
  /// スカラータイプの要素に対するビット指定/範囲指定をエラーとする．
  /// error_on_index_for_scalar == false の時には警告とする．
  /// argument == true の時には配列などシステムタスク／関数の
  /// の引数になりうるものも認める．
  /// event == true の時には named event も認める．
  /// エラーが起きたら NULL を返す．
  ///
  /// このクラスのデフォルト実装は NULL を返す．
  virtual
  VpiExpr*
  cast_to_expr(VpiFactory* factory,
	       const FileRegion& file_region,
	       const vector<VpiExpr*>& index_list,
	       VpiExpr* index1,
	       VpiExpr* index2,
	       tVpiRangeMode mode,
	       bool error_on_index_for_scalar,
	       bool argument,
	       bool event);

  /// @brief primary を NamedEvent に変換する．
  /// @param[in] factory VpiFactory
  /// @param[in] file_region もとの記述のファイル位置
  /// @param[in] index_list 添字のリスト
  /// @return 変換された NamedEvent へのポインタを返す．
  /// @note このクラスのデフォルト実装は NULL を返す．
  virtual
  VpiNamedEvent*
  cast_to_namedevent(VpiFactory* factory,
		     const FileRegion& file_region,
		     const vector<VpiExpr*>& index_list);

  /// @brief 定数値を表すオブジェクトの時 true を返す．
  /// @note このクラスのデフォルト実装では false を返す．
  virtual
  bool
  is_const() const;
  
  /// @brief 値を設定する．
  /// @param[in] val 設定するビットベクタ値
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  void
  set_value(const BitVector& val);
  
  /// @brief ビット指定付きの値を設定する
  /// @param[in] index 設定するビット位置
  /// @param[in] val 設定するスカラ値
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  void
  set_bit_select_value(int index,
		       tVpiScalarVal val);

  /// @brief 範囲指定付きの値を設定する
  /// @param[in] index1 設定する範囲の MSB
  /// @param[in] index2 設定する範囲の LSB
  /// @param[in] val 設定するビットベクタ値
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  void
  set_part_select_value(int index1,
			int index2,
			const BitVector& val);

  /// @brief 値を設定する．
  /// @param[in] val 設定する実数値
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  void
  set_value(double val);

  /// @brief 値を返す．
  /// @param[out] val 値を格納するビットベクタ
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  void
  bitvector_value(BitVector& val) const;

  /// @brief ビット指定された位置の値を返す．
  /// @param[in] index 読み出すビット位置
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  tVpiScalarVal
  bit_select_value(int index) const;

  /// @brief 範囲に対応した部分の値を返す．
  /// @param[in] index1 読み出す範囲の MSB
  /// @param[in] index2 読み出す範囲の LSB
  /// @param[out] val 読み出す値を格納するビットベクタ
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  void
  part_select_value(int index1,
		    int index2,
		    BitVector& val) const;

  /// @brief 実数値を読み出す．
  /// @note このクラスのデフォルト実装はエラーとなる．
  virtual
  double
  real_value() const;

  /// @brief port expression として用いられたときに実際の
  /// @brief net/reg の mPortsListに該当の port を追加する．
  /// @note このクラスのデフォルト実装はなにもしない．
  virtual
  void
  add_port(VpiPorts* port);

  /// @brief port expression として用いられたときに実際の
  /// @brief net/reg の mPortInstList に該当の port を追加する．
  /// @note このクラスのデフォルト実装はなにもしない．
  virtual
  void
  add_portinst(VpiPorts* port);

  /// @brief prim term 用の expression として用いられたときに
  /// @brief net/reg の mPrimTermList に該当の prim_term を追加する．
  /// @note このクラスのデフォルト実装はなにもしない．
  virtual
  void
  add_primterm(VpiPrimTerm* prim_term);

  /// @brief その expression を用いているオブジェクトを登録する．
  /// @note このクラスのデフォルト実装はなにもしない．
  virtual
  void
  add_use(VpiObj* obj);
  
  /// @brief この式が net_lvalue かどうか調べる．
  /// @note 具体的には net 系の primary 自身もしくはその定数範囲指定
  /// @note および定数ビット指定のみからなる式の時のみ true を返す．
  /// @note 上記の concatenation もOK
  /// @note このクラスのデフォルト実装は false を返す．
  virtual
  bool
  check_net_lvalue() const;
  
  /// @brief この式が variable_lvalue かどうか調べる．
  /// @note 具体的には reg/variable 系の primary 自身もしくはその範囲指定
  /// @note およびビット指定のみからなるときのみ true を返す．
  /// @note 上記の concatenation も OK
  /// @note このクラスのデフォルト実装は false を返す．
  virtual
  bool
  check_var_lvalue() const;

  /// @brief この式が procedural continuous assignment の左辺式かどうか調べる．
  /// @note 具体的には variable そのものか variable の concatenation のみ
  /// @note true を返す．
  /// @note このクラスのデフォルト実装は false を返す．
  virtual
  bool
  check_assign_lvalue() const;

  /// @brief この式が force 文の左辺式かどうか調べる．
  /// @note 具体的には variable/net/bit-select of net/part-select of net
  /// @note もしくは上記の concatenation のみ true を返す．
  /// @note このクラスのデフォルト実装は false を返す．
  virtual
  bool
  check_force_lvalue() const;

  /// @brief この式が parameter の初期値かどうか調べる．
  /// @note 具体的には parameter(localparam)/constant/constant function call
  /// @note のみを終端とする式が OK
  /// @note このクラスのデフォルト実装は false を返す．
  virtual
  bool
  check_param_rvalue() const;

#if 0
  /// @brief 自分を使っている terminal/ports などのリストの反復子を返す．
  virtual
  VpiUseIteratorImpl*
  use_iterator_obj();
#endif
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // VPI 関数をオブジェクト指向にしたもの
  //////////////////////////////////////////////////////////////////////

  /// @brief property に対応したブール型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応したブール型の値を返す．
  virtual
  bool
  get_bool(int property) const;

  /// @brief property に対応した整数型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応した整数型の値を返す．
  virtual
  int
  get_int(int property) const;
  
  /// @brief property に対応した文字列型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応した文字列型の値を返す．
  virtual
  string
  get_str(int property) const;
  
  /// @brief property に対応したオブジェクトを返す．
  /// @param[in] property 取り出す項目を表す値
  /// @brief return に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj(int property) const;
  
  /// @brief property に対応した反復子を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_iterator_obj(int property);


public:

  /// @brief str にエスケープが必要なときエスケープする
  static
  string
  escaped_string(const string& str);

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPINAMEDOBJ_H
