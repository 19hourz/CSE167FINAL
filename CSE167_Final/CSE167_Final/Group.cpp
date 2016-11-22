//
//  Group.cpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/29/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#include "Group.h"

using namespace std;

Group::Group()
{
}

Group::~Group()
{
    children.clear();
}

void Group::draw(mat4 C)
{
    for (Node* child : children)
    {
        child->draw(C);
    }
}

void Group::update()
{
    for (Node* child : children)
    {
        child->update();
    }
}

void Group::addChild(Node* n)
{
    children.push_back(n);
}

Node* Group::removeChild(int n)
{
    Node * node = children[n];
    children.erase(children.begin() + n);
    return node;
    
}
