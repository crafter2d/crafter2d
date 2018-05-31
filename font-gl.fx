// Font effect file

texture diffuseMap
sampler diffuseSampler
language ogl

#define MAX_TRANSFORMS 92

cbuffer mpv
{
	mat4 proj;
	mat4 world;
	mat4 object;
};

cbuffer transformBuffer
{
	mat4 transforms[MAX_TRANSFORMS];
};

struct VertexData
{
	vec2 pos;
	vec2 tex;
	uint tr;
};

struct PixelData
{
    vec2 tex;
};

void mainVertex()
{
	PixelData.texCoord = tex;
	
	vec4 tpos = vec4(pos, 0, 1) * transforms[tr];
	gl_Position = tpos * world * proj;
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
