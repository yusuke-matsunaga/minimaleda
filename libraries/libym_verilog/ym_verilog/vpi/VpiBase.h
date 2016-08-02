#ifndef YM_VERILOG_VPI_VPIBASE_H
#define YM_VERILOG_VPI_VPIBASE_H

/// @file ym_verilog/vpi/VpiBase.h
/// @brief VpiBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiBase.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiObj.h>


BEGIN_NAMESPACE_YM_VERILOG

// クラス名の先行宣言
class BitVector;
class VlTime;
class VpiAttribute;

//////////////////////////////////////////////////////////////////////
/// @class VpiBase VpiBase.h <ym_verilog/vpi/VpiBase.h>
/// @ingroup VlParser
/// @brief VpiObj の仮想関数のデフォルト実装
//////////////////////////////////////////////////////////////////////
class VpiBase :
  public VpiObj
{
public:
  
  /// @brief デストラクタ
  virtual
  ~VpiBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VpiObj に固有の仮想関数
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

  /// @brief インデックスに対応したオブジェクトの取得
  /// @param[in] index 取得する要素の位置
  /// @return index に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj_by_index(int index) const;
  
  /// @brief 多次元のインデックスに対応したオブジェクトの取得
  /// @param[in] index_array 取得する要素の位置を表すインデックスのベクタ
  /// @return index_array に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj_by_index(const vector<int>& index_array) const;
  
  /// @brief name に対応したオブジェクトの取得
  /// @param[in] name 取得する要素の名前
  /// @return name に対応したオブジェクト
  virtual
  VpiObj*
  get_obj_by_name(const string& name) const;

  /// @brief 次の要素の取得
  /// @return 次の要素を返す．
  /// @note vpiIterater タイプの時のみ意味を持つ．
  virtual
  VpiObj*
  scan();

  /// @brief 空の反復子のチェック
  /// @return 空の反復子の時 true を返す．
  /// @note vpiIterator タイプの時のみ意味を持つ．
  virtual
  bool
  is_empty_iterator() const;
  
  /// @brief シミュレーション時刻の取得
  /// @param[out] time_p 得られたシミュレーション時刻を格納する構造体
  /// @note このオブジェクトのタイムスケールを使って今のシミュレーション
  /// @note 時刻を得る．
  virtual
  void
  get_time(p_vpi_time time_p) const;
  
  /// @brief 遅延情報の取得
  /// @param[out] delay_p 遅延情報を格納する構造体
  virtual
  void
  get_delays(p_vpi_delay delay_p) const;
  
  /// @brief 遅延情報の設定
  /// @param[in] delay_p 設定する遅延情報
  virtual
  void
  put_delays(p_vpi_delay delay_p);
  
  /// @brief オブジェクトの「値」の取得
  /// @param[out] value_p 取得した値を格納する構造体
  virtual
  void
  get_value(p_vpi_value value_p) const;
  
  /// @brief オブジェクトの「値」の設定
  /// @param[in] value_p 設定する値
  /// @param[in] time_p 設定する時刻
  /// @param[in] flags 種々のフラグ
  virtual
  VpiObj*
  put_value(p_vpi_value value_p,
	    p_vpi_time time_p,
	    int flags);
  
  /// @brief ユーザー定義システムタスク/関数情報の取得
  /// @param[out] systf_data_p システムタスク/関数情報
  virtual
  void
  get_systf_info(p_vpi_systf_data systf_data_p) const;
  
  /// @brief ユーザー定義システムタスク/関数に関連づけられたデータの取得
  /// @return put_userdata で設定されたデータ
  virtual
  void*
  get_userdata() const;
  
  /// @brief ユーザー定義システムタスク/関数に関連づけられたデータの設定
  /// @param[in] userdata 設定するデータ
  virtual
  void
  put_userdata(void* userdata) const;
  
  /// @brief コールバック情報の取得
  /// @param[out] cb_data_p コールバック情報を格納する構造体
  virtual
  void
  get_cb_info(p_cb_data cb_data_p) const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // ファイル位置の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル名の取得
  /// @note file_region() から値をとってくる．
  virtual
  string
  filename() const;

  /// @brief 行番号の取得
  /// @note file_region() から値をとってくる．
  virtual
  int
  line_no() const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // attribute_instance の追加
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute_instance のリストをセットする．
  /// @note デフォルトでは何もしない．
  virtual
  void
  set_attribute(const vector<VpiAttribute*>& attr_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 参照回数のアクセス
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす．
  /// @note このクラスのデフォルト実装ではなにもしない．
  virtual
  void
  inc_ref();

  /// @brief 参照回数を減らす
  /// @note 場合によっては自身を削除する．
  /// @note このクラスのデフォルト実装ではなにもしない．
  virtual
  void
  dec_ref();


protected:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列を vpi_value に設定する．
  /// @param[in] str 設定する文字列
  /// @param[out] value_p 文字列の格納先
  static
  void
  str2vpi_value(const string& str,
		p_vpi_value value_p);

  /// @brief スカラー値を vpi_value に設定する．
  /// @param[in] val 設定するスカラー値
  /// @param[out] value_p スカラー値の格納先
  static
  void
  scalar2vpi_value(int val,
		   p_vpi_value value_p);
  
  /// @brief 整数を vpi_value に設定する．
  /// @param[in] val 設定する整数値
  /// @param[out] value_p 整数値の格納先
  static
  void
  int2vpi_value(int val,
		p_vpi_value value_p);

  /// @brief 実数を vpi_value に設定する．
  /// @param[in] val 設定する実数値
  /// @param[out] value_p 実数値の格納先
  static
  void
  real2vpi_value(double val,
		 p_vpi_value value_p);

  /// @brief ビットベクタを vpi_value に設定する．
  /// @param[in] bv 設定するビットベクタ
  /// @param[out] value_p ビットベクタの格納先
  static
  void
  bitvector2vpi_value(const BitVector& bv,
		      p_vpi_value value_p);

  /// @brief time を vpi_value に設定する．
  /// @param[in] timeval 設定する時刻
  /// @param[out] value_p 時刻の格納先
  static
  void
  time2vpi_value(const s_vpi_time& timeval,
		 p_vpi_value value_p);

  /// @brief time を vpi_value に設定する．
  /// @param[in] timeval 設定する時刻
  /// @param[out] value_p 時刻の格納先
  static
  void
  time2vpi_value(const VlTime& timeval,
		 p_vpi_value value_p);

  /// @brief time を vpi_value に設定する．
  /// @param[in] timeval 設定する時刻
  /// @param[out] value_p 時刻の格納先
  static
  void
  time2vpi_value(double timeval,
		 p_vpi_value value_p);

  /// @brief strength を vpi_value に設定する．
  /// @param[in] sarray 設定する strength の配列
  /// @param[out] value_p strength の格納先
  static
  void
  strength2vpi_value(const vector<s_vpi_strengthval>& sarray,
		     p_vpi_value value_p);

  /// @brief 文字列用バッファの確保
  /// @param[in] size 必要なバイト数(end-marker を含む)
  /// @return 確保したバッファの先頭アドレスを返す．
  static
  char*
  string_buff(ymuint32 size);

  /// @brief ビットベクタ用バッファの確保
  /// @param[in] size 必要なブロック数
  /// @return 確保したバッファの先頭アドレスを返す．
  static
  p_vpi_vecval
  vec_buff(ymuint32 size);

  /// @brief time 用バッファの確保
  /// @return 確保したバッファの先頭アドレスを返す．
  static
  s_vpi_time*
  time_buff();

  /// @brief strength 用バッファの確保
  /// @param[in] size 必要な要素数
  static
  p_vpi_strengthval
  strength_buff(ymuint32 size);


private:
  //////////////////////////////////////////////////////////////////////
  // 静的データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列用バッファ
  static
  char* mStrBuff;

  // 文字列用バッファのサイズ
  static
  ymuint32 mStrBuffSize;

  // ビットベクタ用バッファ
  static
  p_vpi_vecval mVecBuff;

  // ビットベクタ用バッファのサイズ
  static
  ymuint32 mVecBuffSize;

  // time 用バッファ
  static
  s_vpi_time mTimeBuff;

  // strength 用バッファ
  static
  p_vpi_strengthval mStrengthBuff;

  // strength 用バッファのサイズ
  static
  ymuint32 mStrengthBuffSize;
  
};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIBASE_H
