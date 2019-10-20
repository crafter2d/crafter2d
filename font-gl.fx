// Font effect file

texture diffuseMap
sampler diffuseSampler
language ogl

#define MAX_TRANSFORMS 92

cbuffer mpv
{
	mat4 proj;
};

cbuffer TransformVars
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
    vec2 texCoord;
};

PixelData mainVertex(VertexData in)
{
	PixelData.texCoord = tex;
	
	gl_Position = (vec4(pos, 0, 1) * transforms[tr]) * proj;
}

void mainPixel()
{
	float color = texture2D(diffuseMap, PixelData.texCoord).r;
	frag = vec4(color, color, color, color);
}

technique Basic
{
	vertex = vs_4_0_level_9_3 mainVertex
	pixel  = ps_4_0_level_9_3 mainPixel
}
