#ifndef VLVIEW_H
#define VLVIEW_H

/// @file libym_verilog/tests/vlview/VlView.h
/// @brief VlView のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlView.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QWidget>


class QTextEdit;
class QTextCursor;

//////////////////////////////////////////////////////////////////////
/// @class VlView VlView.h
/// @brief verilog の解析結果を表示するウィジェット
//////////////////////////////////////////////////////////////////////
class VlView :
  public QWidget
{
  Q_OBJECT
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  VlView(QWidget* parent = NULL);

  /// @brief デストラクタ
  ~VlView();


public:

  /// @brief ファイルと結びつける．
  /// @param[in] file_name ファイル名
  /// @return 読み込みに成功したら true を返す．
  bool
  open(const char* file_name);
  

  /// @brief 該当する箇所を表すカーソルを返す．
  /// @param[in] start_line 開始位置の行番号
  /// @param[in] start_column 開始位置のコラム番号
  /// @param[in] end_line 終了位置の行番号
  /// @param[in] end_column 終了位置のコラム番号
  QTextCursor*
  cursor(int start_line,
	 int start_column,
	 int end_line,
	 int end_column);
  
  /// @brief 指定された領域を強調表示する．
  void
  hilight(QTextCursor* cursor);
  
			      
public slots:

  /// @brief 指定された領域を強調表示する．
  /// @param[in] start_line 開始位置の行番号
  /// @param[in] start_column 開始位置のコラム番号
  /// @param[in] end_line 終了位置の行番号
  /// @param[in] end_column 終了位置のコラム番号
  void
  hilight(int start_line,
	  int start_column,
	  int end_line,
	  int end_column);
  
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号とコラム番号から絶対位置を取り出す．
  /// @param[in] line 行番号
  /// @param[in] column コラム番号
  int
  xy2pos(int line,
	 int column) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容を表示するウィジェット
  QTextEdit* mTextEdit;
  
  // 各行の先頭の位置を記録する配列
  std::vector<int> mStartPos;
  
};

#endif // VLVIEW_H
