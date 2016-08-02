#ifndef YM_MVN_MVN_NSDEF_H
#define YM_MVN_MVN_NSDEF_H

/// @file ym_mvn/mvn_nsdef.h
/// @brief mvn の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/DlList.h"


/// @brief mvn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MVN			\
  BEGIN_NAMESPACE_YM				\
  BEGIN_NAMESPACE(nsMvn)

/// @brief mvn 用の名前空間の終了
#define END_NAMESPACE_YM_MVN			\
  END_NAMESPACE(nsMvn)				\
  END_NAMESPACE_YM

/// @brief verilog reader 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MVN_VERILOG		\
  BEGIN_NAMESPACE_YM_MVN			\
  BEGIN_NAMESPACE(nsVerilog)

/// @brief verilog reader 用の名前空間の終了
#define END_NAMESPACE_YM_MVN_VERILOG		\
  END_NAMESPACE(nsVerilog)			\
  END_NAMESPACE_YM_MVN


BEGIN_NAMESPACE_YM_MVN

// クラス名の先行宣言

class MvMgr;
class MvVerilogReader;
class MvModule;
class MvPort;
class MvPortRef;
class MvNode;
class MvInputPin;
class MvOutputPin;
class MvNet;

typedef DlList<MvInputPin> MvInputPinList;

class Mvn2Sbj;
class MvNodeMap;

END_NAMESPACE_YM_MVN

BEGIN_NAMESPACE_YM

using nsMvn::MvMgr;
using nsMvn::MvVerilogReader;
using nsMvn::MvModule;
using nsMvn::MvPort;
using nsMvn::MvPortRef;
using nsMvn::MvNode;
using nsMvn::MvInputPin;
using nsMvn::MvInputPinList;
using nsMvn::MvOutputPin;
using nsMvn::MvNet;

using nsMvn::Mvn2Sbj;
using nsMvn::MvNodeMap;

END_NAMESPACE_YM

#endif // YM_MVN_MVN_NSDEF_H
