cbuffer CBuf
{
	matrix gWorld;
	matrix gWorldViewProj;
};

struct VSInput
{
	float3 Position	: POSITION;
	float3 Normal	: NORMAL;
	float2 TexCoord	: TEXCOORD;
};

struct VSOut
{
	float4 Position	: SV_Position;
	float3 Normal	: NORMAL0;
	float2 TexCoord	: TEXCOORD;
};


VSOut main(VSInput input )
{
	VSOut vsOutput = ( VSOut )0;
	vsOutput.Position = mul( float4( input.Position, 1 ), gWorldViewProj );
	vsOutput.Normal = normalize( mul( input.Normal, ( float3x3 )gWorld ) );
	vsOutput.Normal += float3( 1,1,1);
	vsOutput.Normal /= 2.f;
	//vsOutput.Normal = normalize( vsOutput.Normal );

	vsOutput.TexCoord = input.TexCoord;
	return vsOutput;
}