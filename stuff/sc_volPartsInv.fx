float4 volPartInv():COLOR0
{
	return 0;
}

technique tvolPartInv
{
	pass p0
	{
		PixelShader = compile ps_1_0 volPartInv();
	}
}