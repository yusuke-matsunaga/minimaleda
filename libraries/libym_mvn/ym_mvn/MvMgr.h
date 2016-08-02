#ifndef YM_MVN_MVMGR_H
#define YM_MVN_MVMGR_H

/// @file ym_mvn/MvMgr.h
/// @brief MvMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_utils/ItvlMgr.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvMgr MvMgr.h "ym_mvn/MvMgr.h"
/// @brief 多値ネットワークの生成/設定を行うクラス
//////////////////////////////////////////////////////////////////////
class MvMgr
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvMgr();

  /// @brief デストラクタ
  ~MvMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルモジュールのリストを得る．
  /// @param[out] module_list モジュールを格納するリスト
  /// @return 要素数を返す．
  /// @note この関数はトップモジュール数ではなく全モジュール数に比例した
  /// 実行時間を要する．
  ymuint
  topmodule_list(list<const MvModule*>& module_list) const;

  /// @brief モジュール番号の最大値+1を得る．
  ymuint
  max_module_id() const;

  /// @brief モジュールIDをキーにしてモジュールにアクセスする．
  /// @param[in] id モジュールID ( 0 <= id < max_module_id() )
  /// @return 該当するモジュールを返す．
  /// @note 該当するモジュールがない場合は NULL を返す．
  const MvModule*
  module(ymuint id) const;

  /// @brief モジュールIDをキーにしてモジュールにアクセスする．
  /// @param[in] id モジュールID ( 0 <= id < max_module_id() )
  /// @return 該当するモジュールを返す．
  /// @note 該当するモジュールがない場合は NULL を返す．
  MvModule*
  _module(ymuint id);

  /// @brief ノードの ID番号の最大値 + 1 を返す．
  ymuint
  max_node_id() const;

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_node_id() )
  /// @note NULL が還されることもある．
  const MvNode*
  node(ymuint id) const;

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_node_id() )
  /// @note NULL が還されることもある．
  MvNode*
  _node(ymuint id);

  /// @brief ネットの ID番号の最大値 + 1 を返す．
  ymuint
  max_net_id() const;

  /// @brief ネットを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_net_id() )
  const MvNet*
  net(ymuint id) const;

  /// @brief ネットを得る．
  /// @param[in] id ID番号 ( 0 <= id < max_net_id() )
  MvNet*
  _net(ymuint id);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の設定を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールを生成する．
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ni 入力ノード数
  /// @param[in] no 出力ノード数
  /// @param[in] nio 入出力ノード数
  /// @return 生成したモジュールを返す．
  /// @note 入出力ノードのビット幅は1で初期化される．
  MvModule*
  new_module(const char* name,
	     ymuint np,
	     ymuint ni,
	     ymuint no,
	     ymuint nio);

  /// @brief モジュールを生成する．
  /// @param[in] name 名前
  /// @param[in] np ポート数
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  /// @param[in] obitwidth_array 出力のビット幅の配列
  /// @param[in] iobitwidth_array 入出力のビット幅の配列
  /// @return 生成したモジュールを返す．
  MvModule*
  new_module(const char* name,
	     ymuint np,
	     const vector<ymuint>& ibitwidth_array,
	     const vector<ymuint>& obitwidth_array,
	     const vector<ymuint>& iobitwidth_array);

  /// @brief モジュールを削除する．
  /// @param[in] module 対象のモジュール
  /// @note モジュールインスタンスとして使われているモジュールは削除できない．
  void
  delete_module(MvModule* module);

  /// @brief モジュールのポートを初期化する．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] name 名前
  /// @param[in] port_ref_num ポート参照式の数
  void
  init_port(MvModule* module,
	    ymuint pos,
	    const char* name,
	    ymuint port_ref_num);

  /// @brief ポート参照式の内容を設定する(単純な形式)．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] port_ref_pos ポート参照式の番号
  /// @param[in] node 対応する入出力ノード
  void
  set_port_ref(MvModule* module,
	       ymuint pos,
	       ymuint port_ref_pos,
	       MvNode* node);

  /// @brief ポート参照式の内容を設定する(ビット指定形式)．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] port_ref_pos ポート参照式の番号
  /// @param[in] node 対応する入出力ノード
  /// @param[in] index ビット指定位置
  void
  set_port_ref(MvModule* module,
	       ymuint pos,
	       ymuint port_ref_pos,
	       MvNode* node,
	       ymuint index);

  /// @brief ポート参照式の内容を設定する(範囲指定形式)．
  /// @param[in] module 対象のモジュール
  /// @param[in] pos ポート番号
  /// @param[in] port_ref_pos ポート参照式の番号
  /// @param[in] node 対応する入出力ノード
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  void
  set_port_ref(MvModule* module,
	       ymuint pos,
	       ymuint port_ref_pos,
	       MvNode* node,
	       ymuint msb,
	       ymuint lsb);

  /// @brief 冗長な through ノードを取り除く
  void
  sweep();

  /// @brief 非同期セット/リセットタイプの FF ノードを生成する．
  MvNode*
  new_dff1(MvModule* module,
	   ymuint bit_width = 1);

  /// @brief 同期セット/リセットタイプの FF ノードを生成する．
  MvNode*
  new_dff2(MvModule* module,
	   ymuint bit_width = 1);

  /// @brief through ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_through(MvModule* module,
	      ymuint bit_width = 1);

  /// @brief not ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_not(MvModule* module,
	  ymuint bit_width = 1);

  /// @brief and ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_and(MvModule* module,
	  ymuint bit_width = 1);

  /// @brief or ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_or(MvModule* module,
	 ymuint bit_width = 1);

  /// @brief xor ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_xor(MvModule* module,
	  ymuint bit_width = 1);

  /// @brief reduction and ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_rand(MvModule* module,
	   ymuint bit_width = 1);

  /// @brief reduction or ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_ror(MvModule* module,
	  ymuint bit_width = 1);

  /// @brief reduction xor ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_rxor(MvModule* module,
	   ymuint bit_width = 1);

  /// @brief equal ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_equal(MvModule* module,
	    ymuint bit_width);

  /// @brief less than ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_lt(MvModule* module,
	 ymuint bit_width);

  /// @brief shift left logical ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sll(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief shift right logical ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_srl(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief shift left arithmetic ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sla(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief shift right arithmetic ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力データのビット幅
  /// @param[in] bit_width2 シフト量のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sra(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief cmpl ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  /// @note 2の補数を計算する．
  MvNode*
  new_cmpl(MvModule* module,
	   ymuint bit_width);

  /// @brief add ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_add(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief sub ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_sub(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief mult ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_mult(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief div ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_div(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief modulo ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_mod(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief power ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_pow(MvModule* module,
	  ymuint bit_width1,
	  ymuint bit_width2,
	  ymuint bit_width3);

  /// @brief condition ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_ite(MvModule* module,
	  ymuint bit_width);

  /// @brief concatenate ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  /// @return 生成したノードを返す．
  /// @note 出力のビット幅は入力のビット幅の和となる．
  MvNode*
  new_concat(MvModule* module,
	     const vector<ymuint>& ibitwidth_arary);

  /// @brief bit-selectノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bitpos ビット位置
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_constbitselect(MvModule* module,
		     ymuint bitpos,
		     ymuint bit_width);

  /// @brief part-select ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  /// @param[in] bit_width ビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_constpartselect(MvModule* module,
		      ymuint msb,
		      ymuint lsb,
		      ymuint bit_width);

  /// @brief 可変 bit-selectノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_widht2 ビット指定入力のビット幅
  /// @return 生成したノードを返す．
  MvNode*
  new_bitselect(MvModule* module,
		ymuint bit_width1,
		ymuint bit_width2);

  /// @brief 可変 part-select ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_width2 範囲指定入力のビット幅
  /// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
  /// @return 生成したノードを返す．
  MvNode*
  new_partselect(MvModule* module,
		 ymuint bit_width1,
		 ymuint bit_width2,
		 ymuint bit_width3);

  /// @brief combinational UDP ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] ni 入力数
  /// @note ビット幅はすべて1ビット
  MvNode*
  new_combudp(MvModule* module,
	      ymuint ni);

  /// @brief sequential UDP ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] ni 入力数
  /// @note ビット幅はすべて1ビット
  MvNode*
  new_sequdp(MvModule* module,
	     ymuint ni);

  /// @brief constant ノードを生成する．
  /// @param[in] module ノードが属するモジュール
  /// @param[in] bit_width ビット幅
  /// @param[in] val 値
  MvNode*
  new_const(MvModule* module,
	    ymuint bit_width,
	    const vector<ymuint32>& val);

  /// @brief 連結演算からビットを抜き出す．
  /// @param[in] src_node 連結演算ノード
  /// @param[in] bitpos 抜き出すビット位置
  MvNode*
  select_from_concat(MvNode* src_node,
		     ymuint bitpos);

  /// @brief 部分指定子からビットを抜き出す．
  /// @param[in] src_node 部分指定ノード
  /// @param[in] bitpos 抜き出すビット位置
  MvNode*
  select_from_partselect(MvNode* src_node,
			 ymuint bitpos);

  /// @brief 連結演算から部分を抜き出す．
  /// @param[in] src_node 連結演算ノード
  /// @param[in] msb 抜き出す部分の MSB
  /// @param[in] lsb 抜き出す部分の LSB
  MvNode*
  select(MvNode* src_node,
	 ymuint msb,
	 ymuint lsb);

  /// @brief ノードを削除する．
  /// @param[in] node 対象のノード
  /// @note 入力ノード, 出力ノードは削除できない
  /// @note 接続が残っているノードは削除できない．
  void
  delete_node(MvNode* node);

  /// @brief node を alt_node に置き換える．
  /// @param[in] node 置換元のノード
  /// @param[in] alt_node 置換先のノード
  /// @note node は削除される．
  void
  replace(MvNode* node,
	  MvNode* alt_node);

  /// @brief ピンとピンを接続する．
  /// @param[in] src_node 入力元のノード
  /// @param[in] src_pin_pos 入力元のピン番号
  /// @param[in] dst_node 出力先のノード
  /// @param[in] dst_pin 出力先のピン番号
  /// @retval true 接続が成功した．
  /// @retval false 接続が失敗した．
  /// @note 接続が失敗するのは，
  ///  - ピンが異なるモジュールに属していた．
  ///  - ピンのビット幅が異なっていた．
  bool
  connect(MvNode* src_node,
	  ymuint src_pin_pos,
	  MvNode* dst_node,
	  ymuint dst_pin_pos);

  /// @brief 接続を取り除く
  /// @param[in] src_node 入力元のノード
  /// @param[in] src_pin_pos 入力元のピン番号
  /// @param[in] dst_node 出力先のノード
  /// @param[in] dst_pin 出力先のピン番号
  /// @note 現在の実装はあまり効率が良くない．
  void
  disconnect(MvNode* src_node,
	     ymuint src_pin_pos,
	     MvNode* dst_node,
	     ymuint dst_pin_pos);

  /// @brief 接続を切り替える．
  /// @param[in] old_node 元のノード
  /// @param[in] old_pin_pos 元のピン番号
  /// @param[in] new_node 新しいノード
  /// @param[in] new_pin 新しいピン番号
  void
  reconnect(MvNode* old_node,
	    ymuint old_pin_pos,
	    MvNode* new_node,
	    ymuint new_pin_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを登録する．
  /// @param[in] node 対象のノード
  void
  reg_node(MvNode* node);

  /// @brief ノードの登録を削除する．
  /// @param[in] node 対象のノード
  void
  unreg_node(MvNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュールを納めた配列
  // ID番号をキーにしている．
  // 場合によっては穴が空いている．
  vector<MvModule*> mModuleArray;

  // モジュールのID番号を管理するためのオブジェクト
  ItvlMgr mModuleItvlMgr;

  // 全ノードを ID 番号をキーにして格納する配列
  vector<MvNode*> mNodeArray;

  // ノードのID番号を管理するためのオブジェクト
  ItvlMgr mNodeItvlMgr;

  // 全ネットを ID 番号をキーにして格納する配列
  vector<MvNet*> mNetArray;

  // ネットの ID番号を管理するためのオブジェクト
  ItvlMgr mNetItvlMgr;

};


/// @brief 内容を出力する
/// @param[in] s 出力先のストリーム
/// @param[in] mgr MvMgr
/// @note デバッグ用
void
dump(ostream& s,
     const MvMgr& mgr);


/// @brief 内容を Verilog-HDL 形式で出力する
/// @param[in] s 出力先のストリーム
/// @param[in] mgr MvMgr
void
dump_verilog(ostream& s,
	     const MvMgr& mgr);

/// @brief ノード番号ともとのVerilog名の対応を出力する．
void
dump_node_map(ostream& s,
	      const MvMgr& mgr,
	      const vector<pair<const VlDecl*, ymuint> >& node_map);

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVMGR_H
