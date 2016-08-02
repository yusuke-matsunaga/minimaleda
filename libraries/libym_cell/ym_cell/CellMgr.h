#ifndef YM_CELL_CELLMGR_H
#define YM_CELL_CELLMGR_H

/// @file　ym_cell/Cell.h
/// @brief Cell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id:
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


// クラス名宣言
class Cell;
class CellGroup;
class Mcell;
class McellGroup;
class Ucell;
class UcellGroup;
class UcellEquiv;
class Scell;
class ScellGroup;
class Zcell;
class ZcellGroup;
class Xcell;
class XcellGroup;
class Celsyn2Cell;

typedef hf_TSAssoc<Cell*> CellDict;
typedef hf_TSAssoc<CellGroup*> CellGroupDict;

////////////////////////////////////////////
// Cellのインスタンスを管理するクラス //
////////////////////////////////////////////
class CellMgr {
  friend class Cell;
private:
  hf_TList<Cell*> mCells;	// Cellを収めるリスト
  hf_TList<Ucell*> mUcells;	// Ucellを収めるリスト
  CellDict mDict;		// Cellを名前で検索する辞書
  UcellEquiv* mR_const;	// 定数関数グループの同値類
  UcellGroup* mG_const0;	// 定数 0 関数グループ
  UcellGroup* mG_const1;	// 定数 1 関数グループ
  UcellEquiv* mR_buf;	// バッファ／インバータグループの同値類
  UcellGroup* mG_buf;	// バッファのグループ
  UcellGroup* mG_inv;	// インバータのグループ

  CellGroupDict mGroupDict;		// CellGroupを名前で検索する辞書
  hf_TList<CellGroup*> mGroups;	// 全CellGroupを収めるリスト
  hf_TList<McellGroup*> mMcellGroups;  // 出力に論理を持ったセルグループ
  hf_TList<UcellGroup*> mUcellGroups;  // 1出力で論理を持ったセルグループ
  hf_TList<ScellGroup*> mScellGroups;  // 順序回路セルグループ
  hf_TList<ZcellGroup*> mZcellGroups;  // Tri-state を持ったセルグループ
  hf_TList<XcellGroup*> mXcellGroups;  // Xcellセルグループ

  hf_TList<UcellEquiv*> mEquivs;	// UcellEquiveを収めるリスト

  Celsyn2Cell*	mCelsynMgr;	// Celsyn のコモン情報を管理。
  
  // gname のグループを探し、なければ新たに作る。
  virtual CellGroup* NewCellGroup(const char* gname, int ni, int no);
  virtual McellGroup* NewMcellGroup(const char* gname,
				int ni, int no, const FactForm* ffs);
  virtual UcellGroup* NewUcellGroup(const char* gname,
					int ni, const FactForm& ff);
  virtual ScellGroup* NewScellGroup(const char* gname, int ni, int no);
  virtual ZcellGroup* NewZcellGroup(const char* gname,
					int ni, int no, int nb);
  virtual XcellGroup* NewXcellGroup(const char* gname,
					int ni, int no, int nb);

  // 全ての Ucell グループの名前を調べて、無ければユニークな名前を付ける。
  // 名前は整数 (startNo から始まる) を10進文字列に変換したもの。
  // 返り値は、最後のグループ名 + 1。
  // MisLib のようにグループ名が無いライブラリ向け。
  int CheckUcellGroupName(int startNo = 0);
  
protected:
  // デフォルト値設定用のセルをとってくる。
  // このクラスは最小面積の2-NAND。継承クラスによって変えることが出来る。
  virtual const Cell* GetDefaultCell(void) const;

public:
  // コンストラクタ
  CellMgr(void);

  // デストラクタ
  virtual ~CellMgr(void);
private:
  // コピーは禁止。
  CellMgr(const CellMgr& src);
  const CellMgr& operator=(const CellMgr& src);
public:
  // 登録されているセル数を返す。
  int GetCellNum(void) const { return mCells.GetNum(); }

  // GetCell(loc)のためのロケータをセットする。
  void SetCellLoc(ListLoc& loc) const
  {
    loc.Attach(mCells);
  }

  // locで指定されたセルを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  Cell* GetCell(const ListLoc& loc) const
  {
    Cell* ans;
    hf_tBoole stat = mCells.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 名前で検索してセルを返す。
  // 見つからない場合は NULL ポインタを返す。
  Cell* GetCell(const char* name) const;

  // 名前で検索して Mcell を返す。
  // 見つからない場合は NULL ポインタを返す。
  Mcell* GetMcell(const char* name) const;

  // 名前で検索して Ucell を返す。
  // 見つからない場合は NULL ポインタを返す。
  Ucell* GetUcell(const char* name) const;

  // 名前で検索して Scell を返す。
  // 見つからない場合は NULL ポインタを返す。
  Scell* GetScell(const char* name) const;

  // 名前で検索して Zcell を返す。
  // 見つからない場合は NULL ポインタを返す。
  Zcell* GetZcell(const char* name) const;

  // 名前で検索して Xcell を返す。
  // 見つからない場合は NULL ポインタを返す。
  Xcell* GetXcell(const char* name) const;

  // 登録されている Ucell の個数を返す。
  int GetUcellNum(void) const { return mUcells.GetNum(); }

  // GetUcell(loc)のためのロケータをセットする。
  void SetUcellLoc(ListLoc& loc) const
  {
    loc.Attach(mUcells);
  }

  // locで指定された Ucell を取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  Ucell* GetUcell(const ListLoc& loc) const
  {
    Ucell* ans;
    hf_tBoole stat = mUcells.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 新たなセルを作成し、登録する。
  // 引数に、グループ名と入出力端子数を指定する。
  // 属するグループが無い場合は自動的に作られる。
  virtual Cell* NewCell(const char* gname, int ni, int no);
  
  // 登録されているCellGroupの数を求める。
  int GetCellGroupNum(void) const { return mGroups.GetNum(); }

  // GetCellGroup(loc)のためのロケータをセットする。
  void SetCellGroupLoc(ListLoc& loc) const
  {
    loc.Attach(mGroups);
  }

  // locで指定されたCellGroupを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  CellGroup* GetCellGroup(const ListLoc& loc) const
  {
    CellGroup* ans;
    hf_tBoole stat = mGroups.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 名前で検索してセルグループを返す。
  // 見つからない場合は NULL ポインタを返す。
  CellGroup* GetCellGroup(const char* name) const;

  // 新たな Mcell を作成し、登録する。
  // 引数に、グループ名、入出力端子数、出力関数のファクタードフォーム
  //を指定する。
  // 属するグループが無い場合は自動的に作られる。
  virtual Mcell* NewMcell(const char* gname,
			      int ni, int no, const FactForm* ffs);
  
  // 登録されているMcellGroupの数を求める。
  int GetMcellGroupNum(void) const { return mMcellGroups.GetNum(); }

  // GetMcellGroup(loc)のためのロケータをセットする。
  void SetMcellGroupLoc(ListLoc& loc) const {
    loc.Attach(mMcellGroups);
  }

  // locで指定されたMcellGroupを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  McellGroup* GetMcellGroup(const ListLoc& loc) const
  {
    McellGroup* ans;
    hf_tBoole stat = mMcellGroups.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 名前で検索してセルグループを返す。
  // 見つからない場合は NULL ポインタを返す。
  McellGroup* GetMcellGroup(const char* name) const;

  // 新たな Ucell を作成し、登録する。
  // 引数に、グループ名、入力数、出力関数のファクタードフォームを指定する。
  // 属するグループが無い場合は自動的に作られる。
  virtual Ucell* NewUcell(const char* gname, int ni,
			      const FactForm& ff);
  
  // 登録されているUcellGroupの数を求める。
  int GetUcellGroupNum(void) const { return mUcellGroups.GetNum(); }

  // GetUcellGroup(loc)のためのロケータをセットする。
  void SetUcellGroupLoc(ListLoc& loc) const
  {
    loc.Attach(mUcellGroups);
  }

  // locで指定されたUcellGroupを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  UcellGroup* GetUcellGroup(const ListLoc& loc) const
  {
    UcellGroup* ans;
    hf_tBoole stat = mUcellGroups.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 名前で検索してセルグループを返す。
  // 見つからない場合は NULL ポインタを返す。
  UcellGroup* GetUcellGroup(const char* name) const;

  // factored form ff で表された機能を持つセルグループを探す。
  // 見つからなければ NULL ポインタを返す。
  UcellGroup* FindUcellGroup(const FactForm& ff) const;

  // fdで表された機能を持つセルグループを探す。
  // 見つからなければ NULL ポインタを返す。
  UcellGroup* FindUcellGroup(const NpnFd& fd) const;

  // BDD fで表された機能を持つセルグループを探す。
  // 見つからなければ NULL ポインタを返す。
  UcellGroup* FindUcellGroup(const Bdd& f) const;

  // 新たな Scell を作成し、登録する。
  // 引数に、グループ名、入出力数を指定する。
  // 属するグループが無い場合は自動的に作られる。
  virtual Scell* NewScell(const char* gname, int ni, int no);
  
  // 登録されているScellGroupの数を求める。
  int GetScellGroupNum(void) const { return mScellGroups.GetNum(); }

  // GetScellGroup(loc)のためのロケータをセットする。
  void SetScellGroupLoc(ListLoc& loc) const
  {
    loc.Attach(mScellGroups);
  }

  // locで指定されたScellGroupを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  ScellGroup* GetScellGroup(const ListLoc& loc) const
  {
    ScellGroup* ans;
    hf_tBoole stat = mScellGroups.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 名前で検索してセルグループを返す。
  // 見つからない場合は NULL ポインタを返す。
  ScellGroup* GetScellGroup(const char* name) const;

  // 新たな Zcell を作成し、登録する。
  // 引数に、グループ名、入出力数、バス端子数を指定する。
  // 属するグループが無い場合は自動的に作られる。
  virtual Zcell* NewZcell(const char* gname, int ni, int no, int nb);
  
  // 登録されているZcellGroupの数を求める。
  int GetZcellGroupNum(void) const { return mZcellGroups.GetNum(); }

  // GetZcellGroup(loc)のためのロケータをセットする。
  void SetZcellGroupLoc(ListLoc& loc) const
  {
    loc.Attach(mZcellGroups);
  }

  // locで指定されたZcellGroupを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  ZcellGroup* GetZcellGroup(const ListLoc& loc) const
  {
    ZcellGroup* ans;
    hf_tBoole stat = mZcellGroups.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 名前で検索してセルグループを返す。
  // 見つからない場合は NULL ポインタを返す。
  ZcellGroup* GetZcellGroup(const char* name) const;

  // 新たな Xcell を作成し、登録する。
  // 引数に、グループ名、入出力数、バス端子数を指定する。
  // 属するグループが無い場合は自動的に作られる。
  virtual Xcell* NewXcell(const char* gname, int ni, int no, int nb);
  
  // 登録されているXcellGroupの数を求める。
  int GetXcellGroupNum(void) const { return mZcellGroups.GetNum(); }

  // GetXcellGroup(loc)のためのロケータをセットする。
  void SetXcellGroupLoc(ListLoc& loc) const
  {
    loc.Attach(mZcellGroups);
  }

  // locで指定されたXcellGroupを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  XcellGroup* GetXcellGroup(const ListLoc& loc) const
  {
    XcellGroup* ans;
    hf_tBoole stat = mXcellGroups.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 名前で検索してセルグループを返す。
  // 見つからない場合は NULL ポインタを返す。
  XcellGroup* GetXcellGroup(const char* name) const;

  // fidを持つ同値類を探す。
  // 見つからない場合は NULL ポインタを返す。
  UcellEquiv* FindUcellEquiv(hf_tNpnId fid) const;

  // fidを持つ同値類を探す。なければ新たに作成する。
  virtual UcellEquiv* NewUcellEquiv(hf_tNpnId fid);

  // UcellEquivの数を求める。
  int GetUcellEquivNum(void) const { return mEquivs.GetNum(); }

  // GetEvuiq(loc)のためのロケータをセットする。
  void SetUcellEquivLoc(ListLoc& loc) const { loc.Attach(mEquivs); }

  // locで指定されたUcellEquivを取り出す。
  // loc が範囲外の場合は NULL ポインタを返す。
  UcellEquiv* GetUcellEquiv(const ListLoc& loc) const
  {
    UcellEquiv* ans;
    hf_tBoole stat = mEquivs.Get(loc, ans);
    return (stat == hf_kTrue) ? ans : NULL;
  }

  // 定数グループの同値類を取り出す。
  UcellEquiv* GetConstEquiv(void) const { return mR_const; }

  // 定数 0 のグループを取り出す。
  // ライブラリによっては 1つもセルが登録されていないかもしれない。
  UcellGroup* GetConst0Group(void) const { return mG_const0; }

  // 定数 1 のグループを取り出す。
  // ライブラリによっては 1つもセルが登録されていないかもしれない。
  UcellGroup* GetConst1Group(void) const { return mG_const1; }

  // バッファ／インバータグループの同値類を取り出す。
  UcellEquiv* GetBufEquiv(void) const { return mR_buf; }

  // バッファのグループを取り出す。
  // ライブラリによっては 1つもセルが登録されていないかもしれない。
  UcellGroup* GetBufGroup(void) const { return mG_buf; }

  // インバータのグループを取り出す。
  // ライブラリによっては 1つもセルが登録されていないかもしれない。
  UcellGroup* GetInvGroup(void) const { return mG_inv; }

  // デフォルトの入力負荷を取り出す。
  const Load2& GetDefaultIlu(void) const;

  // デフォルトのスルー関数を取り出す。
  const SlewFunc2& GetDefaultSlewFunc(void) const;

  // デフォルトのスルー値(負荷もデフォルト)を取り出す。
  Slew2 GetDefaultSlew(void) const;

  // デフォルトの負荷依存係数(Kcl)を取り出す。
  Kcl2 GetDefaultKcl(void) const;

  //セルの内容を全て破棄する。
  //予約済みの CellGroup (0, 1, Buf, INV)とそれらのCellEquiv を作る。
  virtual void Clear(void);
  
  // MIS 形式のライブラリ情報を読み込む。
  // 読み込みに失敗したときは False を返す。
  hf_tBoole ReadMisLib(FILE* fp);
  
  // MIS 形式のライブラリ情報を書き込む。
  void WriteMisLib(FILE* fp) const;
  
  // Celsyn のコモン情報を管理するマネージャを返す。
  // Celsyn を読み込んでいない場合は NULL ポインタを返す。
  const Celsyn2Cell* GetCelsynMgr(void) const { return mCelsynMgr; }

  // Celsyn 形式のライブラリ情報を読み込む。
  // 読み込みに成功したら True を、失敗したら False を返す。
  //
  // celsynDir は Celsyn ライブラリのディレクトリを指定する。celsynDir 配下に
  //         "common" と "celsyn" のエントリがあることを仮定する。"celsyn" が
  //         単一ファイルの場合はバイナリデータと見なす。
  // commonFileOrCellFile でライブラリコモン情報ファイルか、1つのセルファイル
  //         を指定。NULL なら読まない。
  // cellDir でセルファイルを納めたディレクトリを指定。NULL なら読まない。
  //
  // 最初の呼び出しは、必ずコモン情報ファイル名を commonFileOrCellFile に
  // セットする。
  //
  // 1つのセルを追加するとき、そのファイル名を commonFileOrCellFileにセット
  // し、 cellDir = NULL で呼び出す。
  //
  // 複数セルを追加するときは、そのディレクトリ名を cellDir にセットし、
  // commonFileOrCellFile = NULL で呼び出す。
  hf_tBoole ReadCelsyn(const char* celsynDir);
  hf_tBoole ReadCelsyn(const char* commonFileOrCellFile, const char* cellDir);

  
  // デバッグ用の内容表示関数
  void Dump(FILE* fp) const;

  // 内容をバイナリダンプする。
  friend const BinDump& operator<<(const BinDump& bindump,
				       const CellMgr& mgr);

  // バイナリを読み込む。
  friend const BinDump& operator>>(const BinDump& bindump,
				       CellMgr& mgr);
};

// Cellを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// Cell* Cell;
// hf_mForEachCell(obj, loc, Cell) {
//   .....
// というふうに用いる。
// 定義は、 hf_Ucell.h の中

// Ucellを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// Ucell* Cell;
// hf_mForEachUcell(obj, loc, Cell) {
//   .....
// というふうに用いる。
#define hf_mForEachUcell(obj, loc, Cell)\
  for ((obj).SetUcellLoc(loc); ((Cell) = (obj).GetUcell(loc)) != NULL;\
       (loc).MoveFore())

// CellGroupを順々にとりだすマクロ
//
// CellEquiv obj; または CellMgr obj;
// ListLoc loc;
// CellGroup* group;
// hf_mForEachCellGroup(obj, loc, group) {
//   .....
// というふうに用いる。
// 定義は、 hf_Ucell.h の中

// McellGroupを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// McellGroup* group;
// hf_mForEachMcell(obj, loc, group) {
//   .....
// というふうに用いる。
#define hf_mForEachMcellGroup(obj, loc, group)\
  for ((obj).SetMcellGroupLoc(loc);((group)=(obj).GetMcellGroup(loc))!= NULL;\
       (loc).MoveFore())

// UcellGroupを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// UcellGroup* group;
// hf_mForEachUcell(obj, loc, group) {
//   .....
// というふうに用いる。
#define hf_mForEachUcellGroup(obj, loc, group)\
  for ((obj).SetUcellGroupLoc(loc);((group)=(obj).GetUcellGroup(loc))!=NULL;\
       (loc).MoveFore())

// ScellGroupを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// ScellGroup* group;
// hf_mForEachScell(obj, loc, group) {
//   .....
// というふうに用いる。
#define hf_mForEachScellGroup(obj, loc, group)\
  for ((obj).SetScellGroupLoc(loc);((group)=(obj).GetScellGroup(loc))!=NULL;\
       (loc).MoveFore())

// ZcellGroupを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// ZcellGroup* group;
// hf_mForEachZcell(obj, loc, group) {
//   .....
// というふうに用いる。
#define hf_mForEachZcellGroup(obj, loc, group)\
  for ((obj).SetZcellGroupLoc(loc);((group)=(obj).GetZcellGroup(loc))!=NULL;\
       (loc).MoveFore())

// XcellGroupを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// XcellGroup* group;
// hf_mForEachXcell(obj, loc, group) {
//   .....
// というふうに用いる。
#define hf_mForEachXcellGroup(obj, loc, group)\
  for ((obj).SetXcellGroupLoc(loc);((group)=(obj).GetXcellGroup(loc))!=NULL;\
       (loc).MoveFore())

// UcellEquivを順々にとりだすマクロ
//
// CellMgr obj;
// ListLoc loc;
// UcellEquiv* equiv;
// hf_mForEachUcellEquiv(obj, loc, equiv) {
//   .....
// というふうに用いる。
#define hf_mForEachUcellEquiv(obj, loc, equiv)\
  for ((obj).SetUcellEquivLoc(loc);((equiv)=(obj).GetUcellEquiv(loc))!=NULL;\
       (loc).MoveFore())

#endif // YM_CELL_CELLMGR_H
