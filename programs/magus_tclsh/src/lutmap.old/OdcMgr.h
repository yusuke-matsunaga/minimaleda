#ifndef MAGUS_LUTMAP_ODCMGR_H
#define MAGUS_LUTMAP_ODCMGR_H

/// @file magus/src/lutmap/OdcMgr.h
/// @brief OdcMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: OdcMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "lutmap.h"
#include <ym_bdd/Bdd.h>
#include <ym_bnet/bnet_nsdef.h>
#include "CalcOdc.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class OdcMgr OdcMgr.h "OdcMgr.h"
/// @brief ODC を保持しておくためのクラス
class OdcMgr
{
public:

  /// @brief コンストラクタ
  OdcMgr();

  /// @brief デストラクタ
  ~OdcMgr();
  

public:
  
  /// @brief ODC の計算を行う．
  /// @param[in] bddmgr BDD マネージャ
  /// @param[in] bnetwork 対象のネットワーク
  void
  calc_odc(BddMgrRef& bddmgr,
	   const BNetwork& bnetwork);
  
  /// @brief ノードの出力の ODC を得る．
  /// @param[in] node 対象のノード
  Bdd
  output_odc(const BNode* node);
  
  /// @brief ノードの入力の ODC を得る．
  /// @param[in] node 対象のノード
  /// @param[in] pos 入力位置
  Bdd
  input_odc(const BNode* node,
	    ymuint32 pos);
  

private:
  
  // ノードごとのデータ構造
  struct NodeData {
    
    // 出力の global function
    Bdd mGf;
    
    // 出力の ODC
    Bdd mOutputOdc;

    // 入力の ODC の配列
    vector<Bdd> mInputOdc;
    
  };
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードごとのデータ
  vector<NodeData> mNodeData;
  
  // ノードの ODC を計算するためのクラス
  CalcOdc mCalcOdc;
  
};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_ODCMGR_H
