#ifndef LIBYM_BNET_STRBNODEMAP_H
#define LIBYM_BNET_STRBNODEMAP_H

/// @file libym_bnet/StrBNodeMap.h
/// @brief StrBNodeMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: StrBNodeMap.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/bnet_nsdef.h"
#include "ym_bnet/BNetwork.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class StrBNodeMap StrBNodeMap.h "StrBNodeMap.h"
/// @brief 名前をキーにして BNode 番号を格納する連想配列
//////////////////////////////////////////////////////////////////////
class StrBNodeMap
{
public:

  /// @brief コンストラクタ
  StrBNodeMap();

  /// @brief デストラクタ
  ~StrBNodeMap();


public:

  /// @brief 名前を登録する．
  /// @param[in] name 登録する名前
  /// @param[in] id 登録するノード番号
  /// @retval true 登録が成功した．
  /// @retval false 同名のノードが既に存在していた．
  bool
  reg_name(const char* name,
	   BNode::tId id);

  /// @brief 名前を削除する．
  void
  del_name(const char* name);

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 名前からノード番号を取り出す．
  BNode::tId
  find_id(const char* name) const;


private:

  // ハッシュ表の一つの要素
  struct Cell
  {
    // 名前
    const char* mName;
    // ノード番号
    BNode::tId mId;
    // 次の要素
    Cell* mLink;
  };


private:

  // ハッシュ表の領域を確保する．
  void
  new_table(size_t req_size);

  // Cell を取ってくる．
  Cell*
  new_cell();

  // Cell を未使用にする．
  void
  free_cell(Cell* cell);
  
  // cell の名前が name に等しければ true を返す．
  static
  bool
  check_name(Cell* cell,
	     const char* name);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell 用のアロケータ
  SimpleAlloc mAlloc;

  // 未使用の Cell の先頭
  Cell* mAvail;
  
  // ハッシュ表の大きさ
  size_t mTableSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表に登録されているノード数
  size_t mNum;

  // ハッシュ表を拡大する基準値
  size_t mNextLimit;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// cell の名前が name に等しければ true を返す．
inline
bool
StrBNodeMap::check_name(Cell* cell,
			const char* name)
{
  const char* name1 = cell->mName;
  return name[0] == name1[0] && strcmp(name1, name) == 0;
}

END_NAMESPACE_YM_BNET

#endif // LIBYM_BNET_STRBNODEMAP_H
