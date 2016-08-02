#ifndef YM_VERILOG_PT_PTDUMPER_H
#define YM_VERILOG_PT_PTDUMPER_H

/// @file include/ym_verilog/pt/PtDumper.h
/// @brief PtDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtDumper.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_utils/FileLoc.h"
#include "ym_utils/FileRegion.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtDumper PtDumper.h <ym_veriog/pt/PtDumper.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief Pt クラスの内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class PtDumper
{
  friend class PtHeader;
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  PtDumper(ostream& s);

  /// @brief デストラクタ
  ~PtDumper();


public:

  /// @brief UDP とモジュールの内部情報の表示
  /// @param[in] udp_list UDP のリスト
  /// @param[in] module_list モジュールのリスト
  void
  put(const list<const PtUdp*>& udp_list,
      const list<const PtModule*>& module_list);


  /// @brief UDP を表示する
  /// @param[in] udp UDP
  void
  put(const PtUdp* udp);

  /// @brief UDP テーブルの値を表示する
  /// @param[in] label ラベル
  /// @param[in] v UDP テーブルの値
  void
  put(const char* label,
      const PtUdpValue* v);

  /// @brief module を表示する
  /// @param[in] m モジュール
  void
  put(const PtModule* m);

  /// @brief IO のヘッダを出力する．
  /// @param[in] label ラベル
  /// @param[in] io IOヘッダ
  void
  put(const char* label,
      const PtIOHead* io);

  /// @brief 宣言のヘッダを出力する．
  /// @param[in] label ラベル
  /// @param[in] decl 宣言ヘッダ
  void
  put(const char* label,
      const PtDeclHead* decl);

  /// @brief 要素の出力
  /// @param[in] label ラベル
  /// @param[in] item 要素
  void
  put(const char* label,
      const PtItem* item);

  /// @brief ステートメントの出力
  /// @param[in] label ラベル
  /// @param[in] stmt ステートメント
  void
  put(const char* label,
      const PtStmt* stmt);

  /// @brief 式の内容を出力する
  /// @param[in] label ラベル
  /// @param[in] expr 式
  void
  put(const char* label,
      const PtExpr* expr);


public:

  /// @brief ファイル位置情報を出力するモードににする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報の出力モードの取得
  /// @return ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;


private:

  /// @brief インクルード元のファイル情報の出力
  /// @param[in] file_loc ファイルの位置情報
  void
  put_parent_file(const FileLoc& file_loc);
  
  /// @brief 宣言を出力する．
  void
  put_decls(PtIOHeadArray iohead_array,
	    PtDeclHeadArray paramhead_array,
	    PtDeclHeadArray lparamhead_array,
	    PtDeclHeadArray declhead_array);
  
  /// @brief 宣言と要素を出力する．
  /// @param[in] label ラベル
  /// @param[in] decl_array 宣言の配列
  /// @param[in] item_array 要素の配列
  void
  put_decl_item(const char* label,
		PtDeclHeadArray decl_array,
		PtItemArray item_array);
  
  /// @brief 階層名の出力
  void
  put(PtNameBranchArray nb_array);
  
  /// @brief ブール型データの出力
  /// @param[in] label ラベル
  /// @param[in] b ブール値
  void
  put(const char* label,
      bool b);

  /// @brief 整数型データの出力
  /// @param[in] label ラベル
  /// @param[in] d 整数値
  void
  put(const char* label,
      int d);

  /// @brief 実数型データの出力
  /// @param[in] label ラベル
  /// @param[in] r 実数値
  void
  put(const char* label,
      double r);

  /// @brief 文字列型データの出力
  /// @param[in] label ラベル
  /// @param[in] str 文字列
  void
  put(const char* label,
      const char* str);

  /// @brief 文字列型データの出力
  /// @param[in] label ラベル
  /// @param[in] str 文字列
  void
  put(const char* label,
      const string& str);

  /// @brief 補助的なデータ型の出力
  /// @param[in] label ラベル
  /// @param[in] aux_type 補助的なデータ型
  void
  put(const char* label,
      tVpiAuxType aux_type);

  /// @brief ネット型の出力
  /// @param[in] label ラベル
  /// @param[in] net_type ネット型
  void
  put(const char* label,
      tVpiNetType net_type);

  /// @brief 変数型の出力
  /// @param[in] label ラベル
  /// @param[in] var_type 変数型
  void
  put(const char* label,
      tVpiVarType var_type);

  /// @brief 方向の出力
  /// @param[in] label ラベル
  /// @param[in] direction 方向
  void
  put(const char* label,
      tVpiDirection direction);

  /// @brief unconnected drive の出力
  /// @param[in] label ラベル
  /// @param[in] ud unconnected drive
  void
  put(const char* label,
      tVpiUnconnDrive ud);

  /// @brief default delay mode の出力
  /// @param[in] label ラベル
  /// @param[in] delay_mode default delay mode
  void
  put(const char* label,
      tVpiDefDelayMode delay_mode);

  /// @brief primitive型の出力
  /// @param[in] label ラベル
  /// @param[in] prim_type primitive 型
  void
  put(const char* label,
      tVpiPrimType prim_type);

  /// @brief 演算子型の出力
  /// @param[in] label ラベル
  /// @param[in] op_type 演算子型
  void
  put(const char* label,
      tVpiOpType op_type);

  /// @brief 定数型の出力
  /// @param[in] label ラベル
  /// @param[in] const_type 定数型
  void
  put(const char* label,
      tVpiConstType const_type);

  /// @brief strength の出力
  /// @param[in] label ラベル
  /// @param[in] strength strength
  void
  put(const char* label,
      tVpiStrength strength);

  /// @brief vectored/scalared 属性の出力
  /// @param[in] label ラベル
  /// @param[in] vs_type vectored/scalared 属性
  void
  put(const char* label,
      tVpiVsType vs_type);

  /// @brief 範囲指定モードの出力
  /// @param[in] label ラベル
  /// @param[in] range_mode
  void
  put(const char* label,
      tVpiRangeMode range_mode);

  /// @brief FileLoc の情報を出力する．
  /// @param[in] label ラベル
  /// @param[in] file_loc ファイルの位置情報
  void
  put(const char* label,
      const FileLoc& file_loc);

  /// @brief FileRegion の情報を出力する．
  /// @param[in] label ラベル
  /// @param[in] file_region ファイル領域の情報
  void
  put(const char* label,
      const FileRegion& file_loc);

  /// @brief 属性インスタンスを出力する．
  /// @param[in] label ラベル
  /// @param[in] attr_top 属性インスタンスの先頭
  void
  put(const char* label,
      const PtAttrInst* attr_top);

  /// @brief コントロールの内容を表示する．
  /// @param[in] label ラベル
  /// @param[in] ctrl コントロール
  void
  put(const char* label,
      const PtControl* ctrl);

  /// @brief strength の内容を表示する．
  /// @param[in] label ラベル
  /// @param[in] str 信号強度
  void
  put(const char* label,
      const PtStrength* str);

  /// @brief 遅延情報を出力する．
  /// @param[in] label ラベル
  /// @param[in] delay 遅延
  void
  put(const char* label,
      const PtDelay* delay);

  /// @brief 接続情報を表示する．
  /// @param[in] label ラベル
  /// @param[in] con 接続情報
  void
  put(const char* label,
      const PtConnection* con);

  /// @brief 階層ブランチの情報を出力する
  /// @param[in] label ラベル
  /// @param[in] nm 階層ブランチ
  void
  put(const char* label,
      const PtNameBranch* nb);

  /// @brief 開始マーカを出力する．
  /// @param[in] label マーカのラベル
  /// @param[in] type タイプ
  /// @param[in] need_cr true の時はマーカ出力後に改行する．
  void
  put_begin(const char* label,
	    const char* type,
	    bool need_cr = true);

  /// @brief 終了マーカを出力する．
  /// @param[in] label マーカのラベル
  void
  put_end(const char* label);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mStream;
  
  // マーカ出力後に改行したことを示すフラグ
  list<bool> mDoCR;
  
  // インデント量
  int mIndent;

  // ファイル位置情報の出力フラグ
  bool mFileLocMode;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_PT_PTDUMPER_H
