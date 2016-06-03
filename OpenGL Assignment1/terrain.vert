#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;

out vec3 fragmentColor;
out vec2 TexCoords;

uniform mat4 MVP;

void main(){	

	gl_Position =  MVP * vec4(in_Position, 1);
	fragmentColor = in_Color;

}