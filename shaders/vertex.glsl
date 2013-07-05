
layout (std140) uniform mpv
{
	mat4 proj;
	mat4 world;
	mat4 view;
}

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = proj * world * view * gl_Vertex;
}
