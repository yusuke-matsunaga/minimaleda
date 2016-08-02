#ifndef TECHMAP_TECHMAPCMD_H
#define TECHMAP_TECHMAPCMD_H

/// @file magus/techmap/TechmapCmd.h
/// @brief TechmapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TechmapCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"
#include "ym_techmap/PatMgr.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_techmap/CnGraph.h"


/// @brief techmap 用の名前空間の開始
#define BEGIN_NAMESPACE_MAGUS_TECHMAP \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsTechmap)

/// @brief techmap 用の名前空間の終了
#define END_NAMESPACE_MAGUS_TECHMAP \
END_NAMESPACE(nsTechmap) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class TechmapData TechmapCmd.h "TechmapCmd.h"
/// @brief TechmapCmd に共通のデータ
//////////////////////////////////////////////////////////////////////
struct TechmapData
{
  // パタンマネージャ
  PatMgr mPatMgr;

  // サブジェクトグラフ
  SbjGraph mSbjGraph;

  // セルネットワーク
  CnGraph mCnGraph;

};


//////////////////////////////////////////////////////////////////////
/// @class TechmapCmd TechmapCmd.h "TechmapCmd.h"
/// @brief テクノロジマッピングを行うコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class TechmapCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  TechmapCmd(MagMgr* mgr,
	     TechmapData* data);

  /// @brief デストラクタ
  virtual
  ~TechmapCmd();


protected:

  /// @brief PatMgr を得る．
  PatMgr&
  pat_mgr();

  /// @brief SbjGraph を得る．
  SbjGraph&
  sbjgraph();

  /// @brief セルネットワークを得る．
  CnGraph&
  cngraph();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  TechmapData* mData;

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_TECHMAPCMD_H
