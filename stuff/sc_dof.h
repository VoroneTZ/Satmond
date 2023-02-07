#define SC_DOF

#ifndef SC_SCENERT
	#define SC_SCENERT
#endif

/*******************************************************************************************************************
VARIABLES
*******************************************************************************************************************/
var sc_dofParams[3] = {800, 400, 0.8};
// x = focal plane depth "area of sharpness"
// y = focal plane pos
// z = blurstrength

//rendertarget resize factor
var sc_dofRT = 2;

// 16/32/14444 dof bitdepth
var sc_dofBits = 32;

//max scene depth
var sc_maxDepth = 50000;

//turn Depth of Field on/off
var sc_bDOF = 1;

// DON'T CHANGE ANYTHING BELOW THIS LINE ---------------------------------------------------------------------------
var s_dofDynFocusParams[3] = {0,0,0};

/*******************************************************************************************************************
MATERIAL-EVENTS
*******************************************************************************************************************/

void sc_mtlEvent_depth(){
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

/*******************************************************************************************************************
MATERIALS
*******************************************************************************************************************/

//________________________________________________
//Scenedepth(r), Waterdepth(g), DoF-Depth(b)
MATERIAL* sc_mtl_depth =
{
	event = sc_mtlEvent_depth;
	flags = ENABLE_RENDER;
	effect = "sc_depth.fx";	
}
//

//________________________________________________
//DoF
/*
#ifdef SC_VOLPARTS
MATERIAL* sc_mtl_dofDownsample =
{
	effect = "sc_dofDownsampleVP.fx";
}
#else
MATERIAL* sc_mtl_dofDownsample =
{
	effect = "sc_dofDownsample.fx";
}
#endif
*/
MATERIAL* sc_mtl_dofDownsample =
{
	//effect = "sc_dofDownsample.fx";
}

MATERIAL* sc_mtl_dofHBlur =
{
	effect = "sc_dofHBlur.fx";	
}

MATERIAL* sc_mtl_dofVBlur =
{
	effect = "sc_dofVBlur.fx";	
}

MATERIAL* sc_mtl_dof =
{
	effect = "sc_dof.fx";	
}
//

/*******************************************************************************************************************
VIEWS
*******************************************************************************************************************/

//________________________________________________
//Scenedepth(r), Waterdepth(g) ,DoF-Depth(b) 
VIEW* sc_view_depth = {material = sc_mtl_depth; layer = -2;}

//________________________________________________
//Depth of Field
VIEW* sc_view_dofDownsample = {material = sc_mtl_dofDownsample; flags = PROCESS_TARGET;}
VIEW* sc_view_dofHBlur = {material = sc_mtl_dofHBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_dofVBlur = {material = sc_mtl_dofVBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_dof = {material = sc_mtl_dof; flags = PROCESS_TARGET;}