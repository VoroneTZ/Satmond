Texture entSkin2;

sampler2D colorSampler = sampler_state
{
	texture = <entSkin2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

matrix matWorld;
matrix matWorldInv;
matrix matWorldViewProj;

float4 vecSkill1;
float4 vecLightPos[8];
float4 vecViewPos;
float4 vecFog;
float4 vecTime;

/*
float4 volParts_PS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float4 Color = tex2D(colorSampler,(Tex.xy));
	//Color.rgb = Color.r;
	//Color.a = 1;
	//Color.r = vecSkill1.x;
	Color.rgb = Color.a;
	if(vecSkill1.x == 0) Color = 0;
	Color.a = Color.rgb;
	Color.rgb = 0.0;
	//Color.a = Color.rgb;
	//if(Color.a > 0) Color.a = 1;
	
	return Color;
}

float4 volParts2_PS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float4 Color = tex2D(colorSampler,(Tex.xy));
	//Color.rgb = Color.r;
	//Color.a = 1;
	//Color.r = vecSkill1.x;
	if(vecSkill1.x == 0) Color = 0;
	Color.a = 0.0;
	//if(Color.a > 0) Color.a = 1;
	//Color.rgb = Color.a;
	return Color;
}
*/

void volParts_VS(
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0,
   out float4 OutPos: POSITION, 
   out float2 OutTex: TEXCOORD0, 
   out float OutFog : FOG) 
{ 
	//scale model
	float3 P = InPos.xyz + (InNormal * 50);
	if(vecSkill1.x == 0) OutPos = mul(InPos, matWorldViewProj);
	else OutPos = mul(float4(P, 1.0f), matWorldViewProj);
	//OutPos = mul(float4(P, 1.0f), matWorldViewProj);
	//
	
	// Pass the texture coordinate to the pixel shader: 
   OutTex.xy = InTex+(vecTime.w*0.001*vecSkill1.y);
   
   float3 PosWorld = mul(InPos, matWorld);
  	
  	//Fog
	OutFog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
   //
}


float4 volParts_PS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float4 Color = tex2D(colorSampler,(Tex.xy));
	//Color.rgb = Color.r;
	//Color.a = 1;
	//Color.r = vecSkill1.x;
	//Color.rgb = Color.a;
	
	//if(vecSkill1.x == 0) Color = 0;
	if(vecSkill1.x == 0) clip(-1);
	Color.a = 1;
	
	//Color.rgb = vecViewPos.xyz;
	//if(Color.a == 1)Color.a = 0;
	//Color.rgb = 0.0;
	//Color.a = Color.rgb;
	//if(Color.a > 0) Color.a = 1;
	
	
	
	return Color;
}



technique volParts
{
	pass one
	{

		cullmode = CW;
		zwriteenable = true;
		alphablendenable = false;
		VertexShader = compile vs_2_0 volParts_VS();
		PixelShader = compile ps_2_0 volParts_PS();
	
		
	}
}
