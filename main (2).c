#define USE_SHADE_C			// comment this line to use modified 3DGS water shader

#define PRAGMA_PATH "shade-c"

#include <acknex.h>
#include <default.c>
#include <d3d9.h>			
#include <level.c>		
//#include <mtlFX.c>			// -> custom version
#include <mtlView.c>	
#include <shadows.c>	

#include "sc_core.c";		// Shade-C

#include "MBmtlFX.c"			// custom version used

////////////////////////////////////////////////////////////////////////////////////////////////////////

VECTOR* 	vScale 					= { x=0; y=0; z=0; } 
VECTOR*	vWater 					= { x=0; y=0; z=0; }   	

BMAP* 	mousearrow 				= "yellowarrow.bmp";

char* 	TerrainFileName1 		= "001dry_m0.hmp";
char* 	TerrainFileName3 		= "003drygrass.hmp";
char* 	TerrainFileName4 		= "003drygrass_1skin.hmp";

char* 	WaterFileName			= "water_sc2.hmp";

ENTITY*  terrain_entity;
ENTITY*	water_entity;
var*		map_water_vertices 	= NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////

ENTITY* ent_unitshadow_rot = 
	{ 
		type = "unit_shadow.dds"; 
	}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////

MATERIAL* terrain_multi_mat =
{
	effect = "terraintexMB.fx"; 
	ambient_blue  = 0;  		ambient_green  = 0;  	ambient_red  = 0;  
	diffuse_blue  = 175;  	diffuse_green  = 200;  	diffuse_red  = 200;  
	specular_blue = 50;  	specular_green = 60;  	specular_red = 60;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 3;
	albedo = 100;
}

MATERIAL* terrain_single_mat =
{	
	ambient_blue  = 0;  		ambient_green  = 0;  	ambient_red  = 0;  
	diffuse_blue  = 175;  	diffuse_green  = 200;  	diffuse_red  = 200;  
	specular_blue = 50;  	specular_green = 60;  	specular_red = 60;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 3;
	albedo = 100;
}

MATERIAL* water_multi_mat =
{	
	effect = "terraintex.fx"; 	
	ambient_blue  = 100;  	ambient_green  = 100;  	ambient_red  = 100;  // own brightness
	diffuse_blue  = 255;  	diffuse_green  = 255;  	diffuse_red  = 255;  // reflect sun light
	specular_blue = 175;  	specular_green = 150;  	specular_red = 150;  // towards sun
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;		// own light color
	power = 7;
	albedo = 100;
}

MATERIAL* water_single_mat =
{	
	ambient_blue  = 100;  	ambient_green  = 100;  	ambient_red  = 100;  // own brightness
	diffuse_blue  = 255;  	diffuse_green  = 255;  	diffuse_red  = 255;  // reflect sun light
	specular_blue = 175;  	specular_green = 150;  	specular_red = 150;  // towards sun
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;		// own light color
	power = 7;
	albedo = 100;
}

MATERIAL* mtl_obstacle =
{	
	ambient_blue  = 225;  	ambient_green  = 225;  	ambient_red  = 225;  
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;  
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 0;
	albedo = 100;
}

MATERIAL* mtl_tree =
{
	effect = "animTreeMB.fx";	
	ambient_blue  = 225;  	ambient_green  = 225;  	ambient_red  = 225;  
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;  
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 0;
	albedo = 100;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

function PosInfo3D(tempx, tempy);
function PosInfo3D2(tempx, tempy);

////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "camerahandling.c"
#include "playertest01.c"

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

// ground terrain settings
action Map_Terrain_Action()
{
	if (!terrain_entity) terrain_entity = me;
	
	if ( ent_status(me,8)>2 )
		{
			my.material = terrain_multi_mat;
		}
	else
		{
			my.material = terrain_single_mat;
		}
	set(my,SHADOW);
	
	vec_for_vertex(vScale, my, 1);								// vertex 1 = upper-left corner needed for water hmp scaling
	
	wait(1);
	my.emask &= ~DYNAMIC;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

// water terrain settings
action Map_Water_Action()
{
	if (!water_entity) water_entity = me;
	set(my , PASSABLE | CAST);	
	reset(my,SHADOW);
	
	// scale to terrain
	vec_for_vertex(vWater, my, 1);								// vertex 1 = upper-left corner
	
	if (vWater.x!=(var)0)
		my.scale_x = abs( vScale.x/vWater.x ); 
	if (vWater.y!=(var)0)
		my.scale_y = abs( vScale.y/vWater.y );
	
	wait(1);
	
	//-------------------------------------------------	
{}
#ifdef USE_SHADE_C
	// Shade-C
   sc_water(my);
#else	
	//-------------------------------------------------	
//	fx_envWater();						// not so nice
//	my.skill41 = floatv(10);		// speed x
//	my.skill42 = floatv(25);		// speed	y
//	my.skill43 = floatv(75);		//	ripple - waves
//	my.skill44 = floatv(10);		// scale
	
	//-------------------------------------------------			
	fx_mirrorWater();					// fine		// results in strange individual decal shadow behaviour in A7 and A8.30
	my.red 	= 10;
	my.green = 40;						
	my.blue 	= 30;
	my.skill41 = floatv(10);		// 10 speed x
	my.skill42 = floatv(25);		// 25 speed	y
	my.skill43 = floatv(150);		//	25 ripple - waves
	my.skill44 = floatv(100);		// 100 scale
	
	//-------------------------------------------------	
//	my.skill41 = floatd(2,10000);		// 2
//	my.skill42 = floatd(2,10000);		// 2
//	my.skill43 = floatv(0.2*0.1);		// 0.2
//	my.skill44 = floatv(0.5);			// 0.5
//	my.red 	= 160;
//	my.green = 170;						// give a colour tint r110,g105,b100 - it could have a color panel in sky class to adjust colour!!!
//	my.blue 	= 180;
//	fx_water_mirror();					// old mirror water - too flat

#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// helper functions

function PosInfo3D(tempx, tempy)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(tempx, tempy, -1000) );
	vec_set( pos2 , vector(tempx, tempy, 3000) );       		
	
	c_trace (pos2, pos1, IGNORE_ME | IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS | IGNORE_PASSABLE );  
	
	if (HIT_TARGET) 
	{
		return(hit.z);		// measured Z
	}
	else
	{
		return(-128);			// default Z
	}
}

// c-trace for getting surface info at x;y ignring FLAG2 objects, like trees and obstacles
function PosInfo3D2(tempx, tempy)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(tempx, tempy, -1000) );
	vec_set( pos2 , vector(tempx, tempy, 3000) ); // should be a big value       		
	
	c_trace (pos2, pos1, IGNORE_ME | IGNORE_SPRITES | IGNORE_FLAG2 | IGNORE_PASSABLE);    
	
	if (HIT_TARGET) 
	{
		return(hit.z);		// measured Z
	}
	else
	{
		return(-128);			// default Z
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

action Obstacle_CreateItem()
{			
	my.z = PosInfo3D(my.x, my.y) - 8 ;
	
	set(my, SHADOW | FLAG2);
	if (shadow_stencil<(var)8)
		{
			my.shadow = ent_unitshadow_rot; 	
		}

	wait(1);									// needed after scaling and before update collision size	
	set(me, POLYGON);
	my.material 	= mtl_obstacle;			

	wait(1);
	my.emask &= ~DYNAMIC;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


action Obstacle_CreateTree()
{			
	my.z -= 8 ;
	
	set(my, SHADOW | OVERLAY | CAST | FLAG2);
	if (shadow_stencil<(var)8)
		{
			my.shadow = ent_unitshadow_rot; 	
		}
	my.pan = random(360);		
	
	wait(1);										// needed after scaling and before update collision size	
	set(me, POLYGON );						// overlay for water, cast for non transparent shadows
	
	my.skill41 = floatv(10);				// speed x
	my.skill42 = floatv(25);				// speed	y
	my.skill43 = floatv(35);				//	oscillation	
	my.skill44 = floatv(my.max_z/2);		// bending position
	my.material 	= mtl_tree;			
	
	wait(1);
	my.emask &= ~DYNAMIC;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void		main()
{
	//----------------------------------------------------------------------
	// general settings
	video_set(1366,768,0,1);	// 16:9
//	video_set(1024,768,0,1);	// 4:3
//	fps_max = 40; 					// limit the frame rate
	fps_min = 16; 	
//	video_mode = 8; 				// run in 1024x768 pixels
//	video_depth = 32; 			// start the engine in 32 bit mode
//	video_screen = 1; 			// start in full screen mode
	preload_mode = 3+4; 			// preload level entities, created and cloned entities, and precalculate their environment light
   tex_share = 1; 				// same entities share texture memory	
	d3d_triplebuffer = 1; 		// activate Triple Buffering - more memory needed but higher frame rate achievable	
	d3d_entsort = 2;				// 2: sort transparent entities based on XY distance from camera instead of XYZ distance - or 6 for all the enities
	d3d_alpharef = 100;			// overlay transparency treshold level 0..255, default=127
//	if (d3d_caps & 2) 			// good cards only 
		{
			d3d_anisotropy = 2; 	// 1..7 anisotropic filtering active - needed by anisotropic mipmapping
			d3d_mipmapping = 4;	// anisotropic mipmapping - much better terrain texture
		} 
	
	//---------------------------------------------------------------------
	// shadow mode
	
	shadow_stencil = 8;			// 1 or 8
	
	//---------------------------------------------------------------------
	// terrain settings
	
	terrain_chunk 	= 32;			
	
	//----------------------------------------------------------------------
	// wmb level - removed
	
//	detail_size = -50;
//	level_load("dry_level.wmb");	
//	wait(1);
//	ent_createlayer("scube02b_clearnoon+6.dds", SKY | CUBE | SHOW, 1);
//	wait(1);
	
	//----------------------------------------------------------------------
	// created level
	
	level_load(NULL);

	//----------------------------------------------------------------------
	// start Shade-C
#ifdef USE_SHADE_C

#ifdef SC_DOF	
	sc_bDOF = 0;			//*** disable Dof - overwrite default value 
								//*** if disabled HDR should be set to avoid black screen - afterwards HDR can be reset
#endif
	sc_setup();				//*** start Shade-C
#endif	
	//----------------------------------------------------------------------
	// PSSM shadow settings
	
	if (shadow_stencil==(var)8)
		{			
			pssm_transparency = 0.30;			// 0.5 -> 0.3
			pssm_res = 2048;						// (4096 slow)-2048 nice-1024 faster-(512 ugly)
			pssm_splitweight = 0.75;			// 0.5 -> 0.95				
			pssm_fbias = (float)0.0003;		// 0.0005-0.0001 level size dependent
			pssm_numsplits = 4;
//			pssm_limit = 3;						// only in custom MBshadows.c
			wait(3);
			pssm_run(4);
		}
	
	//---------------------------------------
	// sky
	
//	sky_cube_level = 																			// for fx_envWater, set current sky as level sky instead of default one, when no wmb level used
	ent_createlayer("scube02b_clearnoon+6.dds", SKY | CUBE | SHOW, 1);
//	ent_createlayer("skycube+6.dds", SKY | CUBE | SHOW, 1);						// for fx_envWater, better if sky cube contains ground and sun
		
	//---------------------------------------
	// select a ground terrain
	
	detail_size   		= -50;			
//	terrain_entity 	= ent_create( TerrainFileName1 , vector(0,0,-200), Map_Terrain_Action);
	terrain_entity 	= ent_create( TerrainFileName3 , vector(0,0,-450), Map_Terrain_Action);
//	terrain_entity 	= ent_create( TerrainFileName4 , vector(0,0,-450), Map_Terrain_Action);
	while(!terrain_entity) { wait(1); }
	wait(3);				// because of water scaling !!!
	
	//---------------------------------------
	// water terrain

//	detail_size 		= -25;		// does not matter if shader used
	water_entity 		= ent_create( WaterFileName , nullvector, Map_Water_Action);						
	while(!water_entity) { wait(1); }
	wait(3);
	
	//----------------------------------------------------------------------
	// set HDR - needed if Shade-C Depth used but Dof disabled to avoid black screen
	
	mtl_hdr.skill1 = floatv(5);	// strength
	mtl_hdr.skill2 = floatv(35);	// treshold
	mtl_hdr.skill3 = floatv(50); 	// exposure compensation factor
	mtl_hdr.skill4 = floatv(1);	// time
	pp_set(camera,mtl_hdr);
	wait(3);								
	
	//----------------------------------------------------------------------
//	// reset HDR - needed if Shade-C Depth used but Dof disabled to avoid black screen
//	
//	pp_set(camera,NULL);
//	wait(3);		
//	
	//----------------------------------------------------------------------
	// sun, sky, camera, fog, lod
	
	// camera
	camera.clip_far = 15000;
	camera.clip_near = 32;
	vec_set(camera.x,vector(0,0,1000));
	vec_set(camera.pan,vector(240,-25,0));
	camera.ambient = 25;
	// sun color, direction, height
   vec_set(sun_color,vector(225,235,245));
   sun_angle.pan = 315;
   sun_angle.tilt = 50;
   // sky color	   
	vec_set(sky_color,vector(220,210,200));		// light: 255,200,175 , darker: 225,175,125	- set by shade-c to 1,0,0
	sky_curve = 1.5;										// 1.5...2 is okay
   sky_clip = 17.5;										// min.15, but 20 is okay always
   sky_blend = 1;											// fade out around bottom edge of sky
	// fog
	vec_set(d3d_fogcolor2,vector(220,210,200));	//*** was incompatible with shade-c depth
	fog_color = 2; 
	camera.fog_start 	= camera.clip_far*0.10; 	// 1000
	camera.fog_end 	= camera.clip_far*0.67;	
	// lod distances in quants
	var 		map_lod1 					= 500;				
	var 		map_lod2 					= 1000;
	var 		map_lod3 					= 4000;
   // lod settings - use constant distances, but should be given relatively to cameracip far, in percentage, which can be different
   var temp1 = map_lod1*100;
   var temp2 = map_lod2*100;
   var temp3 = map_lod3*100;
   // lod
   vec_set(d3d_lodfactor,vector( temp1/camera.clip_far , temp2/camera.clip_far , temp3/camera.clip_far )); 		// 10,20,50. % values of view.clip_far, default: 12.5 , 25 , 50
   
	//------------------------------------------------------------------------	
	// place entities
	
	ent_create( "TOWER1.WMB" , vector(-1488,-1456,428) , Obstacle_CreateItem );
	ent_create( "TOWER2.WMB" , vector(-304,-1424,395) , Obstacle_CreateItem );
	ent_create( "CSTLEPOR.WMB" , vector(-1296,-1442,439) , Obstacle_CreateItem );
	ent_create( "PALISADE.WMB" , vector(-976,-1421,405) , Obstacle_CreateItem );
	ent_create( "PALISADE.WMB" , vector(-721,-1421,405) , Obstacle_CreateItem );
	ent_create( "PALISADE.WMB" , vector(-464,-1421,405) , Obstacle_CreateItem );
	ent_create( "PORTCULL.WMB" , vector(-1296,-1424,475) , Obstacle_CreateItem );
	ent_create( "STEPS09.WMB" , vector(-976,-1264,422) , Obstacle_CreateItem );
	ent_create( "STEPS09.WMB" , vector(-976,-1340,422) , Obstacle_CreateItem );
	
	// trees	
	int i;
	random_seed(100);
	for (i=0;i<1000;i++)
		{
			var posx;
			var posy;
			var posz;
			do 
				{
					posx = random(terrain_entity.max_x*2)-terrain_entity.max_x;
					posy = random(terrain_entity.max_y*2)-terrain_entity.max_y;
					posz = PosInfo3D2(posx,posy);
				}
			while(posz<=(var)(-128));
			
			ent_create( "mb_oak1_0.MDL" , vector(posx,posy,posz) , Obstacle_CreateTree );	
		}
	//------------------------------------------------------------------------	
	// mouse and camera control 
	
	mouse_map = mousearrow;
	mouse_mode = 1;
	
	Interface_Init();				// creates a camera object to be followed, and a red cube showing tile position on ground	
		
	//--------------------------------------------------------------------------
	// ui loop
	var screencounter = 0;
	
	while(1)	
		{
			draw_text("toggle character on/off: T",10,125,COLOR_WHITE);
			draw_text("move character: W-A-S-D",10,150,COLOR_WHITE);
			
			draw_text("move camera: arrow keys or mouse to screen edge",10,200,COLOR_WHITE);
			draw_text("camera height: Home/End or mouse wheel",10,225,COLOR_WHITE);
									
			draw_text("screenshot: K",10,275,COLOR_WHITE);	
			
			// toggle player character--------------------------------
			if (key_t)
				{
					while (key_t) {wait (1);}
					PlayerTest01_Init();						
				}
			
			// make screenshot any time-------------------------------
			if (key_k)
				{
					while (key_k) {wait (1);}
//					str_cpy(save_dir,"screenshots");
					file_for_screen( "shot.jpg" , screencounter ); 	
					screencounter++;
				}
			
			vec_set(mouse_pos,mouse_cursor);	
					
			wait(1);
		}	
	//------------------------------------------------------------------------		
}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////