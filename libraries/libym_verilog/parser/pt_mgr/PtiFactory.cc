
/// @file libym_verilog/parser/PtiFactory.cc
/// @brief PtiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtiFactory.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PtiFactory.h"

#include "ym_verilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG
  
// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
PtiFactory::PtiFactory(AllocBase& alloc) :
  mAlloc(alloc)
{
}

// @brief デストラクタ
PtiFactory::~PtiFactory()
{
}

// @brief パス記述の生成
// @param[in] fr ファイル位置の情報
// @param[in] edge
// @param[in] input_array
// @param[in] input_pol
// @param[in] op
// @param[in] output_array
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
// @return 生成されたパス記述
PtPathDecl*
PtiFactory::new_PathDecl(const FileRegion& fr,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 int op,
			 PtExpr* output,
			 int output_pol,
			 PtExpr* expr,
			 PtPathDelay* path_delay)
{
  void* p = mAlloc.get_memory(sizeof(PtExpr*));
  PtExpr** array = new (p) PtExpr*[1];
  array[0] = output;
  return new_PathDecl(fr, edge, input_array, input_pol,
		      op, PtExprArray(1, array), output_pol,
		      expr, path_delay);
}
  
// @brief constant primary の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 識別子名
// @param[in] index インデックス
// @return 生成された const primary
PtExpr*
PtiFactory::new_CPrimary(const FileRegion& fr,
			 const char* name,
			 PtExpr* index)
{
  void* p = mAlloc.get_memory(sizeof(PtExpr*));
  PtExpr** array = new (p) PtExpr*[1];
  array[0] = index;
  return new_CPrimary(fr, name, PtExprArray(1, array));
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @return 生成されたイベントコントロール
PtControl*
PtiFactory::new_EventControl(const FileRegion& fr)
{
  return new_EventControl(fr, PtExprArray());
}

// @brief イベントコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] event イベント
// @return 生成されたイベントコントロール
PtControl*
PtiFactory::new_EventControl(const FileRegion& fr,
			     PtExpr* event)
{
  void* p = mAlloc.get_memory(sizeof(PtExpr*));
  PtExpr** array = new (p) PtExpr*[1];
  array[0] = event;
  return new_EventControl(fr, PtExprArray(1, array));
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @return 生成されたリピートコントロール
PtControl*
PtiFactory::new_RepeatControl(const FileRegion& fr,
			      PtExpr* expr)
{
  return new_RepeatControl(fr, expr, PtExprArray());
}

// @brief リピートコントロールの生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 繰り返し数を表す式
// @param[in] event 繰り返しの単位となるイベント
// @return 生成されたリピートコントロール
PtControl*
PtiFactory::new_RepeatControl(const FileRegion& fr,
			      PtExpr* expr,
			      PtExpr* event)
{
  void* p = mAlloc.get_memory(sizeof(PtExpr*));
  PtExpr** array = new (p) PtExpr*[1];
  array[0] = event;
  return new_RepeatControl(fr, expr, PtExprArray(1, array));
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     PtExpr* expr1)
{
  return new_Inst(fr, ConArray(expr1));
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     PtExpr* expr1,
		     PtExpr* expr2)
{
  return new_Inst(fr, ConArray(expr1, expr2));
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     PtExpr* expr1,
		     PtExpr* expr2,
		     PtExpr* expr3)
{
  return new_Inst(fr, ConArray(expr1, expr2, expr3));
}

// @brief module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_Inst(const FileRegion& fr,
		     PtExpr* expr1,
		     PtExpr* expr2,
		     PtExpr* expr3,
		     PtExpr* expr4)
{
  return new_Inst(fr, ConArray(expr1, expr2, expr3, expr4));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      PtExpr* expr1)
{
  return new_InstN(fr, name, ConArray(expr1));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      PtExpr* expr1,
		      PtExpr* expr2)
{
  return new_InstN(fr, name, ConArray(expr1, expr2));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      PtExpr* expr1,
		      PtExpr* expr2,
		      PtExpr* expr3)
{
  return new_InstN(fr, name, ConArray(expr1, expr2, expr3));
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstN(const FileRegion& fr,
		      const char* name,
		      PtExpr* expr1,
		      PtExpr* expr2,
		      PtExpr* expr3,
		      PtExpr* expr4)
{
  return new_InstN(fr, name, ConArray(expr1, expr2, expr3, expr4));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      PtExpr* left,
		      PtExpr* right,
		      PtExpr* expr1)
{
  return new_InstV(fr, name, left, right, ConArray(expr1));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      PtExpr* left,
		      PtExpr* right,
		      PtExpr* expr1,
		      PtExpr* expr2)
{
  return new_InstV(fr, name, left, right, ConArray(expr1, expr2));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      PtExpr* left,
		      PtExpr* right,
		      PtExpr* expr1,
		      PtExpr* expr2,
		      PtExpr* expr3)
{
  return new_InstV(fr, name, left, right, ConArray(expr1, expr2, expr3));
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] expr1, expr2, expr3, expr4 ポート割り当て
// @return 生成された module instance/UDP instance の要素
PtInst*
PtiFactory::new_InstV(const FileRegion& fr,
		      const char* name,
		      PtExpr* left,
		      PtExpr* right,
		      PtExpr* expr1,
		      PtExpr* expr2,
		      PtExpr* expr3,
		      PtExpr* expr4)
{
  return new_InstV(fr, name, left, right, ConArray(expr1, expr2, expr3, expr4));
}
  
// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
PtConnectionArray
PtiFactory::ConArray(PtExpr* expr1)
{
  void* p = mAlloc.get_memory(sizeof(PtConnection*));
  PtConnection** array = new (p) PtConnection*[1];
  array[0] = new_OrderedCon(expr1);
  return PtConnectionArray(1, array);
}
  
// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
// @param[in] expr2 結合する式
PtConnectionArray
PtiFactory::ConArray(PtExpr* expr1,
		     PtExpr* expr2)
{
  void* p = mAlloc.get_memory(sizeof(PtConnection*) * 2);
  PtConnection** array = new (p) PtConnection*[2];
  array[0] = new_OrderedCon(expr1);
  array[1] = new_OrderedCon(expr2);
  return PtConnectionArray(2, array);
}
  
// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
// @param[in] expr2 結合する式
// @param[in] expr3 結合する式
PtConnectionArray
PtiFactory::ConArray(PtExpr* expr1,
		     PtExpr* expr2,
		     PtExpr* expr3)
{
  void* p = mAlloc.get_memory(sizeof(PtConnection*) * 3);
  PtConnection** array = new (p) PtConnection*[3];
  array[0] = new_OrderedCon(expr1);
  array[1] = new_OrderedCon(expr2);
  array[2] = new_OrderedCon(expr3);
  return PtConnectionArray(3, array);
}
  
// @brief 順序付き結合子の配列の生成
// @param[in] expr1 結合する式
// @param[in] expr2 結合する式
// @param[in] expr3 結合する式
// @param[in] expr4 結合する式
PtConnectionArray
PtiFactory::ConArray(PtExpr* expr1,
		     PtExpr* expr2,
		     PtExpr* expr3,
		     PtExpr* expr4)
{
  void* p = mAlloc.get_memory(sizeof(PtConnection*) * 4);
  PtConnection** array = new (p) PtConnection*[4];
  array[0] = new_OrderedCon(expr1);
  array[1] = new_OrderedCon(expr2);
  array[2] = new_OrderedCon(expr3);
  array[3] = new_OrderedCon(expr4);
  return PtConnectionArray(4, array);
}

// @brief メモリアロケータを得る．
AllocBase&
PtiFactory::alloc()
{
  return mAlloc;
}

END_NAMESPACE_YM_VERILOG
