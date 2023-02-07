
//rendertarget resize factor
var sc_giRT = 4;

BMAP* sc_bmap_giNoise = "sc_giNoise.bmp";

MATERIAL* sc_mtl_gi =
{
	skin1 = sc_bmap_giNoise;
	effect = "sc_gi.fx";
}

MATERIAL* sc_mtl_giBump =
{
	skin1 = sc_bmap_giNoise;
	effect = "sc_giBump.fx";
}

MATERIAL* sc_mtl_giHBlur =
{
	effect = "sc_giHBlur.fx";
}


MATERIAL* sc_mtl_giVBlur =
{
	effect = "sc_giVBlur.fx";
}

MATERIAL* sc_mtl_giCombine =
{
	effect = "sc_giCombine.fx";
}


VIEW* sc_view_giNormals = {material = sc_mtl_giBump; layer = -20;}



BMAP* sc_bmap_giNormals;
BMAP* sc_bmap_gi;
void sc_setupGI()
{
	
	set(sc_view_giNormals,VISIBLE);
	sc_bmap_giNormals = bmap_createblack(screen_size.x/sc_giRT,screen_size.y/sc_giRT,14444);
	sc_view_giNormals.size_x = screen_size.x/sc_giRT;
	sc_view_giNormals.size_y = screen_size.y/sc_giRT;
	sc_view_giNormals.bmap = sc_bmap_giNormals;
	
	
	sc_bmap_gi = bmap_createblack(screen_size.x/sc_giRT,screen_size.y/sc_giRT,32);
	sc_bmap_scene = bmap_createblack(screen_size.x, screen_size.y,32);
	camera.bmap = sc_bmap_scene;
	sc_mtl_gi.skin2 = sc_bmap_depth;
	sc_mtl_gi.skin3 = sc_bmap_scene;
	sc_ppAdd(sc_mtl_gi,sc_view_giNormals,bmap_createblack(screen_size.x/sc_giRT,screen_size.y/sc_giRT,32));
	//sc_ppAdd(sc_mtl_gi,sc_view_giNormals,sc_bmap_gi);
	
	
	
	sc_mtl_giHBlur.skill1 = floatv(4);
	sc_mtl_giHBlur.skin1 = sc_bmap_depth;
	sc_mtl_giVBlur.skill1 = floatv(4);
	sc_mtl_giVBlur.skin1 = sc_bmap_depth;
	sc_ppAdd(sc_mtl_giHBlur,sc_view_giNormals,bmap_createblack(screen_size.x/sc_giRT,screen_size.y/sc_giRT,32));
	sc_ppAdd(sc_mtl_giVBlur,sc_view_giNormals,bmap_createblack(screen_size.x/sc_giRT,screen_size.y/sc_giRT,32));
	sc_ppAdd(sc_mtl_giHBlur,sc_view_giNormals,bmap_createblack(screen_size.x/sc_giRT,screen_size.y/sc_giRT,32));
	sc_ppAdd(sc_mtl_giVBlur,sc_view_giNormals,sc_bmap_gi);
	
	
	sc_mtl_giCombine.skin1 = sc_bmap_gi; 
	sc_ppAdd(sc_mtl_giCombine,camera,0);
	
	
	/*
	set(sc_view_giBump,VISIBLE);
	sc_bmap_giBump = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	sc_view_giBump.size_x = screen_size.x/2;
	sc_view_giBump.size_y = screen_size.y/2;
	sc_view_giBump.bmap = sc_bmap_giBump;
	sc_mtl_gi.skin2 = sc_bmap_depth;
	sc_mtl_gi.skin3 = sc_bmap_giBump;
	sc_ppAdd(sc_mtl_gi,camera,0);
	
	sc_mtl_pp_HBlur.skill1 = floatv(1);
	sc_mtl_pp_VBlur.skill1 = floatv(1);
	sc_ppAdd(sc_mtl_pp_HBlur,camera,0);
	sc_ppAdd(sc_mtl_pp_VBlur,camera,0);
	
	sc_bmap_scene = bmap_createblack(screen_size.x, screen_size.y,32);
	camera.bmap = sc_bmap_scene;
	sc_mtl_giCombine.skin1 = sc_bmap_scene;
	sc_ppAdd(sc_mtl_giCombine,camera,0);
	*/
	
	proc_mode = PROC_EARLY;
	
	while(1)
	{
		vec_set(sc_view_giNormals.x, camera.x);
		vec_set(sc_view_giNormals.pan, camera.pan);
		sc_view_giNormals.arc = camera.arc;
		wait(1);
	}
}