#include "parse-util.h"
#include "tree-util.hpp"

#include <stack>
#include <unordered_set>
#include <cassert>

using namespace std;

lineResult parseLine( int line_num, const string &line )
{
  lineResult LR;
  auto partition = partitionAtAngleBrackets(line);
  // cout << "line " << line_num << " partition: (";
  // for( auto ind : partition )
  // {
  //   cout << ind << ", ";
  // }
  // cout << ")\n";
  int start = first_non_whitespace(line);
  for( const auto end : partition )
  {
    auto p = line.substr(start, end - start);
    // cout << "p = (" << p << ")\n";
    auto col = invalid_column(p);
    if( col >= 0 )
    {
      // cout << "invalid text: " << p << endl;
      LR.lf = LineFault::invalid_text;
      LR.col = start + col;
      return LR;
    }
    if( !p.empty() )
      LR.pieces.emplace_back(make_shared<Element>(Element{line_num, start, p}));
    start = end;
  }

  auto p = line.substr(start);
  // cout << "p = (" << p << ")\n";
  auto col = invalid_column(p);
  if( col >= 0 )
  {
    // cout << "invalid text: " << p << endl;
    LR.lf = LineFault::invalid_text;
    LR.col = start + col;
    return LR;
  }
  LR.pieces.emplace_back(make_shared<Element>(Element{line_num, start, p}));
  return LR;
}

vector<string> parseFile(ifstream &fin)
{
  stack<shared_ptr<Node>> stack;
  unordered_set<string> labels; // this set is intended as a sanity check
  vector<string> output;
  Tree T;
  T.root = nullptr;
  int line_num = 0, col = -1;

  for( string line; getline(fin, line); )
  {
    ++line_num;
    if( line.empty() )
      continue;
    // cout << "parsing line " << line_num + 1 << endl;
    auto LR = parseLine(line_num, line);
    // cout << "line " << line_num << " parsed\n";
    if( line_num == 1 )
    {
      for( col = 0; col < 5; ++col )
      {
        if( line[first_non_whitespace(line) + col] != string("<doc>")[col] )
          break;
      }
      if( col < 5 )
      {
        col += first_non_whitespace(line);
        LR.lf = LineFault::invalid_text;
        // cout << "invalid text on line 1\n";
        break;
      }
      col = -1;
    }
    else if( LR.lf != LineFault::none )
    {
      // cout << "line fault " << (int)LR.lf << " at line " << line_num << endl;
      col = LR.col;
      break;
    }
    // cout << "LR.pieces().size() = " << LR.pieces.size() << endl;
    for( const auto &p : LR.pieces )
    {
      // cout << "processing piece: " << p->s << endl;
      if( p->type() == StringType::endlabel )
      {
        // it should not be found in `labels`; insert it
        auto [it, ins] = labels.insert(p->s);
        // cout << "label inserted = " << ins << endl;
        // corresponding label should be the current top of the stack and should be found in `labels`
        // remove label and endlabel from `labels`
        // if( stack.top()->p->type() != StringType::label || !matching_labels(stack.top()->p->s, p->s) )
        if( stack.top()->p->type() != StringType::label )
        {
          // the nesting of labels is invalid
          // cout << "invalid sequence: stack top type = " << (char)stack.top()->p->type() << ", strings: " << stack.top()->p->s << " : " << p->s << endl;
          col = p->col;
          break;
        }
        // cout << "top of stack = (" << stack.top()->p->s << "), current label = (" << p->s << ")\n";
        int max_match = matching_labels(stack.top()->p->s, p->s);
        // cout << "max_match = " << max_match << endl;
        if( max_match < stack.top()->p->s.size() - 2 ) // don't count the angle brackets
        {
          col = p->col + max_match + 2; // count "</" because p is an endlabel
          break;
        }
        // cout << "stack size = " << stack.size() << ", top of stack = " << stack.top()->p->s << endl;
        if( stack.size() == 1 && stack.top()->p->s != string("<doc>") ) // this should be the last element
        {
          // cout << "shouldn't have this string on the stack: " << stack.top()->p->s << endl;
          col = p->col;
          break;
        }
        stack.pop();
      }
      else if( p->type() == StringType::label )
      {
        // it should not be found in `labels`
        auto [it, ins] = labels.insert(p->s);
        // cout << "label inserted = " << ins << endl;
        if( T.root == nullptr )
        {
          assert(stack.empty());
          if( p->s != string("<doc>") ) // Error
          {
            for( col = 0; col < 5; ++col )
            {
              if( line[first_non_whitespace(line) + col] != string("<doc>")[col] )
                break;
            }
            if( col < 5 )
            {
              col += first_non_whitespace(line);
              LR.lf = LineFault::invalid_text;
              // cout << "invalid text on line 1\n";
              break;
            }
          }
          T.root = make_shared<Node>(Node{p});
          stack.push(T.root);
        }
        else
        {
          // insert as child of the node at the top of the stack
          auto newNode = make_shared<Node>(Node{p});
          stack.top()->children.emplace_back(newNode);
          stack.push(newNode);
        }
      }
      else if( !p->s.empty() ) // plain text - skip empty strings
      {
        // the owning label should be at the top of the stack and should be found in `labels`
          stack.top()->children.emplace_back(make_shared<Node>(Node{p}));
      }
    }
    if( col >= 0 )
      break;
  }
  // cout << "column = " << col << endl;
  if( col < 0 )
  {
    if( stack.empty() ) // stack should be empty
      return T.BF_all_plain_text();
    col = 0;
    ++line_num; // there should be at least one more line
  }
  auto error_msg = string("Error line ") + to_string(line_num) + " column " + to_string(col + 1);
  return vector<string>{error_msg};
}
