// Textures
Texture TargetMap;

// Variables
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

static const int g_c_PixelOffsetSize = 13;

float2 PixelOffsets[g_c_PixelOffsetSize] =
{
	{ 0, -0.006 },
	{ 0, -0.005 },
	{ 0, -0.004 },
	{ 0, -0.003 },
	{ 0, -0.002 },
	{ 0, -0.001 },
	{ 0,  0.000 },
	{ 0,  0.001 },
	{ 0,  0.002 },	
	{ 0,  0.003 },
	{ 0,  0.004 },
	{ 0,  0.005 },
	{ 0,  0.006 },
};


float2 PixelOffset_fix = {0.0, 0.002};

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

float4 dofHBlur_PS(float2 texcoord0 : TEXCOORD0) : COLOR
{

	float4 pixel = 0;
	float3 pixel2 = 0;

	for(int i = 0; i < g_c_PixelOffsetSize; i++)
	{
		pixel += 1 * tex2D(currentScene,(texcoord0-vecViewPort.zw+PixelOffset_fix) + PixelOffsets[i] * vecSkill1.x) * BlurWeights[i];	
	}
	
	
	pixel2.rgb = pixel.a*3;
	pixel2.rgb = clamp(pixel2.rgb,0,1);
	pixel.rgb -= pixel2.rgb*pixel.rgb;
	pixel2.rgb *= tex2D(currentScene,(texcoord0-vecViewPort.zw+PixelOffset_fix)).rgb;	
	pixel.rgb += pixel2.rgb;
	
	
	return pixel;
	
}

technique HBlur
{
	pass pass_00
	{
		alphablendenable=false;
		Pixelshader = compile ps_2_0 dofHBlur_PS();
	}
}