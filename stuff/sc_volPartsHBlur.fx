// Textures
Texture TargetMap;
Texture sc_map_depth_bmap;

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

sampler depthSampler = sampler_state
{
	texture 		= (sc_map_depth_bmap);
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

float2 PixelOffset_fix = {0, 0.04};

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


float4 main_PS(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	vecSkill1.x = 8; //4
	float4 color = 0;
	
	float depth = 1-tex2D(depthSampler,texcoord0).r;
	
	for(int i = 0; i < g_c_PixelOffsetSize; i++)
	{
		color += 1 * tex2D(currentScene,(texcoord0-vecViewPort.zw+PixelOffset_fix) + PixelOffsets[i] * (vecSkill1.x)) * BlurWeights[i];	
	}
	
	return color;
	
}

technique HBlur
{
	pass pass1
	{
		alphablendenable=false;
		Pixelshader = compile ps_2_0 main_PS();
	}
}