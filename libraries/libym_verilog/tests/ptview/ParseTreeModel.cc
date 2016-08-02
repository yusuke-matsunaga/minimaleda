
/// @file libym_verilog/tests/ptview/ParseTreeModel.cc
/// @brief ParseTreeModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ParseTreeModel.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "ParseTreeModel.h"
#include "PtNode.h"
#include "PtNode_root.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] parent 親のオブジェクト
ParseTreeModel::ParseTreeModel(QObject* parent) :
  QAbstractItemModel(parent),
  mRootNode(NULL)
{
}

// @brief デストラクタ
ParseTreeModel::~ParseTreeModel()
{
}
  
// @brief 対応するインデックスを返す．
// @param[in] row 行番号
// @param[in] column 列番号
// @param[in] parent 親のインデックス
QModelIndex
ParseTreeModel::index(int row,
		      int column,
		      const QModelIndex& parent) const
{
  PtNode* node = index2node(parent);
  if ( !node ) {
    return QModelIndex();
  }
  
  PtNode* cnode = node->child(row);
  cnode->mParentIndex = parent;
  return createIndex(row, column, cnode);
}

// @brief 親のインデックスを返す．
// @param[in] child 子のインデックス
QModelIndex
ParseTreeModel::parent(const QModelIndex& child) const
{
  PtNode* node = index2node(child);
  if ( node ) {
    return node->parent_index();
  }
  else {
    return QModelIndex();
  }
}
  
// @brief コラム数を返す．
int
ParseTreeModel::columnCount(const QModelIndex& parent) const
{
  // ラベル, 値 の2つ
  return 2;
}
  
// @brief 行数を返す．
int
ParseTreeModel::rowCount(const QModelIndex& parent) const
{
  PtNode* node = index2node(parent);
  if ( node ) {
    return node->child_num();
  }
  else {
    return 0;
  }
}
  
// @brief 該当するデータを返す．
QVariant
ParseTreeModel::data(const QModelIndex& index,
		     int role) const
{
  PtNode* node = index2node(index);
  if ( node ) {
    return node->data(index.column(), role);
  }
  else {
    return QVariant();
  }
}

/// @brief ヘッダのデータを返す．
QVariant
ParseTreeModel::headerData(int section,
			   Qt::Orientation orientation,
			   int role) const
{
  if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
    if ( section == 0 ) {
      return "Label";
    }
    else if ( section == 1 ) {
      return "Value";
    }
  }
  return QVariant();
}

// @brief 関連するパース木をセットする．
void
ParseTreeModel::set_pt(const VlMgr& vl_mgr)
{
  delete mRootNode;
  mRootNode = new RootNode(vl_mgr);
}
  
// @brief トークンのファイル上の位置を返す．
FileRegion
ParseTreeModel::loc(const QModelIndex& index)
{
  PtNode* node = index2node(index);
  if ( node ) {
    return node->loc();
  }
  else {
    return FileRegion();
  }
}

// @brief インデックスをノードに変換する．
PtNode*
ParseTreeModel::index2node(const QModelIndex& index) const
{
  if ( index.isValid() ) {
    return static_cast<PtNode*>(index.internalPointer());
  }
  else {
    return mRootNode;
  }
}

END_NAMESPACE_YM_VERILOG
