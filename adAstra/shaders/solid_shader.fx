#pragma target 3.0

cbuffer ViewBuffer : register(b0)
{
	matrix View;
	matrix Projection;
}

cbuffer ConstantBuffer : register(b1)
{
	matrix World;
	float4 Color;
}

struct VS_INPUT
{
    float4 Pos   : POSITION;
};

struct PS_INPUT
{
    float4 Pos  : SV_POSITION;
};

PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

	output.Pos = input.Pos;
    output.Pos = mul(output.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
	return Color;
}
