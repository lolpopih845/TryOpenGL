#version 330 core
out vec4 FragColor;

in vec3 position;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	float pos = ((position.x+1)/2.-(position.y+1)/2.)/2;
	FragColor = texture(texture1, TexCoord) * vec4(1.0,0.5+0.5*-pos,0.75*-pos, 1.0f);
}