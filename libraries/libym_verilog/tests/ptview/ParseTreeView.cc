
/// @file libym_verilog/tests/ptview/ParseTreeView.cc
/// @brief ParseTreeView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ParseTreeView.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "ParseTreeView.h"
#include "ParseTreeModel.h"

  
// @brief コンストラクタ
// @param[in] parent 親のウィジェット
ParseTreeView::ParseTreeView(QWidget* parent) :
  QTreeView(parent)
{
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
}

// @brief デストラクタ
ParseTreeView::~ParseTreeView()
{
}

// @brief モデルをセットする．
void
ParseTreeView::setModel(nsYm::nsVerilog::ParseTreeModel* model)
{
  mModel = model;
  QTreeView::setModel(model);
  setColumnWidth(0, 300);
}

// @brief selection が変化したときに呼ばれる関数
void
ParseTreeView::selectionChanged(const QItemSelection& selected,
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
