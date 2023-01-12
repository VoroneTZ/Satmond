void sc_waterViewsLoop(ENTITY* ent)
{
	VECTOR temp_vec[3];
 	proc_kill(4);
 	
 	#ifdef SC_REFLECT
 	sc_mtl_water.skin1 = sc_map_reflect;
 	#endif
 	#ifdef SC_REFRACT
	sc_mtl_water.skin2 = sc_map_refract;
	#endif
 
 	while(1)
 	{
		proc_mode = PROC_LATE;
		
		#ifdef SC_DOF
			sc_mtl_depth.skill5 = floatv(ent.z);
			sc_mtl_depth.skill6 = floatv(sc_waterDepth);
 		#endif
 	
  		#ifdef SC_REFLECT
  		sc_view_reflect.arc = camera.arc;
  		#endif
  		
		temp_vec.x = 0; temp_vec.y = 0; temp_vec.z = 0; vec_rotate(temp_vec.x, camera.pan); vec_add(temp_vec.x, camera.x);
  		
  		#ifdef SC_REFLECT
  		sc_view_reflect.x = temp_vec.x;
  		sc_view_reflect.y = temp_vec.y;
 		sc_view_reflect.z = ent.z-camera.z;
 		#endif
 		 	
 		//vec_set(sc_view_refract.x,camera.x);
 		//vec_set(sc_view_refract.pan,camera.pan);

		#ifdef SC_REFLECT
		sc_view_reflect.pan = camera.pan;
		sc_view_reflect.tilt = -camera.tilt;
		#endif

		wait(1);
	}
}

void sc_water(ENTITY* ent)
{
	//wait till level has been loaded
	wait(10);
	
	#ifdef SC_DOF
		sc_mtl_water.skin3 = sc_map_depth;
	#endif
	
	//set myDepth to 2, so water gets clipped when generating the depthmap
	sc_skill(ent,sc_myDepth, 2);
		//set water material
	ent.material = sc_mtl_water;

	//initialize views & shader
	if(sc_bWater) sc_waterViewsLoop(ent);
	
}