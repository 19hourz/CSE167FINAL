#include "Plane.h"
#include "Window.h"


using namespace std;
using namespace glm;

Cube *propeller1;
Cube *propeller2;

Plane::Plane()
{
    toWorld = mat4(1.0f);
    direction = vec3(0.7071f, 0.0f, 0.7071f);
    center = vec3(1.0f, 0.6f, 0.0f);
    propeller1 = new Cube(3);
    propeller2 = new Cube(3);
    
}

void Plane::draw(mat4 C)
{
    
}

void Plane::update()
{
    
}
