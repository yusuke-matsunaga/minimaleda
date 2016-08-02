#ifndef TOKENVIEW_H
#define TOKENVIEW_H

/// @file libym_verilog/tests/ptview/TokenView.h
/// @brief TokenView のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenView.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QTableView>
#include <ym_utils/FileRegion.h>


class TokenListModel;
class QItemSelection;

//////////////////////////////////////////////////////////////////////
/// @class TokenView TokenView.h
/// @brief verilog のトークンを表示するウィジェット
//////////////////////////////////////////////////////////////////////
class TokenView :
  public QTableView
{
  Q_OBJECT
  
  typedef nsYm::FileRegion FileRegion;
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  TokenView(QWidget* parent = NULL);

  /// @brief デストラクタ
  ~TokenView();


public:

  /// @brief モデルをセットする．
  void
  setModel(TokenListModel* model);

  
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
  TokenListModel* mModel;
  
};

#endif // TOKENVIEW_H
