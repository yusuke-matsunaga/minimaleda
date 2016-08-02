#include <iostream>

int
main(int argc, char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " # of instances ( in K-units)" << endl;
    return 1;
  }

  int n = atoi(argv[1]);
  if ( n == 0 ) {
    cerr << "illegal argument: " << argv[1]
	 << " : it must be a positive integer" << endl;
    return 1;
  }

  n *= 1000;

  cout << "module test2;" << endl;
  for (int i = 0; i < n; ++ i) {
    cout << "  assign a" << i << " = x" << i << " & y" << i
	 << " | z" << i << ";"
	 << endl;
  }
  cout << "endmodule" << endl;
  return 0;
}
