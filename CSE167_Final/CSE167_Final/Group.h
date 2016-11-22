//
//  Group.hpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/29/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>

#include <vector>
#include "Node.h"


class Group :public Node
{
    
public:
    Group();
    ~Group();
    void draw(glm::mat4) override;
    void update() override;
    void addChild(Node*);
    Node* removeChild(int);

};


#endif /* Group_hpp */
