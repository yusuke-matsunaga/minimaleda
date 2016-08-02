#ifndef LIBYM_VERILOG_ELABORATOR_ELBPARAMCON_H
#define LIBYM_VERILOG_ELABORATOR_ELBPARAMCON_H

/// @file libym_verilog/elaborator/ElbParamCon.h
/// @brief ElbParamCon のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParamCon.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/FileRegion.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

class ElbExpr;

//////////////////////////////////////////////////////////////////////
/// @class ElbParamCon ElbParamCon.h "ElbParamCon.h"
/// @brief インスタンス化で用いる stub クラス
//////////////////////////////////////////////////////////////////////
class ElbParamCon
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル位置
  /// @param[in] num 要素数
  /// @param[in] named_con 名前による割り当ての時 true となるフラグ
  ElbParamCon(const FileRegion& file_region,
	      ymuint32 num,
	      bool named_con);

  /// @brief 仮想デストラクタ
  virtual
  ~ElbParamCon();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部からアクセスするための関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief ファイル位置を返す．
  const FileRegion&
  file_region() const;

  /// @brief 名前による割り当ての時 true を返す．
  bool
  named_con() const;
  
  /// @brief 要素数を返す．
  ymuint32
  elem_num() const;

  /// @brief pos 番目の要素に対応するパース木の要素を返す．
  /// @param[in] pos 位置
  const PtConnection*
  pt_con(ymuint32 pos) const;
  
  /// @brief pos 番目の要素の名前を返す．
  /// @param[in] pos 位置
  const char*
  name(ymuint32 pos) const;

  /// @brief pos 番目の要素の値を返す．
  /// @param[in] pos 位置
  ElbExpr*
  expr(ymuint32 pos) const;
  
  /// @brief 名前と値を設定する．
  void
  set(ymuint32 pos,
      const PtConnection* pt_con,
      ElbExpr* expr);
  

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使うデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Unit {
    
    // パース木の要素
    const PtConnection* mPtCon;

    // 値
    ElbExpr* mExpr;
    
  };
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ファイル位置
  FileRegion mFileRegion;

  // 名前による割り当ての時 true となるフラグ
  bool mNamedCon;
  
  // 要素のベクタ
  vector<Unit> mList;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル位置
// @param[in] num 要素数
/// @param[in] named_con 名前による割り当ての時 true となるフラグ
inline
ElbParamCon::ElbParamCon(const FileRegion& file_region,
			 ymuint32 num,
			 bool named_con) :
  mFileRegion(file_region),
  mNamedCon(named_con),
  mList(num)
{
}

// @brief 仮想デストラクタ
inline
ElbParamCon::~ElbParamCon()
{
}

// @brief ファイル位置を返す．
inline
const FileRegion&
ElbParamCon::file_region() const
{
  return mFileRegion;
}

// @brief 名前による割り当ての時 true を返す．
inline
bool
ElbParamCon::named_con() const
{
  return mNamedCon;
}
  
// @brief 要素数を返す．
inline
ymuint32
ElbParamCon::elem_num() const
{
  return mList.size();
}

// @brief pos 番目の要素に対応するパース木の要素を返す．
// @param[in] pos 位置
inline
const PtConnection*
ElbParamCon::pt_con(ymuint32 pos) const
{
  return mList[pos].mPtCon;
}

// @brief 名前を返す．
inline
const char*
ElbParamCon::name(ymuint32 pos) const
{
  return mList[pos].mPtCon->name();
}

// @brief 値を返す．
inline
ElbExpr*
ElbParamCon::expr(ymuint32 pos) const
{
  return mList[pos].mExpr;
}
  
// @brief 名前と値を設定する．
inline
void
ElbParamCon::set(ymuint32 pos,
		 const PtConnection* pt_con,
  		 ElbExpr* expr)
{
  Unit& unit = mList[pos];
  unit.mPtCon = pt_con;
  unit.mExpr = expr;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ELBPARAMCON_H
