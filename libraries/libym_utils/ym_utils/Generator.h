#ifndef YM_UTILS_GENERATOR_H
#define YM_UTILS_GENERATOR_H

/// @file ym_utils/Generator.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Generator.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup GeneratorGroup 組み合わせ生成器と順列生成器
/// @ingroup YmUtils
///
/// - n 個の中から k 個の組み合わせを選びだす CombiGen
/// - n 個の中から k 個の順列を選びだす PermGen
/// - (n1, n2, ...) 個の中から (k1, k2, ...) 個の組合わせを選びだす
///   MultiCombiGen
/// - (n1, n2, ...) 個の中から (k1, k2, ...) 個の順列を選びだす
///   MultiPermGen
///
/// 使いかたは全てのクラスで共通で，コンストラクタでオブジェクトを
/// 生成した後，メンバ関数の begin() で反復子を取り出し，その反復子が
/// 末尾に達するまで(!is_end())，繰り返せばよい．
/// 反復子から値を取り出すには () 演算子を用いる．


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GenBase Generator.h <ym_utils/Generator.h>
/// @ingroup GeneratorGroup
/// @brief CombiGen と PermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class GenBase
{
public:

  /// @brief 順列/組合わせ列挙するための反復子の基底クラス
  class iterator
  {
  public:
    /// @brief 空のコンストラクタ
    /// @note なにも指さない．
    iterator();

    /// @brief 要素の取得
    /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
    /// @return pos 番目の要素
    size_t
    operator()(size_t pos) const;


  protected:
    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @note 継承クラスが用いる．
    iterator(const GenBase* parent);

    /// @brief 内容をコピーする関数
    /// @param[in] src コピー元のオブジェクト
    void
    copy(const iterator& src);

    /// @brief 全要素数を得る．
    /// @return 全要素数
    size_t
    n() const;

    /// @brief 順列/組合わせ数を得る．
    /// @return 順列/組み合わせ数
    size_t
    k() const;

    /// @brief 要素の取得
    /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
    /// @return pos 番目の要素
    /// @note operator() の別名
    size_t
    elem(size_t pos) const;

    /// @brief 要素の参照の取得
    /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
    /// @return pos 番目の要素への参照
    size_t&
    elem(size_t pos);


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 現在の要素
    vector<size_t> mElem;

    // 親の GenBase
    const GenBase* mParent;

  };

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  GenBase(size_t n,
	  size_t k);
  
  /// @brief デストラクタ
  ~GenBase();

  /// @brief 全要素数を得る．
  /// @return 全要素数
  size_t
  n() const;

  /// @brief 選択する要素数を得る．
  /// @return 選択する要素数
  size_t
  k() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全要素数
  size_t mN;

  // 選択する要素数
  size_t mK;

};


//////////////////////////////////////////////////////////////////////
/// @class CombiGen Generator.h <ym_utils/Generator.h>
/// @ingroup GeneratorGroup
/// @brief 組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class CombiGen :
  public GenBase
{
public:

  /// @brief CombiGen の反復子
  class iterator :
    public GenBase::iterator
  {
    friend class CombiGen;
  public:
    /// @brief 空のコンストラクタ
    iterator();
    
    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    iterator(const iterator& src);

    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:

    /// @brief コンストラクタ
    /// @param[in] parent 親の CombiGen オブジェクト
    /// @note CombiGen が用いる．
    iterator(const CombiGen* parent);

    /// @brief operator++() のサブルーティン
    void
    next(size_t pos);

  };


public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  CombiGen(size_t n,
	   size_t k);
  
  /// @brief デストラクタ
  ~CombiGen();
  
  /// @brief 最初の組み合わせを取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class PermGen Generator.h <ym_utils/Generator.h>
/// @ingroup GeneratorGroup
/// @brief 順列生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class PermGen :
  public GenBase
{
public:

  /// @brief PermGen の反復子
  class iterator :
    public GenBase::iterator
  {
    friend class PermGen;
  public:
    /// @brief 空のコンストラクタ
    iterator();

    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    iterator(const iterator& src);
    
    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:

    /// @brief コンストラクタ
    /// @param[in] parent 親の PermGen オブジェクト
    /// @note PermGen が用いる．
    iterator(const PermGen* parent);

  };


public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選択する要素数
  PermGen(size_t n,
	  size_t k);
  
  /// @brief デストラクタ
  ~PermGen();
  
  /// @brief 最初の順列を取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MultiGenBase Generator.h <ym_utils/Generator.h>
/// @ingroup GeneratorGroup
/// @brief MultiCombiGen/MultiPermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class MultiGenBase
{
public:

  /// @brief 順列/組合わせを表すクラス
  class iterator {
  public:

    /// @brief 空のコンストラクタ
    iterator();

    /// @brief デストラクタ
    ~iterator();

    /// @brief 要素の取得
    /// @param[in] grp グループ番号
    /// @param[in] pos 要素の位置
    /// @return grp 番目のグループの pos 番目の要素を取り出す．
    size_t
    operator()(size_t grp,
	       size_t pos) const;


  protected:

    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @note 継承クラスが用いる．
    iterator(const MultiGenBase* parent);

    /// @brief コピーする．
    /// @param[in] src コピー元のオブジェクト
    void
    copy(const iterator& src);
    
    /// @brief グループ数を得る．
    /// @return グループ数
    size_t
    ngrp() const;

    /// @brief 要素配列の初期化
    /// @param[in] grp グループ番号
    /// @note grp 番目のグループの要素配列を初期化する．
    void
    init(size_t grp);

    /// @brief 要素数の取得
    /// @param[in] grp グループ番号
    /// @return grp 番目のグループの全要素数
    size_t
    n(size_t grp) const;

    /// @brief 選択する要素数の取得
    /// @param[in] grp グループ番号
    /// @return grp 番目のグループの選択する要素数
    size_t
    k(size_t grp) const;
    
    /// @brief 要素配列の取得
    /// @param[in] grp グループ番号
    /// @return grp 番目のグループの要素配列
    vector<size_t>&
    elem(size_t grp);

    /// @brief 要素配列の取得
    /// @param[in] grp グループ番号
    /// @return grp 番目のグループの要素配列
    /// @note こちらは const 版
    const vector<size_t>&
    elem(size_t grp) const;


  private:

    /// @brief 確保したメモリを解放する
    void
    free();


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 現在の要素(二重の配列なので少しめんどくさい)
    vector<vector<size_t>*> mElemArray;

    // 親の MultiGenBase
    const MultiGenBase* mParent;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiGenBase(const vector<pair<size_t, size_t> >& nk_array);
  
  /// @brief デストラクタ
  ~MultiGenBase();

  /// @brief グループ数の取得
  /// @return グループ数
  size_t
  ngrp() const;

  /// @brief 全要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの全要素数
  size_t
  n(size_t grp) const;

  /// @brief 選択する要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの選択する要素数
  size_t
  k(size_t grp) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各グループごとの全要素数を選択する要素数を入れる配列
  vector<pair<size_t, size_t> > mNkArray;

};


//////////////////////////////////////////////////////////////////////
/// @class MultiCombiGen Generator.h <ym_utils/Generator.h>
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの組み合わせを生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiCombiGen :
  public MultiGenBase
{
public:

  /// @brief 一つの組合わせを表すクラス
  class iterator :
    public MultiGenBase::iterator
  {
    friend class MultiCombiGen;
  public:

    /// @brief 空のコンストラクタ
    iterator();

    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    iterator(const iterator& src);

    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:
    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @brief MultiCombiGen が用いる．
    iterator(const MultiCombiGen* parent);

    /// @brief operator++() のサブルーティン
    void
    next(size_t g,
	 size_t pos);

    /// @brief grp 番目のグループが終了状態の時 true を返す．
    bool
    is_end_sub(size_t grp) const;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiCombiGen(const vector<pair<size_t, size_t> >& nk_array);
  
  /// @brief デストラクタ
  ~MultiCombiGen();
  
  /// @brief 最初の組み合わせを取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MultiPermGen Generator.h <ym_utils/Generator.h>
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの順列を生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiPermGen :
  public MultiGenBase
{
public:

  /// @brief 一つの順列を表すクラス
  class iterator :
    public MultiGenBase::iterator
  {
    friend class MultiPermGen;
  public:
    /// @brief 空のコンストラクタ
    iterator();

    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    iterator(const iterator& src);

    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:
    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @brief MultiPermGen が用いる．
    iterator(const MultiPermGen* parent);

    /// @brief operator++() のサブルーティン
    void
    next(size_t pos);

    /// @brief grp 番目のグループが終了状態の時 true を返す．
    bool
    is_end_sub(size_t grp) const;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiPermGen(const vector<pair<size_t, size_t> >& nk_array);
  
  /// @brief デストラクタ
  ~MultiPermGen();
  
  /// @brief 最初の順列を取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 全要素数を得る．
inline
size_t
GenBase::iterator::n() const
{
  return mParent->n();
}

// 順列/組合わせ数を得る．
inline
size_t
GenBase::iterator::k() const
{
  return mParent->k();
}

// pos 番目の要素を取り出す．
inline
size_t
GenBase::iterator::operator()(size_t pos) const
{
  return elem(pos);
}

// pos 番目の要素を取り出す．
inline
size_t
GenBase::iterator::elem(size_t pos) const
{
  return mElem[pos];
}

// pos 番目の要素への参照を取り出す．
inline
size_t&
GenBase::iterator::elem(size_t pos)
{
  return mElem[pos];
}

// 全要素数を得る．
inline
size_t
GenBase::n() const
{
  return mN;
}

// 選択する要素数を得る．
inline
size_t
GenBase::k() const
{
  return mK;
}

// grp 番目のグループの pos 番目の要素を取り出す．
inline
size_t
MultiGenBase::iterator::operator()(size_t grp,
				   size_t pos) const
{
  return (*mElemArray[grp])[pos];
}

// グループ数を得る．
inline
size_t
MultiGenBase::iterator::ngrp() const
{
  return mParent->ngrp();
}

// grp 番目のグループの全要素数を得る．
inline
size_t
MultiGenBase::iterator::n(size_t grp) const
{
  return mParent->n(grp);
}

// grp 番目のグループの選択する要素数を得る．
inline
size_t
MultiGenBase::iterator::k(size_t grp) const
{
  return mParent->k(grp);
}

// グループ数を得る．
inline
size_t
MultiGenBase::ngrp() const
{
  return mNkArray.size();
}

// grp 番目のグループの全要素数を得る．
inline
size_t
MultiGenBase::n(size_t grp) const
{
  return mNkArray[grp].first;
}

// grp 番目のグループの選択する要素数を得る．
inline
size_t
MultiGenBase::k(size_t grp) const
{
  return mNkArray[grp].second;
}

END_NAMESPACE_YM

#endif // YM_UTILS_GENERATOR_H
