/*
MATERIAL* sc_mtl_pp_lightRayCut =
{
	effect = "sc_lightRayCut.fx";
}

MATERIAL* sc_mtl_lightRay =
{
	effect = "sc_lightRay.fx";
}

MATERIAL* sc_mtl_lightRayShow =
{
	effect = "sc_lightRayShow.fx";
}
*/

MATERIAL* sc_mtl_lightRayShow =
{
	effect = "sc_lightRayShow.fx";
}

var sc_lightRayStr = 0.8;
var sc_lightRayLength = 2.0;

PANEL* sc_pan_sunDummy =
{
	bmap = "imSun.tga";
 	//flags = SHOW;
 	scale_y = 0.5;
 	scale_x = 0.5;
   layer = 0;	
}


void sc_lightRays()
{
	wait(5);
	if(sc_bDOF == 0) sc_setupDepth();
	
	MATERIAL* sc_mtl_lightRayCut;
	sc_mtl_lightRayCut = mtl_create();
	sc_mtl_lightRayCut.effect = "sc_lightRayCut.fx";
	MATERIAL* sc_mtl_lightRay;
	sc_mtl_lightRay = mtl_create();
	sc_mtl_lightRay.effect = "sc_lightRay.fx";
	MATERIAL* sc_mtl_lightRayBlur;
	sc_mtl_lightRayBlur = mtl_create();
	sc_mtl_lightRayBlur.effect = "sc_lightRayBlur.fx";
/*
	MATERIAL* sc_mtl_lightRayShow;
	sc_mtl_lightRayShow = mtl_create();
	sc_mtl_lightRayShow.effect = "sc_lightRayShow.fx";
*/	
	VIEW* view_lightRayCut;
	view_lightRayCut = malloc(sizeof(VIEW*));
	view_lightRayCut = view_create(2);
	view_lightRayCut.size_x = screen_size.x/2;
	view_lightRayCut.size_y = screen_size.y/2;
	set(view_lightRayCut,PROCESS_TARGET);
	set(view_lightRayCut,VISIBLE);
	sc_mtl_lightRayCut.skin1 = sc_map_depth;
	sc_mtl_lightRayCut.skin2 = sc_map_scene;
	view_lightRayCut.material = sc_mtl_lightRayCut;
	view_lightRayCut.arc = camera.arc;
	
	BMAP* map_lightRayCut;
	map_lightRayCut = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	view_lightRayCut.bmap = map_lightRayCut;
	
	VIEW* view_lightRay;
	view_lightRay = malloc(sizeof(VIEW*));
	view_lightRay = view_create(2);
	view_lightRay.size_x = screen_size.x/2;
	view_lightRay.size_y = screen_size.y/2;
	set(view_lightRay,PROCESS_TARGET);
	sc_mtl_lightRay.skill1 = floatv(0.5);
	sc_mtl_lightRay.skill2 = floatv(0.5);
	sc_mtl_lightRay.skill3 = floatv(sc_lightRayLength);
	//sc_mtl_lightRay.skin1 = map_lightRayCut;
	view_lightRay.arc = camera.arc;
	view_lightRay.material = sc_mtl_lightRay;
	view_lightRayCut.stage = view_lightRay;
	
	BMAP* map_lightRay;
	map_lightRay = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	view_lightRay.bmap = map_lightRay;
	
	VIEW* view_lightRayBlur;
	view_lightRayBlur = malloc(sizeof(VIEW*));
	view_lightRayBlur = view_create(2);
	view_lightRayBlur.size_x = screen_size.x/2;
	view_lightRayBlur.size_y = screen_size.y/2;
	set(view_lightRayBlur,PROCESS_TARGET);
	sc_mtl_lightRayBlur.skill1 = floatv(0.005);
	view_lightRayBlur.arc = camera.arc;
	view_lightRayBlur.material = sc_mtl_lightRayBlur;
	view_lightRay.stage = view_lightRayBlur;
	
	BMAP* map_lightRayBlur;
	map_lightRayBlur = bmap_createblack(screen_size.x/2,screen_size.y/2,32);
	view_lightRayBlur.bmap = map_lightRayBlur;
	
	sc_ppAdd(sc_mtl_lightRayShow,camera,0);
	sc_mtl_lightRayShow.skin1 = map_lightRayBlur;
	//sc_ppAdd(sc_mtl_lightRayShow,camera,0);
	//sc_mtl_lightRayShow.skin1 = map_lightRay;

	VECTOR vTest;
	var lightAdded = 1;
	while(1)
	{
		
		sc_mtl_lightRay.skill3 = floatv(sc_lightRayLength);
		
		vec_set(vTest,sun_pos);
		//if(volEnt == NULL) vec_set(vTest,sun_pos);
		//else vec_set(vTest,volEnt.x);
		if(vec_to_screen(vTest,camera))
		{
			if(lightAdded == 0){
				sc_ppAdd(sc_mtl_lightRayShow,camera,0);
				lightAdded = 1;
			}
			
			sc_pan_sunDummy.pos_x = vTest.x;
			sc_pan_sunDummy.pos_y = vTest.y;
			
			vTest.x /= screen_size.x;
			vTest.y /= screen_size.y;
			sc_mtl_lightRay.skill1 = floatv(vTest.x);
			sc_mtl_lightRay.skill2 = floatv(vTest.y);
			
			if(vTest.x > 0.5) vTest.x = 1-vTest.x;
			if(vTest.y > 0.5) vTest.y = 1-vTest.y;
			sc_mtl_lightRayShow.skill1 = floatv(((vTest.x*(vTest.y*2))+(vTest.y*(vTest.x*2)))*sc_lightRayStr);			
			//sc_mtl_lightRay.skill3 = floatv(1.5);
		}
		else
		{
			if(lightAdded == 1)
			{
				sc_mtl_lightRayShow.skill1 = floatv(0);
				//sc_mtl_lightRay.skill3 = floatv(0);
				sc_ppRemove(sc_mtl_lightRayShow,camera,0);
				lightAdded = 0;
			}
			
			//
		} 
		wait(1);
		
	}
	/*
	while(1)
	{
		vec_set(arght2,sun_pos);
		if (vec_to_screen(arght2,camera) != NULL) // if sun position visible at screen
		{
			
			
			vec_to_screen(blargh1,camera);
			blargh1.z = 200;
			vec_set(testEnt,blargh1);
			sc_mtl_lightRay.skill1 = floatv(blargh1.x/screen_size.x);
			sc_mtl_lightRay.skill2 = floatv(blargh1.y/screen_size.y);
			sc_mtl_lightRay.skill3 = floatv(4.5);
		}
		wait(1);
	}
	*/

}