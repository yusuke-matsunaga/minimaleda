#ifndef LIBYM_MISLIB_MISLIBPTIMPL1_H
#define LIBYM_MISLIB_MISLIBPTIMPL1_H

/// @file libym_mislib/MislibPtImpl1.h
/// @brief MislibPt の派生クラスのヘッダファイル(その1)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl1.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPt.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPtStr :
  public MislibPt
{
  friend class MislibParserImpl;
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
  friend class MislibParserImpl;
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
  friend class MislibParserImpl;
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
  friend class MislibParserImpl;
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
  friend class MislibParserImpl;
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
  friend class MislibParserImpl;
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
  friend class MislibParserImpl;
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

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBPTIMPL_H
