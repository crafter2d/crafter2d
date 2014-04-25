// Basic effect file

texture diffuseMap
sampler diffuseSampler
language ogl

cbuffer mpv
{
	mat4 proj;
	mat4 world;
	mat4 object;
};

struct VertexData
{
	vec2 pos;
	vec2 tex;
};

struct PixelData
{
	vec2 texCoord;
};

void mainVertex()
{
	PixelData.texCoord = tex;
	gl_Position = vec4(pos, 0, 1) * world * proj;
}

void mainPixel()
{
	vec4 color = texture2D(diffuseMap, PixelData.texCoord);
	frag = color;
}

technique Basic
{
	vertex = vs_4_0_level_9_3 mainVertex
	pixel  = ps_4_0_level_9_3 mainPixel
}
