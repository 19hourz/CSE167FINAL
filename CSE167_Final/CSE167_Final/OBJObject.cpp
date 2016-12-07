#include "OBJObject.h"
#include "Window.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace glm;


float OBJObject::minX;

OBJObject::OBJObject(const char *filepath)
{

    
    toWorld = glm::mat4(1.0f);
    angle = 0.0f;
    square_point_size = 0;
    pointSize = 1.0f;

	parse(filepath);
    target_obj = 1;

    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_Normals);
    glGenBuffers(1, &EBO);
    

    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

  
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    
    // Enable the usage of layout location 1 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
    
    

    
    int numStack = 50;
    float yDiff = 0.113483 + 0.113483;
    points.clear();
    for (int j = 0; j <= numStack + 1; j++){
        float height = -0.113483f + yDiff/numStack * j;
        points.push_back(-0.5);points.push_back(height);points.push_back(-0.303750);
        points.push_back(-0.5);points.push_back(height);points.push_back(0.303750);
        points.push_back(0.5);points.push_back(height);points.push_back(0.303750);
        points.push_back(0.5);points.push_back(height);points.push_back(-0.303750);
        points.push_back(-0.5);points.push_back(height);points.push_back(-0.303750);
    }
    
    
    
    glBindVertexArray(0);
    glGenVertexArrays(1, &CAO);
    glGenBuffers(1, &CBO);
    glBindVertexArray(CAO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(GLfloat), &points[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(GLfloat),
                          (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

OBJObject::~OBJObject()
{
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    float r,g,b;  // vertex color
    unsigned int i_1,i_2,i_3;
    float min_x,max_x,min_y,max_y,min_z,max_z;
    float dif_x,dif_y,dif_z;
    float avg_x,avg_y,avg_z;
    
    float longest_axis;
    
    fp = fopen(filepath,"rb");  // make the file name configurable so you can load other files
    if (fp==NULL) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt
    
    int res;
    do{
        
        char first_word[128];
        char sep_char[128];
        res = fscanf (fp, "%s", first_word);
        
        if ( strcmp( first_word, "v" ) == 0 ){
            fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            if (min_x==NULL || min_y==NULL || min_z==NULL){
                min_x = x;
                max_x = x;
                min_y = y;
                max_y = y;
                min_z = z;
                max_z = z;
            }
            else{
                if(x<min_x){
                    min_x = x;
                }
                if(x>max_x){
                    max_x = x;
                }
                if(y<min_y){
                    min_y = y;
                }
                if(y>max_y){
                    max_y = y;
                }
                if(z<min_z){
                    min_z = z;
                }
                if(z>max_z){
                    max_z = z;
                }
            }
        }
        else if( strcmp( first_word, "vn" ) == 0 ){
            fscanf(fp, "%f %f %f", &x, &y, &z);
            //glm::vec3 cur_normal = glm::vec3(x,y,z);
            //normals.push_back(cur_normal);
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
        else if( strcmp( first_word, "f" ) == 0 ){
            fscanf(fp, "%d %s %d %s %d", &i_1, sep_char, &i_2, sep_char, &i_3);
            indices.push_back(i_1-1);
            indices.push_back(i_2-1);
            indices.push_back(i_3-1);
        }
        
        
    }while(res != EOF);
    //cout<< "count: " << count << endl;
    // read normal data accordingly
    
    fclose(fp);   // make sure you don't forget to close the file when done
    
    //Calculate average and longest axis
    avg_x = (max_x + min_x)/2.0f;
    avg_y = (max_y + min_y)/2.0f;
    avg_z = (max_z + min_z)/2.0f;
    dif_x = max_x - min_x;
    dif_y = max_y - min_y;
    dif_z = max_z - min_z;
    
    
   

    
    if ( dif_x < dif_y ){
        longest_axis = dif_y;
    }
    else{
        longest_axis = dif_x;
    }
    if ( z > longest_axis ){
        longest_axis = dif_z;
    }
    
//    minX = (min_x-avg_x)/longest_axis;
//    float maxX = (max_x-avg_x)/longest_axis;
//    float minY = (min_y-avg_y)/longest_axis;
//    float maxY = (max_y-avg_y)/longest_axis;
//    float minZ = (min_z-avg_z)/longest_axis;
//    float maxZ = (max_z-avg_z)/longest_axis;
    
//    printf("x: %f, %f \n",minX,maxX);
//    printf("y: %f, %f \n",minY,maxY);
//    printf("z: %f, %f \n",minZ,maxZ);
    

    
    
    fp = fopen(filepath,"rb");  // make the file name configurable so you can load other files
    if (fp==NULL) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt
    
    //int res;
    do{
        char first_word[128];
        char sep_char[128];
        res = fscanf (fp, "%s", first_word);
        
        if ( strcmp( first_word, "v" ) == 0 ){
            fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            vertices.push_back((x-avg_x)/longest_axis);
            vertices.push_back((y-avg_y)/longest_axis);
            vertices.push_back((z-avg_z)/longest_axis);
        }
    }while(res != EOF);
    
    fclose(fp);
    
}

void OBJObject::draw(glm::mat4 mat)
{
    glUseProgram(Window::planeShader);
    glm::mat4 model = mat * toWorld;
    glUniformMatrix4fv(glGetUniformLocation(Window::planeShader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(Window::planeShader, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(Window::planeShader, "view"), 1, GL_FALSE, &Window::V[0][0]);
    glUniform3f(glGetUniformLocation(Window::planeShader, "cameraPos"), Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
    
    
    if(Window::showBounding){
        glm::mat4 modelview = Window::V * mat * toWorld;
        glUseProgram(Window::cubeShader);
        uProjection = glGetUniformLocation(Window::cubeShader, "projection");
        uModelview = glGetUniformLocation(Window::cubeShader, "modelview");
        GLuint utype = glGetUniformLocation(Window::cubeShader, "type");
        if(Window::isCollide)
            glUniform1i(utype, 12);
        else
            glUniform1i(utype, 11);

        
        glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
        glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
        glBindVertexArray(CAO);
        glDrawArrays(GL_LINE_STRIP, 0, points.size()/3);
        glBindVertexArray(0);
    }
}

void OBJObject::update()
{
    //spin(1.0f);
}

//Additional Feature Functions
void OBJObject::spin(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    // This creates the matrix to rotate the cube
    this->toWorld = this->toWorld * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}


void OBJObject::move(float x, float y, float z)
{
    this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z)) * this->toWorld;
}

void OBJObject::scale(float size)
{
    this->toWorld =  this->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(size,size,size));
}

void OBJObject::orbit(float deg,vec3 axis)
{
    this->toWorld =  glm::rotate(glm::mat4(1.0f), deg/180.0f * glm::pi<float>(), glm::vec3(axis.x,axis.y, 0.0f)) * this->toWorld ;
    
}

void OBJObject::reset(){
    this->toWorld = glm::mat4(1.0f);
}
