// $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/CellMgr.cc,v $
// $Author: matsunaga $
// $Revision: 1.1.1.1 $
// $Date: 2005/12/11 17:07:25 $

// セルライブラリを機能ごとに分割し、NPN同値類として管理するクラス

#if 0

#include <synth/hf_NpnMgr.h>
#include <synth/hf_Cell.h>
#include <synth/hf_Celsyn2Cell.h>
#include <limits.h>	// for INT_MAX
#include <synth/hf_Binder.h>
#include <synth/hf_CEventList.h>

// コンストラクタ
hf_CCellMgr::hf_CCellMgr(void) : hf_CLibrary( hf_lt_Cell_OLD ),
  mCelsynMgr(NULL), mDefaultCell(new hf_CCell* [hf_kDelayModelNum])
{
  // デフォルトセル配列を NULL で初期化
  for (int i = 0; i < hf_kDelayModelNum; i++) {
    mDefaultCell[i] = NULL;
  }

  // たぶん他のところで初期化されているはずだけれど念のため．
  hf_CBddMgr::Init();
  hf_CNpnMgr::Init();

  Clear();				
}

// デストラクタ
hf_CCellMgr::~hf_CCellMgr(void)
{
  hf_CListLoc loc;
  hf_CCell* gcell;
  hf_CCellGroup* group;
  hf_CUcellEquiv* equiv;
  hf_mForEachUcellEquiv(*this, loc, equiv) {
    delete equiv;
  }
  hf_mForEachCellGroup(*this, loc, group) {
    delete group;
  }
  hf_mForEachCell(*this, loc, gcell) {
    delete gcell;
  }
  mCells.Clear();
  mUcells.Clear();
  mGroups.Clear();
  mMcellGroups.Clear();
  mUcellGroups.Clear();
  mScellGroups.Clear();
  mZcellGroups.Clear();
  mEquivs.Clear();

  // デフォルトセル配列を解放
  for (int i = 0; i < hf_kDelayModelNum; i++) {
    if (i != hf_kDelayModelMapped) {
      delete mDefaultCell[i];
    }
  }
  delete [] mDefaultCell;

  delete mCelsynMgr;	// 必要なら CelsynMgr も開放する。
}

// 初期化する。予約済みのhf_CUcellEquiv, hf_CUcellGroupを作る。
void hf_CCellMgr::Clear(void)
{
  hf_CListLoc loc;
  hf_CCell* gcell;
  hf_CCellGroup* group;
  hf_CUcellEquiv* equiv;
  
  hf_mForEachUcellEquiv(*this, loc, equiv) {
    delete equiv;
  }
  hf_mForEachCellGroup(*this, loc, group) {
    delete group;
  }
  hf_mForEachCell(*this, loc, gcell) {
    delete gcell;
  }

  mCells.Clear();
  mUcells.Clear();
  mDict.Clear();

  mGroupDict.Clear();
  mGroups.Clear();
  mMcellGroups.Clear();
  mUcellGroups.Clear();
  mScellGroups.Clear();
  mZcellGroups.Clear();
  mXcellGroups.Clear();
  
  mEquivs.Clear();

  // デフォルトセル配列を解放
  for (int i = 0; i < hf_kDelayModelNum; i++) {
    if (i != hf_kDelayModelMapped) {
      delete mDefaultCell[i];
    }
    mDefaultCell[i] = NULL;
  }

  delete mCelsynMgr;
  mCelsynMgr = NULL;

  // 定数０関数
  hf_CNpnFd fd0 = hf_CNpnMgr::ChkMatch(hf_kBdd0, true);

  mR_const = NewUcellEquiv(fd0.GetId());
  mG_const0 = NewUcellGroup(NULL, 0, hf_CFactForm::Const0());
  hf_mAssert(mG_const0->UcellEquiv() == mR_const, "libCell", "CL14");

  // 定数１関数
  mG_const1 = NewUcellGroup(NULL, 0, hf_CFactForm::Const1());
  hf_mAssert(mG_const1->UcellEquiv() == mR_const, "libCell", "CL15");

  // バッファ(f = x)
  hf_CFactForm buffer = hf_CFactForm(0, hf_kFactFormPosi);
  hf_CNpnFd fd2 = hf_CNpnMgr::ChkMatch(hf_CBdd::Literal(0), true);
  mR_buf = NewUcellEquiv(fd2.GetId());
  mG_buf = NewUcellGroup(NULL, 1, buffer);
  hf_mAssert(mG_buf->UcellEquiv() == mR_buf, "libCell", "CL16");

  // インバータ(f = ~x)
  hf_CFactForm inverter = hf_CFactForm(0, hf_kFactFormNega);
  hf_CNpnFd fd3 = hf_CNpnMgr::ChkMatch(hf_CBdd::Literal(0, hf_kBddPolN), true);
  mG_inv = NewUcellGroup(NULL, 1, inverter);
  hf_mAssert(mG_inv->UcellEquiv() == mR_buf, "libCell", "CL17");
}

// 名前で検索してセルを返す。
hf_CCell* hf_CCellMgr::Cell(const hf_CString& name) const
{
  hf_CCell* ans;
  bool stat = mDict.Get(name, ans);
  return (stat == true) ? ans : NULL;
}

// hf_CMcell のセルを名前で検索して返す。
hf_CMcell* hf_CCellMgr::Mcell(const hf_CString& name) const
{
  hf_CCell* ans;
  bool stat = mDict.Get(name, ans);
  return (stat == true && ans->IsMcell()) ? (hf_CMcell*)ans : NULL;
}

// hf_CUcell のセルを名前で検索して返す。
hf_CUcell* hf_CCellMgr::Ucell(const hf_CString& name) const
{
  hf_CCell* ans;
  bool stat = mDict.Get(name, ans);
  return (stat == true && ans->IsUcell()) ? (hf_CUcell*)ans : NULL;
}

// hf_CScell のセルを名前で検索して返す。
hf_CScell* hf_CCellMgr::Scell(const hf_CString& name) const
{
  hf_CCell* ans;
  bool stat = mDict.Get(name, ans);
  return (stat == true && ans->IsScell()) ? (hf_CScell*)ans : NULL;
}

// hf_CZcell のセルを名前で検索して返す。
hf_CZcell* hf_CCellMgr::Zcell(const hf_CString& name) const
{
  hf_CCell* ans;
  bool stat = mDict.Get(name, ans);
  return (stat == true && ans->IsZcell()) ? (hf_CZcell*)ans : NULL;
}

// hf_CXcell のセルを名前で検索して返す。
hf_CXcell* hf_CCellMgr::Xcell(const hf_CString& name) const
{
  hf_CCell* ans;
  bool stat = mDict.Get(name, ans);
  return (stat == true && ans->IsXcell()) ? (hf_CXcell*)ans : NULL;
}

// fdで表された機能を持つセルグループを探す。
// 実際には担当のrelativeを探し、そいつに押しつける。
hf_CUcellGroup* hf_CCellMgr::FindUcellGroup(const hf_CNpnFd& fd) const
{
  hf_CUcellEquiv* equiv = FindUcellEquiv(fd.GetId());
  return (equiv != NULL) ? equiv->FindCellGroup(fd.GetMap()) : NULL;
}

// 上と同じ機能だが、入力が生のBDD
hf_CUcellGroup* hf_CCellMgr::FindUcellGroup(const hf_CBdd& f) const
{
  hf_CNpnFd fd = hf_CNpnMgr::ChkMatch(f, false);
  return fd.GetId() != hf_kNpnBadId ? FindUcellGroup(fd) : NULL;
}

// 上と同じ機能だが、入力が Factored Form 
hf_CUcellGroup* hf_CCellMgr::FindUcellGroup(const hf_CFactForm& ff) const
{
  hf_CBdd fbdd = FactForm2Bdd(ff);
  return FindUcellGroup(fbdd);
}

// fidを持つequivを探す。なければ NULL を返す。
hf_CUcellEquiv* hf_CCellMgr::FindUcellEquiv(hf_tNpnId fid) const
{
  hf_CListLoc loc;
  hf_CUcellEquiv* equiv;
  // 単純なリニアサーチ
  hf_mForEachUcellEquiv(*this, loc, equiv) {
    if (equiv->Fid() == fid) return equiv;
  }
  return NULL;
}

// 新たなセルを作成し、登録する。
hf_CCell* hf_CCellMgr::NewCell(const hf_CString& gname, size_t ni, size_t no)
{
  hf_CCellGroup* const group = NewCellGroup(gname, ni, no);
  if (group != NULL) {
    hf_CCell* const cell = group->NewCell();
    mCells.PutEnd(cell);
    return cell;
  }
  else {
    return NULL;
  }
}

// 新たな hf_CMcell を作成し、登録する。
// 引数に、グループ名、入出力端子数、出力関数のファクタードフォーム
//を指定する。
hf_CMcell* hf_CCellMgr::NewMcell(const hf_CString& gname, size_t ni, size_t no,
				 const hf_CFactForm* ffs)
{
  hf_CMcellGroup* const group = NewMcellGroup(gname, ni, no, ffs);
  if (group != NULL) {
    hf_CMcell* const cell = (hf_CMcell*)group->NewCell();
    mCells.PutEnd(cell);
    return cell;
  }
  else {
    return NULL;
  }
}  

// 新たな hf_CUcell を作成し、登録する。
// 引数に、グループ名、入力端子数、出力関数のファクタードフォーム
//を指定する。
hf_CUcell* hf_CCellMgr::NewUcell(const hf_CString& gname, size_t ni,
				 const hf_CFactForm& ff)
{
  hf_CUcellGroup* const group = NewUcellGroup(gname, ni, ff);
  if (group != NULL) {
    hf_CUcell* const cell = (hf_CUcell*)group->NewCell();
    mCells.PutEnd(cell);
    mUcells.PutEnd(cell);
    return cell;
  }
  else {
    return NULL;
  }
}  

// 新たな hf_CScell を作成し、登録する。
// 引数に、グループ名、入出力端子数を指定する。
hf_CScell* hf_CCellMgr::NewScell(const hf_CString& gname, size_t ni, size_t no)
{
  hf_CScellGroup* const group = NewScellGroup(gname, ni, no);
  if (group != NULL) {
    hf_CScell* const cell = (hf_CScell*)group->NewCell();
    mCells.PutEnd(cell);
    return cell;
  }
  else {
    return NULL;
  }
}  

// 新たな hf_CZcell を作成し、登録する。
// 引数に、グループ名、入出力端子数、バス端子数を指定する。
hf_CZcell* hf_CCellMgr::NewZcell(const hf_CString& gname,
				 size_t ni, size_t no, size_t nb)
{
  hf_CZcellGroup* const group = NewZcellGroup(gname, ni, no, nb);
  if (group != NULL) {
    hf_CZcell* const cell = (hf_CZcell*)group->NewCell();
    mCells.PutEnd(cell);
    return cell;
  }
  else {
    return NULL;
  }
}  

// 新たな hf_CXcell を作成し、登録する。
// 引数に、グループ名、入出力端子数、バス端子数を指定する。
hf_CXcell* hf_CCellMgr::NewXcell(const hf_CString& gname,
				 size_t ni, size_t no, size_t nb)
{
  hf_CXcellGroup* const group = NewXcellGroup(gname, ni, no, nb);
  if (group != NULL) {
    hf_CXcell* const cell = (hf_CXcell*)group->NewCell();
    mCells.PutEnd(cell);
    return cell;
  }
  else {
    return NULL;
  }
}  

// セルを削除する。 cell が NULL の時は何もしない。
// セルがマネージャに登録されていたら true を、それ以外は false を返す。
bool hf_CCellMgr::DeleteCell(hf_CCell* cell)
{
  if (cell == NULL || cell->Name() == NULL) {
    return false;
  }
  mDict.Delete(cell->Name());
  hf_CListLoc loc;
  hf_CCell* cell2;
  hf_mForEachListElem(mCells, loc, cell2) {
    if (cell == cell2) {
      mCells.Delete(loc);
      break;
    }
  }
  if (cell->IsUcell()) {
    hf_CUcell* ucell;
    hf_mForEachListElem(mUcells, loc, ucell) {
      if (cell == ucell) {
	mCells.Delete(loc);
	break;
      }
    }
  }
  delete cell;

  // 削除するセルが Mapped のデフォルトセルだったら、デフォルトセルを
  // 選択し直す。
  if (cell == DefaultCell(hf_kDelayModelMapped)) {
    SetDefaultCell();
  }

  return true;
}

// 名前で検索してセルグループを返す。
// 見つからない場合は NULL ポインタを返す。
hf_CCellGroup* hf_CCellMgr::CellGroup(const hf_CString& name) const
{
  hf_CCellGroup* tmp;
  return mGroupDict.Get(name, tmp) ? tmp : NULL;
}

// 名前で検索してセルグループを返す。
// 見つからない場合は NULL ポインタを返す。
hf_CMcellGroup* hf_CCellMgr::McellGroup(const hf_CString& name) const
{
  hf_CCellGroup* tmp;
  return mGroupDict.Get(name, tmp) && tmp->IsMcellGroup() ?
    (hf_CMcellGroup*)tmp : NULL;
}

// 名前で検索してセルグループを返す。
// 見つからない場合は NULL ポインタを返す。
hf_CUcellGroup* hf_CCellMgr::UcellGroup(const hf_CString& name) const
{
  hf_CCellGroup* tmp;
  return mGroupDict.Get(name, tmp) && tmp->IsUcellGroup() ?
    (hf_CUcellGroup*)tmp : NULL;
}

// 名前で検索してセルグループを返す。
// 見つからない場合は NULL ポインタを返す。
hf_CScellGroup* hf_CCellMgr::ScellGroup(const hf_CString& name) const
{
  hf_CCellGroup* tmp;
  return mGroupDict.Get(name, tmp) && tmp->IsScellGroup() ?
    (hf_CScellGroup*)tmp : NULL;
}

// 名前で検索してセルグループを返す。
// 見つからない場合は NULL ポインタを返す。
hf_CZcellGroup* hf_CCellMgr::ZcellGroup(const hf_CString& name) const
{
  hf_CCellGroup* tmp;
  return mGroupDict.Get(name, tmp) && tmp->IsZcellGroup() ?
    (hf_CZcellGroup*)tmp : NULL;
}

// 名前で検索してセルグループを返す。
// 見つからない場合は NULL ポインタを返す。
hf_CXcellGroup* hf_CCellMgr::XcellGroup(const hf_CString& name) const
{
  hf_CCellGroup* tmp;
  return mGroupDict.Get(name, tmp) && tmp->IsXcellGroup() ?
    (hf_CXcellGroup*)tmp : NULL;
}

// hf_CCellのセルグループを探す。なければ新たに作成する。
hf_CCellGroup* hf_CCellMgr::NewCellGroup(const hf_CString& gname,
					 size_t ni, size_t no)
{
  if (gname != hf_CString::Null()) {
    // グループ名が与えられた時。
    hf_CCellGroup* group;
    if (mGroupDict.Get(gname, group)) {
      // グループのタイプと端子数をチェック。
      if (group->Ni() != ni || group->No() != no) {
	hf_CString msg = hf_fFormat("Group \"%s\" has different I/O pins, "
				    "or different functions.", gname.Get());
	hf_mThrowError("libCell", "CL07", msg);
      }
      else {
	return group; 
      }
    }
  }
  else {
    return NULL;
  }
  // グループ名が未登録の場合
  hf_CCellGroup* group = new hf_CCellGroup(this, gname, ni, no);
  mGroupDict.Add(gname, group);
  mGroups.PutEnd(group);
  return group;
}

// hf_CMcellのセルグループを探す。なければ新たに作成する。
hf_CMcellGroup* hf_CCellMgr::NewMcellGroup(const hf_CString& gname,
					   size_t ni, size_t no,
					   const hf_CFactForm* ffs)
{
  hf_CMcellGroup *group;
  if (gname != hf_CString::Null()) {
    // グループ名が与えられた時。
    hf_CCellGroup* tmp;
    if (mGroupDict.Get(gname, tmp)) {
      // グループのタイプと端子数をチェック。
      if (! tmp->IsMcellGroup() || tmp->Ni() != ni || tmp->No() != no) {
	hf_CString msg = hf_fFormat("Group \"%s\" has different I/O pins, "
				    "or different functions.", gname.Get());
	hf_mThrowError("libCell", "CL08", msg);
      }
      else {
	// 出力論理の等価性をチェック。
	group = (hf_CMcellGroup*)tmp;
	hf_tIdx i;
	for (i = 0; i < no; i++) {
	  hf_CBdd fbdd = FactForm2Bdd(ffs[i]);
	  if (fbdd != group->Bdd(i)) {
	    hf_CString msg = hf_fFormat("Group \"%s\" has different I/O pins, "
					"or different functions.",
					gname.Get());
	    hf_mThrowError("libCell", "CL08", msg);
	  }
	}
	return group;
      }
    }
  }
  else {
   return NULL;
  }
  // グループ名が未登録の場合
  group = new hf_CMcellGroup(this, gname, ni, no, ffs);
  mGroupDict.Add(gname, group);
  mGroups.PutEnd(group);
  mMcellGroups.PutEnd(group);
  return group;
}

// hf_CUcellのセルグループを探す。なければ新たに作成する。
hf_CUcellGroup* hf_CCellMgr::NewUcellGroup(const hf_CString& gname,
					   size_t ni, const hf_CFactForm& ff)
{
  hf_CUcellGroup *group;

  //ff からグループを検索。なければ作る。
  {
    hf_CBdd fbdd = FactForm2Bdd(ff);
    hf_CNpnFd fd = hf_CNpnMgr::ChkMatch(fbdd, true);
    hf_mAssert(fd.GetId() != hf_kNpnBadId, "libCell", "CL26");
    hf_CUcellEquiv* equiv = NewUcellEquiv(fd.GetId());
    group = equiv->FindCellGroup(fd.GetMap());
    if (group == NULL) {
      group = new hf_CUcellGroup(equiv, gname, ni, ff, fd.GetMap());
      mGroups.PutEnd(group);
      mMcellGroups.PutEnd(group);
      mUcellGroups.PutEnd(group);
      equiv->AddGroup(group);
    }
  }

  // グループ名が与えられた時の処理
  if (gname != hf_CString::Null()) {
    if (group->Name() == NULL) {
      //名前が無いので、付ける。
      group->SetName(gname);
      mGroupDict.Add(gname, group);
    }
    else {
      //group の名前と gname との整合をチェック。
      if (group->Name() != gname) {
	//違う名前で同じ関数のグループを登録しようとした
	//Celsyn に存在するので、敢えてエラーにせず、 gname を無視する。
	if (mCelsynMgr && mCelsynMgr->GetDebug()) {
	  mCelsynMgr->AddErrorList(hf_fFormat("[CL25] (Warning): "
	      "Group name \"%s\" conflicts to an existing group \"%s\"\n"
	      "Ucell Function = ", gname.Get(), group->Name().Get()));
	}
      }
      else {
	hf_CCellGroup* tmp;
	if (mGroupDict.Get(gname, tmp)) {
	  //グループ名辞書にgnameが既に登録されていれば、同一かどうかチェック。
	  if (tmp != group) {
	    hf_CString msg = hf_fFormat("Group \"%s\" has different I/O pins, "
					"or different functions.",
					gname.Get());
	    hf_mThrowError("libCell", "CL09", msg);
	  }
	}
      }
    }
  }

  return group;
}

// hf_CScellのセルグループを探す。なければ新たに作成する。
hf_CScellGroup* hf_CCellMgr::NewScellGroup(const hf_CString& gname,
					   size_t ni, size_t no)
{
  if (gname != hf_CString::Null()) {
    // グループ名が与えられた時。
    hf_CCellGroup* tmp;
    if (mGroupDict.Get(gname, tmp)) {
      // グループのタイプと端子数をチェック。
      if (! tmp->IsScellGroup() || tmp->Ni() != ni || tmp->No() != no) {
	hf_CString msg = hf_fFormat("Group \"%s\" has different I/O pins, "
				    "or different functions.",
				    gname.Get());
	hf_mThrowError("libCell", "CL10", msg);
      }
      else {
	return (hf_CScellGroup*)tmp; 
      }
    }
  }
  else {
   return NULL;
  }
  // グループ名が未登録の場合
  hf_CScellGroup* group = new hf_CScellGroup(this, gname, ni, no);
  mGroupDict.Add(gname, group);
  mGroups.PutEnd(group);
  mScellGroups.PutEnd(group);
  return group;
}

// hf_CZcellのセルグループを探す。なければ新たに作成する。
hf_CZcellGroup* hf_CCellMgr::NewZcellGroup(const hf_CString& gname,
					   size_t ni, size_t no, size_t nb)
{
  if (gname != hf_CString::Null()) {
    // グループ名が与えられた時。
    hf_CCellGroup* tmp;
    if (mGroupDict.Get(gname, tmp)) {
      // グループのタイプと端子数をチェック。
      if (! tmp->IsZcellGroup() || tmp->Ni() != ni || tmp->No() != no
	  || ((hf_CZcellGroup*)tmp)->Nb() != nb) {
	hf_CString msg = hf_fFormat("Group \"%s\" has different I/O pins, "
				    "or different functions.",
				    gname.Get());
	hf_mThrowError("libCell", "CL11", msg);
      }
      else {
	return (hf_CZcellGroup*)tmp; 
      }
    }
  }
  else {
   return NULL;
  }
  // グループ名が未登録の場合
  hf_CZcellGroup *group = new hf_CZcellGroup(this, gname, ni, no, nb);
  mGroupDict.Add(gname, group);
  mGroups.PutEnd(group);
  mZcellGroups.PutEnd(group);
  return group;
}

// hf_CXcellのセルグループを探す。なければ新たに作成する。
hf_CXcellGroup* hf_CCellMgr::NewXcellGroup(const hf_CString& gname,
					   size_t ni, size_t no, size_t nb)
{
  if (gname != hf_CString::Null()) {
    // グループ名が与えられた時。
    hf_CCellGroup* tmp;
    if (mGroupDict.Get(gname, tmp)) {
      // グループのタイプと端子数をチェック。
      if (! tmp->IsXcellGroup() || tmp->Ni() != ni || tmp->No() != no
	  || ((hf_CXcellGroup*)tmp)->Nb() != nb) {
	hf_CString msg = hf_fFormat("Group \"%s\" has different I/O pins, "
				    "or different functions.",
				    gname.Get());
	hf_mThrowError("libCell", "CL15", msg);
      }
      else {
	return (hf_CXcellGroup*)tmp; 
      }
    }
  }
  else {
   return NULL;
  }
  // グループ名が未登録の場合
  hf_CXcellGroup *group = new hf_CXcellGroup(this, gname, ni, no, nb);
  mGroupDict.Add(gname, group);
  mGroups.PutEnd(group);
  mXcellGroups.PutEnd(group);
  return group;
}

// fidを持つequivを探す。なければ新たに作成する。
hf_CUcellEquiv* hf_CCellMgr::NewUcellEquiv(hf_tNpnId fid)
{
  hf_CUcellEquiv* equiv = FindUcellEquiv(fid);
  if (equiv == NULL) {
    equiv = new hf_CUcellEquiv(this, fid);
    mEquivs.PutEnd(equiv);
  }
  return equiv;
}

// 各遅延モデルのデフォルトセルを設定する。
// Mapped には2-NANDの最小のものを取り出す。
// 現状の Unit と Unit Fanout はチューニングが必要。
bool hf_CCellMgr::SetDefaultCell(void) const
{
  // 今までのデフォルトセルを解放する。
  for (int i = 0; i < hf_kDelayModelNum; i++) {
    if (i != hf_kDelayModelMapped) {
      delete mDefaultCell[i];
    }
    mDefaultCell[i] = NULL;
  }

  // ライブラリ中のセルで、2-NANDの最小のものを Mapped のデフォルトセルにする。
  hf_CBdd f = ~(hf_CBdd::Literal(0) & hf_CBdd::Literal(1));
  hf_CUcellGroup* defgroup = FindUcellGroup(f);
  if (defgroup == NULL) {
    defgroup = InvGroup();
    if (defgroup == NULL)
      return false;
  }
  const hf_CCell* const mcell = defgroup->MinSizeCell();
  if (mcell == NULL)
    return false;

  // Warning の回避のために const を取り除く。
  mDefaultCell[hf_kDelayModelMapped] = (hf_CCell*) mcell;

  // rise/fall の平均値を取った値。
  const hf_tLoad  aveIlu   = mcell->IpinsIlu(0).Average();
  const hf_CLoad2 aveIlu2  = hf_CLoad2(aveIlu);
  const hf_tSlew  aveSlew  = mcell->IpinsDefaultSlew(0).Average();
  const hf_CSlew2 aveSlew2 = hf_CSlew2(aveSlew);
  const hf_tLoad  aveOlu   = mcell->OpinsOlu(0);
  const hf_CSlew2 aveTs0_2 = hf_CSlew2(mcell->OpinsTs0(0).Average());
  const hf_CKscl2 aveKscl2 = hf_CKscl2(mcell->OpinsKscl(0).Average());

  // パス (in:0 ->out:0) の全てのイベントの平均値を取った値。
  hf_tTime  aveT0 = 0;
  hf_tKcl   aveKcl = 0;
  hf_tAlpha aveAlpha = 0;
  hf_tBeta  aveBeta = 0;

  hf_tEvent ev;
  int nEv = 0;	// イベント数のカウント
  hf_mForEachEvent(ev) {
    const hf_CDelayPara* dpara;
    if ((dpara = mcell->DelayPara(0, 0, ev)) != NULL) {
      nEv ++;
      aveT0    += dpara->T0();
      aveKcl   += dpara->Kcl();
      aveAlpha += dpara->Alpha();
      aveBeta  += dpara->Beta();
    }
  }
  aveT0    /= nEv;
  aveKcl   /= nEv;
  aveAlpha /= nEv;
  aveBeta  /= nEv;

  // Mapped のデフォルトセルから Unit のデフォルトセルを作る。
  {
    hf_CCell* const ucell
      = hf_CCell::DummyCell("Dummy Cell for Unit Delay Model");
    mDefaultCell[hf_kDelayModelUnit] = ucell;
    ucell->SetIpin(0, "in", aveIlu2, aveSlew2);
    ucell->SetOpin(0, "out", aveOlu, aveTs0_2, aveKscl2);
    const hf_tTime t0 = aveT0 + aveAlpha * aveSlew
				+ (aveKcl + aveBeta * aveSlew) * aveIlu;
    hf_mForEachEvent(ev) {
      // 全てのイベントに登録
      ucell->SetDelayPara(0, 0, ev, t0, 0 /*kcl*/, 0 /*alpha*/, 0 /*beta*/);
    }
  }

  // Mapped のデフォルトセルから Unit Fanout のデフォルトセルを作る。
  {
    hf_CCell* const fcell
      = hf_CCell::DummyCell("Dummy Cell for Unit Fanout Delay Model");
    mDefaultCell[hf_kDelayModelUnitFanout] = fcell;
    fcell->SetIpin(0, "in", aveIlu2, aveSlew2);
    fcell->SetOpin(0, "out", aveOlu, aveTs0_2, aveKscl2);
    const hf_tTime t0 = aveT0 + aveAlpha * aveSlew;
    const hf_tKcl kcl = aveKcl + aveBeta * aveSlew;
    hf_mForEachEvent(ev) {
      // 全てのイベントに登録
      fcell->SetDelayPara(0, 0, ev, t0, kcl, 0 /*alpha*/, 0 /*beta*/);
    }
  }

  return true;
}

// デフォルトの負荷依存係数を取り出す。
hf_CKcl2 hf_CCellMgr::DefaultKcl(hf_tDelayModel model) const
{
  const hf_CCell* cell = DefaultCell(model);
  hf_CSlew2 slew = DefaultSlew(model);
  const hf_CDelayPara* para[2];
  hf_tKcl ans[2];
  hf_tBv val;
  hf_mForEachBv(val) {
    cell->DelayPara(0, 0, val, para);
    hf_tKcl Kcl1 = 0.0;
    if (para[hf_kBv0]) {
      Kcl1 = para[hf_kBv0]->Kcl(slew[hf_kBv0]);
    }
    hf_tKcl Kcl2 = 0.0;
    if (para[hf_kBv1]) {
      Kcl2 = para[hf_kBv1]->Kcl(slew[hf_kBv1]);
    }
    ans[val] = (Kcl1 > Kcl2) ? Kcl1 : Kcl2;
  }
  return hf_CKcl2(ans);
}

// セルの種類を表す定数
enum tCellType{kCell, kUcell, kMcell, kScell, kZcell, kXcell};

static tCellType CellType(const hf_CCell* gcell)
{
  if (gcell->IsXcell()) {
    return kXcell;
  }
  else if (gcell->IsZcell()) {
    return kZcell;
  }
  else if (gcell->IsScell()) {
    return kScell;
  }
  else if (gcell->IsUcell()) {
    return kUcell;
  }
  else if (gcell->IsMcell()) {
    return kMcell;
  }
  else {
    return kCell;
  }
}

//バイナリダンプファイルの識別子とバージョン
//上位互換の変更をしたら、Revision が上がるたびに gBindumpRevision を増やす。
const char* gBindumpID = "libCell Bindump File 0.2";
const short gBindumpRevision = 0;

// バイナリダンプする。
const hf_CBinDump& operator<<(const hf_CBinDump& bindump,
			      const hf_CCellMgr& mgr)
{
  //バイナリダンプファイルの識別子とバージョンを出力
  bindump << gBindumpID;
  bindump << gBindumpRevision;

    //セル総数
  bindump << (long)mgr.CellNum();
  hf_CListLoc loc;
  hf_CCell* gcell;
  hf_mForEachCell(mgr, loc, gcell) {
    tCellType type = CellType(gcell);
    //セルの、種類の識別子、グループ名、入出力数を出力
    bindump << (short)type << hf_CString(gcell->CellGroup()->Name());
    bindump << (short)gcell->Ni() << (short)gcell->No();
    //セル種類固有の情報
    switch (type) {
    case kCell:
      bindump << *gcell;
      break;
    case kUcell:
      {
	const hf_CUcell *const ucell = (hf_CUcell*)gcell;
	bindump << ucell->OpinsFunc();
	bindump << *ucell;
      }
      break;
    case kMcell:
      {
	const hf_CMcell *const mcell = (hf_CMcell*)gcell;
	hf_tIdx i;
	for (i = 0; i < mcell->No(); i++) {
	  bindump << mcell->OpinsFunc(i);
	}
	bindump << *mcell;
      }
      break;
    case kScell:
      {
	const hf_CScell *const scell = (hf_CScell*)gcell;
	bindump << *scell;
      }
      break;
    case kZcell:
      {
	const hf_CZcell *const zcell = (hf_CZcell*)gcell;
	bindump << (short)zcell->Nb();
	bindump << *zcell;
      }
      break;
    case kXcell:
      {
	const hf_CXcell *const xcell = (hf_CXcell*)gcell;
	bindump << (short)xcell->Nb();
	bindump << *xcell;
      }
      break;
    }
  }

  //登録されているセルが１つもない UcellGroup の保存
  const hf_CUcellGroup *ugroup;
  {
    // セルを持たない UcellGroup の個数を出力
    long num = 0;
    hf_mForEachUcellGroup(mgr, loc, ugroup) {
      if (ugroup->CellNum() == 0) num ++;
    }
    bindump << num;
  }

  hf_mForEachUcellGroup(mgr, loc, ugroup) {
    if (ugroup->CellNum() > 0) continue;
  
    //グループ名と出力関数を出力
    bindump << hf_CString(ugroup->Name()) << (short)ugroup->Ni();
    bindump << ugroup->OpinsFunc();
  }

  return bindump;
}

// バイナリを読み込む。
const hf_CBinDump& operator>>(const hf_CBinDump& bindump,
			      hf_CCellMgr& mgr)
{
  hf_CString id;
  short revision;
  long num;
  short type;
  hf_CString gname;
  short ni, no;

  //バイナリダンプファイルのバージョンをチェック
  bindump >> id;
  if (id != gBindumpID) {
    hf_CString msg = hf_fFormat("Cannot read a bindump file:"
				"file ID \"%s\" != program ID \"%s\"",
				id.Get(), gBindumpID);
    hf_mThrowError("libCell", "CL22", msg);
  }
  bindump >> revision;
  if (revision > gBindumpRevision) {
    hf_CString msg = hf_fFormat("Cannot read a bindump file:"
				"file rev. %d > program rev. %d",
				revision, gBindumpRevision);
    hf_mThrowError("libCell", "CL23", msg);
  }

  mgr.Clear();

  //セル総数
  bindump >> num;

#if 0
  // progress bar
  int num_total = num;
  unsigned progress;
  unsigned progress_last = 1000;
#endif

  for (; num > 0; num --) {
#if 0
    if(progBar!=NULL) {
      progress = (num_total-num+1)*100/num_total;
      if(progress!=progress_last) {
	hf_CEventList el;
	//printf("%d ", progress);
	el.add( progress );
	el.add( "..." );
	progBar->EventProc(el);
	progress_last = progress;
      }
    }
#endif
    bindump >> type >> gname >> ni >> no;
    switch ((tCellType)type) {
    case kCell:
      {
	hf_CCell *const cell = mgr.NewCell(gname, ni, no);
	bindump >> *cell;
      }
      break;
    case kUcell:
      {
	hf_CFactForm ff;
	bindump >> ff;
	hf_CUcell *const ucell = mgr.NewUcell(gname, ni, ff);
	bindump >> *ucell;
      }
      break;
    case kMcell:
      {
	hf_CFactForm *ffs = new hf_CFactForm [no];
	hf_tIdx i;
	for (i = 0; i < (size_t) no; i++) {
	  bindump >> ffs[i];
	}
	hf_CMcell *const mcell = mgr.NewMcell(gname, ni, no, ffs);
	delete [] ffs;
	bindump >> *mcell;
      }
      break;
    case kScell:
      {
	hf_CScell *const scell = mgr.NewScell(gname, ni, no);
	bindump >> *scell;
      }
      break;
    case kZcell:
      {
	short nb;
	bindump >> nb;
	hf_CZcell *const zcell = mgr.NewZcell(gname, ni, no, nb);
	bindump >> *zcell;
      }
      break;
    case kXcell:
      {
	short nb;
	bindump >> nb;
	hf_CXcell *const xcell = mgr.NewXcell(gname, ni, no, nb);
	bindump >> *xcell;
      }
      break;
    default:
      {
	hf_CString cellName;
	bindump >> cellName;
	hf_CString msg = hf_fFormat("Invalid cell type: %d, name = \"%s\"",
				    type, cellName.Get());
	hf_mThrowError("libCell", "CL24", msg);
      }
    }
  }
  
  //登録されているセルが１つもない UcellGroup の処理
  bindump >> num;
  for (; num > 0; num--) {
    hf_CFactForm ff;
    //グループ名と出力関数を読む
    bindump >> gname >> ni;
    bindump >> ff;
    //ff の論理関数を持つ UcellGroup について、無ければ作り、グループ名が
    //ついていなければ gname を付ける。
    (void) mgr.NewUcellGroup(gname, ni, ff);
  }
  return bindump;
}

// 全ての Ucell グループの名前を調べて、無ければユニークな名前を付ける。
// 名前は整数 (startNo から始まる) を10進文字列に変換したもの。
// MisLib のようにグループ名が無いライブラリ向け。
hf_tIdx hf_CCellMgr::CheckUcellGroupName(hf_tIdx startNo)
{
  hf_CUcellGroup *group;
  hf_CListLoc loc;
  char gname[sizeof(int)*3+2];	// 10進表記は１バイト当たり３桁で十分。
				// 加えてマイナスと終端文字用に２足す。
  hf_mForEachUcellGroup(*this, loc, group) {
    if (group->Name() == NULL) {
      hf_CCellGroup *tmp;
      do {
	sprintf(gname, "%d", startNo);
	if (startNo ++ == UINT_MAX) {
	  hf_mThrowError("libCell", "CL22",
			 "Number for Ucell groups exausted");
	}
      } while (mGroupDict.Get(gname, tmp));
      group->SetName(gname);
      mGroupDict.Add(gname, group);
    }
  }
  return startNo;
}

// デバッグ用の内容表示関数
void hf_CCellMgr::Dump(FILE* fp) const
{
  fprintf(fp, "# of cells          = %d\n", CellNum());
  fprintf(fp, "# of cell groups    = %d\n", CellGroupNum());
  fprintf(fp, "# of cell relatives = %d\n", UcellEquivNum());
  hf_CListLoc loc;
  hf_CUcellEquiv* equiv;
  hf_mForEachUcellEquiv(*this, loc, equiv) {
    equiv->Dump(fp);
  }
}
#endif
