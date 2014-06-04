// Basic effect file

texture diffuseMap : 0;
sampler diffuseSampler : 0;

cbuffer mpv : 0
{
	float4x4 proj;
	float4x4 world;
	float4x4 object;
};

struct VSDataIn
{
	float2 pos : POSITION;
	float4 col : COLOR0;
	float  radius: RADIUS;
};

struct PSDataIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXTURE0;
	float4 col : COLOR0;
};

VSDataIn mainVertex(VSDataIn input)
{
	return input;
}

[maxvertexcount(4), inputtype(point)]
void mainGeometry(VSDataIn input[1], TriangleStream<PSDataIn> stream)
{
    PSDataIn output;
	output.col = input[0].col;
    
	output.pos = mul(float4(input[0].pos.x - input[0].radius, input[0].pos.y - input[0].radius, 0.0f, 1.0f), proj);
	output.tex = float2(0, 0);
	stream.Append(output);
	
	output.pos = mul(float4(input[0].pos.x + input[0].radius, input[0].pos.y - input[0].radius, 0.0f, 1.0f), proj);
	output.tex = float2(1, 0);
	stream.Append(output);
	
	output.pos = mul(float4(input[0].pos.x - input[0].radius, input[0].pos.y + input[0].radius, 0.0f, 1.0f), proj);
	output.tex = float2(0, 1);
	stream.Append(output);
	
	output.pos = mul(float4(input[0].pos.x + input[0].radius, input[0].pos.y + input[0].radius, 0.0f, 1.0f), proj);
	output.tex = float2(1, 1);
	stream.Append(output);
}

float4 mainPixel(PSDataIn input) : SV_TARGET
{
	return diffuseMap.Sample(diffuseSampler, input.tex) * input.col;
}

technique Particle
{
	vertex = vs_4_0 mainVertex
	geometry = gs_4_0 mainGeometry
	pixel  = ps_4_0 mainPixel
}
