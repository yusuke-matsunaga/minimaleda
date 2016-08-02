
/// @file libym_gds/GdsDumper.cc
/// @brief GDS-II の内容を出力するためのクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsDumper.cc 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"
#include "ym_gds/GdsDumper.h"
#include "ym_gds/GdsRecord.h"
#include "GdsRecTable.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// GDS-II ファイルの中身をダンプするためのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
GdsDumper::GdsDumper(ostream& os) :
  mOs(os)
{
}
  
// デストラクタ
GdsDumper::~GdsDumper()
{
}

// record の内容を出力する．
void
GdsDumper::operator()(const GdsRecord& record)
{
  // 生データの出力
  mOs << endl
      << hex << setw(8) << record.offset() << dec << ": ";
  size_t us = record.size() >> 8;
  size_t ls = record.size() & 255;
  dump_byte(us);
  dump_byte(ls);
  mOs << " ";
  size_t rt = static_cast<tGdsByte>(record.rtype());
  size_t dt = static_cast<tGdsByte>(record.dtype());
  dump_byte(rt);
  dump_byte(dt);
  mOs << " ";
  size_t dsize = record.dsize();
  for (size_t i = 0; i < dsize; ++ i) {
    dump_byte(record.conv_1byte(i));
    if ( (i + 4) % 24 == 23 ) {
      mOs << endl << "          ";
    }
    else if ( i % 2 == 1 ) {
      mOs << " ";
    }
  }
  mOs << endl;

  const GdsRecTable& table = GdsRecTable::obj();

  // 整形された出力
  mOs << "  " << record.rtype_string();
  switch ( record.rtype() ) {
  case kGdsHEADER:       dump_HEADER(record);       return;
  case kGdsBGNLIB:       dump_BGNLIB(record);       return;
  case kGdsUNITS:        dump_UNITS(record);        return;
  case kGdsBGNSTR:       dump_BGNSTR(record);       return;
  case kGdsXY:           dump_XY(record);           return;
  case kGdsCOLROW:       dump_COLROW(record);       return;
  case kGdsPRESENTATION: dump_PRESENTATION(record); return;
  case kGdsSTRANS:       dump_STRANS(record);       return;
  case kGdsELFLAGS:      dump_ELFLAGS(record);      return;
  case kGdsLIBSECUR:     dump_LIBSECUR(record);     return;
  default:
    break;
  }
  if ( record.dtype() == kGdsNodata ) {
    mOs << endl;
    return;
  }

  if ( table.data_num(record.rtype()) == 1 ) {
    switch ( record.dtype() ) {
    case kGds2Int:   dump_2int(record);         return;
    case kGds4Int:   dump_4int(record);         return;
    case kGds8Real:  dump_8real(record, false); return;
    case kGdsString: dump_string(record);       return;
    default:
      break;
    }
  }

  mOs << "Error: no handler for this record type" << endl;
}

// HEADER の出力
void
GdsDumper::dump_HEADER(const GdsRecord& record)
{
  int version = record.conv_2byte_int(0);
  mOs << "  Release " << version << endl;
}

// BGNLIB の出力
void
GdsDumper::dump_BGNLIB(const GdsRecord& record)
{
  int buf[12];
  for (size_t i = 0; i < 12; ++ i) {
    buf[i] = record.conv_2byte_int(i);
  }

  mOs << endl
      << "    Last modified ";
  dump_date(buf);
  mOs << endl
      << "    Last accessed ";
  dump_date(buf + 6);
  mOs << endl;
}

// UNITS の出力
void
GdsDumper::dump_UNITS(const GdsRecord& record)
{
  double uu = record.conv_8byte_real(0);
  double mu = record.conv_8byte_real(1);
  
  mOs << endl << scientific
      << "    1 database unit = " << uu << " user units" << endl
      << "    1 database unit = " << mu << " meters" << endl;
}

// BGNSTR の出力
void
GdsDumper::dump_BGNSTR(const GdsRecord& record)
{
  int buf[12];
  for (size_t i = 0; i < 12; ++ i) {
    buf[i] = record.conv_2byte_int(i);
  }
  mOs << endl
      << "    Creation time ";
  dump_date(buf);
  mOs << endl
      << "    Last modified ";
  dump_date(buf + 6);
  mOs << endl;
}

// XY の出力
void
GdsDumper::dump_XY(const GdsRecord& record)
{
  mOs << endl;
  size_t n = record.dsize() / 8;
  for (size_t i = 0; i < n; ++ i) {
    int x = record.conv_4byte_int(i * 2);
    int y = record.conv_4byte_int(i * 2 + 1);
    mOs << "  (" << x << ", " << y << ")"
	<< endl;
  }
}

// COLROW の出力
void
GdsDumper::dump_COLROW(const GdsRecord& record)
{
  int col = record.conv_2byte_int(0);
  int row = record.conv_2byte_int(1);
  mOs << " " << col << " cols, " << row << " rows"
      << endl;
}

// PRESENTATION の出力
void
GdsDumper::dump_PRESENTATION(const GdsRecord& record)
{
  size_t data = static_cast<size_t>(record.conv_2byte_int(0));

  // font type
  int font = 0;
  switch ( (data >> 10) & 3 ) {
  case 0: font = 0; break;
  case 1: font = 1; break;
  case 2: font = 2; break;
  case 3: font = 3; break;
  }

  // vertical justification
  const char* vj = "";
  switch ( (data >> 12) & 3 ) {
  case 0: vj = "top"; break;
  case 1: vj = "middle"; break;
  case 2: vj = "bottom"; break;
  }

  // horizontal justification
  const char* hj = "";
  switch ( (data >> 14) & 3 ) {
  case 0: hj = "left"; break;
  case 1: hj = "middle"; break;
  case 2: hj = "right"; break;
  }

  mOs << "  font " << font << "  vert: " << vj << "  horiz: " << hj
      << endl;
}

// STRANS の出力
void
GdsDumper::dump_STRANS(const GdsRecord& record)
{
  size_t data = static_cast<size_t>(record.conv_2byte_int(0));

  // reflection
  const char* ref = "no";
  if ( data & 1 ) {
    ref = "yes";
  }

  // absolute magnification
  const char* am = "no";
  if ( data & (1 << 13) ) {
    am = "yes";
  }

  // absolute angle
  const char* aa = "no";
  if ( data & (1 << 14) ) {
    aa = "yes";
  }

  mOs << "  reflection: " << ref << "  absolute magnification: "
      << am << "  absolute angle: " << aa
      << endl;
}

// ELFLAGS の出力
void
GdsDumper::dump_ELFLAGS(const GdsRecord& record)
{
  size_t data = record.conv_2byte_int(0);

  // Template data
  const char* td = "no";
  if ( data & (1 << 15) ) {
    td = "yes";
  }

  // External data
  const char* ed = "no";
  if ( data & (1 << 14) ) {
    ed = "yes";
  }

  mOs << "  template data: " << td << "  external data: " << ed
      << endl;
}

// LIBSECUR の出力
void
GdsDumper::dump_LIBSECUR(const GdsRecord& record)
{
  // 実は良く分かっていない．
  mOs << endl;
  size_t n = record.dsize() / 6;
  for (size_t i = 0; i < n; ++ i) {
    int g = record.conv_2byte_int(i * 3);
    int u = record.conv_2byte_int(i * 3 + 1);
    int a = record.conv_2byte_int(i * 3 + 2);
    mOs << "    group: " << g << "  user: " << u << "  access: "
	<< oct << a << dec << endl;
  }
}

// data type が 2 byte integer 一つの場合の出力
void
GdsDumper::dump_2int(const GdsRecord& record)
{
  mOs << " " << record.conv_2byte_int(0)
      << endl;
}

// data type が 4 byte integer 一つの場合の出力
void
GdsDumper::dump_4int(const GdsRecord& record)
{
  mOs << " " << record.conv_4byte_int(0)
      << endl;
}

// data type が 8 byte real 一つの場合の出力
void
GdsDumper::dump_8real(const GdsRecord& record, bool s_form)
{
  mOs << " ";
  if ( s_form ) {
    mOs << scientific;
  }
  else {
    mOs << fixed;
  }
  mOs << record.conv_8byte_real(0)
      << endl;
}

// data type が ASCII String の場合の出力
void
GdsDumper::dump_string(const GdsRecord& record)
{
  mOs << " " << record.conv_string()
      << endl;
}

// 時刻のデータを出力する．
void
GdsDumper::dump_date(int buf[])
{
  dump_2digit(buf[1]);  // month;
  mOs << "/";
  dump_2digit(buf[2]);  // day;
  mOs << "/"
     << buf[0] + 1900   // year
     << " ";
  dump_2digit(buf[3]);  // hour
  mOs << ":";
  dump_2digit(buf[4]);  // minute
  mOs << ":";
  dump_2digit(buf[5]);  // second
}

// 2桁の整数を0つきで出力する．
void
GdsDumper::dump_2digit(int num)
{
  int u = num / 10;
  int l = num - u * 10;
  mOs << u << l;
}

// 1バイトのデータを出力する．
void
GdsDumper::dump_byte(tGdsByte byte)
{
  int hb[2];
  hb[0] = byte >> 4;
  hb[1] = byte & 15;
  for (size_t i = 0; i < 2; ++ i) {
    int x = hb[i];
    if ( x >= 10 ) {
      mOs << static_cast<char>('a' + x - 10);
    }
    else {
      mOs << static_cast<char>('0' + x);
    }
  }
}

END_NAMESPACE_YM_GDS
