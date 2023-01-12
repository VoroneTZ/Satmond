/*******************************************************************************************************************************************************
ALL EFFECTS EXAMPLE

(work in progress)

This example shows how to use all effects together
*********************************************************************************************************************************************************/

#define PRAGMA_PATH "shade-c"
#define PRAGMA_PATH "stuff"

#include <litec.h>
#include <d3d9.h> // <--- THIS IS IMPORTANT!
#include <acknex.h>
#include <default.c>
#include "sc_core.c"

BMAP* map_envMap = "sky04S+6.tga"; //an environment map for later use in a shader

ENTITY* skycube =
{
  type = "sky04+6.tga"; // the image for a backdrop scene
  flags2 = SKY | CUBE | VISIBLE; // sky cube, visible
  red = 140;
  green = 140;
  blue = 140;
}

void ent_camera()
{
	set(my,INVISIBLE);
	set(my,PASSABLE);
	while(1)
	{
		c_move(my,vector(key_force.y*10*time_step,-key_force.x*10*time_step,0),nullvector,GLIDE + IGNORE_PASSABLE);
		my.pan += -mickey.x;
		my.tilt += -mickey.y;
		
		vec_set(camera.x,my.x);
		vec_set(camera.pan,my.pan);
		wait(1);
	}
}

void ent_heatFlare()
{
	
   my.material = sc_mtl_obj_heatHaze;
   sc_skill(my, sc_myTexMov, 3);
      sc_skill(my, sc_myDepth, 2); //clip from depthmap, as it isn't really needed there and looks ugly with god rays
	vec_scale(my.scale_x,0.25);
	
   VECTOR tempPos;

   while(1)
   {
      //behave like a sprite -> face camera
      vec_set(tempPos,camera.x); 
      vec_sub(tempPos,my.x);
      vec_to_angle(my.pan,tempPos); 
      wait(1);
   }
}


void ent_volPart()
{
   sc_volPart(my); //model is a volumetric particle
   sc_skill(my, sc_myTexMov, 2.3); //animate/shift normalmap by factor 0.3
   vec_scale(my.scale_x,0.15);
} 

//A custom shader, that was built out of sc_obj_aio.fx. See Shade-C Manual for more information (Customizing Shade-C -> "Click together" Shaders)
MATERIAL* mtl_advObject =
{
	// set standardt material settings. These will be used by the shader
	diffuse_red = 200;
	diffuse_green = 200;
	diffuse_blue = 200;
	specular_blue = 180;
	specular_green = 180;
	specular_red = 180;
	power = 10;
	
	skin1 = map_envMap; //Set the environment map, as we are using Environment Mapping
   effect = "myShader.fx"; //This is a custom shader. You can easily create your own out of sc_obj_aio.fx in the shade-c/fx folder
   event = sc_mtlEvent_aio; //As we created a custom shader out of sc_obj_aio.fx, it is a good idea to set sc_mtlEvent_aio as our event, to set up the shader
	
}

void main ()
{
	video_switch(9,32,0);
	camera.arc = 90;
	level_load("level.wmb");
	
	wait(3);
	
	//setup shade-c. Activate everything as we want to use everything!
	sc_bHDR = 1;
	sc_bDOF = 1;
	sc_bRefract = 1;
	sc_bWater = 0; //we don't use water yet
	sc_bReflect = 0; //we don't use water yet, so refelction is not needed
	sc_bVolParts = 1;
	sc_setup();
	
	//we want the Depth of Field Effect to autofocus objects, so activate the autofocus
	sc_dofDynFocus(400,50000,1);
	//we want dynamic softshadows, so activate them
	sc_smSunSetup(screen_size.x, 2000, 1500, 0.002, 0);
	//setup sky
	sc_skill(skycube,sc_myDepth, 0.95); //sky is 5% affected by DoF. If you are using DoF, you should set this (not needed, but this way the sky remains sharp)
	//set skycube as non volumetric particle, this is needed. ALWAYS! (well, not always, but most of the time. It doesn't hurt to set this)
	sc_skill(skycube,sc_myVolPart, 0);
	
	//set parameters (can be changed at any time, as they will be updated in real time)
	sc_dofParams[0] = 400; //set DoF focal length
	sc_dofParams[1] = 200; //set DoF focal pos (this will be overwritten by sc_dofDynFocus() each frame)
	sc_hdrParams[2] = 82; //set bloom strength	
	sc_lightRayStr = 0.6; //set light ray strength
	sc_lightRayLength = 3.5; //set light ray length
	
	//activate light rays
	sc_lightRays();
	
	//set mat_model to shadow material. Now all models without any material will receive shadows
	mat_model.effect = "sc_obj_doShadow.fx";
	//set mat_flat to shadow material. Now all level blocks with flat shading and without any material will receive shadows
	mat_flat.effect = "sc_level_doShadow.fx";
	mat_shaded.effect = "sc_level_doShadow.fx";
	
	
	
	
	
	
	//create camera
	ent_create("xyz.mdl",vector(0,0,20),ent_camera);
	
	//fill level with objects
	you = ent_create("soldiera.mdl",vector(-70,10,10),NULL);
	you.material = mtl_advObject;
	you = ent_create("soldiera.mdl",vector(-70,-30,10),NULL);
	you.material = mtl_advObject;
	you = ent_create("soldiera.mdl",vector(-70,-70,10),NULL);
	you.material = mtl_advObject;
	ent_create("fire.mdl",vector(140,0,90),ent_heatFlare);
	ent_create("nebula.mdl",vector(-30,32,200),ent_volPart);
		
	//make the sun panel dummy visible, just so you know where the sun is ;)
	set(sc_pan_sunDummy,SHOW);
	
	bmap_to_cubemap(map_envMap);
}