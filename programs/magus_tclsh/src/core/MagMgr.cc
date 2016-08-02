
/// @file magus/logbase/MagMgr.cc
/// @brief MagMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagMgr.h"
#include "NetHandle.h"
#include "BNetHandle.h"
#include "BdnHandle.h"
#include "MvnHandle.h"


BEGIN_NAMESPACE_MAGUS

// カレントネットワークを常に表す文字列．
string MagMgr::kCurNetwork("_network");


// コンストラクタ
MagMgr::MagMgr() :
  mAlloc(sizeof(Cell), 1024),
  mNetHandleHashSize(0),
  mNetHandleHash(NULL),
  mNetHandleHashNum(0)
{
  alloc_table(16);

  // カレントネットワークは NULL
  mCurNet = NULL;
}

// デストラクタ
// 全てのネットワークを破壊する．
MagMgr::~MagMgr()
{
  // このオブジェクトの管理しているネットワークを全て破棄する．
  // といってもこのループでは name_list にネットワーク名を入れている
  // だけ．
  vector<string> name_list;
  get_network_names(name_list);

  // delete_net() 中で mNetHash を変更するので上のループ内
  // では削除できない．
  for (vector<string>::iterator p = name_list.begin();
       p != name_list.end(); ++ p) {
    string name = *p;
    bool stat = delete_nethandle(name);
    assert_cond(stat, __FILE__, __LINE__);
  }
}

// @brief 名前が適切かどうか調べる関数
// @param[in] name 調べる名前
// @param[in] err_out エラー出力
// @return name が ^@?[a-zA-Z_]*$ で表される正規表現なら true を返す．
bool
MagMgr::check_name(const string& name,
		   ostream* err_out)
{
  // 今は @?[a-zA-Z0-9_]* となっているかをチェックする．
  ymuint pos = 0;
  if ( name[pos] == '@' ) {
    ++ pos;
  }
  for (; pos < name.size(); ++ pos) {
    int c = name[pos];
    if ( !isalnum(c) && c != '_' ) {
      // 名前が不正
      if ( err_out ) {
	*err_out << name << " : illegal name";
      }
      return false;
    }
  }
  return true;
}

// @brief 新たな BNetwork を作成して登録する．
// @param[in] name 名前
// @param[in] err_out エラー出力
// @return 作成したネットハンドル
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合には NULL を返す．
NetHandle*
MagMgr::new_bnethandle(const string& name,
		       ostream* err_out)
{
  NetHandle* neth = new BNetHandle(name);
  bool stat = reg_nethandle(neth, err_out);
  if ( !stat ) {
    delete neth;
    neth = NULL;
  }
  return neth;
}

// @brief 新たな BdNetwork を作成して登録する．
// @param[in] name 名前
// @param[in] err_out エラー出力
// @return 作成したネットハンドル
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合には NULL を返す．
NetHandle*
MagMgr::new_bdnhandle(const string& name,
		      ostream* err_out)
{
  NetHandle* neth = new BdnHandle(name);
  bool stat = reg_nethandle(neth, err_out);
  if ( !stat ) {
    delete neth;
    neth = NULL;
  }
  return neth;
}

// @brief 新たな MvNetwork を作成して登録する．
// @param[in] name 名前
// @param[in] err_out エラー出力
// @return 作成したネットハンドル
// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
// @note また，名前が不適切な場合にもエラーとなる．
// @note エラーが起きた場合には NULL を返す．
NetHandle*
MagMgr::new_mvnhandle(const string& name,
		      ostream* err_out)
{
  NetHandle* neth = new MvnHandle(name);
  bool stat = reg_nethandle(neth, err_out);
  if ( !stat ) {
    delete neth;
    neth = NULL;
  }
  return neth;
}

// @brief ネットワークの検索
// @param[in] name ネットワーク名
// @param[in] err_out エラー出力
// @return name という名のネットワークを返す．
// @note 名前が不適切な場合やその名前のネットワークが存在しない場合
// にはエラーとなり，NULL を返す．
NetHandle*
MagMgr::find_nethandle(const string& name,
		       ostream* err_out)
{
  if ( name == kCurNetwork ) {
    // カレントネットワークを返す．
    return mCurNet;
  }

  // そうでなければハッシュを探す．
  NetHandle* neth = _find_nethandle(name);
  if ( neth == NULL ) {
    // 存在していなければエラー
    if ( err_out ) {
      *err_out << name << " : No such network.";
    }
  }
  return neth;
}

// @brief ネットワークの削除
// @param[in] name ネットワーク名
// @param[int] err_out エラー出力
// @retrun name という名のネットワークが存在したら削除して true を返す．
// @note 名前が不適切な場合や存在しない場合にはエラーとなり，false を返す．
bool
MagMgr::delete_nethandle(const string& name,
			 ostream* err_out)
{
  NetHandle* neth = _find_nethandle(name);
  if ( neth == NULL ) {
    // 存在していなければエラー
    if ( err_out ) {
      *err_out << name << " : No such network.";
    }
    return false;
  }

  if ( neth->mCount > 0 ) {
    // カレントネットワークは削除できない．
    if ( err_out ) {
      *err_out << "Can't delete. The network is in use.";
    }
    return false;
  }

  // temporary name なら mTempNameSet に戻す
  if ( name[0] == '@' ) {
    size_t n = name.size();
    int v = 0;
    for (size_t i = 1; i < n; i ++) {
      char c = name[i];
      v = v * 10 + (c - '0');
    }
    mTempNameSet.add(v);
  }


  // ハッシュから抜き，neth を削除する．
  _delete_nethandle(neth);

  // 正常終了
  return true;
}

// @brief カレントネットワークの変更
// @param[in] name ネットワーク名
// @param[in] err_out エラー出力
// @return 正常終了したら true を返す．
// @note name という名のネットワークをカレントネットワークに設定する．
// @note 名前が不適切な場合や存在しない場合にはエラーとなり，false を返す．
bool
MagMgr::change_cur_nethandle(const string& name,
			     ostream* err_out)
{
  NetHandle* neth = find_nethandle(name, err_out);
  if ( neth == NULL ) {
    // エラー，たぶん名前が不正
    return false;
  }

  // 以下のコードは mCurNet と neth が等しくても安全に実行できる
  -- mCurNet->mCount;
  ++ neth->mCount;
  mCurNet = neth;

  return true;
}

// 操作対象のネットワークをスタックに追加する．
bool
MagMgr::push_cur_nethandle(const string& name,
			   ostream* err_out)
{
  NetHandle* neth = find_nethandle(name, err_out);
  if ( neth == NULL ) {
    // エラー，たぶん名前が不正
    return false;
  }

  if ( mCurNet != NULL ) {
    // 昔のカレントネットワークをスタックに追加する．
    mNetStack.push_back(mCurNet);
  }

  // 新しいネットワークをカレントネットワークにする．
  mCurNet = neth;
  ++ neth->mCount;

  return true;
}

// 最新のネットワークをスタックから取り出す．
// スタックが空の場合には false を返す．
// その場合にはカレントネットワークは不変
// エラーが起きた場合に leave_errmsg が true ならばはインタプリタ
// にメッセージをセットする．
bool
MagMgr::pop_cur_nethandle(ostream* ostream)
{
  if ( mNetStack.empty() ) {
    // スタックが空だった．
    if ( ostream ) {
      *ostream << "network stack is empty.";
    }
    return false;
  }

  // スタックの頭からネットワークを取り出す．
  NetHandle* neth = mNetStack.back();
  mNetStack.pop_back();

  // 取り出したネットワークをカレントネットワークにする．
  -- mCurNet->mCount;
  mCurNet = neth;

  return true;
}

// @brief カレントネットワークの取得
// @note この関数はエラーとなることはない．
NetHandle*
MagMgr::cur_nethandle() const
{
  assert_cond( mCurNet != NULL, __FILE__, __LINE__);
  return mCurNet;
}

// @brief ネットワーク名のリストの作成
// @param[out] name_list 作成されたネットワーク名を収めるリスト
void
MagMgr::get_network_names(vector<string>& name_list) const
{
  name_list.clear();
  name_list.reserve(mNetHandleHashNum);
  for (ymuint i = 0; i < mNetHandleHashSize; ++ i) {
    for (Cell* cell = mNetHandleHash[i]; cell; cell = cell->mLink) {
      name_list.push_back(cell->mHandle->name());
    }
  }
}

// @brief テンポラリ名の生成
// @note temporary name は '@' + 数字
string
MagMgr::new_tmpname()
{
  int num = mTempNameSet.avail_num();
  if ( num == -1 ) {
    // 使用可能な数字が残っていない！！
    return string();
  }

  mTempNameSet.erase(num);

  ostringstream buf;
  buf << '@' << num;
  buf.flush();
  return string(buf.str());
}

// @brief ネットワークの登録
// @param[in] handle 登録するネットワークハンドル
// @param[in] err_out エラー出力
// @return 実行結果
// @note 同名のネットワークが既に存在していた場合や名前が不適切な
// 場合にはエラーとなり，false を返す．
bool
MagMgr::reg_nethandle(NetHandle* handle,
		      ostream* err_out)
{
  string name = handle->name();

  // 名前が適切か調べる
  if ( !check_name(name, err_out) ) {
    // 名前が不適切だった．
    return false;
  }

  // ハッシュを調べる．
  if ( name == kCurNetwork || _find_nethandle(name) != NULL ) {
    // 同名のネットワークが存在していた．
    if ( err_out ) {
      *err_out << name << " : Already exists.";
    }
    return false;
  }

  // 存在していないので登録する．
  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell;
  cell->mHandle = handle;

  if ( mNetHandleHashNum >= mNetHandleHashNextLimit ) {
    alloc_table(mNetHandleHashSize * 2);
  }
  ymuint pos = hash_func(handle->name()) % mNetHandleHashSize;
  cell->mLink = mNetHandleHash[pos];
  mNetHandleHash[pos] = cell;
  ++ mNetHandleHashNum;

  return true;
}

// @brief ネットワークを検索する．
NetHandle*
MagMgr::_find_nethandle(const string& name)
{
  ymuint pos = hash_func(name) % mNetHandleHashSize;
  for (Cell* cell = mNetHandleHash[pos]; cell; cell = cell->mLink) {
    if ( cell->mHandle->name() == name ) {
      return cell->mHandle;
    }
  }
  return NULL;
}

// @brief ネットワークを削除する．
void
MagMgr::_delete_nethandle(NetHandle* net_handle)
{
  ymuint pos = hash_func(net_handle->name()) % mNetHandleHashSize;
  Cell* cell;
  for (Cell** prev = &mNetHandleHash[pos]; (cell = *prev);
       prev = &cell->mLink) {
    if ( cell->mHandle == net_handle ) {
      *prev = cell->mLink;
      delete cell->mHandle;
      void* p = reinterpret_cast<void*>(cell);
      mAlloc.put_memory(sizeof(Cell), p);
      -- mNetHandleHashNum;
      return;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ハッシュ表を拡大する．
void
MagMgr::alloc_table(ymuint req_size)
{
  ymuint old_size = mNetHandleHashSize;
  Cell** old_table = mNetHandleHash;
  while ( mNetHandleHashSize < req_size ) {
    if ( mNetHandleHashSize == 0 ) {
      mNetHandleHashSize = 1;
    }
    else {
      mNetHandleHashSize <<= 1;
    }
  }

  if ( old_size < mNetHandleHashSize ) {
    mNetHandleHash = new Cell*[mNetHandleHashSize];
    for (ymuint i = 0; i < mNetHandleHashSize; ++ i) {
      mNetHandleHash[i] = NULL;
    }
    mNetHandleHashNextLimit = static_cast<ymuint>(mNetHandleHashSize * 1.8);

    // 再ハッシュを行う．
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* tmp = cell;
	cell = tmp->mLink;
	ymuint pos = hash_func(tmp->mHandle->name()) % mNetHandleHashSize;
	tmp->mLink = mNetHandleHash[pos];
	mNetHandleHash[pos] = tmp;
      }
    }
  }
}

// @brief string のハッシュ関数
ymuint
MagMgr::hash_func(const string& name)
{
  ymuint v = 0;
  ymuint l = name.size();
  for (ymuint i = 0; i < l; ++ i) {
    v += v * 33 + name[i];
  }
  return v;
}

END_NAMESPACE_MAGUS
