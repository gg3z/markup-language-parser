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

  // cout << fin_to_fout(fname) << endl;
  // test_chop();
  // test_bfs();
  // test_valid_element();
  // test_matching_labels();
  auto all_plain_text = parseFile(fin);

  string fout_name = fin_to_fout(fname);
  // cout << fout_name << endl;
  ofstream fout(fout_name.c_str());
  if(!fout)
    cout << "unable to write to selected file\n";
  for( const auto &s : all_plain_text )
    fout << s << endl;
  fout.close();
  
  fin.close();
  return 0;
}