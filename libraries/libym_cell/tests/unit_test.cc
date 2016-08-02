// $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/tests/unit_test.cc,v $
// $Author: matsunaga $
// $Revision: 1.1.1.1 $
// $Date: 2005/12/11 17:07:26 $

// Unit のテストプログラム

#include <iomanip>
#include "../Unit.h"

BEGIN_NAMESPACE_YM_CELL

void
sub(const Unit& unit, int value)
{
  cout << "Unit(" << setw(2) << unit.bias()
       << ", " << unit.ustr() << ")"
       << ", value = " << setw(8) << value
       << ":\t" << setw(20) << unit.value2str(value)
       << endl;
}

END_NAMESPACE_YM_CELL


int
main(int argc, char** argv)
{
  using namespace nsYm::nsCell;

  Unit unit1(0, "ns");
  sub(unit1, 0);
  sub(unit1, 12345);

  Unit unit2(2, "pf");
  sub(unit2, 0);
  sub(unit2, 12345);

  Unit unit3(5, "aa");
  sub(unit3, 0);
  sub(unit3, 12345);

  Unit unit4(8, "bb");
  sub(unit4, 0);
  sub(unit4, 12345);

  Unit unit5(-2, "cc");
  sub(unit5, 0);
  sub(unit5, 12345);
  
  return 0;
}
