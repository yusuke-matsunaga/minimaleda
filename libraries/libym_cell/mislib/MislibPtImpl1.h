#ifndef LIBYM_CELL_MISLIB_MISLIBPTIMPL1_H
#define LIBYM_CELL_MISLIB_MISLIBPTIMPL1_H

/// @file libym_cell/mislib/MislibPtImpl1.h
/// @brief MislibPt の派生クラスのヘッダファイル(その1)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl1.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPt.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtStr :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] str 共有された文字列のID
  MislibPtStr(const FileRegion& loc,
	      ShString str);

  /// @brief デストラクタ
  ~MislibPtStr();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 文字列を取り出す
  virtual
  ShString
  str() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列
  ShString mStr;

};


//////////////////////////////////////////////////////////////////////
/// @brief 数値を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtNum :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] num 数値
  MislibPtNum(const FileRegion& loc,
	      double num);

  /// @brief デストラクタ
  ~MislibPtNum();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 数値を取り出す
  virtual
  double
  num() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 数値
  double mNum;

};


//////////////////////////////////////////////////////////////////////
/// @brief NONINV 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtNoninv :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibPtNoninv(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibPtNoninv();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @brief INV 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtInv :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibPtInv(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibPtInv();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @brief UNKNOWN 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtUnknown :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibPtUnknown(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibPtUnknown();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @brief 定数0を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtConst0 :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibPtConst0(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibPtConst0();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @brief 定数1を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtConst1 :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  MislibPtConst1(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~MislibPtConst1();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPtList MislibPtImpl1.h "MislibPtImpl1.h"
/// @brief MislibPtのリストを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtList :
  public MislibPt
{
  friend class MislibParser;
private:

  /// @brief コンストラクタ
  MislibPtList();

  /// @brief デストラクタ
  virtual
  ~MislibPtList();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 末尾に要素を追加する．
  virtual
  void
  push_back(MislibPt* pin);

  /// @brief 先頭の要素を取り出す．
  virtual
  MislibPt*
  top() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  MislibPt* mTop;

  // 末尾の要素
  MislibPt* mEnd;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBPTIMPL_H
