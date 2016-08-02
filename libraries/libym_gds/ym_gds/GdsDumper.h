#ifndef YM_GDS_GDSDUMPER_H
#define YM_GDS_GDSDUMPER_H

/// @file ym_gds/GdsDumper.h
/// @brief GDS-II ファイルの内容をダンプするためのクラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GdsDumper.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_gds/gds_nsdef.h>
#include <ym_gds/GdsRecord.h>


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsDumper GdsDumper.h "ym_gds/GdsDumper.h"
/// @brief GDS-II ファイルの中身をダンプするためのクラス
//////////////////////////////////////////////////////////////////////
class GdsDumper
{
public:

  /// @brief コンストラクタ
  /// @param[in] os 出力先のストリーム
  explicit
  GdsDumper(ostream& os);
  
  /// @brief デストラクタ
  ~GdsDumper();

public:

  /// @brief record の内容を出力する．
  /// @param[in] record レコード
  void
  operator()(const GdsRecord& record);


private:

  // HEADER の出力
  void
  dump_HEADER(const GdsRecord& record);

  // BGNLIB の出力
  void
  dump_BGNLIB(const GdsRecord& record);

  // UNITS の出力
  void
  dump_UNITS(const GdsRecord& record);

  // BGNSTR の出力
  void
  dump_BGNSTR(const GdsRecord& record);

  // XY の出力
  void
  dump_XY(const GdsRecord& record);

  // COLROW の出力
  void
  dump_COLROW(const GdsRecord& record);

  // PRESENTATION の出力
  void
  dump_PRESENTATION(const GdsRecord& record);

  // STRANS の出力
  void
  dump_STRANS(const GdsRecord& record);

  // ELFLAGS の出力
  void
  dump_ELFLAGS(const GdsRecord& record);

  // LIBSECUR の出力
  void
  dump_LIBSECUR(const GdsRecord& record);

  // data type が 2 byte integer 一つの場合の出力
  void
  dump_2int(const GdsRecord& record);

  // data type が 4 byte integer 一つの場合の出力
  void
  dump_4int(const GdsRecord& record);

  // data type が 8 byte real 一つの場合の出力
  void
  dump_8real(const GdsRecord& record, bool s_form);

  // data type が ASCII String の場合の出力
  void
  dump_string(const GdsRecord& record);

  // 時刻のデータを出力する．
  void
  dump_date(int buf[]);

  // 2桁の整数を0つきで出力する．
  void
  dump_2digit(int num);

  // 1バイトのデータを出力する．
  void
  dump_byte(tGdsByte byte);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mOs;

};

END_NAMESPACE_YM_GDS

#endif
