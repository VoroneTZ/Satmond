typedef struct
{
	BMAP* col;
	BMAP* norm;
	BMAP* pos;
}SC_GBUFFER;

SC_GBUFFER sc_bmap_gBuffer;

//BMAP* sc_bmap_defEnvMap = "nebulacube1S2+6.tga";
BMAP* sc_bmap_defEnvMap = "nebula02S+6.tga";
BMAP* sc_bmap_defNoise = "sc_defNoise.bmp";
BMAP* sc_bmap_ssao;

/*
void sc_mtlEvent_gBuffer()
{
	if(mtl.skin1) bmap_to_cubemap(mtl.skin1);
	
	mtl.skill5 = 0;
	mtl.skill6 = 0;
	mtl.skill7 = 0;
	
	mtl.skill9 = 0;
	mtl.skill10 = 0;
	mtl.skill11 = 0;
	if(my)
	{
		if(my.skill99)
		{
			SC_OBJECT* ObjData = (SC_OBJECT*)(my.skill99);
			
			if(ObjData.myTeam){
			mtl.skill5 = floatv(ObjData.myTeam.x);
			mtl.skill6 = floatv(ObjData.myTeam.y);
			mtl.skill7 = floatv(ObjData.myTeam.z);
			}
			
			if(ObjData.myLuminance){
			mtl.skill9 = floatv(ObjData.myLuminance.x);
			mtl.skill10 = floatv(ObjData.myLuminance.y);
			mtl.skill11 = floatv(ObjData.myLuminance.z);
			}
		}
	}
}
*/

void sc_mtlEvent_gBuffer2(){
	mtl.skill1 = floatv(sc_dofParams[0]);
	mtl.skill2 = floatv(sc_dofParams[1]);
	mtl.skill3 = floatv(0);
	mtl.skill4 = floatv(sc_maxDepth);
	if(my != 0){
		if(my.skill99 != 0){
			
			SC_OBJECT* ObjData = (SC_OBJECT*)(my.skill99);
			
			mtl.skill3 = floatv(ObjData.myDepth);
			mtl.skill7 = floatv(ObjData.myVolPart);
		}
	}
}

MATERIAL* sc_mtl_gBuffer =
{
	effect = "sc_gBufferSpace.fx";
	flags = ENABLE_RENDER;
	event = sc_mtlEvent_gBuffer2;
}

MATERIAL* sc_mtl_deferred =
{
	//skin4 = sc_bmap_defEnvMap;
	skin4 = sc_bmap_defEnvMap;
	effect = "sc_deferredSpace.fx";
}

MATERIAL* sc_mtl_defSSAODown =
{
	//skin4 = sc_bmap_defEnvMap;
	skin4 = sc_bmap_defNoise;
	effect = "sc_defSSAODownSpace.fx";
}

MATERIAL* sc_mtl_debug =
{
	effect = "sc_showTM.fx";
}

MATERIAL* sc_mtl_defMultiply =
{
	effect = "sc_defMultiply.fx";
}

VIEW* sc_view_deferred = {
	layer = 3;
	flags = PROCESS_TARGET;
	material = sc_mtl_deferred;
}

VIEW* sc_view_defSSAODown = {
	layer = -4;
	flags = PROCESS_TARGET;
	material = sc_mtl_defSSAODown;
}


void sc_setupMRT()
{
	
	sc_bmap_gBuffer.col = bmap_createblack(screen_size.x,screen_size.y,32);
	sc_bmap_gBuffer.norm = bmap_createblack(screen_size.x,screen_size.y,32);
	sc_bmap_gBuffer.pos = bmap_createblack(screen_size.x,screen_size.y,14);
	
	camera.material = sc_mtl_gBuffer;
	
	camera.bmap = sc_bmap_gBuffer.col;
	bmap_rendertarget(sc_bmap_gBuffer.norm,1,1);
	bmap_rendertarget(sc_bmap_gBuffer.pos,2,1);
	
	//set(camera,INVISIBLE);
	set(sc_view_deferred,VISIBLE);
	camera.stage = sc_view_deferred;
	
	
	sc_mtl_deferred.skin1 = sc_bmap_gBuffer.col;
	sc_mtl_deferred.skin2 = sc_bmap_gBuffer.norm;
	sc_mtl_deferred.skin3 = sc_bmap_gBuffer.pos;
	
	/*
	var sc_ssaoRT = 1;
	//set deferred ssao downsample view dimensions and textures
	set(sc_view_defSSAODown,VISIBLE);
	sc_view_defSSAODown.size_x = screen_size.x / sc_ssaoRT;
	sc_view_defSSAODown.size_y = screen_size.y / sc_ssaoRT;
	sc_mtl_defSSAODown.skin1 = sc_bmap_gBuffer.col;
	sc_mtl_defSSAODown.skin2 = sc_bmap_gBuffer.norm;
	sc_mtl_defSSAODown.skin3 = sc_bmap_gBuffer.pos;
	
	sc_bmap_ssao = bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32);//bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32);
	sc_mtl_pp_HBlur.skill1 = floatv(1);
	//sc_mtl_giHBlur.skin1 = sc_bmap_gBuffer.pos;
	sc_mtl_pp_VBlur.skill1 = floatv(1);
	//sc_mtl_giVBlur.skin1 = sc_bmap_gBuffer.pos;
	//sc_ppAdd(sc_mtl_giHBlur,sc_view_defSSAODown,bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32));
	//sc_ppAdd(sc_mtl_giVBlur,sc_view_defSSAODown,bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32));
	sc_ppAdd(sc_mtl_pp_HBlur,sc_view_defSSAODown,bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32));
	sc_ppAdd(sc_mtl_pp_VBlur,sc_view_defSSAODown,sc_bmap_ssao);
	//sc_ppAdd(sc_mtl_debug,sc_view_defSSAODown,sc_bmap_ssao);
	
	sc_ppAdd(sc_mtl_defMultiply, sc_view_deferred,0);
	sc_mtl_defMultiply.skin1 = sc_bmap_ssao;
*/
	//sc_mtl_deferred.skin4 = sc_bmap_ssao;
	
	while(1)
	{
		proc_mode = PROC_LATE;
		
		mat_set(sc_mtl_deferred.matrix,matWorld);
		mat_multiply(sc_mtl_deferred.matrix,matView);
		mat_multiply(sc_mtl_deferred.matrix,matProj);
		mat_inverse(sc_mtl_deferred.matrix,sc_mtl_deferred.matrix);
		
		/*mat_set(sc_mtl_defSSAODown.matrix,matWorld);
		mat_multiply(sc_mtl_defSSAODown.matrix,matView);
		mat_multiply(sc_mtl_defSSAODown.matrix,matProj);
		mat_inverse(sc_mtl_defSSAODown.matrix,sc_mtl_deferred.matrix);
		*/
		wait(1);
	}
}

void sc_setupMRT2()
{
	
	sc_bmap_gBuffer.col = bmap_createblack(screen_size.x,screen_size.y,32);
	sc_bmap_gBuffer.norm = bmap_createblack(screen_size.x,screen_size.y,32);
	sc_bmap_gBuffer.pos = bmap_createblack(screen_size.x,screen_size.y,14);
	
	camera.material = sc_mtl_gBuffer;
	
	camera.bmap = sc_bmap_gBuffer.col;
	bmap_rendertarget(sc_bmap_gBuffer.norm,1,1);
	bmap_rendertarget(sc_bmap_gBuffer.pos,2,1);
	
	//set(camera,INVISIBLE);
	set(sc_view_deferred,VISIBLE);
	camera.stage = sc_view_deferred;
	
	
	sc_mtl_deferred.skin1 = sc_bmap_gBuffer.col;
	sc_mtl_deferred.skin2 = sc_bmap_gBuffer.norm;
	sc_mtl_deferred.skin3 = sc_bmap_gBuffer.pos;
	
	
	
	var sc_ssaoRT = 1;
	//set deferred ssao downsample view dimensions and textures
	set(sc_view_defSSAODown,VISIBLE);
	sc_view_defSSAODown.size_x = screen_size.x / sc_ssaoRT;
	sc_view_defSSAODown.size_y = screen_size.y / sc_ssaoRT;
	sc_mtl_defSSAODown.skin1 = sc_bmap_gBuffer.col;
	sc_mtl_defSSAODown.skin2 = sc_bmap_gBuffer.norm;
	sc_mtl_defSSAODown.skin3 = sc_bmap_gBuffer.pos;
	
	sc_bmap_ssao = bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32);//bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32);
	sc_mtl_pp_HBlur.skill1 = floatv(1);
	//sc_mtl_giHBlur.skin1 = sc_bmap_gBuffer.pos;
	sc_mtl_pp_VBlur.skill1 = floatv(1);
	//sc_mtl_giVBlur.skin1 = sc_bmap_gBuffer.pos;
	//sc_ppAdd(sc_mtl_giHBlur,sc_view_defSSAODown,bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32));
	//sc_ppAdd(sc_mtl_giVBlur,sc_view_defSSAODown,bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32));
	sc_ppAdd(sc_mtl_pp_HBlur,sc_view_defSSAODown,bmap_createblack(screen_size.x/sc_ssaoRT,screen_size.y/sc_ssaoRT,32));
	sc_ppAdd(sc_mtl_pp_VBlur,sc_view_defSSAODown,sc_bmap_ssao);
	//sc_ppAdd(sc_mtl_debug,sc_view_defSSAODown,sc_bmap_ssao);
	
	
	
	
	
	
	sc_ppAdd(sc_mtl_defMultiply, sc_view_deferred,0);
	sc_mtl_defMultiply.skin1 = sc_bmap_ssao;

	//sc_mtl_deferred.skin4 = sc_bmap_ssao;
	
	
	
	while(1)
	{
		proc_mode = PROC_LATE;
		
		mat_set(sc_mtl_deferred.matrix,matWorld);
		mat_multiply(sc_mtl_deferred.matrix,matView);
		mat_multiply(sc_mtl_deferred.matrix,matProj);
		mat_inverse(sc_mtl_deferred.matrix,sc_mtl_deferred.matrix);
		
		/*
		mat_set(sc_mtl_defSSAODown.matrix,matWorld);
		mat_multiply(sc_mtl_defSSAODown.matrix,matView);
		mat_multiply(sc_mtl_defSSAODown.matrix,matProj);
		mat_inverse(sc_mtl_defSSAODown.matrix,sc_mtl_deferred.matrix);
		*/
		wait(1);
	}
}