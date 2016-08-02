#ifndef HF_CELL_INT_H
#define HF_CELL_INT_H

// $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Cell_int.h,v $
// $Author: matsunaga $
// $Revision: 1.1.1.1 $
// $Date: 2005/12/11 17:07:26 $

// 複数出力、組合せ回路、Z 状態を出力しないセルを表すクラス hf_CCell
// とそのセルの機能グループを扱う hf_CCellGroup
// Note: セル機能グループ == ドライブ能力が異なるが、機能が等価なセル集合

// 全てのライブラリセルの基底クラスとなる。
// このライブラリのほとんどは TeddyBear の GCell のコードから流用です。

#include <synth/hf_stdincl.h>
#include <synth/hf_Array.h>
#include <synth/hf_String.h>
#include <synth/hf_List.h>
#include <synth/hf_Assoc.h>
#include <synth/hf_PwLnFn.h>
#include <synth/hf_BinDump.h>
#include <synth/hf_Unit.h>

// バージョン情報
static const char* hf_CellIntRCSid UNUSED = "$Header: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Cell_int.h,v 1.1.1.1 2005/12/11 17:07:26 matsunaga Exp $";

// 前方参照のためのクラス定義
class hf_CCell;
class hf_CCellGroup;
class hf_CCellMgr;

// 例外送出用のクラスの宣言。
class hf_CXcpt;

// イベントの区分
//   Z 状態を含まない普通のイベント
enum hf_tEvent {
  hf_kEvent0to0 = 0,	// 入力が 0 になり、出力が 0 になる
  hf_kEvent1to0 = 1,	// 入力が 1 になり、出力が 0 になる
  hf_kEvent0to1 = 2,	// 入力が 0 になり、出力が 1 になる
  hf_kEvent1to1 = 3,	// 入力が 1 になり、出力が 1 になる

  hf_kEvent0to10 = hf_kEvent0to0,
  hf_kEvent1to10 = hf_kEvent1to0,
  hf_kEvent0to01 = hf_kEvent0to1,
  hf_kEvent1to01 = hf_kEvent1to1
};

// Z 状態を含まない普通のイベントの総数。
const int hf_kEventNum = 4;

// 入力側の信号遷移を返す関数
inline hf_tBv In(hf_tEvent ev)
{
  return (hf_tBv)(ev & 1);
}

// 出力側の信号遷移を返す関数
inline hf_tBv Out(hf_tEvent ev)
{
  return (hf_tBv)(ev >> 1);
}

// 入力と出力の信号遷移に対応するイベントを返す関数
inline hf_tEvent Event(hf_tBv inVal, hf_tBv outVal)
{
  return (hf_tEvent)((outVal << 1) + inVal);
}

// 入力側の信号遷移が逆のイベントを返す関数
inline hf_tEvent InvertIn(hf_tEvent ev)
{
  return (hf_tEvent)(ev ^ 1);
}

// 出力側の信号遷移が逆のイベントを返す関数
inline hf_tEvent InvertOut(hf_tEvent ev)
{
  return (hf_tEvent)(ev ^ 2);
}

// イベントに対応する文字列を返す関数
extern const hf_CString& Ev2String(hf_tEvent ev);

// イベントの列挙型をバイナリダンプするための関数
inline const hf_CBinDump& operator<<(const hf_CBinDump& bindump,
				     hf_tEvent ev)
{
  return bindump << (char)ev;
}

inline const hf_CBinDump& operator>>(const hf_CBinDump& bindump,
				     hf_tEvent& ev)
{
  char d;
  bindump >> d;
  ev = (hf_tEvent)d;
  return bindump;
}

//////////////////////////////////
// 機能セルグループを表すクラス //
//////////////////////////////////
class hf_CCellGroup {
  friend class hf_CCellMgr;
private:
  hf_CShStr mName;		// Group の名前
  size_t mNi;			// 入力ピン数
  size_t mNo;			// 出力ピン数
  hf_CCellMgr* mMgr;		// 自分が属する hf_CCellMgr
  hf_TList<hf_CCell*> mCells;	// セルを納めるリスト

public:
  // コンストラクタ。セルマネージャ、入力 & 出力ピン数を引数にとる。
  hf_CCellGroup(hf_CCellMgr* mgr, const hf_CString& gname,
		size_t ni, size_t no);

  // デストラクタ
  virtual ~hf_CCellGroup(void);

private:
  // コピーコンストラクタと代入演算子は、使用禁止。実体も定義しない。
  hf_CCellGroup(const hf_CCellGroup& src);
  const hf_CCellGroup& operator=(const hf_CCellGroup& src);

public:
  // 出力全てに論理が定義された組み合わせ回路セルグループなら True
  virtual bool IsMcellGroup(void) const;

  // 1出力組み合わせ論理セルグループなら True
  virtual bool IsUcellGroup(void) const;

  // 順序回路セルグループなら True
  virtual bool IsScellGroup(void) const;

  // Tri-State セルグループなら True
  virtual bool IsZcellGroup(void) const;

  // パス遅延とタイミング情報が無いソフトマクロセルのグループなら True
  virtual bool IsXcellGroup(void) const;

  // グループ名を求める。
  const hf_CShStr& Name(void) const { return mName; }

  // 入力ピン数を求める
  size_t Ni(void) const { return mNi; }

  // 出力ピン数を求める
  size_t No(void) const { return mNo; }

  // 親のhf_CCellMgrを求める．
  hf_CCellMgr* CellMgr(void) const { return mMgr; }

  // セル数を得る．
  size_t CellNum(void) const { return mCells.Num(); }

  // Cell(loc)のためのロケーターをセットする．
  void SetCellLoc(hf_CListLoc& loc) const
  {
    loc.Attach(mCells);
  }

  // locで指定されたセルを取り出す．
  // 範囲外の場合には 0 を返す．
  hf_CCell* Cell(const hf_CListLoc& loc) const
  {
    hf_CCell* ans;
    bool stat = mCells.Get(loc, ans);
    return (stat == true) ? ans : 0;
  }

  // 名前で検索してセルを返す。
  // 見つからない場合は 0 ポインタを返す。
  hf_CCell* Cell(const hf_CString& name) const;

  // 最もセル面積の小さなセルを取り出す．
  // 同じ面積のセルが複数ある時は駆動能力が最小のセルを取り出す．
  hf_CCell* MinSizeCell(void) const;

  // デバッグ用の内容表示関数
  void Dump(FILE* fp) const;

protected:
  // posが入力ピン数より小さいか調べる．
  void ChkNi(hf_tIdx pos, const char* msg) const
  {
    if (pos >= mNi) {
      hf_mThrowError("libCell", "CL03", msg);
    }
  }

  // posが出力ピン数より小さいか調べる．
  void ChkNo(hf_tIdx pos, const char* msg) const
  {
    if (pos >= mNo) {
      hf_mThrowError("libCell", "CL04", msg);
    }
  }

  // セルのリストの最後に追加する。
  void AddCell(hf_CCell* cell)
  {
    mCells.PutEnd(cell);
  }

  // グループに名前をつける。
  // 出力論理があるセルなど、名前に頼らなくても作る事ができるグループに
  // 対して後から名前をつける時に使う。
  void SetName(const hf_CShStr& gname)
  {
    mName = gname;
  }
  
private:  
  // hf_CellMgr が新たなセルを作成する時に呼ぶ関数。セルを生成すると同
  // 時に、登録(AddCell)もする。派生クラスが overload 出来るように仮想
  // 関数で実現。
  virtual hf_CCell* NewCell(void);
};

// 遅延の計算メソッドを持つクラス
// CG51のタイプのみを考える．
class hf_CDelayPara {
  friend class hf_CCell;
private:
  hf_tTime mT0;          // 基本のオフセット
    			 // mT0 == 0.0 の時、遅延は定義されていない。
  hf_tKcl mKcl;          // 負荷容量依存係数
  hf_tAlpha mAlpha;      // スルーの係数１
  hf_tBeta mBeta;        // スルーの係数２
public:
  // 引数無しのコンストラクタ
  hf_CDelayPara(void) {}

  // コピーコンストラクタはデフォルト生成のを使う。
  //hf_CDelayPara(const hf_CDelayPara& src);

  // 対応するパス＆イベントの遅延パラメタを無効にする。
  void SetInvalid(void)
  {
    mT0 = mKcl = mAlpha = mBeta = DBL_MAX;
  }

  // 各パラメータを設定する．
  void Set(hf_tTime t0, hf_tKcl Kcl, hf_tAlpha alpha, hf_tBeta beta)
  {
    mT0 = t0;
    mKcl = Kcl;
    mAlpha = alpha;
    mBeta = beta;
  }
  void SetT0(hf_tTime t0) { mT0 = t0; }
  void SetKcl(hf_tKcl Kcl) { mKcl = Kcl; }
  void SetAlpha(hf_tAlpha alpha) { mAlpha = alpha; }
  void SetBeta(hf_tBeta beta) { mBeta = beta; }

  // 対応するパス＆イベントの遅延パスが存在する(validな)時 True を返す。
  bool IsValid(void) const { return (bool)(mT0 != DBL_MAX); }

  // 各パラメータを取り出す．
  hf_tTime T0(void) const { return mT0; }
  hf_tKcl Kcl(void) const { return mKcl; }
  hf_tAlpha Alpha(void) const { return mAlpha; }
  hf_tBeta Beta(void) const { return mBeta; }

  // スルーが既知の時のオフセットを求める．
  hf_tTime T0(hf_tSlew slew) const
  {
    return mT0 + mAlpha * slew;
  }

  // スルーが既知の時の係数を求める．
  hf_tKcl Kcl(hf_tSlew slew) const
  {
    return mKcl + mBeta * slew;
  }

  // スルーを0として入力負荷効果から遅延時間を計算する．
  hf_tTime Delay(hf_tLoad load) const
  {
    return T0() + Kcl() * load;
  }

  // 入力負荷効果とスルーから遅延時間を計算する．
  hf_tTime Delay(hf_tLoad load, hf_tSlew slew) const
  {
    return T0(slew) + Kcl(slew) * load;
  }
};

//////////////////////
// セルを表すクラス //
//////////////////////
// セルの入力ピンを表すクラス
class hf_CCellIpin {
  friend class hf_CCell;
private:
  hf_CShStr mName;          // 入力ピン名
  hf_CLoad2 mIlu;           // 入力負荷(rise/fall)
  hf_CSlew2 mDefaultSlew;   // デフォルトの入力 slew
  
  // 内容をバイナリダンプする．
  friend const hf_CBinDump& operator<<(const hf_CBinDump& bindump,
				       const hf_CCellIpin& pin);

  // バイナリを読み込む．
  friend const hf_CBinDump& operator>>(const hf_CBinDump& bindump,
				       hf_CCellIpin& pin);
};

// セルの出力ピンを表すクラス
class hf_CCellOpin {
  friend class hf_CCell;
private:
  hf_CShStr mName;          // 出力ピン名
  hf_tLoad mOlu;            // 駆動能力
  hf_CSlewFunc2 mSlew;      // スルー関数(rise/fall)

  // 内容をバイナリダンプする．
  friend const hf_CBinDump& operator<<(const hf_CBinDump& bindump,
				       const hf_CCellOpin& pin);

  // バイナリを読み込む．
  friend const hf_CBinDump& operator>>(const hf_CBinDump& bindump,
				       hf_CCellOpin& pin);
};

// セル本体のクラス
class hf_CCell {
private:
  hf_CShStr mName;                 // セル名
  hf_tArea mArea;                  // 面積
  int mIntArea;                    // 整数化された面積
  hf_TArray<hf_CCellIpin> mIpins;  // 入力ピンの配列
  hf_TArray<hf_CCellOpin> mOpins;  // 出力ピンの配列
  hf_TArray<hf_CDelayPara> mDparas;// ディレイパラメータの配列(入力*出力*4)
  double mPowerType;		   // 出力駆動能力を表す番号。大きい方が強い。
  hf_CCellGroup* const mGroup;     // 所属するセルグループ

private:
  // コピーコンストラクタと代入演算子は、使用禁止。実体も定義しない。
  hf_CCell(const hf_CCell& src);
  const hf_CCell& operator=(const hf_CCell& src);

  // DummyCell() で使うデフォルトコンストラクタ。非公開。
  hf_CCell(void)
    : mGroup(NULL) {}

protected:
#if defined(DOCHK)
  // posが入力ピン数より小さいか調べる．
  // 検査に失敗したら例外 hf_CXcpt を投げる。
  void ChkNi(hf_tIdx pos, const char* msg) const
  {
    if (pos >= Ni()) {
      hf_mThrowError("libCell", "CL01", msg);
    }
  }

  // posが出力ピン数より小さいか調べる．
  // 検査に失敗したら例外 hf_CXcpt を投げる。
  void ChkNo(hf_tIdx pos, const char* msg) const
  {
    if (pos >= No()) {
      hf_mThrowError("libCell", "CL02", msg);
    }
  }
#else
#define ChkNi(x,y)
#define ChkNo(x,y)
#endif
public:
  // コンストラクタ．親のグループを指定する．
  hf_CCell(hf_CCellGroup* group);

public:
  // デストラクタ
  virtual ~hf_CCell(void);

  // ダミーセル (代表的な遅延情報を持つ 1入力 1出力セル)を作る。あとで 
  // SetIpin(), SetOpin(), SetDelayPara() で遅延情報を設定しなくてはな
  // らない。
  // mGroup は NULL なので、セルグループやセルマネージャにはアクセスできない。
  static hf_CCell* DummyCell(const hf_CString& dummyCellName);

  // 各メンバの設定
  // 名前の設定
  void SetName(const hf_CString& name);

  // 面積の設定
  void SetArea(hf_tArea area);

  // 出力駆動能力を設定する．
  void SetPowerType(double powerType);

  // 入力ピン情報(ピン名、入力負荷、デフォルトの入力 slew )の設定
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  void SetIpin(hf_tIdx pos, const hf_CString& name,
	       const hf_CLoad2& Ilu, const hf_CSlew2& defaultSlew);

  // 出力ピン情報(ピン名、出力負荷、スルーパラメータ)の設定
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  void SetOpin(hf_tIdx pos, const hf_CString& name, hf_tLoad Olu,
	       const hf_CSlew2& Ts0, const hf_CKscl2& Kscl);

  // 遅延情報パラメータの設定
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  void SetDelayPara(hf_tIdx Ipin, hf_tIdx Opin, hf_tEvent event,
		    hf_tTime t0, hf_tKcl Kcl, hf_tAlpha alpha, hf_tBeta beta);

  // セル情報の取得
  // 出力全てに論理が定義された組み合わせ回路セルなら True
  virtual bool IsMcell(void) const;

  // 1出力組み合わせ論理セルなら True
  virtual bool IsUcell(void) const;

  // 順序回路セルなら True
  virtual bool IsScell(void) const;

  // Z 状態を出力に持つセルなら True
  virtual bool IsZcell(void) const;

  // パス遅延とタイミング情報が無いソフトマクロセルなら True
  virtual bool IsXcell(void) const;

  // 出力駆動能力の取得
  double PowerType(void) const { return mPowerType; }

  // 名前の取得
  const hf_CShStr& Name(void) const { return mName; }

  // 面積の取得
  hf_tArea Area(void) const { return mArea; }

  // 入力数の取得
  size_t Ni(void) const
  {
    return mIpins.Size();
  }

  // 出力数の取得
  size_t No(void) const
  {
    return mOpins.Size();
  }

  // 属しているセル機能グループ hf_CCellGroupの取得
  hf_CCellGroup* CellGroup(void) const { return mGroup; }

  // 属しているセルマネージャ hf_CCellMgrの取得
  hf_CCellMgr* CellMgr(void) const
  {
    return CellGroup()->CellMgr();
  }

  // 入力ピン情報の取得
  // 入力ピン名の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  const hf_CShStr& IpinsName(hf_tIdx pos) const
  {
    ChkNi(pos, "IpinsName");
    return mIpins[pos].mName;
  }

  // 入力ピン名から入力ピンIDを得る．見つからない場合は -1 を返す.
  int IpinsId(const hf_CString& name) const;

  // 入力ピンの負荷効果の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  const hf_CLoad2& IpinsIlu(hf_tIdx pos) const
  {
    ChkNi(pos, "IpinsIlu");
    return mIpins[pos].mIlu;
  }

  // 入力ピンのデフォルトの入力 slew の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_CSlew2 IpinsDefaultSlew(hf_tIdx pos) const;

  // 出力ピン情報の取得
  // 出力ピン名の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  const hf_CShStr& OpinsName(hf_tIdx pos) const
  {
    ChkNo(pos, "OpinsName");
    return mOpins[pos].mName;
  }

  // 出力ピン名から入力ピンIDを得る．見つからない場合は -1 を返す.
  int OpinsId(const hf_CString& name) const;

  // 出力ピンの駆動能力の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_tLoad OpinsOlu(hf_tIdx pos) const
  {
    ChkNo(pos, "OpinsOlu");
    return mOpins[pos].mOlu;
  }

  // スルー関数の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  const hf_CSlewFunc2& OpinsSlewFunc(hf_tIdx pos) const
  {
    ChkNo(pos, "OplusSlewFunc");
    return mOpins[pos].mSlew;
  }

  // スルーのベース値(Ts0)の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_tSlew OpinsTs0(hf_tIdx pos, hf_tBv val) const
  {
    ChkNo(pos, "OpinsTs0");
    return mOpins[pos].mSlew[val].Ts0();
  }
  hf_CSlew2 OpinsTs0(hf_tIdx pos) const
  {
    ChkNo(pos, "OpinsTs0");
    return hf_CSlew2(mOpins[pos].mSlew[hf_kBv0].Ts0(),
		     mOpins[pos].mSlew[hf_kBv1].Ts0() );
  }

  // スルーの負荷依存係数(Kscl)の取得
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_tKscl OpinsKscl(hf_tIdx pos, hf_tBv val) const
  {
    ChkNo(pos, "OpinsKscl");
    return mOpins[pos].mSlew[val].Kscl();
  }
  hf_CKscl2 OpinsKscl(hf_tIdx pos) const
  {
    ChkNo(pos, "OpinsKscl");
    return hf_CKscl2(mOpins[pos].mSlew[hf_kBv0].Kscl(),
		     mOpins[pos].mSlew[hf_kBv1].Kscl() );
  }

  // 出力のスルーの計算
  // pos が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_tSlew EvalOpinsSlew(hf_tIdx pos, hf_tLoad load, hf_tBv val) const
  {
    ChkNo(pos, "EvalOpinsSlew");
    return mOpins[pos].mSlew[val].Eval(load);
  }
  hf_CSlew2 EvalOpinsSlew(hf_tIdx pos, const hf_CLoad2& load) const
  {
    ChkNo(pos, "EvalOpinsSlew");
    return mOpins[pos].mSlew.Eval(load);
  }

  // イベントを指定付きで、遅延パスの有無を返す。
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  bool ChkPath(hf_tIdx Ipin, hf_tIdx Opin, hf_tEvent which) const
  {
    return (bool)(DelayPara(Ipin, Opin, which) != 0);
  }

  // 遅延パスの有無を返す。イベント無指定。
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  bool ChkPath(hf_tIdx Ipin, hf_tIdx Opin) const
  {
    return (bool)(ChkPath(Ipin, Opin, hf_kEvent0to0)
		       || ChkPath(Ipin, Opin, hf_kEvent1to0)
		       || ChkPath(Ipin, Opin, hf_kEvent0to1)
		       || ChkPath(Ipin, Opin, hf_kEvent1to1));
  }

  // 遅延パラメータの取得
  // パスが定義されていない時は 0 ポインタを返す。
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  const hf_CDelayPara* DelayPara(hf_tIdx Ipin, hf_tIdx Opin,
				 hf_tEvent which) const
  {
    ChkNi(Ipin, "DelayPara");
    ChkNo(Opin, "DelayPara");
    const hf_CDelayPara* tmp;
    tmp = &mDparas[which + hf_kEventNum * (Ipin + Ni() * Opin)];
    if( tmp->IsValid() )	return tmp;
    else			return 0;
  }

  // 出力の遷移を指定して、入力の遷移に応じたディレイパラメータを ans に得る。
  // パスが定義されていない時は 0 を格納。
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  void DelayPara(hf_tIdx Ipin, hf_tIdx Opin, hf_tBv OpinsBv,
		 const hf_CDelayPara* ans[2]) const
  {
    ChkNi(Ipin, "DelayParas");
    ChkNo(Opin, "DelayParas");				  
    if (OpinsBv == hf_kBv0) {
      ans[hf_kBv0] = DelayPara(Ipin, Opin, hf_kEvent0to0);
      ans[hf_kBv1] = DelayPara(Ipin, Opin, hf_kEvent1to0);
    }
    else {
      ans[hf_kBv0] = DelayPara(Ipin, Opin, hf_kEvent0to1);
      ans[hf_kBv1] = DelayPara(Ipin, Opin, hf_kEvent1to1);
    }
  }

  // パス情報，遷移，入力のスルー値，出力の負荷を与えて遅延時間を得る．
  // 未定義の場合には 0.0 を返す．
  hf_tTime Delay(hf_tIdx Ipin, hf_tIdx Opin, hf_tEvent which,
		 hf_tLoad load, hf_tSlew slew = 0.0) const
  {
    const hf_CDelayPara* dpp = DelayPara(Ipin, Opin, which);
    if (dpp == NULL) {
      // 定義されていない．
      return 0.0;
    }
    return dpp->Delay(load, slew);
  }

  // 入力側の到達時刻、スルーから出力の到達時刻関数を計算する．
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_CAtimeFunc2 AtimeFunc(hf_tIdx Ipin, hf_tIdx Opin,
			   const hf_CTime2& iatime,
			   const hf_CSlew2& islew) const;

  // 上と同じ関数で、さらに出力の負荷が確定しているバージョン
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_CTime2 Atime(hf_tIdx Ipin, hf_tIdx Opin,
		  const hf_CTime2& iatime,
		  const hf_CSlew2& islew,
		  const hf_CLoad2& oload) const;

  // 出力側の要求時刻関数から入力の要求時刻関数を計算する．
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_CRtimeFunc2 RtimeFunc(hf_tIdx Ipin, hf_tIdx Opin,
			   const hf_CRtimeFunc2& ortime_func,
			   const hf_CLoad2& oload) const;

  // 上の関数で要求時刻が確定しているバージョン
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_CRtimeFunc2 RtimeFunc(hf_tIdx Ipin,hf_tIdx Opin,
			   const hf_CTime2& ortime,
			   const hf_CLoad2& oload) const;

  // 上の関数でさらに入力のスルーも確定しているバージョン
  // Ipin または Opin が範囲外の時は例外クラス hf_CXcpt を投げる。
  hf_CTime2 Rtime(hf_tIdx Ipin, hf_tIdx Opin,
		  const hf_CTime2& ortime,
		  const hf_CLoad2& oload,
		  const hf_CSlew2& islew) const;

  // デバッグ用の内容表示関数
  void Dump(FILE* fp) const;

  // 内容をバイナリダンプする。
  // グループ名、端子数はダンプしない。
  friend const hf_CBinDump& operator<<(const hf_CBinDump& bindump,
				       const hf_CCell& Cell);

  // バイナリを読み込む。
  // グループ名、端子数は読み込まない。
  friend const hf_CBinDump& operator>>(const hf_CBinDump& bindump,
				       hf_CCell& Cell);
};

// Event を順に処理するマクロ
//
// hf_tEvent ev;
// hf_mForEachEvent(ev) { //0to0, 0to1, 1to0, 1to1 の 4つのイベントを処理。
//   ....
// 
// というように用いる。  
#define hf_mForEachEvent(ev)\
  for ((ev) = (hf_tEvent)0; (ev) < (hf_tEvent)4; (ev) = (hf_tEvent)((ev)+1))

// hf_CCellを順々にとりだすマクロ
//
// hf_CCellGroup obj; または hf_CCellMgr obj;
// hf_CListLoc loc;
// hf_CCell* cell;
// hf_mForEachCell(obj, loc, cell) {
//   .....
// というふうに用いる．
#define hf_mForEachCell(obj, loc, cell)\
  for ((obj).SetCellLoc(loc); ((cell) = (obj).Cell(loc)) != 0;\
       (loc).MoveFore())


#endif
