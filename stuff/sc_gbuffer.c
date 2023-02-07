MATERIAL* sc_mtl_gbuffer =
{
	effect = "sc_gbuffer.fx";
}

void sc_setupGBuffer()
{
	camera.material = sc_mtl_gbuffer;
}