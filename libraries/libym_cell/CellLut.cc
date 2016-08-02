
/// @file libym_cell/CellLut.cc
/// @brief CellLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellLut.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CellLutImpl.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellLutTemplateBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellLutTemplateBase::CellLutTemplateBase(ShString name) :
  mName(name)
{
}

// @brief デストラクタ
CellLutTemplateBase::~CellLutTemplateBase()
{
}
  
// @brief 名前の取得
const char*
CellLutTemplateBase::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス CellLutTemplate1D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellLutTemplate1D::CellLutTemplate1D(ShString name,
				     tCellVarType var_type,
				     const vector<double>& index_array) :
  CellLutTemplateBase(name),
  mVarType(var_type),
  mIndexArray(index_array.size())
{
  ymuint32 n = index_array.size();
  for (ymuint32 i = 0; i < n; ++ i) {
    mIndexArray[i] = index_array[i];
  }
}

// @brief デストラクタ
CellLutTemplate1D::~CellLutTemplate1D()
{
}

// @brief 次元数の取得
ymuint32
CellLutTemplate1D::dimension() const
{
  return 1;
}

// @brief 変数型の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
tCellVarType
CellLutTemplate1D::variable_type(ymuint32 var) const
{
  assert_cond( var == 0, __FILE__, __LINE__);
  return mVarType;
}
  
// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CellLutTemplate1D::index_num(ymuint32 var) const
{
  assert_cond( var == 0, __FILE__, __LINE__);
  return mIndexArray.size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CellLutTemplate1D::index(ymuint32 var,
			 ymuint32 pos) const
{
  assert_cond( var == 0, __FILE__, __LINE__);
  assert_cond( pos < index_num(0), __FILE__, __LINE__);
  return mIndexArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CellLutTemplate2D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellLutTemplate2D::CellLutTemplate2D(ShString name,
				     tCellVarType var1,
				     const vector<double>& index_array1,
				     tCellVarType var2,
				     const vector<double>& index_array2) :
  CellLutTemplateBase(name)
{
  mVarType[0] = var1;
  ymuint32 n1 = index_array1.size();
  mIndexArray[0].resize(n1);
  for (ymuint32 i = 0; i < n1; ++ i) {
    mIndexArray[0][i] = index_array1[i];
  }
  
  mVarType[1] = var2;
  ymuint32 n2 = index_array2.size();
  mIndexArray[1].resize(n2);
  for (ymuint32 i = 0; i < n2; ++ i) {
    mIndexArray[1][i] = index_array2[i];
  }
}

// @brief デストラクタ
CellLutTemplate2D::~CellLutTemplate2D()
{
}

// @brief 次元数の取得
ymuint32
CellLutTemplate2D::dimension() const
{
  return 2;
}

// @brief 変数型の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
tCellVarType
CellLutTemplate2D::variable_type(ymuint32 var) const
{
  assert_cond( var < 2 , __FILE__, __LINE__);
  return mVarType[var];
}
  
// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CellLutTemplate2D::index_num(ymuint32 var) const
{
  assert_cond( var < 2 , __FILE__, __LINE__);
  return mIndexArray[var].size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CellLutTemplate2D::index(ymuint32 var,
			 ymuint32 pos) const
{
  assert_cond( var < 2 , __FILE__, __LINE__);
  assert_cond( pos < index_num(var) , __FILE__, __LINE__);
  return mIndexArray[var][pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CellLutTemplate3D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellLutTemplate3D::CellLutTemplate3D(ShString name,
				     tCellVarType var1,
				     const vector<double>& index_array1,
				     tCellVarType var2,
				     const vector<double>& index_array2,
				     tCellVarType var3,
				     const vector<double>& index_array3) :
  CellLutTemplateBase(name)
{
  mVarType[0] = var1;
  ymuint32 n1 = index_array1.size();
  mIndexArray[0].resize(n1);
  for (ymuint32 i = 0; i < n1; ++ i) {
    mIndexArray[0][i] = index_array1[i];
  }
  
  mVarType[1] = var2;
  ymuint32 n2 = index_array2.size();
  mIndexArray[1].resize(n2);
  for (ymuint32 i = 0; i < n2; ++ i) {
    mIndexArray[1][i] = index_array2[i];
  }
  
  mVarType[2] = var3;
  ymuint32 n3 = index_array3.size();
  mIndexArray[2].resize(n3);
  for (ymuint32 i = 0; i < n3; ++ i) {
    mIndexArray[2][i] = index_array3[i];
  }
}

// @brief デストラクタ
CellLutTemplate3D::~CellLutTemplate3D()
{
}

// @brief 次元数の取得
ymuint32
CellLutTemplate3D::dimension() const
{
  return 3;
}

// @brief 変数型の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
tCellVarType
CellLutTemplate3D::variable_type(ymuint32 var) const
{
  assert_cond( var < 3 , __FILE__, __LINE__);
  return mVarType[var];
}
  
// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CellLutTemplate3D::index_num(ymuint32 var) const
{
  assert_cond( var < 3 , __FILE__, __LINE__);
  return mIndexArray[var].size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CellLutTemplate3D::index(ymuint32 var,
			 ymuint32 pos) const
{
  assert_cond( var < 3 , __FILE__, __LINE__);
  assert_cond( pos < index_num(var) , __FILE__, __LINE__);
  return mIndexArray[var][pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CellLut1D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellLut1D::CellLut1D(const CellLutTemplate1D* lut_template,
		     const vector<double>& index_array) :
  mTemplate(lut_template)
{
  if ( index_array.empty() ) {
    ymuint32 n = mTemplate->index_num(0);
    mIndexArray.resize(n);
    for (ymuint32 i = 0; i < n; ++ i) {
      mIndexArray[i] = mTemplate->index(0, i);
    }
  }
  else {
    ymuint32 n = index_array.size();
    mIndexArray.resize(n);
    for (ymuint32 i = 0; i < n; ++ i) {
      mIndexArray[i] = index_array[i];
    }
  }
}

// @brief デストラクタ
CellLut1D::~CellLut1D()
{
}

// @brief テンプレートの取得
const CellLutTemplate*
CellLut1D::lut_template() const
{
  return mTemplate;
}

// @brief インデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CellLut1D::index(ymuint32 var,
		 ymuint32 pos) const
{
  assert_cond( var < 1 , __FILE__, __LINE__);
  assert_cond( pos < mIndexArray.size() , __FILE__, __LINE__);
  return mIndexArray[pos];
}



END_NAMESPACE_YM_CELL
