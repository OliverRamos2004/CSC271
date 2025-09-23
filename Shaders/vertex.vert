#version 410 core
layout(location = 0) in vec3 aPOS;
in vec3 aColor;
in vec2 aTexCoord;
out vec4 vertexColor;
out vec2 TexCoord;

void main(){
    gl_Position = vec4(aPOS, 1.0);
    vertexColor = vec4(aColor, 1.0);
    TexCoord = aTexCoord;
}
// Output is gl_position: Create data, create buffer, link data, use shader.
//    vertexColor = v FIX