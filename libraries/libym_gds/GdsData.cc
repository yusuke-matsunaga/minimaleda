
/// @file libym_gds/GdsData.cc
/// @brief GDS-II の基本データ型
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsData.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "GdsData.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// GDS-II のヘッダ情報
//////////////////////////////////////////////////////////////////////

// コンストラクタ
GdsHeader::GdsHeader()
{
}

// デストラクタ
GdsHeader::~GdsHeader()
{
}

// バージョン番号を返す．
int
GdsHeader::version_no() const
{
  return mVersionNo;
}

// 最終更新日時を返す．
Date
GdsHeader::last_modification_time() const
{
  return mLastModificationTime;
}

// 最終アクセス日時を返す．
Date
GdsHeader::last_access_time() const
{
  return mLastAccessTime;
}

// LIBDIRSIZE を返す．
int
GdsHeader::lib_dir_size() const
{
  return mLibDirSize;
}

// SRFNAME(spacing rule file name) を返す．
string
GdsHeader::srf_name() const
{
  return mSrfName;
}

// ACL リストを返す．
const vector<ACL>&
GdsHeader::acl_list() const
{
  return mAclList;
}

// ライブラリ名を返す．
string
GdsHeader::lib_name() const
{
  return mLibName;
}

// 参照しているライブラリ名のリストを返す．
const vector<string>&
GdsHeader::reflib_names() const
{
  return mRefLibNames;
}

// フォント名のリストを返す．
const vector<string>&
GdsHeader::font_names() const
{
  return mFontNames;
}

// 属性定義ファイル名を返す．
string
GdsHeader::attr_table_name() const
{
  return mAttrTableName;
}

// 世代を返す．
int
GdsHeader::generations() const
{
  return mGenerations;
}

// フォーマット番号を返す．
int
GdsHeader::format() const
{
  return mFormat;
}

// マスク情報を返す．
const vector<string>&
GdsHeader::masks() const
{
  return mMasks;
}

// user unit を返す．
double
GdsHeader::user_unit() const
{
  return mUserUnit;
}

// unit in meters を返す．
double
GdsHeader::meter_unit() const
{
  return mMeterUnit;
}


//////////////////////////////////////////////////////////////////////
// GDS-II の構造単位
//////////////////////////////////////////////////////////////////////

// コンストラクタ
GdsStruct::GdsStruct()
{
}

// デストラクタ
GdsStruct::~GdsStruct()
{
}

// 生成日時を返す．
Date
GdsStruct::creation_time() const
{
  return mCreationTime;
}

// 最終更新日時を返す．
Date
GdsStruct::last_modification_time() const
{
  return mLastModificationTime;
}
 
// 名前を返す．
string
GdsStruct::name() const
{
  return mName;
}

// 要素のリストを返す．
const list<Element*>&
GdsStruct::element_list() const
{
  return mElementList;
}


//////////////////////////////////////////////////////////////////////
// 要素の基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Element::Element()
{
}

// デストラクタ
Element::~Element()
{
}

// external data ビットが立っているとき true を返す．
bool
Element::external_data() const
{
  return (mElFlags >> 14) & 1;
}

// template data ビットが立っているとき true を返す．
bool
Element::template_data() const
{
  return (mElFlags >> 15) & 1;
}

// plex 番号を返す．
int
Element::plex() const
{
  return mPlex;
}

// property のリストを返す．
const list<Property*>&
Element::property_list() const
{
  return mPropertyList;
}


//////////////////////////////////////////////////////////////////////
// BOUNDARY
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
Boundary::Boundary()
{
}

// デストラクタ
Boundary::~Boundary()
{
}

// 層番号を返す．
int
Boundary::layer() const
{
  return mLayer;
}

// データ型を返す．
int
Boundary::datatype() const
{
  return mDataType;
}

// 座標のリストを返す．
const vector<XY>&
Boundary::xy_list() const
{
  return mXYList;
}


//////////////////////////////////////////////////////////////////////
// PATH
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
Path::Path()
{
}

// デストラクタ
Path::~Path()
{
}

// 層番号を返す．
int
Path::layer() const
{
  return mLayer;
}

// データ型を返す．
int
Path::datatype() const
{
  return mDataType;
}

// パスタイプを返す．
int
Path::pathtype() const
{
  return mPathType;
}

// 幅を返す．
int
Path::width() const
{
  return mWidth;
}

// BGNEXTN を返す．
int
Path::bgn_extn() const
{
  return mBgnExtn;
}

// ENDEXTN を返す．
int
Path::end_extn() const
{
  return mEndExtn;
}

// 座標のリストを返す．
const vector<XY>&
Path::xy_list() const
{
  return mXYList;
}


//////////////////////////////////////////////////////////////////////
// 参照型要素の基底クラス
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
RefBase::RefBase()
{
}

// デストラクタ
RefBase::~RefBase()
{
}

// 参照している構造名を返す．
string
RefBase::strname()
{
  return mStrName;
}

// reflection ビットが立っていたら true を返す．
bool
RefBase::reflection() const
{
  return mStrans & 1;
}

// absolute magnification ビットが立っていたら true を返す．
bool
RefBase::absolute_magnification() const
{
  return (mStrans >> 13) & 1;
}

// absolute angle ビットが立っていたら true を返す．
bool
RefBase::absolute_angle() const
{
  return (mStrans >> 14) & 1;
}

// magnification factor を返す．
double
RefBase::mag() const
{
  return mMag;
}

// angular rotation factor を返す．
double
RefBase::angle() const
{
  return mAngle;
}


//////////////////////////////////////////////////////////////////////
// SREF
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Sref::Sref()
{
}

// デストラクタ
Sref::~Sref()
{
}

// XY 座標を返す．
XY
Sref::xy() const
{
  return mXY;
}


//////////////////////////////////////////////////////////////////////
// AREF
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Aref::Aref()
{
}

// デストラクタ
Aref::~Aref()
{
}

// column 数を返す．
int
Aref::column() const
{
  return mColumn;
}

// row 数を返す．
int
Aref::row() const
{
  return mRow;
}
  
// 基準(左下)の XY 座標を返す．
XY
Aref::xy1() const
{
  return mXY[0];
}

// 右下隅の XY 座標を返す．
XY
Aref::xy2() const
{
  return mXY[1];
}

// 左上隅の XY 座標を返す．
XY
Aref::xy3() const
{
  return mXY[2];
}


//////////////////////////////////////////////////////////////////////
// TEXT
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Text::Text()
{
}

// デストラクタ
Text::~Text()
{
}

// 層番号を返す．
int
Text::layer() const
{
  return mLayer;
}

// テキスト型を返す．
int
Text::texttype() const
{
  return mTextType;
}

// パスタイプを返す．
int
Text::pathtype() const
{
  return mPathType;
}

// 幅を返す．
int
Text::width() const
{
  return mWidth;
}

// reflection ビットが立っていたら true を返す．
bool
Text::reflection() const
{
  return mStrans & 1;
}

// absolute magnification ビットが立っていたら true を返す．
bool
Text::absolute_magnification() const
{
  return (mStrans >> 13) & 1;
}

// absolute angle ビットが立っていたら true を返す．
bool
Text::absolute_angle() const
{
  return (mStrans >> 14) & 1;
}

// magnification factor を返す．
double
Text::mag() const
{
  return mMag;
}

// angular rotation factor を返す．
double
Text::angle() const
{
  return mAngle;
}

// 座標を返す．
XY
Text::xy() const
{
  return mXY;
}

// 本体の文字列を返す．
string
Text::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// NODE
//////////////////////////////////////////////////////////////////////
  
// コンストラクタ
Node::Node()
{
}

// デストラクタ
Node::~Node()
{
}

// 層番号を返す．
int
Node::layer() const
{
  return mLayer;
}

// ノード型を返す．
int
Node::nodetype() const
{
  return mNodeType;
}

// 座標のリストを返す．
const vector<XY>&
Node::xy_list() const
{
  return mXYList;
}


//////////////////////////////////////////////////////////////////////
// BOX
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Box::Box()
{
}

// デストラクタ
Box::~Box()
{
}

// 層番号を返す．
int
Box::layer() const
{
  return mLayer;
}

// ボックス型を返す．
int
Box::boxtype() const
{
  return mBoxType;
}

// 座標を返す．
XY
Box::xy1() const
{
  return mXY[0];
}

XY
Box::xy2() const
{
  return mXY[1];
}

XY
Box::xy3() const
{
  return mXY[2];
}

XY
Box::xy4() const
{
  return mXY[3];
}


//////////////////////////////////////////////////////////////////////
// property
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Property::Property()
{
}

// デストラクタ
Property::~Property()
{
}

// 属性番号
int
Property::attr() const
{
  return mAttr;
}

// 属性値
string
Property::value() const
{
  return mValue;
}

END_NAMESPACE_YM_GDS
