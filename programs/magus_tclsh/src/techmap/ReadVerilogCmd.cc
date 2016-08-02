
/// @file magus/techmap/ReadVerilogCmd.cc
/// @brief ReadVerilogCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ReadVerilogCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ReadVerilogCmd.h"
#include "ym_sbj/sbj_nsdef.h"
#include "ym_mvn/Mvn2Sbj.h"
#include "ym_mvn/MvNodeMap.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvVerilogReader.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
// BNetwork から SbjGraph に変換するコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadVerilogCmd::ReadVerilogCmd(MagMgr* mgr,
			       TechmapData* data) :
  TechmapCmd(mgr, data)
{
}

// @brief デストラクタ
ReadVerilogCmd::~ReadVerilogCmd()
{
}

// @brief コマンドを実行する仮想関数
int
ReadVerilogCmd::cmd_proc(TclObjVector& objv)
{
  try {
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
    MvMgr mgr;
    vector<pair<const VlDecl*, ymuint> > node_map;
    bool stat = reader.gen_network(mgr, node_map);
    if ( !stat ) {
      TclObj emsg;
      emsg << "Error converting mvn network";
      set_result(emsg);
      return TCL_ERROR;
    }

    // SbjGraph に変換
    Mvn2Sbj conv;
    MvNodeMap mvnode_map(mgr.max_node_id());
    conv(mgr, sbjgraph(), mvnode_map);

    return TCL_OK;
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS_TECHMAP
