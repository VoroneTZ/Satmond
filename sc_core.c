#include "sc_core.h";

//Module header files
#include "sc_volPart.h";
#include "sc_hdr.h";
#include "sc_dof.h";
#include "sc_refract.h";
#include "sc_reflect.h";
#include "sc_water.h";
#include "sc_objShaders.h";
#include "sc_pp.h";
#include "sc_shadows.h";
//#include "sc_ssao.h";
//#include "sc_gi.h";

//Modules
#include "sc_volPart.c";
#include "sc_hdr.c";
#include "sc_dof.c";
#include "sc_refract.c";
#include "sc_reflect.c";
#include "sc_water.c";
#include "sc_objShaders.c";
#include "sc_pp.c";
#include "sc_shadows.c";
#include "sc_lightRay.c";
//#include "sc_ssao.c";
//#include "sc_ssao.h";
//#include "sc_gi.h";
//#include "sc_gbuffer.c";
//#include "sc_deferred.c";


void sc_setup(){
	//proc_kill(4);
	
	//reset shade-c...
	//(this is far from perfect, but it works kinda nice)
		var screenRes = 0;
		screenRes = screen_size.x;
		wait(1);
		video_set(screenRes-1,0,0,0);
		sc_scSetup = 0;
		wait(1);
		sc_scSetup = 1;
		video_set(screenRes,0,0,0);
	//
	
	
	
	//...and then set it up
	
	//set black background
	sky_color.red = 1;
	sky_color.green = 0;
	sky_color.blue = 0;
	//
	
	//RT Scene
	#ifdef SC_SCENERT
		sc_map_scene = bmap_createblack(screen_size.x, screen_size.y,32);
		camera.bmap = sc_map_scene;
		//camera.stage = sc_view_showScene;
		//sc_mtl_showScene.skin1 = sc_map_scene;
	#endif
	
	//DoF
	#ifdef SC_DOF
	if(sc_bDOF)
	{
		sc_setupDepth();
		if(sc_bDOF){
			sc_setupDOF();
			sc_dof();
		}
	}
	#endif
	//
		
	//HDR
	#ifdef SC_HDR
	if(sc_bHDR == 1)
	{
		sc_setupHDR();
		sc_hdr();
	}
	#endif
	//
	
	//Refraction
	#ifdef SC_REFRACT
	if(sc_bRefract == 1)
	{
		sc_refract();
		#ifdef SC_OBJSHADERS
			sc_mtl_obj_heatHaze.skin1 = sc_map_refract;
			sc_mtl_obj_glass.skin1 = sc_map_refract;
		#endif
	}
	#endif
	//
	
	//Reflection
	#ifdef SC_REFLECT
	if(sc_bReflect == 1)
	{
		sc_reflect();
	}
	#endif
	//
	
	//Volumetric Particles
	#ifdef SC_VOLPARTS
		if(sc_bVolParts == 1)
		{
		
		sc_volParts();
		sc_mtl_volPartsCombine.skin1 = sc_map_scene;
		#ifdef SC_DOF
		sc_mtl_dof.skin1 = sc_bmap_volParts;
		sc_mtl_dofDownsample.skin2 = sc_bmap_volParts;
			#ifdef SC_HDR
			if(sc_bDOF == 0) sc_mtl_hdr.skin1 = sc_bmap_volParts;
			#endif
		#else
		#ifdef SC_HDR
		sc_mtl_hdr.skin1 = sc_bmap_volParts;
		#endif
		#endif
		}
	#endif
	//
	
	
	
	proc_mode = PROC_LATE;
	while(sc_scSetup == 1){
		
		/*
		#ifdef SC_DOF
		if(sc_bDOF)
		{
		vec_set(sc_view_depth.x, camera.x);
		vec_set(sc_view_depth.pan, camera.pan);
		sc_view_depth.arc = camera.arc;
		}
		#endif
		*/
		
		#ifdef SC_REFRACT
		if(sc_bRefract)
		{
			sc_view_refract.arc = camera.arc;
			vec_set(sc_view_refract.x,camera.x);
 			vec_set(sc_view_refract.pan,camera.pan);
		}
		#endif
		
		#ifdef SC_VOLPARTS
			if(sc_bVolParts == 1)
			{
			vec_set(sc_view_volParts.x,camera.x);
 			vec_set(sc_view_volParts.pan,camera.pan);
 			sc_view_volParts.arc = camera.arc;
 			
 			vec_set(sc_view_volParts2.x,camera.x);
 			vec_set(sc_view_volParts2.pan,camera.pan);
 			sc_view_volParts2.arc = camera.arc;
 			}
		#endif
		
		#ifdef SC_SSAO
			vec_set(sc_view_ssaoNormals.x,camera.x);
 			vec_set(sc_view_ssaoNormals.pan,camera.pan);
 			sc_view_ssaoNormals.arc = camera.arc;
		#endif
		
		#ifdef SC_HDR
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
		#endif
		//vec_set(sc_view_ssao.x, camera.x);
		//vec_set(sc_view_ssao.pan, camera.pan);
		
		wait(1);
	}
	
	sc_reset();
}



/*
void sc_resetFuncs()
{
	proc_kill((void*)sc_setup);
	proc_kill((void*)sc_setupDOF);
	proc_kill((void*)sc_dof);
	proc_kill((void*)sc_setupHDR);
	proc_kill((void*)sc_hdr);
	proc_kill((void*)sc_refract);
	proc_kill((void*)sc_reflect);
}

void sc_resetViews()
{
	//SC_DOF
	reset(sc_view_depth,VISIBLE);
	sc_view_depth.stage = NULL;
	sc_view_dofDownsample.stage = NULL;
	sc_view_dofHBlur.stage = NULL;
	sc_view_dofVBlur.stage = NULL;
	sc_view_dof.stage = NULL;
	
	//SC_HDR
	reset(sc_view_hdrGamma,VISIBLE);
	sc_view_hdrDownsample.stage = NULL;
	sc_view_hdrHighpass.stage = NULL;
	sc_view_hdrBlur.stage = NULL;
	sc_view_hdrHBlur.stage = NULL;
	sc_view_hdrVBlur.stage = NULL;
	sc_view_hdr.stage = NULL;
	sc_view_hdrGamma.stage = NULL;
	sc_view_hdrGamma2.stage = NULL;
	sc_view_hdrGamma3.stage = NULL;
	sc_view_hdrGamma4.stage = NULL;
	
	//SC_REFLECT
	reset(sc_view_reflect,VISIBLE);
	sc_view_reflect.stage = NULL;
	
	//SC_REFRACT
	reset(sc_view_refract,VISIBLE);
	sc_view_refract.stage = NULL;
	
	
	//SC_VOLPARTS
	sc_view_volParts.stage = NULL;
	sc_view_volParts2.stage = NULL;
	sc_view_volPartsBlur.stage = NULL;
	sc_view_volPartsHBlur.stage = NULL;
	sc_view_volPartsVBlur.stage = NULL;
	sc_view_volPartsCombine.stage = NULL;
	
	camera.stage = NULL;
}

void sc_resetRTs()
{
	bmap_purge(sc_map_scene);
	bmap_purge(sc_bmap_depth);
	bmap_purge(sc_bmap_dof);
	bmap_purge(sc_map_reflect);
	bmap_purge(sc_map_refract);
	//bmap_purge(sc_bmap_volParts);
	//bmap_purge(sc_bmap_volPartsFractal);
	

	//bmap_purge(sc_view_dofDownsample.bmap);
	//bmap_purge(sc_view_dofHBlur.bmap);
	//bmap_purge(sc_view_dofVBlur.bmap);
	
	camera.bmap = NULL;
}
*/

void sc_reset()
{
	proc_kill(4);
	
	//sc_bHDR = 0;
   //sc_bDOF = 1;
   //sc_bReflect = 0;
   //sc_bRefract = 0;
   //sc_scSetup = 0;
   /*
	sc_resetFuncs();
	sc_resetViews();
	//sc_resetRTs();
	
	
	bmap_purge(sc_map_scene);
	bmap_purge(sc_view_dofDownsample.bmap);
	bmap_purge(sc_view_dofHBlur.bmap);
	bmap_purge(sc_view_dofVBlur.bmap);
		
	free(sc_view_dofDownsample.bmap);
	free(sc_view_dofHBlur.bmap);
	free(sc_view_dofVBlur.bmap);
	free(sc_view_dof.bmap);
		
	free(sc_map_scene);
	free(sc_bmap_depth);
	free(sc_bmap_dof);
	free(sc_map_reflect);
	free(sc_map_refract);
	//free(sc_bmap_volParts);
	//free(sc_bmap_volPartsFractal);
	
	free(sc_view_hdrDownsample.bmap);
	free(sc_view_hdrHighpass.bmap);
	free(sc_view_hdrBlur.bmap);
	free(sc_view_hdrHBlur.bmap);
	free(sc_view_hdrVBlur.bmap);
	free(sc_view_hdrGamma4.bmap);
	
	free(sc_view_dofDownsample.bmap);
	free(sc_view_dofHBlur.bmap);
	free(sc_view_dofVBlur.bmap);
	*/
	wait(1);
	sc_setup();
	
	//wait(10);
	

}
