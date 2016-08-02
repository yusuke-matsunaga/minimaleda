#ifndef YM_VERILOG_BITVECTOR_H
#define YM_VERILOG_BITVECTOR_H

/// @file ym_verilog/BitVector.h
/// @brief BitVector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BitVector.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/VlTime.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class BitVector BitVector.h <ym_verilog/BitVector.h>
/// @ingroup VlCommon
/// @brief Verilog-HDL のビットベクタ値を表すクラス
///
/// 各ビットは 0, 1, X, Z の4値をとりうる
///
/// それ以外の属性としては
///   - サイズ
///   - 符号の有無
///   - 基数
///
/// を持つ．
//////////////////////////////////////////////////////////////////////
class BitVector
{
public:

  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/代入演算子
  /// @{

  /// @brief 空のコンストラクタ
  /// @param[in] val 値
  /// @note 符号なし整数からのキャスト用コンストラクタでもある
  /// @note 結果の型は
  /// - サイズは無し
  /// - 符号なし
  /// - 基数は10
  explicit
  BitVector(ymuint32 val = 0);
  
  /// @brief int からのキャスト用コンストラクタ
  /// @param[in] val 値
  /// @note 結果の型は
  /// - サイズは無し
  /// - 符号あり
  /// - 基数は10
  explicit
  BitVector(int val);
  
  /// @brief bool からの変換コンストラクタ
  /// @param[in] value ブール値
  /// @note 結果の型は
  /// - サイズはあり(1ビット)
  /// - 符号なし
  /// - 基数は2
  explicit
  BitVector(bool value);

  /// @brief time 型からの変換コンストラクタ
  /// @param[in] time time 値
  /// @note 結果の型は
  /// - サイズはあり(64ビット)
  /// - 符号なし
  /// - 基数は10
  explicit
  BitVector(const VlTime& time);
  
  /// @brief スカラ値からのキャスト用コンストラクタ
  /// @param[in] value 値 (kVpiScalar{0, 1, X, Z}
  /// @param[in] size サイズ (ビット幅)
  /// size を指定するとその数分 value を繰り返す．
  /// @note 結果の型は
  /// - サイズはあり(= size)
  /// - 符号なし
  /// - 基数は2
  explicit
  BitVector(tVpiScalarVal value,
	    ymuint32 size = 1);
  
  /// @brief C文字列からの変換用コンストラクタ
  /// @param[in] str 文字列 (C文字列)
  /// @note 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  explicit
  BitVector(const char* str);
  
  /// @brief string 文字列からの変換用コンストラクタ
  /// @param[in] str 文字列 (string)
  /// @note 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  explicit
  BitVector(const string& str);
  
  /// @brief 浮動小数点数をビットベクタにする
  /// @param[in] val 浮動小数点数
  /// @warning 整数で表せない範囲の場合には値は不定
  /// @todo 整数を経由しない方法に書き換えること
  explicit
  BitVector(double val);

  /// @brief Verilog-HDL 形式の文字列からの変換コンストラクタ
  /// @param[in] size サイズ
  /// @param[in] is_signed 符号の有無を表すフラグ
  /// @param[in] base 基数を表す数字 (2, 8, 10, 16 のみが妥当な値)
  /// @param[in] str 値の内容を表す文字列
  /// @note とはいってもサイズと基数は str に含まれていない
  BitVector(ymuint32 size,
	    bool is_signed,
	    ymuint32 base,
	    const string& str);

  /// @brief 連結演算用のコンストラクタ
  /// @param[in] src_list 連結する値のリスト
  /// @note src_list の内容を連結したものをセットする
  BitVector(const list<BitVector>& src_list);

  /// @brief 連結演算用のコンストラクタ
  /// @param[in] src_list 連結する値のリスト
  /// @note src_list の内容を連結したものをセットする
  BitVector(const vector<BitVector>& src_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BitVector(const BitVector& src);
  
  /// @brief ビット長の変換を行うコピーコンストラクタもどき
  /// @param[in] src 返還元ののビットベクタ
  /// @param[in] size 指定サイズ
  /// @note 変換ルールは以下の通り
  /// - src のビット長が size よりも短ければ適切な拡張を行う．
  /// - unsigned なら上位に0を補う．signed なら符号拡張を行う．
  /// - MSB が x か z の場合には x または z を補う．
  /// - src のビット長が size よりも長ければ切り捨てる．
  BitVector(const BitVector& src,
	    ymuint32 size);
  
  /// @brief ビット長の変換と属性の変更を行うコピーコンストラクタもどき
  /// @param[in] src 変換元のオブジェクト
  /// @param[in] size 指定サイズ
  /// @param[in] is_sized サイズの有無
  /// @param[in] is_signed 符号の有無
  /// @param[in] base 基数
  BitVector(const BitVector& src,
	    ymuint32 size,
	    bool is_sized,
	    bool is_signed,
	    ymuint32 base);
  
  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const BitVector&
  operator=(const BitVector& src);

  /// @brief 符号なし整数からの代入演算子
  /// @param[in] val 値
  /// @note 結果の型は
  /// - サイズは無し
  /// - 符号なし
  /// - 基数は10
  const BitVector&
  operator=(ymuint32 val);
  
  /// @brief int からの代入演算子
  /// @param[in] val 値
  /// @note 結果の型は
  /// - サイズは無し
  /// - 符号あり
  /// - 基数は10
  const BitVector&
  operator=(int val);
  
  /// @brief bool からの代入演算子
  /// @param[in] value ブール値
  /// @note 結果の型は
  /// - サイズはあり(1ビット)
  /// - 符号なし
  /// - 基数は2
  const BitVector&
  operator=(bool value);

  /// @brief time 型からの代入演算子
  /// @param[in] time time 値
  /// @note 結果の型は
  /// - サイズはあり(64ビット)
  /// - 符号なし
  /// - 基数は10
  const BitVector&
  operator=(const VlTime& time);
  
  /// @brief スカラ値からの代入演算子
  /// @param[in] value 値 (kVpiScalar{0, 1, X, Z}
  /// @note 結果の型は
  /// - サイズはあり(1ビット)
  /// - 符号なし
  /// - 基数は2
  const BitVector&
  operator=(tVpiScalarVal value);
  
  /// @brief C文字列からの代入演算子
  /// @param[in] str 文字列 (C文字列)
  /// @note 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  const BitVector&
  operator=(const char* str);
  
  /// @brief string 文字列からの代入演算子
  /// @param[in] str 文字列 (string)
  /// @note 結果の型は
  /// - サイズはあり
  /// - 符号なし
  /// - 基数は2
  const BitVector&
  operator=(const string& str);
  
  /// @brief 浮動小数点数からの代入演算子
  /// @param[in] val 浮動小数点数
  /// @warning 整数で表せない範囲の場合には値は不定
  /// @todo 整数を経由しない方法に書き換えること
  const BitVector&
  operator=(double val);
  
  /// @brief ビット長の変換と属性の変更を行う代入演算子もどき
  /// @param[in] src 変換元のオブジェクト
  /// @param[in] size 指定サイズ
  /// @param[in] is_sized サイズの有無
  /// @param[in] is_signed 符号の有無
  /// @param[in] base 基数
  void
  set_with_attr(const BitVector& src,
		ymuint32 size,
		bool is_sized,
		bool is_signed,
		ymuint32 base);

  /// @brief Verilog-HDL (IEEE1364-2001) 形式の文字列から値をセットする関数．
  /// @param[in] str Verilog形式の文字列
  /// @retval true 正しく変換できた
  /// @retval false str が形式に沿っていなかった
  /// @note もちろん IEEE1364-1995 の形式も OK
  bool
  set_from_verilog_string(const string& str);

  /// @brief 型変換を行う．
  /// @param[in] type 要求される型(サイズも含む)
  void
  coerce(tVpiValueType type);

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name スカラ値の繰り返しを作るクラスメソッド
  /// 上のコンストラクタを使うためには tVpiScalarVal の値を指定しなければならず
  /// かっこ悪いので
  /// @{

  /// @brief 0 を表すオブジェクトを生成する
  /// @param[in] size() ビット数．デフォルトは 1
  /// @return 生成されたオブジェクト
  static
  BitVector
  zero(ymuint32 size = 1);

  /// @brief 1 を表すオブジェクトを生成する
  /// @param size ビット数．デフォルトは 1
  /// @return 生成されたオブジェクト
  static
  BitVector
  one(ymuint32 size = 1);

  /// @brief X を表すオブジェクトを生成する
  /// @param size ビット数．デフォルトは 1
  /// @return 生成されたオブジェクト
  static
  BitVector
  x(ymuint32 size = 1);

  /// @brief Z を表すオブジェクトを生成する
  /// @param size ビット数．デフォルトは 1
  /// @return 生成されたオブジェクト
  static
  BitVector
  z(ymuint32 size = 1);

  /// @}
  //////////////////////////////////////////////////////////////////////

 
  //////////////////////////////////////////////////////////////////////
  // 以下の演算は IEEE1364-2001 section 4.1 準拠
  //////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  /// @name Arithmetic member operators (IEEE1364-2001 4.1.5)
  /// 結果の型は以下のように決定される．
  /// - 符号:
  ///   - ともに signed -> signed
  ///   - 上記以外      -> unsigned
  /// - size:
  ///   unsized のオペランドは integer とおなじとみなす．
  ///   で，2つのオペランドの size の大きい方に合わせる．
  ///   ただし，どちらも unsized なら結果も unsized
  /// - 基数: なし(10)
  /// @{
  
  /// @brief 2の補数を計算し自分自身に代入する．
  /// @return 自分自身を返す．
  const BitVector&
  complement();
  
  /// @brief 加算つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身に src を足したものを代入する
  const BitVector&
  operator+=(const BitVector& src);
  
  /// @brief 減算つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身から src を引いたものを代入する
  const BitVector&
  operator-=(const BitVector& src);
  
  /// @brief 乗算つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身に src を掛けたものを代入する
  const BitVector&
  operator*=(const BitVector& src);
  
  /// @brief 除算つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身を src で割ったものを代入する
  const BitVector&
  operator/=(const BitVector& src);
  
  /// @brief 剰余算つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身を src で割った余りを代入する
  const BitVector&
  operator%=(const BitVector& src);
  
  /// 巾乗つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身を src 乗したものを代入する
  const BitVector&
  power(const BitVector& src);

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name Bit-wise logical member operators (IEEE1364-2001 4.1.10)
  /// 2つのオペランドの長さが合わないときは長い方に合わせる．
  /// この場合は常に0が埋められる．
  ///
  /// 結果の型は以下のとおり
  /// - 符号:
  ///   - ともに signed -> signed
  ///   - 上記以外      -> unsigned
  /// - size:
  ///   - unsized のオペランドは integer とおなじとみなす．
  ///   - 2つのオペランドの size の大きい方に合わせる．
  ///   - ただし，どちらも unsized なら結果も unsized
  /// - 基数は2
  /// @{

  /// @brief NOT
  /// @return 自分自身を返す．
  /// @note 自分自身の値をビットごとに反転させる
  const BitVector&
  negate();

  /// @brief AND つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身と src との AND を代入する
  const BitVector&
  operator&=(const BitVector& src);
  
  /// @brief Or つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身と src との OR を代入する
  const BitVector&
  operator|=(const BitVector& src);
  
  /// @brief XOR つき代入
  /// @param[in] src オペランド
  /// @return 自分自身を返す．
  /// @note 自分自身と src との XOR を代入する
  const BitVector&
  operator^=(const BitVector& src);

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name Reduction operators (IEEE1364-2001 4.1.11)
  /// 結果の型は以下のとおり
  /// - 符号なし
  /// - サイズあり(1)
  /// - 基数は2
  /// @{

  /// @brief リダクションAND
  /// @return 演算結果を返す．
  /// @note すべてのビットの AND を計算する
  tVpiScalarVal
  reduction_and() const;
  
  /// @brief リダクションOR
  /// @return 演算結果を返す．
  /// @note すべてのビットの OR を計算する
  tVpiScalarVal
  reduction_or() const;
  
  /// @brief リダクションXOR
  /// @return 演算結果を返す．
  /// @note すべてのビットの XOR を計算する
  tVpiScalarVal
  reduction_xor() const;
  
  /// @brief リダクションNAND
  /// @return 演算結果を返す．
  /// @note すべてのビットの NAND を計算する
  tVpiScalarVal
  reduction_nand() const;
  
  /// @brief リダクションNOR
  /// @return 演算結果を返す．
  /// @note すべてのビットの NOR を計算する
  tVpiScalarVal
  reduction_nor() const;
  
  /// @brief リダクションXNOR
  /// @return 演算結果を返す．
  /// @note すべてのビットの XNOR を計算する
  tVpiScalarVal
  reduction_xnor() const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name Shift member operators (IEEE1364-2001 4.1.12)
  /// 結果の型は以下のとおり
  /// - 符号   : 第1オペランドと同じ
  /// - サイズ : 第1オペランドと同じ
  /// - 基数   : 第1オペランドと同じ
  /// @{

  /// @brief 論理左シフトつき代入
  /// @param[in] src シフト量 (BitVector)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ論理左シフトしたものを代入する．
  const BitVector&
  operator<<=(const BitVector& src);
  
  /// @brief 論理左シフトつき代入
  /// @param[in] src シフト量 (ymuint32)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ論理左シフトしたものを代入する．
  const BitVector&
  operator<<=(ymuint32 src);
  
  /// @brief 論理右シフトつき代入
  /// @param[in] src シフト量 (BitVector)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ論理右シフトしたものを代入する．
  const BitVector&
  operator>>=(const BitVector& src);
  
  /// @brief 論理右シフトつき代入
  /// @param[in] src シフト量 (ymuint32)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ論理右シフトしたものを代入する．
  const BitVector&
  operator>>=(ymuint32 src);
  
  /// @brief 算術左シフトつき代入
  /// @param[in] src シフト量 (BitVector)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ算術左シフトしたものを代入する．
  const BitVector&
  alshift(const BitVector& src);
  
  /// @brief 算術左シフトつき代入
  /// @param[in] src シフト量 (ymuint32)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ算術左シフトしたものを代入する．
  const BitVector&
  alshift(ymuint32 src);
  
  /// @brief 算術右シフトつき代入
  /// @param[in] src シフト量 (BitVector)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ算術右シフトしたものを代入する．
  const BitVector&
  arshift(const BitVector& src);
  
  /// @brief 算術右シフトつき代入
  /// @param[in] src シフト量 (ymuint32)
  /// @return 自分自身を返す．
  /// @note 自分自身を src だけ算術右シフトしたものを代入する．
  const BitVector&
  arshift(ymuint32 src);

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name Vector bit-select and part-select addressing (IEEE1364-2001 4.2.1)
  /// 範囲外は x を返す．
  ///
  /// 結果の型は以下の通り
  /// - 符号   : 常に unsigned
  /// - サイズ : 指定された値
  /// - 基数   : 2
  /// @{

  /// @brief part-select 演算子
  /// @param[in] msb 選択範囲の MSB
  /// @param[in] lsb 選択範囲の LSB
  /// @return 選択された範囲のビットベクタ
  BitVector
  part_select(int msb,
	      int lsb) const;
  
  /// @brief part-select 書き込み
  /// @param[in] msb 選択範囲の MSB
  /// @param[in] lsb 選択範囲の LSB
  /// @param[in] val 書き込む値
  /// @note [msb:lsb] の範囲に val を書き込む
  /// @note 範囲外ならなにもしない．
  void
  part_select(int msb,
	      int lsb,
	      const BitVector& val);
  
  /// @brief bit-select 演算子
  /// @param[in] bpos 選択するビット位置
  /// @return bpos で指定された位置の値
  tVpiScalarVal
  bit_select(int bpos) const;
  
  /// @brief bit-select 書き込み
  /// @param[in] bpos 選択するビット位置
  /// @param[in] val 書き込む値
  /// @note bpos の位置に val を書き込む．
  /// @note 範囲外ならないもしない．
  void
  bit_select(int bpos,
	     tVpiScalarVal val);

  /// @}
  //////////////////////////////////////////////////////////////////////
  

  //////////////////////////////////////////////////////////////////////
  /// @name その他のメンバ関数
  /// @{
  
  /// @brief srcの値をビットごとにマージする．
  /// @param[in] src オペランド
  /// @return 自分自身
  /// @note 異なっているビットは X となる．
  const BitVector&
  merge(const BitVector& src);
  
  /// @brief サイズを返す．
  ymuint32
  size() const;
  
  /// @brief 実際のサイズはともかくサイズの指定があるかどうかを返す．
  bool
  is_sized() const;
  
  /// @brief 符号付きの場合に true を返す
  bool
  is_signed() const;
  
  /// @brief 表示用の基数を得る．
  ymuint32
  base() const;
  
  /// @brief pos ビット目の値を得る．
  /// @param[in] pos 取得するビット位置
  /// @return pos ビット目の値を返す．
  /// @return pos が範囲を越えていたら kVpiScalarX を返す．
  tVpiScalarVal
  value(int pos) const;
  
  /// @brief pos ビット目の値をセットする．
  /// @param[in] pos 値を設定するビット位置
  /// @param[in] val 設定する値
  /// @note pos が範囲を越えていたら何もしない
  void
  set_value(int pos,
	    tVpiScalarVal val);
  
  /// @brief 符号付きかつ負数の時に true を返す
  bool
  is_negative() const;
  
  /// @brief x 値を含んでいたら true を返す
  bool
  has_x() const;
  
  /// @brief z 値を含んでいたら true を返す
  bool
  has_z() const;
  
  /// @brief x/z 値を含んでいたら true を返す
  bool
  has_xz() const;
  
  /// @brief z を x に変える．
  /// @note ほとんどの演算で z は x と区別されていない
  void
  z_to_x();
  
  /// @brief x/z を 0 に変える．
  /// @note Verilog-HDL で4値を2値に強引に変換するときのやり方
  void
  xz_to_0();
  
  /// @brief 32 ビットの符合なし数に変換可能なら true を返す．
  /// @note 具体的には size() が 32以下で x や z を含んでいないこと
  bool
  is_uint32() const;
  
  /// @brief is_uint32 の条件を満たしているときに ymuint32 に変換する．
  /// @note 上の条件を満たしていないときの値は不定
  /// (というか実際にどういう値を返すのかはソースコードを見ればわかる)
  ymuint32
  to_uint32() const;
  
  /// @brief int の数値に変換可能なら true を返す．
  /// @note 実際には is_uint32() と同一
  bool
  is_int() const;
  
  /// @brief is_int の条件を満たしているときに int に変換する．
  /// @note 上の条件を満たしていないときの値は不定
  /// (というか実際にどういう値を返すのかはソースコードを見ればわかる)
  int
  to_int() const;
  
  /// @brief 値を double 型に変換する．
  /// @note X/Z は0と見なす．
  double
  to_real() const;
  
  /// @brief 値をバイトベクターと見なして文字列に変換する．
  /// @note 長さが8の倍数でない場合にはパディングする．
  string
  to_string() const;
  
  /// @brief 1ビットのスカラー値に変換する．
  /// @note 実際には LSB を返すだけ．
  tVpiScalarVal
  to_scalar() const;

  /// @brief 論理値として評価する．
  /// @retval kVpiScalar0 0 の時
  /// @retval kVpiScalar1 0 以外の確定値の時
  /// @retval kVpiScalarX 不定値を1ビットでも含む場合
  tVpiScalarVal
  to_logic() const;

  /// @brief 内容をブール値に変換する．
  /// @note to_logic() と同様で，かつ X/Z を false と見なす．
  bool
  to_bool() const;
  
  /// @brief time 型に変換可能なら true を返す．
  /// @note 具体的には size() が 64 以下で X/Z を含んでいないこと
  bool
  is_time() const;

  /// @brief 内容を time 型に変換する．
  /// @note 具体的には下位64ビットを設定するだけなのでオーバーフロー
  /// しているときには値は正しくない．
  VlTime
  to_time() const;

  /// @brief 内容を Verilog-HDL (IEEE1364-2001) の形式の文字列に変換する．
  /// @param[in] opt_base 基数
  /// @return Verilog-HDL 形式の文字列表現を返す．
  /// @note opt_base が 2, 8, 10, 16 の時には内部で持っている基数を無視して
  /// opt_base を基数と見なす．
  string
  verilog_string(ymuint32 opt_base = 0) const;
  
  /// @brief 内容を10進数で表した文字列を返す．
  string
  dec_str() const;
  
  /// @brief 内容を2進数で表した文字列を返す．
  /// @param[in] skip_zeros 0スキップフラグ
  /// @return 内容を2進数で表した文字列を返す．
  /// @note skip_zeros が true なら上位は0は出力しない．
  string
  bin_str(bool skip_zeros = true) const;
  
  /// @brief 内容を8進数で表した文字列を返す．
  /// @param[in] skip_zeros 0スキップフラグ
  /// @return 内容を8進数で表した文字列を返す．
  /// @note skip_zeros が true なら上位は0は出力しない．
  string
  oct_str(bool skip_zeros = true) const;
  
  /// @brief 内容を16進数で表した文字列を返す．
  /// @param[in] skip_zeros 0スキップフラグ
  /// @return 内容を16進数で表した文字列を返す．
  /// @note skip_zeros が true なら上位は0は出力しない．
  string
  hex_str(bool skip_zeros = true) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算用の関数
  /// @{

  /// @brief 大小比較演算用の共通関数
  /// @param[in] src1, src2 オペランド
  /// @return src1 < src2 の時 true を返す．
  static
  bool
  lt_base(const BitVector& src1,
	  const BitVector& src2);

  /// @brief 等価比較演算用の共通関数
  /// @param[in] src1, src2 オペランド
  /// @param[in] mode:
  ///   - 1 普通の等価比較
  ///   - 2 x をワイルドカードと見なす等価比較
  ///   - 3 x と z をワイルドカードと見なす等価比較
  /// @return 等しいと見なすとき true を返す．
  static
  bool
  eq_base(const BitVector& src1,
	  const BitVector& src2,
	  int mode);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  
  // mVal0, mVal1 のリサイズをする．
  void
  resize(ymuint32 size);
  
  // 属性(サイズの有無, 符号の有無, 基数)をセットする．
  void
  set_type(bool has_size,
	   bool has_sign,
	   ymuint32 base);
  
  // 値をセットする．(1ワードバージョン)
  void
  set(ymuint32 val0,
      ymuint32 val1,
      ymuint32 size,
      bool is_sized,
      bool is_signed,
      int base);
  
  // 値をセットする．(ベクタバージョン)
  void
  set(const ymuint32* val0,
      const ymuint32* val1,
      ymuint32 src_size,
      ymuint32 size,
      bool is_sized,
      bool is_signed,
      int base);
  
  // 値をセットする．(ベクタバージョン)
  void
  set(const vector<ymuint32>& val0,
      const vector<ymuint32>& val1,
      ymuint32 src_size,
      ymuint32 size,
      bool is_sized,
      bool is_signed,
      int base);

  // Verilog 形式の2進数から変換するための共通ルーティン
  void
  set_from_binstring(ymuint32 size,
		     bool is_sized,
		     bool is_signed,
		     const string& str,
		     ymuint32 pos);

  // Verilog 形式の8進数から変換するための共通ルーティン
  void
  set_from_octstring(ymuint32 size,
		     bool is_sized,
		     bool is_signed,
		     const string& str,
		     ymuint32 pos);

  // Verilog 形式の10進数から変換するための共通ルーティン
  void
  set_from_decstring(ymuint32 size,
		     bool is_sized,
		     bool is_signed,
		     const string& str,
		     ymuint32 pos);

  // Verilog 形式の16進数から変換するための共通ルーティン
  void
  set_from_hexstring(ymuint32 size,
		     bool is_sized,
		     bool is_signed,
		     const string& str,
		     ymuint32 pos);
  
  // 文字列からの変換用コンストラクタの共通ルーティン
  void
  set_from_string(ymuint32 strsize,
		  const char* str);

  /// @brief src を 10 で割る．
  /// @param[in] src 元の値
  /// @param[out] q 求まった商を格納する変数
  /// @return 余りを返す．
  static
  ymuint32
  div10(const ymuint32* src,
	ymuint32 n,
	ymuint32* q);

  /// @brief val を10進数で表した文字列を返す．
  static
  string
  dec_str_sub(const ymuint32* val,
	      ymuint32 n);

  /// @brief ビット長からブロック数を得る．
  static
  ymuint32
  block(ymuint32 size);

  /// @brief ビット長から最後のブロックのシフト数を得る．
  static
  ymuint32
  shift(ymuint32 size);

  /// @brief size からマスクパタンを作る
  static
  ymuint32
  mask(ymuint32 size);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット長
  ymuint32 mSize;

  // sized, signed, base をパックした変数
  ymuint32 mFlags;

  // 値を保持するベクタ
  // サイズは block(mSize)
  // mVal0:Val1 の組み合わせで値を表す．
  ymuint32* mVal0;
  ymuint32* mVal1;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  /// @brief ymuint32 のビット長
  static
  const ymuint32 kBlockSize = sizeof(ymuint32) * 8;

  /// @brief すべてが0のパタン
  static
  const ymuint32 kAll0 = 0x00000000;

  /// @brief すべてが1のパタン
  static
  const ymuint32 kAll1 = 0xFFFFFFFF;

};


//////////////////////////////////////////////////////////////////////
/// @name Arithmetic operators (IEEE1364-2001 4.1.5)
/// 結果の型は以下のように決定される．
/// - 符号:
///   - ともに signed -> signed
///   - 上記以外      -> unsigned
/// - size:
///   - unsized のオペランドは integer とおなじとみなす．
///   - 2つのオペランドの size の大きい方に合わせる．
///   - ただし，どちらも unsized なら結果も unsized
/// - 基数はなし(10)
/// @{

/// @relates BitVector
/// @brief 単項のマイナス(complement)演算子
/// @return 2の補数を返す．
///
/// 2の補数を求める．
/// パタンとして2の補数を求めるだけで
/// 符号の有無は変わらない．
BitVector
operator-(const BitVector& src);

/// @relates BitVector
/// @brief 加算
/// @param[in] src1, src2 オペランド
/// @return src1 + src2 を返す
BitVector
operator+(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief 減算
/// @param[in] src1, src2 オペランド
/// @return src1 - src2 を返す
BitVector
operator-(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief 乗算
/// @param[in] src1, src2 オペランド
/// @return src1 * src2 を返す
BitVector
operator*(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief 除算
/// @param[in] src1, src2 オペランド
/// @return src1 / src2 を返す
BitVector
operator/(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief 剰余算
/// @param[in] src1, src2 オペランド
/// @return src1 % src2 を返す
BitVector
operator%(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief 巾乗
/// @param[in] src1, src2 オペランド
/// @return src1 の src2 乗を返す
BitVector
power(const BitVector& src1,
      const BitVector& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Relational operators (IEEE1364-2001 4.1.7)
/// 結果が x になる場合があるので bool ではなく BitVector を返す．
///
/// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
///
/// ともに符号付きの場合には符号付き数として比較する．
///
/// そうでなければ符号なし数として比較する．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates BitVector
/// @brief less than 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 < src2 の時
/// @retval kVpiScalar0 src1 >= src2 の時
/// @retval kVpiScalarX 比較不能の時
tVpiScalarVal
lt(const BitVector& src1,
   const BitVector& src2);

/// @relates BitVector
/// @brief less than 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 < src2 を返す
bool
operator<(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief greater than 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 > src2 の時
/// @retval kVpiScalar0 src1 <= src2 の時
/// @retval kVpiScalarX 比較不能の時
tVpiScalarVal
gt(const BitVector& src1,
   const BitVector& src2);

/// @relates BitVector
/// @brief greater than 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 > src2 を返す
bool
operator>(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief less than or equal 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 <= src2 の時
/// @retval kVpiScalar0 src1 > src2 の時
/// @retval kVpiScalarX 比較不能の時
tVpiScalarVal
le(const BitVector& src1,
   const BitVector& src2);

/// @relates BitVector
/// @brief less than or equal 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 <= src2 を返す
bool
operator<=(const BitVector& src1,
	   const BitVector& src2);

/// @relates BitVector
/// @brief greater than or equal 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 >= src2 の時
/// @retval kVpiScalar0 src1 < src2 の時
/// @retval kVpiscalarX 比較不能の時
tVpiScalarVal
ge(const BitVector& src1,
   const BitVector& src2);

/// @relates BitVector
/// @brief greater than or equal 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 >= src2 を返す
bool
operator>=(const BitVector& src1,
	   const BitVector& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Equality operators (IEEE1364-2001 4.1.8)
/// 結果が x になる場合があるので bool ではなく BitVector を返す．
///
/// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates BitVector
/// @brief 等価比較演算子
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 == src2 の時
/// @retval kVpiScalar0 src1 != src2 の時
/// @retval kVpiScalarX 比較不能の時
tVpiScalarVal
eq(const BitVector& src1,
   const BitVector& src2);
  
/// @relates BitVector
/// @brief x が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @param[in] src1, src2 オペランド
/// @return 等価と見なせるとき true を返す．
bool
eq_with_x(const BitVector& src1,
	  const BitVector& src2);
  
/// @relates BitVector
/// @brief x および z が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @param[in] src1, src2 オペランド
/// @return 等価と見なせるとき true を返す．
bool
eq_with_xz(const BitVector& src1,
	   const BitVector& src2);

/// @relates BitVector
/// @brief 等価比較演算子 (bool)
/// @param[in] src1, src2 オペランド
/// @return 1 src1 == src2 を返す．
bool
operator==(const BitVector& src1,
	   const BitVector& src2);

/// @relates BitVector
/// @brief 非等価比較演算子
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 != src2 の時
/// @retval kVpiScalar0 src1 == src2 の時
/// @retval kVpiScalarX 比較不能の時
tVpiScalarVal
ne(const BitVector& src1,
   const BitVector& src2);

/// @relates BitVector
/// @brief 非等価比較演算子 (bool)
/// @param[in] src1, src2 オペランド
/// @retval 1 src1 != src2 を返す．
bool
operator!=(const BitVector& src1,
	   const BitVector& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Logical operators (scalar) (IEEE1364-2001 4.1.9)
/// 結果が x になる場合があるので bool ではなく BitVector を返す．
///
/// オペランドがスカラーで無い場合には強制的にスカラーに変換する．
/// 具体的には最下位ビットをそのままスカラー値だと思う．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates BitVector
/// @brief NOT演算
/// @param[in] src オペランド
/// @retval kVpiScalar0 src が真の時
/// @retval kVpiScalar1 src が偽の時
/// @retval kVpiScalarX 計算不能の時
tVpiScalarVal
operator!(const BitVector& src);

/// @relates BitVector
/// @brief AND演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar0 src1 と src2 のどちらか一方が偽の時
/// @retval kVpiScalar1 src1 と src2 がともに真の時
/// @retval kVpiScalarX 計算不能の時
tVpiScalarVal
operator&&(const BitVector& src1,
	   const BitVector& src2);

/// @relates BitVector
/// @brief OR演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar0 src1 と src2 がともに偽の時
/// @retval kVpiScalar1 src1 と src2 のどちらか一方が真の時
/// @retval kVpiScalarX 計算不能の時
tVpiScalarVal
operator||(const BitVector& src1,
	   const BitVector& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Bit-wise logical operators (IEEE1364-2001 4.1.10)
/// 2つのオペランドの長さが合わないときは長い方に合わせる．
/// この場合は常に0が埋められる．
///
/// 結果の型は以下のとおり
/// - 符号:
///   - ともに signed -> signed
///   - 上記以外      -> unsigned
/// - size:
///   - unsized のオペランドは integer とおなじとみなす．
///   - 2つのオペランドの size の大きい方に合わせる．
///   - ただし，どちらも unsized なら結果も unsized
/// - 基数は2
/// @{

/// @relates BitVector
/// @brief bitwise NOT 演算
/// @param[in] src オペランド
/// @return src のビットごとに否定したもの
BitVector
operator~(const BitVector& src);

/// @relates BitVector
/// @brief bitwise AND 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 をビットごとに AND したもの
BitVector
operator&(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief bitwise OR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 をビットごとに OR したもの
BitVector
operator|(const BitVector& src1,
	  const BitVector& src2);

/// @relates BitVector
/// @brief bitwise XOR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 をビットごとに XOR したもの
BitVector
operator^(const BitVector& src1,
	  const BitVector& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Shift operators (IEEE1364-2001 4.1.12)
/// 結果の型は以下のとおり
/// - 符号   : 第1オペランドと同じ
/// - サイズ : 第1オペランドと同じ
/// - 基数   : 第1オペランドと同じ
/// @{

/// @relates BitVector
/// @brief 論理左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (BitVector)
/// @return src1 を src2 だけ論理左シフトしたもの
BitVector
operator<<(const BitVector& src1,
	   const BitVector& src2);

/// @relates BitVector
/// @brief 論理左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ論理左シフトしたもの
BitVector
operator<<(const BitVector& src1,
	   ymuint32 src2);

/// @relates BitVector
/// @brief 論理右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (BitVector)
/// @return src1 を src2 だけ論理右シフトしたもの
BitVector
operator>>(const BitVector& src1,
	   const BitVector& src2);

/// @relates BitVector
/// @brief 論理右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ論理右シフトしたもの
BitVector
operator>>(const BitVector& src1,
	   ymuint32 src2);

/// @relates BitVector
/// @brief 算術左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (BitVector)
/// @return src1 を src2 だけ算術左シフトしたもの
/// 算術左シフト
BitVector
alshift(const BitVector& src1,
	const BitVector& src2);

/// @relates BitVector
/// @brief 算術左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ算術左シフトしたもの
BitVector
alshift(const BitVector& src1,
	ymuint32 src2);

/// @relates BitVector
/// @brief 算術右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (BitVector)
/// @return src1 を src2 だけ算術右シフトしたもの
BitVector
arshift(const BitVector& src1,
	const BitVector& src2);

/// @relates BitVector
/// @brief 算術右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ算術右シフトしたもの
BitVector
arshift(const BitVector& src1,
	ymuint32 src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Conditional operators (IEEE1364-2001 4.1.13)
/// 第1オペランドは強制的にスカラーに変換される．
///
/// 結果の型は以下のとおり
/// - 符号:
///   - 第2,第3オペランドがともに signed -> signed
///   - 上記以外 -> unsigned
/// - サイズ:
///   - 第2,第3オペランドの長い方に合わせる．
///   - ただし unsized なら integer と同じと見なす．
///   - ともに unsized なら 結果も unsized
/// - 基数:
///   - 第1オペランドが 1 の時には第2オペランドの基数
///   - 第1オペランドが 0 の時には第3オペランドの基数
///   - 第1オペランドが x, z の時に第2オペランドと
///     第3オペランドの基数が異なっていたら2とする．
/// @{

/// @relates BitVector
/// @brief 条件演算
/// @param[in] src1 条件
/// @param[in] src2 src1 が真の時に選ばれる値
/// @param[in] src3 src1 が偽の時に選ばれる値
/// @return 演算結果
BitVector
ite(const BitVector& src1,
    const BitVector& src2,
    const BitVector& src3);

/// @relates BitVector
/// @brief 条件演算
/// @param[in] src1 条件
/// @param[in] src2 src1 が真の時に選ばれる値
/// @param[in] src3 src1 が偽の時に選ばれる値
/// @return 演算結果
BitVector
ite(tVpiScalarVal src1,
    const BitVector& src2,
    const BitVector& src3);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Concatenation (IEEE1364-2001 4.1.14)
/// 結果の型は以下の通り
/// - 符号: 常に unsigned
/// - サイズ: 連結するオペランドのサイズの和．
///           もしもオペランドに unsized な値が含まれていたらエラー
///           (今はintegerと見なす)
/// - 基数: 常に2
/// @{

/// @relates BitVector
/// @brief 連結演算
/// @param[in] src_list 連結する値のリスト
/// @return 連結した結果を返す．
BitVector
concat(const list<BitVector>& src_list);

/// @relates BitVector
/// @brief 連結演算
/// @param[in] src_list 連結する値のリスト
/// @return 連結した結果を返す．
BitVector
concat(const vector<BitVector>& src_list);

/// @relates BitVector
/// @brief 繰り返し連結演算
/// @param[in] rep 繰り返し数
/// @param[in] src_list 連結する値のリスト
/// @return src_list の内容を rep 回繰り返して連結したもの
BitVector
multi_concat(const BitVector& rep,
	     const list<BitVector>& src_list);

/// @relates BitVector
/// @brief 繰り返し連結演算
/// @param[in] rep 繰り返し数
/// @param[in] src_list 連結する値のリスト
/// @return src_list の内容を rep 回繰り返して連結したもの
BitVector
multi_concat(const BitVector& rep,
	     const vector<BitVector>& src_list);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name ストリーム出力
/// @{

/// @relates BitVector
/// @brief Verilog-HDL (IEEE1364-2001) の形式で出力する．
/// @param[in] s 出力ストリーム
/// @param[in] bitval 出力対象のビットベクタ
/// @return s を返す．
ostream&
operator<<(ostream& s,
	   const BitVector& bitval);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 単項のマイナス(complement)演算子
inline
BitVector
operator-(const BitVector& src)
{
  return BitVector(src).complement();
}

// 加算
inline
BitVector
operator+(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) += src2;
}

// 減算
inline
BitVector
operator-(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) -= src2;
}

// 乗算
inline
BitVector
operator*(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) *= src2;
}

// 除算
inline
BitVector
operator/(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) /= src2;
}

// 剰余算
inline
BitVector
operator%(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) %= src2;
}

// 巾乗
inline
BitVector
power(const BitVector& src1,
      const BitVector& src2)
{
  return BitVector(src1).power(src2);
}

// greater than 比較演算
inline
tVpiScalarVal
gt(const BitVector& src1,
   const BitVector& src2)
{
  return lt(src2, src1);
}

// greater than 比較演算 (bool)
inline
bool
operator>(const BitVector& src1,
	  const BitVector& src2)
{
  return operator<(src2, src1);
}

// less than or equal 比較演算
inline
tVpiScalarVal
le(const BitVector& src1,
   const BitVector& src2)
{
  return !lt(src2, src1);
}

// less than or equal 比較演算 (bool)
inline
bool
operator<=(const BitVector& src1,
	   const BitVector& src2)
{
  return !operator<(src2, src1);
}

// greater than or equal 比較演算
inline
tVpiScalarVal
ge(const BitVector& src1,
   const BitVector& src2)
{
  return !lt(src1, src2);
}

// greater than or equal 比較演算 (bool)
inline
bool
operator>=(const BitVector& src1,
	   const BitVector& src2)
{
  return !operator<(src1, src2);
}

// 非等価比較演算子
inline
tVpiScalarVal
ne(const BitVector& src1,
   const BitVector& src2)
{
  return !eq(src1, src2);
}

// 非等価比較演算子 (bool)
inline
bool
operator!=(const BitVector& src1,
	   const BitVector& src2)
{
  return !operator==(src1, src2);
}

// 論理否定
inline
BitVector
operator~(const BitVector& src)
{
  return BitVector(src).negate();
}

// 論理積
inline
BitVector
operator&(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) &= src2;
}

// 論理和
inline
BitVector
operator|(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) |= src2;
}

// 排他的論理和
inline
BitVector
operator^(const BitVector& src1,
	  const BitVector& src2)
{
  return BitVector(src1) ^= src2;
}

// 論理左シフト
inline
BitVector
operator<<(const BitVector& src1,
	   const BitVector& src2)
{
  return BitVector(src1) <<= src2;
}

// 論理左シフト src2 が ymuint32 のバージョン
inline
BitVector
operator<<(const BitVector& src1,
	   ymuint32 src2)
{
  return BitVector(src1) <<= src2;
}

// 論理右シフト
inline
BitVector
operator>>(const BitVector& src1,
	   const BitVector& src2)
{
  return BitVector(src1) >>= src2;
}

// 論理右シフト src2 が ymuint32 のバージョン
inline
BitVector
operator>>(const BitVector& src1,
	   ymuint32 src2)
{
  return BitVector(src1) >>= src2;
}

// 算術左シフトつき代入
inline
const BitVector&
BitVector::alshift(const BitVector& src)
{
  // 実は論理左シフトそのもの
  return operator<<=(src);
}

// 算術左シフトつき代入
// src が ymuint32 のバージョン
inline
const BitVector&
BitVector::alshift(ymuint32 src)
{
  // 実は論理左シフトそのもの
  return operator<<=(src);
}

// 算術左シフト
inline
BitVector
alshift(const BitVector& src1,
	const BitVector& src2)
{
  // 実は論理左シフトそのもの
  return operator<<(src1, src2);
}

// 算術左シフト src2 が ymuint32 のバージョン
inline
BitVector
alshift(const BitVector& src1,
	ymuint32 src2)
{
  // 実は論理左シフトそのもの
  return operator<<(src1, src2);
}

// 算術右シフト
inline
BitVector
arshift(const BitVector& src1,
	const BitVector& src2)
{
  return BitVector(src1).arshift(src2);
}

// 算術右シフト src2 が ymuint32 のバージョン
inline
BitVector
arshift(const BitVector& src1,
	ymuint32 src2)
{
  return BitVector(src1).arshift(src2);
}

// サイズを返す．
inline
ymuint32
BitVector::size() const
{
  return mSize;
}

// 実際のサイズはともかくサイズの指定があるかどうかを返す．
inline
bool
BitVector::is_sized() const
{
  return mFlags & 1;
}

// 符号付きの場合に true を返す
inline
bool
BitVector::is_signed() const
{
  return (mFlags >> 1) & 1;
}

// 表示用の基数を得る．
inline
ymuint32
BitVector::base() const
{
  return mFlags >> 2;
}

// 符合付きかつ負数の時に true を返す
inline
bool
BitVector::is_negative() const
{
  return is_signed() && value(size() - 1) == kVpiScalar1;
}

// 32ビットの符合なし数に変換可能なら true を返す．
// 具体的には size() が 32以下で x や z を含んでいないこと
inline
bool
BitVector::is_uint32() const
{
  return (size() <= kBlockSize && !has_xz());
}

// 上の条件を満たしているときに ymuint32 に変換する．
// 上の条件を満たしていないときの値は不定
// (というか実際にどういう値を返すのかは見ればわかる)
inline
ymuint32
BitVector::to_uint32() const
{
  return mVal1[0];
}

// int の数値に変換可能なら true を返す．
// 具体的には size が kBlockSize 以下で x や z を含んでいないこと
inline
bool
BitVector::is_int() const
{
  return (size() <= kBlockSize && !has_xz());
}

// 上の条件を満たしているときに int に変換する．
// 上の条件を満たしていないときの値は不定
// (というか実際にどういう値を返すのかは見ればわかる)
inline
int
BitVector::to_int() const
{
  return static_cast<int>(mVal1[0]);
}

// 1ビットのスカラー値に変換する．
// 実際には LSB を返すだけ．
inline
tVpiScalarVal
BitVector::to_scalar() const
{
  return value(0);
}

// 内容をブール値に変換する．
inline
bool
BitVector::to_bool() const
{
  return to_logic() == kVpiScalar1;
}

// time 型に変換可能なら true を返す．
// 具体的には size が 64 以下で x や z を含んでいないこと
inline
bool
BitVector::is_time() const
{
  return (size() <= 64 && !has_xz());
}

// @brief 内容を time 型に変換する．
inline
VlTime
BitVector::to_time() const
{
  PLI_UINT32 l = static_cast<PLI_UINT32>(mVal1[0]);
  PLI_UINT32 h = static_cast<PLI_UINT32>(mVal1[1]);
  return VlTime(l, h);
}

// bit-select 演算子
inline
tVpiScalarVal
BitVector::bit_select(int bpos) const
{
  // 範囲外のチェックは value() でやっている．
  return value(bpos);
}

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_BITVECTOR_H
