#ifndef LIBYM_CELL_CELLLUTIMPL_H
#define LIBYM_CELL_CELLLUTIMPL_H

/// @file　libym_cell/CellLutImpl.h
/// @brief CellLutTemplate/CellLut のの実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellLutImpl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLut.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLutTemplateBase CellLutImpl.h "CellLutImpl.h"
/// @brief CellLutTemplate の(擬似)基底クラス
//////////////////////////////////////////////////////////////////////
class CellLutTemplateBase :
  public CellLutTemplate
{
protected:

  /// @brief コンストラクタ
  CellLutTemplateBase(ShString name);

  /// @brief デストラクタ
  virtual
  ~CellLutTemplateBase();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 名前の取得
  const char*
  name() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellLutTemplate1D CellLutImpl.h "CellLutImpl.h"
/// @brief 1次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CellLutTemplate1D :
  public CellLutTemplateBase
{
  friend class CellManip;
  
private:

  /// @brief コンストラクタ
  CellLutTemplate1D(ShString name,
		    tCellVarType var_type,
		    const vector<double>& index_array);

  /// @brief デストラクタ
  virtual
  ~CellLutTemplate1D();


public:

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  tCellVarType
  variable_type(ymuint32 var) const;
  
  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const;

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  tCellVarType mVarType;

  // デフォルトインデックス値の配列
  vector<double> mIndexArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellLutTemplate2D CellLutImpl.h "CellLutImpl.h"
/// @brief 2次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CellLutTemplate2D :
  public CellLutTemplateBase
{
  friend class CellManip;
  
private:

  /// @brief コンストラクタ
  CellLutTemplate2D(ShString name,
		    tCellVarType var1,
		    const vector<double>& index_array1,
		    tCellVarType var2,
		    const vector<double>& index_array2);

  /// @brief デストラクタ
  virtual
  ~CellLutTemplate2D();


public:

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  tCellVarType
  variable_type(ymuint32 var) const;
  
  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const;

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  tCellVarType mVarType[2];

  // デフォルトインデックス値の配列
  vector<double> mIndexArray[2];
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellLutTemplate3D CellLutImpl.h "CellLutImpl.h"
/// @brief 3次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CellLutTemplate3D :
  public CellLutTemplateBase
{
  friend class CellManip;
  
private:

  /// @brief コンストラクタ
  CellLutTemplate3D(ShString name,
		    tCellVarType var1,
		    const vector<double>& index_array1,
		    tCellVarType var2,
		    const vector<double>& index_array2,
		    tCellVarType var3,
		    const vector<double>& index_array3);

  /// @brief デストラクタ
  virtual
  ~CellLutTemplate3D();


public:

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  tCellVarType
  variable_type(ymuint32 var) const;
  
  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const;

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  tCellVarType mVarType[3];

  // デフォルトインデックス値の配列
  vector<double> mIndexArray[3];
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellLut1D CellLutImpl.h "CellLutImpl.h"
/// @brief 1次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLut1D :
  public CellLut
{
public:

  /// @brief コンストラクタ
  CellLut1D(const CellLutTemplate1D* lut_template,
	    const vector<double>& index_array = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CellLut1D();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CellLutTemplate1D* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellLut2D CellLutImpl.h "CellLutImpl.h"
/// @brief 2次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLut2D :
  public CellLut
{
public:

  /// @brief コンストラクタ
  CellLut2D(const CellLutTemplate2D* lut_template,
	    const vector<double>& index_array1 = vector<double>(),
	    const vector<double>& index_array2 = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CellLut2D();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CellLutTemplate2D* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray[2];
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellLut3D CellLutImpl.h "CellLutImpl.h"
/// @brief 3次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLut3D :
  public CellLut
{
public:

  /// @brief コンストラクタ
  CellLut3D(const CellLutTemplate3D* lut_template,
	    const vector<double>& index_array1 = vector<double>(),
	    const vector<double>& index_array2 = vector<double>(),
	    const vector<double>& index_array3 = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CellLut3D();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CellLutTemplate3D* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray[3];
  
};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLLUT_H
