
typedef struct SHADEC_OBJECT{
   var myDepth; //custom depth for an entity. Range: 0 to 1
	var test2;
}SHADEC_OBJECT;

/*
void structTest()
{

	SHADEC_OBJECT *myData = malloc(sizeof(SHADEC_OBJECT));
	myData.myDepth = 0.9;
	
	my.skill99 = myData;
	
	SHADEC_OBJECT* gibsHer = (SHADEC_OBJECT*)(my.skill99);
	my.x = gibsHer.myDepth;
	
}
*/
/*******************************************************************************************************************/

#define PRAGMA_PATH "shade-c/fx"

/*******************************************************************************************************************
VARIABLES
*******************************************************************************************************************/

var sc_rtScale = 256; // render-target size
// 128 for resolutions < 512x384
// 256 for resolutions < 1024x768 (default)
// 512 for resolutions > 1024x768
// 1024 for very high resolutions (~2048x?). Just set this if hdr or dof looks strange for very high resolutions

// DON'T CHANGE ANYTHING BELOW THIS LINE ---------------------------------------------------------------------------

var scObjDepth = 1;

/*******************************************************************************************************************
BITMAPS
*******************************************************************************************************************/

BMAP* sc_bmap_scene;
BMAP* sc_bmap_depth;
BMAP* sc_bmap_dof;
BMAP* sc_bmap_gamma = "#1x1x32";

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
			
			SHADEC_OBJECT* getObjData = (SHADEC_OBJECT*)(my.skill99);
			mtl.skill3 = floatv(getObjData.myDepth);
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
MATERIAL* sc_mtl_dofDownsample =
{
	effect = "sc_dofDownsample.fx";
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

//________________________________________________
//High Dyanmic Range Rendering
MATERIAL* sc_mtl_hdrDownsample =
{
	effect = "sc_hdrDownsample.fx";
}

MATERIAL* sc_mtl_hdrHighpass =
{
	effect = "sc_hdrHighpass.fx";
}

MATERIAL* sc_mtl_hdrBlur =
{
	effect = "sc_hdrBlur.fx";	
}

MATERIAL* sc_mtl_hdrHBlur =
{
	effect = "sc_hdrHBlur.fx";	
}

MATERIAL* sc_mtl_hdrVBlur =
{
	effect = "sc_hdrVBlur.fx";	
}

MATERIAL* sc_mtl_hdrCombine =
{
	effect = "sc_hdrTonemap.fx";	
}

MATERIAL* sc_mtl_hdrGamma =
{
	effect = "sc_hdrGamma.fx";	
}

MATERIAL* sc_mtl_hdrGamma2 =
{
	effect = "sc_hdrGamma2.fx";	
}

MATERIAL* sc_mtl_hdrGamma3 =
{
	effect = "sc_hdrGamma3.fx";
}

MATERIAL* sc_mtl_hdr =
{
	effect = "sc_hdr.fx";	
}
//

//________________________________________________
//Screen Spaced Ambience Occlusion
MATERIAL* sc_mtl_ssaoNormals =
{
	effect = "sc_ssaoNormals.fx";
}

MATERIAL* sc_mtl_ssaoPrepare =
{
	effect = "sc_ssaoPrepare.fx";
}

MATERIAL* sc_mtl_ssao =
{
	effect = "sc_ssao.fx";
}

MATERIAL* sc_mtl_ssaoNoise =
{
	effect = "sc_ssaoNoise.fx";
}

MATERIAL* sc_mtl_ssaoHBlur =
{
	effect = "sc_ssaoHBlur.fx";
}

MATERIAL* sc_mtl_ssaoVBlur =
{
	effect = "sc_ssaoVBlur.fx";
}

MATERIAL* sc_mtl_ssaoCombine =
{
	effect = "sc_ssaoCombine.fx";
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

//________________________________________________
//High Dyanmic Range Rendering
VIEW* sc_view_hdrDownsample = {material = sc_mtl_hdrDownsample; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrHighpass= {material = sc_mtl_hdrHighpass; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrBlur = {material = sc_mtl_hdrBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrHBlur = {material = sc_mtl_hdrHBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrVBlur = {material = sc_mtl_hdrVBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrCombine= {material = sc_mtl_hdrCombine; flags = PROCESS_TARGET;}
VIEW* sc_view_hdrGamma= {material = sc_mtl_hdrGamma; flags = PROCESS_TARGET + VISIBLE; layer = -10; bmap="#64x64x32";}
VIEW* sc_view_hdrGamma2= {material = sc_mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 16; size_y = 16; bmap="#16x16x32";}
VIEW* sc_view_hdrGamma3= {material = sc_mtl_hdrGamma2; flags = PROCESS_TARGET;size_x = 4; size_y = 4; bmap="#4x4x32";}
VIEW* sc_view_hdrGamma4= {material = sc_mtl_hdrGamma3; flags = PROCESS_TARGET;size_x = 1; size_y = 1;}
VIEW* sc_view_hdr = {material = sc_mtl_hdr; flags = PROCESS_TARGET;}
//

//________________________________________________
//Screen Spaced Ambience Occlusion
VIEW* sc_view_ssaoNormals = {material = sc_mtl_ssaoNormals; layer = 3;}
VIEW* sc_view_ssaoPrepare = {material = sc_mtl_ssaoPrepare; flags = PROCESS_TARGET;}
VIEW* sc_view_ssao = {material = sc_mtl_ssao; flags = PROCESS_TARGET;}
VIEW* sc_view_ssaoNoise = {material = sc_mtl_ssaoNoise; flags = PROCESS_TARGET;}
VIEW* sc_view_ssaoHBlur = {material = sc_mtl_ssaoHBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_ssaoVBlur = {material = sc_mtl_ssaoVBlur; flags = PROCESS_TARGET + VISIBLE; layer = -11;}
VIEW* sc_view_ssaoCombine = {material = sc_mtl_ssaoCombine; flags = PROCESS_TARGET;}
//

//________________________________________________
VIEW* sc_view_last;
