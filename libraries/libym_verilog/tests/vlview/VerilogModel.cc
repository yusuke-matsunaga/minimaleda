
/// @file libym_verilog/tests/vlview/VerilogModel.cc
/// @brief VerilogModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VerilogModel.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VerilogModel.h"
#include "VmNode.h"
#include "VmRootNode.h"
  

BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] parent 親のオブジェクト
VerilogModel::VerilogModel(QObject* parent) :
  QAbstractItemModel(parent),
  mRootNode(NULL)
{
}

// @brief デストラクタ
VerilogModel::~VerilogModel()
{
}
  
// @brief 対応するインデックスを返す．
// @param[in] row 行番号
// @param[in] column 列番号
// @param[in] parent 親のインデックス
QModelIndex
VerilogModel::index(int row,
		    int column,
		    const QModelIndex& parent) const
{
  VmNode* node = index2node(parent);
  if ( !node ) {
    return QModelIndex();
  }
  
  VmNode* cnode = node->child(row);
  cnode->mParentIndex = parent;
  return createIndex(row, column, cnode);
}

// @brief 親のインデックスを返す．
// @param[in] child 子のインデックス
QModelIndex
VerilogModel::parent(const QModelIndex& child) const
{
  VmNode* node = index2node(child);
  if ( node ) {
    return node->parent_index();
  }
  else {
    return QModelIndex();
  }
}
  
// @brief コラム数を返す．
int
VerilogModel::columnCount(const QModelIndex& /* parent */) const
{
  // ラベル, 値 の2つ
  return 2;
}
  
// @brief 行数を返す．
int
VerilogModel::rowCount(const QModelIndex& parent) const
{
  VmNode* node = index2node(parent);
  if ( node ) {
    return node->child_num();
  }
  else {
    return 0;
  }
}
  
// @brief 該当するデータを返す．
QVariant
VerilogModel::data(const QModelIndex& index,
		       int role) const
{
  VmNode* node = index2node(index);
  if ( node ) {
    return node->data(index.column(), role);
  }
  else {
    return QVariant();
  }
}

/// @brief ヘッダのデータを返す．
QVariant
VerilogModel::headerData(int section,
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

// @brief 関連するモデルをセットする．
void
VerilogModel::set_vlmgr(const VlMgr& vl_mgr)
{
  delete mRootNode;
  mRootNode = new VmRootNode(vl_mgr);
}

// @brief トークンのファイル上の位置を返す．
FileRegion
VerilogModel::loc(const QModelIndex& index)
{
  VmNode* node = index2node(index);
  if ( node ) {
    return node->loc();
  }
  else {
    return FileRegion();
  }
}

// @brief インデックスをノードに変換する．
VmNode*
VerilogModel::index2node(const QModelIndex& index) const
{
  if ( index.isValid() ) {
    return static_cast<VmNode*>(index.internalPointer());
  }
  else {
    return mRootNode;
  }
}

END_NAMESPACE_YM_VERILOG
