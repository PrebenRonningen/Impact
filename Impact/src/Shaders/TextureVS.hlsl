
Texture2D gHeightMap : register( t0 );

static const float maxHeight = .128;

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
	uint2 dimensions = ( uint2 )0;
	gHeightMap.GetDimensions( dimensions.x, dimensions.y );

	const float height = gHeightMap[input.TexCoord*dimensions] * maxHeight;
	
	input.Position += height * input.Normal;
	
	vsOutput.Normal = normalize( mul( input.Normal, ( float3x3 )gWorld ) );
	
	vsOutput.Position = mul( float4( input.Position, 1 ), gWorldViewProj );
	
	vsOutput.WorldPos = ( float3 )mul( float4( input.Position, 1 ), gWorld );
	
	vsOutput.TexCoord = input.TexCoord;
	
	return vsOutput;
}