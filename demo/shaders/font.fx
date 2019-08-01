// Font effect file

texture diffuseMap : 0;
sampler diffuseSampler : 0;

#define MAX_TRANSFORMS 92

cbuffer mpv : 0
{
	float4x4 proj;
};

cbuffer TransformVars : 1
{
   float4x4 transforms[MAX_TRANSFORMS];
};

struct InputData
{
	float2 pos : POSITION;
	float2 tex : TEXCOORD0;
	uint   tr  : BLENDINDICES0;
};

struct OutputData
{
	float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

OutputData mainVertex(InputData input)
{
	float4 pos = mul(mul(float4(input.pos, 0.0f, 1.0f), transforms[input.tr]), proj);
	
	//pos = mul(tranformedPos, object);
	//pos = mul(pos, world);
	//pos = mul(pos, proj);

	OutputData data;
	data.pos = pos;
	data.tex = input.tex;
	return data;
}

float4 mainPixel(OutputData input) : SV_TARGET
{
	float texelColor = diffuseMap.Sample(diffuseSampler, input.tex);
	return float4(texelColor, texelColor, texelColor, 1);
}

technique Basic
{
	vertex = vs_4_1 mainVertex
	pixel  = ps_4_1 mainPixel
}
