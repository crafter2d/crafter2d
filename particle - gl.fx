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
	vec4 col;
	float radius;
};

struct PixelData
{
    vec4 color;
	vec2 texCoord;
};

VertexData mainVertex()
{
	VertexData.col = col;
	VertexData.radius = radius;
	gl_Position = vec4(pos, 0, 1);
}

[maxvertexcount(4), inputtype(point), outputtype(trianglestrip)]
void mainGeometry(VertexData data, PixelData outdata)
{
	gl_Position = vec4(gl_in[0].gl_Position.x - VertexData[0].radius, gl_in[0].gl_Position.y - VertexData[0].radius, 0, 1) * proj;
	PixelData.texCoord = vec2(0, 0);
	PixelData.color = VertexData[0].col;
	EmitVertex();
	
	gl_Position = vec4(gl_in[0].gl_Position.x + VertexData[0].radius, gl_in[0].gl_Position.y - VertexData[0].radius, 0, 1) * proj;
	PixelData.texCoord = vec2(1, 0);
	PixelData.color = VertexData[0].col;
	EmitVertex();
	
	gl_Position = vec4(gl_in[0].gl_Position.x - VertexData[0].radius, gl_in[0].gl_Position.y + VertexData[0].radius, 0, 1) * proj;
	PixelData.texCoord = vec2(0, 1);
	PixelData.color = VertexData[0].col;
	EmitVertex();
	
	gl_Position = vec4(gl_in[0].gl_Position.x + VertexData[0].radius, gl_in[0].gl_Position.y + VertexData[0].radius, 0, 1) * proj;
	PixelData.texCoord = vec2(1, 1);
	PixelData.color = VertexData[0].col;
	EmitVertex();
}

void mainPixel()
{
	vec4 color = texture2D(diffuseMap, PixelData.texCoord) * PixelData.color;
	frag = color;
}

technique Basic
{
	vertex = v410 mainVertex
	geometry = v410 mainGeometry
	pixel  = v410 mainPixel
}
