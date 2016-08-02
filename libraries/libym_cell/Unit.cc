// $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Unit.cc,v $
// $Author: matsunaga $
// $Revision: 1.1.1.1 $
// $Date: 2005/12/11 17:07:26 $

/// 単位を表すクラス

#include "Unit.h"
#include <sstream>


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// 単位を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// \param bias 小数点の位置．正なら左方向にずらす
// \param ustr 単位を表す文字列
Unit::Unit(int bias, const string& ustr) :
  mBias(bias),
  mUstr(ustr)
{
}

// デストラクタ
Unit::~Unit()
{
}

// 単位つきの数値の文字列表現をつくる．
string
Unit::value2str(int value) const
{
  ostringstream buf;
  buf << value;
  string tmp = buf.str();
  if ( bias() > 0 ) {
    int len = tmp.size();
    if ( len > bias() ) {
      // tmp の内側に小数点を挿入
      tmp.insert(len - bias(), ".");
    }
    else {
      // tmp の左側(MSB)に0を挿入してから小数点を挿入
      // さらにもうひとつ0を挿入
      for (int i = len; i < bias(); ++ i) {
	tmp.insert(0, "0");
      }
      tmp.insert(0, "0.");
    }
  }
  else if ( bias() < 0 && value != 0 ) {
    // tmp の右側(LSB)に0を挿入
    // 普通あんまりやらない for-loop の使い方
    for (int i = bias(); i < 0; ++ i) {
      tmp += "0";
    }
  }
  return tmp + ustr();
}

END_NAMESPACE_YM_CELL
