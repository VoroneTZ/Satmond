void sc_volParts()
{
	set(sc_view_volParts, VISIBLE);
	set(sc_view_volParts2, VISIBLE);
	
	sc_view_volParts.stage = sc_view_volParts2;
	sc_view_volParts2.stage = sc_view_volPartsHBlur;
	//sc_view_volPartsBlur.stage = sc_view_volPartsHBlur;
	sc_view_volPartsHBlur.stage = sc_view_volPartsVBlur;
	sc_view_volPartsVBlur.stage = sc_view_volPartsCombine;

/*
	sc_bmap_volPartsFractal = bmap_createblack(screen_size.x,screen_size.y,32);
	sc_view_volParts.bmap = sc_bmap_volPartsFractal;
	sc_view_volParts2.bmap = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	sc_view_volPartsBlur.bmap = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	sc_view_volPartsHBlur.bmap = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	sc_view_volPartsVBlur.bmap = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
*/	
	sc_bmap_volPartsFractal = bmap_createblack(screen_size.x/8,screen_size.y/8,32);
	sc_view_volParts.bmap = sc_bmap_volPartsFractal;
	sc_view_volParts2.bmap = bmap_createblack(screen_size.x/8,screen_size.y/8,32);
	//sc_view_volPartsBlur.bmap = bmap_createblack(screen_size.x/8,screen_size.y/8,32);
	sc_view_volPartsHBlur.bmap = bmap_createblack(screen_size.x/8,screen_size.y/8,32);
	sc_view_volPartsVBlur.bmap = bmap_createblack(screen_size.x/8,screen_size.y/8,32);
	sc_bmap_volParts = bmap_createblack(screen_size.x,screen_size.y,32);
	
	#ifdef SC_HDR
		if(sc_bHDR) sc_view_volPartsCombine.bmap = sc_bmap_volParts;
	#endif
	#else
		#ifdef SC_DOF
			if(sc_bDOF)	sc_view_volPartsCombine.bmap = sc_bmap_volParts;
		#endif
	#endif

	sc_mtl_volPartsBlur.skin2 = sc_map_depth;
	sc_mtl_volPartsCombine.skin2 = sc_bmap_volPartsFractal;
	sc_mtl_volPartsCombine.skin3 = sc_map_depth;
}

void sc_volPart(ENTITY* ent)
{
	ent.material = sc_mtl_volPartsInv;
	sc_skill(ent, sc_myVolPart, 1);
}