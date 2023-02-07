#define SC_OBJSHADERS

BMAP* sc_map_aioEnvmap = "sc_aioReflect+6.tga";

void sc_mtlEvent_aio()
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
			
			//if(ObjData.myTeamR || ObjData.myTeamG || ObjData.myTeamB){
			mtl.skill5 = floatv(ObjData.myTeamR);
			mtl.skill6 = floatv(ObjData.myTeamG);
			mtl.skill7 = floatv(ObjData.myTeamB);
			//}
			
			if(ObjData.myLuminance){
			mtl.skill9 = floatv(ObjData.myLuminance.x);
			mtl.skill10 = floatv(ObjData.myLuminance.y);
			mtl.skill11 = floatv(ObjData.myLuminance.z);
			}
		}
	}
}

MATERIAL* sc_mtl_obj_aio =
{
	skin1 = sc_map_aioEnvmap;
	effect = "sc_obj_aio.fx";
	event = sc_mtlEvent_aio;
	flags = ENABLE_RENDER;
}

BMAP* sc_bmap_toonLookup = "sc_toonLookup.bmp";

MATERIAL* sc_mtl_obj_toon =
{
	skin1 = sc_bmap_toonLookup; //not used yet...
	effect = "sc_obj_toon.fx";
	event = sc_mtlEvent_aio;
}

BMAP* sc_bmap_terrainColor = "terrain_color.dds";

MATERIAL* sc_mtl_obj_terrain =
{
	skin1 = sc_bmap_terrainColor;
	effect = "sc_obj_terrain.fx";
}

MATERIAL* sc_mtl_obj_softSkin =
{
	effect = "sc_obj_softSkin.fx";
}

#ifdef SC_REFRACT

void sc_mtlEvent_objRefract()
{
	mtl.skill1 = floatv(1.0);
	if(render_view == sc_view_refract)
	{
		mtl.skill1 = floatv(-1.0);
	}
	
	mtl.skill2 = floatv(0);
	
	mtl.skill5 = floatv(1);
	mtl.skill6 = floatv(1);
	mtl.skill7 = floatv(1);
	if(my)
	{
		if(my.skill99)
		{
			SC_OBJECT* ObjData = (SC_OBJECT*)(my.skill99);
			
			if(ObjData.myTexMov > 0){
			mtl.skill2 = floatv(ObjData.myTexMov);
			
			mtl.skill5 = floatv(ObjData.myRefrR);
			mtl.skill6 = floatv(ObjData.myRefrG);
			mtl.skill7 = floatv(ObjData.myRefrB);
			}
		}
	}
}

MATERIAL* sc_mtl_obj_heatHaze =
{
	event = sc_mtlEvent_objRefract;
	flags = ENABLE_RENDER;
	effect = "sc_obj_heatHaze.fx";
}

MATERIAL* sc_mtl_obj_glass =
{
	event = sc_mtlEvent_objRefract;
	flags = ENABLE_RENDER;
	effect = "sc_obj_glass.fx";
}

#endif
