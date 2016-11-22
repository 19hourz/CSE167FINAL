//
//  Node.cpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/30/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#include <stdio.h>
#include "Node.h"

Node::Node()
{
}


Node::~Node()
{
}

std::vector<Node*> Node::getChildren()
{
    return children;
}
