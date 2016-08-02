#ifndef YM_BNET_BNETMANIP_H
#define YM_BNET_BNETMANIP_H

/// @file ym_bnet/BNetManip.h
/// @brief BNetManip のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetManip.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNetwork.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetManip BNetManip.h <ym_bnet/BNetManip.h>
/// @ingroup BnetGroup
/// @brief BNetwork を操作するクラス
///
/// BNetwork は基本的に read-only メソッドしか持たないので
/// 内容を変更するためにはこのクラスを経由する．
//////////////////////////////////////////////////////////////////////
class BNetManip
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/初期化
  /// @{

  /// @brief コンストラクタ．
  /// @param[in] network 操作対象のネットワーク
  BNetManip(BNetwork* network);
  
  /// @brief デストラクタ
  virtual
  ~BNetManip();

  /// @}
  //////////////////////////////////////////////////////////////////////
  

public:
  //////////////////////////////////////////////////////////////////////
  /// @name データを設定/変更するための関数
  /// @{
  
  /// @brief 外部入力節点を追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  BNode*
  new_input(const char* name);
  
  /// @brief 外部入力節点を追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  BNode*
  new_input(const string& name);

  /// @brief 外部出力節点を追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  /// @note 初期状態では入力数0になっている．
  /// その後，ChgOutput() を呼ぶことで適切に入力がセットされる．
  /// @note 外部出力は外部入力/中間節点とは別の名前空間に属するが，
  /// 外部入力と同じ名前はエラーとなる．
  BNode*
  new_output(const char* name);

  /// @brief 外部出力節点を追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  /// @note 初期状態では入力数0になっている．
  /// その後，ChgOutput() を呼ぶことで適切に入力がセットされる．
  /// @note 外部出力は外部入力/中間節点とは別の名前空間に属するが，
  /// 外部入力と同じ名前はエラーとなる．
  BNode*
  new_output(const string& name);
  
  /// @brief 中間節点を追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  /// @note 入力数無しの定数0節点の状態で初期化される．
  BNode*
  new_logic(const char* name = NULL);
  
  /// @brief 中間節点を追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  /// @note 入力数無しの定数0節点の状態で初期化される．
  BNode*
  new_logic(const string& name);

  /// @brief latch ノードを追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  /// @note ファンインがつながっていない初期値0の状態で初期化される．
  BNode*
  new_latch(const char* name = NULL);

  /// @brief latch ノードを追加する．
  /// @param[in] name 名前
  /// @retval 生成されたノード
  /// @retval NULL 名前が重複していた場合(エラー)
  /// @note ファンインがつながっていない初期値0の状態で初期化される．
  BNode*
  new_latch(const string& name);
  
  /// @brief ノードの消去(eliminate)を行う．
  /// @param[in] node 消去対象のノード
  /// @note node がファンアウトしているノードの論理式の中に
  /// node の論理式が含まれるように変更される．
  /// @note node が外部出力につながっている場合には，
  /// true バッファの場合のみファンイン元のノードのつなぎ替えを行なう．
  /// @note いかなる場合でも node そのものは削除されない．
  void
  eliminate_node(BNode* node);

  /// @brief ノードの削除を行う．
  /// @param[in] node 削除対象のノード
  /// @retval true 削除が成功した．
  /// @retval false 削除が失敗した．
  /// @note 削除が失敗するのは以下の原因
  ///  - nodeがこのネットワークに属していない場合
  ///  - nodeが他の節点にファンアウトしている場合(外部出力節点から参照されて
  ///    いる場合も含む)
  bool
  delete_node(BNode* node);

  /// @brief ノード名の変更を行う．
  /// @param[in] node 設定対象のノード
  /// @param[in] name 新しい名前
  /// @retval true 設定変更が成功した．
  /// @retval false 名前が重複しており設定変更が失敗した．
  /// @note name が空の場合には自動的に新しい名前が生成される．
  /// (これはかなり特殊な使い方)
  bool
  rename_node(BNode* node,
	      const char* name);

  /// @brief ノード名の変更を行う．
  /// @param[in] node 設定対象のノード
  /// @param[in] name 新しい名前
  /// @retval true 設定変更が成功した．
  /// @retval false 名前が重複しており設定変更が失敗した．
  /// @note name が空の場合には自動的に新しい名前が生成される．
  /// (これはかなり特殊な使い方)
  bool
  rename_node(BNode* node,
	      const string& name);

  /// @brief ノードの置き換え
  /// @param[in] old_node 置き換え対象の元のノード
  /// @param[in] new_node 置き換え後の新しいノード
  /// @retval true 置き換えが成功した．
  /// @retval false 置き換えが失敗した．
  /// @note old_node のファンアウト先のノードのファンインを
  /// new_node に置き換える．
  /// @note old_node のファンアウト数は0になるが削除はされない．
  /// @note old_node や new_node がこのネットワークに属していない
  /// 場合にはエラーとなり，false を返す．
  /// @note old_node が外部出力の場合(もともファンアウトがないので)，
  /// なにも起きない．
  /// @note new_node が外部出力の場合，そのファンインのノードを用いて
  /// 置き換えを行う．
  bool
  replace_node(BNode* old_node,
	       BNode* new_node);

  /// @brief 外部出力の置き換え
  /// @param[in] onode 操作対象の外部出力ノード
  /// @param[in] node 新しいファンインノード
  /// @retval true 置き換えが成功した．
  /// @retval false 置き換えが失敗した．
  /// @note 外部出力節点 onode のファンインを node に変更する．
  /// @note 置き換えが失敗するのは node が他のネットワークに属
  /// している場合
  bool
  change_output(BNode* onode,
		BNode* node);
  
  /// @brief 中間ノードの変更
  /// @param[in] node 対象のノード
  /// @param[in] expr 新しい論理式
  /// @param[in] fanins 新しいファンインのベクタ
  /// @param[in] tfo_check fanins が node の TFO でないことをチェックする．
  /// @retval true 変更が成功した．
  /// @retval false 変更が失敗した．
  /// @note fanins のノードは論理式中で変数の表れる順に対応している．
  /// @note 失敗するのは以下の場合
  ///  - fanins に含まれるノードがこのネットワークに属していない場合
  ///  - 論理式中に現われる変数の数と fanins の要素数が異なる場合
  /// @note fanins の中に同一のノードが二回以上現われてもエラーとはならない．　
  /// その場合は同一の節点は一つのファンインになるように論理式が修正される．
  /// ただし，簡単化しているわけではないので，冗長な場合もある．
  bool
  change_logic(BNode* node,
	       const LogExpr& expr,
	       const BNodeVector& fanins,
	       bool tfo_check = true);

  /// @brief 中間ノードの変更
  /// @param[in] node 対象のノード
  /// @param[in] expr 新しい論理式
  /// @retval true 変更が成功した．
  /// @retval false 変更が失敗した．
  /// @note ファンインは現在のものをそのまま用いる．
  /// @note 失敗するのは以下の場合
  ///  - 論理式中に現われる変数の数とファンイン数が異なる場合
  bool
  change_logic(BNode* node,
	       const LogExpr& expr);

  /// @brief latch ノードの変更
  /// @param[in] node 対象のノード
  /// @param[in] fanin ファンインのノード
  /// @param[in] reset_value 初期値
  /// @retval true 変更が成功した．
  /// @retval false 変更が失敗した．
  /// @note 失敗するのは以下の場合
  ///  - fanin がこのネットワークに属していない場合
  bool
  change_latch(BNode* node,
	       BNode* fanin,
	       int reset_value);

  /// @}
  //////////////////////////////////////////////////////////////////////


  
  //////////////////////////////////////////////////////////////////////
  /// @name データを設定/変更するための関数(その2)
  /// 上記の関数を組み合わせた簡便関数
  /// @{
  
  /// @brief 定数ノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] cval 0の時，定数0ノードに設定する．
  /// 非0の時，定数1ノードに設定する．
  /// @retval true 変更が成功した．
  bool
  change_to_const(BNode* node,
		  int cval);
  
  /// @brief バッファ(恒等関数)ノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] src_node 入力ノード
  /// @retval true 変更が成功した．
  /// @note node を src_node を入力としたバッファに変更する．
  bool
  change_to_buffer(BNode* node,
		   BNode* src_node);

  /// @brief インバータ(NOT)ノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] src_node 入力ノード
  /// @retval true 変更が成功した．
  /// @note node を src_node を入力としたインバータに変更する．
  bool
  change_to_inverter(BNode* node,
		     BNode* src_node);

  /// @brief ANDノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval true 変更が成功した．
  /// @note node を fanins を入力としたANDノードに変更する．
  bool
  change_to_and(BNode* node,
		const BNodeVector& fanins);

  /// @brief NANDノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval true 変更が成功した．
  /// @note node を fanins を入力としたNANDノードに変更する．
  bool
  change_to_nand(BNode* node,
		 const BNodeVector& fanins);
  
  /// @brief ORノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval true 変更が成功した．
  /// @note node を fanins を入力としたORノードに変更する．
  bool
  change_to_or(BNode* node,
	       const BNodeVector& fanins);
  
  /// @brief NORノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval true 変更が成功した．
  /// @note node を fanins を入力としたNORノードに変更する．
  bool
  change_to_nor(BNode* node,
		const BNodeVector& fanins);
  
  /// @brief XORノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval true 変更が成功した．
  /// @note node を fanins を入力としたXORノードに変更する．
  bool
  change_to_xor(BNode* node,
		const BNodeVector& fanins);
  
  /// @brief XNORノードへの変更
  /// @param[in] node 対象のノード
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval true 変更が成功した．
  /// @note node を fanins を入力としたXNORノードに変更する．
  bool
  change_to_xnor(BNode* node,
		 const BNodeVector& fanins);
  
  /// @brief 定数ノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] cval 0の時，定数0ノードを生成する．
  ///  非0の時，定数1ノードを生成する．
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複などでノードが生成されなかった場合
  BNode*
  make_const(int cval,
	     const char* dst_name = NULL);
  
  /// @brief 定数ノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] cval 0の時，定数0ノードを生成する．
  ///  非0の時，定数1ノードを生成する．
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複などでノードが生成されなかった場合
  BNode*
  make_const(int cval,
	     const string& dst_name);

  /// @brief バッファノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] src_node 入力のノード
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複などでノードが生成されなかった場合
  /// @note src_node を入力とした，dst_name という名前のバッファ
  /// ノードを生成する．
  BNode*
  make_buffer(BNode* src_node,
	      const char* dst_name = NULL);

  /// @brief バッファノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] src_node 入力のノード
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複などでノードが生成されなかった場合
  /// @note src_node を入力とした，dst_name という名前のバッファ
  /// ノードを生成する．
  BNode*
  make_buffer(BNode* src_node,
	      const string& dst_name);
  
  /// @brief インバータノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] src_node 入力のノード
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複などでノードが生成されなかった場合
  /// @note src_node を入力とした，dst_name という名前のインバータ
  /// ノードを生成する．
  BNode*
  make_inverter(BNode* src_node,
		const char* dst_name = NULL);
  
  /// @brief インバータノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] src_node 入力のノード
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複などでノードが生成されなかった場合
  /// @note src_node を入力とした，dst_name という名前のインバータ
  /// ノードを生成する．
  BNode*
  make_inverter(BNode* src_node,
		const string& dst_name);

  /// @brief ANDノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のANDノード
  ///  を生成する．
  BNode*
  make_and(const BNodeVector& fanins,
	   const char* dst_name = NULL);

  /// @brief ANDノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のANDノード
  ///  を生成する．
  BNode*
  make_and(const BNodeVector& fanins,
	   const string& dst_name);

  /// @brief NANDノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のNANDノード
  ///  を生成する．
  BNode*
  make_nand(const BNodeVector& fanins,
	    const char* dst_name = NULL);

  /// @brief NANDノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のNANDノード
  ///  を生成する．
  BNode*
  make_nand(const BNodeVector& fanins,
	    const string& dst_name);
  
  /// @brief ORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のORノード
  ///  を生成する．
  BNode*
  make_or(const BNodeVector& fanins,
	  const char* dst_name = NULL);
  
  /// @brief ORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のORノード
  ///  を生成する．
  BNode*
  make_or(const BNodeVector& fanins,
	  const string& dst_name);
  
  /// @brief NORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のNORノード
  ///  を生成する．
  BNode*
  make_nor(const BNodeVector& fanins,
	   const char* dst_name = NULL);
  
  /// @brief NORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のNORノード
  ///  を生成する．
  BNode*
  make_nor(const BNodeVector& fanins,
	   const string& dst_name);
  
  /// @brief XORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のXORノード
  ///  を生成する．
  BNode*
  make_xor(const BNodeVector& fanins,
	   const char* dst_name = NULL);
  
  /// @brief XORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のXORノード
  ///  を生成する．
  BNode*
  make_xor(const BNodeVector& fanins,
	   const string& dst_name);
  
  /// @brief XNORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のXNORノード
  ///  を生成する．
  BNode*
  make_xnor(const BNodeVector& fanins,
	    const char* dst_name = NULL);
  
  /// @brief XNORノードの生成
  /// @param[in] dst_name 生成するノード名
  /// @param[in] fanins 入力ノードのベクタ
  /// @retval 生成されたノード
  /// @retval NULL 名前の重複や fanins の不備などでノードが生成
  ///  されなかった場合
  /// @note fanins を入力とした，dst_name という名前のXNORノード
  ///  を生成する．
  BNode*
  make_xnor(const BNodeVector& fanins,
	    const string& dst_name);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name BNetwork の同名の関数を呼び出すラッパ関数
  /// @{
  
  /// @brief BNetwork そのものを得る．
  BNetwork*
  network();
  
  /// @brief 入力数を得る．
  size_t
  input_num();
  
  /// @brief 入力ノードのリストを返す．
  const BNodeList&
  inputs();
  
  /// @brief 入力ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  inputs_begin();
  
  /// @brief 入力ノードのリストの末尾の反復子を返す．
  BNodeList::const_iterator
  inputs_end();
  
  /// @brief 出力数を得る．
  size_t
  output_num();
  
  /// @brief 出力ノードのリストを返す．
  const BNodeList&
  outputs();
  
  /// @brief 出力ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  outputs_begin();
  
  /// @brief 出力ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  outputs_end();
  
  /// @brief 中間節点の数を得る．
  size_t
  logic_node_num();
  
  /// @brief 中間節点のリストを返す．
  const BNodeList&
  logic_nodes();
  
  /// @brief 中間節点のリストの先頭の反復子を返す．
  BNodeList::const_iterator
  logic_nodes_begin();
  
  /// @brief 中間節点のリストの末尾の反復子を返す．
  BNodeList::const_iterator
  logic_nodes_end();

  /// @brief latch ノードの数を得る．
  size_t
  latch_node_num();

  /// @brief latch ノードのリストを返す．
  const BNodeList&
  latch_nodes();

  /// @brief latch ノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  latch_nodes_begin();

  /// @brief latch ノードのリストの末尾の反復子を返す．
  BNodeList::const_iterator
  latch_nodes_end();
  
  /// @brief nameという名前をもつノードを探す．
  /// @param[in] name 探索対象の名前
  /// @retval name という名前を持つノード
  /// @retval NULL 該当するノードがない場合
  BNode*
  find_node(const char* name);
  
  /// @brief name という名前を持つ外部出力節点を取出す．
  /// @param[in] name 探索対象の名前
  /// @retval name という名前を持つ外部出力ノード
  /// @retval NULL 該当する節点が無い場合
  BNode*
  find_ponode(const char* name);
  
  /// @brief すべてのノード数を得る．
  size_t
  node_num();
  
  /// @brief ノードの id 番号の最大値 + 1 を得る．
  size_t
  max_node_id();
  
  /// @brief すべてのノードのリストを返す．
  const BNodeList&
  nodes();
  
  /// @brief すべてのノードのリストの先頭の反復子を返す．
  BNodeList::const_iterator
  nodes_begin();
  
  /// @brief すべてのノードのリストの末尾の反復子を返す．
  BNodeList::const_iterator
  nodes_end();
  
  /// @brief ID番号からノードを取出す．
  BNode*
  node(BNode::tId id);

  /// @brief 入力からのトポロジカルソート順のノードベクタを取り出す．
  void
  tsort(BNodeVector& node_list);

  /// @brief ノード名を取り出す．
  const char*
  node_name(const BNode* node);
  
  /// @brief 全部のリテラル数を得る．
  size_t
  litnum();
  
  /// @brief SOP形式のリテラル数の見積りを得る．
  /// @note Boolean factored formの場合には実際よりも
  /// 多めの値になる場合がある．
  size_t
  sop_litnum();

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////
  
  // TFO にマークをつける．
  void
  mark_TFO(BNode* node);

  // TFO のマークを消す．
  void
  clear_TFO(BNode* node);

  // TFO マークを得る．
  bool
  get_TFO(BNode* node);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  BNetwork* mNetwork;

  // 作業領域として用いるノード配列
  vector<BNode*> mTmpNodes;

  // 作業領域として用いる整数配列
  vector<int> mTmpPos;

  // 論理式の入力変数付け替え用の作業領域
  VarVarMap mTmpMap;

  // TFO チェックで用いる作業領域
  vector<int> mTmpMark;

  // TFO チェック用のカウンタ
  int mCurTmpMark;
  
};



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 外部入力節点を追加する．
// 通常は新たに生成された節点を返すが，名前が重複していた場合には
// 節点を作らずに NULL を返す(エラーとなる)．
inline
BNode*
BNetManip::new_input(const string& name)
{
  return new_input(name.c_str());
}

// name という名前を持つ外部出力節点を作り登録する．
// そのときに作られた外部出力節点を返す．
// 初期状態では入力数0になっている．
// その後，ChgOutput() を呼ぶことで適切に入力がセットされる．
// 同一の名前を持つ節点が既に存在すればエラーとなる(NULLを返す)．
inline
BNode*
BNetManip::new_output(const string& name)
{
  return new_output(name.c_str());
}

// 中間節点を追加する．
// そのときに作られた中間出力節点を返す．
// 入力数無しの定数0節点の状態で初期化される．
// 名前が重複していたらエラーとなり，NULLを返す．
inline
BNode*
BNetManip::new_logic(const string& name)
{
  return new_logic(name.c_str());
}

// latch 節点を追加する．
// そのときに作られた中間節点を返す．
// ファンインが未接続で，初期値0の状態でえ初期化される．
// 名前が重複していたらエラーとなり，NULLを返す．
inline
BNode*
BNetManip::new_latch(const string& name)
{
  return new_latch(name.c_str());
}

// @brief ノード名の変更を行う．
inline
bool
BNetManip::rename_node(BNode* node,
		       const string& name)
{
  return rename_node(node, name.c_str());
}
  
// @brief 定数ノードの生成
inline
BNode*
BNetManip::make_const(int cval,
		      const string& dst_name)
{
  return make_const(cval, dst_name.c_str());
}

// @brief バッファノードの生成
inline
BNode*
BNetManip::make_buffer(BNode* src_node,
		       const string& dst_name)
{
  return make_buffer(src_node, dst_name.c_str());
}

// @brief インバータノードの生成
inline
BNode*
BNetManip::make_inverter(BNode* src_node,
			 const string& dst_name)
{
  return make_inverter(src_node, dst_name.c_str());
}

// @brief ANDノードの生成
inline
BNode*
BNetManip::make_and(const BNodeVector& fanins,
		    const string& dst_name)
{
  return make_and(fanins, dst_name.c_str());
}

// @brief NANDノードの生成
inline
BNode*
BNetManip::make_nand(const BNodeVector& fanins,
		     const string& dst_name)
{
  return make_nand(fanins, dst_name.c_str());
}

// @brief ORノードの生成
inline
BNode*
BNetManip::make_or(const BNodeVector& fanins,
		   const string& dst_name)
{
  return make_or(fanins, dst_name.c_str());
}

// @brief NORノードの生成
inline
BNode*
BNetManip::make_nor(const BNodeVector& fanins,
		    const string& dst_name)
{
  return make_nor(fanins, dst_name.c_str());
}

// @brief XORノードの生成
inline
BNode*
BNetManip::make_xor(const BNodeVector& fanins,
		    const string& dst_name)
{
  return make_xor(fanins, dst_name.c_str());
}

// @brief XNORノードの生成
inline
BNode*
BNetManip::make_xnor(const BNodeVector& fanins,
		     const string& dst_name)
{
  return make_xnor(fanins, dst_name.c_str());
}
  
// @brief BNetwork そのものを得る．
inline
BNetwork*
BNetManip::network()
{
  return mNetwork;
}
    
// @brief 入力数を得る．
inline
size_t
BNetManip::input_num()
{
  return mNetwork->input_num();
}
  
// @brief 入力ノードのリストを返す．
inline
const BNodeList&
BNetManip::inputs()
{
  return mNetwork->inputs();
}
  
// @brief 入力ノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::inputs_begin()
{
  return mNetwork->inputs_begin();
}
  
// @brief 入力ノードのリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::inputs_end()
{
  return mNetwork->inputs_end();
}
  
// @brief 出力数を得る．
inline
size_t
BNetManip::output_num()
{
  return mNetwork->output_num();
}
  
// @brief 出力ノードのリストを返す．
inline
const BNodeList&
BNetManip::outputs()
{
  return mNetwork->outputs();
}
  
// @brief 出力ノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::outputs_begin()
{
  return mNetwork->outputs_begin();
}
  
// @brief 出力ノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::outputs_end()
{
  return mNetwork->outputs_end();
}
  
// @brief 中間節点の数を得る．
inline
size_t
BNetManip::logic_node_num()
{
  return mNetwork->logic_node_num();
}
  
// @brief 中間節点のリストを返す．
inline
const BNodeList&
BNetManip::logic_nodes()
{
  return mNetwork->logic_nodes();
}
  
// @brief 中間節点のリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::logic_nodes_begin()
{
  return mNetwork->logic_nodes_begin();
}
  
// @brief 中間節点のリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::logic_nodes_end()
{
  return mNetwork->logic_nodes_end();
}

// @brief latch ノードの数を得る．
inline
size_t
BNetManip::latch_node_num()
{
  return mNetwork->latch_node_num();
}

// @brief latch ノードのリストを返す．
inline
const BNodeList&
BNetManip::latch_nodes()
{
  return mNetwork->latch_nodes();
}

// @brief latch ノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::latch_nodes_begin()
{
  return mNetwork->latch_nodes_begin();
}

// @brief latch ノードのリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::latch_nodes_end()
{
  return mNetwork->latch_nodes_end();
}
  
// @brief nameという名前をもつノードを探す．
// @param[in] name 探索対象の名前
// @retval name という名前を持つノード
// @retval NULL 該当するノードがない場合
inline
BNode*
BNetManip::find_node(const char* name)
{
  return mNetwork->find_node(name);
}
  
// @brief name という名前を持つ外部出力節点を取出す．
// @param[in] name 探索対象の名前
// @retval name という名前を持つ外部出力ノード
// @retval NULL 該当する節点が無い場合
inline
BNode*
BNetManip::find_ponode(const char* name)
{
  return mNetwork->find_ponode(name);
}
  
// @brief すべてのノード数を得る．
inline
size_t
BNetManip::node_num()
{
  return mNetwork->node_num();
}
  
// @brief ノードの id 番号の最大値 + 1 を得る．
inline
size_t
BNetManip::max_node_id()
{
  return mNetwork->max_node_id();
}
  
// @brief すべてのノードのリストを返す．
inline
const BNodeList&
BNetManip::nodes()
{
  return mNetwork->nodes();
} 
  
// @brief すべてのノードのリストの先頭の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::nodes_begin()
{
  return mNetwork->nodes_begin();
}
  
// @brief すべてのノードのリストの末尾の反復子を返す．
inline
BNodeList::const_iterator
BNetManip::nodes_end()
{
  return mNetwork->nodes_end();
}
  
// @brief ID番号からノードを取出す．
inline
BNode*
BNetManip::node(BNode::tId id)
{
  return mNetwork->node(id);
}

// @brief 入力からのトポロジカルソート順のノードベクタを取り出す．
inline
void
BNetManip::tsort(BNodeVector& node_list)
{
  return mNetwork->tsort(node_list);
}

// @brief ノード名を取り出す．
inline
const char*
BNetManip::node_name(const BNode* node)
{
  return mNetwork->node_name(node);
}
  
// @brief 全部のリテラル数を得る．
inline
size_t
BNetManip::litnum()
{
  return mNetwork->litnum();
}
  
// @brief SOP形式のリテラル数の見積りを得る．
// @note Boolean factored formの場合には実際よりも
// 多めの値になる場合がある．
inline
size_t
BNetManip::sop_litnum()
{
  return mNetwork->sop_litnum();
}

END_NAMESPACE_YM_BNET

#endif // YM_BNET_BNETMANIP_H
