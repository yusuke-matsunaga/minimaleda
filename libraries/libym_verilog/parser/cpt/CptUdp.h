#ifndef LIBYM_VERILOG_PT_COMPACT_CPTUDP_H
#define LIBYM_VERILOG_PT_COMPACT_CPTUDP_H

/// @file libym_verilog/pt_compact/CptUdp.h
/// @brief CptUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CptUdp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/pt/PtUdp.h"
#include "ym_verilog/pt/PtArray.h"
#include "ym_verilog/pt/PtP.h"

#include "ym_utils/FileRegion.h"

#include "PtiFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class CptUdp :
  public PtUdp
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptUdp(const FileRegion& file_region,
	 const char* name,
	 PtiPortArray port_array,
	 PtIOHeadArray iohead_array,
	 bool is_seq,
	 PtExpr* init_value,
	 PtUdpEntryArray entry_array);
  
  /// @brief デストラクタ
  virtual
  ~CptUdp();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief primitivie type を返す．
  virtual
  tVpiPrimType
  prim_type() const;

  /// @brief 名前を返す．
  virtual
  const char*
  name() const;

  /// @brief ポート数を取り出す．
  virtual
  ymuint32
  port_num() const;
  
  /// @brief ポートを取り出す．
  virtual
  const PtPort*
  port(ymuint32 pos) const;
  
  /// @brief 入出力宣言ヘッダ配列の取得
  virtual
  PtIOHeadArray
  iohead_array() const;

  /// @brief 初期値を取出す．
  virtual
  const PtExpr*
  init_value() const;

  /// @brief テーブルを取り出す．
  virtual
  PtUdpEntryArray
  table_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブ名
  const char* mName;
  
  // ポートの配列
  PtiPortArray mPortArray;

  // 入出力宣言の配列
  PtIOHeadArray mIOHeadArray;

  // sequential primitive の時 true
  bool mSeq;

  // 初期値
  PtExpr* mInitValue;

  // テーブル要素の配列
  PtUdpEntryArray mTableArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP の中身のテーブルを表すクラス(組合せ回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntry :
  public PtUdpEntry
{
  friend class CptFactory;

protected:
  
  /// @brief コンストラクタ
  CptUdpEntry(const FileRegion& file_region,
	      PtUdpValueArray input_array,
	      PtUdpValue* output);
  
  /// @brief デストラクタ
  virtual
  ~CptUdpEntry();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;
  
  /// @brief 入力値の配列を取り出す．
  virtual
  PtUdpValueArray
  input_array() const;

  /// @brief 現状態の値を取り出す．
  /// @note このクラスでは NULL を返す．
  virtual
  const PtUdpValue*
  current() const;
  
  /// @brief 出力の値を取り出す．
  virtual
  PtUdpValue*
  output() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;
  
  // 入力パタンの配列
  PtUdpValueArray mInputArray;

  // 出力のパタン
  PtUdpValue* mOutput;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP の中身のテーブルを表すクラス(順序回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntryS :
  public CptUdpEntry
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptUdpEntryS(const FileRegion& file_region,
	       PtUdpValueArray input_array,
	       PtUdpValue* current,
	       PtUdpValue* output);
  
  /// @brief デストラクタ
  virtual
  ~CptUdpEntryS();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現状態の値を取り出す．
  virtual
  PtUdpValue*
  current() const;
    

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現状態のパタン
  PtUdpValue* mCurrent;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class CptUdpValue :
  public PtUdpValue
{
  friend class CptFactory;

private:
  
  /// @brief コンストラクタ
  CptUdpValue(const FileRegion& file_region,
	      tVpiUdpVal symbol);
  
  /// @brief デストラクタ
  virtual
  ~CptUdpValue();
  

public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpValue の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;

  /// @brief シンボルを取り出す．
  virtual
  tVpiUdpVal
  symbol() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // シンボル
  tVpiUdpVal mSymbol;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PT_COMPACT_CPTUDP_H
