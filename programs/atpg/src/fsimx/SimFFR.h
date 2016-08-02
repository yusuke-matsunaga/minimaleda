#ifndef FSIMX_SIMFFR_H
#define FSIMX_SIMFFR_H

/// @file fsim/SimFFR.h
/// @brief SimFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: SimFFR.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "fsimx_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG_FSIMX

class SimNode;
class FsimFault;

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
  
  /// @brief 根のノードをセットする．
  void
  set_root(SimNode* root);
  
  /// @brief 根のノードを得る．
  SimNode*
  root() const;
  
  /// @brief この FFR に属する故障のリストを返す．
  vector<FsimFault*>&
  fault_list();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  SimNode* mRoot;
  
  // この FFR に属する故障のリスト
  vector<FsimFault*> mFaultList;
  
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

// @brief 根のノードをセットする．
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

// @brief この FFR に属する故障のリストを返す．
inline
vector<FsimFault*>&
SimFFR::fault_list()
{
  return mFaultList;
}

END_NAMESPACE_YM_ATPG_FSIMX

#endif // FSIMX_SIMFFR_H
