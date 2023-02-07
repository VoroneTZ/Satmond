void sc_reflect()
{
 	sc_map_reflect = bmap_createblack(sc_reflectRT[0],sc_reflectRT[1],32);
 
 
 	sc_view_reflect.bmap = sc_map_reflect;
 	sc_view_reflect.clip_near = 1;
 	set(sc_view_reflect,VISIBLE);
 	//set(sc_view_reflect,NOSHADOW);
 	//set(sc_view_reflect,NOPARTICLE);
 	set(sc_view_reflect,NOCULL);
 	set(sc_view_reflect,PORTALCLIP);
 	set(sc_view_reflect,UNTOUCHABLE);
 	reset(sc_view_reflect,AUDIBLE);
 	sc_view_reflect.aspect = -1;
 	vec_set(sc_view_reflect.portal_x,vector(0,0,-1));
 	
}