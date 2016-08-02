#ifndef YM_VERILOG_VPI_VPIOBJ_H
#define YM_VERILOG_VPI_VPIOBJ_H

/// @file ym_verilog/vpi/VpiObj.h
/// @brief VpiObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiObj.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/verilog.h>
#include <ym_utils/FileRegion.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VpiObj VpiObj.h <ym_verilog/vpi/VpiObj.h>
/// @ingroup VlParser
/// @brief Verilog-HDL の内部構造を表すクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class VpiObj
{
  friend class VpiHandle;

public:
  
  /// @brief デストラクタ
  virtual
  ~VpiObj() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VPI 関数をオブジェクト指向にしたもの
  //////////////////////////////////////////////////////////////////////

  /// @brief property に対応したブール型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応したブール型の値を返す．
  virtual
  bool
  get_bool(int property) const = 0;

  /// @brief property に対応した整数型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応した整数型の値を返す．
  virtual
  int
  get_int(int property) const = 0;
  
  /// @brief property に対応した文字列型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応した文字列型の値を返す．
  virtual
  string
  get_str(int property) const = 0;
  
  /// @brief property に対応したオブジェクトを返す．
  /// @param[in] property 取り出す項目を表す値
  /// @brief return に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj(int property) const = 0;
  
  /// @brief property に対応した反復子を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_iterator_obj(int property) = 0;

  /// @brief インデックスに対応したオブジェクトの取得
  /// @param[in] index 取得する要素の位置
  /// @return index に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj_by_index(int index) const = 0;
  
  /// @brief 多次元のインデックスに対応したオブジェクトの取得
  /// @param[in] index_array 取得する要素の位置を表すインデックスのベクタ
  /// @return index_array に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj_by_index(const vector<int>& index_array) const = 0;
  
  /// @brief name に対応したオブジェクトの取得
  /// @param[in] name 取得する要素の名前
  /// @return name に対応したオブジェクト
  virtual
  VpiObj*
  get_obj_by_name(const string& name) const = 0;

  /// @brief 次の要素の取得
  /// @return 次の要素を返す．
  /// @note vpiIterater タイプの時の意味を持つ．
  virtual
  VpiObj*
  scan() = 0;

  /// @brief 空の反復子のチェック
  /// @return 空の反復子の時 true を返す．
  /// @note vpiIterator タイプの時のみ意味を持つ．
  virtual
  bool
  is_empty_iterator() const = 0;
  
  /// @brief シミュレーション時刻の取得
  /// @param[out] time_p 得られたシミュレーション時刻を格納する構造体
  /// @note このオブジェクトのタイムスケールを使って今のシミュレーション
  /// @note 時刻を得る．
  virtual
  void
  get_time(p_vpi_time time_p) const = 0;
  
  /// @brief 遅延情報の取得
  /// @param[out] delay_p 遅延情報を格納する構造体
  virtual
  void
  get_delays(p_vpi_delay delay_p) const = 0;
  
  /// @brief 遅延情報の設定
  /// @param[in] delay_p 設定する遅延情報
  virtual
  void
  put_delays(p_vpi_delay delay_p) = 0;
  
  /// @brief オブジェクトの「値」の取得
  /// @param[out] value_p 取得した値を格納する構造体
  virtual
  void
  get_value(p_vpi_value value_p) const = 0;
  
  /// @brief オブジェクトの「値」の設定
  /// @param[in] value_p 設定する値
  /// @param[in] time_p 設定する時刻
  /// @param[in] flags 種々のフラグ
  virtual
  VpiObj*
  put_value(p_vpi_value value_p,
	    p_vpi_time time_p,
	    int flags) = 0;
  
  /// @brief ユーザー定義システムタスク/関数情報の取得
  /// @param[out] systf_data_p システムタスク/関数情報
  virtual
  void
  get_systf_info(p_vpi_systf_data systf_data_p) const = 0;
  
  /// @brief ユーザー定義システムタスク/関数に関連づけられたデータの取得
  /// @return put_userdata で設定されたデータn
  virtual
  void*
  get_userdata() const = 0;
  
  /// @brief ユーザー定義システムタスク/関数に関連づけられたデータの設定
  /// @param[in] userdata 設定するデータ
  virtual
  void
  put_userdata(void* userdata) const = 0;
  
  /// @brief コールバック情報の取得
  /// @param[out] cb_data_p コールバック情報を格納する構造体
  virtual
  void
  get_cb_info(p_cb_data cb_data_p) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 型の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型を返す．
  virtual
  int
  type() const = 0;

  /// @brief 型を表す文字列の取得
  /// @return 型を表す文字列を返す．
  virtual
  string
  s_type() const = 0;

  
public:
  //////////////////////////////////////////////////////////////////////
  // ファイル位置の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 詳細なファイル位置の取得
  /// @return ファイル位置の情報を返す．
  virtual
  FileRegion
  file_region() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 参照回数のアクセス
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす．
  virtual
  void
  inc_ref() = 0;

  /// @brief 参照回数を減らす
  /// @note 場合によっては自身を削除する．
  virtual
  void
  dec_ref() = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIOBJ_H
