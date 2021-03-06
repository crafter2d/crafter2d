// Basic effect file

texture diffuseMap : 0;
sampler diffuseSampler : 0;

cbuffer mpv : 0
{
	float4x4 proj;
	float4x4 world;
	float4x4 object;
};

struct InputData
{
	float2 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct OutputData
{
	float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

OutputData mainVertex(InputData input)
{
	OutputData data;
	
	float4 pos = float4(input.pos, 0.0f, 1.0f);
	pos = mul(pos, object);
	pos = mul(pos, world);
	pos = mul(pos, proj);
	
	data.pos = pos;
	data.tex = input.tex;
	
	return data;
}

float4 mainPixel(OutputData input) : SV_TARGET
{
	float4 texelColor = diffuseMap.Sample(diffuseSampler, input.tex);
	return texelColor;
}

technique Basic
{
	vertex = vs_4_0_level_9_3 mainVertex
	pixel  = ps_4_0_level_9_3 mainPixel
}
