
/// @file libym_mincov/MaxClique.cc
/// @brief MaxClique の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: MaxClique.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MaxClique.h"
#include "ym_mincov/MincovCost.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MaxClique
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size ノード数
MaxClique::MaxClique(ymuint size) :
  mCostArray(size),
  mNlistArray(size)
{
}
  
// @brief デストラクタ
MaxClique::~MaxClique()
{
}

// @brief コストを設定する．
void
MaxClique::set_cost(ymuint32 id,
		    const MincovCost* cost)
{
  mCostArray[id] = cost;
}
  
// @brief 2つのノードを隣接させる．
// @param[in] id1, id2 ノード番号
void
MaxClique::connect(ymuint32 id1,
		   ymuint32 id2)
{
  mNlistArray[id1].push_back(id2);
  mNlistArray[id2].push_back(id1);
}


BEGIN_NONAMESPACE

// left の要素のうち right にも含まれるもののみを残す．
void
merge_nlist(vector<ymuint32>& left,
	    const vector<ymuint32>& right)
{
  vector<ymuint32>::iterator l_rpos = left.begin();
  vector<ymuint32>::iterator l_wpos = left.begin();
  vector<ymuint32>::iterator l_end = left.end();
  vector<ymuint32>::const_iterator r_rpos = right.begin();
  vector<ymuint32>::const_iterator r_end = right.end();
  for ( ; ; ) {
    if ( l_rpos == l_end ) {
      return;
    }
    ymuint32 l_id = *l_rpos;
    if ( r_rpos == r_end ) {
      left.erase(l_wpos, l_end);
      return;
    }
    ymuint32 r_id = *r_rpos;
    if ( l_id < r_id ) {
      ++ l_rpos;
    }
    else if ( l_id == r_id ) {
      if ( l_rpos != l_wpos ) {
	*l_wpos = l_id;
      }
      ++ l_rpos;
      ++ l_wpos;
      ++ r_rpos;
    }
    else { // l_id > r_id
      ++ r_rpos;
    }
  }
}

END_NONAMESPACE

// @brief 最大クリークを求める．
// @param[out] ans 解のノード番号を入れる配列
MincovCost
MaxClique::solve(vector<ymuint32>& ans)
{
  // mNlistArray を整列させる．
  for (vector<vector<ymuint32> >::iterator p = mNlistArray.begin();
       p != mNlistArray.end(); ++ p) {
    vector<ymuint32>& nlist = *p;
    sort(nlist.begin(), nlist.end());
  }
  
  ans.clear();

  MincovCost cost(MincovCost::zero());
  ymuint32 n = mCostArray.size();
  vector<ymuint32> v_list;
  { // 種となるノードを求める．
    MincovCost max_cost(0.0, 0.0);
    ymuint32 max_id = 0;
    for (ymuint i = 0; i < n; ++ i) {
      MincovCost tmp_cost = *mCostArray[i];
      vector<ymuint32>& nlist = mNlistArray[i];
      for (vector<ymuint32>::iterator p = nlist.begin();
	   p != nlist.end(); ++ p) {
	tmp_cost += *mCostArray[*p];
      }
      if ( max_cost < tmp_cost ) {
	max_cost = tmp_cost;
	max_id = i;
      }
    }
    ans.push_back(max_id);
    cost = *mCostArray[max_id];
    v_list = mNlistArray[max_id];
    for (vector<ymuint32>::iterator p = v_list.begin();
	 p != v_list.end(); ++ p) {
      ymuint32 id = *p;
      merge_nlist(mNlistArray[id], v_list);
    }
  }
  while ( !v_list.empty() ) {
    MincovCost max_cost(0.0, 0.0);
    ymuint32 max_id = 0;
    for (vector<ymuint32>::iterator p = v_list.begin();
	 p != v_list.end(); ++ p) {
      ymuint32 id = *p;
      MincovCost tmp_cost = *mCostArray[id];
      vector<ymuint32>& nlist = mNlistArray[id];
      for (vector<ymuint32>::iterator q = nlist.begin();
	   q != nlist.end(); ++ q) {
	tmp_cost += *mCostArray[*q];
      }
      if ( max_cost < tmp_cost ) {
	max_cost = tmp_cost;
	max_id = id;
      }
    }
    ans.push_back(max_id);
    cost += *mCostArray[max_id];
    merge_nlist(v_list, mNlistArray[max_id]);
    for (vector<ymuint32>::iterator p = v_list.begin();
	 p != v_list.end(); ++ p) {
      merge_nlist(mNlistArray[*p], mNlistArray[max_id]);
    }
  }
  return cost;
}

END_NAMESPACE_YM_MINCOV
