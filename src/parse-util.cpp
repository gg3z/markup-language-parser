#include "parse-util.h"
#include "tree-util.hpp"

#include <stack>
#include <unordered_set>
#include <cassert>

using namespace std;

#define DEBUG 0

static bool is_delimiter(char c, vector<char> &delim)
{
  for(auto d : delim)
    if( c == d )
      return true;
  return false;
}

// delimiter is not included in the output
vector<string> chop(const string &s, char delim)
{
  vector<string> pieces;
  int beg = 0, end = 0;
  char last = delim;

  for( const auto &c : s)
  {
    if( c == delim && last != delim ) // end of result substring
    {
      pieces.emplace_back(s.substr(beg, end - beg));
      cout << pieces.back() << ", ";
      beg = end;
    }
    else if( c != delim && last == delim ) // start of result substring
      beg = end;
    last = c;
    ++end;
  }
  if( end > beg )
  {
    pieces.emplace_back(s.substr(beg, end - beg));
    cout << pieces.back() << endl;
  }

  return pieces;
}

// vector<string> partitionAtAngleBrackets( const string &s )
vector<int> partitionAtAngleBrackets( const string &s )
{
  vector<int> partition;
  int col = 0;
  
#if DEBUG
  cout << "break at col ";
#endif

  for( const auto &c : s)
  {
    if( c == '<' )
    {
      if( (partition.empty() && col > 0) || (!partition.empty() && col > partition.back()) )
      {
          partition.emplace_back(col);
#if DEBUG
          cout << col << ", ";
#endif
      }
    }
    else if( c == '>')
    {
      if( col < s.size() - 1 )
      {
        partition.emplace_back(col + 1);
#if DEBUG
        cout << col + 1 << ", ";
#endif
      }
    }
    ++col;
  }
  
  vector<string> pieces;
  int ind = 0;
  for( const auto &col : partition )
  {
    auto p = s.substr(ind, col - ind);
    pieces.emplace_back(p);
    ind = col;
  }
  auto p = s.substr(ind);
  pieces.emplace_back(p);

#if DEBUG
  int num = 0;
  for( const auto &p : pieces )
  {
      cout << "piece " << num++ << " = (" << p << ")\n";
  }
#endif

  pieces.clear();
  // return pieces;
  return partition;
}

// must have one of the forms (x is any character not equal to '<', '/', or'>')
// <xxx>
// </xxx>
// xxxx
bool valid_piece(const string &piece)
{
  if( piece.empty() )
    return true;
  
  // check for invalid characters
  if( piece.front() == '<' && piece.back() == '>' )
  {
    auto itbeg = piece.cbegin() + 1;
    if( *itbeg == '/' )
      ++itbeg;
    for(auto it = itbeg; it != piece.cend(); ++it )
    {
      if ( *it == '/' || *it == ' ')
        return false;
    }
  }
  else
  {
    for(auto it = piece.cbegin(); it != piece.cend(); ++it )
    {
      if ( *it == '<' || *it == '/' || *it == '>' )
        return false;
    }
  }
  return true;
}

int invalid_column(const string &piece)
{
  if( piece.empty() )
    return -1;
  
  // check for invalid characters
  if( piece.front() == '<' && piece.back() == '>' )
  {
    auto itbeg = piece.cbegin() + 1;
    if( *itbeg == '/' )
      ++itbeg;
    for(auto it = itbeg; it != piece.cend(); ++it )
    {
      if ( *it == '/' || *it == ' ')
        return (int)(it - piece.cbegin());
    }
  }
  else
  {
    for(auto it = piece.cbegin(); it != piece.cend(); ++it )
    {
      if ( *it == '<' || *it == '/' || *it == '>' )
        return (int)(it - piece.cbegin());
    }
  }
  return -1;
}

bool matching_labels(const string &label, const string &endlabel)
{
    return label.substr(1, label.size() - 2) == endlabel.substr(2, endlabel.size() - 3);
}
