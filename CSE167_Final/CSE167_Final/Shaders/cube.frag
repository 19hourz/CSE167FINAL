#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec2 TexCoord;

uniform int type;
uniform sampler2D ourTexture;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
//    if(type == 1 || type == 2){
//        color = vec4(0.7f, 0.7f, 0.3f, 1.0f);
//    }
//    else if(type == 3){
//        color = vec4(0.25f, 0.06f, 0.00f,  1.0f);
//    }
//    else if(type == 4){
//        color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//    }
    color = texture(ourTexture, TexCoord);

}

