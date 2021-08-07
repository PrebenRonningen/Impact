
cbuffer CBuffer
{
	matrix transform;
};

struct VSInput
{
	float3 pos		: POSITION;
//	float4 color	: Color;
};

//struct VSOutput
//{
//	float4 pos		: SV_POSITION;
//	float4 color	: Color;
//};

//VSOutput main(VSInput input)
//{
//	VSOutput output;
//	output.pos = mul( float4( input.pos.xyz, 1.0f ), transform );
//	output.color = input.color;
//	return output;
//}

float4 main(float3 pos : Position) : SV_Position
{
	return mul( float4( pos, 1.0f ), transform );
}