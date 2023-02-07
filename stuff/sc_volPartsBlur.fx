Texture TargetMap; //Particles
Texture mtlSkin1; //Fractal
Texture mtlSkin2; //Depthmap

float4 vecViewPort;
float4 vecSkill1;

sampler currentScene = sampler_state
{
	texture 		= (TargetMap);
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
   AddressV = Clamp;
};

sampler fracScene = sampler_state
{
	texture 		= (mtlSkin1);
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
   AddressV = Clamp;
};

sampler depthSampler = sampler_state
{
	texture 		= (mtlSkin2);
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
   AddressV = Clamp;
};

static const int g_c_PixelOffsetSize = 13;

float2 PixelOffset_fix = {0.003, 0.003};



static const float BlurWeights[g_c_PixelOffsetSize] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};


float2 PixelOffsets[g_c_PixelOffsetSize] = {
   0.0, 0.0,
   0.0, 1.5,
   1.5, 0.0,
   1.0, 1.0,
   0.0,-1.5,
  -1.5, 0.0,
  -1.0,-1.0,
  -1.0, 1.0,
   1.0,-1.0,
   0.5, 0.5,
   0.5,-0.5,
  -0.5,-0.5,
  -0.5, 0.5,

};

float4 HorBloom_PS(float2 texcoord0 : TEXCOORD0) : COLOR
{
	vecSkill1.x = 0.0;
	float4 pixel = 0;
	
	//float depth = 1;
	float depth = 1-tex2D(depthSampler,texcoord0).r;
	
	for(int i = 0; i < g_c_PixelOffsetSize; i++)
	{
		pixel += tex2D(currentScene,(texcoord0-vecViewPort.zw+PixelOffset_fix) + PixelOffsets[i] * (vecSkill1.x));	
	}
	
	pixel /= 13;
	
	//pixel *= tex2D(fracScene,texcoord0).a;
	
	return pixel;
	//return float4(pixel,tex2D(currentScene,texcoord0).a);
	
}

technique tech_00
{
	pass pass_00
	{
		alphablendenable=false;
		Pixelshader = compile ps_2_0 HorBloom_PS();
	}
}


