#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    float pos = ((FragPos.x+1)/2.-(FragPos.y+1)/2.)/2;
    FragColor = texture(texture_diffuse1, TexCoords) * vec4(1.0,0.5+0.5*-pos,0.75*-pos, 1.0f);;
}