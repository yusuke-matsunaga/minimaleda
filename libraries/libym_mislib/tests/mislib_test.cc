
/// @file libym_mislib/tests/mislib_test.cc
/// @brief MislibParser のテスト用プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: mislib_test.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include <ym_utils/StopWatch.h>
#include <ym_mislib/MislibParser.h>
#include <ym_mislib/MislibHandler.h>


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// テスト用の MislibHandler クラス
//////////////////////////////////////////////////////////////////////
class TestMislibHandler :
  public MislibHandler
{
public:

  /// @brief コンストラクタ
  TestMislibHandler();

  /// @brief デストラクタ
  ~TestMislibHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // MislibHandler で定義されている仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  virtual
  void
  init();

  /// @brief GATE 文の処理
  /// @param[in] loc GATE 文の位置情報
  /// @param[in] name 名前
  /// @param[in] name_loc 名前の位置情報
  /// @param[in] area 面積
  /// @param[in] area_loc 面積の位置情報
  /// @param[in] oname 出力ピン名
  /// @param[in] oname_loc 出力ピン名の位置情報
  /// @param[in] expr 論理式
  /// @param[in] expr_loc 論理式の位置情報
  /// @param[in] ipin_list 入力ピンの情報の配列
  virtual
  void
  gate(const FileRegion& loc,
       ShString name,
       const FileRegion& name_loc,
       double area,
       const FileRegion& area_loc,
       ShString oname,
       const FileRegion& oname_loc,
       const LogExpr& expr,
       const FileRegion& expr_loc,
       const vector<MislibPin>& ipin_list);

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();
  
};

// @brief コンストラクタ
TestMislibHandler::TestMislibHandler()
{
}

// @brief デストラクタ
TestMislibHandler::~TestMislibHandler()
{
}

// @brief 初期化
void
TestMislibHandler::init()
{
}

// @brief GATE 文の処理
// @param[in] loc GATE 文の位置情報
// @param[in] name 名前
// @param[in] name_loc 名前の位置情報
// @param[in] area 面積
// @param[in] area_loc 面積の位置情報
// @param[in] oname 出力ピン名
// @param[in] oname_loc 出力ピン名の位置情報
// @param[in] expr 論理式
// @param[in] expr_loc 論理式の位置情報
// @param[in] ipin_list 入力ピンの情報の配列
void
TestMislibHandler::gate(const FileRegion& loc,
			ShString name,
			const FileRegion& name_loc,
			double area,
			const FileRegion& area_loc,
			ShString oname,
			const FileRegion& oname_loc,
			const LogExpr& expr,
			const FileRegion& expr_loc,
			const vector<MislibPin>& ipin_list)
{
  cout << "GATE:  ";
  cout << loc << endl
       << "  NAME: " << name << ": " << name_loc << endl
       << "  AREA: " << area << ": " << area_loc << endl
       << "  ONAME: " << oname << ": " << oname_loc << endl
       << "  EXPR: " << expr << " : " << expr_loc << endl;
  for (size_t i = 0; i < ipin_list.size(); ++ i) {
    const MislibPin& pin = ipin_list[i];
    cout << "  PIN#" << i << ": " << pin.loc() << endl
	 << "    NAME: " << pin.name()
	 << ": " << pin.name_loc() << endl
	 << "    PHASE: ";
    switch ( pin.phase() ) {
    case MislibPin::kNoninv:
      cout << "NONINV";
      break;

    case MislibPin::kInv:
      cout << "INV";
      break;

    case MislibPin::kUnknown:
      cout << "UNKNOWN";
      break;
    }
    cout << ": " << pin.phase_loc() << endl
	 << "    INPUT_LOAD: " << pin.input_load()
	 << ": " << pin.input_load_loc() << endl
	 << "    MAX_LOAD: " << pin.max_load()
	 << ": " << pin.max_load_loc() << endl
	 << "    RB_DELAY: " << pin.rise_block_delay()
	 << ": " << pin.rise_block_delay_loc() << endl
	 << "    RF_DELAY: " << pin.rise_fanout_delay()
	 << ": " << pin.rise_fanout_delay_loc() << endl
	 << "    FB_DELAY: " << pin.fall_block_delay()
	 << ": " << pin.fall_block_delay_loc() << endl
	 << "    FF_DELAY: " << pin.fall_fanout_delay()
	 << ": " << pin.fall_fanout_delay_loc() << endl;
  }
}

// @brief 通常終了時の処理
void
TestMislibHandler::normal_exit()
{
}

// @brief エラー終了時の処理
void
TestMislibHandler::error_exit()
{
}

END_NAMESPACE_YM_MISLIB


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;
  using nsYm::nsMislib::MislibParser;
  using nsYm::nsMislib::MislibHandler;
  using nsYm::nsMislib::TestMislibHandler;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 255;
  }
  
  MislibParser mislib_parser;
  
  MislibHandler* handler = new TestMislibHandler;
  mislib_parser.add_handler(handler);

  MsgHandler* msg_handler = new StreamMsgHandler(&cout);
  mislib_parser.add_msg_handler(msg_handler);
  
  bool stat = mislib_parser.read(argv[1]);
  if ( !stat ) {
    return 1;
  }
  
  return 0;
}
