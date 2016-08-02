#ifndef YM_BDN_BDNBLIFWRITER_H
#define YM_BDN_BDNBLIFWRITER_H

/// @file ym_bdn/BdnBlifWriter.h
/// @brief BdnBlifWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnBlifWriter.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <ym_bdn/bdn_nsdef.h>


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnBlifWriter BdnBlifWriter.h <ym_bdn/BdnBlifWriter.h>
/// @ingroup BnetGroup
/// @brief Bdnwork の内容を blif 形式で出力するクラス
/// @sa Bdnwork
//////////////////////////////////////////////////////////////////////
class BdnBlifWriter
{
public:

  /// @brief コンストラクタ
  BdnBlifWriter();

  /// @brief デストラクタ
  ~BdnBlifWriter();


public:

  /// @brief 出力する．
  void
  operator()(ostream& s,
	     const BdNetwork& network);

};

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNBLIFWRITER_H
