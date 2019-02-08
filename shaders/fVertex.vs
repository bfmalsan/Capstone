#version 420 

layout(location = 0) in vec2 pos;

uniform mat4 viewMat;

uniform mat4 projMat;


void main()
{

	
	vec4 position = vec4(pos,.5,1.0);

	//gl_Position = projMat*viewMat*position;

	gl_Position = position;
}	