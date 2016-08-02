#ifndef LIBYM_VERILOG_ELB_ELBPRIMITIVE_H
#define LIBYM_VERILOG_ELB_ELBPRIMITIVE_H

/// @file libym_verilog/elaborator/include/ElbPrimitive.h
/// @brief ElbPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbPrimitive.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlPrimitive.h"

#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbPrimHead ElbPrimitive.h "ElbPrimitive.h"
/// @brief primitive/primitive array のヘッダ情報
//////////////////////////////////////////////////////////////////////
class ElbPrimHead
{
protected:
  
  /// @brief コンストラクタ
  ElbPrimHead();
  
  /// @brief デストラクタ
  virtual
  ~ElbPrimHead();

  
public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const = 0;

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const = 0;

  /// @brief UDP 定義を返す．
  virtual
  const ElbUdpDefn*
  udp_defn() const = 0;

  /// @brief 0 の強さを得る．
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief 1 の強さを得る．
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief 遅延式を得る．
  virtual
  ElbDelay*
  delay() const = 0;

  /// @brief 遅延式を設定する．
  virtual
  void
  set_delay(ElbDelay* expr) = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimArray ElbPrimitive.h "ElbPrimitive.h"
/// @brief gate/UDP instance の配列を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class ElbPrimArray :
  public VlPrimArray
{
  friend class CellPrimArray;
  
protected:
  
  /// @brief コンストラクタ
  ElbPrimArray();
  
  /// @brief デストラクタ
  virtual
  ~ElbPrimArray();
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のプリミティブを取り出す．
  virtual
  ElbPrimitive*
  _primitive_by_offset(ymuint32 offset) const = 0;

  /// @brief 要素のプリミティブを取り出す．
  virtual
  ElbPrimitive*
  _primitive_by_index(int index) const = 0;
  
  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbPrimArray*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbPrimArray* mNext;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimitive ElbItem.h "ElbItem.h"
/// @brief gate/UDP instance を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbPrimitive :
  public VlPrimitive
{
  friend class CellPrimitive;

protected:
  
  /// @brief コンストラクタ
  ElbPrimitive();
  
  /// @brief デストラクタ
  virtual
  ~ElbPrimitive();

  
public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 接続する．
  /// @param[in] pos ポート番号 (0 から始まる)
  /// @param[in] expr 接続する式
  virtual
  void
  connect(ymuint32 pos,
	  ElbExpr* expr) = 0;
    
  /// @brief ヘッダを得る．
  virtual
  ElbPrimHead*
  head() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbPrimitive*
  next() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 便利関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 入出力ポート数を計算する．
  /// @param[in] type プリミティブの型
  /// @param[in] port_size 全ポート数
  /// @param[out] output_num 出力ポート数
  /// @param[out] inout_num 入出力ポート数
  /// @param[out] input_num 入力ポート数
  /// @retval 1 port_size が多すぎる．
  /// @retval 0 port_size が範囲内に収まっている．
  /// @retval -1 port_size が少なすぎる．
  static
  int
  get_port_size(tVpiPrimType type,
		ymuint32 port_size,
		ymuint32& output_num,
		ymuint32& inout_num,
		ymuint32& input_num);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbPrimitive* mNext;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ElbPrimTerm ElbPrimitive.h <ym_verilog/vl/VlPrimitive.h>
/// @brief プリミティブインスタンスのポート端子を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class ElbPrimTerm :
  public VlPrimTerm
{
protected:
  
  /// @brief コンストラクタ
  ElbPrimTerm() { }
  
  /// @brief デストラクタ
  virtual
  ~ElbPrimTerm() { }
  

public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  virtual
  void
  set(ElbPrimitive* primitive,
      ymuint32 index,
      tVpiDirection dir) = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbPrimHead::ElbPrimHead()
{
}

// @brief デストラクタ
inline
ElbPrimHead::~ElbPrimHead()
{
}

// @brief コンストラクタ
inline
ElbPrimArray::ElbPrimArray() :
  mNext(NULL)
{
}

// @brief デストラクタ
inline
ElbPrimArray::~ElbPrimArray()
{
}

// @brief 次の要素を得る．
inline
const ElbPrimArray*
ElbPrimArray::next() const
{
  return mNext;
}

// @brief コンストラクタ
inline
ElbPrimitive::ElbPrimitive() :
  mNext(NULL)
{
}

// @brief デストラクタ
inline
ElbPrimitive::~ElbPrimitive()
{
}

// @brief 次の要素を得る．
inline
const ElbPrimitive*
ElbPrimitive::next() const
{
  return mNext;
}


END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBPRIMITIVE_H
