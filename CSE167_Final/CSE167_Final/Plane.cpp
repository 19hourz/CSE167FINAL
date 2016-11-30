#include "Plane.h"
#include "Window.h"


using namespace std;
using namespace glm;

Cube *propeller1;
Cube *propeller2;
mat4 propeller1Setup;
mat4 propeller2Setup;
vec3 oriDirec = vec3(0.0f, 0.0f, -1.0f);
Plane::Plane()
{
    deg = 0.0;
    toWorld = mat4(1.0f);
    direction = vec3(0.7071f, 0.0f, 0.7071f);
    upDirection = vec3(0.0f, 1.0f, 0.0f);
    center = vec3(0.0f, 0.5f, 0.0f);
    propeller1 = new Cube(10);
    propeller2 = new Cube(10);
    direction = normalize(direction);
}

void Plane::draw(mat4 C)
{
    mat4 transform = translate(mat4(1.0f), vec3(0.0f, -0.05f, 0.0f)) * translate(mat4(1.0f), vec3(0.01f, 0.05f, 0.005f)) * rotate(mat4(1.0f), deg, oriDirec) * scale(mat4(1.0f), vec3(0.02,0.1,0.01)) * translate(mat4(1.0f), vec3(-0.5f, -0.5f, -0.5f));
    vec3 rotAxis = cross(oriDirec, direction);
    GLfloat rotAngle = dot(oriDirec, direction);
    rotAngle = acos(rotAngle);
    transform =  translate(mat4(1.0f), center) * rotate(mat4(1.0f), rotAngle, rotAxis) * transform;
    propeller1->draw(transform);
}

void Plane::update()
{
    if (deg < 360) {
        deg = deg+0.5*propellerSpeed;
    }
    else{
        deg = 0.0;
    }
    center += airSpeed * direction;
}

GLfloat Plane::speedUp()
{
    airSpeed += 0.01;
    if (airSpeed>=0.2) {
        airSpeed = 0.2;
    }
    return airSpeed;
}

GLfloat Plane::speedDown()
{
    airSpeed -= 0.1;
    if (airSpeed<=0.0) {
        airSpeed = 0.0;
    }
    return airSpeed;
}

void Plane::up()
{
    GLfloat updeg = 0.01;
    vec3 right = cross(direction, upDirection);
    vec4 tempup = vec4(upDirection,0.0);
    vec4 tempdirec = vec4(direction, 0.0);
    tempup = rotate(mat4(1.0f), updeg, right) * tempup;
    tempdirec = rotate(mat4(1.0f), updeg, right) * tempdirec;
    direction = vec3(tempdirec.x, tempdirec.y, tempdirec.z);
    direction = normalize(direction);
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
}

void Plane::down()
{
    GLfloat updeg = 0.01;
    vec3 left = cross(upDirection, direction);
    vec4 tempup = vec4(upDirection,0.0);
    vec4 tempdirec = vec4(direction, 0.0);
    tempup = rotate(mat4(1.0f), updeg, left) * tempup;
    tempdirec = rotate(mat4(1.0f), updeg, left) * tempdirec;
    direction = vec3(tempdirec.x, tempdirec.y, tempdirec.z);
    direction = normalize(direction);
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
}

void Plane::rightRot()
{
    GLfloat rdeg = 0.01;
    vec4 tempup = vec4(upDirection,0.0);
    tempup = rotate(mat4(1.0f), rdeg, direction) * tempup;
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
}

void Plane::leftRot()
{
    GLfloat rdeg = 0.01;
    vec4 tempup = vec4(upDirection,0.0);
    tempup = rotate(mat4(1.0f), rdeg, -direction) * tempup;
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
}
