//
//  Node.hpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/29/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Node
{
protected:
    Node* parent;
    std::vector<Node*> children;
    
public:
    Node();
    ~Node();
    std::vector<Node*> getChildren();
    virtual void draw(mat4) = 0;
    virtual void update() = 0;
    
    
};

#endif /* Node_hpp */

