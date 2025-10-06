#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 1) in vec3 aColor;

out vec2 TexCoord;

//in vec3 aColor;
//in vec2 aTexCoord;
//out vec4 vertexColor;

void main(){
    gl_Position = vec4(aPos, 1.0);
//    vertexColor = vec4(aColor, 1.0);
    TexCoord = aTexCoord;
}
// Output is gl_position: Create data, create buffer, link data, use shader.
//    vertexColor = v FIX