#ifndef H_MATPG_NETWORK
#define H_MATPG_NETWORK

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: network.h,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/26  19:57:30  yusuke
 * Final revision of version 2
 * 
 * Revision 2.0  91/12/21  18:55:03  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "matpg.h"
#include "gate.h"
#include <ym_tgnet/tgnet_nsdef.h>


BEGIN_NAMESPACE_YM_MATPG

extern gate_t** pos;
extern gate_t** gates;
extern gate_t** act_gates;

extern void gn_gen(const TgNetwork& src);
extern void gn_read_blif(FILE* ofp, FILE* efp);
extern void gn_erase();
extern void gn_dump(FILE* fp);

extern ymuint gn_get_npi();
extern ymuint gn_get_npo();
extern ymuint gn_get_ngate();

inline gate_t* gn_get_pi(int p) { return gates[p]; }
inline gate_t* gn_get_po(int p) { return pos[p]; }
inline gate_t* gn_get_gate(int p) { return gates[p]; }

extern ymuint gn_get_max_level();
extern void gn_activate_all();
extern bool gn_activate_1fault(gate_t* f_node);
extern void gn_deactivate();
extern void gn_clr_mark1();

const ymuint count_limit = 1 << ((sizeof(ymuint) * 8) - 1);

inline
void
gn_clr_mark()
{
  if (gn_common_count >= count_limit) {
    gn_clr_mark1();
  }
  gn_common_count ++;
}

extern gate_t** lvlq_array;
extern int lvlq_min_lvl;
extern int lvlq_max_lvl;
extern void lvlq_init(int max_level);
extern void lvlq_clr();
extern void lvlq_put(gate_t* gate);
extern gate_t* lvlq_get_from_top();
extern gate_t* lvlq_get_from_bottom();

inline
void
lvlq_put(gate_t* gate)
{
  int level = gate->lvl_i;
  gate->next = lvlq_array[level];
  lvlq_array[level] = gate;
  if (lvlq_min_lvl == -1 || lvlq_min_lvl > level) {
    lvlq_min_lvl = level;
  }
  if (lvlq_max_lvl < level) {
    lvlq_max_lvl = level;
  }
}

END_NAMESPACE_YM_MATPG

#endif
