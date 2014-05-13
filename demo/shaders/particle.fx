// Basic effect file

texture diffuseMap : 0;
sampler diffuseSampler : 0;

cbuffer mpv : 0
{
	float4x4 proj;
	float4x4 world;
	float4x4 object;
};

cbuffer cbImmutable
{
    float2 g_positions[4] =
    {
        float2( -1, -1 ),
        float2(  1, -1 ),
        float2(  1,  1 ),
        float2( -1,  1 ),
    };
    float2 g_texcoords[4] = 
    { 
        float2(0,0), 
        float2(1,0),
        float2(1,1),
        float2(0,1),
    };
};

struct VSDataIn
{
	float2 pos : POSITION;
	float4 col : COLOR0;
	float  radius: RADIUS;
};

struct VSDataOut
{
	float2 pos : POSITION;
	float4 col : COLOR0;
	float radius : RADIUS;
};

struct PSDataIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXTURE0;
	float4 col : COLOR0;
};

VSDataOut mainVertex(VSDataIn input)
{
	VSDataOut data;
	
	data.pos = input.pos;
	data.col = input.col;
	data.radius = input.radius;
	
	return data;
}

[maxvertexcount(4), inputtype(point)]
void mainGeometry(VSDataOut input[1], TriangleStream<PSDataIn> SpriteStream)
{
    PSDataIn output;
    
    // Emit two new triangles
    for( int i = 0; i < 4; i++ )
    {
		float2 pos = input[0].pos + (g_positions[i] * input[0].radius);
        float4 position = float4(pos, 0.0f, 1.0f);
		position = mul(position, proj);
		
        output.pos = position;
		output.tex = g_texcoords[i];
        output.col = input[0].col;        
		
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
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
