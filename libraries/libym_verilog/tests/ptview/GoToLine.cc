
/// @file libym_verilog/tests/ptview/GoToLine.cc
/// @brief GoToLine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GoToLine.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include <iostream>
#include "GoToLine.h"
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QHBoxLayout>


// @brief コンストラクタ
// @param[in] parent 親のウィジェット
GoToLine::GoToLine(QWidget* parent) :
  QDialog(parent)
{
  mLabel = new QLabel(tr("Line:"));
  mLineEdit = new QLineEdit;
  mValidator = new QIntValidator(mLineEdit);
  mGotoButton = new QPushButton(tr("Go To"));
  
  mLabel->setBuddy(mLineEdit);
  mLineEdit->setValidator(mValidator);
  mGotoButton->setDefault(true);
  mGotoButton->setEnabled(false);
  
  connect(mLineEdit, SIGNAL(textChanged(const QString&)),
	  this, SLOT(on_lineedit_changed(const QString&)));

  connect(mGotoButton, SIGNAL(clicked()),
	  this, SLOT(on_goto_clicked()));
  
  QHBoxLayout* hbox = new QHBoxLayout;
  hbox->addWidget(mLabel);
  hbox->addWidget(mLineEdit);
  hbox->addWidget(mGotoButton);

  setLayout(hbox);
}

// @brief デストラクタ
GoToLine::~GoToLine()
{
}

// @brief 最小値を設定する．
void
GoToLine::set_minimum(int val)
{
  mValidator->setBottom(val);
}

// @brief 最大値を設定する．
void
GoToLine::set_maximum(int val)
{
  mValidator->setTop(val);
}

// @brief 値を設定する．
// @param[in] val 設定する値
void
GoToLine::set_value(int val)
{
  mLineEdit->setText(QString::number(val, 10));
}

// @brief LineEdit のシグナルを受け取る．
void
GoToLine::on_lineedit_changed(const QString& text)
{
  bool ok;
  text.toInt(&ok, 10);
  if ( ok ) {
    mGotoButton->setEnabled(true);
  }
  else {
    mGotoButton->setEnabled(false);
  }
}

// @brief GoTo ボタンのクリックイベント
void
GoToLine::on_goto_clicked()
{
  QString text = mLineEdit->text();
  bool ok;
  int val = text.toInt(&ok, 10);
  if ( ok ) {
    emit on_goto(val);
  }
}
