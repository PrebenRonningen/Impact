
cbuffer CBuffer
{
	float4 face_Color[8];
}

struct PSInput
{
	float4 pos		: SV_Position;
	float4 color	: Color;
};


float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return face_Color[( tid / 2 ) % 8];
}