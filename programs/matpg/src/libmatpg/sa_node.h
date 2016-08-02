#ifndef H_MATPG_SA_NODE
#define H_MATPG_SA_NODE

/*
 *	matpg - Mat's Automatic Testpattern Generator
 *	10/5/91 by Mat.
 *	$Log: sa_node.h,v $
 * Revision 2.3  1992/01/31  00:20:11  yusuke
 * Febrary '92 version
 *
 * Revision 2.2  1991/12/28  16:59:15  yusuke
 * Final , Final revision
 *
 * Revision 2.2  1991/12/28  16:59:15  yusuke
 * Final , Final revision
 *
 * Revision 2.1  91/12/23  23:12:10  yusuke
 * a slightly speed-up
 * 
 * Revision 2.0  91/12/21  18:55:56  yusuke
 * '91 Cristmas version
 * 
 * Revision 1.5  1991/10/05  08:18:18  yusuke
 * add Log and RCSid for RCS
 *
 */

#include "gate.h"

BEGIN_NAMESPACE_YM_MATPG

class sa_node :
  public gate_t
{
public:
  
  sa_node();
  
  virtual
  bool
  is_f_site() const;

  virtual
  void
  insert(gate_t* i_gate);

  virtual
  void
  insert(gate_t* o_gate,
	 int ipos);

  virtual
  void
  remove();

  virtual
  void
  fwd_imp_g(val3 val);

  virtual
  void
  fwd_imp_f(val3 val);

  virtual
  void
  bwd_imp_g(val3 val,
	    gate_t* from);

  virtual
  void
  bwd_imp_f(val3 val,
	    gate_t* from);
  
};

class sa0_node :
  public sa_node
{
public:
  
  virtual
  const char*
  get_name() const;
  
  virtual
  tTgGateType
  get_gtype() const;

  virtual
  bool
  is_sa0() const;

};

class sa1_node :
  public sa_node
{
public:

  virtual
  const char*
  get_name() const;
  
  virtual
  tTgGateType
  get_gtype() const;

  virtual
  bool
  is_sa1() const;
  
};

END_NAMESPACE_YM_MATPG

#endif
