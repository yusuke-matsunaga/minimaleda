
/// @file magus/lutmap/GenIlpCmd.cc
/// @brief GenIlpCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: GenIlpCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LutmapCmd.h"
#include "ym_lutmap/SbjGraph.h"
#include "ym_lutmap/EnumCutOp.h"
#include "ILPGen.h"


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// カット番号を表す文字列を作る．
string
cut_str(ymuint id)
{
  ostringstream buf;
  buf << "C" << id;
  return buf.str();
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス ILPGenCut
//////////////////////////////////////////////////////////////////////

// コンストラクタ
ILPGenCut::ILPGenCut(ymuint id,
		     SbjNode* root,
		     ymuint ni,
		     SbjNode** inputs) :
  mId(id),
  mRoot(root),
  mInputs(ni)
{
  for (ymuint i = 0; i < ni; ++ i) {
    mInputs[i] = inputs[i];
  }
}

// デストラクタ
ILPGenCut::~ILPGenCut()
{
}

// ID 番号を得る．
ymuint
ILPGenCut::id() const
{
  return mId;
}

// @brief 根のノードを得る．
SbjNode*
ILPGenCut::root() const
{
  return mRoot;
}
  
// @brief 入力のサイズを得る．
ymuint
ILPGenCut::ni() const
{
  return mInputs.size();
}

// @brief pos 番目の入力を得る．
SbjNode*
ILPGenCut::input(ymuint pos) const
{
  return mInputs[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス ILPGen
//////////////////////////////////////////////////////////////////////

// コンストラクタ
ILPGen::ILPGen()
{
}

// デストラクタ
ILPGen::~ILPGen()
{
  delete_cuts();
}

// 現在の制約式を出力する．
void
ILPGen::dump(ostream& s) const
{
  s << "minimize" << endl;
  const char* delim = "";
  for (list<ymuint>::const_iterator p = mVars.begin();
       p != mVars.end(); ++ p) {
    s << delim << cut_str(*p);
    delim = " + ";
  }
  s << endl;
  s << "subject to" << endl;
  for (list<string>::const_iterator p = mConstr.begin();
       p != mConstr.end(); ++ p) {
    s << *p << endl;
  }
  s << "binaries" << endl;
  for (list<ymuint>::const_iterator p = mVars.begin();
       p != mVars.end(); ++ p) {
    s << cut_str(*p) << endl;
  }
  s << "end" << endl;
}

// 最初に呼ばれる関数
void
ILPGen::all_init(const SbjGraph& sbjgraph,
		 ymuint limit,
		 tCutMode mode)
{
  delete_cuts();
  mCutId = 0;
  mAllCut.resize(sbjgraph.max_node_id());
  mVars.clear();
  mConstr.clear();
}

void
ILPGen::node_init(SbjNode* node,
		  ymuint pos)
{
}

void
ILPGen::found(SbjNode* root)
{
}

void
ILPGen::found(SbjNode* root,
	      ymuint ni,
	      SbjNode* inputs[])
{
  ILPGenCut* cut = new_cut(root, ni, inputs);
  mVars.push_back(cut->id());
  for (ymuint i = 0; i < ni; ++ i) {
    string buf = "-" + cut_str(cut->id());
    SbjNode* inode = cut->input(i);
    if ( inode->is_input() ) continue;
    const list<ILPGenCut*>& cut_list = mAllCut[inode->id()];
    assert_cond(!cut_list.empty(), __FILE__, __LINE__);
    for (list<ILPGenCut*>::const_iterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      ILPGenCut* cut1 = *p;
      buf += " + " + cut_str(cut1->id());
    }
    buf += " >= 0";
    mConstr.push_back(buf);
  }
}

void
ILPGen::node_end(SbjNode* node,
		 ymuint pos,
		 ymuint ncuts)
{
}

// 最後に呼ばれる関数
void
ILPGen::all_end(const SbjGraph& sbjgraph,
		ymuint limit,
		tCutMode mode)
{
  const SbjNodeList& output_list = sbjgraph.output_list();
  for (SbjNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    SbjNode* onode = *p;
    SbjNode* node = onode->fanin(0);
    if ( node ) {
      string buf;
      const char* delim = "";
      const list<ILPGenCut*>& cut_list = mAllCut[node->id()];
      for (list<ILPGenCut*>::const_iterator p = cut_list.begin();
	   p != cut_list.end(); ++ p) {
	ILPGenCut* cut = *p;
	buf += delim + cut_str(cut->id());
	delim = " + ";
      }
      buf += " >= 1";
      mConstr.push_back(buf);
    }
  }
}

// カットを生成する．
ILPGenCut*
ILPGen::new_cut(SbjNode* root,
		ymuint ni,
		SbjNode* inputs[])
{
  ILPGenCut* cut = new ILPGenCut(mCutId, root, ni, inputs);
  ++ mCutId;
  mAllCut[root->id()].push_back(cut);

  return cut;
}

// 全てのカットを削除する．
void
ILPGen::delete_cuts()
{
  ymuint n = mAllCut.size();
  for (ymuint i = 0; i < n; ++ i) {
    const list<ILPGenCut*>& cut_list = mAllCut[i];
    for (list<ILPGenCut*>::const_iterator p = cut_list.begin();
	 p != cut_list.end(); ++ p) {
      delete *p;
    }
  }
}

// DAG covering 用の制約を作る．
void
generate_ILP(const SbjGraph& sbjgraph,
	     ymuint limit,
	     tCutMode mode,
	     ostream& s)
{
  ILPGen gen;

  gen.enum_cut(sbjgraph, limit, mode);

  gen.dump(s);
}


//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GenIlpCmd::GenIlpCmd(MagMgr* mgr,
		     LutmapData* data) :
  LutmapCmd(mgr, data)
{
  set_usage_string("?<filename>? <#inputs>");
}

// @brief デストラクタ
GenIlpCmd::~GenIlpCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
GenIlpCmd::cmd_proc(TclObjVector& objv)
{
  ymuint objc = objv.size();
  if ( objc > 3 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    ostream* outp = &cout;
    ofstream ofs;
    ymuint base = 1;
    if ( objc == 3 ) {
      base = 2;
      string file_name = objv[1];
      if ( !open_ofile(ofs, file_name) ) {
	// ファイルが開けなかった．
	return TCL_ERROR;
      }
      outp = &ofs;
    }
    int limit;
    int code = int_conv(objv[base], limit);
    if ( code != TCL_OK ) {
      return code;
    }
    generate_ILP(sbjgraph(), limit, kCmAll, *outp);
    return TCL_OK;
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_MAGUS
