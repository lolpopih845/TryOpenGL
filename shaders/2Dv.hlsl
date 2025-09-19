#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 position;

uniform mat4 model;
layout(std140) uniform Camera {
	mat4 uView;
	mat4 uProjection;
};

void main()
{
	gl_Position = uProjection * uView * model * vec4(aPos, 1.0);
	position = aPos;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}