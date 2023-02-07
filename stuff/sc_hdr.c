void sc_hdr()
{
	#ifdef SC_DOF
	if(sc_bDOF == 1)
	{
		sc_view_dof.stage = sc_view_hdrDownsample;
	}
	else camera.stage = sc_view_hdrDownsample;
	#endif
	#ifndef SC_DOF
		camera.stage = sc_view_hdrDownsample;
	#endif
	
	sc_view_hdrDownsample.stage = sc_view_hdrHighpass;
	sc_view_hdrHighpass.stage = sc_view_hdrBlur;
	sc_view_hdrBlur.stage = sc_view_hdrHBlur;
	sc_view_hdrHBlur.stage = sc_view_hdrVBlur;
	sc_view_hdrVBlur.stage = sc_view_hdr;
}

void sc_setupHDR()
{
	set(sc_view_hdrGamma,VISIBLE);
	
	//SET s_rtScale
	sc_mtl_hdr.skill4 = floatv(sc_hdrRT);
	
	//SET DOWNSAMPLE FACTOR
	sc_mtl_hdrDownsample.skill1 = floatv(sc_hdrRT);
	
	//SET HDR BlURSTRENGTH + HIGHPASS + EXPOSURE
	sc_mtl_hdrBlur.skill1 = floatv(sc_hdrParams[0]);
	sc_mtl_hdrHBlur.skill1 = floatv(sc_hdrParams[1]);
	sc_mtl_hdrVBlur.skill1 = floatv(sc_hdrParams[1]);
	sc_mtl_hdrHighpass.skill1 = floatv(sc_hdrParams[2]);
	sc_mtl_hdrHighpass.skill2 = floatv(sc_hdrParams[3]);
	sc_mtl_hdrHighpass.skill3 = floatv(sc_hdrParams[4]);
	sc_mtl_hdrHighpass.skill4 = floatv(sc_hdrParams[5]);
	sc_mtl_hdrGamma3.skill1 = floatv(sc_hdrParams[6]);
	sc_mtl_hdr.skill1 = floatv(sc_hdrParams[3]);
	
	//SET VIEW DIMENSIONS AND RTs
	sc_view_hdrDownsample.arc = camera.arc;
	sc_view_hdrDownsample.size_x = screen_size.x/sc_hdrRT;
	sc_view_hdrDownsample.size_y = screen_size.y/sc_hdrRT;
	sc_view_hdrDownsample.bmap = bmap_createblack(screen_size.x/sc_hdrRT,screen_size.y/sc_hdrRT,sc_hdrBits);
	sc_view_hdrHighpass.arc = camera.arc;
	sc_view_hdrHighpass.size_x = screen_size.x/sc_hdrRT;
	sc_view_hdrHighpass.size_y = screen_size.y/sc_hdrRT;
	sc_view_hdrHighpass.bmap = bmap_createblack(screen_size.x/sc_hdrRT,screen_size.y/sc_hdrRT,sc_hdrBits);
	sc_view_hdrBlur.arc = camera.arc;
	sc_view_hdrBlur.size_x = screen_size.x/sc_hdrRT;
	sc_view_hdrBlur.size_y = screen_size.y/sc_hdrRT;
	sc_view_hdrBlur.bmap = bmap_createblack(screen_size.x/sc_hdrRT,screen_size.y/sc_hdrRT,sc_hdrBits);
	sc_view_hdrHBlur.arc = camera.arc;
	sc_view_hdrHBlur.size_x = screen_size.x/sc_hdrRT;
	sc_view_hdrHBlur.size_y = screen_size.y/sc_hdrRT;
	sc_view_hdrHBlur.bmap = bmap_createblack(screen_size.x/sc_hdrRT,screen_size.y/sc_hdrRT,sc_hdrBits);
	sc_view_hdrVBlur.arc = camera.arc;
	sc_view_hdrVBlur.size_x = screen_size.x/sc_hdrRT;
	sc_view_hdrVBlur.size_y = screen_size.y/sc_hdrRT;
	sc_view_hdrVBlur.bmap = bmap_createblack(screen_size.x/sc_hdrRT,screen_size.y/sc_hdrRT,sc_hdrBits);
	sc_view_hdr.arc = camera.arc;
	sc_view_hdr.size_x = screen_size.x;
	sc_view_hdr.size_y = screen_size.y;
	
	//SET DYNAMIC EXPOSURE CONTROL STAGES
	sc_view_hdrGamma.stage = sc_view_hdrGamma2;
	sc_view_hdrGamma2.stage = sc_view_hdrGamma3;
	sc_view_hdrGamma3.stage = sc_view_hdrGamma4;
	sc_view_hdrGamma4.bmap = sc_bmap_gamma;
	
	//SET MATERIAL SKINS
	sc_mtl_hdr.skin1 = sc_map_scene;
	#ifdef SC_DOF
	if(sc_bDOF == 1)
	{
		sc_mtl_hdr.skin1 = sc_bmap_dof;
	}
	#endif
	sc_mtl_hdr.skin2 = sc_bmap_gamma;
	sc_mtl_hdrHighpass.skin1 = sc_bmap_gamma;
	sc_mtl_hdrGamma.skin1 = sc_map_scene;
	sc_mtl_hdrGamma.skin2 = sc_bmap_gamma;
}