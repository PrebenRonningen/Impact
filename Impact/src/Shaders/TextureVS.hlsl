cbuffer bcPerFrame : register(b0)
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
	float4 Color	: COLOR;
	float3 Normal	: NORMAL;
	float3 WorldPos : POSITION;
	float2 TexCoord	: TEXCOORD;
};


VSOut main(VSInput input )
{
	VSOut vsOutput = ( VSOut )0;
	vsOutput.Position = mul( float4( input.Position, 1 ), gWorldViewProj );
	vsOutput.Color = float4( 1, 1, 1, 1 ); // not rly needed
	
	vsOutput.Normal = normalize( mul( input.Normal, ( float3x3 )gWorld ) );
	//vsOutput.Normal.z *= -1;
	//vsOutput.Normal += float3( 1,1,1);
	//vsOutput.Normal /= 2.f;
	vsOutput.WorldPos = ( float3 )mul( float4( input.Position, 1 ), gWorld );
	
	vsOutput.TexCoord = input.TexCoord;
	
	return vsOutput;
}