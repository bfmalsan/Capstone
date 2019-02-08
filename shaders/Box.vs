#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 modelMat;

uniform mat4 viewMat;

uniform mat4 projMat;

void main()
{

	vec4 pos = vec4(position,1.0);
	//gl_Position = projMat*viewMat*modelMat*pos;
	gl_Position = pos;
}