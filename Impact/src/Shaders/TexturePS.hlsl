Texture2D gTexDiffuse : register( t0 );
//Texture2D gTexHeight : register( t1 );

SamplerState gSamplerState: register( s0 );

struct LightData
{
	float3 Pos;
};


cbuffer CBuf
{
	int size;
	LightData lightData[100];
};

struct PSInput
{
	float4 Position	: SV_Position;
	float3 Normal	: NORMAL0;
	float2 TexCoord	: TEXCOORD;
};


float4 main(PSInput input) : SV_TARGET
{
	float4 finalColor = float4( 0, 0, 0, 1 );
	for( int i = 0 ; i < size ; i++ )
	{
		finalColor.rgb += lightData[i].Pos.rgb;
	}
	finalColor.rgb /= float( size - 1 );
	//float4 finalColor = ;
	finalColor *= gTexDiffuse.Sample( gSamplerState, input.TexCoord ); // * float4( input.Normal, 1 );
	//clip( finalColor.a < 0.1f ? -1 : 1 );
	return finalColor;
}