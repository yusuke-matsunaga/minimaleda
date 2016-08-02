
/// @file libym_sat/tests/ymsat.cc
/// @brief 簡単な SAT プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ymsat.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/DimacsParser.h"
#include "ym_sat/SatSolver.h"

#include "ym_utils/StopWatch.h"

#if USE_ZSTREAM
#include "ym_utils/zstream.h"
#endif


BEGIN_NAMESPACE_YM
BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
/// @class SatDimacsHandler ymsat.cc
/// @brief ymsat 用の DimacsHandler
//////////////////////////////////////////////////////////////////////
class SatDimacsHandler :
  public DimacsHandler
{
public:

  /// @brief コンストラクタ
  SatDimacsHandler(SatSolver* solver);

  /// @brief デストラクタ
  virtual
  ~SatDimacsHandler();


public:

  /// @brief 初期化
  virtual
  bool
  init();
  
  /// @brief p 行の読込み
  /// @param[in] lineno 行番号
  /// @param[in] nv 変数の数
  /// @param[in] nc 節の数
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_p(int lineno,
	 size_t nv,
	 size_t nc);
  
  /// @brief clause 行の読込み
  /// @param[in] lineno 行番号
  /// @param[in] lits リテラルの配列．最後の0は含まない
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_clause(int lineno,
	      const vector<int>& lits);
  
  /// @brief 終了処理
  virtual
  bool
  end();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();

  /// @brief 検証する．
  bool
  verify(const vector<Bool3>& model) const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  SatSolver* mSolver;

  vector<vector<int> > clause_list;

};


//////////////////////////////////////////////////////////////////////
// ymsat 用の DimacsMsgHandler
//////////////////////////////////////////////////////////////////////
class SatDimacsMsgHandler :
  public DimacsMsgHandler
{
public:

  /// @brief コンストラクタ
  SatDimacsMsgHandler();

  /// @brief デストラクタ
  virtual
  ~SatDimacsMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは下記の仮想関数を上書きすること
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] lineno ファイル中の行番号
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  operator()(const char* src_file,
	     int src_line,
	     int lineno,
	     const string& label,
	     const string& body);
};


//////////////////////////////////////////////////////////////////////
// ymsat 用の SatMsgHandler
//////////////////////////////////////////////////////////////////////
class YmsatMsgHandler :
  public SatMsgHandler
{
public:

  /// @brief コンストラクタ
  YmsatMsgHandler(ostream& s);

  /// @brief デストラクタ
  virtual
  ~YmsatMsgHandler();

  /// @brief メッセージ出力関数
  virtual
  void
  operator()(const SatStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mS;

};


// @brief コンストラクタ
SatDimacsHandler::SatDimacsHandler(SatSolver* solver) :
  mSolver(solver)
{
}

// @brief デストラクタ
SatDimacsHandler::~SatDimacsHandler()
{
}

// @brief 初期化
bool
SatDimacsHandler::init()
{
  return true;
}
  
// @brief p 行の読込み
// @param[in] lineno 行番号
// @param[in] nv 変数の数
// @param[in] nc 節の数
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
SatDimacsHandler::read_p(int lineno,
			 size_t nv,
			 size_t nc)
{
  for (size_t i = 0; i < nv; ++ i) {
    mSolver->new_var();
  }

  return true;
}

// @brief clause 行の読込み
// @param[in] lineno 行番号
// @param[in] lits リテラルの配列．最後の0は含まない
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
SatDimacsHandler::read_clause(int lineno,
			      const vector<int>& lits)
{
  vector<Literal> tmp;
  for (vector<int>::const_iterator p = lits.begin();
       p != lits.end(); ++ p) {
    int i = *p;
    tVarId vid;
    tPol pol;
    if ( i > 0 ) {
      vid = static_cast<tVarId>(i - 1);
      pol = kPolPosi;
    }
    else {
      vid = static_cast<tVarId>(- i - 1);
      pol = kPolNega;
    }
    tmp.push_back(Literal(vid, pol));
  }
  mSolver->add_clause(tmp);

  clause_list.push_back(lits);

  return true;
}
  
// @brief 終了処理
// @param[in] loc 位置情報
bool
SatDimacsHandler::end()
{
  return true;
}

// @brief エラー終了時の処理
void
SatDimacsHandler::error_exit()
{
}

// @brief 検証する．
bool
SatDimacsHandler::verify(const vector<Bool3>& model) const
{
  for (vector<vector<int> >::const_iterator p = clause_list.begin();
       p != clause_list.end(); ++ p) {
    const vector<int>& lits = *p;
    bool sat = false;
    for (vector<int>::const_iterator q = lits.begin();
	 q != lits.end(); ++ q) {
      int l = *q;
      if ( l > 0 ) {
	if ( model[l - 1] == kB3True ) {
	  sat = true;
	  break;
	}
      }
      else {
	if ( model[-l - 1] == kB3False ) {
	  sat = true;
	  break;
	}
      }
    }
    if ( !sat ) {
      const char* plus = "";
      for (vector<int>::const_iterator q = lits.begin();
	   q != lits.end(); ++ q) {
	int l = *q;
	cout << plus << l;
	plus = " + ";
      }
      cout << " is unsatisfied" << endl;
      return false;
    }
  }
  return true;
}


// @brief コンストラクタ
SatDimacsMsgHandler::SatDimacsMsgHandler()
{
}

// @brief デストラクタ
SatDimacsMsgHandler::~SatDimacsMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] lineno ファイル中の行番号
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
SatDimacsMsgHandler::operator()(const char* src_file,
				int src_line,
				int lineno,
				const string& label,
				const string& body)
{
  cerr << "Line " << lineno << ": [" << label << "]: " << body << endl;
}

// @brief コンストラクタ
YmsatMsgHandler::YmsatMsgHandler(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
YmsatMsgHandler::~YmsatMsgHandler()
{
}

// @brief メッセージ出力関数
void
YmsatMsgHandler::operator()(const SatStats& stats)
{
  mS << "| "
     << setw(9) << stats.mConflictNum
     << " | "
     << setw(9) << stats.mConstrClauseNum
     << " "
     << setw(9) << stats.mConstrLitNum
     << " | "
     << setw(9) << stats.mLearntLimit
     << " "
     << setw(9) << stats.mLearntClauseNum
     << " "
     << setw(9) << stats.mLearntLitNum
     << " |" << endl;
}

END_NONAMESPACE
END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " cnf-file" << endl;
    return 2;
  }
  
  ifstream s(argv[1]);
  if ( !s.is_open() ) {
    cerr << "Could not open " << argv[1] << endl;
    return 3;
  }
  
  try {
#if USE_ZSTREAM
    izstream zs(s);
    istream& s1 = zs;
#else
    istream& s1 = s;
#endif
    SatSolver* solver = SatSolverFactory::gen_solver();
    
    DimacsParser parser;
    SatDimacsHandler handler(solver);
    SatDimacsMsgHandler msg_handler;
    parser.add_handler(&handler);
    parser.add_msg_handler(&msg_handler);
    
    if ( !parser.read(s1) ) {
      cerr << "Error in reading " << argv[1] << endl;
      return 4;
    }

    YmsatMsgHandler satmsghandler(cout);
    solver->reg_msg_handler(&satmsghandler);
    solver->timer_on(true);
    
    cout << "===================================================================" << endl;
    cout << "| conflicts |       ORIGINAL      |             LEARNT            |" << endl;
    cout << "|           |   Clauses      Lits |     limit   Clauses      Lits |" << endl;
    cout << "===================================================================" << endl;

    StopWatch sw;
    sw.start();
    vector<Bool3> model;
    Bool3 ans = solver->solve(model);
    sw.stop();

    SatStats stats;
    solver->get_stats(stats);

    cout << "===================================================================" << endl;
    cout << "restarts          : " << stats.mRestart << endl
	 << "conflicts         : " << stats.mConflictNum << endl
	 << "decisions         : " << stats.mDecisionNum << endl
	 << "propagations      : " << stats.mPropagationNum << endl
	 << "conflict literals : " << stats.mLearntLitNum << endl
	 << "CPU time          : " << stats.mTime << endl;
    
    if ( ans == kB3True ) {
      handler.verify(model);
    }

    switch ( ans ) {
    case kB3True:  cout << "SAT" << endl; break;
    case kB3False: cout << "UNSAT" << endl; break;
    case kB3X:     cout << "UNKNOWN" << endl; break;
    }
    
    USTime ust = sw.time();
    cout << ust << endl;
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

#if USE_ZSTREAM
  catch ( zlib_error x ) {
    cout << x.mMsg << endl;
  }
#endif
 
  return 0;
}
