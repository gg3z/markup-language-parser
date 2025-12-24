#include "parse-util.h"
#include "tree-util.hpp"

#include <stack>
#include <unordered_set>
#include <cassert>

using namespace std;

lineResult parseLine( int line_num, const string &line)
{
  lineResult LR;
  auto partition = partitionAtAngleBrackets(line);
  int start = 0;
  for( const auto end : partition )
  {
    auto p = line.substr(start, end - start);
    auto col = invalid_column(p);
    if( col >= 0 )
    {
      // cout << "invalid text: " << p << endl;
      LR.lf = LineFault::invalid_text;
      LR.col = start + col;
      return LR;
    }
    LR.pieces.emplace_back(make_shared<Element>(Element{line_num, start, p}));
    start = end;
  }

  auto p = line.substr(start);
  auto col = invalid_column(p);
  if( col >= 0 )
  {
    cout << "invalid text: " << p << endl;
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
  unordered_set<string> labels;
  vector<string> output;
  Tree T;
  T.root = nullptr;
  int line_num = 0, col = -1;

  for( string line; getline(fin, line); )
  {
    // cout << "parsing line " << line_num + 1 << endl;
    auto LR = parseLine(++line_num, line);
    if( LR.lf != LineFault::none )
    {
      // cout << "line fault " << (int)LR.lf << " at line " << line_num << endl;
      col = LR.col;
      break;
    }
    else if( line_num == 1 ) // check up front
    {
      if( LR.pieces.front()->s != string("<doc>") )
      {
        col = LR.pieces.front()->s.size();
        break;
      }
    }
    for( const auto &p : LR.pieces )
    {
      // cout << "element (" << p->s << ")\n";
      if( p->type() == StringType::endlabel )
      {
        // it should not be found in `labels`; insert it
        // corresponding label should be the current top of the stack and should be found in `labels`
        // remove label and endlabel from `labels`
        if( stack.top()->p->type() != StringType::label || !matching_labels(stack.top()->p->s, p->s) )
        {
          // the sequence of labels is invalid
          // cout << "invalid sequence: stack top type = " << (char)stack.top()->p->type() << ", strings: " << stack.top()->p->s << " : " << p->s << endl;
          col = p->col;
          break;
        }
        // cout << "stack size = " << stack.size() << ", top of stack = " << stack.top()->p->s << endl;
        if( stack.size() == 1 && stack.top()->p->s != string("<doc>") ) // this should be the last element
        {
          // cout << "shouldn't have this string on the stack: " << stack.top()->p->s << endl;
          col = p->col;
          break;
        }
        // if( stack.top()->children.empty() )
        // {
        //   // create a node with an empty string; not sure whether this is required output
        //   [[maybe_unused]] auto top = stack.top()->children.emplace_back(make_shared<Node>(Node{make_shared<Element>(Element{line_num, (int)stack.top()->p->s.size(), string("")})}));
        // }
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
          assert(p->s == string("<doc>"));
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
          [[maybe_unused]] auto top = stack.top()->children.emplace_back(make_shared<Node>(Node{p}));
      }
    }
    if( col >= 0 )
      break;
  }
  
  if( col < 0 )
  {
    return T.BF_all_plain_text();
  }
  auto error_msg = string("Error line ") + to_string(line_num) + " column " + to_string(col + 1);
  return vector{error_msg};
}