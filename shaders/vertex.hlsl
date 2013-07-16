
cbuffer Parameters : register(b0)
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

OutputData main(InputData input)
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
