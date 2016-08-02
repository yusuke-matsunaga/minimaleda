
/// @file libym_lutmap/Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cut.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Cut.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

// 空のコンストラクタ
Cut::Cut() :
  mLink(NULL)
{
}

// @brief デストラクタ
Cut::~Cut()
{
}


BEGIN_NONAMESPACE

// inputs に登録されているノードを終端と見なしたときの node の表す論理式
// を返す．
LogExpr
calc_expr_for_node(const SbjNode* node,
		   const hash_map<ymuint, LogExpr>& logmap)
{
  LogExpr ans;
  if ( node == NULL ) {
    ans =  LogExpr::make_zero();
  }
  else {
    hash_map<ymuint, LogExpr>::const_iterator p = logmap.find(node->id());
    if ( p != logmap.end() ) {
      ans = p->second;
    }
    else {
      assert_cond( node->is_logic(), __FILE__, __LINE__);

      LogExpr cexp0 = calc_expr_for_node(node->fanin(0), logmap);
      LogExpr cexp1 = calc_expr_for_node(node->fanin(1), logmap);
      ymuint fcode = node->fcode();

      if ( fcode & 1U ) {
	cexp0 = ~cexp0;
      }
      if ( fcode & 2U ) {
	cexp1 = ~cexp1;
      }
      if ( fcode & 4U ) {
	ans = cexp0 ^ cexp1;
      }
      else {
	ans = cexp0 & cexp1;
      }
    }
  }
  return ans;
}

END_NONAMESPACE

// @brief 内容を表す論理式を得る．
LogExpr
Cut::expr() const
{
  if ( root() == NULL ) {
    return LogExpr::make_zero();
  }

  hash_map<ymuint, LogExpr> logmap;
  for (ymuint i = 0; i < ni(); i ++) {
    const SbjNode* node = mInputs[i];
    ymuint id = node->id();
    logmap.insert(make_pair(id, LogExpr::make_posiliteral(i)));
  }
  return calc_expr_for_node(root(), logmap);
}

// デバッグ用の表示関数
void
Cut::dump(ostream& s) const
{
  if ( root() == NULL ) {
    s << "null cut" << endl;
  }
  else {
    s << root()->id_str() << " : ";
    string comma = "";
    for (ymuint i = 0; i < ni(); i ++) {
      const SbjNode* node = mInputs[i];
      s << comma << node->id_str();
      comma = ", ";
    }
    s << " : ";
    s << expr();
    s << endl;
  }
}

END_NAMESPACE_YM_LUTMAP
