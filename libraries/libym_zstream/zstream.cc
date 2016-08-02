
/// @file libym_utils/zstream.cc
/// @brief zstream 実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: zstream.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_zstream/zstream.h"


BEGIN_NAMESPACE_YM

// @brief 入力用のコンストラクタ
// @param[in] s 入力ストリーム
zstream_buff::zstream_buff(istream* s,
			   size_t size) :
  mInStream(s),
  mOutStream(NULL),
  mBuff(new Bytef[size]),
  mSize(size),
  mInFlush(Z_NO_FLUSH)
{
  inflate_init();
  
  set_inbuf(Z_NULL, 0);
}

// @brief 出力用のコンストラクタ
// @param[in] s 出力ストリーム
zstream_buff::zstream_buff(ostream* s,
			   size_t size) :
  mInStream(NULL),
  mOutStream(s),
  mBuff(new Bytef[size]),
  mSize(size),
  mInFlush(Z_NO_FLUSH)
{
  deflate_init(Z_DEFAULT_COMPRESSION);
  
  set_outbuf(mBuff, mSize);
}

// @brief デストラクタ
zstream_buff::~zstream_buff()
{
  delete [] mBuff;
}

// @brief データの圧縮を行う．
// @param[in] buff 圧縮するデータを格納しているバッファ
// @param[in] size バッファ中の有効なデータのサイズ
// @param[in] flush フラッシュ制御用のフラグ
// @note 結果は出力ストリームに書き込まれる．
void
zstream_buff::compress(Bytef* buff,
		       int size,
		       int flush)
{
  set_inbuf(buff, size);
  for( ; ; ){
    int status = deflate(flush); 
    if ( status == Z_STREAM_END ) {
      // 完了
      write();
      break;
    }
    if ( avail_out() == 0 ) {
      // 出力バッファが尽きた
      write();
      set_outbuf(mBuff, mSize);
    }
    if ( avail_in() == 0 && flush != Z_FINISH ) {
      // 入力バッファが尽きた
      break;
    }
  }
}
  
// @brief データの伸長を行う．
// @param[in] buff 伸長したデータを格納するバッファ
// @param[in] バッファ中の空きサイズ(in byte)
// @return バッファに書き出されたサイズ(in bytes)を返す．
// @note データは入力ストリームから読み込まれる．
int
zstream_buff::decompress(Bytef* buff,
			 int size)
{
  set_outbuf(buff, size);
  int orig_size = avail_out();
  for( ; ; ){
    if ( avail_in() == 0 ) {
      // 入力バッファが尽きた
      mInStream->read(reinterpret_cast<char*>(mBuff), mSize);
      set_inbuf(mBuff, mInStream->gcount());
      if ( avail_in() == 0 ) {
	mInFlush = Z_FINISH;
      }
    }
    int status = inflate(mInFlush); 
    if ( status == Z_STREAM_END ) {
      // 完了
      return orig_size - avail_out();
    }
    if ( avail_out() == 0 ) {
      // 出力バッファが尽きた
      return orig_size;
    }
  }
}

END_NAMESPACE_YM
