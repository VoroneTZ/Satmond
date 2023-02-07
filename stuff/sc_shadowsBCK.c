/*
void sc_setupShadows()
{
	vec_set(sc_view_shadowDepth.x, camera.x);
	vec_set(sc_view_shadowDepth.pan, camera.pan);
	set(sc_view_shadowDepth,VISIBLE);
	sc_view_shadowDepth.bmap = sc_bmap_shadowDepth;
	
	
	set(sc_view_shadow,VISIBLE);
	sc_mtl_shadow.skin1 = sc_bmap_scene;
	sc_mtl_shadow.skin2 = sc_bmap_shadowDepth;
	sc_mtl_shadow.skin3 = sc_bmap_shadowAlpha;
	sc_view_shadow.bmap = sc_bmap_shadow;
	
	sc_ppAdd(sc_mtl_pp_multiply,camera,0);
	sc_mtl_pp_multiply.skin1 = sc_bmap_shadow;
		
	
	while(0)
	{
		vec_set(sc_view_shadow.x, camera.x);
		vec_set(sc_view_shadow.pan, camera.pan);
		
		if(key_a) sc_view_shadowDepth.pan += 1;
		if(key_d) sc_view_shadowDepth.pan -= 1;
		wait(1);
	}
	
}
*/

//BMAP* testCube = "sky04+6.tga";
//BMAP* testCube = "shadowTest.bmp";
BMAP* sc_bmap_shadowDepthCube = "shadowTest.bmp";

void sc_setupDepthCube()
{
	while(!camera) wait(1);
	//vec_set(lightPos.x, vector(0,0,0));
	vec_set(lightPos.x, vector(0,70,0));
	
	sc_bmap_shadowDepthCube = bmap_createblack(sc_shadowDepthSize*6,sc_shadowDepthSize,12222);
	//bmap_to_format(sc_bmap_shadowDepthCube,14);
	//bmap_to_format(sc_bmap_shadowDepthCube,12222);
	bmap_to_cubemap(sc_bmap_shadowDepthCube);
	wait(1);
	
	
	/*
	set(sc_view_shadowDepth1,VISIBLE);
	sc_view_shadowDepth1.arc = 90;
	vec_set(sc_view_shadowDepth1.x,lightPos.x);
	sc_view_shadowDepth1.pan = 180;
	sc_view_shadowDepth1.tilt = 0;
	sc_view_shadowDepth1.pos_x = 0;
	
	set(sc_view_shadowDepth2,VISIBLE);
	sc_view_shadowDepth2.arc = 90;
	vec_set(sc_view_shadowDepth2.x,lightPos.x);
	sc_view_shadowDepth2.pan = 90;
	sc_view_shadowDepth2.tilt = 0;
	sc_view_shadowDepth2.pos_x = sc_shadowDepthSize;
	
	set(sc_view_shadowDepth3,VISIBLE);
	sc_view_shadowDepth3.arc = 90;
	vec_set(sc_view_shadowDepth3.x,lightPos.x);
	sc_view_shadowDepth3.pan = 0;
	sc_view_shadowDepth3.tilt = 0;
	sc_view_shadowDepth3.pos_x = 2*sc_shadowDepthSize;
	
	set(sc_view_shadowDepth4,VISIBLE);
	sc_view_shadowDepth4.arc = 90;
	vec_set(sc_view_shadowDepth4.x,lightPos.x);
	sc_view_shadowDepth4.pan = 270;
	sc_view_shadowDepth4.tilt = 0;
	sc_view_shadowDepth4.pos_x = 3*sc_shadowDepthSize;

	set(sc_view_shadowDepth5,VISIBLE);
	sc_view_shadowDepth5.arc = 90;
	vec_set(sc_view_shadowDepth5.x,lightPos.x);
	sc_view_shadowDepth5.pan = 90;
	sc_view_shadowDepth5.tilt = -90;
	sc_view_shadowDepth5.pos_x = 4*sc_shadowDepthSize;
	
	set(sc_view_shadowDepth6,VISIBLE);
	sc_view_shadowDepth6.arc = 90;
	vec_set(sc_view_shadowDepth6.x,lightPos.x);
	sc_view_shadowDepth6.pan = 90;
	sc_view_shadowDepth6.tilt = 90;
	sc_view_shadowDepth6.pos_x = 5*sc_shadowDepthSize;
	*/
	
	
	set(sc_view_shadowDepth1,VISIBLE);
	sc_view_shadowDepth1.arc = 90;
	view_to_light(sc_view_shadowDepth1,1,NULL);
	sc_view_shadowDepth1.pan = 180;
	sc_view_shadowDepth1.tilt = 0;
	sc_view_shadowDepth1.pos_x = 0;

	set(sc_view_shadowDepth2,VISIBLE);
	sc_view_shadowDepth2.arc = 90;
	vec_set(sc_view_shadowDepth2.x,sc_view_shadowDepth1.x);
	sc_view_shadowDepth2.pan = 90;
	sc_view_shadowDepth2.tilt = 0;
	sc_view_shadowDepth2.pos_x = sc_shadowDepthSize;
	
	set(sc_view_shadowDepth3,VISIBLE);
	sc_view_shadowDepth3.arc = 90;
	vec_set(sc_view_shadowDepth3.x,sc_view_shadowDepth1.x);
	sc_view_shadowDepth3.pan = 0;
	sc_view_shadowDepth3.tilt = 0;
	sc_view_shadowDepth3.pos_x = 2*sc_shadowDepthSize;
	
	set(sc_view_shadowDepth4,VISIBLE);
	sc_view_shadowDepth4.arc = 90;
	vec_set(sc_view_shadowDepth4.x,sc_view_shadowDepth1.x);
	sc_view_shadowDepth4.pan = 270;
	sc_view_shadowDepth4.tilt = 0;
	sc_view_shadowDepth4.pos_x = 3*sc_shadowDepthSize;

	set(sc_view_shadowDepth5,VISIBLE);
	sc_view_shadowDepth5.arc = 90;
	vec_set(sc_view_shadowDepth5.x,sc_view_shadowDepth1.x);
	sc_view_shadowDepth5.pan = 90;
	sc_view_shadowDepth5.tilt = -90;
	sc_view_shadowDepth5.pos_x = 4*sc_shadowDepthSize;
	
	set(sc_view_shadowDepth6,VISIBLE);
	sc_view_shadowDepth6.arc = 90;
	vec_set(sc_view_shadowDepth6.x,sc_view_shadowDepth1.x);
	sc_view_shadowDepth6.pan = 90;
	sc_view_shadowDepth6.tilt = 90;
	sc_view_shadowDepth6.pos_x = 5*sc_shadowDepthSize;
	
	

	sc_view_shadowDepth1.bmap = sc_bmap_shadowDepthCube;
	sc_view_shadowDepth2.bmap = sc_bmap_shadowDepthCube;
	sc_view_shadowDepth3.bmap = sc_bmap_shadowDepthCube;
	sc_view_shadowDepth4.bmap = sc_bmap_shadowDepthCube;
	sc_view_shadowDepth5.bmap = sc_bmap_shadowDepthCube;
	sc_view_shadowDepth6.bmap = sc_bmap_shadowDepthCube;
	
	sc_mtl_shadow.skin1 = sc_bmap_shadowDepthCube;
	bmap_to_cubemap(sc_mtl_shadow.skin1);
	set(sc_view_shadow,VISIBLE);
	sc_view_shadow.bmap = sc_bmap_shadow;
	
	
	//blur shadow
	sc_mtl_pp_VBlur.skill1 = floatv(2);
	sc_mtl_pp_HBlur.skill1 = floatv(2);
	//sc_ppAdd(sc_mtl_pp_VBlur,sc_view_shadow,0);
	//sc_ppAdd(sc_mtl_pp_HBlur,sc_view_shadow,0);
	//sc_ppAdd(sc_mtl_pp_VBlur,sc_view_shadow,0);
	//sc_ppAdd(sc_mtl_pp_HBlur,sc_view_shadow,sc_bmap_shadow);
	
	sc_mtl_pp_multiply.skin1 = sc_bmap_shadow;
	sc_ppAdd(sc_mtl_pp_multiply,camera,0);
	
	while(1)
	{
		vec_set(sc_view_shadow.x, camera.x);
		vec_set(sc_view_shadow.pan, camera.pan);
		
		
		view_to_light(sc_view_shadowDepth1,1,NULL);
		vec_set(sc_view_shadowDepth2.x,sc_view_shadowDepth1.x);
		vec_set(sc_view_shadowDepth3.x,sc_view_shadowDepth1.x);
		vec_set(sc_view_shadowDepth4.x,sc_view_shadowDepth1.x);
		vec_set(sc_view_shadowDepth5.x,sc_view_shadowDepth1.x);
		vec_set(sc_view_shadowDepth6.x,sc_view_shadowDepth1.x);
		
		wait(1);
	}
	
	
	
	//sc_mtl_pp_add.skin1 = sc_bmap_shadowDepthCube;
	//sc_ppAdd(sc_mtl_pp_add,camera,0);
	
}