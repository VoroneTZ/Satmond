#define SC_VOLPARTS


#ifndef SC_SCENERT
	#define SC_SCENERT
#endif

/*******************************************************************************************************************
VARIABLES
*******************************************************************************************************************/

//enable/disable volumetric particles
var sc_bVolParts = 0;

// DON'T CHANGE ANYTHING BELOW THIS LINE ---------------------------------------------------------------------------

/*******************************************************************************************************************
MATERIAL-EVENTS
*******************************************************************************************************************/
VIEW* sc_view_volParts;

void sc_mtlEvent_volParts()
{
	mtl.skill1 = 0;
	if(my)
	{
		if(my.skill99)
		{
			SC_OBJECT* ObjData = (SC_OBJECT*)(my.skill99);
			
			if(ObjData.myVolPart > 0){
			mtl.skill1 = floatv(ObjData.myVolPart);
			}
			if(ObjData.myVolPart > 0){
			mtl.skill2 = floatv(ObjData.myTexMov);
			}
		}
	}
}

/*******************************************************************************************************************
MATERIALS
*******************************************************************************************************************/

BMAP* sc_bmap_volParts;
BMAP* sc_bmap_volPartsFractal;

MATERIAL* sc_mtl_volParts =
{
	event = sc_mtlEvent_volParts;
	flags = ENABLE_RENDER;
	effect = "sc_volParts.fx";
}

MATERIAL* sc_mtl_volParts2 =
{
	event = sc_mtlEvent_volParts;
	flags = ENABLE_RENDER;
	effect = "sc_volParts2.fx";
}

MATERIAL* sc_mtl_volPartsVBlur =
{
	effect = "sc_volPartsVBlur.fx";
}

MATERIAL* sc_mtl_volPartsHBlur =
{
	effect = "sc_volPartsHBlur.fx";
}

MATERIAL* sc_mtl_volPartsBlur =
{
	effect = "sc_volPartsBlur.fx";
}

MATERIAL* sc_mtl_volPartsCombine =
{
	effect = "sc_volPartsCombine.fx";
}

MATERIAL* sc_mtl_volPartsInv =
{
	effect = "sc_volPartsInv.fx";
}

/*******************************************************************************************************************
VIEWS
*******************************************************************************************************************/

VIEW* sc_view_volParts = {material = sc_mtl_volParts; layer = 3;}
VIEW* sc_view_volParts2 = {material = sc_mtl_volParts2; layer = 4;}
VIEW* sc_view_volPartsBlur = {material = sc_mtl_volPartsBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_volPartsHBlur = {material = sc_mtl_volPartsHBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_volPartsVBlur = {material = sc_mtl_volPartsVBlur; flags = PROCESS_TARGET;}
VIEW* sc_view_volPartsCombine = {material = sc_mtl_volPartsCombine; flags = PROCESS_TARGET;}