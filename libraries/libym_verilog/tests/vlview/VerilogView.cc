
/// @file libym_verilog/tests/vlview/VerilogView.cc
/// @brief VerilogView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlParseTreeView.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VerilogView.h"


// @brief コンストラクタ
// @param[in] parent 親のウィジェット
VerilogView::VerilogView(QWidget* parent) :
  QTreeView(parent)
{
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
}

// @brief デストラクタ
VerilogView::~VerilogView()
{
}

// @brief モデルをセットする．
void
VerilogView::setModel(nsYm::nsVerilog::VerilogModel* model)
{
  mModel = model;
  QTreeView::setModel(model);
  setColumnWidth(0, 300);
}

// @brief selection が変化したときに呼ばれる関数
void
VerilogView::selectionChanged(const QItemSelection& selected,
			      const QItemSelection& deselected)
{
  QAbstractItemView::selectionChanged(selected, deselected);
  
  QList<QModelIndex> index_list = selected.indexes();
  QListIterator<QModelIndex> p(index_list);
  if ( p.hasNext() ) {
    const QModelIndex& index = p.next();
    nsYm::FileRegion file_region = mModel->loc(index);
    if ( file_region.is_valid() ) {
      emit select_token(file_region.start_line(),
			file_region.start_column(),
			file_region.end_line(),
			file_region.end_column());
    }
  }
}
