#include "shade-c.h";

//Modules
#include "sc_hdr.h";
//#include "sc_dof.h";

#include "sc_hdr.c";
//#include "sc_dof.c";

void sc_setup()
{
	//check defines
	#ifdef SC_DOF
	sc_intern_dof = 1;
	#endif
	
	
	
	
	//SET s_rtScale
	sc_mtl_dof.skill4 = floatv(sc_rtScale);
	sc_mtl_hdrCombine.skill4 = floatv(sc_rtScale);
	
	//SET DOF BlURSTRENGTH
	sc_mtl_dofHBlur.skill1 = floatv(sc_dofParams[2]);
	sc_mtl_dofVBlur.skill1 = floatv(sc_dofParams[2]);
	
	//SET HDR BlURSTRENGTH + HIGHPASS + CONTRAST
	sc_mtl_hdrBlur.skill1 = floatv(sc_hdrParams[0]);
	sc_mtl_hdrHBlur.skill1 = floatv(sc_hdrParams[1]);
	sc_mtl_hdrVBlur.skill1 = floatv(sc_hdrParams[1]);
	sc_mtl_hdrHighpass.skill1 = floatv(1-sc_hdrParams[2]);
	sc_mtl_hdrCombine.skill1 = floatv(sc_hdrParams[3]);
	
	//SET VIEW DIMENSIONS AND RTs
	sc_view_dofDownsample.size_x = sc_rtScale;
	sc_view_dofDownsample.size_y = sc_rtScale;
	sc_view_dofDownsample.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_dofBits);
	sc_view_dofHBlur.size_x = sc_rtScale;
	sc_view_dofHBlur.size_y = sc_rtScale;
	sc_view_dofHBlur.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_dofBits);
	sc_view_dofVBlur.size_x = sc_rtScale;
	sc_view_dofVBlur.size_y = sc_rtScale;
	sc_view_dofVBlur.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_dofBits);
	
	sc_view_hdrDownsample.arc = camera.arc;
	sc_view_hdrDownsample.size_x = sc_rtScale;
	sc_view_hdrDownsample.size_y = sc_rtScale;
	sc_view_hdrDownsample.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_hdrBits);
	sc_view_hdrHighpass.arc = camera.arc;
	sc_view_hdrHighpass.size_x = sc_rtScale;
	sc_view_hdrHighpass.size_y = sc_rtScale;
	sc_view_hdrHighpass.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_hdrBits);
	sc_view_hdrBlur.arc = camera.arc;
	sc_view_hdrBlur.size_x = sc_rtScale;
	sc_view_hdrBlur.size_y = sc_rtScale;
	sc_view_hdrBlur.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_hdrBits);
	sc_view_hdrHBlur.arc = camera.arc;
	sc_view_hdrHBlur.size_x = sc_rtScale;
	sc_view_hdrHBlur.size_y = sc_rtScale;
	sc_view_hdrHBlur.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_hdrBits);
	sc_view_hdrVBlur.arc = camera.arc;
	sc_view_hdrVBlur.size_x = sc_rtScale;
	sc_view_hdrVBlur.size_y = sc_rtScale;
	sc_view_hdrVBlur.bmap = bmap_createblack(sc_rtScale,sc_rtScale,sc_hdrBits);
	sc_view_hdrCombine.arc = camera.arc;
	sc_view_hdrCombine.size_x = screen_size.x;
	sc_view_hdrCombine.size_y = screen_size.y;
}

void sc_init(){
	
	//setup views & materials
	sc_setup();
	
	//RT Scene
	sc_bmap_scene = bmap_createblack(screen_size.x, screen_size.y,32);
	camera.bmap = sc_bmap_scene;
	
	//DoF
	sc_dof();
	//
	
	sc_view_hdrGamma.stage = sc_view_hdrGamma2;
	sc_view_hdrGamma2.stage = sc_view_hdrGamma3;
	sc_view_hdrGamma3.stage = sc_view_hdrGamma4;
	sc_view_hdrGamma4.bmap = sc_bmap_gamma;
	
	//HDR
	sc_hdr();
	//
	
	proc_mode = PROC_LATE;
	while(1){
		vec_set(sc_view_depth.x, camera.x);
		vec_set(sc_view_depth.pan, camera.pan);
		//sc_view_depth.arc = camera.arc;
		
		vec_set(sc_view_ssaoNormals.x, camera.x);
		vec_set(sc_view_ssaoNormals.pan, camera.pan);
		sc_view_ssaoNormals.arc = camera.arc;
		wait(1);
	}
}

void sc_obj(var objMode, var objVar)
{
	if(objMode == 0){return;}
	
	SHADEC_OBJECT *myData = malloc(sizeof(SHADEC_OBJECT));
	
	if(objMode == 1){myData.myDepth = objVar;}
	
	my.skill99 = myData;
}