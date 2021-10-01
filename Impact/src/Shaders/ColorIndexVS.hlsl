
cbuffer CBuffer
{
	matrix gWorld;
	matrix gWorldViewProj;
};

struct VSInput
{
	float3 Position	: POSITION;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
};


VSOutput main(VSInput input)
{
	VSOutput output = ( VSOutput )0;
	output.pos = mul( float4( input.Position, 1.0f ), gWorldViewProj );
	return output;
}