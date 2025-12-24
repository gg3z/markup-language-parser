#include "parse-util.h"
#include "tree-util.hpp"

#include <cassert>

using namespace std;
static bool peek = false;

void test_bfs()
{
  cout << "begin test_bfs()\n";
  auto N0 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "0"})});
  Tree T;
  T.root = N0;
  auto vs0 = T.BF_all_plain_text();
  assert( !vs0.empty() && vs0.back() == string{"0"});

  auto N1 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "1"})});
  N0->children.emplace_back(N1);
  auto N2 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "2"})});
  N0->children.emplace_back(N2);
  auto N3 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "3"})});
  N0->children.emplace_back(N3);

  auto vs3 = T.BF_all_plain_text();
  string expected3("123");
  assert( [expected3](auto vs) {
    string concat;
    for( const auto& s : vs)
      concat.append(s);
      if( peek )
        cout << concat << endl;
    return( concat == expected3 );
  }(vs3));

  auto N4 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "4"})});
  N1->children.emplace_back(N4);
  auto N5 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "5"})});
  N1->children.emplace_back(N5);
  auto N6 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "6"})});
  N3->children.emplace_back(N6);

  auto vs4 = T.BF_all_plain_text();
  string expected4("2456");
  assert( [expected4](auto vs) {
    string concat;
    for( const auto& s : vs)
      concat.append(s);
      if( peek )
        cout << concat << endl;
    return( concat == expected4 );
  }(vs4));


  auto N7 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "7"})});
  N5->children.emplace_back(N7);
  auto N8 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "8"})});
  N5->children.emplace_back(N8);

  auto N9 = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "9"})});
  N6->children.emplace_back(N9);

  auto NA = make_shared<Node>(Node{make_shared<Element>(Element{0, 0, "A"})});
  N8->children.emplace_back(NA);

  auto vs5 = T.BF_all_plain_text();
  string expected5("2479A");
  assert( [expected5](auto vs) {
    string concat;
    for( const auto& s : vs)
      concat.append(s);
      if( peek )
        cout << concat << endl;
    return( concat == expected5 );
  }(vs5));
  cout << "end of test - success\n";
}

void test_chop()
{
  cout << "begin test_chop()\n";
  string s0("kkkk");
  auto vs0 = chop(s0, ' ');
  assert( vs0.size() == 1 && vs0.back() == s0 );

  string s1("k zkk");
  auto vs1 = chop(s1, ' ');
  assert( vs1.size() == 2 && vs1.front() == string("k") && vs1.back() == string("zkk") );

  string s2("  uhk ee");
  auto vs2 = chop(s2, ' ');
  assert( vs2.size() == 2 && vs2.front() == string("uhk") && vs2.back() == string("ee") );
  cout << "end of test - success\n";
}

void test_valid_element()
{
  cout << "begin test_valid_element()\n";
  assert( valid_piece(string("<>")) );
  assert( valid_piece(string("<>")) );
  assert( valid_piece(string("</>")) );
  assert( valid_piece(string("<x>")) );
  assert( valid_piece(string("</x>")) );
  assert( valid_piece(string("")) );
  assert( valid_piece(string("abc")) );
  assert( valid_piece(string("abc def")) );
  assert( !valid_piece(string("<")) );
  assert( !valid_piece(string(">")) );
  assert( !valid_piece(string("< x>")) );
  assert( !valid_piece(string("</x ")) );
  assert( !valid_piece(string("x/b")) );
  cout << "end of test - success\n";
}

void test_matching_labels()
{
  cout << "begin test_matching_labels()\n";
  assert(matching_labels(string("<doc>"), string("</doc>")));
  assert(matching_labels(string("<w>"), string("</w>")));
  assert(!matching_labels(string("<doc>"), string("</dot>")));
  cout << "end of test - success\n";
}