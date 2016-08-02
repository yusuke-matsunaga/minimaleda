
/// @file libym_verilog/tests/ptview/vlview_main.cc
/// @brief vlview の main ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: vlview_main.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <QApplication>
#include <QSplitter>
#include "VerilogView.h"
#include "TokenView.h"
#include "TokenListModel.h"
#include "ParseTreeModel.h"
#include "ParseTreeView.h"
#include "GoToLine.h"

#include "Lex.h"
#include "ym_verilog/VlMgr.h"



int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  using namespace nsYm::nsVerilog;
  
  QApplication app(argc, argv);
  
  MsgMgr msg_mgr;
  FileDescMgr fd_mgr;
  Lex lex(msg_mgr, fd_mgr);
  VlMgr vl_mgr(msg_mgr);

  // サーチパスリスト
  // 今は未設定
  const char* spath = NULL;
  SearchPathList splist;
  if ( spath ) {
    splist = SearchPathList(spath);
  }

  // トークンリストを作る．
  lex.set_searchpath(splist);
  if ( !lex.open_file(argv[1]) ) {
    cerr << argv[1] << " : could not open" << endl;
    return 1;
  }

  // トークンを読み出して token_model にセットする．
  TokenListModel* token_model = new TokenListModel;
  for ( ; ; ) {
    int id = lex.get_token();
    if ( id == EOF ) {
      break;
    }
    token_model->add_token(id, lex.cur_token_loc(), lex.cur_string());
  }

  // パース木を作る．
  if ( !vl_mgr.read_file(argv[1], splist) ) {
    return 1;
  }
  ParseTreeModel* pt_model = new ParseTreeModel;
  pt_model->set_pt(vl_mgr);

  // 表示用ウィジェットを作る．
  VerilogView* vlview = new VerilogView;
  TokenView* token_view = new TokenView;
  ParseTreeView* pt_view = new ParseTreeView;
  
  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(pt_view);
  splitter->addWidget(vlview);
  splitter->addWidget(token_view);
  splitter->setStretchFactor(0, 3);
  splitter->setStretchFactor(1, 4);
  splitter->setStretchFactor(2, 2);
  splitter->resize(1024, 760);
  
  if ( !vlview->open(argv[1]) ) {
    return 2;
  }
  token_view->setModel(token_model);
  pt_view->setModel(pt_model);

  QObject::connect(token_view, SIGNAL(select_token(int, int, int, int)),
		   vlview, SLOT(hilight(int, int, int, int)));

  QObject::connect(pt_view, SIGNAL(select_token(int, int, int, int)),
		   vlview, SLOT(hilight(int, int, int, int)));

  splitter->show();

  GoToLine* goto_line = new GoToLine;
  QObject::connect(goto_line, SIGNAL(on_goto(int)),
		   vlview, SLOT(hilight(int)));

  goto_line->set_minimum(1);
  goto_line->set_maximum(100000);
  
  goto_line->show();
  
  return app.exec();
}
