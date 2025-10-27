#version 410 core
layout(location = 0) in vec3 aPOS;
in vec3 aColor;
in vec2 aTexCoord;
out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 model; // uniform Means never changes
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model  * vec4(aPOS, 1.0);
    vertexColor = vec4(aColor, 1.0);
    TexCoord = aTexCoord;
}
