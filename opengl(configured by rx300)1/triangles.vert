
#version 430 core

layout( location = 0 ) in vec4 vPosition; /*locationӦ��Ĭ����0;��������vPosition����һ����������*/
out vec4 fscolor;
uniform float time;

uniform Model{
vec4 ModelView;
vec4 Color;
} ModelName;

void
main()
{	
    gl_Position = vPosition;
	gl_Position[0]+=time;
	fscolor=vPosition;
}
