
/// @file libym_blif/tests/lexptest.cc
/// @brief blif ファイルの読み込みに置ける LogExpr の性能テスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: lexptest.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/BlifParser.h"
#include "ym_blif/BlifHandler.h"
#include "ym_utils/MsgHandler.h"
#include "ym_lexp/LogExpr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TestBlifHandler parsertest.cc
/// @brief テスト用の BlifHandler
//////////////////////////////////////////////////////////////////////
class TestBlifHandler :
  public BlifHandler
{
public:

  /// @brief コンストラクタ
  TestBlifHandler();

  /// @brief デストラクタ
  virtual
  ~TestBlifHandler();


public:

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init();
  
  /// @brief .model 文の処理
  /// @param[in] loc1 .model の位置情報
  /// @param[in] loc2 文字列の位置情報
  /// @param[in] name 文字列
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  model(const FileRegion& loc1,
	const FileRegion& loc2,
	const char* name);

  /// @brief .inputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  inputs_elem(ymuint32 name_id);

  /// @brief .outputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  outputs_elem(ymuint32 name_id);

  /// @brief .names 文の処理
  /// @param[in] name_id_array 各識別子のID番号の配列
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 入力カバーを表す文字列
  /// @param[in] opat 出力の極性
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  /// @note cover_pat は ni 個ごとに1行のパタンを表す．
  /// 各要素のとりうる値は '0', '1', '-' を表す．
  /// @note opat は '0' か '1' のどちらか
  virtual
  bool
  names(const vector<ymuint32>& name_id_array,
	ymuint32 nc,
	const char* cover_pat,
	char opat);

  /// @brief .gate 文の開始
  /// @param[in] loc1 .gate の位置情報
  /// @param[in] loc2 セル名の位置情報
  /// @param[in] name セル名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_begin(const FileRegion& loc1,
	     const FileRegion& loc2,
	     const char* name);

  /// @brief .gate 文中のピン割り当ての処理
  /// @param[in] loc1 ピン名の位置情報
  /// @param[in] f_name ピン名
  /// @param[in] loc2 ノード名の位置情報
  /// @param[in] a_name ノード名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_assign(const FileRegion& loc1,
	      const char* f_name,
	      const FileRegion& loc2,
	      const char* a_name);

  /// @brief .gate 文の終了
  virtual
  bool
  gate_end();

  /// @brief .latch 文の処理
  /// @param[in] name1_id 最初の識別子のID番号
  /// @param[in] name2_id 次の識別子のID番号
  /// @param[in] loc4 リセット値の位置情報
  /// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  latch(ymuint32 name1_id,
	ymuint32 name2_id,
	const FileRegion& loc4,
	char rval);

  /// @brief .end 文の処理
  /// @param[in] loc 位置情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end(const FileRegion& loc);

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


private:

  size_t mNi;
  int opol;
  
  // cubepat2expr で用いる作業領域
  vector<LogExpr> mChd1;
  vector<LogExpr> mChd2;

  list<LogExpr*> mPageList;
  
  size_t mCount;
  LogExpr* mPage;
};


// @brief コンストラクタ
TestBlifHandler::TestBlifHandler()
{
  mCount = 0;
  mPage = NULL;
}

// @brief デストラクタ
TestBlifHandler::~TestBlifHandler()
{
  for (list<LogExpr*>::iterator p = mPageList.begin();
       p != mPageList.end(); ++ p) {
    delete [] *p;
  }

}

// @brief 初期化
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::init()
{
  return true;
}
  
// @brief .model 文の処理
// @param[in] loc1 .model の位置情報
// @param[in] loc2 文字列の位置情報
// @param[in] name 文字列
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::model(const FileRegion& loc1,
		       const FileRegion& loc2,
		       const char* name)
{
  return true;
}

// @brief .inputs 文中の文字列の処理
// @param[in] name_id 文字列のID番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::inputs_elem(ymuint32 name_id)
{
  return true;
}

// @brief .outputs 文中の文字列の処理
// @param[in] name_id 文字列のID番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::outputs_elem(ymuint32 name_id)
{
  return true;
}

// @brief .names 文の処理
// @param[in] name_id_array 各識別子のID番号の配列
// @param[in] nc キューブ数
// @param[in] cover_pat 入力カバーを表す文字列
// @param[in] opat 出力の極性
// @retval true 処理が成功した．
// @retval false エラーが起こった．
// @note cover_pat は ni 個ごとに1行のパタンを表す．
// 各要素のとりうる値は '0', '1', '-' を表す．
// @note opat は '0' か '1' のどちらか
bool
TestBlifHandler::names(const vector<ymuint32>& name_id_array,
		       ymuint32 nc,
		       const char* cover_pat,
		       char opat)
{
  ymuint32 ni = name_id_array.size() - 1;
  LogExpr expr;
  if ( opat == '1' ) {
    vector<LogExpr> or_expr;
    for (ymuint32 c = 0; c < nc; ++ c) {
      vector<LogExpr> and_expr;
      for (ymuint32 i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  and_expr.push_back(LogExpr::make_negaliteral(i));
	}
	else if ( v == '1' ) {
	  and_expr.push_back(LogExpr::make_posiliteral(i));
	}
      }
      or_expr.push_back(LogExpr::make_and(and_expr));
    }
    if ( or_expr.empty() ) {
      expr = LogExpr::make_one();
    }
    else {
      expr = LogExpr::make_or(or_expr);
    }
  }
  else {
    vector<LogExpr> and_expr;
    for (ymuint32 c = 0; c < nc; ++ c) {
      vector<LogExpr> or_expr;
      for (ymuint32 i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  or_expr.push_back(LogExpr::make_posiliteral(i));
	}
	else if ( v == '1' ) {
	  or_expr.push_back(LogExpr::make_negaliteral(i));
	}
      }
      and_expr.push_back(LogExpr::make_or(or_expr));
    }
    if ( and_expr.empty() ) {
      expr = LogExpr::make_zero();
    }
    else {
      expr = LogExpr::make_and(and_expr);
    }
  }
  
  if ( mPage == NULL || mCount >= 1024 ) {
    mPage = new LogExpr[1024];
    mCount = 0;
    mPageList.push_back(mPage);
  }
  mPage[mCount] = expr;
  ++ mCount;
  
  return true;
}

// @brief .gate 文の開始
// @param[in] loc1 .gate の位置情報
// @param[in] loc2 セル名の位置情報
// @param[in] name セル名
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::gate_begin(const FileRegion& loc1,
			    const FileRegion& loc2,
			    const char* name)
{
  return true;
}

// @brief .gate 文中のピン割り当ての処理
// @param[in] loc1 ピン名の位置情報
// @param[in] f_name ピン名
// @param[in] loc2 ノード名の位置情報
// @param[in] a_name ノード名
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::gate_assign(const FileRegion& loc1,
			     const char* f_name,
			     const FileRegion& loc2,
			     const char* a_name)
{
  return true;
}

// @brief .gate 文の終了
bool
TestBlifHandler::gate_end()
{
  return true;
}

// @brief .latch 文の処理
// @param[in] name1_id 最初の識別子のID番号
// @param[in] name2_id 次の識別子のID番号
// @param[in] loc4 リセット値の位置情報
// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::latch(ymuint32 name1_id,
		       ymuint32 name2_id,
		       const FileRegion& loc4,
		       char rval)
{
  return true;
}

// @brief .end 文の処理
// @param[in] loc 位置情報
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::end(const FileRegion& loc)
{
  return true;
}

// @brief 通常終了時の処理
void
TestBlifHandler::normal_exit()
{
}

// @brief エラー終了時の処理
void
TestBlifHandler::error_exit()
{
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " blif-file" << endl;
    return 2;
  }
  
  try {
    cout << "BEFORE LOOP" << endl;
    LogExpr::print_stats(cout);
    string filename = argv[1];
    for (size_t i = 0; i < 10; ++ i) {
      BlifParser parser;
      {
	TestBlifHandler* handler = new TestBlifHandler;
	parser.add_handler(handler);
	StreamMsgHandler* msg_handler = new StreamMsgHandler(&cerr);
	parser.add_msg_handler(msg_handler);
    
	if ( !parser.read(filename) ) {
	  cerr << "Error in reading " << argv[1] << endl;
	  return 4;
	}
	cout << "END_OF_READ" << endl;
	LogExpr::print_stats(cout);
      }
      cout << "END_OF_LOOP" << endl;
      LogExpr::print_stats(cout);
    }
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

  return 0;
}
