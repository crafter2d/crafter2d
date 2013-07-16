
Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

struct InputData
{
	float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputData input) : SV_TARGET
{
	float4 texelColor = diffuseMap.Sample(diffuseSampler, input.tex);
	return texelColor;
}
