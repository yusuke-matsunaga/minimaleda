#ifndef YM_MVN_MVMODULE_H
#define YM_MVN_MVMODULE_H

/// @file ym_mvn/MvModule.h
/// @brief MvModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvModule MvModule.h "ym_mvn/MvModule.h"
/// @brief 多値ネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class MvModule
{
  friend class MvMgr;

public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部情報を得るためのメンバ関数
  /// @{

  /// @brief ID番号を得る．
  ymuint
  id() const;
  
  /// @brief 名前を得る．
  const char*
  name() const;

  /// @brief 親のノードを得る．
  /// @note トップレベルモジュールの場合には NULL を返す．
  MvNode*
  parent() const;

  /// @brief ポート数を得る．
  ymuint
  port_num() const;

  /// @brief ポートを得る．
  /// @param[in] pos 位置 ( 0 <= pos < port_num() )
  const MvPort*
  port(ymuint pos) const;

  /// @brief 入力ノード数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  MvNode*
  input(ymuint pos) const;

  /// @brief 出力ノード数を得る．
  ymuint
  output_num() const;

  /// @brief 出力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < output_num() )
  MvNode*
  output(ymuint pos) const;

  /// @brief 入出力ノード数を得る．
  ymuint
  inout_num() const;

  /// @brief 入出力ノードを得る．
  /// @param[in] pos 位置 ( 0 <= pos < inout_num() )
  MvNode*
  inout(ymuint pos) const;

  /// @brief 内部ノードの数を得る．
  ymuint
  node_num() const;

  /// @brief 内部ノードのリストを得る．
  const list<MvNode*>&
  node_list() const;

  /// @brief 内部ノードのリストの先頭の反復子を得る．
  list<MvNode*>::const_iterator
  nodes_begin() const;

  /// @brief 内部ノードのリストの末尾の反復子を得る．
  list<MvNode*>::const_iterator
  nodes_end() const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ni 入力ノード数
  /// @param[in] no 出力ノード数
  /// @param[in] nio 入出力ノード数
  MvModule(const char* name,
	   ymuint np,
	   ymuint ni,
	   ymuint no,
	   ymuint nio);

  /// @brief デストラクタ
  ~MvModule();
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 名前
  const char* mName;

  // 親のノード
  MvNode* mParent;

  // ポートの配列
  vector<MvPort*> mPortArray;
  
  // 入力ノードの配列
  vector<MvNode*> mInputArray;

  // 出力ノードの配列
  vector<MvNode*> mOutputArray;

  // 入出力ノードの配列
  vector<MvNode*> mInoutArray;

  // 内部ノードのリスト
  list<MvNode*> mNodeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
MvModule::id() const
{
  return mId;
}
  
// @brief 名前を得る．
inline
const char*
MvModule::name() const
{
  return mName;
}

// @brief 親のノードを得る．
// @note トップレベルモジュールの場合には NULL を返す．
inline
MvNode*
MvModule::parent() const
{
  return mParent;
}

// @brief ポート数を得る．
inline
ymuint
MvModule::port_num() const
{
  return mPortArray.size();
}

// @brief ポートを得る．
// @param[in] pos 位置 ( 0 <= pos < port_num() )
inline
const MvPort*
MvModule::port(ymuint pos) const
{
  return mPortArray[pos];
}

// @brief 入力ノード数を得る．
inline
ymuint
MvModule::input_num() const
{
  return mInputArray.size();
}

// @brief 入力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
inline
MvNode*
MvModule::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ノード数を得る．
inline
ymuint
MvModule::output_num() const
{
  return mOutputArray.size();
}

// @brief 出力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < output_num() )
inline
MvNode*
MvModule::output(ymuint pos) const
{
  return mOutputArray[pos];
}

// @brief 入出力ノード数を得る．
inline
ymuint
MvModule::inout_num() const
{
  return mInoutArray.size();
}

// @brief 入出力ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < inout_num() )
inline
MvNode*
MvModule::inout(ymuint pos) const
{
  return mInoutArray[pos];
}

// @brief 内部ノードの数を得る．
inline
ymuint
MvModule::node_num() const
{
  return mNodeList.size();
}

// @brief 内部ノードのリストを得る．
inline
const list<MvNode*>&
MvModule::node_list() const
{
  return mNodeList;
}

// @brief 内部ノードのリストの先頭の反復子を得る．
inline
list<MvNode*>::const_iterator
MvModule::nodes_begin() const
{
  return mNodeList.begin();
}

// @brief 内部ノードのリストの末尾の反復子を得る．
inline
list<MvNode*>::const_iterator
MvModule::nodes_end() const
{
  return mNodeList.end();
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVMODULE_H
