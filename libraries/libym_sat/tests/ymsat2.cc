
/// @file libym_sat/tests/ymsat2.cc
/// @brief 簡単な SAT プログラム(2)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ymsat2.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_sat/SatSolver.h"

#include "ym_utils/StopWatch.h"

#if USE_ZSTREAM
#include "ym_utils/zstream.h"
#endif


BEGIN_NAMESPACE_YM
BEGIN_NONAMESPACE

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

// c という文字がでるまで読み飛ばす．
void
skip_until(istream& s,
	   int c)
{
  while ( s ) {
    int c1 = s.get();
    if ( c1 == c ) {
      break;
    }
  }
}

// リテラルのリストを読み込む．
void
read_lits(istream& s,
	  vector<Literal>& lits)
{
  while ( s ) {
    int c = s.get();
    if ( c == ' ' || c == '\t' ) continue;
    if ( c == '\n' ) {
      break;
    }
    if ( c < '0' || c > '9' ) {
      cerr << "illegal input" << endl;
      exit(255);
    }
    int id = c - '0';
    while ( s ) {
      int c = s.get();
      if ( c == 'N' ) {
	lits.push_back(Literal(id, kPolNega));
	break;
      }
      else if ( c == 'P' ) {
	lits.push_back(Literal(id, kPolPosi));
	break;
      }
      else if ( c >= '0' && c <= '9' ) {
	id = id * 10 + (c - '0');
      }
      else {
	cerr << "illegal input" << endl;
	exit(255);
      }
    }
  }
}

END_NONAMESPACE
END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  int verbose = 0;
  bool minisat = false;

  int wpos = 1;
  string opt;
  for (int rpos = 1; rpos < argc; ++ rpos) {
    if ( argv[rpos][0] == '-' ) {
      if ( strcmp(argv[rpos], "-v") == 0 ||
	   strcmp(argv[rpos], "--verbose") == 0 ) {
	++ verbose;
      }
      else if ( strcmp(argv[rpos], "--minisat") == 0 ) {
	minisat = true;
      }
      else if ( strcmp(argv[rpos], "--analyzer-uip1") == 0 ) {
	opt = "uip1";
      }
      else if ( strcmp(argv[rpos], "--analyzer-simple") == 0 ) {
	opt = "simple";
      }
      else {
	cerr << argv[rpos] << " : illegal option" << endl;
	return 1;
      }
    }
    else {
      if ( rpos != wpos ) {
	argv[wpos] = argv[rpos];
      }
      ++ wpos;
    }
  }
  argc = wpos;
  
  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " satlog-file" << endl;
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
    SatSolver* solver;
    if ( minisat ) {
      solver = SatSolverFactory::gen_minisat();
    }
    else {
      solver = SatSolverFactory::gen_solver(opt);
    }
    
    YmsatMsgHandler satmsghandler(cout);
    if ( verbose > 2 ) {
      solver->reg_msg_handler(&satmsghandler);
    
      cout << "===================================================================" << endl;
      cout << "| conflicts |       ORIGINAL      |             LEARNT            |" << endl;
      cout << "|           |   Clauses      Lits |     limit   Clauses      Lits |" << endl;
      cout << "===================================================================" << endl;
    }

    solver->timer_on(true);
    StopWatch sw;
    sw.start();

    while ( s1 ) {
      int c = s1.get();
      if ( c == '\n' ) continue;
      if ( c == '#' ) {
	skip_until(s1, '\n');
	continue;
      }
      if ( c == 'N' ) {
	skip_until(s1, '\n');
	solver->new_var();
      }
      else if ( c == 'A' ) {
	vector<Literal> lits;
	read_lits(s1, lits);
	if ( verbose > 1 ) {
	  cout << "A";
	  for (vector<Literal>::iterator p = lits.begin();
	       p != lits.end(); ++ p) {
	    Literal l = *p;
	    cout << " " << l;
	  }
	  cout << endl;
	}
	solver->add_clause(lits);
      }
      else if ( c == 'S' ) {
	vector<Literal> lits;
	read_lits(s1, lits);
	if ( verbose > 1 ) {
	  cout << "S";
	  for (vector<Literal>::iterator p = lits.begin();
	       p != lits.end(); ++ p) {
	    Literal l = *p;
	    cout << " " << l;
	  }
	  cout << endl;
	}
	vector<Bool3> model;
	solver->solve(lits, model);
	if ( verbose ) {
	  SatStats stats;
	  solver->get_stats(stats);
	  cout << "===================================================================" << endl;
	  cout << "restarts          : " << stats.mRestart << endl
	       << "conflicts         : " << stats.mConflictNum << endl
	       << "decisions         : " << stats.mDecisionNum << endl
	       << "propagations      : " << stats.mPropagationNum << endl
	       << "conflict literals : " << stats.mLearntLitNum << endl
	       << "clauses           : " << stats.mConstrClauseNum << endl
	       << "clause literals   : " << stats.mConstrLitNum << endl
	       << "learnts           : " << stats.mLearntClauseNum << endl
	       << "CPU time          : " << stats.mTime << endl;
	}
      }
    }

    SatStats stats;
    solver->get_stats(stats);

    cout << "===================================================================" << endl;
    cout << "restarts          : " << stats.mRestart << endl
	 << "conflicts         : " << stats.mConflictNum << endl
	 << "decisions         : " << stats.mDecisionNum << endl
	 << "propagations      : " << stats.mPropagationNum << endl
	 << "conflict literals : " << stats.mLearntLitNum << endl
	 << "clauses           : " << stats.mConstrClauseNum << endl
	 << "clause literals   : " << stats.mConstrLitNum << endl
	 << "learnts           : " << stats.mLearntClauseNum << endl
	 << "CPU time          : " << stats.mTime << endl;

    sw.stop();
    
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
