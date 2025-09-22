#version 410 core
layout(location = 0) in vec3 aPOS;
//in vec3 aColor;
//out vec4 vertexColor; ---CHANGED 09/20/25 line 3, 4, 8

void main(){
    gl_Position = vec4(aPOS, 1.0);  // Output is gl_position: Create data, create buffer, link data, use shader.
//    vertexColor = vec4(aColor, 1.0);
}