/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: lvlq.c,v $
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.2  1992/01/22  23:49:14  yusuke
 * no difference
 *
 * Revision 2.1  91/12/28  16:59:00  yusuke
 * Final , Final revision
 * 
 * Revision 2.0  91/12/21  18:52:57  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "network.h"


BEGIN_NAMESPACE_YM_MATPG


/* ADT lvlq */
static
int size = 0;
gate_t** lvlq_array = NULL;
int lvlq_max_lvl = -1;
int lvlq_min_lvl = -1;

void
lvlq_init(int max_level)
{
  int req_size = max_level + 1;
  if (req_size > size) {
    if (lvlq_array) {
      delete [] lvlq_array;
    }
    size = 1;
    do {
      size <<= 1;
    } while (size < req_size);
    lvlq_array = new gate_t*[size];
  }
}

void
lvlq_clr()
{
  for (int i = 0; i < size; ++ i) {
    lvlq_array[i] = NULL;
  }
  lvlq_max_lvl = -1;
  lvlq_min_lvl = -1;
}

gate_t*
lvlq_get_from_top()
{
  if (lvlq_max_lvl < 0 || lvlq_max_lvl < lvlq_min_lvl) {
    return NULL;
  }
  gate_t* gate;
  while ((gate = lvlq_array[lvlq_max_lvl]) == NULL) {
    if (-- lvlq_max_lvl < lvlq_min_lvl) {
      return NULL;
    }
  }
  lvlq_array[lvlq_max_lvl] = gate->next;
  gate->next = NULL;
  return gate;
}

gate_t*
lvlq_get_from_bottom()
{
  if (lvlq_min_lvl < 0 || lvlq_max_lvl < lvlq_min_lvl) {
    return NULL;
  }
  gate_t* gate;
  while ((gate = lvlq_array[lvlq_min_lvl]) == NULL) {
    if (lvlq_max_lvl < ++ lvlq_min_lvl) {
      return NULL;
    }
  }
  lvlq_array[lvlq_min_lvl] = gate->next;
  gate->next = NULL;
  return gate;
}

END_NAMESPACE_YM_MATPG
