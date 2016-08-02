
/// @file magus/lutmap/Cut.cc 
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cut.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "Cut.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

// 空のコンストラクタ
Cut::Cut() :
  mLink(NULL)
{
}

// @brief デストラクタ
Cut::~Cut()
{
}

// @brief 内部に fo-mark の付いたノードを含んでいたら true を返す．
bool
Cut::fo_check() const
{
  for (ymuint i = 0; i < ni(); ++ i) {
    SbjNode* node = input(i);
    node->set_tmark();
  }
  bool ans = fo_check_sub(root());
  for (ymuint i = 0; i < ni(); ++ i) {
    SbjNode* node = input(i);
    node->clear_tmark();
  }
  return ans;
}

// fo_check のサブルーティン
bool
Cut::fo_check_sub(SbjNode* node) const
{
  for ( ; ; ) {
    if ( node->tmark() ) {
      // node は cut の葉だった．
      return false;
    }
    if ( node->is_fo() && node != root() ) {
      return true;
    }
    SbjNode* inode0 = node->fanin(0);
    if ( fo_check_sub(inode0) ) {
      return true;
    }
    node = node->fanin(1);
  }
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

      // 単純なコード
      switch ( fcode ) {
      case 0x0: // 0000
      case 0x3: // 0011
      case 0x5: // 0101
      case 0xa: // 1010
      case 0xc: // 1100
      case 0xf: // 1111
	assert_not_reached(__FILE__, __LINE__);
	break;
      case 0x1: // 0001
	ans = ~cexp0 & ~cexp1;
	break;
      case 0x2: // 0010
	ans = cexp0 & ~cexp1;
	break;
      case 0x4: // 0100
	ans = ~cexp0 & cexp1;
	break;
      case 0x6: // 0110
	ans = cexp0 ^ cexp1;
	break;
      case 0x7: // 0111
	ans = ~cexp0 | ~cexp1;
	break;
      case 0x8: // 1000
	ans = cexp0 & cexp1;
	break;
      case 0x9: // 1001
	ans = ~cexp0 ^ cexp1;
	break;
      case 0xb: // 1011
	ans = cexp0 | ~cexp1;
	break;
      case 0xd: // 1101
	ans = ~cexp0 | cexp1;
	break;
      case 0xe: // 1110
	ans = cexp0 | cexp1;
	break;
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

END_NAMESPACE_MAGUS_LUTMAP
