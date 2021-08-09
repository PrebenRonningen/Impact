Texture2D gTexDifuse : register( t0 );
//Texture2D gTexHeight : register( t1 );

SamplerState gSamplerState: register( s0 );

struct PSInput
{
	float4 pos		: SV_Position;
	float2 TexCoord	: TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
	float4 finalColor = gTexDifuse.Sample( gSamplerState, input.TexCoord );
	clip( finalColor.a < 0.1f ? -1 : 1 );
	return finalColor;
}