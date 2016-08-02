
/// @file libym_mvn/MvMgr.cc
/// @brief MvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"

#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"

#include "MvInput.h"
#include "MvOutput.h"
#include "MvInout.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvMgr
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
MvMgr::MvMgr()
{
}

// @brief デストラクタ
MvMgr::~MvMgr()
{
}

// @brief トップレベルモジュールのリストを得る．
// @param[out] module_list モジュールを格納するリスト
// @return 要素数を返す．
// @note この関数はトップモジュール数ではなく全モジュール数に比例した
// 実行時間を要する．
ymuint
MvMgr::topmodule_list(list<const MvModule*>& module_list) const
{
  module_list.clear();
  for (vector<MvModule*>::const_iterator p = mModuleArray.begin();
       p != mModuleArray.end(); ++ p) {
    MvModule* module = *p;
    if ( module && module->parent() == NULL ) {
      module_list.push_back(module);
    }
  }
  return module_list.size();
}

// @brief モジュール番号の最大値+1を得る．
ymuint
MvMgr::max_module_id() const
{
  return mModuleArray.size();
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は NULL を返す．
const MvModule*
MvMgr::module(ymuint id) const
{
  return mModuleArray[id];
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は NULL を返す．
MvModule*
MvMgr::_module(ymuint id)
{
  return mModuleArray[id];
}

// @brief ノードの ID番号の最大値 + 1 を返す．
ymuint
MvMgr::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note NULL が還されることもある．
const MvNode*
MvMgr::node(ymuint id) const
{
  return mNodeArray[id];
}

// @brief ノードを得る．
// @param[in] id ID番号 ( 0 <= id < max_node_id() )
// @note NULL が還されることもある．
MvNode*
MvMgr::_node(ymuint id)
{
  return mNodeArray[id];
}

// @brief ネットの ID番号の最大値 + 1 を返す．
ymuint
MvMgr::max_net_id() const
{
  return mNetArray.size();
}

// @brief ネットを得る．
// @param[in] id ID番号 ( 0 <= id < max_net_id() )
const MvNet*
MvMgr::net(ymuint id) const
{
  return mNetArray[id];
}

// @brief ネットを得る．
// @param[in] id ID番号 ( 0 <= id < max_net_id() )
MvNet*
MvMgr::_net(ymuint id)
{
  return mNetArray[id];
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @param[in] nio 入出力ノード数
// @return 生成したモジュールを返す．
// @note 入出力ノードのビット幅は1で初期化される．
MvModule*
MvMgr::new_module(const char* name,
		  ymuint np,
		  ymuint ni,
		  ymuint no,
		  ymuint nio)
{
  vector<ymuint> ibitwidth_array(ni, 1);
  vector<ymuint> obitwidth_array(no, 1);
  vector<ymuint> iobitwidth_array(nio, 1);
  return new_module(name, np,
		    ibitwidth_array, obitwidth_array, iobitwidth_array);
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] np ポート数
// @param[in] ibitwidth_array 入力のビット幅の配列
// @param[in] obitwidth_array 出力のビット幅の配列
// @param[in] iobitwidth_array 入出力のビット幅の配列
// @return 生成したモジュールを返す．
MvModule*
MvMgr::new_module(const char* name,
		  ymuint np,
		  const vector<ymuint>& ibitwidth_array,
		  const vector<ymuint>& obitwidth_array,
		  const vector<ymuint>& iobitwidth_array)
{
  int tmp = mModuleItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // IDが枯渇？
    return NULL;
  }
  mModuleItvlMgr.erase(tmp);
  ymuint id = tmp;

  ymuint ni = ibitwidth_array.size();
  ymuint no = obitwidth_array.size();
  ymuint nio = iobitwidth_array.size();
  MvModule* module = new MvModule(name, np, ni, no, nio);
  module->mId = id;
  while ( mModuleArray.size() <= id ) {
    mModuleArray.push_back(NULL);
  }
  mModuleArray[id] = module;

  for (ymuint i = 0; i < ni; ++ i) {
    MvNode* node = new MvInput(module, ibitwidth_array[i]);
    reg_node(node);
    module->mInputArray[i] = node;
  }
  for (ymuint i = 0; i < no; ++ i) {
    MvNode* node = new MvOutput(module, obitwidth_array[i]);
    reg_node(node);
    module->mOutputArray[i] = node;
  }
  for (ymuint i = 0; i < nio; ++ i) {
    MvNode* node = new MvInout(module, iobitwidth_array[i]);
    reg_node(node);
    module->mInoutArray[i] = node;
  }
  return module;
}

// @brief モジュールを削除する．
// @param[in] module 対象のモジュール
// @note モジュールインスタンスとして使われているモジュールは削除できない．
void
MvMgr::delete_module(MvModule* module)
{
  if ( module->parent() ) {
    // モジュールインスタンスは削除できない．
    return;
  }
  // module の要素を削除

  // module の下位モジュールを再帰的に削除

  mModuleItvlMgr.add(module->mId);
  mModuleArray[module->mId] = NULL;
  delete module;
}

// @brief モジュールのポートを初期化する．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] name 名前
// @param[in] port_ref_num ポート参照式の数
void
MvMgr::init_port(MvModule* module,
		 ymuint pos,
		 const char* name,
		 ymuint port_ref_num)
{
  module->mPortArray[pos] = new MvPort(name, port_ref_num);
}

// @brief ポート参照式の内容を設定する(単純な形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
void
MvMgr::set_port_ref(MvModule* module,
		    ymuint pos,
		    ymuint port_ref_pos,
		    MvNode* node)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node);
}

// @brief ポート参照式の内容を設定する(ビット指定形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
// @param[in] index ビット指定位置
void
MvMgr::set_port_ref(MvModule* module,
		    ymuint pos,
		    ymuint port_ref_pos,
		    MvNode* node,
		    ymuint index)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node, index);
}

// @brief ポート参照式の内容を設定する(範囲指定形式)．
// @param[in] module 対象のモジュール
// @param[in] pos ポート番号
// @param[in] port_ref_pos ポート参照式の番号
// @param[in] node 対応する入出力ノード
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
void
MvMgr::set_port_ref(MvModule* module,
		    ymuint pos,
		    ymuint port_ref_pos,
		    MvNode* node,
		    ymuint msb,
		    ymuint lsb)
{
  module->mPortArray[pos]->mPortRefArray[port_ref_pos].set(node, msb, lsb);
}

// @brief ノードを削除する．
// @param[in] node 対象のノード
// @note 入力ノード, 出力ノードは削除できない
// @note 接続が残っているノードは削除できない．
void
MvMgr::delete_node(MvNode* node)
{
  if ( node->type() == MvNode::kInput || node->type() == MvNode::kOutput ) {
    return;
  }
  for (ymuint i = 0; i < node->input_num(); ++ i) {
    if ( node->input(i)->src_pin() ) {
      cerr << "node" << node->id() << " has fanin" << endl;
      return;
    }
  }
  for (ymuint i = 0; i < node->output_num(); ++ i) {
    if ( !node->output(i)->dst_pin_list().empty() ) {
      cerr << "node" << node->id() << " has fanout" << endl;
      return;
    }
  }
  unreg_node(node);
  delete node;
}

bool
no_fanouts(MvNode* node)
{
  ymuint no = node->output_num();
  for (ymuint j = 0; j < no; ++ j) {
    if ( !node->output(j)->dst_pin_list().empty() ) {
      return false;
    }
  }
  return true;
}

// @brief 冗長な through ノードを取り除く
void
MvMgr::sweep()
{
  ymuint n = max_node_id();
  // ビット/部分選択と接続している連結演算の削除を行う．
  vector<MvNode*> node_list;
  node_list.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node = _node(i);
    if ( node == NULL ) continue;
    node_list.push_back(node);
  }
  for (vector<MvNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    MvNode* node = *p;
    if ( node->type() == MvNode::kConstBitSelect ) {
      MvNode* src_node = node->input(0)->src_pin()->node();
      if ( src_node->type() == MvNode::kConcat ) {
	MvNode* alt_node = select_from_concat(src_node, node->bitpos());
	// node を alt_node に置き換える．
	replace(node, alt_node);
      }
      else if ( src_node->type() == MvNode::kConstPartSelect ) {
	MvNode* alt_node = select_from_partselect(src_node, node->bitpos());
	// node を alt_node に置き換える．
	replace(node, alt_node);
      }
    }
#if 0
    else if ( node->type() == MvNode::kConstPartSelect ) {
      MvNode* src_node = node->input(0)->src_pin()->node();
      if ( src_node->type() == MvNode::kConcat ) {
	MvNdoe* alt_node = select(src_node, node->mbs(), node->lsb());
	// node を alt_node に置き換える．
	replace(node, alt_node);
      }
    }
#endif
  }

  // どこにも出力していないノードを削除する．
  list<MvNode*> node_queue;
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node = _node(i);
    if ( node == NULL ) continue;
    if ( node->type() == MvNode::kOutput ) continue;
    if ( no_fanouts(node) ) {
      node_queue.push_back(node);
    }
  }
  while ( !node_queue.empty() ) {
    MvNode* node = node_queue.front();
    node_queue.pop_front();
    ymuint ni = node->input_num();
    for (ymuint i = 0; i < ni; ++ i) {
      MvOutputPin* src_pin = node->input(i)->src_pin();
      if ( src_pin ) {
	MvNode* src_node = src_pin->node();
	disconnect(src_node, src_pin->pos(), node, i);
	if ( no_fanouts(src_node) ) {
	  node_queue.push_back(src_node);
	}
      }
    }
    delete_node(node);
  }
}

// @brief 連結演算からビットを抜き出す．
// @param[in] src_node 連結演算ノード
// @param[in] bitpos 抜き出すビット位置
MvNode*
MvMgr::select_from_concat(MvNode* src_node,
			  ymuint bitpos)
{
  assert_cond( src_node->type() == MvNode::kConcat, __FILE__, __LINE__);
  ymuint ni = src_node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint idx = ni - i - 1;
    const MvInputPin* ipin = src_node->input(idx);
    ymuint bw = ipin->bit_width();
    if ( bitpos < bw ) {
      MvNode* inode = ipin->src_pin()->node();
      if ( inode->type() == MvNode::kConcat ) {
	return select_from_concat(inode, bitpos);
      }
      else if ( inode->type() == MvNode::kConstPartSelect ) {
	return select_from_partselect(inode, bitpos);
      }
      else if ( bw == 1 ) {
	return inode;
      }
      else {
	MvNode* bitsel = new_constbitselect(src_node->mParent, bitpos, bw);
	connect(inode, 0, bitsel, 0);
	return bitsel;
      }
    }
    bitpos -= bw;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 部分指定子からビットを抜き出す．
// @param[in] src_node 部分指定ノード
// @param[in] bitpos 抜き出すビット位置
MvNode*
MvMgr::select_from_partselect(MvNode* src_node,
			      ymuint bitpos)
{
  assert_cond( src_node->type() == MvNode::kConstPartSelect,
	       __FILE__, __LINE__);
  const MvInputPin* ipin = src_node->input(0);
  ymuint bw = ipin->bit_width();
  assert_cond( bitpos < bw, __FILE__, __LINE__);
  ymuint msb = src_node->msb();
  ymuint lsb = src_node->lsb();
  if ( msb > lsb ) {
    bitpos = bitpos + lsb;
  }
  else {
    bitpos = lsb - bitpos;
  }
  MvNode* inode = ipin->src_pin()->node();
  if ( inode->type() == MvNode::kConcat ) {
    return select_from_concat(inode, bitpos);
  }
  else if ( inode->type() == MvNode::kConstPartSelect ) {
    return select_from_partselect(inode, bitpos);
  }
  else {
    MvNode* bitsel = new_constbitselect(src_node->mParent, bitpos, bw);
    connect(inode, 0, bitsel, 0);
    return bitsel;
  }
}

// @brief 連結演算から部分を抜き出す．
// @param[in] src_node 連結演算ノード
// @param[in] msb 抜き出す部分の MSB
// @param[in] lsb 抜き出す部分の LSB
MvNode*
MvMgr::select(MvNode* src_node,
	      ymuint msb,
	      ymuint lsb)
{
#if 0
  assert_cond( src_node->type() == MvNode::kConcat, __FILE__, __LINE__);
  ymuint ni = src_node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvInputPin* ipin = src_node->input(i);
    ymuint bw = ipin->bit_width();
    if ( bitpos < bw ) {
      MvNode* inode = ipin->net()->src_pin()->node();
      if ( inode->type() == MvNode::kConcat ) {
	return select(inode, bitpos);
      }
      else {
	MvNode* bitsel = new_constbitselect(src_node->mParent, bitpos, bw);
	connect(inode, 0, bitsel, 0);
	return bitsel;
      }
    }
    bitpos -= bw;
  }
  assert_not_reached(__FILE__, __LINE__);
#endif
  return NULL;
}

// @brief node を alt_node に置き換える．
// @param[in] node 置換元のノード
// @param[in] alt_node 置換先のノード
// @note node は削除される．
void
MvMgr::replace(MvNode* node,
	       MvNode* alt_node)
{
  ymuint no = node->output_num();
  assert_cond( no == alt_node->output_num(), __FILE__, __LINE__);
  for (ymuint i = 0; i < no; ++ i) {
    reconnect(node, i, alt_node, i);
  }
}

// @brief ピンとピンを接続する．
// @param[in] src_node 入力元のノード
// @param[in] src_pin_pos 入力元のピン番号
// @param[in] dst_node 出力先のノード
// @param[in] dst_pin 出力先のピン番号
// @return 接続を表すネットを返す．
// @note 接続が失敗したら NULLを返す．
// @note 接続が失敗するのは，
//  - ピンが異なるモジュールに属していた．
//  - ピンのビット幅が異なっていた．
bool
MvMgr::connect(MvNode* src_node,
	       ymuint src_pin_pos,
	       MvNode* dst_node,
	       ymuint dst_pin_pos)
{
  if ( src_node->parent() != dst_node->parent() ) {
    cerr << "connecting between differnt module" << endl;
    abort();
    return false;
  }
  MvOutputPin* src_pin = src_node->_output(src_pin_pos);
  MvInputPin* dst_pin = dst_node->_input(dst_pin_pos);
  if ( src_pin->bit_width() != dst_pin->bit_width() ) {
    cerr << "src_pin->bit_width() = " << src_pin->bit_width()
	 << ", dst_pin->bit_width() = " << dst_pin->bit_width() << endl;
    cerr << "bit_width mismatch" << endl;
    abort();
    return false;;
  }
  src_pin->mDstPinList.push_back(dst_pin);
  dst_pin->mSrcPin = src_pin;
  return true;
}

// @brief 接続を取り除く
// @param[in] src_node 入力元のノード
// @param[in] src_pin_pos 入力元のピン番号
// @param[in] dst_node 出力先のノード
// @param[in] dst_pin 出力先のピン番号
void
MvMgr::disconnect(MvNode* src_node,
		  ymuint src_pin_pos,
		  MvNode* dst_node,
		  ymuint dst_pin_pos)
{
  MvOutputPin* src_pin = src_node->_output(src_pin_pos);
  MvInputPin* dst_pin = dst_node->_input(dst_pin_pos);
  assert_cond( dst_pin->mSrcPin == src_pin, __FILE__, __LINE__);
  src_pin->mDstPinList.erase(dst_pin);
  dst_pin->mSrcPin = NULL;
}

// @brief 接続を切り替える．
// @param[in] old_node 元のノード
// @param[in] old_pin_pos 元のピン番号
// @param[in] new_node 新しいノード
// @param[in] new_pin 新しいピン番号
void
MvMgr::reconnect(MvNode* old_node,
		 ymuint old_pin_pos,
		 MvNode* new_node,
		 ymuint new_pin_pos)
{
  MvOutputPin* old_pin = old_node->_output(old_pin_pos);
  MvOutputPin* new_pin = new_node->_output(new_pin_pos);
  const MvInputPinList& fo_list = old_pin->dst_pin_list();
  // リンクトリストをたどっている途中でリンクの変更はできないので
  // 配列にコピーする．
  vector<MvInputPin*> tmp_list;
  tmp_list.reserve(fo_list.size());
  for (MvInputPinList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    MvInputPin* ipin = *p;
    tmp_list.push_back(ipin);
  }
  for (vector<MvInputPin*>::iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    MvInputPin* ipin = *p;
    old_pin->mDstPinList.erase(ipin);
    ipin->mSrcPin = new_pin;
    new_pin->mDstPinList.push_back(ipin);
  }
}

// @brief ノードを登録する．
// @param[in] node 対象のノード
void
MvMgr::reg_node(MvNode* node)
{
  int tmp = mNodeItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // ID番号が枯渇？
    return;
  }
  mNodeItvlMgr.erase(tmp);
  ymuint id = tmp;
  node->mId = id;
  while ( mNodeArray.size() <= id ) {
    mNodeArray.push_back(NULL);
  }
  mNodeArray[id] = node;

  if ( node->type() != MvNode::kInput && node->type() != MvNode::kOutput ) {
    MvModule* module = node->mParent;
    list<MvNode*>& nodelist = module->mNodeList;
    nodelist.push_back(node);
    node->mSelfRef = nodelist.end();
    -- node->mSelfRef;
    assert_cond( *node->mSelfRef == node , __FILE__, __LINE__);
  }
}

// @brief ノードの登録を削除する．
// @param[in] node 対象のノード
void
MvMgr::unreg_node(MvNode* node)
{
  mNodeItvlMgr.add(node->id());
  mNodeArray[node->id()] = NULL;
  if ( node->type() != MvNode::kInput && node->type() != MvNode::kOutput ) {
    MvModule* module = node->mParent;
    module->mNodeList.erase(node->mSelfRef);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MvInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvInputPin::MvInputPin() :
  mNode(NULL),
  mBitWidth(0),
  mPos(0),
  mSrcPin(NULL)
{
}

// @brief デストラクタ
MvInputPin::~MvInputPin()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvOutputPin::MvOutputPin() :
  mNode(NULL),
  mBitWidth(0),
  mPos(0)
{
}

// @brief デストラクタ
MvOutputPin::~MvOutputPin()
{
}

END_NAMESPACE_YM_MVN
