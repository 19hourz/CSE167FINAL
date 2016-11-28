//
//  MatrixTransform.cpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/29/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#include "MatrixTransform.h"
#include "Window.h"


using namespace std;

MatrixTransform::MatrixTransform(mat4 M)
{
    this->M = M;
}

MatrixTransform::~MatrixTransform()
{
}


void MatrixTransform::draw(mat4 C)
{
    for(Node* child : children){
        child->draw(C * M);
    }
}

void MatrixTransform::setM(mat4 m)
{
    M = m;
}

mat4 MatrixTransform::getM(){
    return M;
}

void MatrixTransform::update(){
    for (Node* child : children)
    {
        //std::cout << "update" << endl;
        child->update();
    }
}

