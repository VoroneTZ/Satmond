
MATERIAL* sc_mtl_shadowBlur =
{
	effect = "sc_vsmSunBlur.fx";
}

/*
VIEW* blurDepthView =
{
	flags = PROCESS_TARGET | CHILD;
	material = sc_mtl_shadowBlur;
}
*/



// void sc_smSunSetup(var SMMapRes, var SMRes, var SMdepth, float SMblur, var sType)
// SMMapRes = Shadowmap Resolution (must be smaller or equal to screen_size.x)
// SMRes = ShadowView Resolution | Area of Shadow (how many quants should be shadowed at once? keep this as low as possible for nice shadows)
// SMdepth = max depthmap depth (distance after which shadows are clipped. Keep this as low as possible for nice shadows)
// SMblur = shadow blur amount
// sType = shadow Type | 0 = Shader integrated Shadows (fast & nice), 1 = Shadows are rendered for the whole scene and multiplied with it (slow & ugly)
void sc_smSunSetup(var SMMapRes, var SMRes, var SMdepth, float SMblur, var sType)
{
	wait(5);
	VIEW* blurDepthView;
	blurDepthView = malloc(sizeof(VIEW*));
	blurDepthView = view_create(-2);
	set(blurDepthView,PROCESS_TARGET);
	set(blurDepthView,CHILD);
	blurDepthView.material = sc_mtl_shadowBlur;
	
	VIEW* shadowView;
	if(sType == 1)
	{
		shadowView = malloc(sizeof(VIEW*));
		shadowView = view_create(-2);
		shadowView.material = sc_mtl_shadowSun;
		set(shadowView,VISIBLE);
		
		sc_map_shadow = bmap_createblack(screen_size.x,screen_size.y,32);
		shadowView.bmap = sc_map_shadow;
		sc_mtl_shadowSunAdd.skin1 = sc_map_shadow;
		sc_ppAdd(sc_mtl_shadowSunAdd,camera,0);
	}
	
	//set(shadowView,VISIBLE);

	
	sc_orthoMat = 0;
	
	while(camera == NULL) wait(1);
	
	//D3DXMatrixOrthoLH(sc_orthoMat, SMRes*1.5, SMRes, 0.0, 100000);
	D3DXMatrixOrthoLH(sc_orthoMat, SMRes*1.5, SMRes, 0.0, camera.clip_far*5);
	mat_set(sc_mtl_shadowDepthSun.matrix,sc_orthoMat);
	mat_effect1 = sc_orthoMat;
	
	VIEW* shadowDepthView;
	shadowDepthView = malloc(sizeof(VIEW*));
	
	sc_map_shadowDepthSun = malloc(sizeof(BMAP*));
	sc_mtl_shadowDepthSun.flags |= ENABLE_VIEW;
	
	//setup sun depth view
	shadowDepthView = view_create(-21);
	shadowDepthView.material = sc_mtl_shadowDepthSun;
	shadowDepthView.pan = 302;
	shadowDepthView.tilt = -38;
	set(shadowDepthView,VISIBLE);
	set(shadowDepthView,UNTOUCHABLE);
	set(shadowDepthView,NOSHADOW);
	set(shadowDepthView,NOPARTICLE);
	shadowDepthView.size_x = SMMapRes;
	shadowDepthView.size_y = SMMapRes;
	//shadowDepthView.clip_far = SMdepth;
	// ----------------------------
	
	sc_map_shadowDepthSun = bmap_createblack(SMMapRes,SMMapRes,14);

	
	//setup blur view
	// ----------------------------
	
	//shadowDepthView.bmap = sc_map_shadowDepthSun;
	shadowDepthView.bmap = bmap_createblack(SMMapRes,SMMapRes,14);
	shadowDepthView.stage = blurDepthView;
	blurDepthView.bmap = sc_map_shadowDepthSun;

	sc_mtl_shadowSun.skin1 = sc_map_shadowDepthSun;


	//set depthmap depth
	sc_orthoLightPos = 0;
   sc_orthoLightPos[3] = floatv(SMdepth); //depthmap depth
   sc_mtl_shadowBlur.skill1 = floatv(SMblur); //shadow blur factor;
   //sc_orthoLightPos[4] = floatv(SMblur); //shadow blur factor;
	
	VECTOR temp;
	VECTOR cam_dist;
	vec_set(temp, nullvector);
	vec_set(cam_dist, nullvector);
	cam_dist.x = SMRes/2;
	cam_dist.y = 0;
	

	you = sc_ent_smSunDummy;
	while(1)
	{
		//sun_angle.pan += 1*time_step;
		
		//align depthmap view to sun_angle
		vec_set(temp, vector(-sun_pos.x, -sun_pos.y, -sun_pos.z));
	   vec_sub(temp, shadowDepthView.x);
   	vec_to_angle(shadowDepthView.pan, temp);
   
	
   	
   	//algin depthmap view to camera position
	   vec_set(you.x,cam_dist);
      vec_rotate(you.x,camera.pan);
      vec_add(you.x,camera.x);
      
      vec_set(temp, sun_pos);
      vec_scale(temp,0.05);
      vec_set(shadowDepthView.x,temp);
      vec_rotate(shadowDepthView.x,you.pan);
      vec_add(shadowDepthView.x,you.x);

		
		//set global lightpos
		vec_set(temp,shadowDepthView.x);
   	sc_orthoLightPos[0] = floatv(temp.x);
   	sc_orthoLightPos[1] = floatv(temp.y);
   	sc_orthoLightPos[2] = floatv(temp.z);
   	mat_effect2 = sc_orthoLightPos;


		if(sType == 1)
		{
			vec_set(shadowView.x,camera.x);
			vec_set(shadowView.pan,camera.pan);
			shadowView.arc = camera.arc;
		}
		
		wait(1);
	}

}



void sc_smPointSetup(var numLights, var resolution)
{
	while(!camera) wait(1);
	sc_bShadow = 0;
	
	proc_mode = PROC_LATE;
	
	VIEW** shadowviews;
	shadowviews = malloc(sizeof(VIEW*)*numLights*6);
	
	BMAP** shadowbmaps;
	shadowbmaps = malloc(sizeof(BMAP*)*numLights);
	
	VIEW* shadowView;
	shadowView = view_create(-1);
	
	int i;
	for(i = 0; i < numLights; i++)
	{
		shadowbmaps[i] = bmap_createblack(resolution*6,resolution,12222);
		bmap_to_cubemap(shadowbmaps[i]);
		
		shadowviews[i*6+0] = view_create(-2);
		shadowviews[i*6+0].material = sc_mtl_shadowDepth;
		shadowviews[i*6+0].arc = 90;
		shadowviews[i*6+0].pan = 180;
		shadowviews[i*6+0].tilt = 0;
		shadowviews[i*6+0].roll = 0;
		shadowviews[i*6+0].bmap = shadowbmaps[i];
		shadowviews[i*6+0].pos_x = 0;
		shadowviews[i*6+0].size_x = resolution;
		shadowviews[i*6+0].size_y = resolution;
		set(shadowviews[i*6+0],VISIBLE);
			
		shadowviews[i*6+1] = view_create(-2);
		shadowviews[i*6+1].material = sc_mtl_shadowDepth;
		shadowviews[i*6+1].arc = 90;
		shadowviews[i*6+1].pan = 90;
		shadowviews[i*6+1].tilt = 0;
		shadowviews[i*6+1].roll = 0;
		shadowviews[i*6+1].bmap = shadowbmaps[i];
		shadowviews[i*6+1].pos_x = resolution;
		shadowviews[i*6+1].size_x = resolution;
		shadowviews[i*6+1].size_y = resolution;
		set(shadowviews[i*6+1],VISIBLE);
		
		shadowviews[i*6+2] = view_create(-2);
		shadowviews[i*6+2].material = sc_mtl_shadowDepth;
		shadowviews[i*6+2].arc = 90;
		shadowviews[i*6+2].pan = 0;
		shadowviews[i*6+2].tilt = 0;
		shadowviews[i*6+2].roll = 0;
		shadowviews[i*6+2].bmap = shadowbmaps[i];
		shadowviews[i*6+2].pos_x = resolution*2;
		shadowviews[i*6+2].size_x = resolution;
		shadowviews[i*6+2].size_y = resolution;
		set(shadowviews[i*6+2],VISIBLE);
		
		shadowviews[i*6+3] = view_create(-2);
		shadowviews[i*6+3].material = sc_mtl_shadowDepth;
		shadowviews[i*6+3].arc = 90;
		shadowviews[i*6+3].pan = 270;
		shadowviews[i*6+3].tilt = 0;
		shadowviews[i*6+3].roll = 0;
		shadowviews[i*6+3].bmap = shadowbmaps[i];
		shadowviews[i*6+3].pos_x = resolution*3;
		shadowviews[i*6+3].size_x = resolution;
		shadowviews[i*6+3].size_y = resolution;
		set(shadowviews[i*6+3],VISIBLE);
		
		shadowviews[i*6+4] = view_create(-2);
		shadowviews[i*6+4].material = sc_mtl_shadowDepth;
		shadowviews[i*6+4].arc = 90;
		shadowviews[i*6+4].pan = 90;
		shadowviews[i*6+4].tilt = -90;
		shadowviews[i*6+4].roll = 0;
		shadowviews[i*6+4].bmap = shadowbmaps[i];
		shadowviews[i*6+4].pos_x = resolution*4;
		shadowviews[i*6+4].size_x = resolution;
		shadowviews[i*6+4].size_y = resolution;
		set(shadowviews[i*6+4],VISIBLE);
		
		shadowviews[i*6+5] = view_create(-2);
		shadowviews[i*6+5].material = sc_mtl_shadowDepth;
		shadowviews[i*6+5].arc = 90;
		shadowviews[i*6+5].pan = 90;
		shadowviews[i*6+5].tilt = 90;
		shadowviews[i*6+5].roll = 0;
		shadowviews[i*6+5].bmap = shadowbmaps[i];
		shadowviews[i*6+5].pos_x = resolution*5;
		shadowviews[i*6+5].size_x = resolution;
		shadowviews[i*6+5].size_y = resolution;
		set(shadowviews[i*6+5],VISIBLE);
		
		view_to_light(shadowviews[i*6+0],i+1,NULL);
		vec_set(shadowviews[i*6+1].x , shadowviews[i*6+0].x);
		vec_set(shadowviews[i*6+2].x , shadowviews[i*6+0].x);
		vec_set(shadowviews[i*6+3].x , shadowviews[i*6+0].x);
		vec_set(shadowviews[i*6+4].x , shadowviews[i*6+0].x);
		vec_set(shadowviews[i*6+5].x , shadowviews[i*6+0].x);
		
			
		switch(i)
		{
			case 0:
				sc_mtl_shadow.skin1 = shadowbmaps[i];
				break;
			
			case 1:
				sc_mtl_shadow.skin2 = shadowbmaps[i];
				break;
			
			case 2:
				sc_mtl_shadow.skin3 = shadowbmaps[i];
				break;
			
			case 3:
				sc_mtl_shadow.skin4 = shadowbmaps[i];
				break;
			
			default:
				break;
		}
		
	}
	
	shadowView.arc = camera.arc;
	shadowView.genius = camera.genius;
	shadowView.clip_near = camera.clip_near;
	shadowView.material = sc_mtl_shadow;
	set(shadowView,VISIBLE);
	shadowView.bmap = sc_map_shadow;
	sc_mtl_pp_multiply.skin1 = sc_map_shadow;
	//sc_ppRemove(sc_mtl_pp_multiply,camera,0);
	sc_ppAdd(sc_mtl_pp_multiply,camera,0);
	
	sc_mtl_shadow.skill1 = floatv(numLights);
	
	var activeLights = numLights;
	VECTOR viewPos;
	var matLightPositions[16];
	sc_bShadow = 1;
	while(sc_bShadow)
	{
		vec_set(shadowView.x, camera.x);
		vec_set(shadowView.pan, camera.pan);
		activeLights = numLights;
		for(i = 0; i < numLights; i++)
		{
			/*
			set(shadowviews[i*6+0],VISIBLE);
			set(shadowviews[i*6+1],VISIBLE);
			set(shadowviews[i*6+2],VISIBLE);
			set(shadowviews[i*6+3],VISIBLE);
			set(shadowviews[i*6+4],VISIBLE);
			set(shadowviews[i*6+5],VISIBLE);
			*/
			//check number of active dynamic lights
			if(view_to_light(shadowviews[i*6+0],i+1,NULL) == 0)
			{
				shadowviews[i*6+0].clip_far = 0;
				
				/*
				reset(shadowviews[i*6+0],VISIBLE);
				reset(shadowviews[i*6+1],VISIBLE);
				reset(shadowviews[i*6+2],VISIBLE);
				reset(shadowviews[i*6+3],VISIBLE);
				reset(shadowviews[i*6+4],VISIBLE);
				reset(shadowviews[i*6+5],VISIBLE);
				*/
				activeLights -= 1;
			}
			
			//check if view/shadow is seen by camera
			// STILL NEEDS WORK - CHECK IF SPHERICAL LIGHT RADIUS IS SEEN ON SCREEN INSTEAD OF JUST ONE POINT
			//camera looks away and is out of lightrange?
			vec_set(viewPos, shadowviews[i*6+0].x);
			if (NULL == vec_to_screen(viewPos,camera))
			{
				if(shadowviews[i*6+0].clip_far < vec_dist(shadowviews[i*6+0].x,camera.x))
				{
					shadowviews[i*6+0].clip_far = 0;
					/*
					reset(shadowviews[i*6+0],VISIBLE);
					reset(shadowviews[i*6+1],VISIBLE);
					reset(shadowviews[i*6+2],VISIBLE);
					reset(shadowviews[i*6+3],VISIBLE);
					reset(shadowviews[i*6+4],VISIBLE);
					reset(shadowviews[i*6+5],VISIBLE);
					*/
				}
			}
			
			
			//view_to_light(shadowviews[i*6+0],i+1,NULL);
			vec_set(shadowviews[i*6+1].x , shadowviews[i*6+0].x);
			vec_set(shadowviews[i*6+2].x , shadowviews[i*6+0].x);
			vec_set(shadowviews[i*6+3].x , shadowviews[i*6+0].x);
			vec_set(shadowviews[i*6+4].x , shadowviews[i*6+0].x);
			vec_set(shadowviews[i*6+5].x , shadowviews[i*6+0].x);
			
			
			shadowviews[i*6+1].clip_far = shadowviews[i*6+0].clip_far;
			shadowviews[i*6+2].clip_far = shadowviews[i*6+0].clip_far;
			shadowviews[i*6+3].clip_far = shadowviews[i*6+0].clip_far;
			shadowviews[i*6+4].clip_far = shadowviews[i*6+0].clip_far;
			shadowviews[i*6+5].clip_far = shadowviews[i*6+0].clip_far;
			
			
			switch(i)
			{
				case 0:
					matLightPositions[0] = floatv(shadowviews[i*6+0].x);
					matLightPositions[1] = floatv(shadowviews[i*6+0].z);
					matLightPositions[2] = floatv(shadowviews[i*6+0].y);
					matLightPositions[3] = floatv(shadowviews[i*6+0].clip_far);
					break;
				
				case 1:
					matLightPositions[4] = floatv(shadowviews[i*6+0].x);
					matLightPositions[5] = floatv(shadowviews[i*6+0].z);
					matLightPositions[6] = floatv(shadowviews[i*6+0].y);
					matLightPositions[7] = floatv(shadowviews[i*6+0].clip_far);
					break;
					
				case 2:
					matLightPositions[8] = floatv(shadowviews[i*6+0].x);
					matLightPositions[9] = floatv(shadowviews[i*6+0].z);
					matLightPositions[10] = floatv(shadowviews[i*6+0].y);
					matLightPositions[11] = floatv(shadowviews[i*6+0].clip_far);
					break;
					
				case 3:
					matLightPositions[12] = floatv(shadowviews[i*6+0].x);
					matLightPositions[13] = floatv(shadowviews[i*6+0].z);
					matLightPositions[14] = floatv(shadowviews[i*6+0].y);
					matLightPositions[15] = floatv(shadowviews[i*6+0].clip_far);
					break;
			}
			mat_set(sc_mtl_shadow.matrix, matLightPositions);
		}
		sc_mtl_shadow.skill1 = floatv(activeLights);
		
		wait(1);
	}
	
	
	while(1)
	{
		vec_set(shadowView.x, camera.x);
		vec_set(shadowView.pan, camera.pan);
		wait(1);
	}
}