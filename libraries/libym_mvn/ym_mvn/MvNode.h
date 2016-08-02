#ifndef YM_MVN_MVNODE_H
#define YM_MVN_MVNODE_H

/// @file ym_mvn/MvNode.h
/// @brief MvNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvNode MvNode.h "ym_mvn/MvNode.h"
/// @brief MvNetwork のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvNode
{
  friend class MvMgr;

public:
  //////////////////////////////////////////////////////////////////////
  /// @brief 型定義
  /// @note 論理演算とビットワイズ論理演算の区別はない．
  //////////////////////////////////////////////////////////////////////
  enum tType {
    /// @brief 入力 ( 0入力, 1出力 )
    kInput,
    /// @brief 出力 ( 1入力, 0出力 )
    kOutput,
    /// @brief 入出力 ( 1入力, 1出力 )
    kInout,

    /// @brief フリップフロップ1
    /// (クロック，データ入力，非同期リセット，非同期セット，データ出力)
    /// リセット，セットは使用しない場合もある．
    kDff1,

    /// @brief フリップフロップ2
    /// (クロック，データ入力，同期リセット，同期セット，データ出力)
    /// リセット，セットは使用しない場合もある．
    kDff2,

    /// @brief through ( 1入力, 1出力 )
    kThrough,

    /// @brief logical not ( 1入力, 1出力 )
    kNot,
    /// @brief logical and ( 2入力, 1出力 )
    kAnd,
    /// @brief logical or ( 2入力, 1出力 )
    kOr,
    /// @brief logical xor ( 2入力, 1出力 )
    kXor,

    /// @brief reduction and ( 1入力, 1出力 )
    kRand,
    /// @brief reduction or ( 1入力, 1出力 )
    kRor,
    /// @brief reduction xor ( 1入力, 1出力 )
    kRxor,

    /// @brief equal ( 2入力, 1出力 )
    kEq,
    /// @brief less than ( 2入力, 1出力 )
    kLt,

    /// @brief shift left logical ( 2入力, 1出力 )
    kSll,
    /// @brief shift right logical ( 2入力, 1出力 )
    kSrl,
    /// @brief shift left arithmetic ( 2入力, 1出力 )
    kSla,
    /// @brief shift right arithmetic ( 2入力, 1出力 )
    kSra,

    /// @brief cmpl ( 1入力, 1出力 )
    /// 2の補数
    kCmpl,
    /// @brief add ( 2入力, 1出力 )
    kAdd,
    /// @brief sub ( 2入力, 1出力 )
    kSub,
    /// @brief multiply ( 2入力, 1出力 )
    kMult,
    /// @brief division ( 2入力, 1出力 )
    kDiv,
    /// @brief modulo ( 2入力, 1出力 )
    kMod,
    /// @brief power ( 2入力, 1出力 )
    kPow,

    /// @brief condition ( 3入力, 1出力 )
    kIte,

    /// @brief concatenate ( n入力, 1出力 )
    kConcat,

    /// @brief constant bit-select ( 1入力, 1出力 )
    kConstBitSelect,

    /// @brief constant part-select ( 1入力, 1出力 )
    kConstPartSelect,

    /// @brief bit-select ( 2入力, 1出力 )
    kBitSelect,

    /// @brief part-select ( 3入力, 1出力 )
    kPartSelect,

    /// @brief combinational UDP ( n入力, 1出力 )
    kCombUdp,

    /// @brief sequential UDP ( n入力, 1出力 )
    kSeqUdp,

    /// @brief constant ( 0入力, 1出力 )
    kConst

  };


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 親のモジュールを得る．
  const MvModule*
  parent() const;

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const = 0;

  /// @brief 入力ピン数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  const MvInputPin*
  input(ymuint pos) const;

  /// @brief 出力ピン数を得る．
  ymuint
  output_num() const;

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  const MvOutputPin*
  output(ymuint pos) const;

  /// @brief ビット位置を得る．
  /// @note type() が kConstBitSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  bitpos() const;

  /// @brief 範囲指定の MSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  lsb() const;

  /// @brief 定数値を得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kConst の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  const_value(vector<ymuint32>& val) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] ni 入力ピン数
  /// @param[in] no 出力ピン数
  MvNode(MvModule* parent,
	 ymuint ni,
	 ymuint no);

  /// @brief デストラクタ
  virtual
  ~MvNode();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  MvInputPin*
  _input(ymuint pos);

  /// @brief 出力ピンを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  MvOutputPin*
  _output(ymuint pos);

  /// @brief 入力ピンのビット幅を設定する．
  /// @param[in] pos 対象のピン番号
  /// @param[in] bit_width ビット幅
  void
  set_ipin_bit_width(ymuint pos,
		     ymuint bit_width);

  /// @brief 出力ピンのビット幅を設定する．
  /// @param[in] pos 対象のピン番号
  /// @param[in] bit_width ビット幅
  void
  set_opin_bit_width(ymuint pos,
		     ymuint bit_width);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 親のモジュール
  MvModule* mParent;

  // mNodeList からの削除で用いる反復子
  list<MvNode*>::iterator mSelfRef;

  // 入力ピン数
  ymuint32 mNi;

  // 入力ピンの配列
  MvInputPin* mInputPins;

  // 出力ピン数
  ymuint32 mNo;

  // 出力ピンの配列
  MvOutputPin* mOutputPins;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
MvNode::id() const
{
  return mId;
}

// @brief 親のモジュールを得る．
inline
const MvModule*
MvNode::parent() const
{
  return mParent;
}

// @brief 入力ピン数を得る．
inline
ymuint
MvNode::input_num() const
{
  return mNi;
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
const MvInputPin*
MvNode::input(ymuint pos) const
{
  return &mInputPins[pos];
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
MvInputPin*
MvNode::_input(ymuint pos)
{
  return &mInputPins[pos];
}

// @brief 出力ピン数を得る．
inline
ymuint
MvNode::output_num() const
{
  return mNo;
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
const MvOutputPin*
MvNode::output(ymuint pos) const
{
  return &mOutputPins[pos];
}

// @brief 出力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
MvOutputPin*
MvNode::_output(ymuint pos)
{
  return &mOutputPins[pos];
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNODE_H
