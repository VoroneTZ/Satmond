#define SC_SSAO
BMAP* sc_bmap_ssaoNoise = "sc_ssaoNoise.bmp";
BMAP* sc_bmap_ssaoNoise2 = "sc_ssaoNoise3.bmp";

MATERIAL* sc_mtl_ssaoNew =
{
	skin1 = sc_bmap_ssaoNoise;
	effect = "sc_ssaoNew.fx";
}

MATERIAL* sc_mtl_ssaoVBlur =
{
	effect = "sc_ssaoVBlur.fx";
}

MATERIAL* sc_mtl_ssaoHBlur =
{
	effect = "sc_ssaoHBlur.fx";
}

MATERIAL* sc_mtl_ssaoCombine =
{
	effect = "sc_ssaoCombine.fx";
}

MATERIAL* sc_mtl_ssaoNormals = 
{
	effect = "sc_ssaoNormals.fx";
}

BMAP* sc_bmap_normals;
BMAP* sc_bmap_ssao;

VIEW* sc_view_ssaoNormals = {flags = VISIBLE; material = sc_mtl_ssaoNormals; layer = 5;}


void sc_ssao()
{
	sc_bmap_normals = bmap_createblack(screen_size.x, screen_size.y, 32);
	sc_bmap_ssao = bmap_createblack(screen_size.x, screen_size.y, 32);
	sc_view_ssaoNormals.bmap = sc_bmap_normals;
	
	sc_ppAdd(sc_mtl_ssaoNew,sc_view_ssaoNormals,0);
	sc_mtl_ssaoNew.skin2 = sc_bmap_depth;
	sc_mtl_ssaoNew.skin3 = sc_bmap_normals;
	
	//sc_ppAdd(sc_mtl_ssaoVBlur,sc_view_ssaoNormals,0);
	//sc_ppAdd(sc_mtl_ssaoHBlur,sc_view_ssaoNormals,0);
	sc_ppAdd(sc_mtl_ssaoVBlur,sc_view_ssaoNormals,0);
	sc_ppAdd(sc_mtl_ssaoHBlur,sc_view_ssaoNormals,sc_bmap_ssao);
	
	sc_mtl_ssaoCombine.skin1 = sc_bmap_ssao;
	sc_ppAdd(sc_mtl_ssaoCombine,camera,0);
}
