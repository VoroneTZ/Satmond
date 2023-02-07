

BMAP* sc_map_shadowAlpha = "sc_shadowAlpha.bmp";
BMAP* sc_map_shadowDepthSun;

BMAP* sc_map_environment = "sky04S+6.tga";

MATERIAL* sc_mtl_shadow;
MATERIAL* sc_mtl_shadowSun;

var sc_orthoMat[16];
var sc_orthoLightPos[16];
var sc_sunLightView[16];
var sc_bShadow = 1;

ENTITY* sc_ent_smSunDummy =
{
	type = "xyz.mdl";
}

void sc_mtl_shadowDepthSunEvent()
{
	//if(render_view != sc_view_depth)
	//{
		mat_set(sc_mtl_shadowSun.matrix,matWorld);
		mat_multiply(sc_mtl_shadowSun.matrix,matView);
	
		mat_set(sc_sunLightView,matWorld);
		mat_multiply(sc_sunLightView,matView);
		mat_effect3 = sc_sunLightView;
		
		#ifdef SC_VOLPARTS
			mat_set(sc_mtl_volParts2.matrix,matWorld);
			mat_multiply(sc_mtl_volParts2.matrix,matView);
		#endif
		
		//SKY ?
		mtl.skill3 = floatv(0);
		mtl.skill4 = floatv(0);
		if(my)
		{
			if(my.skill99)
			{
				SC_OBJECT* ObjData = (SC_OBJECT*)(my.skill99);
				
				mtl.skill3 = floatv(ObjData.myDepth);
				mtl.skill4 = floatv(ObjData.myVolPart);
			}
		}

}

void sc_mtl_shadowDepthEvent()
{
	VECTOR myPos;
	set(render_view,VISIBLE);
	if(my)
	{
		vec_set(myPos,my.x);
		
		if(NULL != vec_to_screen(myPos,camera))
		{
			
			if(NULL != vec_to_screen(myPos,render_view)) //seen by lightview
			{
				//render_view.clip_far = 1;
				reset(render_view,VISIBLE);
				return;
			}
			
		}
		
		/*
		if(vec_to_screen(myPos,render_view) && vec_to_screen(myPos,camera))
		{
			return 0;
		}
		else
		{
			return 1;
		}
		*/
	}
	//return 1;
}

MATERIAL* sc_mtl_shadowDepth =
{
	//event = blubb;
	//flags = ENABLE_VIEW;
	//effect = "sc_shadowDepth.fx";
	//effect = "sc_shadowDepthOmni.fx";
	//event = sc_mtl_shadowDepthEvent;
	//flags = ENABLE_RENDER;
	effect = "sc_vsmDepth.fx";
}

MATERIAL* sc_mtl_shadow =
{
	//event = blubb;
	//flags = ENABLE_VIEW;
	//effect = "sc_shadow.fx";
	//effect = "sc_shadowOmni.fx";
	effect = "sc_vsm.fx";

}

MATERIAL* sc_mtl_shadowDepthSun =
{
	//event = blubb;
	//flags = ENABLE_VIEW;
	//effect = "sc_shadowDepth.fx";
	//effect = "sc_shadowDepthOmni.fx";
	//event = sc_mtl_shadowDepthEvent;
	event = sc_mtl_shadowDepthSunEvent;
	//flags = ENABLE_VIEW;
	effect = "sc_vsmDepthSun3.fx";
}


MATERIAL* sc_mtl_shadowSun =
{
	//event = blubb;
	//flags = ENABLE_VIEW;
	//effect = "sc_shadowDepth.fx";
	//effect = "sc_shadowDepthOmni.fx";
	//event = sc_mtl_shadowDepthEvent;
	//flags = ENABLE_RENDER;
	skin2 = sc_map_shadowAlpha;
	effect = "sc_vsmSun.fx";
}

MATERIAL* sc_mtl_shadowSunAdd =
{
	effect = "sc_vsmSunAdd.fx";
}

BMAP* sc_map_shadow = "#1024x768x32";