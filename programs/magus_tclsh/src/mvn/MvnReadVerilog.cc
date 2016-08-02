
/// @file magus/lutmap/MvnReadVerilog.cc
/// @brief MvnReadVerilog の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MvnReadVerilog.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnReadVerilog.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvVerilogReader.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// verilog から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnReadVerilog::MvnReadVerilog(MagMgr* mgr) :
  MvnCmd(mgr)
{
}

// @brief デストラクタ
MvnReadVerilog::~MvnReadVerilog()
{
}

// @brief コマンドを実行する仮想関数
int
MvnReadVerilog::cmd_proc(TclObjVector& objv)
{
  MvVerilogReader reader;
  MsgHandler* mh = new StreamMsgHandler(&cerr);
  mh->set_mask(MsgHandler::kMaskAll);
  mh->delete_mask(kMsgInfo);
  mh->delete_mask(kMsgDebug);
  reader.add_msg_handler(mh);

  reader.set_ffname("KTECH_DFF", // セル名
		    "D",         // データ入力
		    "CK",        // クロック
		    "Q",         // ノーマル出力
		    "QN",        // 反転出力
		    "",          // セット
		    "");         // リセット

  // Verilog ファイルの読み込み
  for (ymuint i = 1; i < objv.size(); ++ i) {
    TclObj obj = objv[i];
    string filename = obj;
    bool stat = reader.read(filename);
    if ( !stat ) {
      TclObj emsg;
      emsg << "Error reading " << filename;
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  // MvNetwork に変換
  NetHandle* neth = cur_nethandle();
  if ( neth->type() != NetHandle::kMagMvn ) {
    // 今は手抜き
    TclObj emsg;
    emsg << "Error: current network is not Mvn type.";
    set_result(emsg);
    return TCL_ERROR;
  }

  vector<pair<const VlDecl*, ymuint> > node_map;
  bool stat = reader.gen_network(*neth->_mvn(), node_map);
  if ( !stat ) {
    TclObj emsg;
    emsg << "Error converting mvn network";
    set_result(emsg);
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
