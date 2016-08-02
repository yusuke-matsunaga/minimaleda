#ifndef YM_VERILOG_VPI_VPIITERATOR_H
#define YM_VERILOG_VPI_VPIITERATOR_H

/// @file ym_verilog/vpi/VpiIterator.h
/// @brief VpiIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VpiIterator.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include <ym_verilog/vpi/VpiBase.h>
#include <ym_verilog/vpi/VpiObjP.h>


BEGIN_NAMESPACE_YM_VERILOG

template <typename T, int X>
class VpiIteratorImpl;

template <typename T, int X>
class VpiIteratorImplPlus;


//////////////////////////////////////////////////////////////////////
/// @class VpiIterator VpiIterator.h <ym_verilog/vpi/VpiIterator.h>
/// @ingroup VlParser
/// @brief VpiObj のリストに対する反復子の定義
//////////////////////////////////////////////////////////////////////
template <typename T, int X>
class VpiIterator
{
public:

  /// @brief コンストラクタ
  /// @param[in] obj 反復子の実際のオブジェクト
  explicit
  VpiIterator(VpiIteratorImpl<T, X>* obj = NULL) :
    mObj(obj)
  {
    if ( mObj ) {
      mObj->_inc_ref();
    }
  }

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  VpiIterator(const VpiIterator& src) :
    mObj(src.mObj)
  {
    if ( mObj ) {
      mObj->_inc_ref();
    }
  }

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照を返す．
  const VpiIterator&
  operator=(const VpiIterator& src)
  {
    if ( mObj != src.mObj ) {
      if ( mObj ) {
	mObj->_dec_ref();
      }
      mObj = src.mObj;
      if ( mObj ) {
	mObj->_inc_ref();
      }
    }
    return *this;
  }
  
  /// @brief デストラクタ
  ~VpiIterator()
  {
    if ( mObj ) {
      mObj->_dec_ref();
    }
  }

  /// @brief 現在の位置の要素を取り出し一つ進める．
  /// @return 現在の位置の要素を返す．
  /// @return 末尾に達していたら NULL を返す．
  T*
  scan()
  {
    if ( mObj ) {
      return mObj->_scan();
    }
    return 0;
  }

  /// @brief 空の反復子のチェック
  /// @return 空の反復子の時 true を返す．
  bool
  is_empty_iterator() const
  {
    if ( mObj ) {
      return mObj->is_empty_iterator();
    }
    return true;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際に scan() を行う本当のオブジェクト
  VpiIteratorImpl<T, X>* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class VpiIteratorObj VpiIterator.h <ym_verilog/vpi/VpiIterator.h>
/// @ingroup VlParser
/// @brief VpiItarator の実際の機能を実装するクラスのインターフェイス
//////////////////////////////////////////////////////////////////////
class VpiIteratorObj :
  public VpiBase
{
public:
  /// @brief 親クラスのエイリアス
  typedef VpiBase Parent;


protected:

  /// @brief コンストラクタ
  /// @param[in] use この反復子を生成したオブジェクト
  VpiIteratorObj(VpiObj* use);
  
  /// @brief デストラクタ
  virtual
  ~VpiIteratorObj();

  
public:
  //////////////////////////////////////////////////////////////////////
  // このクラスの派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  /// @return 次の要素を返す．
  /// @note vpiIterater タイプの時のみ意味を持つ．
  virtual
  VpiObj*
  scan() = 0;

  /// @brief 空の反復子のチェック
  /// @return 空の反復子の時 true を返す．
  /// @note vpiIterator タイプの時のみ意味を持つ．
  virtual
  bool
  is_empty_iterator() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // VPI 関数をオブジェクト指向にしたもの
  //////////////////////////////////////////////////////////////////////
  
  /// @brief property に対応したブール型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応したブール型の値を返す．
  virtual
  bool
  get_bool(int property) const;

  /// @brief property に対応した整数型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応した整数型の値を返す．
  virtual
  int
  get_int(int property) const;

  /// @brief property に対応した文字列型の値を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応した文字列型の値を返す．
  virtual
  string
  get_str(int property) const;
  
  /// @brief property に対応したオブジェクトを返す．
  /// @param[in] property 取り出す項目を表す値
  /// @brief return に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj(int property) const;
  
  /// @brief property に対応した反復子を返す．
  /// @param[in] property 取り出す項目を表す値
  /// @return property に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_iterator_obj(int property);

  /// @brief インデックスに対応したオブジェクトの取得
  /// @param[in] index 取得する要素の位置
  /// @return index に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj_by_index(int index) const;
  
  /// @brief 多次元のインデックスに対応したオブジェクトの取得
  /// @param[in] index_array 取得する要素の位置を表すインデックスのベクタ
  /// @return index_array に対応したオブジェクトを返す．
  virtual
  VpiObj*
  get_obj_by_index(const vector<int>& index_array) const;
  
  /// @brief name に対応したオブジェクトの取得
  /// @param[in] name 取得する要素の名前
  /// @return name に対応したオブジェクト
  virtual
  VpiObj*
  get_obj_by_name(const string& name) const;
  
  /// @brief シミュレーション時刻の取得
  /// @param[out] time_p 得られたシミュレーション時刻を格納する構造体
  ///
  /// このオブジェクトのタイムスケールを使って今のシミュレーション時刻
  /// を得る．
  virtual
  void
  get_time(p_vpi_time time_p) const;
  
  /// @brief 遅延情報の取得
  /// @param[out] delay_p 遅延情報を格納する構造体
  virtual
  void
  get_delays(p_vpi_delay delay_p) const;
  
  /// @brief 遅延情報の設定
  /// @param[in] delay_p 設定する遅延情報
  virtual
  void
  put_delays(p_vpi_delay delay_p);
  
  /// @brief オブジェクトの「値」の取得
  /// @param[out] value_p 取得した値を格納する構造体
  virtual
  void
  get_value(p_vpi_value value_p) const;
  
  /// @brief オブジェクトの「値」の設定
  /// @param[in] value_p 設定する値
  /// @param[in] time_p 設定する時刻
  /// @param[in] flags 種々のフラグ
  virtual
  VpiObj*
  put_value(p_vpi_value value_p,
	    p_vpi_time time_p,
	    int flags);
  
  /// @brief ユーザー定義システムタスク/関数情報の取得
  /// @param[out] systf_data_p システムタスク/関数情報
  virtual
  void
  get_systf_info(p_vpi_systf_data systf_data_p) const;
  
  /// @brief ユーザー定義システムタスク/関数に関連づけられたデータの取得
  /// @return put_userdata で設定されたデータ
  virtual
  void*
  get_userdata() const;
  
  /// @brief ユーザー定義システムタスク/関数に関連づけられたデータの設定
  /// @param[in] userdata 設定するデータ
  virtual
  void
  put_userdata(void* userdata) const;
  
  /// @brief コールバック情報の取得
  /// @param[out] cb_data_p コールバック情報を格納する構造体
  virtual
  void
  get_cb_info(p_cb_data cb_data_p) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 型の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return vpiIterator を返す．
  virtual
  int
  type() const;

  /// @brief 型を表す文字列の取得
  /// @return "vpiIterator" を返す．
  virtual
  string
  s_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 詳細なファイル位置を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 詳細なファイル位置の取得
  /// @return ファイル位置の情報を返す．
  /// @note このクラスでは無効なデータを返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VpiIteratorObj に固有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 反復子の指しているオブジェクトの型の取得
  /// @return 反復子の指しているオブジェクトの型を返す．
  virtual
  int
  iterator_type() const = 0;

  /// @brief この反復子を生成したオブジェクトの取得
  /// @return この反復子を生成したオブジェクトを返す．
  virtual
  VpiObj*
  use() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 参照回数を制御する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす
  virtual
  void
  inc_ref();

  /// @brief 参照回数を減らす．
  /// @note 0 になったら自動的に自分自身を削除する．
  virtual
  void
  dec_ref();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  ymuint32 mRefCount;

  // use オブジェクト
  VpiObj* mUse;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VpiIteratorImpl VpiIterator.h <ym_verilog/vpi/VpiIterator.h>
/// @ingroup VlParser
/// @brief VpiItarator の実際の機能を実装するクラスのインターフェイス
//////////////////////////////////////////////////////////////////////
template <typename T, int X>
class VpiIteratorImpl :
  public VpiIteratorObj
{
  friend class VpiIterator<T, X>;

protected:

  /// @brief コンストラクタ
  /// @param[in] use この反復子を生成したオブジェクト
  explicit
  VpiIteratorImpl(VpiObj* use) :
    VpiIteratorObj(use)
  {
  }
  
  /// @brief デストラクタ
  virtual
  ~VpiIteratorImpl()
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // VpiIterator の実際の機能
  //////////////////////////////////////////////////////////////////////

  /// @brief 指している内容を返し1つ進める．
  /// @return 次の要素を返す．
  /// @return 末尾を指しているときには NULL を返す．
  /// @note VpiIterator の時のみ意味を持つ．
  virtual
  T*
  _scan() = 0;

  virtual
  VpiObj*
  scan()
  {
    return static_cast<VpiObj*>(_scan());
  }
  
  /// @brief 反復子の指しているオブジェクトの型の取得
  /// @return 反復子の指しているオブジェクトの型を返す．
  virtual
  int
  iterator_type() const
  {
    return X;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 参照回数を制御する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす
  void
  _inc_ref()
  {
    inc_ref();
  }

  /// @brief 参照回数を減らす．
  /// @note 0 になったら自動的に自分自身を削除する．
  void
  _dec_ref()
  {
    dec_ref();
  }

};


//////////////////////////////////////////////////////////////////////
/// @class VpiIteratorImplPlus VpiIterator.h <ym_verilog/vpi/VpiIterator.h>
/// @ingroup VlParser
/// @brief 先頭に要素を付加する反復子
//////////////////////////////////////////////////////////////////////
template <typename T, int X>
class VpiIteratorImplPlus :
  public VpiIteratorImpl<T, X>
{
  friend class VpiIterator<T, X>;

public:

  /// @brief コンストラクタ
  /// @param[in] use このリストの保持者
  /// @param[in] top_obj 先頭の要素
  /// @param[in] iter 2番目以降の要素のリストの反復子
  VpiIteratorImplPlus(VpiObj* use,
		      T* top_obj,
		      const VpiIterator<T, X>& iter) :
    VpiIteratorImpl<T, X>(use),
    mTop(top_obj),
    mIter(iter)
  {
  }

  /// @brief デストラクタ
  virtual
  ~VpiIteratorImplPlus()
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // VpiIterator の実際の機能
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の位置の要素を取り出し一つ進める．
  /// @return 現在の位置の要素
  virtual
  T*
  _scan()
  {
    if ( mTop ) {
      T* ans = mTop;
      mTop = NULL;
      return ans;
    }
    return mIter.scan();
  }

  /// @brief 空の反復子のチェック
  /// @return 空の反復子の時 true を返す．
  /// @note vpiIterator タイプの時のみ意味を持つ．
  virtual
  bool
  is_empty_iterator() const
  {
    if ( mTop ) {
      return false;
    }
    return mIter.is_empty_iterator();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  T* mTop;

  // 2番目以降の要素のリストの反復子
  VpiIterator<T, X> mIter;

};


//////////////////////////////////////////////////////////////////////
// VpiIterator の派生クラスの宣言
//////////////////////////////////////////////////////////////////////

typedef VpiIterator<VpiScope, vpiInternalScope>
VpiInternalScopeIterator;

typedef VpiIterator<VpiModule, vpiModule>
VpiModuleIterator;

typedef VpiIterator<VpiModuleArray, vpiModuleArray>
VpiModuleArrayIterator;

typedef VpiIterator<VpiPrimitive, vpiPrimitive>
VpiPrimitiveIterator;

typedef VpiIterator<VpiPrimArray, vpiPrimitiveArray>
VpiPrimitiveArrayIterator;

typedef VpiIterator<VpiPrimTerm, vpiPrimTerm>
VpiPrimTermIterator;

typedef VpiIterator<VpiPathTerm, vpiPathTerm>
VpiPathTermIterator;

typedef VpiIterator<VpiUdpDefn, vpiUdpDefn>
VpiUdpDefnIterator;

typedef VpiIterator<VpiTableEntry, vpiTableEntry>
VpiTableEntryIterator;

typedef VpiIterator<VpiPort, vpiPort>
VpiPortIterator;

typedef VpiIterator<VpiPortBit, vpiBit>
VpiPortBitIterator;

typedef VpiIterator<VpiPorts, vpiPorts>
VpiPortsIterator;

typedef VpiIterator<VpiPorts, vpiPortInst>
VpiPortInstIterator;

typedef VpiIterator<VpiIODecl, vpiIODecl>
VpiIODeclIterator;

typedef VpiIterator<VpiNet, vpiNet>
VpiNetIterator;

typedef VpiIterator<VpiNetBit, vpiBit>
VpiNetBitIterator;

typedef VpiIterator<VpiNetArray, vpiNetArray>
VpiNetArrayIterator;

typedef VpiIterator<VpiReg, vpiReg>
VpiRegIterator;

typedef VpiIterator<VpiRegBit, vpiBit>
VpiRegBitIterator;

typedef VpiIterator<VpiRegArray, vpiRegArray>
VpiRegArrayIterator;

typedef VpiIterator<VpiVariables, vpiVariables>
VpiVariablesIterator;

typedef VpiIterator<VpiVarSelect, vpiVarSelect>
VpiVarSelectIterator;

typedef VpiIterator<VpiParameter, vpiParameter>
VpiParameterIterator;

typedef VpiIterator<VpiSpecParam, vpiSpecParam>
VpiSpecParamIterator;

typedef VpiIterator<VpiNamedEvent, vpiNamedEvent>
VpiNamedEventIterator;

typedef VpiIterator<VpiNeArray, vpiNamedEventArray>
VpiNamedEventArrayIterator;

typedef VpiIterator<VpiDefParam, vpiDefParam>
VpiDefParamIterator;

typedef VpiIterator<VpiParamAssign, vpiParamAssign>
VpiParamAssignIterator;

typedef VpiIterator<VpiContAssign, vpiContAssign>
VpiContAssignIterator;

typedef VpiIterator<VpiContAssignBit, vpiBit>
VpiContAssignBitIterator;

typedef VpiIterator<VpiCaCommon, vpiContAssign>
VpiCaCommonIterator;

typedef VpiIterator<VpiProcess, vpiProcess>
VpiProcessIterator;

typedef VpiIterator<VpiFunction, vpiFunction>
VpiFunctionIterator;

typedef VpiIterator<VpiTask, vpiTask>
VpiTaskIterator;

typedef VpiIterator<VpiModPath, vpiModPath>
VpiModPathIterator;

typedef VpiIterator<VpiTchk, vpiTchk>
VpiTchkIterator;

typedef VpiIterator<VpiTchkTerm, vpiTchkTerm>
VpiTchkTermIterator;

typedef VpiIterator<VpiPathTerm, vpiPathTerm>
VpiPathTermIterator;

typedef VpiIterator<VpiPathTerm, vpiModPathIn>
VpiModPathInIterator;

typedef VpiIterator<VpiPathTerm, vpiModPathOut>
VpiModPathOutIterator;

typedef VpiIterator<VpiPathTerm, vpiModDataPathIn>
VpiModDataPathInIterator;

typedef VpiIterator<VpiStmt, vpiStmt>
VpiStmtIterator;

typedef VpiIterator<VpiExpr, vpiExpr>
VpiExprIterator;

typedef VpiIterator<VpiExpr, vpiIndex>
VpiIndexIterator;

typedef VpiIterator<VpiExpr, vpiOperand>
VpiOperandIterator;

typedef VpiIterator<VpiExpr, vpiArgument>
VpiArgumentIterator;

typedef VpiIterator<VpiRange, vpiRange>
VpiRangeIterator;

typedef VpiIterator<VpiObj, vpiDriver>
VpiDriverIterator;

typedef VpiIterator<VpiObj, vpiLoad>
VpiLoadIterator;

typedef VpiIterator<VpiObj, vpiLocalDriver>
VpiLocalDriverIterator;

typedef VpiIterator<VpiObj, vpiLocalLoad>
VpiLocalLoadIterator;

typedef VpiIterator<VpiObj, vpiUse>
VpiUseIterator;


//////////////////////////////////////////////////////////////////////
// VpiIteratorImpl の派生クラスの宣言
//////////////////////////////////////////////////////////////////////

typedef VpiIteratorImpl<VpiScope, vpiInternalScope>
VpiInternalScopeIteratorImpl;

typedef VpiIteratorImpl<VpiModule, vpiModule>
VpiModuleIteratorImpl;

typedef VpiIteratorImpl<VpiModuleArray, vpiModuleArray>
VpiModuleArrayIteratorImpl;

typedef VpiIteratorImpl<VpiPrimitive, vpiPrimitive>
VpiPrimitiveIteratorImpl;

typedef VpiIteratorImpl<VpiPrimArray, vpiPrimitiveArray>
VpiPrimitiveArrayIteratorImpl;

typedef VpiIteratorImpl<VpiPrimTerm, vpiPrimTerm>
VpiPrimTermIteratorImpl;

typedef VpiIteratorImpl<VpiPathTerm, vpiPathTerm>
VpiPathTermIteratorImpl;

typedef VpiIteratorImpl<VpiUdpDefn, vpiUdpDefn>
VpiUdpDefnIteratorImpl;

typedef VpiIteratorImpl<VpiTableEntry, vpiTableEntry>
VpiTableEntryIteratorImpl;

typedef VpiIteratorImpl<VpiPort, vpiPort>
VpiPortIteratorImpl;

typedef VpiIteratorImpl<VpiPortBit, vpiBit>
VpiPortBitIteratorImpl;

typedef VpiIteratorImpl<VpiPorts, vpiPorts>
VpiPortsIteratorImpl;

typedef VpiIteratorImpl<VpiPorts, vpiPortInst>
VpiPortInstIteratorImpl;

typedef VpiIteratorImpl<VpiIODecl, vpiIODecl>
VpiIODeclIteratorImpl;

typedef VpiIteratorImpl<VpiNet, vpiNet>
VpiNetIteratorImpl;

typedef VpiIteratorImpl<VpiNetBit, vpiBit>
VpiNetBitIteratorImpl;

typedef VpiIteratorImpl<VpiNetArray, vpiNetArray>
VpiNetArrayIteratorImpl;

typedef VpiIteratorImpl<VpiReg, vpiReg>
VpiRegIteratorImpl;

typedef VpiIteratorImpl<VpiRegBit, vpiBit>
VpiRegBitIteratorImpl;

typedef VpiIteratorImpl<VpiRegArray, vpiRegArray>
VpiRegArrayIteratorImpl;

typedef VpiIteratorImpl<VpiVariables, vpiVariables>
VpiVariablesIteratorImpl;

typedef VpiIteratorImpl<VpiVarSelect, vpiVarSelect>
VpiVarSelectIteratorImpl;

typedef VpiIteratorImpl<VpiParameter, vpiParameter>
VpiParameterIteratorImpl;

typedef VpiIteratorImpl<VpiSpecParam, vpiSpecParam>
VpiSpecParamIteratorImpl;

typedef VpiIteratorImpl<VpiNamedEvent, vpiNamedEvent>
VpiNamedEventIteratorImpl;

typedef VpiIteratorImpl<VpiNeArray, vpiNamedEventArray>
VpiNamedEventArrayIteratorImpl;

typedef VpiIteratorImpl<VpiDefParam, vpiDefParam>
VpiDefParamIteratorImpl;

typedef VpiIteratorImpl<VpiParamAssign, vpiParamAssign>
VpiParamAssignIteratorImpl;

typedef VpiIteratorImpl<VpiContAssign, vpiContAssign>
VpiContAssignIteratorImpl;

typedef VpiIteratorImpl<VpiContAssignBit, vpiBit>
VpiContAssignBitIteratorImpl;

typedef VpiIteratorImpl<VpiCaCommon, vpiContAssign>
VpiCaCommonIteratorImpl;

typedef VpiIteratorImpl<VpiProcess, vpiProcess>
VpiProcessIteratorImpl;

typedef VpiIteratorImpl<VpiFunction, vpiFunction>
VpiFunctionIteratorImpl;

typedef VpiIteratorImpl<VpiTask, vpiTask>
VpiTaskIteratorImpl;

typedef VpiIteratorImpl<VpiModPath, vpiModPath>
VpiModPathIteratorImpl;

typedef VpiIteratorImpl<VpiTchk, vpiTchk>
VpiTchkIteratorImpl;

typedef VpiIteratorImpl<VpiTchkTerm, vpiTchkTerm>
VpiTchkTermIteratorImpl;

typedef VpiIteratorImpl<VpiPathTerm, vpiPathTerm>
VpiPathTermIteratorImpl;

typedef VpiIteratorImpl<VpiPathTerm, vpiModPathIn>
VpiModPathInIteratorImpl;

typedef VpiIteratorImpl<VpiPathTerm, vpiModPathOut>
VpiModPathOutIteratorImpl;

typedef VpiIteratorImpl<VpiPathTerm, vpiModDataPathIn>
VpiModDataPathInIteratorImpl;

typedef VpiIteratorImpl<VpiStmt, vpiStmt>
VpiStmtIteratorImpl;

typedef VpiIteratorImpl<VpiExpr, vpiExpr>
VpiExprIteratorImpl;

typedef VpiIteratorImpl<VpiExpr, vpiIndex>
VpiIndexIteratorImpl;

typedef VpiIteratorImpl<VpiExpr, vpiOperand>
VpiOperandIteratorImpl;

typedef VpiIteratorImpl<VpiExpr, vpiArgument>
VpiArgumentIteratorImpl;

typedef VpiIteratorImpl<VpiRange, vpiRange>
VpiRangeIteratorImpl;

typedef VpiIteratorImpl<VpiObj, vpiDriver>
VpiDriverIteratorImpl;

typedef VpiIteratorImpl<VpiObj, vpiLoad>
VpiLoadIteratorImpl;

typedef VpiIteratorImpl<VpiObj, vpiLocalDriver>
VpiLocalDriverIteratorImpl;

typedef VpiIteratorImpl<VpiObj, vpiLocalLoad>
VpiLocalLoadIteratorImpl;

typedef VpiIteratorImpl<VpiObj, vpiUse>
VpiUseIteratorImpl;

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VPI_VPIITERATOR_H
