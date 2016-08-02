
/// @file libym_verilog/tests/ptview/TokenView.cc
/// @brief TokenView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenView.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "TokenView.h"
#include <QTableView>
#include "TokenListModel.h"

  
// @brief コンストラクタ
// @param[in] parent 親のウィジェット
TokenView::TokenView(QWidget* parent) :
  QTableView(parent)
{
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
}

// @brief デストラクタ
TokenView::~TokenView()
{
}

// @brief モデルをセットする．
void
TokenView::setModel(TokenListModel* model)
{
  mModel = model;
  QTableView::setModel(model);
}

// @brief selection が変化したときに呼ばれる関数
void
TokenView::selectionChanged(const QItemSelection& selected,
			    const QItemSelection& deselected)
{
  QAbstractItemView::selectionChanged(selected, deselected);
  
  QList<QModelIndex> index_list = selected.indexes();
  QListIterator<QModelIndex> p(index_list);
  if ( p.hasNext() ) {
    const QModelIndex& index = p.next();
    int row = index.row();
    const FileRegion& file_region = mModel->loc(row);
    emit select_token(file_region.start_line(),
		      file_region.start_column(),
		      file_region.end_line(),
		      file_region.end_column());
  }
}
