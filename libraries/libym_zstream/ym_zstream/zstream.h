#ifndef YM_UTILS_ZSTREAM_H
#define	YM_UTILS_ZSTREAM_H

/// @file ym_utils/zstream.h
/// @brief zlib を使った圧縮/伸長機能つきストリーム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: zstream.h 1343 2008-03-25 17:15:35Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#if defined(HAVE_ZLIB)
#include <zlib.h>
#else
#error "\"ym_utils/zstream.h\" requires <zlib.h>"
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class zlib_error zstream.h <ym_utils/zstream.h>
/// @brief zlib 関係のエラーオブジェクト
//////////////////////////////////////////////////////////////////////
struct zlib_error
{
  zlib_error(const char* label,
	     int status,
	     const char* msg) :
    mLabel(label),
    mStatus(status),
    mMsg(msg)
  {
  }

  const char* mLabel;
  int mStatus;
  const char* mMsg;
};


//////////////////////////////////////////////////////////////////////
/// @class zstream zstream.h <ym_utils/zstream.h>
/// @brief z_stream の C++ 的に進化したもの
//////////////////////////////////////////////////////////////////////
class zstream
{
public:

  /// @brief zlib のバージョンを表す文字列を返す．
  static
  const char*
  version();


public:

  /// @brief コンストラクタ
  zstream();

  /// @brief コピーコンストラクタ
  zstream(const zstream& src);

  /// @brief 代入演算子
  const zstream&
  operator=(const zstream& src);
  
  /// @brief デストラクタ
  ~zstream();


public:

  /// @brief deflate 用の初期化を行う．
  int
  deflate_init(int level);
  
  /// @brief データを圧縮する．
  int
  deflate(int flush);

  /// @brief deflate 用に確保された領域の解放を行う．
  int
  deflate_end();
  
  /// @brief inflate 用の初期化を行う．
  int
  inflate_init();
  
  /// @brief データを伸長する．
  int
  inflate(int flush);
  
  /// @brief inflate 用に確保された領域の解放を行う．
  int
  inflate_end();

  /// @brief 内部の状態に応じて inflateEnd か deflateEnd を呼ぶ．
  int
  end();

  
public:

  /// @brief zalloc, zfree, opaque を設定する．
  void
  set_alloc(alloc_func af,
	    free_func ff,
	    voidp op);

  /// @brief in バッファを設定する．
  void
  set_inbuf(Bytef* buf,
	    uInt size);

  /// @brief out バッファを設定する．
  void
  set_outbuf(Bytef* buf,
	     uInt size);

  /// @brief avail_in の取得
  uInt
  avail_in() const;

  /// @brief avail_out の取得
  uInt
  avail_out() const;
  
  /// @brief msg を得る．
  const char*
  msg() const;

  
public:

  /// @brief 詳細な設定を行う deflate 用の初期化
  int
  deflate_init2(int level,
		int method,
		int windowBits,
		int memLevel,
		int strategy);

  /// @brief deflate 用の辞書をセットする．
  int
  deflate_set_dictionary(const Bytef* dictionary,
			 uInt dictLength);

  /// @brief deflate の状態をリセットする．
  int
  deflate_reset();

  /// @brief deflate 用のパラメータをセットする．
  int
  deflate_params(int level,
		 int strategy);

  /// @brief 圧縮後のサイズの見積りを行う．
  int
  deflate_bound(uLong sourceLen);

  /// @brief deflate 用の低レベル関数
  int
  deflate_prime(int bits,
		int value);


public:

  /// @brief inflate 用の詳細な設定を行う初期化
  int
  inflate_init2(int windowBits);

  /// @brief inflate 用の辞書の設定
  int
  inflate_set_dictionary(const Bytef* dictionary,
			 uInt dictLength);

  /// @brief 不正なデータをスキップする．
  int
  inflate_sync();

  /// @brief inflate 用の状態の初期化
  int
  inflate_reset();


#if ZLIB_VERNUM >= 0x1230
public:

  /// @brief deflate 用の細かなパラメータをセットする．
  int
  deflate_tune(int good_length,
	       int max_lazy,
	       int nice_length,
	       int max_chain);

  /// @brief deflate 用のヘッダのセット
  int
  deflate_set_header(gz_headerp head);

  /// @brief inflate 用の低レベル関数
  int
  inflate_prime(int bits,
		int value);

  /// @brief inflate 用のヘッダの取得
  int
  inflate_get_header(gz_headerp head);
#endif

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の z_stream
  z_stream mZ;

  // deflate/inflate の状態
  int mMode;

};


//////////////////////////////////////////////////////////////////////
/// @class zstream_buff zstream.h <ym_utils/zstream.h>
/// @brief バッファ付きの zstream クラス
//////////////////////////////////////////////////////////////////////
class zstream_buff :
  public zstream
{
public:

  /// @brief 入力用のコンストラクタ
  /// @param[in] s 入力ストリーム
  /// @param[in] size バッファサイズ
  explicit
  zstream_buff(istream* s,
	       size_t size = kBufSize);

  /// @brief 出力用のコンストラクタ
  /// @param[in] s 出力ストリーム
  /// @param[in] size バッファサイズ
  explicit
  zstream_buff(ostream* s,
	       size_t size = kBufSize);

  /// @brief デストラクタ
  ~zstream_buff();


public:

  /// @brief データの圧縮を行う．
  /// @param[in] buff 圧縮するデータを格納しているバッファ
  /// @param[in] size バッファ中の有効なデータのサイズ
  /// @param[in] flush フラッシュ制御用のフラグ
  /// @note 結果は出力ストリームに書き込まれる．
  void
  compress(Bytef* buff,
	   int size,
	   int flush);

  /// @brief データの伸長を行う．
  /// @param[in] buff 伸長したデータを格納するバッファ
  /// @param[in] size バッファ中の空きサイズ(in bytes)
  /// @return バッファに書き出されたサイズ(in bytes)を返す．
  /// @note データは入力ストリームから読み込まれる．
  int
  decompress(Bytef* buff,
	     int size);
  
  /// @brief 入力ストリームを取り出す．
  istream*
  in();

  /// @brief 出力ストリームを取り出す．
  ostream*
  out();

  
private:

  // バッファの内容を書き出す．
  void
  write();
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  static
  const size_t kBufSize = 4096;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 圧縮データを入力するストリーム
  istream* mInStream;

  // 圧縮データを出力するストリーム
  ostream* mOutStream;

  // 内側の(圧縮されている)バッファ
  Bytef* mBuff;

  // mBuff のサイズ
  int mSize;

  // 伸長モードの時のフラッシュフラグ
  int mInFlush;
  
};


//////////////////////////////////////////////////////////////////////
/// @class basic_zlib_streambuf zstream.h <ym_utils/zstream.h>
/// @brief zlib を使ったストリームバッファのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_zlib_streambuf :
  public std::basic_streambuf<_CharT, _Traits>
{
public:
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;

public:

  /// @brief 入力用のコンストラクタ
  /// @param[in] s 入力ストリーム
  /// @param[in] size バッファサイズ
  explicit
  basic_zlib_streambuf(istream& s,
		       size_t size = kBufSize) :
    mZbuf(&s, size),
    mBuff(NULL),
    mSize(size),
    mBuff0(new char_type[size])
  {
    mBuff = mBuff0;
    int wsize = mZbuf.decompress(byte_buff(), byte_size(mSize));
    setg(mBuff, mBuff, mBuff + char_size(wsize));
  }

  /// @brief 出力用のコンストラクタ
  /// @param[in] s 出力ストリーム
  /// @param[in] size バッファサイズ
  explicit
  basic_zlib_streambuf(ostream& s,
		       size_t size = kBufSize) :
    mZbuf(&s, size),
    mBuff(NULL),
    mSize(size),
    mBuff0(new char_type[size])
  {
    mBuff = mBuff0;
    mOutFlush = Z_NO_FLUSH;
    setp(mBuff, mBuff + mSize);
  }

  /// @brief デストラクタ
  ~basic_zlib_streambuf()
  {
    mOutFlush = Z_FINISH;

    sync();

    mZbuf.end();

    delete [] mBuff0;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // basic_streambuf<_CharT, _Traits> の仮想関数
  //////////////////////////////////////////////////////////////////////

  virtual
  streambuf_type*
  setbuf(char_type* buf,
	 int size)
  {
    if ( mZbuf.in() ) {
      setg(buf, buf, buf + size);
    }
    if ( mZbuf.out() ) {
      setp(buf, buf + size);
    }
    mBuff = buf;
    mSize = size;
    return this;
  }

  virtual
  int_type
  overflow(int_type c = traits_type::eof())
  {
    // バッファに溜っているデータを圧縮する．
    mZbuf.compress(byte_buff(), byte_size(this->pptr() - mBuff), mOutFlush);
    setp(mBuff, mBuff + mSize);
    
    if ( c == traits_type::eof() ) {
      return c;
    }

    // 一文字書き込む
    *(this->pptr()) = traits_type::to_char_type(c);
    this->pbump(1);

    return traits_type::not_eof(c);
  }

  virtual
  int_type
  underflow()
  {
    if ( this->egptr() <= this->gptr() ) {
      // データを伸長してバッファに入れる．
      int wsize = mZbuf.decompress(byte_buff(), byte_size(mSize));
      setg(mBuff, mBuff, mBuff + char_size(wsize));
      if ( wsize == 0 ) {
	return traits_type::eof();
      }
    }

    // 一文字読み出す．
    return traits_type::to_int_type(*(this->gptr()));
  }

  virtual
  int
  sync()
  {
    if ( mZbuf.out() ) {
      mZbuf.compress(byte_buff(), byte_size(this->pptr() - mBuff), mOutFlush);
      setp(mBuff, mBuff + mSize);
    }
    return 0;
  }


protected:

  // mBuff を zstream 用にキャストする．
  Bytef*
  byte_buff()
  {
    return reinterpret_cast<Bytef*>(mBuff);
  }
  
  // char_type 単位のサイズを Bytef 単位のサイズに変換する
  int
  byte_size(int size)
  {
    return size * sizeof(char_type) / sizeof(Bytef);
  }

  // Bytef 単位のサイズを char_type 単位のサイズに変換する
  int
  char_size(int size)
  {
    return size * sizeof(Bytef) / sizeof(char_type);
  }
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  enum {
    kBufSize = 4096
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  zstream_buff mZbuf;

  // 外側の(plainな)バッファ
  char_type* mBuff;

  // mBuff のサイズ
  int mSize;

  // デフォルトのバッファ
  char_type* mBuff0;

  // フラッシュフラグ
  int mOutFlush;

};


//////////////////////////////////////////////////////////////////////
/// @class basic_izstream zstream.h <ym_utils/zstream.h>
/// @brief zlib を使った入力ストリームのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_izstream :
  public std::basic_istream<_CharT, _Traits>
{
public:
  typedef _CharT char_type;
  typedef _Traits traits_type;
  
  typedef std::basic_istream<char_type, traits_type> istream_type;
public:

  /// @brief コンストラクタ
  /// @param[in] s 伸長元の入力ストリーム
  explicit
  basic_izstream(istream& s) :
    istream_type(new basic_zlib_streambuf<char_type, traits_type>(s))
  {
  }

  /// @brief デストラクタ
  ~basic_izstream()
  {
    delete this->rdbuf();
  }
};


//////////////////////////////////////////////////////////////////////
/// @class basic_ozstream zstream.h <ym_utils/zstream.h>
/// @brief zlib を使った出力ストリームのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_ozstream :
  public std::basic_ostream<_CharT, _Traits>
{
public:
  typedef _CharT char_type;
  typedef _Traits traits_type;

  typedef std::basic_ostream<char_type, traits_type> ostream_type;
public:

  /// @brief コンストラクタ
  /// @param[in] s 圧縮前の出力ストリーム
  explicit
  basic_ozstream(ostream& s) :
    ostream_type(new basic_zlib_streambuf<char_type, traits_type>(s))
  {
  }

  /// @brief デストラクタ
  ~basic_ozstream()
  {
    this->flush();
    delete this->rdbuf();
  }
};

typedef basic_izstream<char> izstream;
typedef basic_ozstream<char> ozstream;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief zlib のバージョンを表す文字列を返す．
inline
const char*
zstream::version()
{
  return zlibVersion();
}

// @brief コンストラクタ
inline
zstream::zstream() :
  mMode(0)
{
  mZ.zalloc = Z_NULL;
  mZ.zfree = Z_NULL;
  mZ.opaque = 0;
}

// @brief コピーコンストラクタ
inline
zstream::zstream(const zstream& src) :
  mMode(src.mMode)
{
  if ( mMode == 1 ) {
    deflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
  }
  else if ( mMode == 2 ) {
    inflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
  }
}

// @brief 代入演算子
inline
const zstream&
zstream::operator=(const zstream& src)
{
  if ( mMode == 1 ) {
    deflateEnd(&mZ);
  }
  else if ( mMode == 2 ) {
    inflateEnd(&mZ);
  }
  mMode = src.mMode;
  if ( mMode == 1 ) {
    deflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
  }
  else if ( mMode == 2 ) {
    inflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
  }
  return *this;
}
  
// @brief デストラクタ
inline
zstream::~zstream()
{
  end();
}

// @brief deflate 用の初期化を行う．
inline
int
zstream::deflate_init(int level)
{
  mMode = 1;
  int status = deflateInit(&mZ, level);
  if ( status < 0 ) {
    throw zlib_error("deflateInit", status, msg());
  }
  return status;
}
  
// @brief データを圧縮する．
inline
int
zstream::deflate(int flush)
{
  int status = ::deflate(&mZ, flush);
  if ( status < 0 ) {
    throw zlib_error("deflate", status, msg());
  }
  return status;
}

// @brief deflate 用に確保された領域の解放を行う．
inline
int
zstream::deflate_end()
{
  mMode = 0;
  int status = deflateEnd(&mZ);
  if ( status < 0 ) {
    throw zlib_error("deflateEnd", status, msg());
  }
  return status;
}
  
// @brief inflate 用の初期化を行う．
inline
int
zstream::inflate_init()
{
  mMode = 2;
  int status = inflateInit(&mZ);
  if ( status < 0 ) {
    throw zlib_error("inflateInit", status, msg());
  }
  return status;
}
  
// @brief データを伸長する．
inline
int
zstream::inflate(int flush)
{
  int status = ::inflate(&mZ, flush);
  if ( status < 0 ) {
    throw zlib_error("inflate", status, msg());
  }
  return status;
}
  
// @brief inflate 用に確保された領域の解放を行う．
inline
int
zstream::inflate_end()
{
  mMode = 0;
  int status = inflateEnd(&mZ);
  if ( status < 0 ) {
    throw zlib_error("inflateEnd", status, msg());
  }
  return status;
}

// @brief 内部の状態に応じて inflateEnd か deflateEnd を呼ぶ．
inline
int
zstream::end()
{
  if ( mMode == 2 ) {
    return inflate_end();
  }
  else if ( mMode == 1 ) {
    return deflate_end();
  }
  // なにもしない．
  return Z_OK;
}

// @brief zalloc, zfree, opaque を設定する．
inline
void
zstream::set_alloc(alloc_func af,
		   free_func ff,
		   voidp op)
{
  mZ.zalloc = af;
  mZ.zfree = ff;
  mZ.opaque = op;
}

// @brief in バッファを設定する．
inline
void
zstream::set_inbuf(Bytef* buf,
		   uInt size)
{
  mZ.next_in = buf;
  mZ.avail_in = size;
}

// @brief out バッファを設定する．
inline
void
zstream::set_outbuf(Bytef* buf,
		    uInt size)
{
  mZ.next_out = buf;
  mZ.avail_out = size;
}

// @brief avail_in の取得
inline
uInt
zstream::avail_in() const
{
  return mZ.avail_in;
}

// @brief avail_out の取得
inline
uInt
zstream::avail_out() const
{
  return mZ.avail_out;
}

// @brief msg を得る．
inline
const char*
zstream::msg() const
{
  return mZ.msg;
}

// @brief 詳細な設定を行う deflate 用の初期化
inline
int
zstream::deflate_init2(int level,
		       int method,
		       int windowBits,
		       int memLevel,
		       int strategy)
{
  mMode = 1;
  int status = deflateInit2(&mZ, level, method, windowBits, memLevel, strategy);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief deflate 用の辞書をセットする．
inline
int
zstream::deflate_set_dictionary(const Bytef* dictionary,
				uInt dictLength)
{
  int status = deflateSetDictionary(&mZ, dictionary, dictLength);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
  
}

// @brief deflate の状態をリセットする．
inline
int
zstream::deflate_reset()
{
  int status = deflateReset(&mZ);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief deflate 用のパラメータをセットする．
inline
int
zstream::deflate_params(int level,
			int strategy)
{
  int status = deflateParams(&mZ, level, strategy);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief 圧縮後のサイズの見積りを行う．
inline
int
zstream::deflate_bound(uLong sourceLen)
{
  int status = deflateBound(&mZ, sourceLen);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief deflate 用の低レベル関数
inline
int
zstream::deflate_prime(int bits,
		      int value)
{
  int status = deflatePrime(&mZ, bits, value);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief inflate 用の詳細な設定を行う初期化
inline
int
zstream::inflate_init2(int windowBits)
{
  mMode = 2;
  int status = inflateInit2(&mZ, windowBits);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief inflate 用の辞書の設定
inline
int
zstream::inflate_set_dictionary(const Bytef* dictionary,
				uInt dictLength)
{
  int status = inflateSetDictionary(&mZ, dictionary, dictLength);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief 不正なデータをスキップする．
inline
int
zstream::inflate_sync()
{
  int status = inflateSync(&mZ);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief inflate 用の状態の初期化
inline
int
zstream::inflate_reset()
{
  int status = inflateReset(&mZ);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

#if ZLIB_VERNUM >= 0x1230

// @brief deflate 用の細かなパラメータをセットする．
inline
int
zstream::deflate_tune(int good_length,
		      int max_lazy,
		      int nice_length,
		      int max_chain)
{
  int status = deflateTune(&mZ, good_length, max_lazy, nice_length, max_chain);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief deflate 用のヘッダのセット
inline
int
zstream::deflate_set_header(gz_headerp head)
{
  int status = deflateSetHeader(&mZ, head);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief inflate 用の低レベル関数
inline
int
zstream::inflate_prime(int bits,
		       int value)
{
  int status = inflatePrime(&mZ, bits, value);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}

// @brief inflate 用のヘッダの取得
inline
int
zstream::inflate_get_header(gz_headerp head)
{
  int status = inflateGetHeader(&mZ, head);
  if ( status < 0 ) {
    throw zlib_error("deflateInit2", status, msg());
  }
  return status;
}
#endif

// 入力ストリームを取り出す．
inline
istream*
zstream_buff::in()
{
  return mInStream;
}

// 出力ストリームを取り出す．
inline
ostream*
zstream_buff::out()
{
  return mOutStream;
}

// バッファの内容を書き出す．
inline
void
zstream_buff::write()
{
  mOutStream->write(reinterpret_cast<char*>(mBuff), mSize - avail_out());
}

END_NAMESPACE_YM

#endif // YM_UTILS_ZSTREAM_H
