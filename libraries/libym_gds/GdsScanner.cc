
/// @file libym_gds/GdsScanner.cc
/// @brief GDS-II の字句解析器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsScanner.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsScanner.h"
#include "ym_gds/GdsRecMgr.h"
#include "ym_gds/GdsRecord.h"
#include "ym_gds/Msg.h"
#include "GdsRecTable.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// GDS-II の読み込みを行うクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 入力ストリームを指定する．
GdsScanner::GdsScanner(std::istream& is,
		       GdsRecMgr& mgr) :
  mIs(is),
  mPos(0),
  mMgr(mgr)
{
}

// デストラクタ
GdsScanner::~GdsScanner()
{
}

// レコード一つ分の読み込みを行う．
// エラーが起った場合や末尾に達した場合には NULL を返す．
GdsRecord*
GdsScanner::read_rec()
{
  size_t size = 0;
  size_t offset = 0;
  while ( size == 0 ) {
    if ( mIs.eof() ) {
      return NULL;
    }
    offset = mPos;
    size = read_2byte_uint();
    // null word をスキップする．
  }

  if ( size < 4 || (size & 1) ) {
    // 変なサイズ
    error_header(__FILE__, __LINE__, "GdsScanner", offset)
      << "illegal size (" << size << ")";
    msg_end();
    return NULL;
  }

  size_t dsize = size - 4;
  tGdsRtype rtype = static_cast<tGdsRtype>(read_1byte_uint());
  tGdsDtype dtype = static_cast<tGdsDtype>(read_1byte_uint());

  // データの integrity check を行う．
  const GdsRecTable& table = GdsRecTable::obj();
  if ( table.dtype(rtype) != dtype ) {
    error_header(__FILE__, __LINE__, "GdsScanner", offset)
      << "data type mismatch: record type = "
      << table.rtype_string(rtype)
      << ", data type = " << table.dtype_string(dtype);
    msg_end();
    return NULL;
  }
  int unit_size = 1;
  switch ( dtype ) {
  case kGdsNodata:                   break;
  case kGdsBitArray: unit_size =  2; break;
  case kGds2Int:     unit_size =  2; break;
  case kGds4Int:     unit_size =  4; break;
  case kGds4Real:    unit_size =  4; break;
  case kGds8Real:    unit_size =  8; break;
  case kGdsString:   unit_size = -2; break;
  }
  int exp_dsize = unit_size * table.data_num(rtype);
  if ( exp_dsize >= 0 ) {
    if ( exp_dsize != static_cast<int>(dsize) ) {
      error_header(__FILE__, __LINE__, "GdsScanner", offset)
	<< "data size mismatch: record type = "
	<< table.rtype_string(rtype)
	<< " expected data size = "
	<< exp_dsize
	<< ", real data size = "
	<< dsize;
      msg_end();
      return NULL;
    }
  }
  else {
    // 可変長だが exp_dsize の絶対値の倍数でなければならない．
    int unit = -exp_dsize;
    if ( dsize % unit != 0 ) {
      error_header(__FILE__, __LINE__, "GdsScanner", offset)
	<< "data size mismatch: record type = "
	<< table.rtype_string(rtype)
	<< " expected data size = "
	<< unit
	<< " x N, real data size = "
	<< dsize;
      msg_end();
      return NULL;
    }
  }

  GdsRecord* rec = mMgr.alloc_rec(dsize);
  rec->mOffset = offset;
  rec->mSize = size;
  rec->mRtype = rtype;
  rec->mDtype = dtype;
  mIs.read(reinterpret_cast<char*>(rec->mData), dsize);
  mPos += dsize;

  if ( !mIs.good() ) {
    error_header(__FILE__, __LINE__, "GdsScanner", offset)
      << "unknown error occured during read";
    msg_end();
    delete rec;
    return NULL;
  }

  return rec;
}

// 現在の位置を返す．
size_t
GdsScanner::cur_pos() const
{
  return mPos;
}

// ストリームから1バイト読んで符号なし整数に変換する．
ymuint
GdsScanner::read_1byte_uint()
{
  char buf[1] = { 0 };
  mIs.read(buf, 1);
  mPos += 1;
  return static_cast<ymuint>(buf[0]);
}

// ストリームから2バイト読んで符号なし整数に変換する．
ymuint
GdsScanner::read_2byte_uint()
{
  char buf[2] = { 0, 0 };
  mIs.read(buf, 2);
  mPos += 2;
  ymuint ans = static_cast<ymuint>(buf[0]) << 8;
  ans += static_cast<ymuint>(buf[1]);
  return ans;
}

END_NAMESPACE_YM_GDS
