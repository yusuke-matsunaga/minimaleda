#ifndef VLPARSETREEVIEW_H
#define VLPARSETREEVIEW_H

/// @file libym_verilog/tests/vlview/VlParseTreeView.h
/// @brief VlParseTreeView のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlParseTreeView.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QTreeView>
#include "ym_verilog/verilog.h"
#include "VerilogModel.h"

class QItemSelection;

//////////////////////////////////////////////////////////////////////
/// @class VerilogView VerilogView.h
/// @brief verilog のパース木を表示するウィジェット
//////////////////////////////////////////////////////////////////////
class VerilogView :
  public QTreeView
{
  Q_OBJECT
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  VerilogView(QWidget* parent = NULL);

  /// @brief デストラクタ
  ~VerilogView();


public:

  /// @brief モデルをセットする．
  void
  setModel(nsYm::nsVerilog::VerilogModel* model);

  
signals:

  /// @brief トークンが選択されたときに発生するシグナル．
  /// @param[in] start_line 開始位置の行番号
  /// @param[in] start_column 開始位置のコラム番号
  /// @param[in] end_line 終了位置の行番号
  /// @param[in] end_column 終了位置のコラム番号
  void
  select_token(int start_line,
	       int start_column,
	       int end_line,
	       int end_column);
  
  
protected:

  /// @brief selection が変化したときに呼ばれる関数
  virtual
  void
  selectionChanged(const QItemSelection& selected,
		   const QItemSelection& deselected);

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 内容を表すモデル．
  nsYm::nsVerilog::VerilogModel* mModel;
  
};

#endif // VLPARSETREEVIEW_H
