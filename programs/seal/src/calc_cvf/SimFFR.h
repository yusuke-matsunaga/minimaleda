#ifndef CALC_CVF_SIMFFR_H
#define CALC_CVF_SIMFFR_H

/// @file calc_cvf/SimFFR.h
/// @brief SimFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimFFR.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include "nsdef.h"
#include "seal_utils.h"


BEGIN_NAMESPACE_YM_SEAL_CVF

class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class SimFFR SimFFR.h "SimFFR.h"
/// @brief Fanout-Free-Region を表すクラス
//////////////////////////////////////////////////////////////////////
class SimFFR
{
public:

  /// @brief コンストラクタ
  SimFFR();
  
  /// @brief デストラクタ
  ~SimFFR();


public:
  
  /// @brief 根のノードを設定する．
  void
  set_root(SimNode* root);
  
  /// @brief 根のノードを得る．
  SimNode*
  root() const;
  
  /// @breif DSS ノードを設定する．
  void
  set_dss(const vector<SimNode*>& node_list);
  
  /// @brief DSS ノードを target にする．
  tPackedVal
  set_target();

  /// @brief DSS ノードの target を消す．
  void
  clear_target();
  
  /// @brief DSS ノードのリストを取り出す．
  const vector<SimNode*>&
  dss_list() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  SimNode* mRoot;

  // DSS のリスト
  vector<SimNode*> mDSS;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
inline
SimFFR::SimFFR()
{
}

// @brief デストラクタ
inline
SimFFR::~SimFFR()
{
}

// @brief 根のノードを設定する．
inline
void
SimFFR::set_root(SimNode* root)
{
  mRoot = root;
}

// @brief 根のノードを得る．
inline
SimNode*
SimFFR::root() const
{
  return mRoot;
}
  
// @brief DSS ノードのリストを取り出す．
inline
const vector<SimNode*>&
SimFFR::dss_list() const
{
  return mDSS;
}

END_NAMESPACE_YM_SEAL_CVF

#endif // CALC_CVF_SIMFFR_H
