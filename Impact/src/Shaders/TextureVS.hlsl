cbuffer CBuf
{
	matrix gWorldViewProj;
};

struct VSInput
{
	float3 pos		: POSITION;
	float2 TexCoord	: TEXCOORD;
};

struct VSOut
{
	float4 pos		: SV_Position;
	float2 TexCoord	: TEXCOORD;
};


VSOut main(VSInput input )
{
	VSOut vsOutput = ( VSOut )0;
	vsOutput.pos = mul( float4( input.pos, 1 ), gWorldViewProj );
	vsOutput.TexCoord = input.TexCoord;
	return vsOutput;
}