#include "Building.h"
#include "Window.h"


using namespace std;
using namespace glm;

Cube *block;
int numBlocks;

//Generate random number from 0 to 1
double random1(){
    return ((double) rand() / (RAND_MAX));
}

void Building::randBuilding(){
    //Clear blocksPos vector first
    blocksPos.clear();
    
    //Main building construction
    //Left bottom corner coordinate
//    double mainX = random1()/2.0 + 0.3; //Rand 0.1~0.6
//    double mainZ = random1()/2.0 + 0.3; //Rand 0.1~0.6
//    double mainWidth = ((random1()/2.0 + 0.5) * (2-mainX))/2.0; //Rand 0.5~1.0
//    double mainLength = ((random1()/2.0 + 0.5) * (2-mainZ))/2.0; //Rand 0.5~1.0
//    double mainHeight = random1()*2 + 2.0; // Rand 2~4
    double mainX = random1()/2.0 + 0.3; //Rand 0.1~0.6
    double mainZ = random1()/2.0 + 0.3; //Rand 0.1~0.6
    double mainWidth = ((random1()/2.0 + 0.5) * (2-mainX))*2; //Rand 0.5~1.0
    double mainLength = (random1()/2.0 + 0.5) * (2-mainZ)*2; //Rand 0.5~1.0
    double mainHeight = random1()*4 + 6.0; // Rand 2~4
    //printf("x:%f z:%f width:%f length:%f \n height:%f",x,z,widthFactor,lengthFactor,heightFactor);
    mat4 mainPos = scale(mat4(1.0f), vec3(mainWidth,mainHeight,mainLength));
    mainPos = translate(mat4(1.0f), vec3(mainX,0.0,mainZ)) * mainPos;
    blocksPos.push_back(mainPos);
    
    
    //Side building construction
    for(int i=0; i < numBlocks; i++){
        double x = random1(); //Rand 0~1
        double z = random1(); //Rand 0~1
        double width = mainWidth * (random1()/2.0 + 0.5); //Rand 0.5~1.0
        double length = mainLength * (random1()/2.0 + 0.5); //Rand 0.5~1.0
        double height = mainHeight * (random1()/2.0 + 0.5); // Rand 0.5~1
        //printf("x:%f z:%f width:%f length:%f \n height:%f",x,z,widthFactor,lengthFactor,heightFactor);
        mat4 pos = scale(mat4(1.0f), vec3(width,height,length));
        pos = translate(mat4(1.0f), vec3(x,0.0,z)) * pos;
        blocksPos.push_back(pos);
    }
}

Building::Building()
{
    block = new Cube(1);
    numBlocks = 4;
    toWorld = mat4(1.0f);
    randBuilding();
}

Building::~Building()
{
    
}

void Building::draw(mat4 C)
{
    if(Window::shouldRebuild){
        randBuilding();
    }
    for(int i = 0; i < blocksPos.size(); i++){
        mat4 newPos = C * blocksPos.at(i);
        block->draw(newPos);
    }
}

void Building::update()
{
    
}



