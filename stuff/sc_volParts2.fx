
#define DOSUN

Texture entSkin1;

sampler2D colorSampler = sampler_state
{
	texture = <entSkin1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

Texture entSkin2;

sampler2D normSampler = sampler_state
{
	texture = <entSkin2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

// ShadowDepth map sampler
texture sc_map_shadowDepthSun_bmap;
sampler ShadowMapSampler = sampler_state
{
	Texture = <sc_map_shadowDepthSun_bmap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

Texture sc_map_shadowAlpha_bmap;

sampler ShadowAlphaSampler = sampler_state
{
	texture = <sc_map_shadowAlpha_bmap>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter=LINEAR;
	AddressU = clamp;
	AddressV = clamp;
};

matrix matWorld;
matrix matWorldInv;
matrix matViewInv;
matrix matWorldViewProj;
float4x4 matEffect1;
float4x4 matEffect2;
float4x4 matMtl;

float4 xLightPos = {-427,538,654,1};
float xLightPower = 1;
float xMaxDepth = 800;
float light_vsm_epsilon = 0.0002;
float light_shadow_bias = 0.0025;

float4 vecSkill1;
float4 vecLightPos[8];
float4 vecLightColor[8];
float4 vecViewPos;
float4 vecSunPos;
float4 vecSunColor;
float4 vecFog;

float doVSM(float2 depthmap, float lightDepth)
{
     	//depthmap.y = (depthmap.x * depthmap.x);
     	float lit_factor = (lightDepth <= depthmap.x);
    	
    	// Variance shadow mapping
    	float E_x2 = depthmap.y;
    	float Ex_2 = depthmap.x * depthmap.x;
    	float variance = min(max(E_x2 - Ex_2, 0.0) + light_vsm_epsilon, 1.0);
    	float m_d = (depthmap.x - lightDepth);
    	float p = variance / (variance + m_d * m_d);
    	
    	// Adjust the light color based on the shadow attenuation
    	return 1-max(lit_factor, p);
}

void volParts2_VS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0,
   in float3 InTangent : TEXCOORD0,
   in float2 InShadow : TEXCOORD1,
   out float4 OutPos: POSITION, 
   out float2 OutTex: TEXCOORD0, 
   out float4 OutLight: TEXCOORD1,
   out float3 OutViewDir: TEXCOORD2,
   out float3 OutWorldNormal	: TEXCOORD3,
	out float3 OutWorldTangent	: TEXCOORD4,
	out float3 OutWorldBinorm	: TEXCOORD5,
	out float3 OutWorldPos	: TEXCOORD6,
	out float4 OutShadow : TEXCOORD7,
	//out float4 OutLight2: TEXCOORD7,
   out float OutFog : FOG) 
{ 
	// Transform the vertex from object space to clip space: 
	float3 P = InPos.xyz - (InNormal * 50);
	if(vecSkill1.x == 0) OutPos = mul(InPos, matWorldViewProj);
	else OutPos = mul(float4(P, 1.0f), matWorldViewProj);
	
	// Pass the texture coordinate to the pixel shader: 
   OutTex.xy = InTex;
   
   float3 PosWorld = mul(InPos, matWorld);
  	
   //Light
   #ifdef DOSUN
   OutLight.xyz = vecSunPos - PosWorld;;
   OutLight.w = 1;//distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
   #else
   OutLight.xyz = vecLightPos[0] - PosWorld;
   OutLight.w = 0;
   if(vecLightPos[0].w < 100000) OutLight.w = 1-distance(PosWorld,vecLightPos[0])/vecLightPos[0].w;
   OutLight2.xyz = vecLightPos[1] - PosWorld;
   OutLight2.w = 0;
   if(vecLightPos[1].w < 100000) OutLight2.w = 1-distance(PosWorld,vecLightPos[1])/vecLightPos[1].w;
   #endif
  	//

	//Specular Lighting
	#ifdef DOSUN
	OutViewDir = (vecSunPos) - mul(InPos, matWorld);
	#else
	OutViewDir = (vecLightPos[0]) - mul(InPos, matWorld); 
	#endif
	//
	
	//Environment Mapping
	float3 Binormal = cross(InNormal,InTangent);
	OutWorldNormal.xyz = mul(InNormal, matWorld).xyz;
	OutWorldTangent.xyz = mul(InTangent, matWorldInv).xyz;
	OutWorldBinorm.xyz = mul(Binormal, matWorldInv).xyz;
	OutWorldPos = PosWorld;
	//

	//Shadow
	//Shadowmapping
		float4 ShadowMapSamplingPos = mul(mul(InPos,matWorld), mul(matMtl,matEffect1));
	float4 RealDistance = mul(mul(InPos,matWorld), matMtl).z/matEffect2[0].w;
	
	OutShadow.x = ShadowMapSamplingPos.x;
	OutShadow.y = ShadowMapSamplingPos.y;
	OutShadow.z = ShadowMapSamplingPos.w;
	OutShadow.w = RealDistance.x;
	//
	
	//Fog
	OutFog = 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z);
   //
}

float4 volParts2_PS(
float2 Tex : TEXCOORD0,
float4 InLight: TEXCOORD1,
float3 InViewDir: TEXCOORD2,
float3 InWorldNormal	: TEXCOORD3,
float3 InWorldTangent	: TEXCOORD4,
float3 InWorldBinorm	: TEXCOORD5,
float3 InWorldPos	: TEXCOORD6,
float4 InShadow: TEXCOORD7
 ) : COLOR0 
{
	float3 Ln = normalize(InLight.xyz);
	//float3 Ln2 = normalize(InLight2.xyz);
   float3 Nn = normalize(InWorldNormal);
   float3 Tn = normalize(InWorldTangent);
   float3 Bn = normalize(InWorldBinorm);
   float3 Nb = 0;
   float3 Vn = 0;
   float3 Hn = 0;
   float4 lighting = 0;
   float4 lighting2 = 0;
	
	float3 bumpNormal = 0;
	Nb = Nn + (bumpNormal.x * Tn + bumpNormal.y * Bn);
   Nb = normalize(Nb);
   Vn = normalize(InViewDir);
   Hn = normalize(Vn + Ln);
   lighting = lit(dot(Ln,Nb),dot(Hn,Nb),20);
   //lighting2 = lit(dot(Ln2,Nb),dot(Hn,Nb),20);
   //lighting += lighting2;
	
	
	// ---- SHADOW ----
   float2 ProjectedTexCoords;
   ProjectedTexCoords[0] = InShadow.x/InShadow.z/2.0f +0.5f;
   ProjectedTexCoords[1] = -InShadow.y/InShadow.z/2.0f +0.5f;
   
  

	float blurStr = matEffect2[1].x;
	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
	
	/*
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr,0)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr,0)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(0,blurStr)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(0,-blurStr)).x;
	*/
	/*
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr/2,blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr/2,-blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(-blurStr/2,blurStr/2)).x;
	shadowDepth += tex2D(ShadowMapSampler, ProjectedTexCoords+float2(blurStr/2,-blurStr/2)).x;
	*/
	//shadowDepth /= 5;


	
	float shadow1 = doVSM(float2(shadowDepth,shadowDepth*shadowDepth),InShadow.w);
  	shadow1 = (1-shadow1);
   
   shadow1 *=(1-tex2D(ShadowAlphaSampler, ProjectedTexCoords));
   shadow1 += tex2D(ShadowAlphaSampler, ProjectedTexCoords);
   // ---- // ----
	
	
	float4 Color;
	Color.rgb = tex2D(colorSampler,(Tex.xy)).rgb;
	Color.a = tex2D(colorSampler,(Tex.xy)).a;
	//if(Color.a < 0.1) clip(-1);
	if(vecSkill1.x == 0) Color = 0;
	
	float shadowAlpha = tex2D(normSampler,(Tex.xy)).a;
	Color.rgb *= clamp((lighting.y)*(vecSunColor),shadowAlpha,1);
	//Color.rgb *= lighting.y;
	
	//Velvety
	Vn = normalize(matViewInv[3].xyz - InWorldPos);
	half3 velvety1;
   float vdn = 0.25-dot(Vn,Nn);
   float3 vecColor = float4(vdn.xxx,1.0);
   velvety1 = float4((vecColor).xyz,1);
   velvety1 = clamp(velvety1,0,1);
   Color.rgb += Color.rgb*(velvety1);
   //Color.a = Color.a * pow((1-velvety1),2);
   //
    //Color.a *= 1-vdn;
    
    Color.rgb *= shadow1;
    
   return Color;
}

technique volParts
{
	pass one
	{
		cullmode = none;
		zwriteenable = true;
		alphablendenable = true;
		VertexShader = compile vs_2_0 volParts2_VS();
		PixelShader = compile ps_2_0 volParts2_PS();
	}
}