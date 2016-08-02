
/// @file libym_tgnet/LogicMgr.cc
/// @brief LogicMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LogicMgr.cc 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "LogicMgr.h"


BEGIN_NAMESPACE_YM_TGNET

// @brief コンストラクタ
LogicMgr::LogicMgr()
{
  alloc_table(1024);
}

// @brief デストラクタ
LogicMgr::~LogicMgr()
{
  size_t n = mCellArray.size();
  for (size_t i = 0; i < n; ++ i) {
    delete mCellArray[i];
  }
  delete [] mHashTable;
}

// @brief 初期化する．
void
LogicMgr::clear()
{
  size_t n = mCellArray.size();
  for (size_t i = 0; i < n; ++ i) {
    delete mCellArray[i];
  }
  mCellArray.clear();

  for (size_t i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
}

// @brief 登録されている論理式の数を返す．
size_t
LogicMgr::num() const
{
  return mCellArray.size();
}

// @brief 論理式を取り出す．
// @param[in] gt_id ゲートテンプレート
LogExpr
LogicMgr::get(TgGateTemplate gt_id) const
{
  tTgGateType type = gt_id.type();
  size_t ni = gt_id.ni();
  if ( static_cast<ymuint32>(type) < kBase ) {
    vector<LogExpr> literals(ni);
    for (size_t i = 0; i < ni; ++ i) {
      literals[i] = LogExpr::make_posiliteral(i);
    }
    switch ( type ) {
    case kTgBuff:
      return literals[0];
      
    case kTgNot:
      return ~literals[0];

    case kTgAnd:
      for (size_t i = 1; i < ni; ++ i) {
	literals[0] &= literals[i];
      }
      return literals[0];
      
    case kTgNand:
      for (size_t i = 1; i < ni; ++ i) {
	literals[0] &= literals[i];
      }
      return ~literals[0];
      
    case kTgOr:
      for (size_t i = 1; i < ni; ++ i) {
	literals[0] |= literals[i];
      }
      return literals[0];
      
    case kTgNor:
      for (size_t i = 1; i < ni; ++ i) {
	literals[0] |= literals[i];
      }
      return ~literals[0];
      
    case kTgXor:
      for (size_t i = 1; i < ni; ++ i) {
	literals[0] ^= literals[i];
      }
      return literals[0];
      
    case kTgXnor:
      for (size_t i = 1; i < ni; ++ i) {
	literals[0] ^= literals[i];
      }
      return ~literals[0];

    default:
      assert_not_reached(__FILE__, __LINE__);
      return LogExpr(); // ダミー
    }
  }
  else {
    return mCellArray[type - kBase]->mLexp;
  }
}


BEGIN_NONAMESPACE

// 組み込み型のチェック
tTgGateType
check_builtin(const LogExpr& lexp,
	      size_t ni,
	      const vector<ymulong>& tmp_vec)
{
  if ( lexp.is_posiliteral() ) {
    assert_cond(lexp.varid() == 0, __FILE__, __LINE__);
    return kTgBuff;
  }
  if ( lexp.is_negaliteral() ) {
    assert_cond(lexp.varid() == 0, __FILE__, __LINE__);
    return kTgNot;
  }
  size_t np = 1 << ni;
  const size_t NBPW = sizeof(ymulong) * 8;

  bool and_flag = true;
  bool nand_flag = true;
  bool or_flag = true;
  bool nor_flag = true;
  bool xor_flag = true;
  bool xnor_flag = true;
  for (ymuint p = 0; p < np; ++ p) {
    size_t blk = p / NBPW;
    size_t pos = p % NBPW;
    bool bit = static_cast<bool>((tmp_vec[blk] >> pos) & 1UL);
    if ( and_flag || nand_flag ) {
      if ( p < (np - 1) ) {
	and_flag &= !bit;
	nand_flag &= bit;
      }
      else {
	and_flag &= bit;
	nand_flag &= !bit;
      }
    }
    if ( or_flag || nor_flag ) {
      if ( p == 0UL ) {
	or_flag &= !bit;
	nor_flag &= bit;
      }
      else {
	or_flag &= bit;
	nor_flag &= !bit;
      }
    }
    if ( xor_flag || xnor_flag ) {
      size_t n1 = 0;
      for (size_t i = 0; i < ni; ++ i) {
	if ( p & (1UL << i) ) {
	  ++ n1;
	}
      }
      if ( n1 & 1 ) {
	xor_flag &= bit;
	xnor_flag &= !bit;
      }
      else {
	xor_flag &= !bit;
	xnor_flag &= bit;
      }
    }
  }

  if ( and_flag ) {
    return kTgAnd;
  }
  if ( nand_flag ) {
    return kTgNand;
  }
  if ( or_flag ) {
    return kTgOr;
  }
  if ( nor_flag ) {
    return kTgNor;
  }
  if ( xor_flag ) {
    return kTgXor;
  }
  if ( xnor_flag ) {
    return kTgXnor;
  }
  return kTgUndef;
}

// ハッシュ値を計算する．
ymuint32
hash_func(const vector<ymulong>& tv_array)
{
  size_t n = tv_array.size();
  ymuint32 ans = 0;
  for (size_t i = 0; i < n; ++ i) {
#if SIZEOF_UNSIGNED_LONG == 4
    ymuint32 ans1 = tv_array[i];
    ans ^= ((ans1 << i) | (ans1 >> (31 - i)));
#elif SIZEOF_UNSIGNED_LONG == 8
    ymuint32 ans1 = tv_array[i] & 0xffffffff;
    ans ^= ((ans1 << i) | (ans1 >> (31 - i)));
    ymuint32 ans2 = tv_array[i] >> 32;
    ans ^= ((ans2 << (i + 3)) | (ans2 >> (31 - i - 3)));
#endif
  }
  return ans;
}

END_NONAMESPACE


// @brief 新しい論理式を登録する．
TgGateTemplate
LogicMgr::reg_logic(const LogExpr& lexp)
{
  // 真理値ベクタに変換する．
  vector<ymulong> tmp_vec;
  lexp.make_tv(tmp_vec);
  size_t ni = lexp.input_size();

  // 組み込み型のチェック
  tTgGateType type = check_builtin(lexp, ni, tmp_vec);
  
  if ( type != kTgUndef ) {
    return TgGateTemplate(type, ni);
  }

  // ハッシュ表から同一の関数がないか調べる．
  size_t pos0 = hash_func(tmp_vec);
  size_t pos = pos0 % mHashSize; 
  for (Cell* cell = mHashTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mId.ni() == ni && equiv(cell, tmp_vec) ) {
      // 同じ関数が登録されていた．
      // もしもリテラル数が少なかったら論理式を変更する．
      if ( cell->mLexp.litnum() > lexp.litnum() ) {
	cell->mLexp = lexp;
      }
      return cell->mId;
    }
  }
  
  if ( num() > mNextLimit ) {
    // ハッシュ表を拡大する．
    expand();
    pos = pos0 % mHashSize;
  }
  
  Cell* new_cell = new Cell;
  type = static_cast<tTgGateType>(mCellArray.size() + kBase);
  new_cell->mId = TgGateTemplate(type, ni);
  new_cell->mLexp = lexp;
  new_cell->mTvArray = tmp_vec;
  new_cell->mLink = mHashTable[pos];
  mCellArray.push_back(new_cell);
  mHashTable[pos] = new_cell;
  return new_cell->mId;
}

// 2つの論理式の配列が等価かどうか調べる．
bool
LogicMgr::equiv(Cell* cell,
		const vector<ymulong>& tv_array)
{
  size_t n = cell->mTvArray.size();
  for (size_t i = 0; i < n; ++ i) {
    if ( cell->mTvArray[i] != tv_array[i] ) {
      return false;
    }
  }
  return true;
}

// ハッシュ表を拡大して再ハッシュする．
void
LogicMgr::expand()
{
  size_t old_size = mHashSize;
  Cell** old_table = mHashTable;
  alloc_table(mHashSize << 1);
  for (size_t i = 0; i < old_size; ++ i) {
    Cell* next = NULL;
    for (Cell* cell = old_table[i]; cell; cell = next) {
      next = cell->mLink;
      size_t pos1 = hash_func(cell->mTvArray) % mHashSize;
      cell->mLink = mHashTable[pos1];
      mHashTable[pos1] = cell;
    }
  }
  delete [] old_table;
}

// ハッシュ表用の領域を確保する．
void
LogicMgr::alloc_table(size_t req_size)
{
  mHashSize = req_size;
  mHashTable = new Cell*[mHashSize];
  for (size_t i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<size_t>(mHashSize * kHashCapacity);
}

// @brief デバッグ用の関数
// @param[in] s 出力先のストリーム
void
LogicMgr::dump(ostream& s) const
{
  s << "=====<LogicMgr Dump Start>=====" << endl;
  size_t n = mCellArray.size();
  for (size_t i = 0; i < n; ++ i) {
    Cell* tmp = mCellArray[i];
    s << tmp->mId << "\t: " << tmp->mLexp << endl;
  }
  s << "=====< LogicMgr Dump End >=====" << endl;
}


//////////////////////////////////////////////////////////////////////
// TgGateTemplate
//////////////////////////////////////////////////////////////////////

// @relates TgGateTemplate
// @brief TgGateTemplate の内容を出力する．
ostream&
operator<<(ostream& s,
	   TgGateTemplate gt)
{
  tTgGateType type = gt.type();
  switch ( type ) {
  case kTgUndef:  s << "--UNDEF--"; break;

  case kTgInput:  s << "--INPUT--"; break;
  case kTgOutput: s << "--OUTPUT--"; break;
    
  case kTgConst0: s << "--CONST0--"; break;
  case kTgConst1: s << "--CONST1--"; break;
    
  case kTgBuff:   s << "--BUFF--"; break;
  case kTgNot:    s << "--NOT--"; break;

  case kTgAnd:    s << "--AND(" << gt.ni() << ")--"; break;

  case kTgNand:   s << "--NAND(" << gt.ni() << ")--"; break;

  case kTgOr:     s << "--OR(" << gt.ni() << ")--"; break;

  case kTgNor:    s << "--NOR(" << gt.ni() << ")--"; break;

  case kTgXor:    s << "--XOR(" << gt.ni() << ")--"; break;

  case kTgXnor:   s << "--XNOR(" << gt.ni() << ")--"; break;

  default:
    if ( static_cast<ymuint32>(type) >= static_cast<ymuint32>(kTgUsrDef) ) {
      s << "--CPLX(" << gt.ni() << ")--"; break;
    }
    else {
      s << "--ERROR--"; break;
    }
  }
  return s;
}

END_NAMESPACE_YM_TGNET
