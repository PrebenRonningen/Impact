Texture2D gTexDiffuse : register( t0 );

SamplerState gSamplerState: register( s0 );

static const float3 ambientLight = float3( 0.075f, 0.075f, 0.075f );


struct LightData
{
	float3 lightPos;
	float pad;
	float3 lightDir;
	float pad2;
	float3 lightColor;
	float pad3;
	float lightRange;
	float lightIntensity;
	float attConst;
	int fallOffType;
	int lightType;
};

cbuffer cbLight : register(b0)
{
	int NumLights;
	LightData lightData[100];
};

struct PSInput
{
	float4 Position	: SV_Position;
	float4 Color	: COLOR;
	float3 Normal	: NORMAL;
	float3 WorldPos : POSITION;
	float2 TexCoord	: TEXCOORD;
};


float3 CalculatePointLight(const float3 vertWorldPos, const float3 vertNormal, const LightData light)
{
	const float3 vToLight = light.lightPos - vertWorldPos;
	const float distToLight = length( vToLight );
	const float3 dirToLight = normalize( vToLight / distToLight );
	
	const float attLin = 4.5f / light.lightRange;
	const float attQua = 75.f / pow( light.lightRange, 2.f );
	
	// falloff type: none, linear, quadratic
	// can be added by including 1st, the two first, or all parts

	float attenuation
	{
	};
	switch( light.fallOffType )
	{
		case 0: // None
			attenuation = light.attConst;
			break;
		case 1: // Linear
			attenuation = ( light.attConst + attLin * distToLight );
			break;
		case 2: // Quadratic
			attenuation = ( light.attConst + attLin * distToLight + attQua * pow( distToLight, 2.f ) );
			break;
	}
	// this magic '1.f' can be swapped out for lightIntensity
	const float luminosity = 1.f / attenuation;
	
	const float3 lightDiffuse = light.lightColor * light.lightIntensity * luminosity * saturate( dot( dirToLight, vertNormal ) );

	return pow( lightDiffuse, 2 );
}

float3 CalculateDirectionalLight(const float3 vertNormal, const LightData light)
{
	// this magic '1.f' can be swapped out for lightIntensity
	const float luminosity = 1.f / light.attConst;
	
	const float3 lightDiffuse = light.lightColor * light.lightIntensity * luminosity * saturate( dot( -normalize( light.lightDir), vertNormal ) );

	return pow( lightDiffuse, 2 );
}

float4 main(PSInput input) : SV_TARGET
{
	float3 lightDiffuse = (float3)0;
	for( int i = 0 ; i < NumLights ; i++ )
	{
		switch( lightData[i].lightType )
		{
			case 0:
			{
					lightDiffuse += CalculatePointLight( input.WorldPos, normalize( input.Normal ), lightData[i] );
				break;
			}
			case 1:
			{
					lightDiffuse += CalculateDirectionalLight( normalize( input.Normal ), lightData[i] );
				break;
			}
		}
	}
	
	const float3 t = gTexDiffuse.Sample( gSamplerState, input.TexCoord ).rgb;
	float3 finalColor = t * ( lightDiffuse + ambientLight );
	if( NumLights <= 0 )
	{
		finalColor = t;
	}
	
	return float4( finalColor, 1.0f );
}