#include "Building.h"
#include "Window.h"


using namespace std;
using namespace glm;

Cube *cube1;

Building::Building()
{
    cube1 = new Cube(1);
    srand (1);
    toWorld = mat4(1.0f);
    double posX = rand()%100/100.0;
    double negX = -rand()%100/100.0;
    double posZ = rand()%100/100.0;
    double negZ = -rand()%100/100.0;
    double posY = rand()%100/30.0;
    toWorld = scale(mat4(1.0f), vec3(posX-negX,posY,posZ-negZ)) * toWorld;
    
}

Building::~Building()
{
    
}

void Building::draw(mat4 C)
{
    mat4 newPos = C * toWorld;
    cube1->draw(newPos);
}

void Building::update()
{
    
}



