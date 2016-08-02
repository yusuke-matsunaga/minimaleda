#ifndef PARSETREEMODEL_H
#define PARSETREEMODEL_H

/// @file libym_verilog/tests/ptview/ParseTreeModel.h
/// @brief ParseTreeModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ParseTreeModel.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractItemModel>
#include <ym_verilog/verilog.h>
#include <ym_utils/FileRegion.h>


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;
class PtNode;

//////////////////////////////////////////////////////////////////////
/// @class ParseTreeModel ParseTreeModel.h
/// @brief verilog のパース木を表すモデル
//////////////////////////////////////////////////////////////////////
class ParseTreeModel :
  public QAbstractItemModel
{
public:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  ParseTreeModel(QObject* parent = NULL);

  /// @brief デストラクタ
  ~ParseTreeModel();
  

public:
  //////////////////////////////////////////////////////////////////////
  // QAbstractTableModel の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 対応するインデックスを返す．
  /// @param[in] row 行番号
  /// @param[in] column 列番号
  /// @param[in] parent 親のインデックス
  virtual
  QModelIndex
  index(int row,
	int column,
	const QModelIndex& parent) const;

  /// @brief 親のインデックスを返す．
  /// @param[in] child 子のインデックス
  virtual
  QModelIndex
  parent(const QModelIndex& child) const;
  
  /// @brief コラム数を返す．
  virtual
  int
  columnCount(const QModelIndex& parent = QModelIndex()) const;
  
  /// @brief 行数を返す．
  virtual
  int
  rowCount(const QModelIndex& parent = QModelIndex()) const;
  
  /// @brief 該当するデータを返す．
  virtual
  QVariant
  data(const QModelIndex& index,
       int role = Qt::DisplayRole) const;

  /// @brief ヘッダのデータを返す．
  virtual
  QVariant
  headerData(int section,
	     Qt::Orientation orientation,
	     int role) const;

  
public:

  /// @brief 関連するパース木をセットする．
  void
  set_pt(const VlMgr& vl_mgr);
  
  /// @brief トークンのファイル上の位置を返す．
  FileRegion
  loc(const QModelIndex& index);


private:

  /// @brief インデックスをノードに変換する．
  PtNode*
  index2node(const QModelIndex& index) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 根のノード
  PtNode* mRootNode;
  
};

END_NAMESPACE_YM_VERILOG

#endif // PARSETREEMODEL_H
