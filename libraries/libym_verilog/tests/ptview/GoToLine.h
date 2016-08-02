#ifndef GOTOLINE_H
#define GOTOLINE_H

/// @file libym_verilog/tests/ptview/GoToLine.h
/// @brief GoToLine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenView.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <QDialog>

class QLabel;
class QLineEdit;
class QIntValidator;
class QPushButton;

//////////////////////////////////////////////////////////////////////
/// @class GoToLine GoToLine.h
/// @brief 行番号を指定するウィジェット
//////////////////////////////////////////////////////////////////////
class GoToLine :
  public QDialog
{
  Q_OBJECT

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////
  
  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  GoToLine(QWidget* parent = NULL);

  /// @brief デストラクタ
  ~GoToLine();


public slots:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数．スロットでもある．
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 最小値を設定する．
  void
  set_minimum(int val);

  /// @brief 最大値を設定する．
  void
  set_maximum(int val);
  
  /// @brief 値を設定する．
  /// @param[in] val 設定する値
  void
  set_value(int val);


signals:
  //////////////////////////////////////////////////////////////////////
  // このウィジェットが emit するシグナル
  //////////////////////////////////////////////////////////////////////

  /// @brief "GoTo" ボタンが押された時に emit されるシグナル
  /// @param[in] val 変更後の値
  void
  on_goto(int val);
  

private slots:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるスロット
  //////////////////////////////////////////////////////////////////////

  /// @brief LineEdit のシグナルを受け取る．
  void
  on_lineedit_changed(const QString& text);

  /// @brief GoTo ボタンのクリックイベント
  void
  on_goto_clicked();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ラベル
  QLabel* mLabel;

  // 数値の入力用ウィジェット
  QLineEdit* mLineEdit;

  // 値の validator
  QIntValidator* mValidator;

  // "GoTo" ボタン
  QPushButton* mGotoButton;
  
};

#endif // GOTOLINE_H
