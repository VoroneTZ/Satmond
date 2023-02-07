Texture TargetMap; //particles
Texture mtlSkin1; //scene
Texture mtlSkin2; //fractal-cube
Texture mtlSkin3; //depth

sampler2D partSampler = sampler_state
{
	texture = <TargetMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler2D colorSampler = sampler_state
{
	texture = <mtlSkin1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler fracSampler = sampler_state
{
	texture 		= (mtlSkin2);
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
   AddressV = Clamp;
};

sampler depthSampler = sampler_state
{
	texture 		= (mtlSkin3);
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
   AddressV = Clamp;
};

float4 vecSkill1;
float edgeStr = 0.02;

float4 volParts_PS( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float4 Color = tex2D(colorSampler,Tex.xy/8);
	float4 Part = tex2D(partSampler,Tex.xy/8);
	float4 Frac = tex2D(fracSampler,Tex.xy/8);
	Frac.rgb = max(Frac.rgb,float3(0.5, 0.5, 1.0));
	//if(Frac.g < 0.1 && Frac.b < 0.1 && Frac.r < 0.1) Frac.rgb = float3(0.5,0.5,1.0);
	//if(Frac.g > 0.9) Frac.rgb = float3(0.5,0.5,1.0);
	
	/*
	float FracEdge = 1-tex2D(fracSampler,(Tex.xy/8)+float2(edgeStr,0)).a;
	FracEdge += 1-tex2D(fracSampler,(Tex.xy/8)+float2(-edgeStr,0)).a;
	FracEdge += 1-tex2D(fracSampler,(Tex.xy/8)+float2(0,edgeStr)).a;
	FracEdge += 1-tex2D(fracSampler,(Tex.xy/8)+float2(0,-edgeStr)).a;
	FracEdge /= 4;
	FracEdge -= 1-Frac.a;
	if(FracEdge > 0) FracEdge = 1;
	//Frac.a -= FracEdge;
	*/
	vecSkill1.x = 0.6; // ray scatter strength
	
	float depth = 1;
	//float depth = tex2D(depthSampler,Tex/2).r;
	
	//float3 bump = tex2D(fracSampler, Tex/8) * 2 - 1;
	float3 bump = Frac.rgb * 2 - 1;
	bump = normalize(bump);
	float2 mid = ((Tex/8) + bump.xy * 0.15f * depth * vecSkill1.x);
	Part = tex2D(partSampler, mid);
	
	//Part.a *= 2;
	Color.rgb *= 1-pow((Part.a*(Frac.a)),2);
	Color.rgb = lerp(Color.rgb, (Part.rgb*(Part.a*(Frac.a))), 0.5)*2;

	//Color.rgb = Part.a*2
//Color.rgb = max(Frac.rgb,float3(0.48, 0.48, 0.98));
	//Color.rgb -= Frac.a+FracEdge;
	//Color.rgb = clamp(Color.rgb,0,1);
	//Color.rgb += Part.rgb;
	//Color.ggb = clamp((FracEdge),0,1);
	
	
	
	
	
	//Color.rgb += (Part.rgb*(Part.a*Frac.a));
	//Color.rgb += (Part.rgb*(Part.a*Frac.a))*pow((bump.r+bump.g+bump.b)/3, 2);
	
	//Color.rgb = Frac.a - FracEdge;
	//Color.rgb = Part.a;
	//Color.rgb = tex2D(depthSampler,Tex/2).r;	
	//Color.rgb = tex2D(partSampler,Tex.xy/2).rgb;
	//Color.rgb = tex2D(depthSampler,Tex/2).r;

	return Color;
}

technique volParts
{
	pass one
	{
		alphablendenable = false;
		PixelShader = compile ps_2_0 volParts_PS();
	}
}