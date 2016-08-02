#ifndef H_MATPG_GATE
#define H_MATPG_GATE

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: gate.h,v $
 * Revision 2.7  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.6  1992/01/03  17:40:55  yusuke
 * delete event_s and event_w
 *
 * Revision 2.6  1992/01/03  17:40:55  yusuke
 * delete event_s and event_w
 *
 * Revision 2.5  91/12/29  19:02:38  yusuke
 * 
 * add EVENT_WEAK
 * 
 * Revision 2.4  91/12/28  16:58:49  yusuke
 * Final , Final revision
 * 
 * Revision 2.3  91/12/26  19:57:09  yusuke
 * Final revision of version 2
 * 
 * Revision 2.2  91/12/24  14:40:43  yusuke
 * 
 * a little bit modification
 * 
 * Revision 2.1  91/12/23  23:11:16  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:51:21  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.6  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "val.h"
#include <ym_tgnet/TgGateTemplate.h>


BEGIN_NAMESPACE_YM_MATPG

class gate_t;

struct gate_list_t
{
  gate_t* gate;
  val3 val;
  gate_list_t* next;
};

extern ymuint gn_common_count;

const ymuint J_SHIFT = 2;
const ymulong GVAL3_MASK = (1 << J_SHIFT) - 1;
const ymulong GJ_MASK = 1 << J_SHIFT;
const ymulong GSTATE_MASK = GVAL3_MASK | GJ_MASK;

const ymuint F_SHIFT = 3;
const ymulong FVAL3_MASK = GVAL3_MASK << F_SHIFT;
const ymulong FJ_MASK = GJ_MASK << F_SHIFT;
const ymulong FSTATE_MASK = FVAL3_MASK | FJ_MASK;

const ymulong J_MASK = GJ_MASK | FJ_MASK;

const ymuint P_SHIFT = 6;
const ymulong P_MASK = 1 << P_SHIFT;
const ymuint D_SHIFT = 7;
const ymulong D_MASK = 1 << D_SHIFT;

/*  <<  flag bits in gen_flag  >>  */
const ymulong GN_active	= 1 << 0;
const ymulong GN_PO		= 1 << 1;
const ymulong GN_END		= 1 << 2;
const ymulong GN_basis		= 1 << 3;
const ymulong GN_fcone		= 1 << 4;
const ymulong GN_ppath		= 1 << 5;
const ymulong GN_block		= 1 << 6;
const ymulong GN_used		= 1 << 7;
const ymulong GN_learn		= 1 << 8;
const ymulong GN_const		= 1 << 9;
const ymulong GN_cfna0		= 1 << 10;
const ymulong GN_cfna1		= 1 << 11;
const ymulong GN_scope		= 1 << 12;
const ymulong GN_event_p	= 1 << 13;
const ymulong GN_event_a	= GN_event_p;

const ymulong GN_term0		= GN_cfna0 | GN_const;
const ymulong GN_term1		= GN_cfna1 | GN_const;

struct net_t
{
  gate_t* gate;
  int pos;
};

class gate_t
{
private:
  const TgNode* node;
  ymulong gen_flag;
  ymuint gen_count;

  gate_t* fos;			/* fanout stem */

  void
  init(const TgNode* node,
       int ni);
  
  bool
  chk_generic(ymulong bitpat) const
  {
    return bool((gen_flag & bitpat) == bitpat);
  }
  
  bool
  chk_OR_generic(ymulong bitpat) const
  {
    return bool((gen_flag & bitpat) != 0L);
  }
  
  void
  set_generic(ymulong bitpat)
  {
    gen_flag |= bitpat;
  }
  
  void
  rst_generic(ymulong bitpat)
  {
    gen_flag &= ~bitpat;
  }
  

protected:
  
  int ni;
  
  gate_t** igate;
  
  int* sorted_pos;
  
  int no;
  
  net_t* outputs;
  
  int act_no;
  
  net_t* act_outputs;
  
public:
  ymuint id;
  
  int lvl_i;
  
  int min_lvl_i;
  
  gate_t* next;			/* generic link pointer */
  
  /* for Conflict Free Node Assignment */
  code2val cfna_pat[2];
  
  /* for learning */
  gate_list_t* imp0;
  gate_list_t* imp1;
  gate_t* eq_gate;
  
  /* for DTPG */
  ymulong state;
  
  /* for unique sensitization */
  ymulong side_val;
  
  /* for Fault Simulator */
  gate_t* next_eval;
  pckval g_pckval;
  pckval f_pckval;

  gate_t* ggate;


public:

  /// @brief コンストラクタ
  /// @param[in] node もとのノード
  /// @param[in] ni 入力数
  gate_t(const TgNode* node,
	 int ni);

  /// @brief デストラクタ
  virtual
  ~gate_t();


public:
  //////////////////////////////////////////////////////////////////////
  // 入出力の接続情報
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  int
  get_ni() const;

  /// @brief ファンアウト数を得る．
  int
  get_no() const;

  /// @brief アクティブなファンアウト数を得る．
  int
  get_act_no() const;

  /// @brief アクティブなファンアウト数を設定する．
  void
  set_act_no(int no);

  /// @brief ファンインを取り出す．
  gate_t*
  get_figate(int pos) const;

  /// @brief ファンインを設定する．
  void
  set_figate(int pos,
	     gate_t* input);

  /// @brief ソート順にファンインを取り出す．
  gate_t*
  get_sorted_figate(int pos) const;

  /// @brief ソート順を取り出す．
  int
  get_sorted_pos(int pos) const;

  /// @brief ソート順を設定する．
  void
  set_sorted_pos(int pos,
		 int s_pos);

  /// @brief 通常のファンインの設定を行う．
  void
  init_figate(int pos,
	      gate_t* input);

  /// @brief ファンアウトのゲートを得る．
  gate_t*
  get_fogate(int pos) const;

  /// @brief ファンアウトの入力位置を得る．
  int
  get_fogate_ipos(int pos) const;

  /// @brief ファンアウトを設定する．
  void
  set_fogate(int pos,
	     gate_t* output,
	     int ipos);

  /// @brief 通常のファンアウトの設定を行う．
  void
  init_fogate(gate_t* output,
	      int pos);

  /// @brief アクティブなファンアウトのゲートを得る．
  gate_t*
  get_act_fogate(int pos) const;

  /// @brief アクティブなファンアウトの入力位置を得る．
  int
  get_act_fogate_ipos(int pos) const;

  /// @brief アクティブなファンアウトを設定する．
  void
  set_act_fogate(int pos,
		 gate_t* output,
		 int ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // その他の属性
  //////////////////////////////////////////////////////////////////////

  const TgNode*
  get_node() const { return node; }
  
  gate_t*
  get_fos() const { return fos; }
  
  void
  set_fos(gate_t* gate) { fos = gate; }
  
  bool
  is_fos() const { return bool(this == fos); }
  
  virtual
  bool
  is_pi() const;
  
  virtual
  int get_pi_id() const;
  
  /* Values/Flags */
  val3
  get_gval() const { return val3(state & GVAL3_MASK); }
  
  void
  set_gval(val3 val)
  {
    state = (state & ~(GSTATE_MASK)) | val;
  }
  
  void
  set_gjval(val3 val)
  {
    state = (state & ~(GVAL3_MASK)) | val | GJ_MASK;
  }
  
  bool
  chk_gj() const { return bool((state & GJ_MASK) != 0); }
  
  void
  set_gj() { state |= GJ_MASK; }
  
  void
  rst_gj() { state &= ~GJ_MASK; }
  
  val3
  get_fval() const { return val3((state & FVAL3_MASK) >> F_SHIFT); }
  
  void
  set_fval(val3 val)
  {
    state = (state & ~(FSTATE_MASK)) | (val << F_SHIFT);
  }
  
  void
  set_fjval(val3 val)
  {
    state = (state & ~(FVAL3_MASK)) | (val << F_SHIFT) | FJ_MASK;
  }
  
  bool
  chk_fj() const { return bool((state & FJ_MASK) != 0); }
  
  void
  set_fj() { state |= FJ_MASK; }
  
  void
  rst_fj() { state &= ~FJ_MASK; }
  
  void
  set_hval(val3 val)
  {
    state = (state & ~(GSTATE_MASK | FSTATE_MASK)) | val | (val << F_SHIFT);
  }
  void
  set_hjval(val3 val)
  {
    state = (state & ~(GVAL3_MASK | FVAL3_MASK)) | val | (val << F_SHIFT) | GJ_MASK | FJ_MASK;
  }
  
  bool
  chk_pg() const { return bool((state & P_MASK) != 0); }
  
  void
  set_pg() { state |= P_MASK; }
  
  void
  rst_pg() { state &= ~P_MASK; }
  
  bool
  chk_dif() const { return bool((state & D_MASK) != 0); }
  
  void
  set_dif() { state |= D_MASK; }
  
  void
  rst_dif() { state &= ~D_MASK; }
  
  void
  set_pg_dif() { state |= (P_MASK | D_MASK); }
  
  void
  rst_pg_dif() { state &= ~ (P_MASK | D_MASK); }
  
  bool
  chk_mark() const { return bool(gen_count == gn_common_count); }
  
  void
  set_mark() { gen_count = gn_common_count; }
  
  void
  rst_mark() { gen_count = 0; }
  
  void
  clr_flag() { gen_flag = 0; }
  
  ymuint
  get_flag() const { return gen_flag; }
  
  void
  set_flag(const ymuint val) { gen_flag = val; }
  
  bool
  is_active() const { return chk_generic(GN_active); }
  
  void
  set_active() { set_generic(GN_active); }
  
  void
  rst_active() { rst_generic(GN_active); }
  
  bool
  is_po() const { return chk_generic(GN_PO); }
  
  void
  set_po() { set_generic(GN_PO); }
  
  void
  rst_po() { rst_generic(GN_PO); }
  
  bool
  chk_POJ() const
  {
    return bool(is_po() == true && ((state & J_MASK) == J_MASK));
  }

#if 0
  bool
  is_end() const { return chk_generic(GN_END); }
  
  void
  set_end() { set_generic(GN_END); }
  
  void
  rst_end() { rst_generic(GN_END); }
#endif

  bool
  is_basis() const { return chk_generic(GN_basis); }
  
  void
  set_basis() { set_generic(GN_basis); }
  
  void
  rst_basis() { rst_generic(GN_basis); }
  
  bool
  chk_fcone() const { return chk_generic(GN_fcone); }
  
  void
  set_fcone() { set_generic(GN_fcone); }
  
  void
  rst_fcone() { rst_generic(GN_fcone); }
  
  bool
  chk_ppath() const { return chk_generic(GN_ppath); }
  
  void
  set_ppath() { set_generic(GN_ppath); }
  
  void
  rst_ppath() { rst_generic(GN_ppath); }
  
  bool
  chk_block() const { return chk_generic(GN_block); }
  
  void
  set_block() { set_generic(GN_block); }
  
  void
  rst_block() { rst_generic(GN_block); }
  
  bool
  chk_used() const { return chk_generic(GN_used); }
  
  void
  set_used() { set_generic(GN_used); }
  
  void
  rst_used() { rst_generic(GN_used); }
  
  bool
  chk_learned() const { return chk_generic(GN_learn); }
  
  void
  set_learned() { set_generic(GN_learn); }
  
  void
  rst_learned() { rst_generic(GN_learn); }
  
  bool
  chk_const() const { return chk_generic(GN_const); }
  
  void
  set_const() { set_generic(GN_const); }
  
  void
  rst_const() { rst_generic(GN_const); }
  
  bool
  chk_cfna0() const { return chk_generic(GN_cfna0); }
  
  void
  set_cfna0() { set_generic(GN_cfna0); }
  
  void
  rst_cfna0() { rst_generic(GN_cfna0); }
  
  bool
  chk_cfna1() const { return chk_generic(GN_cfna1); }
  
  void
  set_cfna1() { set_generic(GN_cfna1); }
  
  void
  rst_cfna1() { rst_generic(GN_cfna1); }
  
  bool
  chk_scope() const { return chk_generic(GN_scope); }
  
  void
  set_scope() { set_generic(GN_scope); }
  
  void
  rst_scope() { rst_generic(GN_scope); }
  
  ymulong
  get_EVENT() const { return gen_flag & GN_event_a; }
  
  void
  set_EVENT(const ymulong eventtype) { gen_flag |= eventtype; }
  
  void
  rst_EVENT() { rst_generic(GN_event_a); }
  
  bool
  chk_term0() const
  {
    return bool(chk_OR_generic(GN_term0) == true &&
		chk_generic(GN_fcone) == false);
  }
  bool
  chk_term1() const
  {
    return bool(chk_OR_generic(GN_term1) == true &&
		chk_generic(GN_fcone) == false);
  }
  
  /* set fanin's fault */
  virtual
  void
  set_fi_fault();
  
  /* for CFNA */
  virtual
  bool
  just_cfna(val3 val);
  
  /* calculate testability measure */
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(int pos);
  
  /* for fault_simulation */
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  /* for learning */
  virtual
  void
  l_fwd_imp(val3 val);
  
  virtual
  void
  l_bwd_imp(val3 val, gate_t* from);
  
  /* for DTPG */
  virtual
  val3
  get_c_val() const;
  
  virtual
  val3
  get_nc_val() const;
  
  virtual
  val3
  get_o_val() const;
  
  virtual
  val3
  get_no_val() const;
  
  virtual
  bool
  calc_obs2(int ipos) const;
  
  virtual
  void
  backtrace_g(val3 val);
  
  virtual
  void
  backtrace_f(val3 val);
  
  virtual
  void
  chk_just();
  
  void
  prop_h(val3 val);
  
  void
  prop_g(val3 val);
  
  void
  prop_f(val3 val);
  
  void
  refl_h(val3 val,
	 gate_t* from);
  
  void
  refl_g(val3 val,
	 gate_t* from);
  
  void
  refl_f(val3 val,
	 gate_t* from);
  
  virtual
  void
  fwd_imp_h(val3 val);
  
  virtual
  void
  fwd_imp_g(val3 val);
  
  virtual
  void
  fwd_imp_f(val3 val);
  
  virtual
  void
  bwd_imp_h(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_g(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_f(val3 val,
	    gate_t* from);
  
  void
  gl_imp(val3 val);
  
  void
  eq_imp(val3 val);
  
  virtual
  bool
  is_f_site() const;
  
  virtual
  bool
  is_sa0() const;
  
  virtual
  bool
  is_sa1() const;
  
  virtual
  const char*
  get_name() const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void dump(FILE* fp) const;

};

class C0_gate_t :
  public gate_t
{
public:
  
  C0_gate_t(const TgNode* node) : gate_t(node, 0) { }

  virtual
  bool
  just_cfna(val3 val);
  
  virtual
  void
  calc_C();
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
  virtual
  void
  l_bwd_imp(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_h(val3 val,
	    gate_t* from);
  
};

class C1_gate_t :
  public gate_t
{
public:
  
  C1_gate_t(const TgNode* node) : gate_t(node, 0) { }
  
  virtual
  bool
  just_cfna(val3 val);
  
  virtual
  void
  calc_C();
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
  virtual
  void
  l_bwd_imp(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_h(val3 val,
	    gate_t* from);
  
};

class BUF_gate_t :
  public gate_t
{
public:
  
  BUF_gate_t(const TgNode* node) : gate_t(node, 1) { }
  
  virtual
  bool
  just_cfna(val3 val);
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(int pos);
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
  virtual
  bool
  calc_obs2(int ipos) const;
  
  virtual
  void
  l_fwd_imp(val3 val);
  
  virtual
  void
  l_bwd_imp(val3 val,
	    gate_t* from);
  
  virtual
  void
  backtrace_g(val3 val);
  
  virtual
  void
  backtrace_f(val3 val);
  
  virtual
  void
  chk_just();
  
  virtual
  void
  fwd_imp_h(val3 val);
  
  virtual
  void
  fwd_imp_g(val3 val);
  
  virtual
  void
  fwd_imp_f(val3 val);
  
  virtual
  void
  bwd_imp_h(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_g(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_f(val3 val,
	    gate_t* from);
  
};

class NOT_gate_t :
  public BUF_gate_t
{
public:
  
  NOT_gate_t(const TgNode* node) : BUF_gate_t(node) { }
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(int pos);
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
  virtual
  void
  backtrace_g(val3 val);
  
  virtual
  void
  backtrace_f(val3 val);
  
};

class SIMPLE_gate_t :
  public gate_t
{
protected:
  
  val3 c_val;
  val3 o_val;
  
public:
  
  SIMPLE_gate_t(const TgNode* node, int ni, const val3 v1, const val3 v2)
    : gate_t(node, ni) { c_val = v1; o_val = v2; }
  
  virtual
  void
  set_fi_fault();
  
  virtual
  bool
  just_cfna(val3 val);
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(int pos);
  
  virtual
  void
  l_fwd_imp(val3 val);
  
  virtual
  void
  l_bwd_imp(val3 val,
	    gate_t* from);
  
  virtual
  val3
  get_c_val() const;
  
  virtual
  val3
  get_nc_val() const;
  
  virtual
  val3
  get_o_val() const;
  
  virtual
  val3
  get_no_val() const;
  
  virtual
  void
  backtrace_g(val3 val);
  
  virtual
  void
  backtrace_f(val3 val);
  
  virtual
  void
  fwd_imp_h(val3 val);
  
  virtual
  void
  fwd_imp_g(val3 val);
  
  virtual
  void
  fwd_imp_f(val3 val);
  
  virtual
  void
  bwd_imp_h(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_g(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_f(val3 val,
	    gate_t* from);
  
};

class AND_gate_t :
  public SIMPLE_gate_t
{
public:
  
  AND_gate_t(const TgNode* node, int ni)
    : SIMPLE_gate_t(node, ni, val_0, val_0) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  bool
  calc_obs2(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
};

class AND_2_gate_t :
  public AND_gate_t
{
public:
  
  AND_2_gate_t(const TgNode* node) : AND_gate_t(node, 2) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class AND_3_gate_t :
  public AND_gate_t
{
public:
  
  AND_3_gate_t(const TgNode* node) : AND_gate_t(node, 3) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class AND_4_gate_t :
  public AND_gate_t
{
public:
  
  AND_4_gate_t(const TgNode* node) : AND_gate_t(node, 4) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class NAND_gate_t :
  public SIMPLE_gate_t
{
public:
  
  NAND_gate_t(const TgNode* node, int ni)
    : SIMPLE_gate_t(node, ni, val_0, val_1) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  bool
  calc_obs2(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
};

class NAND_2_gate_t :
  public NAND_gate_t {
public:
  
  NAND_2_gate_t(const TgNode* node) : NAND_gate_t(node, 2) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class NAND_3_gate_t :
  public NAND_gate_t
{
public:
  
  NAND_3_gate_t(const TgNode* node) : NAND_gate_t(node, 3) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class NAND_4_gate_t :
  public NAND_gate_t
{
public:
  
  NAND_4_gate_t(const TgNode* node) : NAND_gate_t(node, 4) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class PI_gate_t :
  public NAND_gate_t
{
public:
  
  PI_gate_t(const TgNode* node,
	    int PI_id) :
    NAND_gate_t(node, 0)
  {
    pi_id = PI_id;
  }
  
  virtual
  bool
  is_pi() const;
  
  virtual
  int
  get_pi_id() const;
  
  virtual
  void
  calc_C();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  void
  dump(FILE* fp) const;

private:

  int pi_id;
  
};

class OR_gate_t :
  public SIMPLE_gate_t
{
public:
  
  OR_gate_t(const TgNode* node, int ni)
    : SIMPLE_gate_t(node, ni, val_1, val_1) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  bool
  calc_obs2(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
};

class OR_2_gate_t :
  public OR_gate_t
{
public:
  
  OR_2_gate_t(const TgNode* node) : OR_gate_t(node, 2) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class OR_3_gate_t :
  public OR_gate_t {
public:
  
  OR_3_gate_t(const TgNode* node) : OR_gate_t(node, 3) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class OR_4_gate_t :
  public OR_gate_t
{
public:
  
  OR_4_gate_t(const TgNode* node) : OR_gate_t(node, 4) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class NOR_gate_t :
  public SIMPLE_gate_t
{
public:
  
  NOR_gate_t(const TgNode* node, int ni)
    : SIMPLE_gate_t(node, ni, val_1, val_0) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  bool
  calc_obs2(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
};

class NOR_2_gate_t :
  public NOR_gate_t
{
public:
  
  NOR_2_gate_t(const TgNode* node) : NOR_gate_t(node, 2) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class NOR_3_gate_t :
  public NOR_gate_t
{
public:
  
  NOR_3_gate_t(const TgNode* node) : NOR_gate_t(node, 3) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class NOR_4_gate_t :
  public NOR_gate_t
{
public:
  
  NOR_4_gate_t(const TgNode* node) : NOR_gate_t(node, 4) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class XOR_gate_t :
  public gate_t
{
public:
  
  XOR_gate_t(const TgNode* node, int ni) : gate_t(node, ni) { }
  
  virtual
  void
  set_fi_fault();
  
  virtual
  bool
  just_cfna(val3 val);
  
  virtual
  void
  calc_C();
  
  virtual
  ymuint
  calc_O(int pos);
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  bool
  calc_obs2(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
  virtual
  void
  l_fwd_imp(val3 val);
  
  virtual
  void
  l_bwd_imp(val3 val,
	    gate_t* from);
  
  virtual
  void
  backtrace_g(val3 val);
  
  virtual
  void
  backtrace_f(val3 val);
  
  virtual
  void
  fwd_imp_h(val3 val);
  
  virtual
  void
  fwd_imp_g(val3 val);
  
  virtual
  void
  fwd_imp_f(val3 val);
  
  virtual
  void
  bwd_imp_h(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_g(val3 val,
	    gate_t* from);
  
  virtual
  void
  bwd_imp_f(val3 val,
	    gate_t* from);
  
};

class XOR_2_gate_t :
  public XOR_gate_t
{
public:
  
  XOR_2_gate_t(const TgNode* node) : XOR_gate_t(node, 2) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

class XNOR_gate_t :
  public XOR_gate_t
{
public:
  
  XNOR_gate_t(const TgNode* node, int ni) : XOR_gate_t(node, ni) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
  virtual
  pckval
  calc_obs(int ipos) const;
  
  virtual
  tTgGateType
  get_gtype() const;
  
  virtual
  void
  dump(FILE* fp) const;
  
};

class XNOR_2_gate_t :
  public XNOR_gate_t
{
public:
  
  XNOR_2_gate_t(const TgNode* node) : XNOR_gate_t(node, 2) { }
  
  virtual
  void
  calc_gval();
  
  virtual
  pckval
  calc_fval() const;
  
};

extern
gate_t*
new_gate_t(tTgGateType id,
	   const TgNode* node,
	   int ni);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
int
gate_t::get_ni() const
{
  return ni;
}

// @brief ファンアウト数を得る．
inline
int
gate_t::get_no() const
{
  return no;
}

// @brief アクティブなファンアウト数を得る．
inline
int
gate_t::get_act_no() const
{
  return act_no;
}

// @brief アクティブなファンアウト数を設定する．
inline
void
gate_t::set_act_no(int no)
{
  act_no = no;
}

// @brief ファンインを取り出す．
inline
gate_t*
gate_t::get_figate(int pos) const
{
  return igate[pos];
}

// @brief ファンインを設定する．
inline
void
gate_t::set_figate(int pos,
		   gate_t* input)
{
  igate[pos] = input;
}

// @brief ソート順にファンインを取り出す．
inline
gate_t*
gate_t::get_sorted_figate(int pos) const
{
  return igate[sorted_pos[pos]];
}

// @brief ソート順を取り出す．
inline
int
gate_t::get_sorted_pos(int pos) const
{
  return sorted_pos[pos];
}

// @brief ソート順を設定する．
inline
void
gate_t::set_sorted_pos(int pos,
		       int s_pos)
{
  sorted_pos[pos] = s_pos;
}

// @brief ファンアウトのゲートを得る．
inline
gate_t*
gate_t::get_fogate(int pos) const
{
  return outputs[pos].gate;
}

// @brief ファンアウトの入力位置を得る．
inline
int
gate_t::get_fogate_ipos(int pos) const
{
  return outputs[pos].pos;
}

// @brief ファンアウトを設定する．
inline
void
gate_t::set_fogate(int pos,
		   gate_t* output,
		   int ipos)
{
  outputs[pos].gate = output;
  outputs[pos].pos = ipos;
}

// @brief アクティブなファンアウトのゲートを得る．
inline
gate_t*
gate_t::get_act_fogate(int pos) const
{
  return act_outputs[pos].gate;
}

// @brief アクティブなファンアウトの入力位置を得る．
inline
int
gate_t::get_act_fogate_ipos(int pos) const
{
  return act_outputs[pos].pos;
}

// @brief アクティブなファンアウトを設定する．
inline
void
gate_t::set_act_fogate(int pos,
		       gate_t* output,
		       int ipos)
{
  act_outputs[pos].gate = output;
  act_outputs[pos].pos = ipos;
}

END_NAMESPACE_YM_MATPG

#endif
