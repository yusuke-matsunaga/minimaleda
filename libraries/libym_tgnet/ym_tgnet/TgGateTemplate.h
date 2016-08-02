#ifndef YM_TGNET_TGGATETEMPLATE_H
#define YM_TGNET_TGGATETEMPLATE_H

/// @file ym_tgnet/TgGateTemplate.h
/// @brief 論理ゲートの種類を定義したファイル
/// @author Yusuke Matsunaga (松永 裕介)
/// 
/// $Id: TgGateTemplate.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tgnet/tgnet_nsdef.h"


BEGIN_NAMESPACE_YM_TGNET

//////////////////////////////////////////////////////////////////////
/// @brief 組み込み型のゲートの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tTgGateType {
  /// @brief 未定義
  kTgUndef  = 0,
  
  /// @brief 外部入力
  kTgInput  = 1,
  
  /// @brief 外部出力
  kTgOutput = 2,
  
  /// @brief 定数0
  kTgConst0 = 3,
  /// @brief 定数1
  kTgConst1 = 4,
  
  /// @brief バッファ
  kTgBuff   = 8,
  /// @brief NOT ゲート
  kTgNot    = 9,
  /// @brief AND ゲート
  kTgAnd    = 10,
  /// @brief NAND ゲート
  kTgNand   = 11,
  /// @brief OR ゲート
  kTgOr     = 12,
  /// @brief NOR ゲート
  kTgNor    = 13,
  /// @brief XOR ゲート
  kTgXor    = 14,
  /// @brief XNOR ゲート
  kTgXnor   = 15,
  
  /// @brief ユーザー定義型の先頭
  kTgUsrDef = 32
};


//////////////////////////////////////////////////////////////////////
/// @class TgGateTemplate TgGateTemplate.h <ym_tgnet/TgGateTemplate.h>
/// @brief 論理ゲートのテンプレートを表すクラス
/// @note 扱える入力数は最大で 4096
/// @note 保持できるユーザー定義論理関数の種類は約 1M 個
//////////////////////////////////////////////////////////////////////
class TgGateTemplate
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 論理関数のID
  /// @param[in] ni 入力数
  TgGateTemplate(tTgGateType type = kTgUndef,
		 size_t ni = 0);

  
public:

  /// @brief 論理関数の型を得る．
  tTgGateType
  type() const;

  /// @brief 入力数を得る．
  size_t
  ni() const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // データ
  ymuint32 mData;
  
};

/// @relates TgGateTemplate
/// @brief TgGateTemplate の内容を出力する．
ostream&
operator<<(ostream& s,
	   TgGateTemplate gt_id);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 論理関数のID
// @param[in] ni 入力数
inline
TgGateTemplate::TgGateTemplate(tTgGateType type,
			       size_t ni)
{
  mData = (static_cast<ymuint32>(type) << 12) | static_cast<ymuint32>(ni);
}

// @brief 論理関数の型を得る．
inline
tTgGateType
TgGateTemplate::type() const
{
  return static_cast<tTgGateType>(mData >> 12);
}

// @brief 入力数を得る．
inline
size_t
TgGateTemplate::ni() const
{
  return static_cast<size_t>(mData & 0xFFF);
}

END_NAMESPACE_YM_TGNET

BEGIN_NAMESPACE_YM

using nsTgnet::tTgGateType;
using nsTgnet::kTgUndef;
using nsTgnet::kTgInput;
using nsTgnet::kTgOutput;
using nsTgnet::kTgConst0;
using nsTgnet::kTgConst1;
using nsTgnet::kTgBuff;
using nsTgnet::kTgNot;
using nsTgnet::kTgAnd;
using nsTgnet::kTgNand;
using nsTgnet::kTgOr;
using nsTgnet::kTgNor;
using nsTgnet::kTgXor;
using nsTgnet::kTgXnor;
using nsTgnet::kTgUsrDef;

END_NAMESPACE_YM

#endif // YM_TGNET_TGGATETEMPLATE_H
