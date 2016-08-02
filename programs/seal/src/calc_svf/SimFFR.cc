
/// @file calc_svf/SimFFR.cc
/// @brief SimFFR の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimFFR.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG_H
#include "seal_config.h"
#endif


#include "SimFFR.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SEAL_SVF

// @breif DSS ノードを追加する．
void
SimFFR::set_dss(const vector<SimNode*>& node_list)
{
  //mDSS = node_list;
  mDSS.resize(node_list.size());
  for (size_t i = 0; i < node_list.size(); ++ i) {
    mDSS[i] = node_list[i];
  }
}
  
// @brief DSS ノードを target にする．
tPackedVal
SimFFR::set_target()
{
  if ( mDSS.empty() ) {
    return kPvAll1;
  }
  tPackedVal req = kPvAll0;
  for (vector<SimNode*>::const_iterator p = mDSS.begin();
       p != mDSS.end(); ++ p) {
    SimNode* node = *p;
    node->set_target();
    req |= node->get_obs();
  }
  return req;
}

// @brief DSS ノードの target を消す．
void
SimFFR::clear_target()
{
  for (vector<SimNode*>::const_iterator p = mDSS.begin();
       p != mDSS.end(); ++ p) {
    SimNode* node = *p;
    node->clear_target();
  }
}

END_NAMESPACE_YM_SEAL_SVF
