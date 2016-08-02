#ifndef VERILOGSYNTAXHIGHLIGTHER_H
#define VERILOGSYNTAXHIGHLIGTHER_H

/// @file libym_verilog/tests/ptview/VerilogSyntaxHighlighter.h
/// @brief VerilogSyntaxHighlighter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <QSyntaxHighlighter>


class QTextEdit;

//////////////////////////////////////////////////////////////////////
/// @class VerilogSyntaxHighlighter VerilogSyntaxHighlighter.h
/// @brief Verilog のシンタックスにしたがって色分けを行うクラス
//////////////////////////////////////////////////////////////////////
class VerilogSyntaxHighlighter :
  public QSyntaxHighlighter
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] parent 親の TextEdit
  VerilogSyntaxHighlighter(QTextEdit* parent = NULL);

  /// @brief デストラクタ
  ~VerilogSyntaxHighlighter();


protected:
  //////////////////////////////////////////////////////////////////////
  // QSyntaxHighlighter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 色分けを行う．
  virtual
  void
  highlightBlock(const QString& text);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる関数
  //////////////////////////////////////////////////////////////////////

  // フォーマットをセットする．
  void
  set_format();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // フォント
  QFont mFont;

  // 通常の色
  QColor mNormalColor;

  // 識別子の色
  QColor mIdentifierColor;

  // 予約語の色
  QColor mKeywordColor;

  // 文字列の色
  QColor mStringColor;

  // 定数の色
  QColor mConstantColor;

  // コメントの色
  QColor mCommentColor;

  // コンパイラディレクティブの色
  QColor mDirectiveColor;

  // エラーの色
  QColor mErrorColor;
  
  // 通常の文字用のフォーマット
  QTextCharFormat mNormalFormat;
  
  // 識別子用のフォーマット
  QTextCharFormat mIdentifierFormat;

  // 予約語用のフォーマット
  QTextCharFormat mKeywordFormat;
  
  // 文字列用のフォーマット
  QTextCharFormat mStringFormat;

  // 定数用のフォーマット
  QTextCharFormat mConstantFormat;

  // コメント用のフォーマット
  QTextCharFormat mCommentFormat;

  // コンパイラディレクティブ用のフォーマット
  QTextCharFormat mDirectiveFormat;
  
  // エラー用のフォーマット
  QTextCharFormat mErrorFormat;

  // 予約語の検査用ハッシュ表
  QHash<QString, int> mKeywordHash;
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる定数値
  //////////////////////////////////////////////////////////////////////

  /// @brief 通常モード
  static
  const int kNormal  = 0;

  /// @brief UDP モード
  static
  const int kUdp     = 1;

  /// @brief 2進定数モード
  static
  const int kBin     = 2;

  /// @brief 8進定数モード
  static
  const int kOct     = 3;

  /// @brief 10進定数モード
  static
  const int kDec     = 4;

  /// @brief 16進定数モード
  static
  const int kHex     = 5;

  /// @brief Cスタイルコメントモード
  static
  const int kComment = 6;

};

#endif // VERILOGSYNTAXHIGHLIGHTER_H
