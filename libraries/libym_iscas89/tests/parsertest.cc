
/// @file libym_iscas89/tests/parsertest.cc
/// @brief iscas89 ファイルの読み書きのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: parsertest.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/Iscas89Parser.h"
#include "ym_iscas89/Iscas89Handler.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TestIscas89Handler parsertest.cc
/// @brief テスト用の Iscas89Handler
//////////////////////////////////////////////////////////////////////
class TestIscas89Handler :
  public Iscas89Handler
{
public:

  /// @brief コンストラクタ
  TestIscas89Handler(ostream* stream);

  /// @brief デストラクタ
  virtual
  ~TestIscas89Handler();


public:

  /// @brief 初期化
  virtual
  bool
  init();

  /// @brief INPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 入力ピン名の ID 番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_input(const FileRegion& loc,
	     ymuint32 name_id);

  /// @brief OUTPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 出力ピン名の ID 番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_output(const FileRegion& loc,
	      ymuint32 name_id);

  /// @brief ゲート文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] type ゲートの型
  /// @param[in] oname_id 出力名の ID 番号
  /// @param[in] iname_list 入力名のリスト
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_gate(const FileRegion& loc,
	    tIscas89GateType type,
	    ymuint32 oname_id,
	    const vector<ymuint32>& iname_list);
  
  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


private:

  ostream* mStreamPtr;

};


// @brief コンストラクタ
TestIscas89Handler::TestIscas89Handler(ostream* streamptr) :
  mStreamPtr(streamptr)
{
}

// @brief デストラクタ
TestIscas89Handler::~TestIscas89Handler()
{
}

// @brief 初期化
bool
TestIscas89Handler::init()
{
  (*mStreamPtr) << "TestIscas89Handler::init()" << endl;
  return true;
}

// @brief INPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 入力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestIscas89Handler::read_input(const FileRegion& loc,
			       ymuint32 name_id)
{
  (*mStreamPtr) << "TestIscas89Handler::read_input()" << endl
		<< "    " << loc << endl;
  (*mStreamPtr) << "  str(name_id) = " << id2str(name_id) << endl
		<< "  loc(name_id) = " << id2loc(name_id) << endl;
  return true;
}

// @brief OUTPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 出力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestIscas89Handler::read_output(const FileRegion& loc,
				ymuint32 name_id)
{
  (*mStreamPtr) << "TestIscas89Handler::read_output()" << endl
		<< "    " << loc << endl;
  (*mStreamPtr) << "  str(name_id) = " << id2str(name_id) << endl
		<< "  loc(name_id) = " << id2loc(name_id) << endl;
  return true;
}

// @brief ゲート文を読み込む．
// @param[in] loc ファイル位置
// @param[in] type ゲートの型
// @param[in] oname_id 出力名の ID 番号
// @param[in] iname_list 入力名のリスト
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestIscas89Handler::read_gate(const FileRegion& loc,
			      tIscas89GateType type,
			      ymuint32 oname_id,
			      const vector<ymuint32>& iname_list)
{
  (*mStreamPtr) << "TestIscas89Handler::read_gate()" << endl
		<< "    " << loc << endl;
  (*mStreamPtr) << "  str(oname_id) = " << id2str(oname_id) << endl
		<< "  loc(oname_id) = " << id2loc(oname_id) << endl;
  (*mStreamPtr) << "  type = ";
  switch ( type ) {
  case kGt89BUFF: (*mStreamPtr) << "BUFF"; break;
  case kGt89NOT:  (*mStreamPtr) << "NOT"; break;
  case kGt89AND:  (*mStreamPtr) << "AND"; break;
  case kGt89NAND: (*mStreamPtr) << "NAND"; break;
  case kGt89OR:   (*mStreamPtr) << "OR"; break;
  case kGt89NOR:  (*mStreamPtr) << "NOR"; break;
  case kGt89XOR:  (*mStreamPtr) << "XOR"; break;
  case kGt89XNOR: (*mStreamPtr) << "XNOR"; break;
  case kGt89DFF:  (*mStreamPtr) << "DFF"; break;
  }
  (*mStreamPtr) << endl;
  size_t i = 0;
  for (vector<ymuint32>::const_iterator p = iname_list.begin();
       p != iname_list.end(); ++ p, ++ i) {
    ymuint32 id = *p;
    (*mStreamPtr) << "  str(iname_list[" << i << "]) = " << id2str(id) << endl
		  << "  loc(iname_list[" << i << "]) = " << id2loc(id) << endl;
  }
  return true;
}

// @brief 通常終了時の処理
void
TestIscas89Handler::normal_exit()
{
  (*mStreamPtr) << "TestIscas89Handler::normal_exit()" << endl;
}

// @brief エラー終了時の処理
void
TestIscas89Handler::error_exit()
{
  (*mStreamPtr) << "TestIscas89Handler::error_exit()" << endl;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " iscas89-file" << endl;
    return 2;
  }
  string filename = argv[1];

  try {
    Iscas89Parser parser;
    TestIscas89Handler* handler = new TestIscas89Handler(&cout);
    parser.add_handler(handler);

    MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
    parser.add_msg_handler(msg_handler);

    if ( !parser.read(filename) ) {
      cerr << "Error in reading " << filename << endl;
      return 4;
    }
  }
  catch ( const AssertError& x) {
    cout << x << endl;
  }

  return 0;
}
