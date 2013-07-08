
layout (std140) uniform mpv
{
	mat4 proj;
	mat4 world;
	mat4 object;
};

out Data
{
	vec2 texCoord;
} DataOut;

in vec2 position;
in vec2 tex;

void main()
{
	DataOut.texCoord = tex;
	gl_Position = vec4(position,0,1) * object * world * proj;
}
