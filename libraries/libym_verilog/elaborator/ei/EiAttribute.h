#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIATTRIBUTE_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIATTRIBUTE_H

/// @file libym_verilog/elaborator/ei/EiAttribute.h
/// @brief EiAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiAttribute.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbAttribute.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiAttribute EiAttribute.h "EiAttribute.h"
/// @brief genvar を表すクラス
//////////////////////////////////////////////////////////////////////
class EiAttribute :
  public ElbAttribute
{
  friend class EiFactory;
  friend class EiAttrList;

private:

  /// @brief コンストラクタ
  EiAttribute();

  /// @brief デストラクタ
  virtual
  ~EiAttribute();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlAttribute の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性名を返す．
  virtual
  const char*
  name() const;

  /// @brief def_attribute なら true を返す．
  virtual
  bool
  def_attribute() const;

  /// @brief 値を表す式を返す．
  virtual
  VlExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtAttrSpec* mPtAttrSpec;

  // 値
  ElbExpr* mExpr;

  // 定義側の属性の時 true をなるフラグ
  bool mDef;

};


//////////////////////////////////////////////////////////////////////
/// @class EiAttrList EiAttribute.h "EiAttribute.h"
/// @brief ElbAttrList の実装クラス
//////////////////////////////////////////////////////////////////////
class EiAttrList :
  public ElbAttrList
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  EiAttrList(ymuint32 n,
	     EiAttribute* array);

  /// @brief デストラクタ
  ~EiAttrList();


public:
  //////////////////////////////////////////////////////////////////////
  // VlAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  virtual
  ymuint32
  size() const;

  /// @brief 内容を返す．
  /// @param[in] pos 位置番号 (0 <= pos < size() )
  virtual
  VlAttribute*
  elem(ymuint32 pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を設定する．
  /// @param[in] pos 位置番号
  /// @param[in] pt_as パース木の定義要素
  /// @param[in] expr 値を表す式
  /// @param[in] def 定義側の属性のとき true とするフラグ
  virtual
  void
  set(ymuint32 pos,
      const PtAttrSpec* pt_as,
      ElbExpr* expr,
      bool def);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mSize;

  // 要素の配列
  EiAttribute* mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIATTRIBUTE_H
