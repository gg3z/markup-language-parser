#pragma once
#include "parse-util.h"

#include <vector>
#include <string>
#include <memory>
#include <deque>

using namespace std;

struct Node
{
    shared_ptr<Element> p; // plain text if no children, otherwise a label
    vector<shared_ptr<Node>> children;
};

struct Tree
{
    shared_ptr<Node> root;

    // traverses the tree breadth-first
    // and returns the strings in the leaf nodes
    // which are the plain text
    vector<string> BF_all_plain_text()
    {

        if( root == nullptr )
            return vector<string>{};

        vector<string> all_strings;
        deque<shared_ptr<Node>> dq;
        dq.emplace_back(root);
        while( !dq.empty() )
        {
            auto n = dq.size();
            for( auto k = 0; k < n; ++k )
            {
                auto cN = dq.front();
                if( cN != nullptr )
                {
                    if( cN->children.empty() && cN->p->s.front() != '<' )
                        all_strings.emplace_back(cN->p->s);
                    else
                    {
                        // don't add string since it's a label
                        // just add the children to the queue
                        for( const auto &node : cN->children )
                        {
                            dq.emplace_back(node);
                        }
                    }
                }
                dq.pop_front();
            }
        }
        return all_strings;
    }
};