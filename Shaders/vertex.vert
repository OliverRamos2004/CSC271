#version 410 core
in vec3 aPos; // the position variable has attribute position 0
in vec3 aColor; // the color variable has attribute position 1
in vec2 aTexCoord; // the texture coords has attribute position 2

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor; // specify a color output to the fragment shader
out vec2 TexCoord; // specify a texture coord output to the fragment shader

void main(){
    //gl_Position = transform * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0);
    vertexColor = vec4(aColor, 1.0);
    TexCoord = aTexCoord;
}