#pragma target 3.0

Texture2D txDiffuse : register( t0 );

SamplerState samLinear : register( s0 );

cbuffer ViewBuffer : register( b0 )
{
	matrix View;
	matrix Projection;
}

cbuffer ConstantBuffer : register( b1 )
{
	matrix World;
}

struct VS_INPUT
{
	float4 Pos   : POSITION;
	float2 tex   : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos      : SV_POSITION;
	float2 tex      : TEXCOORD;
};

PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.tex = input.tex;

    return output;
}


float4 PS( PS_INPUT input) : SV_Target
{
	return txDiffuse.Sample(samLinear, input.tex);
}
