#version 410 core



//in vec4 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1; // Texture unit 1
uniform sampler2D texture2; // Texture unit 2
uniform float mixAmount;

void main(){
    vec4 c0 = texture(texture1, TexCoord);
    vec4 c1 = texture(texture2, TexCoord);
    FragColor = mix(c0,c1, mixAmount);  // Created a variable mixAmount to make things easier and not list the float every time.

}