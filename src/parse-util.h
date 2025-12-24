#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>

using namespace std;

// types of invalid line - these don't include multi-line syntax errors
enum class LineFault { none = 0, mismatched_bracket, invalid_text, no_doc };

// string classification for parsing purposes
enum class StringType : char { label = 'l', endlabel = 'e', plain_text = 't' };

// the basic parsing building block that holds
// the string, its location and its type
struct Element
{
    int line;
    int col;
    string s;
    StringType type()
    {
        return s.starts_with('<') ? (s[1] == '/' ? StringType::endlabel : StringType::label) : StringType::plain_text;
    }
};

// stores the parsed pieces of a line and its status
struct lineResult
{
    LineFault lf;
    int col;
    vector<shared_ptr<Element>> pieces;
    lineResult() : lf(LineFault::none), col(-1) {}
    void print()
    {
        cout << "line fault = " << (int)lf << endl;
        for(const auto &p : pieces )
        {
            cout << "(line " << p->line << ", col " << p->col << ") : " << p->s << endl;
        }
    }
};

// basic string chopping - not used in this code
vector<string> chop(const string &s, char delim = ' ');

// specialized chopping that isolates angle bracket pairs
vector<int> partitionAtAngleBrackets( const string &s );

lineResult parseLine(int line_num, const string& line);

vector<string> parseFile(ifstream &fin);

bool valid_piece(const string &piece);
int invalid_column(const string &piece);

bool matching_labels(const string &label, const string &endlabel);

void test_chop();
void test_valid_element();
void test_bfs();
void test_matching_labels();
