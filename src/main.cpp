#include "parse-util.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>

using namespace std;

int main(int argc, char* argv[])
{
  if( argc > 2 )
  {
    cout << "Too many arguments - only one argument is valid\n";
    return -1;
  }
  string fname;
  if (argc < 2 )
  {
    cout << "Please enter a file name:\n";
    cin >> fname;
  }
  else
  {
    fname = argv[1];
  }

  ifstream fin(fname);
  if( !fin )
  {
    cout << "Unable to open input file\n";
    return -1;
  }
  // test_chop();
  // test_bfs();
  // test_valid_element();
  // test_matching_labels();
#if 1
  auto all_plain_text = parseFile(fin);
  // cout << " output has " << all_plain_text.size() << " lines\n";
  for( const auto &s : all_plain_text )
    cout << s << endl;
#endif
  fin.close();
  return 0;
}