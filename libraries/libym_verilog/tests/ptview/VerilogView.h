#ifndef VERILOGVIEW_H
#define VERILOGVIEW_H

/// @file libym_verilog/tests/ptview/VerilogView.h
/// @brief VerilogView のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VerilogView.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QWidget>


class QTextEdit;
class QTextCursor;
class VerilogSyntaxHighlighter;

//////////////////////////////////////////////////////////////////////
/// @class VerilogView VerilogView.h
/// @brief verilog のソースファイルの内容を表示するウィジェット
/// @note 具体的にはただの TextEdit に行番号の対応を付けたもの
//////////////////////////////////////////////////////////////////////
class VerilogView :
  public QWidget
{
  Q_OBJECT
  
public:
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  VerilogView(QWidget* parent = NULL);

  /// @brief デストラクタ
  ~VerilogView();


public:

  /// @brief ファイルと結びつける．
  /// @param[in] file_name ファイル名
  /// @return 読み込みに成功したら true を返す．
  bool
  open(const char* file_name);
  
  /// @brief カーソルの位置を該当する箇所にセットする．
  /// @param[in] cursor 対象のカーソル
  /// @param[in] start_line 開始位置の行番号
  /// @param[in] start_column 開始位置のコラム番号
  /// @param[in] end_line 終了位置の行番号
  /// @param[in] end_column 終了位置のコラム番号
  void
  set_cursor_pos(QTextCursor& cursor,
		 int start_line,
		 int start_column,
		 int end_line,
		 int end_column);

  /// @brief カーソルの位置を該当する箇所にセットする．
  /// @param[in] cursor 対象のカーソル
  /// @param[in] start_line 開始位置の行番号
  /// @param[in] end_line 終了位置の行番号
  /// @note こちらは行の先頭から行の末尾までを領域とする．
  void
  set_cursor_pos(QTextCursor& cursor,
		 int start_line,
		 int end_line);
			      

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

  /// @brief 指定された領域を強調表示する．
  /// @param[in] start_line 開始位置の行番号
  /// @param[in] end_line 終了位置の行番号
  /// @note こちらは行の先頭から行の末尾までを領域とする．
  void
  hilight(int start_line,
	  int end_line);

  /// @brief 指定された行を強調表示する．
  /// @param[in] line 行番号
  void
  hilight(int line);


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

  /// @brief 行頭の絶対位置を取り出す．
  /// @param[in] line 行番号
  int
  start_pos(int line) const;

  /// @brief 行末の絶対位置を取り出す．
  /// @param[in] line 行番号
  int
  end_pos(int line) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容を表示するウィジェット
  QTextEdit* mTextEdit;
  
  // 各行の先頭の位置を記録する配列
  QList<int> mStartPos;

  // syntax highlighter
  VerilogSyntaxHighlighter* mHighlighter;
  
};

#endif // VERILOGVIEW_H
