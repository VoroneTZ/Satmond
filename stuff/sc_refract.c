void sc_refract()
{
 	sc_map_refract = bmap_createblack(screen_size.x/sc_refractRT,screen_size.y/sc_refractRT,32);
 
 	sc_view_refract.bmap = sc_map_refract;
 	sc_view_refract.clip_near = 1;
 	set(sc_view_refract,VISIBLE);
 	//set(sc_view_refract,NOSHADOW);
 	//set(sc_view_refract,NOPARTICLE);
 	set(sc_view_refract,UNTOUCHABLE);
 	reset(sc_view_refract,AUDIBLE);
}