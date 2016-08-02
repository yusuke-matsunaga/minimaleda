#ifndef YM_UTILS_ALLOC_H
#define YM_UTILS_ALLOC_H

/// @file ym_utils/Alloc.h
/// @brief Alloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Alloc.h 1052 2007-10-24 11:08:51Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class AllocBase Alloc.h <ym_utils/Alloc.h>
/// @brief メモリの管理を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class AllocBase
{
public:

  /// @brief デストラクタ
  virtual
  ~AllocBase() { }


public:

  /// @brief n バイトの領域を確保する．
  virtual
  void*
  get_memory(size_t n) = 0;

  /// @brief n バイトの領域を開放する．
  virtual
  void
  put_memory(size_t n,
	     void* blk) = 0;

  /// @brief 今までに確保した全ての領域を破棄する．
  /// @note 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy() = 0;


public:

  /// @brief 使用されているメモリ量を返す．
  virtual
  size_t
  used_size() const = 0;

  /// @brief used_size() の最大値を返す．
  virtual
  size_t
  max_used_size() const = 0;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  size_t
  allocated_size() const = 0;

  /// @brief 実際に確保した回数を返す．
  virtual
  size_t
  allocated_count() const = 0;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class SimpleAlloc Alloc.h <ym_utils/Alloc.h>
/// @brief 単純なメモリ管理
//////////////////////////////////////////////////////////////////////
class SimpleAlloc :
  public AllocBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size このオブジェクトが管理する最大サイズ
  /// @note max_size 以上のメモリ領域はデフォルトのアロケーターを
  /// 使用する．
  SimpleAlloc(size_t max_size);

  /// @brief デストラクタ
  virtual
  ~SimpleAlloc();


public:

  /// @brief n バイトの領域を確保する．
  virtual
  void*
  get_memory(size_t n);

  /// @brief n バイトの領域を開放する．
  virtual
  void
  put_memory(size_t n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// @note 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy();


public:

  /// @brief 使用されているメモリ量を返す．
  virtual
  size_t
  used_size() const;

  /// @brief used_size() の最大値を返す．
  virtual
  size_t
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  size_t
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  virtual
  size_t
  allocated_count() const;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const;


private:

  /// @brief アラインメントを考慮してサイズを調節する．
  static
  size_t
  align(size_t req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コンストラクタの引数
  size_t mMaxSize;

  // mMaxSize を越えない2の巾乗の数．
  size_t mMaxPowerSize;

  // mMaxPowerSize のログ
  size_t mMaxLogSize;

  // 現在利用可能なメモリ領域
  char* mCurBlock;

  // mCurBlock の次に利用可能な位置
  size_t mNextPos;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

  // 使用中のメモリサイズ
  size_t mUsedSize;
  
  // 使用した最大のメモリサイズ
  size_t mMaxUsedSize;
  
  // 確保したメモリサイズ
  size_t mAllocSize;

  // 確保した回数
  size_t mAllocCount;

};


//////////////////////////////////////////////////////////////////////
/// @class FragAlloc Alloc.h <ym_utils/Alloc.h>
/// @brief 2の巾乗の単位で管理する
//////////////////////////////////////////////////////////////////////
class FragAlloc :
  public AllocBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size このオブジェクトが管理する最大サイズ
  /// @note max_size 以上のメモリ領域はデフォルトのアロケーターを
  /// 使用する．
  FragAlloc(size_t max_size);

  /// @brief デストラクタ
  ~FragAlloc();


public:

  /// @brief n バイトの領域を確保する．
  virtual
  void*
  get_memory(size_t n);

  /// @brief n バイトの領域を開放する．
  virtual
  void
  put_memory(size_t n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// @note 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy();


public:

  /// @brief 使用されているメモリ量を返す．
  virtual
  size_t
  used_size() const;

  /// @brief used_size() の最大値を返す．
  virtual
  size_t
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  size_t
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  virtual
  size_t
  allocated_count() const;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const;


private:

  // サイズ 2^p のブロックを確保する．
  char*
  alloc_block(size_t p);

  // サイズ 2^p のブロックがあれば返す．
  // なければ NULL を返す．
  char*
  get_block(size_t p);

  // サイズ 2^p のブロックをリストに戻す．
  void
  put_block(size_t p,
	    char* block);


private:

  // 利用可能なメモリ領域を管理するための構造体
  struct Block
  {
    // 次の要素を指すポインタ
    Block* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最小のサイズ
  size_t mMinSize;

  // mMinSize の log
  size_t mMinLogSize;
  
  // コンストラクタの引数
  size_t mMaxSize;

  // mMaxSize を越えない2の巾乗の数．
  size_t mMaxPowerSize;

  // mMaxPowerSize の log
  size_t mMaxLogSize;

  // サイズごとに分けられたブロックリストの配列
  // 配列の大きさは mMaxLogSize + 1
  Block** mBlockListArray;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

  // 使用中のメモリサイズ
  size_t mUsedSize;
  
  // 使用した最大のメモリサイズ
  size_t mMaxUsedSize;
  
  // 確保したメモリサイズ
  size_t mAllocSize;

  // 確保した回数
  size_t mAllocCount;

};


//////////////////////////////////////////////////////////////////////
/// @class UnitAlloc Alloc.h <ym_utils/Alloc.h>
/// @brief 単一サイズのメモリ領域の管理を行うクラス
//////////////////////////////////////////////////////////////////////
class UnitAlloc :
  public AllocBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] unit_size メモリ割り当ての単位となるサイズ
  /// @param[in] block_size 一度に確保する個数
  UnitAlloc(size_t unit_size,
	    size_t block_size);

  /// @brief デストラクタ
  virtual
  ~UnitAlloc();


public:

  /// @brief n バイトの領域を確保する．
  /// @note n != unit_size の場合にはデフォルトアロケータを用いる．
  virtual
  void*
  get_memory(size_t n);

  /// @brief n バイトの領域を開放する．
  /// @note n != unit_size の場合にはデフォルトアロケータを用いる．
  virtual
  void
  put_memory(size_t n,
	     void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// @note 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  destroy();


public:

  /// @brief 使用されているメモリ量を返す．
  virtual
  size_t
  used_size() const;

  /// @brief used_size() の最大値を返す．
  virtual
  size_t
  max_used_size() const;

  /// @brief 実際に確保したメモリ量を返す．
  virtual
  size_t
  allocated_size() const;

  /// @brief 実際に確保した回数を返す．
  virtual
  size_t
  allocated_count() const;

  /// @brief 内部状態を出力する．
  virtual
  void
  print_stats(ostream& s) const;


private:

  // サイズ 2^p のブロックがあれば返す．
  // なければ NULL を返す．
  char*
  get_block(size_t p);

  // サイズ 2^p のブロックをリストに戻す．
  void
  put_block(size_t p,
	    char* block);


private:

  // 利用可能なメモリ領域を管理するための構造体
  struct Block
  {
    // 次の要素を指すポインタ
    Block* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 単位サイズ
  size_t mUnitSize;

  // ブロックサイズ
  size_t mBlockSize;

  // 利用可能なメモリ領域のリスト
  Block* mAvailTop;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

  // 使用中のメモリサイズ
  size_t mUsedSize;
  
  // 使用した最大のメモリサイズ
  size_t mMaxUsedSize;
  
  // 確保したメモリサイズ
  size_t mAllocSize;

  // 確保した回数
  size_t mAllocCount;

};

END_NAMESPACE_YM

#endif // YM_UTILS_ALLOC_H
