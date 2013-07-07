
layout (std140) uniform mpv
{
	mat4 proj;
	mat4 world;
	mat4 object;
};

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_Vertex * object * world * proj;
}
