#ifndef GDSLIB_GDSDATA_H
#define GDSLIB_GDSDATA_H

/// @file libym_gds/GdsData.h
/// @brief GDS-II の基本データ型
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsData.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

class Element;
class Property;

//////////////////////////////////////////////////////////////////////
// 時刻を表すクラス
//////////////////////////////////////////////////////////////////////
struct Date
{
  int mYear;
  int mMonth;
  int mDay;
  int mHour;
  int mMin;
  int mSec;
};


//////////////////////////////////////////////////////////////////////
// ACL(Access Control List)
//////////////////////////////////////////////////////////////////////
struct ACL
{
  int mGroup;
  int mUser;
  int mAccess;
};


//////////////////////////////////////////////////////////////////////
// XY 座標
//////////////////////////////////////////////////////////////////////
struct XY
{
  double mX;
  double mY;
};


//////////////////////////////////////////////////////////////////////
// GDS-II のヘッダ情報
//////////////////////////////////////////////////////////////////////
class GdsHeader
{
public:

  // コンストラクタ
  GdsHeader();

  // デストラクタ
  ~GdsHeader();


public:

  // バージョン番号を返す．
  int
  version_no() const;

  // 最終更新日時を返す．
  Date
  last_modification_time() const;

  // 最終アクセス日時を返す．
  Date
  last_access_time() const;

  // LIBDIRSIZE を返す．
  int
  lib_dir_size() const;

  // SRFNAME(spacing rule file name) を返す．
  string
  srf_name() const;

  // ACL リストを返す．
  const vector<ACL>&
  acl_list() const;

  // ライブラリ名を返す．
  string
  lib_name() const;

  // 参照しているライブラリ名のリストを返す．
  const vector<string>&
  reflib_names() const;

  // フォント名のリストを返す．
  const vector<string>&
  font_names() const;

  // 属性定義ファイル名を返す．
  string
  attr_table_name() const;

  // 世代を返す．
  int
  generations() const;

  // フォーマット番号を返す．
  int
  format() const;

  // マスク情報を返す．
  const vector<string>&
  masks() const;

  // user unit を返す．
  double
  user_unit() const;

  // unit in meters を返す．
  double
  meter_unit() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // バージョン番号
  int mVersionNo;

  // 最終更新日時
  Date mLastModificationTime;

  // 最終アクセス日時
  Date mLastAccessTime;

  // LIBDIRSIZE
  int mLibDirSize;

  // SRFNAME(spacing rule file name)
  string mSrfName;

  // LIBSECRUE
  vector<ACL> mAclList;

  // ライブラリ名
  string mLibName;

  // reference libraries
  vector<string> mRefLibNames;

  // フォント名
  vector<string> mFontNames;

  // 属性定義ファイル名
  string mAttrTableName;

  // 世代
  int mGenerations;

  // フォーマット番号
  int mFormat;

  // マスク情報
  vector<string> mMasks;

  // user unit
  double mUserUnit;

  // unit in meters
  double mMeterUnit;

};


//////////////////////////////////////////////////////////////////////
// GDS-II の構造単位
//////////////////////////////////////////////////////////////////////
class GdsStruct
{
public:

  // コンストラクタ
  GdsStruct();

  // デストラクタ
  ~GdsStruct();


public:

  // 生成日時を返す．
  Date
  creation_time() const;

  // 最終更新日時を返す．
  Date
  last_modification_time() const;

  // 名前を返す．
  string
  name() const;

  // 要素のリストを返す．
  const list<Element*>&
  element_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 生成日時
  Date mCreationTime;

  // 最終更新日時
  Date mLastModificationTime;

  // 名前
  string mName;

  // 要素のリスト
  list<Element*> mElementList;

};


//////////////////////////////////////////////////////////////////////
// 要素の基底クラス
//////////////////////////////////////////////////////////////////////
class Element
{
public:

  // コンストラクタ
  Element();

  // デストラクタ
  virtual
  ~Element();


public:

  // external data ビットが立っているとき true を返す．
  bool
  external_data() const;

  // template data ビットが立っているとき true を返す．
  bool
  template_data() const;

  // plex 番号を返す．
  int
  plex() const;

  // property のリストを返す．
  const list<Property*>&
  property_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ELFLAGS
  size_t mElFlags;

  // PLEX
  int mPlex;

  // property のリスト
  list<Property*> mPropertyList;
  
};


//////////////////////////////////////////////////////////////////////
// BOUNDARY
//////////////////////////////////////////////////////////////////////
class Boundary :
  public Element
{
public:
  
  // コンストラクタ
  Boundary();

  // デストラクタ
  virtual
  ~Boundary();


public:

  // 層番号を返す．
  int
  layer() const;

  // データ型を返す．
  int
  datatype() const;

  // 座標のリストを返す．
  const vector<XY>&
  xy_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号
  int mLayer;

  // データ型
  int mDataType;

  // 座標のリスト
  vector<XY> mXYList;

};


//////////////////////////////////////////////////////////////////////
// PATH
//////////////////////////////////////////////////////////////////////
class Path :
  public Element
{
public:
  
  // コンストラクタ
  Path();

  // デストラクタ
  virtual
  ~Path();


public:

  // 層番号を返す．
  int
  layer() const;

  // データ型を返す．
  int
  datatype() const;

  // パスタイプを返す．
  int
  pathtype() const;

  // 幅を返す．
  int
  width() const;

  // BGNEXTN を返す．
  int
  bgn_extn() const;

  // ENDEXTN を返す．
  int
  end_extn() const;
  
  // 座標のリストを返す．
  const vector<XY>&
  xy_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号
  int mLayer;

  // データ型
  int mDataType;

  // パスタイプ
  int mPathType;

  // 幅
  int mWidth;

  // BGNEXTN
  int mBgnExtn;

  // ENDEXTN
  int mEndExtn;
  
  // 座標のリスト
  vector<XY> mXYList;

};


//////////////////////////////////////////////////////////////////////
// 参照型要素の基底クラス
//////////////////////////////////////////////////////////////////////
class RefBase :
  public Element
{
public:
  
  // コンストラクタ
  RefBase();

  // デストラクタ
  virtual
  ~RefBase();


public:

  // 参照している構造名を返す．
  string
  strname();

  // reflection ビットが立っていたら true を返す．
  bool
  reflection() const;

  // absolute magnification ビットが立っていたら true を返す．
  bool
  absolute_magnification() const;

  // absolute angle ビットが立っていたら true を返す．
  bool
  absolute_angle() const;

  // magnification factor を返す．
  double
  mag() const;

  // angular rotation factor を返す．
  double
  angle() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照している構造名
  string mStrName;

  // STRANS bit array
  size_t mStrans;

  // magnification factor
  double mMag;

  // angular rotation factor
  double mAngle;
  
};


//////////////////////////////////////////////////////////////////////
// SREF
//////////////////////////////////////////////////////////////////////
class Sref :
  public RefBase
{
public:

  // コンストラクタ
  Sref();

  // デストラクタ
  virtual
  ~Sref();


public:

  // XY 座標を返す．
  XY
  xy() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // XY座標
  XY mXY;

};


//////////////////////////////////////////////////////////////////////
// AREF
//////////////////////////////////////////////////////////////////////
class Aref :
  public RefBase
{
public:

  // コンストラクタ
  Aref();

  // デストラクタ
  virtual
  ~Aref();


public:

  // column 数を返す．
  int
  column() const;

  // row 数を返す．
  int
  row() const;
  
  // 基準(左下)の XY 座標を返す．
  XY
  xy1() const;

  // 右下隅の XY 座標を返す．
  XY
  xy2() const;

  // 左上隅の XY 座標を返す．
  XY
  xy3() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // column 数
  int mColumn;

  // row 数
  int mRow;
  
  // XY座標
  XY mXY[3];

};


//////////////////////////////////////////////////////////////////////
// TEXT
//////////////////////////////////////////////////////////////////////
class Text
{
public:

  // コンストラクタ
  Text();

  // デストラクタ
  virtual
  ~Text();


public:

  // 層番号を返す．
  int
  layer() const;

  // テキスト型を返す．
  int
  texttype() const;

  // パスタイプを返す．
  int
  pathtype() const;

  // 幅を返す．
  int
  width() const;

  // reflection ビットが立っていたら true を返す．
  bool
  reflection() const;

  // absolute magnification ビットが立っていたら true を返す．
  bool
  absolute_magnification() const;

  // absolute angle ビットが立っていたら true を返す．
  bool
  absolute_angle() const;

  // magnification factor を返す．
  double
  mag() const;

  // angular rotation factor を返す．
  double
  angle() const;

  // 座標を返す．
  XY
  xy() const;

  // 本体の文字列を返す．
  string
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号
  int mLayer;

  // TEXTTYPE
  int mTextType;

  // PRESENTATION
  size_t mPresentation;

  // パスタイプ
  int mPathType;

  // 幅
  int mWidth;

  // STRANS bit array
  size_t mStrans;

  // magnification factor
  double mMag;

  // angular rotation factor
  double mAngle;

  // XY座標
  XY mXY;

  // 本体の文字列
  string mBody;

};


//////////////////////////////////////////////////////////////////////
// NODE
//////////////////////////////////////////////////////////////////////
class Node :
  public Element
{
public:
  
  // コンストラクタ
  Node();

  // デストラクタ
  virtual
  ~Node();


public:

  // 層番号を返す．
  int
  layer() const;

  // ノード型を返す．
  int
  nodetype() const;

  // 座標のリストを返す．
  const vector<XY>&
  xy_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号
  int mLayer;

  // ノード型
  int mNodeType;

  // 座標のリスト
  vector<XY> mXYList;

};


//////////////////////////////////////////////////////////////////////
// BOX
//////////////////////////////////////////////////////////////////////
class Box :
  public Element
{
public:
  
  // コンストラクタ
  Box();

  // デストラクタ
  virtual
  ~Box();


public:

  // 層番号を返す．
  int
  layer() const;

  // ボックス型を返す．
  int
  boxtype() const;

  // 座標を返す．
  XY
  xy1() const;
  
  XY
  xy2() const;

  XY
  xy3() const;

  XY
  xy4() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 層番号
  int mLayer;

  // ボックス型
  int mBoxType;

  // 座標のリスト
  XY mXY[4];

};


//////////////////////////////////////////////////////////////////////
// property
//////////////////////////////////////////////////////////////////////
class Property
{
public:

  // コンストラクタ
  Property();

  // デストラクタ
  ~Property();


public:

  // 属性番号
  int
  attr() const;

  // 属性値
  string
  value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性番号
  int mAttr;

  // 属性値
  string mValue;

};

END_NAMESPACE_YM_GDS

#endif
